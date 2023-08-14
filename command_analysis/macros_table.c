#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"
#include "command_analysis.h"
#include "../function_macros.h"
#include "../absolutes.h"

/*
 * Creates a Macro structure for each definition of a macro in the program,
 * and adds it to a DynamicArray that is created during the program. In the
 * end, the function returns a pointer to the DynamicArray. In addition, the
 * function re-writes the content or creates a new file, expand the macros
 * calls to it, and remove the definitions and the calls of the macros.
 *
 * Parameters:
 * -----------
 * char *source_file    the source file that contains the program.
 * char *dest_file      the destination file to write the new program to.
 */
DynamicArray *expand_macros(char *source_file, char *dest_file) {
    FILE *file = fopen(dest_file, "w");
    DynamicArray *program_lines = get_program_lines(source_file);
    DynamicArray *macros_table = create_dynamic_array();
    DynamicArray *temp_positions_array; /* the positions array of the current command */
    Field field_0, field_1; /* the first two fields of the current command */
    Macro *temp_macro; /* the Macro struct to store the macro */
    Macro current_macro;

    int row_index = 0;
    int length = program_lines->length; /* the number of commands in the program */
    int starting_index, ending_index; /* the row_index where the current field starts and ends */
    char *command_content; /* a content to store the content of the current command */
    int no_of_fields; /* the number of fields in the current command */
    int call_index = 0; /* in which index the macro was called */
    int macro_found_flag = 0; /* indicates if a macro has been started its definition */
    int found_macro_in_table = 0; /* indicates if the macro that was called, was found in the macros table */

    int i, j;

    file = fopen(dest_file, "a");
    while (row_index < length) {
        command_content = GET_STRING(program_lines, row_index);
        /* create the positions array for the current command, and store it */
        temp_positions_array = get_positions_array(command_content);
        no_of_fields = (temp_positions_array->length);

        /* command is only semicolons (;), spaces and tabs */
        if (is_empty_command(command_content) && !macro_found_flag) {
            fprintf(file, "%s\n", command_content); /* add the command to the new file */

            row_index++;
            continue;
        }
        /* if the command is not empty there is at least one field in it */
        field_0 = GET_ELEMENT(temp_positions_array, Field*, 0);

        /* it may be the end of a macro definition, or a call to a macro */
        if (no_of_fields == NO_OF_FIELDS_IN_MACRO_CALL_OR_END) {
            /* it is the end of a macro definition */
            if (strcmp(field_0.content, MACRO_DEFINITION_END_NAME) == 0) {
                ending_index = row_index;
                (temp_macro->start_index) = starting_index;
                (temp_macro->finish_index) = ending_index;
                add_element(macros_table, (void *) temp_macro);
                /* reset flags and indexes */
                call_index = 0;
                macro_found_flag = 0;
                /* free the dynamic memory that the positions array took */
                free_all_elements(temp_positions_array);
                row_index++;
                continue;
            }
                /* it may be a call for a macro */
            else if (!is_empty_field(field_0.content)) {
                /* search the macro in the table and add its content to the new file */
                if ((macros_table->length)) {
                    for (i = 0; ((macros_table->array)[i]) != NULL; i++) {
                        current_macro = GET_ELEMENT(macros_table, Macro*, i);
                        /* if we found the macro in the macros table */
                        if (strcmp(current_macro.name, field_0.content) == 0) {
                            found_macro_in_table = 1;
                            for (j = current_macro.start_index + 1; j < current_macro.finish_index; j++) {
                                fprintf(file, "%s\n", GET_STRING(program_lines, j));
                            }
                            break;
                        }
                    }
                }
                if (found_macro_in_table) {
                    (temp_macro->calls)[call_index] = row_index;
                    call_index++;
                }
                found_macro_in_table = 0;
                /* free the dynamic memory that the positions array took */
                free_all_elements(temp_positions_array);
                row_index++;
                continue;
            }
        } else if (macro_found_flag) {
            /* free the dynamic memory that the positions array took */
            free_all_elements(temp_positions_array);
            row_index++;
            continue; /* do not write the content of the macro to the new file */
        }
            /* it may be the beginning of a macro definition */
        else if (no_of_fields > NO_OF_FIELDS_IN_MACRO_CALL_OR_END) {
            field_1 = GET_ELEMENT(temp_positions_array, Field*, 1);
            /* it's the beginning of a macro definition */
            if (strcmp(field_0.content, MACRO_DEFINITION_START_NAME) == 0) {
                starting_index = row_index;
                temp_macro = malloc(sizeof(Macro)); /* allocate memory for a new macro */
                memset(temp_macro->name, 0, sizeof(temp_macro->name));
                memset(temp_macro->calls, 0, sizeof(temp_macro->calls));
                strcpy(temp_macro->name, field_1.content); /* store the name of the macro */
                /* reset flags and indexes */
                call_index = 0;
                macro_found_flag = 1;

                /* free the dynamic memory that the positions array took */
                free_all_elements(temp_positions_array);
                row_index++;
                continue;
            }
        }
        /* the command is not the beginning/end of a macro definition and not a call to it,
         * it is just a regular command */
        fprintf(file, "%s\n", command_content); /* add the command to the new file */
        /* free the dynamic memory that the positions array took */
        free_all_elements(temp_positions_array);
        row_index++;
    }
    free_dynamic_array(program_lines);
    /* make sure the file we opened will be closed */
    fclose(file);
    return macros_table;
}