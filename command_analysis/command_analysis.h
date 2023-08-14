#ifndef ASSEMBLER_SIMULATOR_COMMAND_ANALYSIS_H
#define ASSEMBLER_SIMULATOR_COMMAND_ANALYSIS_H

#include "../types.h"
#include "../quantities.h"
#include "../data_structures/dynamic_array.h"

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
DynamicArray *get_positions_array(char *command_content);

/*
 * Returns a pointer to a DynamicArray that is created during the
 * execution of the function, and contains strings in the 'array'
 * field, that each represent a line in the program.
 *
 * Parameters:
 * -----------
 * char *file_path  the path to file that contains the program.
 */
DynamicArray *get_program_lines(char *file_path);

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
DynamicArray *expand_macros(char *source_file, char *dest_file);

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
DynamicArray (*get_symbols_table(char *file_path));

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
DynamicArray *get_operands_array(char *command_content, DynamicArray *symbols_table);

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
DynamicArray *second_iteration(char *file_path);

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
void create_object_file(char *file_path, char *output_path);

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
void create_entries_file(char *file_path, char *output_path);

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
void create_externals_file(char *file_path, char *output_path);

#endif
