/*!
	\file 				main.c
	\author 		Žan Šadl-Ferš
	\version		1.0-stable
	\date			2021
	\copyright	MIT
	
	Contains constants, that are defined here, 
	and that need to be implemented in the main program.
*/

#include <stddef.h>

/*!
 * The default byte size of each filestreams data buffer.
 * */
extern const size_t DEFAULT_BUFFER_SIZE;
/*!
 * Determines, if a variable had its index value set.
 * */
extern const int INDEX_NOT_SELECTED;

/*!
 * A set of constants for defining the among and
 * type of output, shown on the terminal.
 * */
enum OutputLevel
{
	/*!
	 * Shows everything on the terminal.
	 * */
	SHOW_ALL,
	/*!
	 * Show only the files, that have matched data.
	 * */
	SHOW_ONLY_MATCHED
};
