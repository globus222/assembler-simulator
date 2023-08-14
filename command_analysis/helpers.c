#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "helpers.h"
#include "command_analysis.h"
#include "../function_macros.h"
#include "../error_detection/helpers.h"
#include "../error_detection/detector.h"
#include "../error_detection/errors.h"

/*
 * Returns 1 if all the characters of the given field content are tabs,
 * spaces and semicolons, and otherwise returns 0.
 *
 * Parameters:
 * -----------
 * char *field_content  the content of the field.
 */
int is_empty_field(char *field_content) {
    int index;
    for (index = 0; index < (signed int)strlen(field_content); index++) {
        if (isalpha(field_content[index]))
            return 0;
    }
    return 1;
}

/*
 * Returns 1 if all the characters of the given command content are tabs,
 * spaces and semicolons, and otherwise returns 0.
 *
 * Parameters:
 * -----------
 * char *command_content  the content of the command.
 */
int is_empty_command(char *command_content) {
    int index;
    if (strcmp(command_content, "") == 0) {
        return 1;
    }
    for (index = 0; index < (signed int)strlen(command_content); index++) {
        if (isalpha(command_content[index]))
            return 0;
    }
    return 1;
}

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
int get_definition_type(char *command_content) {
    DynamicArray *positions_array = get_positions_array(command_content);
    Field field_0 = GET_ELEMENT(positions_array, Field*, 0);
    Field field_1;
    char definition_name[MAX_FIELD_LENGTH];
    int index;

    /* store the characters of the first field in the array */
    for (index = field_0.start; index <= field_0.end; index++) {
        definition_name[index - field_0.start] = command_content[index];
    }
    definition_name[index] = 0;
    /* check if the first field is the definition */
    if (!strcmp(definition_name, DATA_DEFINITION_NAME)) {
        free_dynamic_array(positions_array);
        return DATA_DEFINITION_CODE;
    } else if (!strcmp(definition_name, STRING_DEFINITION_NAME)) {
        free_dynamic_array(positions_array);
        return STRING_DEFINITION_CODE;
    } else if (!strcmp(definition_name, EXTERN_DEFINITION_NAME)) {
        free_dynamic_array(positions_array);
        return EXTERN_DEFINITION_CODE;
    } else if (!strcmp(definition_name, ENTRY_DEFINITION_NAME)) {
        free_dynamic_array(positions_array);
        return ENTRY_DEFINITION_CODE;
    }

    if ((positions_array->length) <= 1) {
        free_dynamic_array(positions_array);
        return COMMAND_DEFINITION_CODE;
    }
    field_1 = GET_ELEMENT(positions_array, Field*, 1);
    /* store the characters of the second field in the array */
    memset(definition_name, 0, sizeof(definition_name));
    for (index = field_1.start; index <= field_1.end; index++) {
        definition_name[index - field_1.start] = command_content[index];
    }
    /* check if the second field is a definition */
    if (!strcmp(definition_name, DATA_DEFINITION_NAME)) {
        free_dynamic_array(positions_array);
        return DATA_DEFINITION_CODE;
    } else if (!strcmp(definition_name, STRING_DEFINITION_NAME)) {
        free_dynamic_array(positions_array);
        return STRING_DEFINITION_CODE;
    } else if (!strcmp(definition_name, EXTERN_DEFINITION_NAME)) {
        free_dynamic_array(positions_array);
        return EXTERN_DEFINITION_CODE;
    } else if (!strcmp(definition_name, ENTRY_DEFINITION_NAME)) {
        free_dynamic_array(positions_array);
        return ENTRY_DEFINITION_CODE;
    }
    free_dynamic_array(positions_array);
    return COMMAND_DEFINITION_CODE;
}

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
char *get_label_name(char *command_content) {
    DynamicArray *positions_array;
    Field field_0;

    char delimiters[] = " "; /* separating characters */
    char *label_name;
    int index;

    variable_1 = 0;
    variable_2 = 0;

    command_content = command_content + strspn(command_content, delimiters);
    positions_array = get_positions_array(command_content);

    field_0 = GET_ELEMENT(positions_array, Field*, 0);
    label_name = malloc(sizeof(char) * (MAX_FIELD_LENGTH + 1));

    if (!has_valid_label(command_content)) {
        return NULL;
    }
    for (index = 0; index < field_0.end; index++) {
        label_name[index] = command_content[index];
    }
    /* add a null terminator */
    label_name[index] = 0;
    free_dynamic_array(positions_array);
    return label_name;


}

/*
 * Returns 1 if the given command has a label definition, and 0 if
 * it doesn't have a label definition. The function doesn't check if
 * a .extern/.entry definition has a label.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 */
