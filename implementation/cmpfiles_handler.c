/*!
 *	Source file, implementing the functionality for comparing files data with each other,
 *	to see, which ones match or not.
 *
 *	\file				cmpfiles_handler.c
 *	\author 		Žan Šadl-Ferš
 *	\version   	1.0-stable
 *	\date			2021
 *	\copyright	MIT
 * */



#include "cmpcomb_handler.h"
#include "cmpfiles_handler.h"



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif



const char* STDIN_FILEPATH_MARK = "stdin";



/* Static functions. */

/*!
 * 	Validates the arguments, and if they are fine,
 * 	free's all allocated memory resources.
 * 
 * 	\param	filepaths	Contains the filepath strings of each filestream.
 * 	\param	among		The number of filepath string.
 * */
static void FreeFilepaths(char** filepaths, size_t among)
{
	if (filepaths == NULL) return;
	
	
	
	for (size_t at_index = 0; at_index < among; at_index += 1) 
	{
		if (filepaths[at_index] != NULL) free(filepaths[at_index]);
	}
	
	free(filepaths);
}

/*!
 * 
 * 	\param	filepaths	The filepath string buffers, which need to be freed.
 * 	\param	among		The number of filepath string.
 * */
static char** AllocateAndCopyFilepaths(char** filepaths, size_t among)
{
	if (filepaths == NULL) return NULL;
	else if (among == 0) return NULL;
	
	
	
	//	Validate the filepath strings.
	for (size_t at_index = 0; at_index < among; at_index += 1)
	{
		char* current_filepath_string = filepaths[at_index];
		
		//	Test 1: Is the filepath string allocated at all?
		if (current_filepath_string == NULL) return NULL;
		
		//	Test 2: Is the filepath string null-terminated?
		char* null_terminator_at = memchr(current_filepath_string, '\0', FILENAME_MAX);
		if (null_terminator_at == NULL) return NULL;
	}
	
	
	
	//	Allocate space for the filepath strings, then copy them over.
	char** cached_filepaths = malloc(sizeof(char**) * among);
	if (cached_filepaths == NULL) return NULL;
	
	//	Incase memory allocation fails for one or several filepath strings, so that their resources can be safely released.
	for (size_t at_index = 0; at_index < among; at_index += 1) cached_filepaths[at_index] = NULL;
	
	
	
	for (size_t at_index = 0; at_index < among; at_index += 1)
	{
		char* null_terminator_at = memchr(filepaths[at_index], '\0', FILENAME_MAX);
		size_t filepath_length = (size_t)(null_terminator_at - filepaths[at_index]);
		
		cached_filepaths[at_index] = calloc(filepath_length + 1, sizeof(char));	//	Reserve one byte as the null-terminator (is automatically provided by calloc, since a null-terminator has the character code of value 0).
		if (cached_filepaths[at_index] == NULL) goto __AllocateAndCopyFilepaths_FreeRemainingResources;

		memcpy(cached_filepaths[at_index], filepaths[at_index], filepath_length);
	}
	
	
	
	return cached_filepaths;
	
	
	
	__AllocateAndCopyFilepaths_FreeRemainingResources:
		//	If not, deallocate all remaining allocated resources, to prevent memory leaks.
		fputs("Error in AllocateAndCopyFilepaths: Couldn't allocate all resources!\n", stderr);
		FreeFilepaths(cached_filepaths, among);
	
	return NULL;
}



/*!
 * 	Validates the arguments, and if they are fine,
 * 	free's all allocated IO and memory resources.
 * 	
 * 	\param	filestreams	The open filestreams, that need to be closed.
 * 	\param	among			The number of filestreams.
 * */
static void CloseFilestreams(FILE** filestreams, size_t among)
{
	if (filestreams == NULL) return;
	
	
	
	for (size_t at_index = 0; at_index < among; at_index += 1) 
	{
		if (filestreams[at_index] == stdin)
		{
			continue;
		}
		else if (filestreams[at_index] != NULL) 
		{
			fclose(filestreams[at_index]);
		}
	}
	
	free(filestreams);
}



/*!
 * 
 * 	\param	filepaths	Contains the filepath strings of each filestream.
 * 	\param	among		The number of filepath string.
 * */
