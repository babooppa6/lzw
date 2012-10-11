/*
bitio.h
contains function declaration for bit I/O

by Geoffrey Litt
*/

#include <limits.h>

// -----------------------------------------------------------------------------
// void putBits
// -----------------------------------------------------------------------------
// Description:
//   writes a code to stdout
// Parameters:
//   int nbits - the number of bits that should be used to represent the code
//   int code - the code being sent

void putBits(int nBits, int code);

// -----------------------------------------------------------------------------
// void sendRemainingBits
// -----------------------------------------------------------------------------
// Description:
//   writes any bits left over after all calls to putBits to stdout

void sendRemainingBits (void);

// -----------------------------------------------------------------------------
// void sendRemainingBits
// -----------------------------------------------------------------------------
// Description:
//   reads nbits bits from stdin and returns the corresponding code
// Parameters:
//   int nbits - the number of bits to read

int getBits (int nBits);