int has_valid_label(char *command_content) {
    DynamicArray *positions_array = get_positions_array(command_content);
    Field field_0 = GET_ELEMENT(positions_array, Field*, 0);
    int index;

    /* check that the first character of the label is a letter
      * and that the last character of the label is ':' */
    if (!isalpha(command_content[field_0.start]) || command_content[field_0.end] != LABEL_ENDING_CHARACTER) {
        return 0;
    }
    /* check that all the inner characters of the label are letters and digits */
    for (index = field_0.start; index < field_0.end; index++) {
        if (!isalpha(command_content[index]) && !isdigit(command_content[index])) {
            return 0;
        }
    }
    free_dynamic_array(positions_array);
    return 1;
}

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
int found_similar_label(DynamicArray *symbols_table, Label *label, int row_index) {
    Label temp_label;

    int index;
    int temp_label_is_external;
    int given_label_is_external = (label->type) == EXTERN_DEFINITION_CODE;
    int have_similar_name;

    for (index = 0; index < (symbols_table->length); index++) {
        temp_label = GET_ELEMENT(symbols_table, Label*, index);
        temp_label_is_external = (temp_label.type == EXTERN_DEFINITION_CODE);
        have_similar_name = !strcmp(temp_label.name, (label->name));

        if (have_similar_name) {
            if (given_label_is_external && !temp_label_is_external) {
                print_error(INVALID_EXTERN_LABEL_DEFINITION, temp_label.index + 1);
                return 1;
            } else if (!given_label_is_external && temp_label_is_external) {
                print_error(INVALID_EXTERN_LABEL_DEFINITION, row_index + 1);
                return 1;
            } else if (!given_label_is_external && !temp_label_is_external) {
                print_error(MULTIPLE_LABEL_DEFINITIONS, index + 1);
                return 1;
            }
        }
    }
    return 0;
}

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
int get_label_index(char *label_name, DynamicArray *symbols_table) {
    Label temp_label;

    int index;
    int have_similar_name;

    for (index = 0; index < (symbols_table->length); index++) {
        temp_label = GET_ELEMENT(symbols_table, Label*, index);
        have_similar_name = !strcmp(temp_label.name, label_name);

        if (have_similar_name) {
            return index;
        }
    }
    return -1;
}

/*
 * Returns 1 if the given string represent a data declaration.
 * Otherwise, returns 0. A data declaration could be a .data/.string
 * declarations.
 *
 * Parameters:
 * -----------
 * char *field_content  the content of a field.
 */
int is_data_declaration_field(char *field_content) {
    return strcmp(field_content, DATA_DEFINITION_NAME) == 0 || strcmp(field_content, STRING_DEFINITION_NAME) == 0;
}

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
void encode_data(char *command_content) {
    DynamicArray *positions_array = get_positions_array(command_content);
    Field temp_field;

    int length = (positions_array->length); /* the number of fields in the command */
    int field_index; /* the index of the field in the positions array */
    int num; /* variable to store the data that will be added to the data segment */
    int index;

    field_index = get_declaration_index(command_content);

    /* there is no data declaration in the command */
    if (field_index == length) {
        free_dynamic_array(positions_array);
        return;
    }
    /* check if there is another field after the data declaration */
    if (field_index + 1 <= length) {
        ++field_index;
        temp_field = GET_ELEMENT(positions_array, Field*,
                                 field_index); /* move the field_index to the index of the data definition */

        /* check if the type of the data is an array of integers */
        if (get_definition_type(command_content) == DATA_DEFINITION_CODE) {
            while (field_index < length) {
                temp_field = GET_ELEMENT(positions_array, Field*, field_index);
                sscanf(temp_field.content, "%d", &num);
                data_segment[DC][0] = num;
                ++field_index; /* increment the index to scan the next integer */
                ++DC; /* increment the DC to store the next integer */
            }
        }
            /* check if the type of the data is a string */
        else if (get_definition_type(command_content) == STRING_DEFINITION_CODE) {
            int start = temp_field.start;
            int end = find_last_quotations_index(command_content);
            /* add all the characters of the string to the data segment */
            for (index = start + 1; index < end; index++) {
                data_segment[DC][0] = (int) command_content[index];
                ++DC;
            }
            /* add a null terminator */
            data_segment[DC][field_index] = 0;
            ++DC;
        }
    }
    free_dynamic_array(positions_array);
}

/*
 * Returns the index of the last occurrence of a double quotation mark
 * in the given command. If there is no occurrence of the double quotation
 * mark, then the function returns -1.
 *
 * Parameters:
 * -----------
 * char *command_content    the string of the command.
 */