static FILE** OpenFilestreams(char** filepaths, size_t among)
{
	FILE** filestreams = malloc(sizeof(FILE**) * among);
	if (filestreams == NULL) return NULL;
	
	
	
	//	Incase IO resource allocation fails for one or several filestreams, so that their resources can be safely released.
	for (size_t at_index = 0; at_index < among; at_index += 1) filestreams[at_index] = NULL;
	
	
	
	bool is_stdin_used = false;
	for (size_t at_index = 0; at_index < among; at_index += 1)
	{
		FILE* new_filestream;
		
		if (strcmp(filepaths[at_index], STDIN_FILEPATH_MARK) == 0)
		{
			if (is_stdin_used) goto __OpenFilestreams_FreeRemainingResources;
			
			#ifdef _WIN32
			_setmode(_fileno(stdin), _O_BINARY);
			#endif
			
			new_filestream = stdin;
			
			is_stdin_used = true;
		}
		else
		{
			new_filestream = fopen(filepaths[at_index], "rb");
			if (new_filestream == NULL) goto __OpenFilestreams_FreeRemainingResources;
			bool filestream_not_configured = setvbuf(new_filestream, NULL, _IONBF, 0) != 0;
			if (filestream_not_configured) goto __OpenFilestreams_FreeRemainingResources;
		}
		
		filestreams[at_index] = new_filestream;
	}
	
	
	
	return filestreams;



	__OpenFilestreams_FreeRemainingResources:
		fputs("Error in OpenFilestreams: Couldn't allocate all resources!\n", stderr);
		CloseFilestreams(filestreams, among);
		
	return NULL;
}



/*!
 * 	Validates the arguments, and if they are fine,
 * 	free's all allocated memory resources.
 * 	
 * 	\param	compare_buffers	The byte buffers, which need to be freed.
 * 	\param	among					The number of byte buffers.
 * */
static void FreeBuffers(unsigned char** compare_buffers, size_t among)
{
	if (compare_buffers == NULL) return;
	
	
	
	for (size_t at_index = 0; at_index < among; at_index += 1) 
	{
		if (compare_buffers[at_index] != NULL) free(compare_buffers[at_index]);
	}
	
	free(compare_buffers);
}



static unsigned char** AllocateBuffers(size_t among, size_t size)
{
	unsigned char** compare_buffers = malloc(sizeof(unsigned char**) * among);
	if (compare_buffers == NULL) return NULL;
	
	
	
	//	Incase memory allocation fails for one or several buffers, that the allocated rest can safely be deallocated.
	for (size_t at_index = 0; at_index < among; at_index += 1) compare_buffers[at_index] = NULL;
	
	
	
	for (size_t at_index = 0; at_index < among; at_index += 1)
	{
		compare_buffers[at_index] = malloc(sizeof(unsigned char) * size);
		
		if (compare_buffers[at_index] == NULL) goto __AllocateBuffers_FreeRemainingResources;
	}
	
	
	
	return compare_buffers;



	__AllocateBuffers_FreeRemainingResources:
		fputs("Error in AllocateBuffers: Couldn't allocate all resources!\n", stderr);
		FreeBuffers(compare_buffers, among);
	
	return NULL;
}



/*!
 * 	Validates the arguments, and if they are fine,
 * 	free's all allocated memory resources.
 * */
static void FreeBuffersByteAmong(size_t* buffers_byte_among)
{
	if (buffers_byte_among == NULL) return;
	
	
	
	free(buffers_byte_among);
}



static size_t* AllocateBuffersByteAmong(size_t number_of_buffers)
{
	if (number_of_buffers == 0) return NULL;
	
	
	
	size_t* buffers_byte_among = calloc(number_of_buffers, sizeof(size_t));
	
	if (buffers_byte_among == NULL) 
	{
		fputs("Error in AllocateBuffersByteAmong: Couldn't allocate the needed resources!\n", stderr);
		return NULL;
	}
	
	
	
	return buffers_byte_among;
}






/* Implemented functions. */

void CmpFiles_Terminate(struct FilesToCompare* handler)
{
	if (handler != NULL)
	{
		CloseFilestreams(handler->_filestreams, handler->_number_of_filestreams);
		FreeFilepaths(handler->_filepaths, handler->_number_of_filestreams);
		FreeBuffers(handler->_compare_buffers, handler->_number_of_filestreams);
		FreeBuffersByteAmong(handler->_buffers_byte_among);
		CmpComb_Terminate(handler->_combinations_handler);
		free(handler);
	}
}



struct FilesToCompare* CmpFiles_Initialize(char** filepaths, size_t number_of_files, size_t compare_buffer_size)
{
	if (filepaths == NULL) return NULL;
	else if (number_of_files == 0) return NULL;
	else if (compare_buffer_size == 0) return NULL;
	
	
	
	struct FilesToCompare* handler = malloc(sizeof(struct FilesToCompare));
	if (handler == NULL) return NULL;
	
	
	
	handler->_filepaths = NULL;
	handler->_filestreams = NULL;
	handler->_compare_buffers = NULL;
	handler->_buffers_byte_among = NULL;
	handler->_combinations_handler = NULL;
	
	handler->_number_of_filestreams = number_of_files;
	handler->_compare_buffer_size = compare_buffer_size;
	
	
	
