#ifndef ASSEMBLER_SIMULATOR_FUNCTION_MACROS_H
#define ASSEMBLER_SIMULATOR_FUNCTION_MACROS_H

/*
 * Returns a copy of an element in a specific index in the given
 * dynamic array, based on the type of the elements in the array.
 *
 * Parameters:
 * -----------
 * dynamic_array    a DynamicArray.
 * type             the type of the elements in the dynamic array.
 * index            the index of the element to return from the array.
 */
#define GET_ELEMENT(dynamic_array, type, index) (*((type)(((dynamic_array)->array)[index])))

/*
 * Returns a content element in a specific index in the given
 * dynamic array. The dynamic array must store strings only.
 *
 * Parameters:
 * -----------
 * dynamic_array    a DynamicArray.
 * index            the index of the content element to return from the array.
 */
#define GET_STRING(dynamic_array, index) ((char*)(((dynamic_array)->array)[index]))

extern int variable_1; /* a variable to solve the empty translation unit problem */

#endif