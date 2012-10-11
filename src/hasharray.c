/*
hasharray.c
contains implementation code for a combined hashtable array data structure
used to store the string table for both the LZW encoder and decoder

A common set of (code, char, prefix) triplets is accessible by either a hash
table lookup (if searching by char and prefix) or by array lookup (if searching
by code), which reduces code repetition and offers increased assurance that the
string tables used in the encoder and decoder act in the same way.

by Geoffrey Litt
*/

#include "globals.h"
#include "hasharray.h"
#include "stack.h"

// -----------------------------------------------------------------------------
// struct hasharray
// -----------------------------------------------------------------------------
// Description:
//   an internal struct that stores data (and metadata) for a hash array
// Fields:
//   int size - the maximum number of elements that can fit in the hash array
//   int elts - the number of elements currently stored in the hash array
//   struct elt **hashtable - a hash table data structure
//   struct elt **array - an array data structure

struct hasharray{
  int size;
  int elts;
  struct elt **hashtable;
  struct elt **array;
};

// -----------------------------------------------------------------------------
// int hash
// -----------------------------------------------------------------------------
// Description:
//   a hash function used to compute indices for a hash table
// Parameters:
//   int prefix - the prefix of the triple being stored
//   int kar - the char of the triple being stored
//   int size - the size of the hash table
// Return value:
//   the index that should be used to store the triplet

int hash(int prefix, int kar, int size){
  int i;
  i = ((unsigned int)(prefix) << CHAR_BIT | (unsigned int)(kar)) % size;
  return i;
}

HashArray HashArrayCreate(int size, int escape){
  HashArray ha;
  int i;

  ha = malloc(sizeof(*ha));

  ha->size = size;
  ha->elts = 0;

  //allocate hashtable memory
  //2*max number of elements for performance
  //+1 to improve hash function performance
  ha->hashtable = calloc((2 * size) + 1, sizeof(struct elt *));

  //allocate array memory
  //simply the max number of elements for performance
  ha->array = calloc(size, sizeof(struct elt *));

  //reserve the 4 special codes
  //use -1 as kar to avoid finding these entries otherwise
  for(i = 0; i < 4; i++){
    HashArrayInsert(ha, -1, 0);
  }

  //populate the string table with single characters (unless -e is set)
  if(!escape){
    for(i = 0; i < (1 << CHAR_BIT); i++){
      HashArrayInsert(ha, i, 0);
    }
  }

  return ha;
}

void HashArrayDestroy(HashArray ha){
  int i;

  for(i = 0; i < ha->size; i++){
    if(ha->array[i] != NULL) free(ha->array[i]);
  }

  free(ha->hashtable);
  free(ha->array);
  free(ha);
}

void HashArrayInsert(HashArray ha, int kar, int prefix){

  if(HashArrayFreeSpots(ha) == 0){
    //this situation should be avoided by the caller
    return;
  }

  struct elt *e;
  int i;
  int code = ha->elts;

  e = malloc(sizeof(*e));

  e->code = code;
  e->kar = kar;
  e->prefix = prefix;
  e->time = 0;

  //insert into hashtable first
  i = hash(prefix, kar, ha->size);
  //keep incrementing the index if something is already there
  while(ha->hashtable[i] != NULL) i++;
  //when we find an empty spot, insert there
  ha->hashtable[i] = e;

  //then insert into the array -- pretty straightforward
  if(ha->array[code] == NULL){
    ha->array[code] = e;
  }
  else{
    fprintf(stderr, "code double counted!\n");
  }

  //increment the hasharray's counter for number of elements
  ha->elts++;
}

struct elt* HashArrayCharPrefixLookup(HashArray ha, int kar, int prefix){
  struct elt *e;
  int i;
  i = hash(prefix, kar, ha->size);

  //look in the hash table, use linear probing
  while((e = ha->hashtable[i++]) != NULL){
    if(e->kar == kar && e->prefix == prefix){
      return e;
    }
  }

  return 0;
}

struct elt* HashArrayCodeLookup(HashArray ha, int code){
  //if the element exists, the elt* will be returned
  //otherwise a null pointer will be returned
  return ha->array[code];
}

void HashArrayUpdateSentTime(HashArray ha, int code, int time){
  (ha->array[code])->time = time;
}

int HashArrayFreeSpots(HashArray ha){
  return ha->size - ha->elts;
}

int HashArrayElts(HashArray ha){
  return ha->elts;
}

HashArray HashArrayPrune(HashArray ha, int window, int escape, int curtime){
  int i, j;
  int size = ha->size;
  struct elt* e;

  //create and initialize array mapping old codes to new
  int newcodes[size];
  for(i = 0; i < size; i++){
    newcodes[i] = 0;
  }
  if(!escape){
    for(i = 0; i < (1 << CHAR_BIT); i++){
      j = i + NUM_SPECIALS;
      newcodes[j] = j;
    }
  }

  int cutofftime = curtime - window;
  if(cutofftime < 0) cutofftime = 0;

  HashArray newha = HashArrayCreate(size, escape);

  Stack codestack = stackCreate();

  for(i = 0; i < size; i++){
    e = ha->array[i];
    if(e->time > cutofftime && newcodes[e->code] == 0){
      //we need to add prefixes starting from beginning of string
      while(e->prefix != EMPTY){
        stackPush(codestack, e->code);
        e = HashArrayCodeLookup(ha, e->prefix);
      }
      //if -e is on, we need to add one character codes too
      if(escape){
        stackPush(codestack, e->code);
      }

      //add all the prefixes and the code itself
      while(!stackEmpty(codestack)){
        e = HashArrayCodeLookup(ha, stackPop(codestack));
        if(newcodes[e->code] == 0){ //if code hasn't been transplanted yet
          newcodes[e->code] = newha->elts;
          HashArrayInsert(newha, e->kar, newcodes[e->prefix]);
          HashArrayUpdateSentTime(newha, newcodes[e->code], e->time);
        }
      }
    }
  }

  HashArrayDestroy(ha);
  stackDestroy(codestack);

  return newha;
}
