#ifndef ASSEMBLER_SIMULATOR_SEGMENTS_H
#define ASSEMBLER_SIMULATOR_SEGMENTS_H

#include "quantities.h"

/* the instruction counter of the program */
extern int IC;
extern int final_IC;
/* the data counter of the program */
extern int DC;
extern int final_DC;
/* the address where the program will be stored */
extern int LOAD_ADDRESS;
/* indicates if an error has occurred in the program */
extern int ERROR_FLAG;

/* the encoding of the program's code */
extern unsigned int code_segment[MAX_NO_OF_COMMANDS][1];
/* the encoding of the program's data */
extern unsigned int data_segment[MAX_NO_OF_COMMANDS][1];

/* all the possible operations in the program */
extern const Operation operations[NO_OF_OPERATIONS];

#endif

