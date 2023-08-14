#ifndef ASSEMBLER_SIMULATOR_ABSOLUTES_H
#define ASSEMBLER_SIMULATOR_ABSOLUTES_H

#define MACRO_DEFINITION_START_NAME "mcro"
#define MACRO_DEFINITION_END_NAME "endmcro"
#define LABEL_ENDING_CHARACTER ':'

#define DATA_DEFINITION_CODE 1 /* the code of a .data definition */
#define STRING_DEFINITION_CODE 2 /* the code of a .string definition */
#define EXTERN_DEFINITION_CODE 3 /* the code of a .extern definition */
#define ENTRY_DEFINITION_CODE 4 /* the code of a .entry definition */
#define COMMAND_DEFINITION_CODE 5 /* the code of a regular command definition */

#define COMMAND_WITH_0_PARAMETERS_CODE 0
#define COMMAND_WITH_1_PARAMETERS_CODE 1
#define COMMAND_WITH_2_PARAMETERS_CODE 2

#define IMMEDIATE_ADDRESSING_CODE 1 /* the addressing code of an immediate value */
#define LABEL_ADDRESSING_CODE 3 /* the addressing code of a label */
#define REGISTER_ADDRESSING_CODE 5 /* the addressing code of a register */
#define UNKNOWN_ADDRESSING_CODE 9 /* the addressing code of an unknown operand */

#define DATA_DEFINITION_NAME ".data"
#define STRING_DEFINITION_NAME ".string"
#define EXTERN_DEFINITION_NAME ".extern"
#define ENTRY_DEFINITION_NAME ".entry"

#define ARE_ABSOLUTE_CODE 0
#define ARE_RELOCATABLE_CODE 2
#define ARE_EXTERNAL_CODE 1

#define ENCODING_FIRST_12_BIT_MASK 0xFFF /* a bit mask to return the first 12 bits of the encoding */
#define ENCODING_SOURCE_REGISTER_SHIFT_MASK 5

#define INPUT_CODE_FILE_EXTENSION ".as"
#define OUTPUT_CODE_FILE_EXTENSION ".am"

#define OUTPUT_NO_MACROS_FILE_EXTENSION ".am"
#define OUTPUT_ENTRIES_FILE_EXTENSION ".ent"
#define OUTPUT_OBJECT_FILE_EXTENSION ".ob"
#define OUTPUT_EXTERNALS_FILE_EXTENSION ".ext"


#define EMPTY_SET {UNKNOWN_ADDRESSING_CODE}

#include "quantities.h"
#include "types.h"

/* the names of all the possible registers */
extern const char registers_names[NO_OF_REGISTERS][MAX_REGISTER_NAME_LENGTH + 1];

#endif
