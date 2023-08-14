#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "helpers.h"
#include "command_analysis.h"
#include "../error_detection/helpers.h"
#include "../function_macros.h"

/*
 * Gets a content that represent a command_content in the program, then breaks
 * it into parts using 'strtok' and other functions, and in the end
 * returns a pointer to the DynamicArray that was created during the
 * function execution, and stores in it Field structures, with order
 * that correspond to the order of the fields of the command_content.
 *
 * Parameters:
 * -----------
 * char *command_content    a content that contains the command_content to tokenize.
 */
DynamicArray *get_positions_array(char *command_content) {
    DynamicArray *positions_array = create_dynamic_array(); /* the head of the dynamic array that will store the fields of the command_content */
    Field *temp_field; /* a temporary field that will store the current fields data */

    char delimiters[] = " ,"; /* separating characters */
    char *start; /* a pointer to the beginning of the current token */
    char *end; /* a pointer to the end of the current token */
    size_t token_length; /* the length of the current token */

    int start_index; /* the index in which the current token starts */
    int end_index; /* the index in which the current token ends */

    int i; /* the index of where to add the character of the name in the 'name' array */
    int j; /* the index of where to take the character of the name from the command_content string */

    /* move to the first place from the beginning where there are no spaces */
    start = command_content + strspn(command_content, delimiters);
    end = command_content + strlen(command_content) - 1;

    while (start <= end) {
        /* find the length of the token */
        token_length = strcspn(start, delimiters);

        /* calculate the starting & ending index of the token */
        start_index = (int) (start - command_content);
        end_index = (int) (start_index + token_length - 1);

        /* store the field in the positions array */
        temp_field = malloc(sizeof(Field));
        temp_field->start = start_index;
        temp_field->end = end_index;

        /* add a null-terminator */
        memset(temp_field->content, 0, sizeof(temp_field->content));

        /* add the content of the field */
        j = start_index;
        for (i = 0; i < end_index - start_index + 1; i++) {
            (temp_field->content)[i] = command_content[j];
            j++;
        }
        add_element(positions_array, temp_field);

        /* skip the delimiters characters and continue to the next token */
        start += token_length;
        start += strspn(start, delimiters);
    }
    return positions_array;
}

/*
 * Returns a pointer to a DynamicArray which contains Operand structures.
 * Each Operand structure stores information about the operand of the given
 * command, such as the addressing and the data of the operand.
 *
 * Parameters:
 * -----------
 * char *command_content            the string of the command.
 * DynamicArray *symbols_table      a DynamicArray pointer.
 */
DynamicArray *get_operands_array(char *command_content, DynamicArray *symbols_table) {
    DynamicArray *positions_array = get_positions_array(command_content);
    DynamicArray *parameters_positions_array;
    DynamicArray *operands_array;

    Operand *temp_operand;
    Field temp_field;

    int name_index = get_name_field_index(command_content);
    int index;
    char delimiters[] = " ,"; /* separating characters */
    char *parameters_part;

    /* get the part of the command that contains the parameters only */
    parameters_part = command_content + GET_ELEMENT(positions_array, Field*, name_index).end + 1;
    parameters_part = parameters_part + strspn(parameters_part, delimiters);

    parameters_positions_array = get_positions_array(parameters_part);
    operands_array = create_dynamic_array();

    /* create and add the Operand structures to the operands array */
    for (index = 0; index < (parameters_positions_array->length); index++) {
        temp_field = GET_ELEMENT(parameters_positions_array, Field*, index);
        temp_operand = malloc(sizeof(Operand));

        strcpy(temp_operand->data, temp_field.content);
        temp_operand->addressing = get_operand_addressing_code(temp_operand->data);
        temp_operand->start = temp_field.start;
        temp_operand->end = temp_field.end;

        if (symbols_table != NULL) {
            temp_operand->ARE = get_operand_ARE_code(temp_operand->data, symbols_table);
        }
        add_element(operands_array, temp_operand);
    }

    free_dynamic_array(positions_array);
    free_dynamic_array(parameters_positions_array);
    return operands_array;
}