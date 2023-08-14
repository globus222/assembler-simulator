#include <stdlib.h>
#include <string.h>
#include "helpers.h"
#include "command_analysis.h"
#include "../function_macros.h"
#include "../error_detection/errors.h"
#include "../error_detection/detector.h"
#include "../segments.h"

/*
 * Returns a pointer to a DynamicArray that contains Label structures.
 * Each label represent a symbol in the program, and contains information
 * such as the address of the label, the type of the label, and more. The
 * function also encodes the data that is defined in the program, to the
 * data segment.
 *
 * Parameters:
 * -----------
 * char *file_path  the path to the file that contains the program.
 */
DynamicArray *get_symbols_table(char *file_path) {
    DynamicArray *symbols_table = create_dynamic_array(); /* the array that stores the labels */
    DynamicArray *program_lines = get_program_lines(file_path); /* the array that stores the lines of the program */
    DynamicArray *temp_positions_array; /* the positions array of the current command */

    char *temp_command_content; /* a temporary variable to contain the content of the current command */

    int row_index; /* the index of the current row in the program_lines array */
    int definition_code; /* the type of the definition (data, string, extern, entry, ...) */

    int found_label; /* indicates if a label has been found in the command */

    Label *temp_label; /* temporary Label pointer to store the label to add to the symbols table */
    char *label_name;

    /* reset the instructions & data counters */
    IC = 0;
    DC = 0;

    for (row_index = 0; row_index < (program_lines->length); row_index++) {
        temp_command_content = GET_STRING(program_lines, row_index);
        temp_positions_array = get_positions_array(temp_command_content);

        if (is_empty_command(temp_command_content)) {
            free_dynamic_array(temp_positions_array);
            continue;
        }
        definition_code = get_definition_type(temp_command_content);
        found_label = has_valid_label(temp_command_content);

        if (definition_code == DATA_DEFINITION_CODE ||
            definition_code == STRING_DEFINITION_CODE ||
            definition_code == COMMAND_DEFINITION_CODE) {
            if (found_label) {
                /* create the label struct to add to the symbols table */
                temp_label = malloc(sizeof(Label));
                temp_label->index = row_index;
                label_name = get_label_name(temp_command_content);
                strcpy(temp_label->name, label_name);
                free(label_name); /* we don't need to use this dynamic memory area after we stored its data in the struct */

                if (definition_code == COMMAND_DEFINITION_CODE) {
                    temp_label->address = IC;
                    temp_label->type = definition_code;
                } else {
                    temp_label->address = DC;
                    temp_label->type = DATA_DEFINITION_CODE;
                }
                /* search for similar labels in the symbols table */
                if (found_similar_label(symbols_table, temp_label, row_index)) {
                    ERROR_FLAG = 1;
                }
                    /* add the label to the symbols table if it's not in it */
                else {
                    add_element(symbols_table, temp_label);

                    if (definition_code == COMMAND_DEFINITION_CODE) {
                        IC += get_no_of_memory_words(temp_command_content);
                    } else {
                        /* encode the data to the data segment based on its type (string/data) */
                        encode_data(temp_command_content);
                    }
                }
            } else {
                /* increase the IC even if the command don't have label */
                if (definition_code == COMMAND_DEFINITION_CODE) {
                    IC += get_no_of_memory_words(temp_command_content);
                } else {
                    /* encode the data to the data segment based on its type (string/data) */
                    encode_data(temp_command_content);
                }
            }
        } else if (definition_code == EXTERN_DEFINITION_CODE) {
            /* add the label of the declaration to the symbols table */
            int label_index = get_declaration_index(temp_command_content) + 1;
            Field label_field = GET_ELEMENT(temp_positions_array, Field*, label_index);

            /* create the label struct to add to the symbols table */
            temp_label = malloc(sizeof(Label));
            temp_label->address = 0;
            temp_label->type = definition_code;
            temp_label->index = row_index;
            strcpy(temp_label->name, label_field.content);

            if (found_similar_label(symbols_table, temp_label, row_index)) {
                ERROR_FLAG = 1;
                free_dynamic_array(temp_positions_array);
                continue;
            }
            add_element(symbols_table, temp_label);
        }
        free_dynamic_array(temp_positions_array);
    }
    address_transformation(symbols_table);
    free_dynamic_array(program_lines);

    final_IC = IC;
    final_DC = DC;

    /* reset the instructions & data counters */
    IC = 0;
    DC = 0;

    return symbols_table;
}




