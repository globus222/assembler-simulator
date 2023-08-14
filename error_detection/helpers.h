#ifndef ASSEMBLER_SIMULATOR_ERROR_DETECTION_HELPERS_H
#define ASSEMBLER_SIMULATOR_ERROR_DETECTION_HELPERS_H

#include "../types.h"
#include "../data_structures/dynamic_array.h"

/*
 * Returns the index of the given operation in the operations array,
 * and returns -1 if an operation with the given name, has not been
 * found.
 *
 * Parameters:
 * -----------
 * char *operation_name     a name of an operation.
 */
int get_operation_index(char *operation_name);

/*
 * Returns the Operation struct of the given command.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 */
Operation get_operation(char *command_content);

/*
 * Returns the index of the name of the given command, in its positions array.
 * If the name is not found in the operations table, or it's not in the first
 * fields that the name of the command should be in, then the function returns -1.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 */
int get_name_field_index(char *command_content);

/*
 * Returns the number of arguments that was given to the command.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 */
int get_no_of_arguments(char *command_content);

/*
 * Returns the number of parameters the operation of the command have.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 */
int get_no_of_parameters(char *command_content);

/*
 * Checks if the addressing code of each argument of the given command, correspond to
 * one of the addressing codes that are allowed by the operation of the command. If the
 * test passed, the function returns 0, and otherwise, returns 1.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 *
 * Return Values:
 * --------------
 * 0    test passed.
 * 1    test failed.
 */
int invalid_operand_type(char *command_content);

#endif
