/*
bitio.c
contains implementation code for bit I/O

by Geoffrey Litt
*/

#include <stdlib.h>
#include <stdio.h>
#include "bitio.h"

//sendRemainingBits needs these variables at the end
static int numOverflow = 0;
static unsigned int overflow = 0;

void putBits (int nBits, int code)
{
  unsigned int c;

  numOverflow += nBits;
  code &= (1 << nBits) - 1;

  overflow = (overflow << nBits) | code;
  while (numOverflow >= CHAR_BIT) { 
  	numOverflow -= CHAR_BIT; 
  	c = overflow >> numOverflow;
  	putchar (c);
  	overflow = overflow ^ (c << numOverflow); //bitwise xor
  }
}

void sendRemainingBits (void)
{
  char c;
  if (numOverflow != 0){
    c = overflow << (CHAR_BIT - numOverflow);
  	putchar(c);
  }
}

int getBits (int nBits)
{
    int c;
    static int numOverflow = 0;
    static int unsigned overflow = 0;
                        
    while (numOverflow < nBits) {
      if ((c = getchar()) == EOF){
        return EOF;
      }
      numOverflow += CHAR_BIT;
      overflow = (overflow << CHAR_BIT) | c;
    }
    numOverflow -= nBits;
    c = overflow >> numOverflow;
    overflow = overflow ^ (c << numOverflow);
    return c;
}