int find_last_quotations_index(char *command_content) {
    int index;
    for (index = (signed int)strlen(command_content) - 1; index >= 0; index--) {
        if (command_content[index] == '"') {
            return index;
        }
    }
    return -1;
}

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
int get_declaration_index(char *command_content) {
    DynamicArray *positions_array = get_positions_array(command_content);
    char *temp_field_content;
    int index;
    /* get the field_index where the declaration starts */
    for (index = 0; index < (positions_array->length); index++) {
        temp_field_content = GET_ELEMENT(positions_array, Field*, index).content;
        if (!strcmp(temp_field_content, DATA_DEFINITION_NAME)
            || !strcmp(temp_field_content, STRING_DEFINITION_NAME)
            || !strcmp(temp_field_content, EXTERN_DEFINITION_NAME)
            || !strcmp(temp_field_content, ENTRY_DEFINITION_NAME)) {
            free_dynamic_array(positions_array);
            return index;
        }
    }
    free_dynamic_array(positions_array);
    return -1;
}

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
int get_no_of_memory_words(char *command_content) {
    DynamicArray *operands_array = get_operands_array(command_content, NULL);
    int no_of_parameters = get_no_of_parameters(command_content);
    int exists_not_register_operand_flag = 0; /* a flag that indicates if there exists an operand in the command which isn't a register */
    int added_words = 0;
    int index;

    if (no_of_parameters == 0) {
        added_words = 0;
    } else if (no_of_parameters == 1) {
        added_words = 1;
    } else {
        /* check if the two arguments are registers */
        for (index = 0; index < (operands_array->length); index++) {
            if (GET_ELEMENT(operands_array, Operand*, index).addressing != REGISTER_ADDRESSING_CODE) {
                exists_not_register_operand_flag = 1;
                break;
            }
        }
        if (exists_not_register_operand_flag) {
            added_words = 2;
        } else {
            added_words = 1;
        }
    }
    free_dynamic_array(operands_array);
    return added_words + MIN_NO_OF_WORDS_IN_COMMAND;
}

/*
 * Returns the addressing code of the given operand.
 *
 * Parameters:
 * -----------
 * char *operand_content    the string of the operand.
 */
int get_operand_addressing_code(char *operand_content) {
    int index;
    int not_a_label_flag = 0;
    int num = 0;
    /* check if the operand is a register */
    for (index = 0; index < (signed int)(sizeof(registers_names) / sizeof(registers_names[0])); index++) {
        if (!strcmp(operand_content, registers_names[index])) {
            return REGISTER_ADDRESSING_CODE;
        }
    }
    /* check if the operand is an integer */
    if (sscanf(operand_content, "%d", &num) > 0) {
        return IMMEDIATE_ADDRESSING_CODE;
    }
    /* check if the operand is a label */
    if (isalpha(operand_content[0])) {
        /* check if there's a character which is not a letter or a digit after the first one */
        for (index = 0; index < (signed int)strlen(operand_content); index++) {
            if (!isalpha(operand_content[index]) && !isdigit(operand_content[index])) {
                not_a_label_flag = 1;
                break;
            }
        }
        if (!not_a_label_flag) {
            return LABEL_ADDRESSING_CODE;
        }
    }
    return UNKNOWN_ADDRESSING_CODE;
}

/*
 * Adds the loading address to each symbol address in the symbols table,
 * and adds the final IC to each data symbol, in order to do that the data
 * segment will be stored after the code segment.
 *
 * Parameters:
 * -----------
 * DynamicArray *symbols_table  a DynamicArray pointer that contain pointers to Label structures.
 */
void address_transformation(DynamicArray *symbols_table) {
    Label *temp_label;
    int index;

    for (index = 0; index < (symbols_table->length); index++) {
        temp_label = (symbols_table->array)[index];

        if ((temp_label->type) != EXTERN_DEFINITION_CODE) {
            temp_label->address += LOAD_ADDRESS;
        }
        if ((temp_label->type) == DATA_DEFINITION_CODE) {
            temp_label->address += IC;
        }

    }
}

/*
 * Returns the A.R.E code of a label with the given name that was found in the given
 * symbols table. If the label don't exist in the table, the function returns -1.
 *
 * Parameters:
 * -----------
 * char *label_name                 the name of the label to search in the symbols table.
 * DynamicArray *symbols_table      a DynamicArray pointer.
 */
