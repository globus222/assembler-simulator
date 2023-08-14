#ifndef ASSEMBLER_SIMULATOR_COMMAND_ANALYSIS_HELPERS_H
#define ASSEMBLER_SIMULATOR_COMMAND_ANALYSIS_HELPERS_H

#include "../types.h"
#include "../segments.h"
#include "../data_structures/dynamic_array.h"


/*
 * Returns 1 if all the characters of the given field content are tabs,
 * spaces and semicolons, and otherwise returns 0.
 *
 * Parameters:
 * -----------
 * char *field_content  the content of the field.
 */
int is_empty_field(char *field_content);

/*
 * Returns 1 if all the characters of the given command content are tabs,
 * spaces and semicolons, and otherwise returns 0.
 *
 * Parameters:
 * -----------
 * char *command_content  the content of the command.
 */
int is_empty_command(char *command_content);

/*
 * Returns a pointer to a string that contains the characters of the
 * label that is defined in the beginning of the given command, if
 * the command have one. Otherwise, returns NULL. The user should
 * free the dynamic memory that was allocated to store the string
 * in the end of the usage. The function doesn't return the name of
 * the labels that are defined in .extern/.entry definitions.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 */
char *get_label_name(char *command_content);

/*
 * Returns 1 if the given command has a label definition, and 0 if
 * it doesn't have a label definition. The function doesn't check if
 * a .extern/.entry definition has a label.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 */
int has_valid_label(char *command_content);

/*
 * Returns an integer that represent the type of the
 * definition. A definition could be Data, String, Extern,
 * Entry, or a Command, and each one has a code that is
 * defined in the absolutes.h header file. If the given
 * command matches the definition, the code of the definition
 * is returned.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 */
int get_definition_type(char *command_content);

/*
 * Returns 1 if the given label was found in the given DynamicArray
 * by its name, and otherwise returns 0. In order to return 1, the
 * given label and the label in the symbols table must have the same
 * name, and the label in the table must not be an external label.
 *
 * Parameters:
 * -----------
 * DynamicArray *symbols_table  a pointer to a dynamic symbols_table.
 * Label *label         a pointer to a label structure.
 * int row_index        the index of the row that the label is defined in.
 */
int found_similar_label(DynamicArray *symbols_table, Label *label, int row_index);

/*
 * Checks if a label with the given name, exists in the given symbols table.
 * If a label with that name exists in the table, the function returns its
 * index in the symbols table. Otherwise, returns -1.
 *
 * Parameters:
 * -----------
 * char *label_name             a string that contains the label name.
 * DynamicArray *symbols_table  a pointer to a dynamic symbols_table.
 */
int get_label_index(char *label_name, DynamicArray *symbols_table);

/*
 * Returns 1 if the given string represent a data declaration.
 * Otherwise, returns 0. A data declaration could be a .data/.string
 * declarations.
 *
 * Parameters:
 * -----------
 * char *field_content  the content of a field.
 */
int is_data_declaration_field(char *field_content);

/*
 * Encodes the data of the given .data/.string declaration to the data segment.
 * If the data declaration is a .data declaration, the function encodes the integers
 * that are defined in the declaration. If the data declaration is a .string declaration,
 * the function encodes the characters of the string that is defined in the declaration
 * and encodes a null terminator in the end.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 */
void encode_data(char *command_content);

/*
 * Returns the index of the last occurrence of a double quotation mark
 * in the given command. If there is no occurrence of the double quotation
 * mark, then the function returns -1.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 */
int find_last_quotations_index(char *command_content);

/*
 * Returns the index of the declaration in the given command.
 * For example, if the given command is: LABEL2: .extern A, B, C
 * then the function returns 1, since the .extern declaration,
 * starts in index 1. If there is no .data/.string/.extern declaration
 * in the given command, the function returns -1.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 */
int get_declaration_index(char *command_content);

/*
 * Returns the addressing code of the given operand.
 *
 * Parameters:
 * -----------
 * char *operand_content    the string of the operand.
 */
int get_operand_addressing_code(char *operand_content);

/*
 * Returns the number of memory words that the given command consumes.
 * The returned value depends on the number of arguments that the command
 * takes. If the command takes 2 arguments, then 1 memory word is added only
 * if the two arguments are registers, and otherwise, 2 memory words are added.
 * If the command takes 1 argument, then 1 memory word is added. If the command
 * takes no arguments, then by default it consumes 1 memory word.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 */
