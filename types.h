#ifndef ASSEMBLER_SIMULATOR_TYPES_H
#define ASSEMBLER_SIMULATOR_TYPES_H

#include "quantities.h"

/*
 * A structure that represent a Macro in the program. Each macro
 * in the program has a name, a starting index, and a finish index.
 * The starting index is the index of the row in the program in which
 * the macro starts, and the finish index is the index of the row in
 * the program in which the macro ends.
 */
typedef struct {
    char name[MAX_FIELD_LENGTH]; /* the name of the macro. */
    int start_index; /* the index of the row in the program that the macro starts. */
    int finish_index; /* the index of the row in the program that the macro ends. */
    int calls[MAX_NO_OF_COMMANDS]; /* the indexes of the rows in the program where the macro have been called.
 * In the end of the list of indexes, there will be a zero */
} Macro;

/*
 * A structure that represent a Label in the program.
 * Any label have a unique name, the address which was assigned
 * to it (IC/DC), a field that tells if the label is assigned to a
 * command or a data declaration, and the index of the row the
 * label is found in.
 */
typedef struct {
    char name[MAX_FIELD_LENGTH]; /* the name of the label. */
    int address; /* the address of the label IC/DC (if the label is assigned to a command,
 * then the address is IC, and DC if assigned to data. */
    int type; /* the type (extern/string/data) of the label is an integer. */
    int index; /* the index of the row in the program that the label is found in. */
} Label;

/*
 * A structure that represent an Operand in a command.
 * Each operand have an addressing method, and data:
 * if the operand is a label/register, the 'data' field
 * is the name of the label/register, if the operand is
 * an immediate, then the 'data' is the immediate number.
 * The type of the data could be inferred by the addressing
 * method of the operand, and with that, the user can
 * translate the data to different types.
 */
typedef struct {
    int addressing; /* the addressing code of the operand */
    int start; /* the index in which the operand starts in the command */
    int end; /* the index in which the operand ends in the command */
    int ARE; /* the A.R.E bits */
    char data[MAX_FIELD_LENGTH];
} Operand;

/*
 * The Command structure is a structure that was build
 * to hold the data of each Command in the program, after
 * the program was analyzed. Each command has its index, and
 * at most two operands. The following data about the command,
 * is sufficient to determine the number of data words the command
 * would be translated to (1-3 words), and the content of each
 * word of data in the translation.
 */
typedef struct {
    int opcode;
    int type; /* how many operands does the command gets (0, 1, or 2). */
    Operand first_operand;
    Operand second_operand;
    int ARE; /* the A.R.E bits */
    int index; /* the index of the row in the program that the command is found in. */
    char data[MAX_COMMAND_LENGTH]; /* all the characters of the command itself. */
} Command;

/*
 * A Field structure represents a segment in the command that stores a crucial value
 * to understand the command. The 'start' field is the index in which the field starts
 * in the command, and the 'end' is the index in which it ends in the command. As an
 * example, the name of the command, the label of the command, and the first operand
 * of the command are fields that will be represented using the Field structure.
 */
typedef struct {
    char content[MAX_FIELD_LENGTH];
    int start; /* the index in which the field starts in the command */
    int end; /* the index in which the field ends in the command */
} Field;

/*
 * A structure that defines an Operation in the program. Each Operation structure
 * has its own 'opcode', 'type', and more attributes such as its 'name' and the
 * addressing methods it can get for each of its operands.
 */
typedef struct {
    int opcode; /* the opcode of the operation */
    int type; /* how many operands does the command gets (0, 1, or 2). */
    char name[MAX_FIELD_LENGTH]; /* the name of the operation */
    int source_operand_addressing[NO_OF_ADDRESSING_METHODS]; /* the addressing methods that the source operand can accept */
    int destination_operand_addressing[NO_OF_ADDRESSING_METHODS]; /* the addressing methods that the destination operand can accept */
} Operation;

/*
 * An Encode structure stores the information that will be used to encode
 * a command into the code segment. In fact, the Encode structure already
 * stores the encodings of all the words of the command, and stores how many
 * words the command takes, in the 'type' field, in order to know how many
 * encodings to use.
 */
typedef struct {
    int memory_words; /* how many memory words does the command need. */
    unsigned int encodings_array[MAX_NO_OF_WORDS_IN_COMMAND]; /* an array of all the encodings words of the command */
} Encode;

/*
 * The FirstMemoryWord structure, is a bit field that stores the encoding
 * of the first memory word in the encoding of the whole command. Each command
 * has the same encoding fields in the first memory word, and this is why the
 * FirstMemoryWord struct has fields such as ARE, dest_operand_addressing and opcode.
 */
typedef struct {
    unsigned int ARE: 2;
    unsigned int dest_operand_addressing: 3;
    unsigned int opcode: 4;
    unsigned int src_operand_addressing: 3;
} FirstMemoryWord;

/*
 * The NextMemoryWord is a bit field structure that stores information
 * about the encoding of a memory word that is not the first, in the
 * encoding of the whole command, and only if the command don't take two
 * registers as arguments.
 */
typedef struct {
    unsigned int ARE: 2;
    unsigned int other: 10;
} NextMemoryWord;

/*
 * The DoubleRegisterMemoryWord is a bit field structure that stores information
 * about the encoding of a memory word that is not the first, in the encoding of
 * the whole command, and only if the command takes two registers as arguments.
 */
typedef struct {
    unsigned int ARE: 2;
    unsigned int dest_register_number: 5;
    unsigned int src_register_number: 5;
} DoubleRegisterMemoryWord;

#endif
