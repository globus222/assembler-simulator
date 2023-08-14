#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "errors.h"
#include "helpers.h"
#include "detector.h"
#include "../function_macros.h"
#include "../command_analysis/helpers.h"
#include "../command_analysis/command_analysis.h"

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
int detect(char *file_path) {
    DynamicArray *program_lines = get_program_lines(file_path);
    DynamicArray *symbols_table = NULL;

    char *temp_command_content;
    char *error_msg = NULL;

    int row_index;
    int definition_type;

    for (row_index = 0; row_index < (program_lines->length); row_index++) {
        temp_command_content = GET_STRING(program_lines, row_index);
        if (is_empty_command(temp_command_content)) {
            continue;
        }
        definition_type = get_definition_type(temp_command_content);
        /* check for errors in the command */
        if (definition_type == COMMAND_DEFINITION_CODE) {
            if (invalid_label_characters(temp_command_content)) {
                error_msg = INVALID_LABEL_CHARACTERS;
            } else if (undefined_command(temp_command_content)) {
                error_msg = UNDEFINED_COMMAND;
            } else if (illegal_comma(temp_command_content) == 1) {
                error_msg = MISSING_COMMA;
            } else if (illegal_comma(temp_command_content) == 2) {
                error_msg = ILLEGAL_COMMA;
            } else if (illegal_comma(temp_command_content) == 3) {
                error_msg = MULTIPLE_CONSECUTIVE_COMMAS;
            } else if (extraneous_text(temp_command_content)) {
                error_msg = EXTRANEOUS_TEXT;
            } else if (missing_arguments(temp_command_content) == 1) {
                error_msg = MISSING_ARGUMENTS;
            } else if (missing_arguments(temp_command_content) == 2) {
                error_msg = INVALID_NO_OF_ARGUMENTS;
            } else if (invalid_operand_type(temp_command_content)) {
                error_msg = INVALID_OPERAND_TYPE;
            } else if (undefined_register_name(temp_command_content)) {
                error_msg = UNDEFINED_REGISTER_NAME;
            }
        }
        else if (definition_type == EXTERN_DEFINITION_CODE || definition_type == ENTRY_DEFINITION_CODE) {
            if (invalid_label_characters(temp_command_content)) {
                error_msg = INVALID_LABEL_CHARACTERS;
            }
            else if (declaration_with_no_label(temp_command_content) == 1) {
                error_msg = INVALID_LABEL_CHARACTERS;
            }
            else if (declaration_with_no_label(temp_command_content) == 2) {
                error_msg = DECLARATION_WITH_NO_LABEL;
            }
            else if (declaration_with_no_label(temp_command_content) == 3) {
                error_msg = EXTRANEOUS_TEXT;
            }
        }
        else if (definition_type == UNKNOWN_ADDRESSING_CODE) {
            error_msg = UNRECOGNIZED_DECLARATION;
        }

        if (error_msg) {
            ERROR_FLAG = 1;
            print_error(error_msg, row_index + 1);
        }
        error_msg = NULL;
    }
    /* general tests */
    if (!ERROR_FLAG) {
        if (memory_overflow(file_path)) {
            ERROR_FLAG = 1;
            error_msg = MEMORY_OVERFLOW;
            print_error(error_msg, -1);
        }
    }
    if (symbols_table) {
        free_dynamic_array(symbols_table);
    }
    free_dynamic_array(program_lines);
    return (ERROR_FLAG) ? 1 : 0;
}

/*
 * Prints the error message and the row that it occurred
 * in the program.
 *
 * Parameters:
 * -----------
 * char *error_msg  the error message to print.
 * int error_row    the row of the error in the program.
 */