int get_operand_ARE_code(char *label_name, DynamicArray *symbols_table) {
    int label_index = get_label_index(label_name, symbols_table);
    /* the label has been found in the table */
    if (label_index >= 0) {
        Label table_label = GET_ELEMENT(symbols_table, Label*, label_index);
        if (table_label.type == EXTERN_DEFINITION_CODE) {
            return ARE_EXTERNAL_CODE;
        }
        return ARE_RELOCATABLE_CODE;
    }
    return ARE_ABSOLUTE_CODE;
}

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
Command get_command_object(char *command_content, int row_index, DynamicArray *symbols_table) {
    Command command;
    DynamicArray *operands_array = get_operands_array(command_content, symbols_table);
    Operation operation = get_operation(command_content);

    command.opcode = operation.opcode;
    command.type = operation.type;
    command.index = row_index;
    command.ARE = ARE_ABSOLUTE_CODE;
    strcpy(command.data, command_content);

    if (operation.type == COMMAND_WITH_1_PARAMETERS_CODE) {
        command.first_operand = GET_ELEMENT(operands_array, Operand*, 0);
    } else if (operation.type == COMMAND_WITH_2_PARAMETERS_CODE) {
        command.first_operand = GET_ELEMENT(operands_array, Operand*, 0);
        command.second_operand = GET_ELEMENT(operands_array, Operand*, 1);
    }
    return command;
}

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
Encode encode_command(char *command_content, int row_index, DynamicArray *symbols_table) {
    Command command = get_command_object(command_content, row_index, symbols_table);
    Encode encode = {0};

    FirstMemoryWord first_mem_word;
    NextMemoryWord second_mem_word;
    NextMemoryWord third_mem_word;

    int word_index = 0;
    int first_operand_data = 0;
    int second_operand_data = 0;

    encode.memory_words = command.type + 1;

    first_mem_word.ARE = command.ARE;
    first_mem_word.dest_operand_addressing = 0;
    first_mem_word.opcode = command.opcode;
    first_mem_word.src_operand_addressing = 0;

    if (command.type == COMMAND_WITH_0_PARAMETERS_CODE) {
        encode.encodings_array[word_index] = (*(unsigned int *) (&first_mem_word)) & ENCODING_FIRST_12_BIT_MASK;
        return encode;
    } else if (command.type == COMMAND_WITH_1_PARAMETERS_CODE) {
        /* encode the first word */
        first_mem_word.dest_operand_addressing = command.first_operand.addressing;
        first_mem_word.opcode = command.opcode;
        first_mem_word.src_operand_addressing = 0;

        encode.encodings_array[word_index] = (*(unsigned int *) (&first_mem_word)) & ENCODING_FIRST_12_BIT_MASK;
        ++word_index;

        second_mem_word.ARE = command.first_operand.ARE;

        if (command.first_operand.addressing == IMMEDIATE_ADDRESSING_CODE) {
            first_operand_data = atoi(command.first_operand.data);
        } else if (command.first_operand.addressing == LABEL_ADDRESSING_CODE) {
            first_operand_data = get_label_address(command.first_operand.data, symbols_table);
        } else if (command.first_operand.addressing == REGISTER_ADDRESSING_CODE) {
            first_operand_data = get_register_number(command.first_operand.data);
        }
        second_mem_word.other = first_operand_data;
        encode.encodings_array[word_index] = (*(unsigned int *) (&second_mem_word)) & ENCODING_FIRST_12_BIT_MASK;
    } else if (command.type == COMMAND_WITH_2_PARAMETERS_CODE) {
        /* encode the first word */
        first_mem_word.ARE = command.ARE;
        first_mem_word.dest_operand_addressing = command.second_operand.addressing;
        first_mem_word.opcode = command.opcode;
        first_mem_word.src_operand_addressing = command.first_operand.addressing;

        encode.encodings_array[word_index] = (*(unsigned int *) (&first_mem_word)) & ENCODING_FIRST_12_BIT_MASK;
        ++word_index;

        /* if the two operands are registers, encode their number in the same memory word */
        if (command.first_operand.addressing == REGISTER_ADDRESSING_CODE &&
            command.second_operand.addressing == REGISTER_ADDRESSING_CODE) {
            DoubleRegisterMemoryWord second_mem_word;

            /* only if the two operands are registers, they are stored in the same memory word and the command
             * takes one less memory words */
            encode.memory_words = command.type;

            second_mem_word.ARE = ARE_ABSOLUTE_CODE;
            second_mem_word.dest_register_number = get_register_number(command.second_operand.data);
            second_mem_word.src_register_number = get_register_number(command.first_operand.data);

            encode.encodings_array[word_index] = (*(unsigned int *) (&second_mem_word)) & ENCODING_FIRST_12_BIT_MASK;
        } else {
            /* encode the second memory word */
            second_mem_word.ARE = command.first_operand.ARE;

            if (command.first_operand.addressing == IMMEDIATE_ADDRESSING_CODE) {
                first_operand_data = atoi(command.first_operand.data);
            } else if (command.first_operand.addressing == LABEL_ADDRESSING_CODE) {
                first_operand_data = get_label_address(command.first_operand.data, symbols_table);
            } else if (command.first_operand.addressing == REGISTER_ADDRESSING_CODE) {
                first_operand_data =
                        get_register_number(command.first_operand.data) << ENCODING_SOURCE_REGISTER_SHIFT_MASK;
            }
            second_mem_word.other = first_operand_data;
            encode.encodings_array[word_index] = (*(unsigned int *) (&second_mem_word)) & ENCODING_FIRST_12_BIT_MASK;

            /* encode the third memory word */
            ++word_index;

            third_mem_word.ARE = command.second_operand.ARE;

            if (command.second_operand.addressing == IMMEDIATE_ADDRESSING_CODE) {
                second_operand_data = atoi(command.second_operand.data);
            } else if (command.second_operand.addressing == LABEL_ADDRESSING_CODE) {
                second_operand_data = get_label_address(command.second_operand.data, symbols_table);
            } else if (command.second_operand.addressing == REGISTER_ADDRESSING_CODE) {
                second_operand_data = get_register_number(command.second_operand.data);
            }
            third_mem_word.other = second_operand_data;
            encode.encodings_array[word_index] = (*(unsigned int *) (&third_mem_word)) & ENCODING_FIRST_12_BIT_MASK;
        }
    }
    return encode;
}

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
unsigned int encode_bit_field(int num, int length, int shift) {
    int bitmask = (1 << length) - 1;
    return (num & bitmask) << shift;
}

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
int get_label_address(char *label_name, DynamicArray *symbols_table) {
    int index = get_label_index(label_name, symbols_table);
    if (index >= 0) {
        Label label = GET_ELEMENT(symbols_table, Label*, index);
        return label.address;
    }
    return -1;
}

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
int get_register_number(char *register_name) {
    char delimiters[] = "@r"; /* separating characters */
    char *start; /* a pointer to the beginning of the current token */
    int digit;
    int values;
    int index;

    start = register_name + strspn(register_name, delimiters);
    values = sscanf(start, "%d", &digit);

    for (index = 0; index < (signed int)strlen(start); index++) {
        if (!isdigit(start[index])) {
            return -1;
        }
    }
    /* the given name is not a register name */
    if (register_name[0] != '@' || values <= 0) {
        return -1;
    }
    return digit;
}

