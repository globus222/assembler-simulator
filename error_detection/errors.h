#ifndef ASSEMBLER_SIMULATOR_ERRORS_H
#define ASSEMBLER_SIMULATOR_ERRORS_H

#define UNDEFINED_COMMAND "Undefined command!"
#define MISSING_ARGUMENTS "Missing arguments!"
#define INVALID_NO_OF_ARGUMENTS "Invalid number of arguments!"
#define INVALID_LABEL_CHARACTERS "Label name contains invalid characters!"
#define EXTRANEOUS_TEXT "Extraneous text after end of command!"
#define INVALID_OPERAND_TYPE "Invalid operand type!"

#define INVALID_EXTERN_LABEL_DEFINITION "External label was defined in this file!"
#define MULTIPLE_LABEL_DEFINITIONS "Multiple definitions of the same label!"

#define UNDEFINED_REGISTER_NAME "Undefined register name!"
#define EXTERN_AND_ENTRY_LABEL "Label was defined as external and as entry label!"
#define ENTRY_LABEL_WASNT_DEFINED "Entry label was not defined in this file!"
#define ILLEGAL_COMMA "Illegal comma!"
#define MISSING_COMMA "Missing comma!"
#define MULTIPLE_CONSECUTIVE_COMMAS "Multiple consecutive commas!"

#define MEMORY_OVERFLOW "The following program consumes more memory than exists!"
#define UNRECOGNIZED_DECLARATION "The following declaration do not exists!"
#define DECLARATION_WITH_NO_LABEL "The following .extern/.entry declaration do not contain a label!"

extern int variable_2; /* a variable to solve the empty translation unit problem */

#endif
