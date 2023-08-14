#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"
#include "command_analysis.h"
#include "../function_macros.h"
#include "../segments.h"

/*
 * Creates the object file of a program that is stored in the given file.
 * In the beginning of the file, the IC and DC are written, and each following
 * line contains the encodings of the program in base 64.
 *
 * Parameters:
 * -----------
 * char *file_path      the path to the file that stores the program.
 * char *output_file    the path that the output file will be stored in.
 */
void create_object_file(char *file_path, char *output_path) {
    FILE *file;
    int index;
    char *base_64_code;

    /* re-write the file */
    file = fopen(output_path, "w");
    file = fopen(output_path, "a");

    /* encode the data */
    get_symbols_table(file_path);
    fprintf(file, "%d %d\n", final_IC, final_DC);

    /* encode the commands */
    second_iteration(file_path);

    /* write the instructions encodings to the file */
    for (index = 0; index < final_IC; index++) {
        base_64_code = convert_to_base_64(code_segment[index][0]);
        fprintf(file, "%s\n", base_64_code);
        free(base_64_code);
    }
    /* write the data encodings to the file */
    for (index = 0; index < final_DC; index++) {
        base_64_code = convert_to_base_64(data_segment[index][0]);
        fprintf(file, "%s\n", base_64_code);
        free(base_64_code);
    }
    fclose(file);
}

/*
 * Creates the entries file of a program that is stored in the given file.
 * Each line contains the name of a label that is defined in the program, and
 * the memory address it is defined in.
 *
 * Parameters:
 * -----------
 * char *file_path      the path to the file that stores the program.
 * char *output_file    the path that the output file will be stored in.
 */
void create_entries_file(char *file_path, char *output_path) {
    /* activate the second iteration in order to create the symbols table and mark the entry labels */
    FILE *file;
    Label temp_label;
    DynamicArray *symbols_table = second_iteration(file_path);

    int index;
    int no_of_entries = 0;

    for (index = 0; index < (symbols_table->length); index++) {
        temp_label = GET_ELEMENT(symbols_table, Label*, index);
        if (temp_label.type == ENTRY_DEFINITION_CODE) {
            no_of_entries += 1;
            /* create the entries file only if there's at least one entry label */
            if (no_of_entries == 1) {
                /* re-write the file */
                file = fopen(output_path, "w");
                file = fopen(output_path, "a");
            }
            fprintf(file, "%s %d\n", temp_label.name, temp_label.address);
        }
    }
    free_dynamic_array(symbols_table);
    fclose(file);
}

/*
 * Creates the externals file of a program that is stored in the given file.
 * Each line contains the name of a label that is defined in the program as
 * external, and the memory address it is used in.
 *
 * Parameters:
 * -----------
 * char *file_path      the path to the file that stores the program.
 * char *output_file    the path that the output file will be stored in.
 */
void create_externals_file(char *file_path, char *output_path) {
    DynamicArray *program_lines = get_program_lines(file_path);
    DynamicArray *symbols_table = get_symbols_table(file_path);
    Command temp_command;

    Operand first_operand;
    Operand second_operand;
    Label temp_label;
    FILE *file;

    int index;
    int label_index;
    int label_address;
    char *temp_command_content;
    int no_of_extern_labels = 0;

    for (index = 0; index < (program_lines->length); index++) {
        temp_command_content = GET_STRING(program_lines, index);

        /* move to the next command if it's an empty command or a command with a different definition code than a regular command */
        if (is_empty_command(temp_command_content) ||
                get_definition_type(temp_command_content) != COMMAND_DEFINITION_CODE) {
            continue;
        }

        temp_command = get_command_object(temp_command_content, index, symbols_table);

        first_operand = temp_command.first_operand;
        second_operand = temp_command.second_operand;

        /* check if the first operand is a label */
        if (first_operand.addressing == LABEL_ADDRESSING_CODE) {
            label_index = get_label_index(first_operand.data, symbols_table);
            temp_label = GET_ELEMENT(symbols_table, Label*, label_index);
            label_address = get_command_address(index, program_lines) + 1;

            if (temp_label.type == EXTERN_DEFINITION_CODE) {
                no_of_extern_labels += 1;
                if (no_of_extern_labels == 1) {
                    /* re-write the file */
                    file = fopen(output_path, "w");
                    file = fopen(output_path, "a");
                }
                fprintf(file, "%s %d\n", temp_label.name, label_address);
                continue;
            }

        }
        /* check if the second operand is a label */
        if (second_operand.addressing == LABEL_ADDRESSING_CODE) {
            label_index = get_label_index(second_operand.data, symbols_table);
            temp_label = GET_ELEMENT(symbols_table, Label*, label_index);
            label_address = get_command_address(index, program_lines) + 2;

            if (temp_label.type == EXTERN_DEFINITION_CODE) {
                no_of_extern_labels += 1;
                if (no_of_extern_labels == 1) {
                    /* re-write the file */
                    file = fopen(output_path, "w");
                    file = fopen(output_path, "a");
                }
                fprintf(file, "%s %d\n", temp_label.name, label_address);
                continue;
            }
        }
    }
    free_dynamic_array(program_lines);
    free_dynamic_array(symbols_table);
    fclose(file);
}