void print_error(char *error_msg, int error_row) {
    printf("Row: %d\t|  Error: %s\n", error_row, error_msg);
}

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
int undefined_command(char *command_content) {
    DynamicArray *positions_array = get_positions_array(command_content);
    Field temp_field;
    int max_name_index = (MAX_NAME_FIELD_INDEX < (positions_array->length) - 1) ? MAX_NAME_FIELD_INDEX :
                         (positions_array->length) - 1;
    int index;

    for (index = 0; index <= max_name_index; index++) {
        temp_field = GET_ELEMENT(positions_array, Field*, index);
        /* check if the name of the command is in the operations table */
        if (get_operation_index(temp_field.content) >= 0) {
            return 0;
        }
    }
    free_dynamic_array(positions_array);
    return 1; /* print */
}

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
int missing_arguments(char *command_content) {
    DynamicArray *positions_array = get_positions_array(command_content);
    Operation operation = get_operation(command_content);

    int no_of_arguments_should_have = operation.type;
    int no_of_arguments_have = (positions_array->length) - (get_name_field_index(command_content) + 1);

    free_dynamic_array(positions_array);
    if (no_of_arguments_have < no_of_arguments_should_have) {
        return 1;
    } else if (no_of_arguments_have > no_of_arguments_should_have) {
        return 2;
    }
    return 0;
}

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
int invalid_label_characters(char *command_content) {
    char delimiters[] = " "; /* separating characters */
    int label_end_index = -1;
    int index;

    command_content = command_content + strspn(command_content, delimiters);
    /* search for the ending character of the label */
    for (index = 0; index < (signed int)strlen(command_content); index++) {
        if (command_content[index] == LABEL_ENDING_CHARACTER) {
            label_end_index = index;
            break;
        }
    }
    /* there is no label in the command, and therefore the test passed */
    if (label_end_index < 0) {
        return 0;
    }
    /* check that all the characters of the label are letters and digits, and the first character is a letter */
    if (!isalpha(command_content[0])) {
        return 1;
    }
    for (index = 0; index < label_end_index; index++) {
        if (!isalpha(command_content[index]) && !isdigit(command_content[index])) {
            return 1;
        }
    }
    return 0;
}

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
int extraneous_text(char *command_content) {
    DynamicArray *positions_array = get_positions_array(command_content);
    Operation operation = get_operation(command_content);
    Field last_argument;

    int last_argument_index = 1 + operation.type; /* the index of the last argument in the positions array */
    int last_character_index; /* the index of the last character of the command */
    int index;

    if (!has_valid_label(command_content)) {
        last_argument_index -= 1;
    }
    if (last_argument_index >= (positions_array->length)) {
        return 1;
    }
    last_argument = GET_ELEMENT(positions_array, Field*, last_argument_index);
    last_character_index = last_argument.end;
    index = last_character_index;
    while (index >= last_argument.start + 1) {
        if (!isalpha(command_content[index]) && !isdigit(command_content[index])) {
            return 1;
        }
        --index;
    }
    ++last_character_index;
    while (last_character_index < (signed int)strlen(command_content)) {
        if (command_content[last_character_index] != ' ' && command_content[last_character_index] != '\t') {
            free_dynamic_array(positions_array);
            return 1;
        }
        ++last_character_index;
    }
    return 0;
}

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
int illegal_comma(char *command_content) {
    DynamicArray *positions_array = get_positions_array(command_content);
    Field current_label;
    Field next_label;

    int command_name_index = get_name_field_index(command_content);
    int first_argument_index = command_name_index + 1; /* the index of the first argument in the positions array */
    int no_of_commas = 0; /* the number of commas between two arguments of the command */
    int start, end;
    int index;

    /* check if there are commas between the label and the name of the command */
    if (has_valid_label(command_content)) {
        start = GET_ELEMENT(positions_array, Field*, 0).end + 1;
        end = GET_ELEMENT(positions_array, Field*, 1).start;

        for (index = start; index < end; index++) {
            if (command_content[index] == ',') {
                free_dynamic_array(positions_array);
                return 2;
            }
        }
    }
    /* check if there are commas between the name of the command and the first argument */
    start = command_name_index;
    end = first_argument_index;

    if (end < (positions_array->length)) {
        current_label = GET_ELEMENT(positions_array, Field*, start);
        next_label = GET_ELEMENT(positions_array, Field*, end);

        for (index = current_label.end + 1; index < next_label.start; index++) {
            if (command_content[index] == ',') {
                free_dynamic_array(positions_array);
                return 2;
            }
        }
    }
    /* check that there is exactly 1 comma between any two arguments of the command */
    start = first_argument_index;
    end = start + 1;
    while (end < (positions_array->length)) {
        current_label = GET_ELEMENT(positions_array, Field*, start);
        next_label = GET_ELEMENT(positions_array, Field*, end);

        for (index = current_label.end + 1; index < next_label.start; index++) {
            if (command_content[index] == ',') {
                ++no_of_commas;
            }
        }
        if (no_of_commas == 0) {
            free_dynamic_array(positions_array);
            return 1;
        } else if (no_of_commas == 2) {
            free_dynamic_array(positions_array);
            return 3;
        }
        no_of_commas = 0;
        ++start;
        ++end;
    }
    free_dynamic_array(positions_array);
    return 0;
}

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
int memory_overflow(char *file_path) {
    DynamicArray *symbols_table = get_symbols_table(file_path);
    if (IC + DC > NO_OF_MEMORY_WORDS_IN_PROGRAM) {
        free_dynamic_array(symbols_table);
        return 1;
    }
    free_dynamic_array(symbols_table);
    return 0;
}

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
int invalid_operand_type(char *command_content) {
    DynamicArray *operands_array = get_operands_array(command_content, NULL);
    Operation operation = get_operation(command_content);
    Operand source_operand;
    Operand destination_operand;

    /* the number of addressing codes that the destination operand can get */
    const int dest_operand_addressing_length =
            (signed int)(sizeof(operation.destination_operand_addressing) / sizeof(operation.destination_operand_addressing[0]));
    /* the number of addressing codes that the source operand can get */
    const int src_operand_addressing_length =
            (signed int)(sizeof(operation.source_operand_addressing) / sizeof(operation.source_operand_addressing[0]));

    int found_dest_addressing_code_flag = 0;
    int found_src_addressing_code_flag = 0;
    int index;

    if ((operands_array->length) == 0) {
        free_dynamic_array(operands_array);
        return 0;
    } else if ((operands_array->length) == 1) {
        destination_operand = GET_ELEMENT(operands_array, Operand*, 0);
        /* check if the addressing code of the destination operand is one of the
         * addressing codes that the destination operand can get */
        for (index = 0; index < dest_operand_addressing_length; index++) {
            if (destination_operand.addressing == operation.destination_operand_addressing[index]) {
                free_dynamic_array(operands_array);
                return 0;
            }
        }
    } else {
        source_operand = GET_ELEMENT(operands_array, Operand*, 0);
        destination_operand = GET_ELEMENT(operands_array, Operand*, 1);

        for (index = 0; index < src_operand_addressing_length; index++) {
            if (source_operand.addressing == operation.source_operand_addressing[index]) {
                found_src_addressing_code_flag = 1;
                break;
            }
        }
        /* check if the addressing code of the destination operand is one of the
         * addressing codes that the destination operand can get */
        for (index = 0; index < dest_operand_addressing_length; index++) {
            if (destination_operand.addressing == operation.destination_operand_addressing[index]) {
                found_dest_addressing_code_flag = 1;
                break;
            }
        }
    }
    if (found_src_addressing_code_flag && found_dest_addressing_code_flag) {
        free_dynamic_array(operands_array);
        return 0;
    }
    free_dynamic_array(operands_array);
    return 1;
}

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
int undefined_register_name(char *command_content) {
    DynamicArray *operands_array = get_operands_array(command_content, NULL);
    Operation operation = get_operation(command_content);
    Operand first_operand;
    Operand second_operand;

    int no_of_addressing_1; /* the number of addressing methods that the first operand can take */
    int no_of_addressing_2; /* the number of addressing methods that the second operand can take */

    if (operation.type == 0) {
        free_dynamic_array(operands_array);
        return 0;
    } else if (operation.type == 1) {
        first_operand = GET_ELEMENT(operands_array, Operand*, 0);
        no_of_addressing_1 = (signed int)(sizeof(operation.destination_operand_addressing) / sizeof(operation.destination_operand_addressing[0]));

        if (is_addressing_method(REGISTER_ADDRESSING_CODE, operation.destination_operand_addressing, no_of_addressing_1)) {
            if (first_operand.data[0] == '@' && !is_existing_register(first_operand.data)) {
                free_dynamic_array(operands_array);
                return 1;
            }
        }

    } else if (operation.type == 2) {
        first_operand = GET_ELEMENT(operands_array, Operand*, 0);
        second_operand = GET_ELEMENT(operands_array, Operand*, 1);

        no_of_addressing_1 = (signed int)(sizeof(operation.source_operand_addressing) / sizeof(operation.source_operand_addressing[0]));
        no_of_addressing_2 = (signed int)(sizeof(operation.destination_operand_addressing) / sizeof(operation.destination_operand_addressing[0]));

        /* if the addressing method of the operand is a register addressing, and
         * the operand is not an existing register, then the test was failed. */
        if (is_addressing_method(REGISTER_ADDRESSING_CODE, operation.source_operand_addressing, no_of_addressing_1)) {
            if (first_operand.data[0] == '@' && !is_existing_register(first_operand.data)) {
                return 1;
            }
        }
        if (is_addressing_method(REGISTER_ADDRESSING_CODE, operation.destination_operand_addressing, no_of_addressing_2)) {
            if (second_operand.data[0] == '@' && !is_existing_register(second_operand.data)) {
                return 1;
            }
        }
    }
    free_dynamic_array(operands_array);
    return 0;
}

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
int declaration_with_no_label(char *command_content) {
    DynamicArray *positions_array = get_positions_array(command_content);
    Field label_field;

    const int declaration_no_of_fields_with_no_label = 2; /* the number of fields in a .extern/.entry declaration, that don't have an opening label */
    const int declaration_no_of_fields_with_label = 3; /* the number of fields in a .extern/.entry declaration, that have an opening label */

    int no_of_fields_should_have; /* the number of fields the command should have */
    int no_of_fields_have = (positions_array->length); /* the actual number of fields in the command */
    int index;

    /* check if an opening label exists */
    if (has_valid_label(command_content)) {
        no_of_fields_should_have = declaration_no_of_fields_with_label;
    }
    else {
        no_of_fields_should_have = declaration_no_of_fields_with_no_label;
    }

    if (no_of_fields_have > no_of_fields_should_have) {
        free_dynamic_array(positions_array);
        return 3;
    }
    else if (no_of_fields_have < no_of_fields_should_have) {
        free_dynamic_array(positions_array);
        return 2;
    }
    /* check if the label characters are valid */
    label_field = GET_ELEMENT(positions_array, Field*, no_of_fields_have-1);
    free_dynamic_array(positions_array);
    /* the first character of a .extern/.entry declaration label should be a letter */
    if (isalpha(command_content[label_field.start])) {
        for (index = label_field.start + 1; index <= label_field.end; index++) {
            if (!isalpha(command_content[index]) && !isdigit(command_content[index])) {
                return 1;
            }
        }
    } else {
        return 1;
    }
    return 0;
}
