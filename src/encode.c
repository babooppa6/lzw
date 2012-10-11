/*
encode.c
contains the implementation of the LZW encoder

by Geoffrey Litt
*/

#include "globals.h"
#include "encode.h"
#include "hasharray.h"
#include "bitio.h"

void encode(Options *opt){
  int maxbits = opt->maxbits;
  int window = opt->prune;
  int escape = opt->escape;
  int kar, code, nbits;
  int timer = 1;

  // send options data at the beginning of the file
  putBits(BITS_TO_SEND_MAXBITS, maxbits);
  putBits(BITS_TO_SEND_WINDOW, window);
  putBits(BITS_TO_SEND_ESCAPE, escape);

  struct elt* e;

  HashArray st = HashArrayCreate(1 << maxbits, escape);

  if(escape){
    nbits = 3;
  }
  else{
    nbits = CHAR_BIT + 1;
  }

  code = EMPTY;
  //encoding loop
  while((kar = getchar()) != EOF){

    //increment nbits if necessary
    if(bitsToRepresent(HashArrayElts(st) + 1) > nbits
      && (nbits + 1) <= maxbits){
        putBits(nbits, INCR_NBITS);
        nbits++;
    } 

    //========== main encoding algorithm ========== 

    //if the pair is in the table, use it and look for next char
    if((e = HashArrayCharPrefixLookup(st, kar, code))){
      code = e->code;
    }
    //if the pair is not found
    else{
      if(code == EMPTY){
        //if (kar, EMPTY) isn't in the table, need to send escape code
        putBits(nbits, ESCAPE);
        putBits(CHAR_BIT, kar);

        if(HashArrayFreeSpots(st) > 0){
          HashArrayInsert(st, kar, EMPTY);
        }
        else if(window != 0){ 
          st = HashArrayPrune(st, window, escape, timer);
          putBits(nbits, PRUNE);
          nbits = bitsToRepresent(HashArrayElts(st));
        }
        continue;
      }
      else{
        //output the code
        putBits(nbits, code);
        HashArrayUpdateSentTime(st, code, timer++);
      }

      e = HashArrayCharPrefixLookup(st, kar, EMPTY);
      if(e != NULL){
        //insert code, kar into string table
        //if we can't insert and pruning is enabled, then prune
        if(HashArrayFreeSpots(st) > 0){
          HashArrayInsert(st, kar, code);
        }
        else if(window != 0){ 
          st = HashArrayPrune(st, window, escape, timer);
          putBits(nbits, PRUNE);
          nbits = bitsToRepresent(HashArrayElts(st));

          //we need to find kar,EMPTY in the new table
          e = HashArrayCharPrefixLookup(st, kar, EMPTY);
        }

        //set code to index of (kar, EMPTY) in table
        code = e->code;
      }
      else{
        //the char we need to add on isn't in the table yet
        //we need to send escape code for this char first
        ungetc(kar, stdin);
        code = EMPTY;
      }

    }
  }

  //output code if not empty at the end
  if(code != EMPTY){
    putBits(nbits, code);
    HashArrayUpdateSentTime(st, code, timer++);
  }

  //output any extra bits left over
  sendRemainingBits();

  HashArrayDestroy(st);
}