	handler->_filestreams = OpenFilestreams(filepaths, number_of_files);
	if (handler->_filestreams == NULL) goto __CmpFiles_Initialize_FreeRemainingResources;
	
	
	
	handler->_filepaths = AllocateAndCopyFilepaths(filepaths, number_of_files);
	if (handler->_filepaths == NULL) goto __CmpFiles_Initialize_FreeRemainingResources;
	
	
	
	handler->_compare_buffers = AllocateBuffers(number_of_files, compare_buffer_size);
	if (handler->_compare_buffers == NULL) goto __CmpFiles_Initialize_FreeRemainingResources;
	
	
	
	handler->_buffers_byte_among = AllocateBuffersByteAmong(number_of_files);
	if (handler->_buffers_byte_among == NULL) goto __CmpFiles_Initialize_FreeRemainingResources;
	
	
	
	handler->_combinations_handler = CmpComb_Initialize(number_of_files);
	if (handler->_combinations_handler == NULL) goto __CmpFiles_Initialize_FreeRemainingResources;
	
	
	
	return handler;
	
	
	
	__CmpFiles_Initialize_FreeRemainingResources:
		fputs("Error in CmpFiles_Initialize: Couldn't allocate all resources!\n", stderr);
		CmpFiles_Terminate(handler);
		
	return NULL;
}



bool CmpFiles_CompareFiles(struct FilesToCompare* handler)
{
	if (handler == NULL) return false;
	else if (handler->_number_of_filestreams < 2) return false;
	
	const size_t COMBINATIONS_AMONG = handler->_combinations_handler->_among_of_combinations;
	size_t skipped_combinations = 0, finished_combinations = 0;
	
	do
	{
		//	Read the files contents into their respective buffers.
		for (size_t at_index = 0; at_index < handler->_number_of_filestreams; at_index += 1)
		{
			//	Note:	When a filestream is no longer used, it gets closed and is replaced with NULL, so that the deed gets indicated.
			//	Note:	Creating a way to determine, when the filestream is redudant...otherwise, this will cause unnecessary IO calls, which slow down the whole process.
			handler->_buffers_byte_among[at_index] = fread(handler->_compare_buffers[at_index], sizeof(unsigned char), handler->_compare_buffer_size, handler->_filestreams[at_index]);
		}
		
		skipped_combinations = 0;
		finished_combinations = 0;
		
		for (size_t at_index = 0; at_index < COMBINATIONS_AMONG; at_index += 1)
		{
			size_t compare_index = handler->_combinations_handler->_compare_indexes[at_index];
			size_t compare_with_index = handler->_combinations_handler->_compare_with_indexes[at_index];
			


			//	If a index combination was marked as eliminated (files not the same), then skip it.
			if (handler->_combinations_handler->_match_states[at_index] == NOT_MATCHED)
			{
				skipped_combinations += 1;
				continue;
			}
			else if (ferror(handler->_filestreams[compare_index]) != 0)
			{
				handler->_combinations_handler->_match_states[at_index] = NOT_MATCHED;
				continue;
			}
			else if (ferror(handler->_filestreams[compare_with_index]) != 0)
			{
				handler->_combinations_handler->_match_states[at_index] = NOT_MATCHED;
				continue;
			}
			
			
			
			//	Validate the length state of the files, that are to be compared.
			if (feof(handler->_filestreams[compare_index]) != feof(handler->_filestreams[compare_with_index]))
			{
				handler->_combinations_handler->_match_states[at_index] = NOT_MATCHED;
				continue;
			}
			else if (handler->_buffers_byte_among[compare_index] != handler->_buffers_byte_among[compare_with_index])
			{
				handler->_combinations_handler->_match_states[at_index] = NOT_MATCHED;
				continue;
			}
			
			//	Last validation: Even if they are the same length, doesn't mean, that their binary content is the same. So, that is checked, and if its matched, that block of data is the same of the files.
			size_t bytes_to_check = handler->_buffers_byte_among[compare_index];
			if (bytes_to_check > 0)
			{
				bool are_not_same = memcmp(handler->_compare_buffers[compare_index], handler->_compare_buffers[compare_with_index], bytes_to_check) != 0;
				
				if (are_not_same)
				{
					handler->_combinations_handler->_match_states[at_index] = NOT_MATCHED;
				}
			}
			else if ((feof(handler->_filestreams[compare_index]) != 0) && (feof(handler->_filestreams[compare_with_index]) != 0))
			{
				handler->_combinations_handler->_match_states[at_index] = MATCHED;
				finished_combinations += 1;
			}
			else
			{
				handler->_combinations_handler->_match_states[at_index] = NOT_MATCHED;
			}
		}
	}
	while ((finished_combinations + skipped_combinations) != COMBINATIONS_AMONG);
	
	return skipped_combinations == 0;
}
