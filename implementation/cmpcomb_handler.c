/*!
 *	Source file, implementing the functionality for creating compare combination sequences,
 *	so that multiple elements data can be compared with each other.
 *
 *	\file				cmpcomb_handler.c
 *	\author 		Žan Šadl-Ferš
 *	\version   	1.0-stable
 *	\date			2021
 *	\copyright	MIT
 * */



#include "cmpcomb_handler.h"



/*!
 * It is calculate by substracting the value of the variable number_of_elements by 1, 
 * and accumulating the result of that into the variable number_of_combinations.
 * 
 * This is done until the variable number_of_elements is zero.
 * 
 * The logic of this function is based on the algorithm used for preparing the combination pairs, 
 * and was created to make the code more clear.
 * */
size_t CmpComb_NumberOfCombinations(size_t number_of_elements)
{
	size_t number_of_combinations = 0;
	
	while (number_of_elements > 0)
	{
		number_of_elements -= 1;
		number_of_combinations += number_of_elements;
	}
	
	return number_of_combinations;
}



/*!
 * Free's all the allocated resources inside the struct, and then the struct itself.
 * The resources freeing process is not performed, if handler is set to NULL.
 * 
 * 	\warning	After this function, you should not use the same handler any further, unless you re-initialize it afterwards!
 * 					Not doing so and re-using it after it gets terminated will result in undefined behavior!
 * */
void CmpComb_Terminate(struct CompareCombinations* handler)
{
	if (handler != NULL)
	{
		if (handler->_compare_indexes != NULL) free(handler->_compare_indexes);
		if (handler->_compare_with_indexes != NULL) free(handler->_compare_with_indexes);
		if (handler->_match_states != NULL) free(handler->_match_states);
		free(handler);
	}
}

/*!
 *	Validates the provided arguments and results of the memory allocations.
 * 	If everything went well, the fully initialized struct is returned.
 * 	Otherwise, any allocated resources inside the function are freed, and it returns NULL.
 * 	
 * 	\warning	After the handler is returned, don't try re-initialize it in the same pointer variable, unless it had its resources freed.
 * 					Doing so will result in a memory leak!
 * */
struct CompareCombinations* CmpComb_Initialize(const size_t number_of_elements)
{
	if (number_of_elements == 0) return NULL;
	
	
	
	struct CompareCombinations* handler = malloc(sizeof(struct CompareCombinations));
	if (handler == NULL) return NULL;
	
	
	
	size_t number_of_combinations = CmpComb_NumberOfCombinations(number_of_elements);
	
	handler->_among_of_elements = number_of_elements;
	handler->_among_of_combinations = number_of_combinations;
	handler->_compare_indexes = malloc(sizeof(size_t) * number_of_combinations);
	handler->_compare_with_indexes = malloc(sizeof(size_t) * number_of_combinations);
	handler->_match_states = malloc(sizeof(enum MatchState) * number_of_combinations);
	
	if (handler->_compare_indexes == NULL || handler->_compare_with_indexes == NULL || handler->_match_states == NULL)
	{
		CmpComb_Terminate(handler);
		return NULL;
	}
	
	
	
	CmpComb_PrepareCombinations(handler);
	
	
	
	return handler;
}



/*!
 *	Validates the provided arguments, 
 * 	and afterwards copies the compare combination pair from the handler struct into the values, 
 * 	pointer by compare_index and compare_with_index.
 * */
bool CmpComb_GetCombination(struct CompareCombinations* handler, const size_t from_position, size_t* compare_index, size_t* compare_with_index)
{
	if (handler == NULL) return false;
	else if (from_position >= handler->_among_of_combinations) return false;
	else if (compare_index == NULL || compare_with_index == NULL) return false;



	*compare_index = handler->_compare_indexes[from_position];
	*compare_with_index = handler->_compare_with_indexes[from_position];
	
	
	
	return true;
}

/*!
 *	Validates the provided arguments, 
 * 	and afterwards copies the compare_index and compare_with_index values as a compare combination pair into the handler struct.
 * */
bool CmpComb_SetCombination(struct CompareCombinations* handler, const size_t from_position, size_t compare_index, size_t compare_with_index)
{
	if (handler == NULL) return false;
	else if (from_position >= handler->_among_of_combinations) return false;



	handler->_compare_indexes[from_position] = compare_index;
	handler->_compare_with_indexes[from_position] = compare_with_index;
	
	
	
	return true;
}



/*!
 *	Validates the provided arguments, 
 * 	and afterwards generated the compare_index and compare_with_index values as compare combination pairs,
 * 	which get copied to the handler struct.
 * 
 * 	After this, any previous match results are reset to UNKNOWN.
 * */
bool CmpComb_PrepareCombinations(struct CompareCombinations* handler)
{
	//	Validate handler argument.
	if (handler == NULL) return false;
	else if (handler->_among_of_combinations == 0) return false;
	else if (handler->_compare_indexes == NULL || handler->_compare_with_indexes == NULL) return false;



	//	Prepare to cache the compare combinations into the struct handler.
	size_t in_position = 0, incrementer = 1;
	
	for (size_t compare_index = 0; compare_index < handler->_among_of_elements; ++compare_index)
	{
		for (size_t compare_with_index = incrementer; compare_with_index < handler->_among_of_elements; ++compare_with_index)
		{
			CmpComb_SetCombination(handler, in_position, compare_index, compare_with_index);
			in_position += 1;
		}
		
		incrementer += 1;
	}
	
	
	
	//	Reset the match states.
	for (size_t at_index = 0; at_index < handler->_among_of_combinations; at_index += 1) handler->_match_states[at_index] = UNKNOWN;
	
	
	
	return true;
}
