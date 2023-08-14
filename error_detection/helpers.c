#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "helpers.h"
#include "../types.h"
#include "../segments.h"
#include "../command_analysis/command_analysis.h"
#include "../function_macros.h"

/*
 * Returns the index of the given operation in the operations array,
 * and returns -1 if an operation with the given name, has not been
 * found.
 *
 * Parameters:
 * -----------
 * char *operation_name     a name of an operation.
 */
int get_operation_index(char *operation_name) {
    Operation temp_operation;
    int index;
    /* check if the name of the command is in the operations table */
    for (index = 0; index < (int)(sizeof(operations) / sizeof(operations[0])); index++) {
        temp_operation = operations[index];
        if (!strcmp(operation_name, temp_operation.name)) {
            return index;
        }
    }
    return -1;
}

/*
 * Returns the Operation struct of the given command.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 */
Operation get_operation(char *command_content) {
    DynamicArray *positions_array = get_positions_array(command_content);
    Field name_field = GET_ELEMENT(positions_array, Field*, get_name_field_index(command_content));
    free_dynamic_array(positions_array);
    return operations[get_operation_index(name_field.content)];
}

/*
 * Returns the index of the name of the given command, in its positions array.
 * If the name is not found in the operations table, or it's not in the first
 * fields that the name of the command should be in, then the function returns -1.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 */
int get_name_field_index(char *command_content) {
    DynamicArray *positions_array = get_positions_array(command_content);
    Field temp_field;

    int index;
    int max_name_index = (MAX_NAME_FIELD_INDEX < (positions_array->length) - 1) ? MAX_NAME_FIELD_INDEX :
                         (positions_array->length) - 1;

    for (index = 0; index <= max_name_index; index++) {
        temp_field = GET_ELEMENT(positions_array, Field *, index);
        /* check if the name of the command is in the operations table */
        if (get_operation_index(temp_field.content) >= 0) {
            free_dynamic_array(positions_array);
            return index;
        }
    }
    free_dynamic_array(positions_array);
    return -1;
}

/*
 * Returns the number of arguments that was given to the command.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 */
int get_no_of_arguments(char *command_content) {
    DynamicArray *positions_array = get_positions_array(command_content);
    int name_index = get_name_field_index(command_content);

    /* get the part of the command that contains the parameters only */
    char *parameters_part = command_content + GET_ELEMENT(positions_array, Field*, name_index).end + 1;
    DynamicArray *parameters_positions_array = get_positions_array(parameters_part);
    int no_of_given_parameters = (parameters_positions_array->length);

    free_dynamic_array(positions_array);
    free_dynamic_array(parameters_positions_array);
    return no_of_given_parameters;
}

/*
 * Returns the number of parameters the operation of the command have.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 */
int get_no_of_parameters(char *command_content) {
    DynamicArray *positions_array = get_positions_array(command_content);
    Field name_field = GET_ELEMENT(positions_array, Field*, get_name_field_index(command_content));

    int no_of_parameters_operation_takes = operations[get_operation_index(name_field.content)].type;
    int name_index = get_name_field_index(command_content);

    /* get the part of the command that contains the parameters only */
    char *parameters_part = command_content + GET_ELEMENT(positions_array, Field*, name_index).end + 1;
    DynamicArray *parameters_positions_array = get_positions_array(parameters_part);

    free_dynamic_array(positions_array);
    free_dynamic_array(parameters_positions_array);
    return no_of_parameters_operation_takes;
}