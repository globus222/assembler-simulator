#include <string.h>
#include "quantities.h"
#include "absolutes.h"
#include "command_analysis/command_analysis.h"
#include "error_detection/detector.h"

/*
 * Given a path to a file that contains an Assembly program, the following function
 * checks for errors in the program and prints them. If there are no errors in the program,
 * the function creates the object, externals, entries and no macros files.
 *
 * Parameters:
 * -----------
 * char *file_path  a path to a file that contains the program.
 */
void compile(char *file_path) {
    char input_file[MAX_FILE_NAME_LENGTH];
    char output_file[MAX_FILE_NAME_LENGTH];

    char no_macros_file_path[MAX_FILE_NAME_LENGTH];
    char object_file_path[MAX_FILE_NAME_LENGTH];
    char entries_file_path[MAX_FILE_NAME_LENGTH];
    char externals_file_path[MAX_FILE_NAME_LENGTH];

    int error_exists;

    /* create the path of each output file */
    strcpy(no_macros_file_path, file_path);
    strcpy(object_file_path, file_path);
    strcpy(entries_file_path, file_path);
    strcpy(externals_file_path, file_path);

    strcat(no_macros_file_path, OUTPUT_NO_MACROS_FILE_EXTENSION);
    strcat(object_file_path, OUTPUT_OBJECT_FILE_EXTENSION);
    strcat(entries_file_path, OUTPUT_ENTRIES_FILE_EXTENSION);
    strcat(externals_file_path, OUTPUT_EXTERNALS_FILE_EXTENSION);

    strcpy(input_file, file_path);
    strcat(input_file, INPUT_CODE_FILE_EXTENSION);

    strcpy(output_file, file_path);
    strcat(output_file, OUTPUT_CODE_FILE_EXTENSION);

    expand_macros(input_file, no_macros_file_path);
    /* don't create the output files if there's an error in the program */
    error_exists = detect(output_file);
    if (error_exists) {
        return;
    }
    /* create the output files */
    create_object_file(output_file, object_file_path);
    create_entries_file(output_file, entries_file_path);
    create_externals_file(output_file, externals_file_path);
}
