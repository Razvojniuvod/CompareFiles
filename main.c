/*!
	\file 				main.c
	\author 		Žan Šadl-Ferš
	\version		1.0-stable
	\date			2021
	\copyright	MIT
	
	Implements main.h, and executes the programs main logic.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "cmpfiles_handler.h"
#include "main.h"


const size_t DEFAULT_BUFFER_SIZE = 16384;
const int INDEX_NOT_SELECTED = -1;

/*	Static functions, exclusive to the main.c source file.*/

static int Main_ShowMessage(const char* type, const char* short_argument, const char* long_argument, const char* message_text)
{
	/*!
	 * \brief	Shows a status message on the terminal.
	 * 
	 * The output of the message depends on the provided arguments.
	 * If the are to be ignored, set them to NULL.
	 * This will influence the message shown.
	 * 
	 * The message itself is written to stderr.
	 * 
	 * \param	type						What the message is about. If set to NULL, is not shown.
	 * \param	short_argument	The short version (like "-h") of a argument, whose state needs to be shown. If set to NULL, is ignored.
	 * \param	long_argument	The long version of a argument (like "--help"), whose state needs to be shown. If set to NULL, is ignored.
	 * \param	message_text		The status message itself. If set to NULL, is not shown.
	 * */
	
	if (ferror(stderr) != 0)
	{
		fputs(
			"Warning: A IO error occured before calling the function showError.\n"
			"The error indicator will be cleared before\n"
			"executing anything in the function, but if gets set again after this function call,\n"
			"something could have gone wrong while writing the error message to stderr.\n",
			stderr
		);
		
		fputs(
			"Warning: A IO error occured before calling the function showError.\n"
			"The error indicator will be cleared before\n"
			"executing anything in the function, but if gets set again after this function,\n"
			"something could have gone wrong while writing the error message to stderr.\n",
			stdout
		);
		
		clearerr(stderr);
	}
	
	
	
	if (type != NULL)
	{
		fprintf(stderr, "%s: ", type);
	}
	
	
	
	if ((short_argument != NULL) && (long_argument != NULL))
	{
		fputs("Argument ", stderr);
	}
	if (short_argument != NULL)
	{
		fprintf(stderr, "%s ", short_argument);
	}
	if (long_argument != NULL)
	{
		fprintf(stderr, "%s ", long_argument);
	}
	
	
	
	if (message_text != NULL)
	{
		fprintf(stderr, "%s\n", message_text);
	}
	else if ((short_argument == NULL) || (long_argument == NULL))
	{
		fputs("is not supported!\n", stderr);
	}
	else if (type != NULL)
	{
		fputs("No message was provided.\n", stderr);
	}
	
	return ferror(stderr) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}



