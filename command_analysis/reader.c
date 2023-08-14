#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"
#include "command_analysis.h"

/*
 * Returns a pointer to a DynamicArray that is created during the
 * execution of the function, and contains strings in the 'array'
 * field, that each represent a line in the program.
 *
 * Parameters:
 * -----------
 * char *file_path  the path to file that contains the program.
 */
DynamicArray *get_program_lines(char *file_path) {
    FILE *file = fopen(file_path, "r");
    DynamicArray *commands = create_dynamic_array();
    char *single_command = malloc(sizeof(char));
    char temp_char; /* a temporary variable to store the character that was scanned */
    int length = 0; /* the current number of characters in the command */
    int last_byte;

    if (file == NULL) {
        printf("Could not open the given file!\n");
        exit(0);
    }
    while (fscanf(file, "%c", &temp_char) != EOF) {
        /* the end of the current command arrived */
        if (temp_char == '\n') {
            /* store null terminator for empty commands */
            single_command[length] = 0;
            /* store the command */
            add_element(commands, single_command);
            /* create a new command */
            single_command = malloc(sizeof(char));
            length = 0;
        } else {
            length++;
            /* increase the memory block */
            single_command = realloc(single_command, (1 + length) * sizeof(char));
            /* store the character */
            single_command[length - 1] = temp_char;
            /* store null terminator */
            single_command[length] = 0;
        }
    }
    fseek(file, -1, SEEK_END);
    last_byte = fgetc(file);

    if (last_byte == '\n') {
        single_command = calloc(2, sizeof(char));
        single_command[0] = ' ';
        single_command[1] = '\0';
    }
    /* store the last command */
    add_element(commands, single_command);
    /* make sure the file we opened will be closed */
    fclose(file);
    return commands;
}