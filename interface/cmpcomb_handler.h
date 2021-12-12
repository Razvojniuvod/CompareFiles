/*!
 *	Interface file for creating compare combination sequences,
 *	so that multiple elements can be compared with each other.
 *
 *	\file				cmpcomb_handler.h
 *	\author 		Žan Šadl-Ferš
 *	\version   	1.0-stable
 *	\date			2021
 *	\copyright	MIT
 * */


/*!
 * \def	CMPCOMB_HANDLER__		Used to prevent including this more then one time.
 * */
#ifndef CMPCOMB_HANDLER__
#define CMPCOMB_HANDLER__



#include <stdlib.h>
#include <stdbool.h>
#include <stdbool.h>



/*!
 * 	Constants for indicating the state of the combination pairs.
 * */
enum MatchState
{
	/*!
	 * 	Whichever combination pair has this,
	 * 	had its elements not compared.
	 * */
	UNKNOWN,
	/*!
	 *	The combination pairs, that point towards towards those elements,
	 * 	have their data not matched.
	 * */
	NOT_MATCHED,
	/*!
	 *	The combination pairs, that point towards towards those elements,
	 * 	have their data fully matched.
	 * */
	MATCHED
};


/*!	
 *	Holds the necessary data for
 *	performing comparing sequences
 *  with multiple elements.
 * */
struct CompareCombinations
{
	/*!	
	 * 	Array of indexes, for which elements to compare. 
	 * */
	size_t* _compare_indexes;
	/*!	
	 * 	Array of indexes, for which elements to compare with. 
	 * */
	size_t* _compare_with_indexes;
	/*!	
	 * 	The data matching state of the combination pairs.
	 * */
	enum MatchState* _match_states;
	/*!	
	 * 	The number of combination pairs.
	 * 	The first half is in the member _compare_indexes,
	 * 	and the second half in the member _compare_with_indexes. 
	 * */
	size_t _among_of_combinations;
	/*!	
	 *	The number of elements, 
	 *	that need to be compared with each other.
	 * */
	size_t _among_of_elements;
};



/*!
 *	\brief 	Calculate and return the among of combinations, from the number of elements.
 *	
 *	\param number_of_elements	The among of elements, that are to be compared with each other.
 *	
 *	\return	The number of combinations, for that among of elements.
 */
size_t CmpComb_NumberOfCombinations(size_t number_of_elements);



/*!
 *	\brief 	Free's the allocated resources of the struct.
 *	
 *	\param handler	The struct to free.
 */
void CmpComb_Terminate(struct CompareCombinations* handler);

/*!
 *	\brief 	Allocated the needed resources for the struct, and initialized them.
 *
 *	\param number_of_elements	The among of elements, that are to be compared with each other.
 * 
 * 	\return	If succesfull, it returns the dynamically allocated handler of the compare combinations. 
 * 				In case of a logic or memory allocation error, any remaining allocated resources inside the function are freed, and NULL is returned.
 */
struct CompareCombinations* CmpComb_Initialize(const size_t number_of_elements);



/*!
 *	\brief	Obtain a combination pair from the handler.
 * 
 * 	\param	handler							Holds the necessary data for handling combinations pairs, which are used to compare elements with each other.
 * 	\param	from_position				From which index position it fetched the combination pair.
 * 	\param	compare_index				The index position of the first element to compare.
 * 	\param	compare_with_index		The index position of the second element to compare with.
 * 
 * 	\return	If succesfull, returns true. 
 * 				In case of a invalid argument value being provided, false is returned instead.
 * */
bool CmpComb_GetCombination(struct CompareCombinations* handler, const size_t from_position, size_t* compare_index, size_t* compare_with_index);

/*!
 *	\brief	Set a compare combination pair in the handler.
 * 
 * 	\param	handler							Holds the necessary data for handling combinations pairs, which are used to compare elements with each other.
 * 	\param	from_position				From which index position it sets the combination pair.
 * 	\param	compare_index				The index position of the first element to compare.
 * 	\param	compare_with_index		The index position of the second element to compare with.
 * 
 * 	\return	If succesfull, returns true. 
 * 				In case of a invalid argument value being provided, false is returned instead.
 * */
bool CmpComb_SetCombination(struct CompareCombinations* handler, const size_t from_position, size_t compare_index, size_t compare_with_index);


/*!
 *	\brief 	Assigns the compare combinations pair into a initialized struct.
 *	
 *	\param handler	Holds the necessary data for handling combinations pairs, which are used to compare elements with each other.
 *	
 * 	\return	If succesfull, returns true. 
 * 				In case of a invalid argument value being provided, or a memory allocation error happing, false is returned instead.
*/
bool CmpComb_PrepareCombinations(struct CompareCombinations* handler);



#endif
