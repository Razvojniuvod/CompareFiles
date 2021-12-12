/*!
 *	Interface file, containing the implementation details for comparing files data with each other,
 *	to see, which ones match or not.
 *
 *	\file				cmpfiles_handler.h
 *	\author 		Žan Šadl-Ferš
 *	\version   	1.0-stable
 *	\date			2021
 *	\copyright	MIT
 * */


/*!
 * \def	CMPFILES_HANDLER__		Used to prevent including this more then one time.
 * */
#ifndef CMPFILES_HANDLER__
#define CMPFILES_HANDLER__



#include <stdio.h>
#include "cmpcomb_handler.h"



/*!
 * Which string is needed, 
 * for stdin to be used a one of the input files.
 * */
extern const char* STDIN_FILEPATH_MARK;



/*!
 *	Holds the necessary data for performing comparing of data with variable amongs of files.
 * */
struct FilesToCompare
{
	/*!
	*	The number of filestreams, that this struct is containing.
	* */
	size_t _number_of_filestreams;
	
	/*!
	* 	The number of bytes, 
	* 	that a buffer of one filestream can store.
	* */
	size_t _compare_buffer_size;
	
	/*!
	* 	Contains the filepath strings of each filestream.
	* */
	char** _filepaths;
	
	/*!
	*	Contains the open (or closed, if NULL) filestreams of the files, 
	* 	that need to be compared with each other.
	* */
	FILE** _filestreams;
	
	/*!
	* 	Contains the pointers to the buffers, 
	* 	which are used to store a block of data from each filestreams.
	* */
	unsigned char** _compare_buffers;
	
	/*!
	* 	The among of bytes each buffer has stored at the moment.
	* */
	size_t* _buffers_byte_among;
	
	/*!
	* 	The struct, that is used for handling the file comparing logic.
	* */
	struct CompareCombinations* _combinations_handler;
};



/*!
 *	\brief 	Free's the allocated resources of the struct.
 *	
 *	\param handler	The struct to free.
 */
void CmpFiles_Terminate(struct FilesToCompare* handler);

/*!
 *	\brief 	Allocated the needed resources for the struct, and initialized them.
 *
 *	\param filepaths					A array of strings, containing the filepaths.
 * 	\param number_of_files			The among of files, that are to be compared with each other.
 * 	\param compare_buffer_size	The number of bytes, that a buffer of one filestream can store.
 * 
 * 	\return	If succesfull, it returns the dynamically allocated handler for the file comparing.
 * 				In case of a logic or memory allocation error, any remaining allocated resources inside the function are freed, and NULL is returned.
 */
struct FilesToCompare* CmpFiles_Initialize(char** filepaths, size_t number_of_files, size_t compare_buffer_size);

/*!
 * 	\brief	Compares the files data contents with each other, and marks the results in the handler.
 * 
 * 	\return	If all files have matched data (byte by byte), returns true. 
 * 				In case of a invalid argument value being provided, or that one or several files byte data is not matched, false is returned instead.
 * */
bool CmpFiles_CompareFiles(struct FilesToCompare* handler);



#endif
