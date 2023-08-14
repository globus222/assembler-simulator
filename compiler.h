#ifndef ASSEMBLER_SIMULATOR_COMPILER_H
#define ASSEMBLER_SIMULATOR_COMPILER_H

/*
 * Given a path to a file that contains an Assembly program, the following function
 * checks for errors in the program and prints them. If there are no errors in the program,
 * the function creates the object, externals, entries and no macros files.
 *
 * Parameters:
 * -----------
 * char *file_path  a path to a file that contains the program.
 */
void compile(char *file_path);

#endif
