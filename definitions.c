#include "types.h"
#include "absolutes.h"
#include "quantities.h"
#include "function_macros.h"
#include "error_detection/errors.h"

/* the instruction counter of the program */
int IC;
int final_IC;
/* the data counter of the program */
int DC;
int final_DC;
/* the address where the program will be stored */
int LOAD_ADDRESS = 100;
/* indicates if an error has occurred in the program */
int ERROR_FLAG = 0;

/* the encoding of the program's code */
unsigned int code_segment[MAX_NO_OF_COMMANDS][1];
/* the encoding of the program's data */
unsigned int data_segment[MAX_NO_OF_COMMANDS][1];

/* the names of all the possible registers */
const char registers_names[NO_OF_REGISTERS][MAX_REGISTER_NAME_LENGTH + 1] = {
        {'@', 'r', '0', '\0'},
        {'@', 'r', '1', '\0'},
        {'@', 'r', '2', '\0'},
        {'@', 'r', '3', '\0'},
        {'@', 'r', '4', '\0'},
        {'@', 'r', '5', '\0'},
        {'@', 'r', '6', '\0'},
        {'@', 'r', '7', '\0'},

};

/* all the possible operations in the program and information about each one of them */
const Operation operations[NO_OF_OPERATIONS] = {
        {0,  COMMAND_WITH_2_PARAMETERS_CODE, "mov", {IMMEDIATE_ADDRESSING_CODE, LABEL_ADDRESSING_CODE, REGISTER_ADDRESSING_CODE}, {LABEL_ADDRESSING_CODE,     REGISTER_ADDRESSING_CODE}},
        {1,  COMMAND_WITH_2_PARAMETERS_CODE, "cmp", {IMMEDIATE_ADDRESSING_CODE, LABEL_ADDRESSING_CODE, REGISTER_ADDRESSING_CODE}, {IMMEDIATE_ADDRESSING_CODE, LABEL_ADDRESSING_CODE, REGISTER_ADDRESSING_CODE}},
        {2,  COMMAND_WITH_2_PARAMETERS_CODE, "add", {IMMEDIATE_ADDRESSING_CODE, LABEL_ADDRESSING_CODE, REGISTER_ADDRESSING_CODE}, {LABEL_ADDRESSING_CODE,     REGISTER_ADDRESSING_CODE}},
        {3,  COMMAND_WITH_2_PARAMETERS_CODE, "sub", {IMMEDIATE_ADDRESSING_CODE, LABEL_ADDRESSING_CODE, REGISTER_ADDRESSING_CODE}, {LABEL_ADDRESSING_CODE,     REGISTER_ADDRESSING_CODE}},
        {4,  COMMAND_WITH_1_PARAMETERS_CODE, "not",  EMPTY_SET,                                                                   {LABEL_ADDRESSING_CODE,     REGISTER_ADDRESSING_CODE}},
        {5,  COMMAND_WITH_1_PARAMETERS_CODE, "clr",  EMPTY_SET,                                                                   {LABEL_ADDRESSING_CODE,     REGISTER_ADDRESSING_CODE}},
        {6,  COMMAND_WITH_2_PARAMETERS_CODE, "lea", {LABEL_ADDRESSING_CODE},                                                      {LABEL_ADDRESSING_CODE,     REGISTER_ADDRESSING_CODE}},
        {7,  COMMAND_WITH_1_PARAMETERS_CODE, "inc",  EMPTY_SET,                                                                   {LABEL_ADDRESSING_CODE,     REGISTER_ADDRESSING_CODE}},
        {8,  COMMAND_WITH_1_PARAMETERS_CODE, "dec",  EMPTY_SET,                                                                   {LABEL_ADDRESSING_CODE,     REGISTER_ADDRESSING_CODE}},
        {9,  COMMAND_WITH_1_PARAMETERS_CODE, "jmp",  EMPTY_SET,                                                                   {LABEL_ADDRESSING_CODE,     REGISTER_ADDRESSING_CODE}},
        {10, COMMAND_WITH_1_PARAMETERS_CODE, "bne",  EMPTY_SET,                                                                   {LABEL_ADDRESSING_CODE,     REGISTER_ADDRESSING_CODE}},
        {11, COMMAND_WITH_1_PARAMETERS_CODE, "red",  EMPTY_SET,                                                                   {LABEL_ADDRESSING_CODE,     REGISTER_ADDRESSING_CODE}},
        {12, COMMAND_WITH_1_PARAMETERS_CODE, "prn",  EMPTY_SET,                                                                   {IMMEDIATE_ADDRESSING_CODE, LABEL_ADDRESSING_CODE, REGISTER_ADDRESSING_CODE}},
        {13, COMMAND_WITH_1_PARAMETERS_CODE, "jsr",  EMPTY_SET,                                                                   {LABEL_ADDRESSING_CODE,     REGISTER_ADDRESSING_CODE}},
        {14, COMMAND_WITH_0_PARAMETERS_CODE, "rts",  EMPTY_SET, EMPTY_SET},
        {15, COMMAND_WITH_0_PARAMETERS_CODE, "stop", EMPTY_SET, EMPTY_SET}
};

/* variables to solve the empty translation unit problem */
int variable_1 = 0;
int variable_2 = 0;
