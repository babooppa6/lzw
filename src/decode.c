/*
decode.c
contains the implementation of the LZW decoder

by Geoffrey Litt
*/

#include "globals.h"
#include "decode.h"
#include "hasharray.h"
#include "bitio.h"
#include "stack.h"

void decode(){
  //load option flags
  int maxbits = getBits(BITS_TO_SEND_MAXBITS);
  int window = getBits(BITS_TO_SEND_WINDOW);
  int escape = getBits(BITS_TO_SEND_ESCAPE);

  if(maxbits <= CHAR_BIT || maxbits > 24){
    fprintf(stderr,"Error: input file corrupted\n");
    exit(EXIT_FAILURE);
  }

	int code, oldcode, newcode, nbits;
  int timer = 1;
  int finalkar = 0;
  int justpruned = 0;

  if(escape){
    nbits = 3;
  }
  else{
    nbits = CHAR_BIT + 1;
  }

  struct elt* e;

  HashArray st = HashArrayCreate(1 << maxbits, escape);

  Stack kstack = stackCreate();

  oldcode = EMPTY;
  while((code = newcode = getBits(nbits)) != EOF){

    //handle nbits incrementing code
    if(code == INCR_NBITS){
      nbits++;
      continue;
    }

    //handle escape code
    if(code == ESCAPE){
      finalkar = getBits(CHAR_BIT);
      putchar(finalkar);
      if(HashArrayFreeSpots(st) != 0){
        HashArrayInsert(st, finalkar, EMPTY);
      }
      oldcode = EMPTY;
      continue;
    }

    //handle pruning code
    if(code == PRUNE){
      st = HashArrayPrune(st, window, escape, timer);
      nbits = bitsToRepresent(HashArrayElts(st));
      justpruned = 1;
      continue;
    }

    //========== main decoding algorithm ========== 

    //if unknown code, assume KwKwK
    if((e = HashArrayCodeLookup(st, code)) == 0){
      stackPush(kstack, finalkar);
      e = HashArrayCodeLookup(st, oldcode);
    }

    if(e == NULL){
      fprintf(stderr, "Error: input file corrupted\n");
      exit(EXIT_FAILURE);
    }

    //add all chars in the code to a stack
    while(e->prefix != EMPTY){
      stackPush(kstack, e->kar);
      code = e->prefix;
      e = HashArrayCodeLookup(st, code);
    }

    //save the first char of this code, and print it
    finalkar = e->kar;
    putchar(finalkar);

    //print all the chars in the stack
    while(!stackEmpty(kstack)){
      putchar(stackPop(kstack));
    }

    //insert the new code
    //(unless no space, or we just pruned the table, or it's a 1-char code)
    if (oldcode != EMPTY){
      if(HashArrayFreeSpots(st) != 0 && justpruned == 0){
        HashArrayInsert(st, finalkar, oldcode);
      }
    }

    HashArrayUpdateSentTime(st, newcode, timer++);

    oldcode = newcode;

    //reset just pruned, to re-enable string table insertions
    if(justpruned == 1) justpruned = 0;

  }

  HashArrayDestroy(st);
  stackDestroy(kstack);
}