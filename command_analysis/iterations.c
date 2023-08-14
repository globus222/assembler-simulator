#include <string.h>
#include "helpers.h"
#include "command_analysis.h"
#include "../function_macros.h"
#include "../error_detection/errors.h"
#include "../error_detection/detector.h"

/*
 * The following function is responsible to change the type of any label in
 * the given program that is defined in a .entry definition, to the type of
 * an entry definition. In addition to that, the function encodes the instructions
 * of the program to the code segment. The function returns the symbols table of
 * the program, after it had changed the type of each entry definition label.
 *
 * Parameters:
 * char *file_path      the path to the file that stores the program.
 */
DynamicArray *second_iteration(char *file_path) {
    DynamicArray *program_lines = get_program_lines(file_path);
    DynamicArray *symbols_table = get_symbols_table(file_path);
    DynamicArray *temp_positions_array;

    int row_index;
    char *temp_command_content;
    int temp_command_type;

    IC = 0;

    for (row_index = 0; row_index < (program_lines->length); row_index++) {
        temp_command_content = GET_STRING(program_lines, row_index);
        temp_positions_array = get_positions_array(temp_command_content);

        if (is_empty_command(temp_command_content)) {
            free_dynamic_array(temp_positions_array);
            continue;
        }
        temp_command_type = get_definition_type(temp_command_content);
        if (temp_command_type == DATA_DEFINITION_CODE ||
                temp_command_type == STRING_DEFINITION_CODE ||
                temp_command_type == EXTERN_DEFINITION_CODE) {
            free_dynamic_array(temp_positions_array);
            continue;
        } else if (temp_command_type == ENTRY_DEFINITION_CODE) {
            int label_index = get_declaration_index(temp_command_content) + 1;
            Field label_field = GET_ELEMENT(temp_positions_array, Field*, label_index);
            Label *temp_label;

            int found_entry_label_definition_flag = 0;
            int found_entry_and_extern_definition_flag = 0;
            int index;
            /* search the entry label in the symbols table */
            for (index = 0; index < (symbols_table->length); index++) {
                temp_label = (symbols_table->array)[index];
                /* the two labels have the same name, and one is defined as entry and the other is a command label */
                if (!strcmp(label_field.content, temp_label->name) && (temp_label->type) != EXTERN_DEFINITION_CODE) {
                    found_entry_label_definition_flag = 1;
                    /* mark the label in the table that belongs to the .entry definition */
                    temp_label->type = ENTRY_DEFINITION_CODE;
                }
                    /* the two labels have the same name, and one is defined as entry and the other as external */
                else if (!strcmp(label_field.content, temp_label->name) && (temp_label->type) &&
                        (temp_label->type) == EXTERN_DEFINITION_CODE) {
                    ERROR_FLAG = 1;
                    print_error(EXTERN_AND_ENTRY_LABEL, row_index + 1);
                    found_entry_and_extern_definition_flag = 1;
                }
            }
            if (found_entry_and_extern_definition_flag) {
                found_entry_and_extern_definition_flag = 0;
                free_dynamic_array(temp_positions_array);
                continue;
            }
            if (!found_entry_label_definition_flag) {
                ERROR_FLAG = 1;
                print_error(ENTRY_LABEL_WASNT_DEFINED, row_index + 1);
            }
            found_entry_label_definition_flag = 0;
            free_dynamic_array(temp_positions_array);
        } else if (temp_command_type == COMMAND_DEFINITION_CODE) {
            Encode encoding = encode_command(temp_command_content, row_index, symbols_table);
            int index;

            for (index = 0; index < encoding.memory_words; index++) {
                code_segment[IC][0] = encoding.encodings_array[index];
                IC += 1;
            }
        }
    }
    free_dynamic_array(program_lines);
    return symbols_table;
}