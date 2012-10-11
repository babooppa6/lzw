/*
globals.h
contains includes, definitions, and declarations used by all files in the LZW
project

by Geoffrey Litt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>

#define NUM_SPECIALS (4)          //the number of special codes

                                  //the special code value signifying:
#define EMPTY (0)                 //empty
#define ESCAPE (1)                //escape
#define PRUNE (2)                 //prune the table
#define INCR_NBITS (3)            //increment nbits

                                  //the number of bits at the beginning of the
                                  //encoded file used to signify:
#define BITS_TO_SEND_MAXBITS (8)  //maxbits
#define BITS_TO_SEND_WINDOW (24)  //window
#define BITS_TO_SEND_ESCAPE (1)   //escape


// -----------------------------------------------------------------------------
// struct options
// -----------------------------------------------------------------------------
// Description:
//   a struct encapsulating various parameters set by the user
// Fields:
//   int decode - 0 if the program should encode, 1 if it should decode
//   int maxbits - the maxbits value set by the user
//   int prune - the window value set by the user
//   int escape - 0 if the -e flag is not set, 1 if the -e flag is set

typedef struct options{
  int decode;
  int maxbits;
  int prune;
  int escape;
} Options;

// -----------------------------------------------------------------------------
// int bitsToRepresent
// -----------------------------------------------------------------------------
// Description:
//   returns the minimum number of bits necessary to represent a value
// Parameters:
//   int codemax - the value being represented
// Return value:
//   the minimum number of bits necessary to represent the value codemax

int bitsToRepresent(int codemax);