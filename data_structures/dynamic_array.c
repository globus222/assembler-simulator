#include <stdio.h>
#include <stdlib.h>
#include "dynamic_array.h"
#include "../function_macros.h"
#include "../types.h"

/*
 * Creates a new instance of the DynamicArray structure,
 * and returns a pointer to head of the array.
 */
DynamicArray *create_dynamic_array() {
    DynamicArray *head = malloc(sizeof(DynamicArray));
    head->array = malloc(sizeof(void **));
    head->length = 0;
    return head;
}

/*
 * Adds the given pointer element to the end of the array, and increases the
 * length of the array by 1.
 *
 * Parameters:
 * -----------
 * DynamicArray *array      a pointer to DynamicArray structure to add the element to
 * void *element            a void pointer that point to the element to add
 */
void add_element(DynamicArray *array, void *element) {
    array->length = array->length + 1;
    array->array = realloc(array->array, ((array->length) + 1) * sizeof(void *));
    (array->array)[(array->length) - 1] = element;
    (array->array)[(array->length)] = NULL; /* null terminator */
}

/*
 * Removes an element in a specific index from the given array,
 * and shifts the array so that there won't be empty memory cells
 * between two elements of the array. In addition, the length of the
 * array is decreased by 1.
 *
 * Parameters:
 * -----------
 * DynamicArray *array      the array to remove the element from
 * int index                the index of the element to remove from the array
 */
void remove_element(DynamicArray *array, int index) {
    if (index < 0 || index >= (array->length)) {
        printf("The given index (index = %d) is not in the boundaries of the array!\n", index);
        exit(0);
    }
    /* shift all the elements to the beginning */
    while (index < (array->length)) {
        (array->array)[index] = (array->array)[index + 1];
        index += 1;
    }
    /* resize the array to consume less memory */
    (array->array) = realloc(array->array, (array->length) * sizeof(void *));
    (array->length) = (array->length) - 1;
}

/*
 * Removes all the elements from the array, starting from the
 * element in the starting_index, and up to and including the
 * element in the finish_index.
 *
 * Parameters:
 * -----------
 * DynamicArray *array      the array to remove the elements from
 * int starting_index       the index to start removing elements from
 * int finish_index         the index to finish removing elements from
 */
void remove_elements(DynamicArray *array, int start_index, int finish_index) {
    int index = 1;
    if (start_index < 0 || finish_index >= (array->length) || finish_index < start_index) {
        printf("The given index values (starting_index = %d, finish_index = %d) are not in the boundaries of the array or not valid!\n",
               start_index, finish_index);
        exit(0);
    }
    while (index <= finish_index - start_index + 1) {
        remove_element(array, start_index);
        index += 1;
    }
}

/* Frees the dynamic memory that was allocated to contain
 * the elements of the DynamicArray.
 *
 * Parameters:
 * -----------
 * DynamicArray *array  a pointer to a DynamicArray.
 */
void free_all_elements(DynamicArray *array) {
    int index = 0;
    void *temp_element;
    while ((temp_element = (array->array)[index]) != NULL) {
        free(temp_element);
        (array->array)[index] = NULL;
        index++;
    }
}

/* Frees the dynamic memory that was allocated to contain
 * the elements of the DynamicArray, and in the end frees the array itself.
 *
 * Parameters:
 * -----------
 * DynamicArray *array  a pointer to a DynamicArray.
 */
void free_dynamic_array(DynamicArray *array) {
    /* the array has no elements */
    if ((array->length) == 0) {
        free(array);
    } else {
        free_all_elements(array); /* free the elements of the array before the array itself */
        free(array); /* free the array itself */
    }
}