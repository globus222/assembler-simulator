#ifndef ASSEMBLER_SIMULATOR_DYNAMIC_ARRAY_H
#define ASSEMBLER_SIMULATOR_DYNAMIC_ARRAY_H

/*
 * The structure DynamicArray was created to hold a list of general pointers.
 * That way, the array can store a list of Macro structures, a list of Command
 * structures, etc. The 'length' field was created, so that it won't be needed
 * to be passed as an argument to functions every time that it's being used. Any
 * DynamicArray have a null-terminator that is added automatically.
 */
typedef struct {
    void **array;
    int length;
} DynamicArray;

/*
 * Creates a new instance of the DynamicArray structure,
 * and returns a pointer to head of the array.
 */
DynamicArray *create_dynamic_array();

/*
 * Adds the given pointer element to the end of the array, and increases the
 * length of the array by 1.
 *
 * Parameters:
 * -----------
 * DynamicArray *array      a pointer to DynamicArray structure to add the element to
 * void *element            a void pointer that point to the element to add
 */
void add_element(DynamicArray *array, void *element);

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
void remove_element(DynamicArray *array, int index);

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
void remove_elements(DynamicArray *array, int start_index, int finish_index);

/* Frees the dynamic memory that was allocated to contain
 * the elements of the DynamicArray.
 *
 * Parameters:
 * -----------
 * DynamicArray *array  a pointer to a DynamicArray.
 */
void free_all_elements(DynamicArray *array);

/* Frees the dynamic memory that was allocated to contain
 * the elements of the DynamicArray, and in the end frees the array itself.
 *
 * Parameters:
 * -----------
 * DynamicArray *array  a pointer to a DynamicArray.
 */
void free_dynamic_array(DynamicArray *array);

#endif