int get_no_of_memory_words(char *command_content);

/*
 * Adds the loading address to each symbol address in the symbols table,
 * and adds the final IC to each data symbol, in order to do that the data
 * segment will be stored after the code segment.
 *
 * Parameters:
 * -----------
 * DynamicArray *symbols_table  a DynamicArray pointer that contain pointers to Label structures.
 */
void address_transformation(DynamicArray *symbols_table);

/*
 * Returns the A.R.E code of a label with the given name that was found in the given
 * symbols table. If the label don't exist in the table, the function returns -1.
 *
 * Parameters:
 * -----------
 * char *label_name                 the name of the label to search in the symbols table.
 * DynamicArray *symbols_table      a DynamicArray pointer.
 */
int get_operand_ARE_code(char *label_name, DynamicArray *symbols_table);

/*
 * Given a command from the program, and a symbols table of the program, the function
 * returns a Command structure that contains information about the command.
 *
 * Parameters:
 * -----------
 * char *command_content            the string of the command.
 * int row_index                    the index of the row of the command.
 * DynamicArray *symbols_table      a DynamicArray pointer.
 */
Command get_command_object(char *command_content, int row_index, DynamicArray *symbols_table);

/*
 * Creates a bit mask in a way that the 'length' first bits of the number
 * 'num', are placed in first 'length' bits of the returned integer, and
 * the returned integer is then shifted to the left 'shift' times. For example,
 * the following function call: "encode_bit_field(10, 2, 3)", will return
 * the number "10000" in binary, since the two first bits of 10 are 10,
 * and they are shifted to the left 3 times.
 *
 * Parameters:
 * -----------
 * int num      the number to create the bit mask over.
 * int length   the number of bits to use from the number.
 * int shift    the number of times to shift the integer.
 */
unsigned int encode_bit_field(int num, int length, int shift);

/*
 * Returns an Encode structure that represent the encodings of the given
 * command to memory words.
 *
 * Parameters:
 * -----------
 * char *command_content            the string of the command.
 * int row_index                    the index of the row of the command.
 * DynamicArray *symbols_table      a DynamicArray pointer.
 */
Encode encode_command(char *command_content, int row_index, DynamicArray *symbols_table);

/*
 * Checks if a label with the given name, exists in the given symbols table.
 * If a label with that name exists in the table, the function returns its
 * address in the symbols table. Otherwise, returns -1.
 *
 * Parameters:
 * -----------
 * char *label_name             a string that contains the label name.
 * DynamicArray *symbols_table  a pointer to a dynamic symbols_table.
 */
int get_label_address(char *label_name, DynamicArray *symbols_table);

/*
 * Given a name of a register, such as "@r2", the function returns
 * an integer which represent the number of the register. For example,
 * the function call: "get_register_number("@r5")", returns the number 5.
 * Returns -1 if the given name is not a register name.
 *
 * Parameters:
 * -----------
 * char *register_name  a name of a register
 */
int get_register_number(char *register_name);

/*
 * Returns a pointer to a string, that contains the converted version
 * of the given integer to base 64.
 *
 * Parameters:
 * -----------
 * unsigned int num     the integer to convert to base 64.
 */
char *convert_to_base_64(unsigned int num);

/*
 * Returns the address that the command in the given index will be stored at.
 * The function counts the IC of each command, until it gets to the given command.
 * In that stage, the function calculated the IC of the command, and adds the
 * LOAD_ADDRESS of the program.
 *
 * Parameters:
 * -----------
 * int row_index                    the index of the row of the command.
 * DynamicArray *program_lines      a DynamicArray that stores the lines of the program.
 */
int get_command_address(int row_index, DynamicArray *program_lines);

/*
 * Returns 1 if the given string is an existing register, and 0 otherwise.
 *
 * Parameters:
 * -----------
 * char *register_name  the name of the register.
 */
int is_existing_register(char *register_name);

/*
 * Returns 1 if the given addressing method is in the array of addressing methods
 * that was given. Otherwise, returns 0.
 *
 * Parameters:
 * -----------
 * unsigned int addressing_method       the addressing method to search in the array.
 * unsigned int addressing_methods[]    the array to search the addressing method in.
 * unsigned int size                    the number of addressing methods in the array.
 */
int is_addressing_method(int addressing_method, int addressing_methods[], int size);

#endif