/*
 * Returns a pointer to a string, that contains the converted version
 * of the given integer to base 64.
 *
 * Parameters:
 * -----------
 * unsigned int num     the integer to convert to base 64.
 */
char *convert_to_base_64(unsigned int num) {
    int length = MAX_NO_OF_CHARS_IN_64_ENCODING;
    char base_64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char *result = malloc(sizeof(char) * (length + 1));

    const int bitmask = 0x3F; /* bit mask to get the rightmost 6 bits of num */
    const int shift_amount = 6;

    /* store a null terminator */
    memset(result, 0, sizeof(length + 1));

    result[0] = base_64_chars[(num >> shift_amount) & bitmask];
    result[1] = base_64_chars[num & bitmask];

    return result;
}

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
int get_command_address(int row_index, DynamicArray *program_lines) {
    char *temp_command_content;
    int index;
    int temp_IC = 0;

    for (index = 0; index < (program_lines->length); index++) {
        temp_command_content = GET_STRING(program_lines, index);
        if (is_empty_command(temp_command_content) ||
            get_definition_type(temp_command_content) != COMMAND_DEFINITION_CODE) {
            continue;
        }
        if (index == row_index) {
            break;
        }
        temp_IC += get_no_of_memory_words(temp_command_content);
    }
    return temp_IC + LOAD_ADDRESS;
}

/*
 * Returns 1 if the given string is an existing register, and 0 otherwise.
 *
 * Parameters:
 * -----------
 * char *register_name  the name of the register.
 */
int is_existing_register(char *register_name) {
    return (MIN_REGISTER_NUMBER <= get_register_number(register_name) && get_register_number(register_name) <= MAX_REGISTER_NUMBER);
}

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
int is_addressing_method(int addressing_method, int addressing_methods[], int size) {
    int index;
    for (index = 0; index < size; index++) {
        if (addressing_methods[index] == addressing_method) {
            return 1;
        }
    }
    return 0;
}