#ifndef ASSEMBLER_SIMULATOR_QUANTITIES_H
#define ASSEMBLER_SIMULATOR_QUANTITIES_H

#define NO_OF_REGISTERS 8           /* the number of registers in the program */
#define MAX_REGISTER_NAME_LENGTH 3  /* the maximum number of characters each register name can have */

#define MAX_NAME_FIELD_INDEX 1 /* the maximum index number that the command name can be in the positions array */

#define NO_OF_OPERATIONS_TYPE_0 2     /* the number of commands that take 0 operands */
#define NO_OF_OPERATIONS_TYPE_1 9     /* the number of commands that take 1 operand */
#define NO_OF_OPERATIONS_TYPE_2 5     /* the number of commands that take 2 operands */

#define MAX_COMMAND_LENGTH 80      /* the maximum amount of characters in a sentence (line) of the program */
#define MAX_FIELD_LENGTH 80         /* the maximum amount of characters in a field of a command */

#define MAX_NO_OF_COMMANDS 1500     /* the maximum number of commands that can be in the program */
#define MAX_NO_OF_DATA 1500         /* the maximum number of data declarations that can be in the program */

#define NO_OF_FIELDS_IN_MACRO_CALL_OR_END 1 /* the number of fields in a macro call or an end of a macro definition */
#define NO_OF_ADDRESSING_METHODS 3 /* the number of addressing methods that exists in the program */

#define MIN_NO_OF_WORDS_IN_COMMAND 1 /* the minimum number of memory words a command can use */
#define MAX_NO_OF_WORDS_IN_COMMAND 3 /* the maximum number of memory words a command can use */

#define MAX_NO_OF_CHARS_IN_64_ENCODING 3 /* the maximum number of characters in the conversion of the assembly code to base 64 */
#define MAX_FILE_NAME_LENGTH 200 /* the maximum length of the name of a program's file */
#define NO_OF_MEMORY_WORDS_IN_PROGRAM 1024 /* the maximum number of memory words in a program */

#define MIN_REGISTER_NUMBER 0 /* the lowest number a register can have */
#define MAX_REGISTER_NUMBER 7 /* the largest number a register can have */

#define NO_OF_OPERATIONS (NO_OF_OPERATIONS_TYPE_0 + NO_OF_OPERATIONS_TYPE_1 + NO_OF_OPERATIONS_TYPE_2) /* the number of operations in the assembly language */

#include "absolutes.h"

#endif
