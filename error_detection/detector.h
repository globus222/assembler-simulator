#ifndef ASSEMBLER_SIMULATOR_DETECTOR_H
#define ASSEMBLER_SIMULATOR_DETECTOR_H

/*
 * Activates all the error detection functions, that check if the
 * given program is valid and prints the errors to the screen with
 * the row of the command that the error was found in. Returns 0
 * if no error was found, and 1 otherwise.
 *
 * Parameters:
 * -----------
 * char *file_path  the path to the file that contains the program.
 *
 * Return Values:
 * --------------
 * 0    test passed.
 * 1    test failed.
 */
int detect(char *file_path);

/*
 * Prints the error message and the row that it occurred
 * in the program.
 *
 * Parameters:
 * -----------
 * char *error_msg  the error message to print.
 * int error_row    the row of the error in the program.
 */
void print_error(char *error_msg, int error_row);

/*
 * Returns 0 if the given command is an existing command with a similar
 * name to one of the commands in the assembly language, and otherwise,
 * returns 1.
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
int undefined_command(char *command_content);

/*
 * Returns 0 if the number of arguments that was given to the command, is equal to the
 * number of parameters that the operation of the command takes. If the number of arguments
 * is lower than the number of parameters that the operation takes, the function returns 1.
 * If the number of arguments that was given is greater than the number of parameters the
 * operation takes, the function returns 2.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 *
 * Return Values:
 * --------------
 * 0    test passed.
 * 1    test failed - missing arguments.
 * 2    test failed - too many arguments.
 */
int missing_arguments(char *command_content);

/*
 * Checks if the given command has a label, and if it does, the function checks
 * that all it's characters are valid. In detail, checks that the first character
 * of the label is a letter, and the following characters are letters and digits only.
 *
 * Parameters:
 * -----------
 * char *command_content        the string of the command.
 * DynamicArray *symbols_table  a DynamicArray pointer to a symbols table.
 *
 * Return Values:
 * --------------
 * 0    test passed.
 * 1    test failed.
 */
int invalid_label_characters(char *command_content);

/*
 * Checks if the given command has a text after the end of it. If the command
 * have a character which is not a space or a tab after the last argument, then
 * the function returns 1. Otherwise, returns 0.
 *
 * Parameters:
 * -----------
 * char *command_content        the string of the command.
 *
 * Return Values:
 * --------------
 * 0    test passed.
 * 1    test failed.
 */
int extraneous_text(char *command_content);

/*
 * Checks if there is no comma between the label and the name of the command
 * and that there is exactly 1 comma between any two arguments of the command.
 * If there is more than 1 comma between two arguments returns 3. If there's a comma
 * between the label and the name of the command, returns 2. If there's no comma
 * between two arguments, the function returns 1. Otherwise, returns 0.
 *
 * Parameters:
 * -----------
 * char *command_content        the string of the command.
 *
 * Return Values:
 * --------------
 * 0    test passed.
 * 1    test failed - missing comma.
 * 2    test failed - illegal comma.
 * 3    test failed - Multiple consecutive commas.
 */
int illegal_comma(char *command_content);

/*
 * Returns 0 if the given .extern/.entry declaration has a valid
 * label definition. If the label has invalid characters, returns 1.
 * If the declaration has no label, returns 2. If there are too many
 * fields in the declaration, returns 3.
 *
 * Parameters:
 * -----------
 * char *command_content        the string of the command.
 *
 * Return Values:
 * --------------
 * 0    test passed.
 * 1    test failed - invalid label characters.
 * 2    test failed - declaration with no label definition.
 * 3    test failed - extraneous text.
 */
int declaration_with_no_label(char *command_content);

/*
 * Returns 1 if the number of memory words that the given program takes,
 * is greater than the number of words in the memory. Otherwise, returns 0.
 *
 * Parameters:
 * -----------
 * char *file_path  the path to the file that contains the program.
 *
 * Return Values:
 * --------------
 * 0    test passed.
 * 1    test failed.
 */
int memory_overflow(char *file_path);

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

/*
 * Checks if the given command have valid register definitions, and if one
 * or more of its operands are registers, it returns 0 if all the registers operands
 * are existing registers. Otherwise, returns 1.
 *
 * Parameters:
 * -----------
 * char *command_content        the string of the command.
 *
 * Return Values:
 * --------------
 * 0    test passed.
 * 1    test failed.
 */
int undefined_register_name(char *command_content);

#endif