int main(int argument_count, char **passed_arguments)
{
	/*!
	 * \brief	The main programs logic.
	 * 
	 * \param	argument_count			The number of arguments, the user provided for this program.
	 * \param	passed_arguments		A array of strings, containing the contents of the provided arguments.
	 * 
	 * 	Short version:
	 *	Fetches the needed components of the defined libraries,
	 * 	initializes them, and they are afterwards used to handle the main logic of the program.
	 * 
	 * 	Long version:
	 * 	The first step is initializing the first batch of variables, then check the number of passed-over arguments from the terminal.
	 * 	If no or to few arguments have been provided, the program warns the user about this, and exits.
	 * 
	 * 	The arguments are afterwards validated, and are used to setup the procedure of the main program.
	 * 	If the arguments happen to be invalid or improperly set, the program warns the user about that, and exits.
	 * 	
	 * 	The second-last step is initializing the handlers from the libraries, and using them to perform the files data comparing.
	 * 	The results are saved after the procedure inside the handlers. 
	 * 	
	 *	And this bring us to the last step, showing the user, what files match (and not) with which.
	 * 	This is done by checking the results, saved inside the handlers, and considering the level of detail, the user wants.
	 * */
	
	int files_start_index = INDEX_NOT_SELECTED, files_end_index = INDEX_NOT_SELECTED;
	size_t buffer_size = DEFAULT_BUFFER_SIZE;
	enum OutputLevel output_level = SHOW_ALL;
	
	if (argument_count <= 1)
	{
		puts("Compare 2 or more files byte by byte with each other, and show, which ones binary data matched or not.");
		puts("For more information about this program, provide the -h argument to the program (examples shown below), and re-run it.");
		
		putchar('\n');
		
		
		
		printf("%s -h\n", passed_arguments[0]);
		printf("%s --help\n", passed_arguments[0]);
		
		
		
		return ferror(stdout) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
	}
	
	
	
	bool argument_was_provided = false;
    for (int argument_position = 1; argument_position < argument_count; ++argument_position)
    {
		//	Check if the user wanted to see any help displayed.
        if ((strcmp(passed_arguments[argument_position], "-h") == 0) ||  (strcmp(passed_arguments[argument_position], "--help") == 0)) 
		{
			//	Give the user some pointers to what is needed.
			puts("Compare 2 or more files byte by byte with each other, and show, which ones binary data matched or not.");
			
			putchar('\n');
			
			
			
			puts("Arguments:");
			
			puts("-h --help");
			puts("\tShow this message.\n");
			
			puts("-bs --buffer-size");
			printf("\tSet the size of the buffers in bytes (by default %zu),\n"
					"\twhich will contain a chunk of data from each file for comparing.\n\n", 
						DEFAULT_BUFFER_SIZE);
				
			puts("-om --only-matching");
			puts("\tOnly shows the files, that have matched data.\n");

			puts("-cf --compare-files");
			printf("\tAny filepath entered after this (till the end of the arguments or the next console argument)\n"
					"\twill have its files data compared with each other, byte by byte.\n"
					"\tIf you want one of the files to be from stdin, enter is as \"%s\".\n\n", 
						STDIN_FILEPATH_MARK);

			putchar('\n');
			
			
			
			puts(	"Filepaths can be directly defined before any arguments are defined,"
					" or after a -cf --compare-files argument till the next differing one.\n");
			
			
			
			puts("Use examples:");
			printf("%s file1.txt file2.txt\n", passed_arguments[0]);
			printf("%s file1.txt file2.txt file3.bin -om\n", passed_arguments[0]);
			printf("%s file1.txt file2.txt file3.bin -bs 65536\n", passed_arguments[0]);
			printf("%s -bs 65536 -om -cf file1.txt file2.txt\n", passed_arguments[0]);
			printf("%s stdin file.bin -bs 65536 < file.txt\n", passed_arguments[0]);
			
			
			
			return ferror(stdout) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
        }
        
		//	Check if the user wants to set the compare buffers size.
        else if (strcmp(passed_arguments[argument_position], "-bs") == 0 || strcmp(passed_arguments[argument_position], "--buffer-size") == 0)
		{
			//	Go to the argument, containing (possibly) needed value.
			++argument_position;
			
			if (argument_position < argument_count)
			{
				//	Note: Can be extended by providing metric unit support, by using the endptr argument together with memchr, to check for the unit.
				buffer_size = strtoul(passed_arguments[argument_position], NULL, 10);
				
				if (buffer_size == 0)
				{
					Main_ShowMessage("Error", "-bs", "--buffer-size", "was provided with an invalid value (which is either zero, negative or to big)!");
					return EXIT_FAILURE;
				}
			}
			else
			{
				Main_ShowMessage("Error", "-bs", "--buffer-size", "has no defined value!");
				return EXIT_FAILURE;
			}
			
			argument_was_provided = true;
        }
        
		//	Check if the user wishes to only see the files, that have matched data.
        else if (strcmp(passed_arguments[argument_position], "-om") == 0 || strcmp(passed_arguments[argument_position], "--only-matching") == 0) 
		{
			output_level = SHOW_ONLY_MATCHED;
			argument_was_provided = true;
        }
        
		//	Check which input files the user wants to compare.
        else if (strcmp(passed_arguments[argument_position], "-cf") == 0 || strcmp(passed_arguments[argument_position], "--check-files") == 0)
		{
			//	Go to the argument, containing (possibly) the needed values.
			++argument_position;
			
			//	Validity check.
			if (files_start_index >= 0 || files_end_index >= 0)
			{
				Main_ShowMessage("Error", "-cf", "--check-files", "cannot be used, since the filenames have already been defined!");
				return EXIT_FAILURE;
			}
			
			
			
			if (argument_position < argument_count)
			{
				files_start_index = argument_position;
				files_end_index = argument_position + 1;
			}
			else
			{
				Main_ShowMessage("Error", "-cf", "--check-files", "has no defined filepaths!");
				return EXIT_FAILURE;
			}
			
			argument_was_provided = false;	//	Is not set. Otherwise, it will cause -cf to only read one filepath!
        }
        
        //	Used when defining the input filepaths. 
        else 
        {
			if (files_start_index < 0)
			{
				files_start_index = argument_position;
				files_end_index = argument_position + 1;
			}
			else if (!argument_was_provided)
			{
				files_end_index = argument_position + 1;
			}
			else
			{
				Main_ShowMessage("Error", NULL, NULL, "Cannot process any more filenames, after one or several arguments!");
				return EXIT_FAILURE;
			}
		}
	}

	
	
	/*	
	 * Since everything is set and ready, time to get its hands dirty.
	 */
	//	Only used in combination with goto.
	int return_code = EXIT_SUCCESS;	
	
	if (files_start_index == INDEX_NOT_SELECTED)
	{
		Main_ShowMessage("Error", NULL, NULL, "No filepaths were defined!");
		return EXIT_FAILURE;
	}
	else if (files_end_index == INDEX_NOT_SELECTED)
	{
		Main_ShowMessage("Error", NULL, NULL, "No filepaths were defined!");
		return EXIT_FAILURE;
	}
	
	size_t number_of_files_to_compare = files_end_index - files_start_index;
	if (number_of_files_to_compare < 2)
	{
		Main_ShowMessage("Error", NULL, NULL, "At least 2 files need to be defined (use -h --help for more information)!");
		return EXIT_FAILURE;
	}
	
	struct FilesToCompare* handler = CmpFiles_Initialize(passed_arguments + files_start_index, number_of_files_to_compare, buffer_size);
	if (handler == NULL)
	{
		Main_ShowMessage("Error", NULL, NULL, "Couldn't allocate the resources for the file comparing handler!");
		return_code = EXIT_FAILURE;
		goto __Main_FreeResources;
	}
	
	bool all_matched = CmpFiles_CompareFiles(handler);	
	
	if (all_matched)
	{
		puts("All files data content is matched, byte by byte!");
	}
	else
	{
		for (size_t index = 0; index < handler->_combinations_handler->_among_of_combinations; index += 1)
		{
			size_t compare_index = handler->_combinations_handler->_compare_indexes[index];
			size_t compare_with_index = handler->_combinations_handler->_compare_with_indexes[index];
			
			if (handler->_combinations_handler->_match_states[index] == MATCHED)
			{
				printf("%s and %s match!\n", handler->_filepaths[compare_index], handler->_filepaths[compare_with_index]);
			}
			else if (handler->_combinations_handler->_match_states[index] == NOT_MATCHED)
			{
				if (output_level == SHOW_ONLY_MATCHED) continue;
				printf("%s and %s do not match!\n", handler->_filepaths[compare_index], handler->_filepaths[compare_with_index]);
			}
			else
			{
				printf("%s and %s matching state is unknown!\n", handler->_filepaths[compare_index], handler->_filepaths[compare_with_index]);
			}
		}
		
		return_code = EXIT_FAILURE;
	}
	

	__Main_FreeResources:
		CmpFiles_Terminate(handler);

	return return_code;
}
