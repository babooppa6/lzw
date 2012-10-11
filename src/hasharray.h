/*
hasharray.h
contains declarations for a combined hashtable array data structure used to
store the string table for both the LZW encoder and decoder (a "HashArray").

A common set of (code, char, prefix) triplets is accessible by either a hash
table lookup (if searching by char and prefix) or by array lookup (if searching
by code), which reduces code repetition and offers increased assurance that the
string tables used in the encoder and decoder act in the same way.

by Geoffrey Litt
*/

typedef struct hasharray *HashArray;

// -----------------------------------------------------------------------------
// struct elt
// -----------------------------------------------------------------------------
// Description:
//   A struct that contains an entry in the string table.
//   The definition is externally exposed because several functions related to
//   HashArrays return a pointer to a struct elt, which is then dealt with
//   by an external caller.
// Fields:
//   int code - the numerical code of the string table entry
//   int kar - the trailing char of the string table entry
//   int prefix - the code of the prefix of the string table entry
//   int time - a number representing the last time the code was sent

struct elt{
  int code;
  int kar;
  int prefix;
  int time;
};

// -----------------------------------------------------------------------------
// HashArray HashArrayCreate
// -----------------------------------------------------------------------------
// Description:
//   creates a new HashArray, allocates the necessary memory
// Parameters:
//   int size - the maximum number of elements that the HashArray should hold
//   int escape - the value of the escape flag (0 or 1), which determines whether
//                to initialize the HashArray with all one-character strings
// Return value:
//   returns an initialized HashArray, which is a pointer to a struct hasharray.

HashArray HashArrayCreate(int size, int escape);

// -----------------------------------------------------------------------------
// void HashArrayDestroy
// -----------------------------------------------------------------------------
// Description:
//   destroys a HashArray and frees all associated memory
// Parameters:
//   HashArray ha - the HashArray to destroy
// External state:
//   destroys the HashArray ha passed in

void HashArrayDestroy(HashArray ha);

// -----------------------------------------------------------------------------
// void HashArrayInsert
// -----------------------------------------------------------------------------
// Description:
//   Inserts a (char, prefix) pair into a string table
// Parameters:
//   HashArray ha - the HashArray to insert into
//   int kar - the character to insert
//   int prefix - the prefix code to insert
// External state:
//   modifies the HashArray passed in by adding an element

void HashArrayInsert(HashArray ha, int kar, int prefix);

// -----------------------------------------------------------------------------
// struct elt* HashArrayCharPrefixLookup
// -----------------------------------------------------------------------------
// Description:
//   looks up the code matching a char and prefix, using the hash table part
//   of the data structure for performance
// Parameters:
//   HashArray ha - the HashArray to search in
//   int kar - the character to search for
//   int prefix - the prefix to search for
// Return value:
//   Returns a pointer to a struct elt which matches the char and prefix
//   searched for. If no match is found, a NULL pointer is returned.
//   The caller can then access fields in the struct such as the code.

struct elt* HashArrayCharPrefixLookup(HashArray ha, int kar, int prefix);

// -----------------------------------------------------------------------------
// struct elt* HashArrayCodeLookup
// -----------------------------------------------------------------------------
// Description:
//   looks up the char and prefix matching a code, using the array part of the
//   data structure
// Parameters:
//   HashArray ha - the HashArray to search in
//   int code - the code to search for
// Return value:
//   Returns a pointer to a struct elt which matches the code searched for.
//   If no match is found, a NULL pointer is returned.
//   The caller can then access fields in the struct such as char and prefix.

struct elt* HashArrayCodeLookup(HashArray ha, int code);

// -----------------------------------------------------------------------------
// void HashArrayUpdateSentTime
// -----------------------------------------------------------------------------
// Description:
//   updates the last sent time for a given code
// Parameters:
//   HashArray ha - the HashArray to update
//   int code - the code of the string table entry to update
//   int time - a number representing the last sent time for the given code
// External state:
//   modifies an entry in the HashArray passed in

void HashArrayUpdateSentTime(HashArray ha, int code, int time);

// -----------------------------------------------------------------------------
// int HashArrayFreeSpots
// -----------------------------------------------------------------------------
// Description:
//   returns the number of free spaces for entries in a HashArray
// Parameters:
//   HashArray ha - the HashArray to check for free spaces
// Return value:
//   the number of free spaces in the HashArray

int HashArrayFreeSpots(HashArray ha);

// -----------------------------------------------------------------------------
// int HashArrayElts
// -----------------------------------------------------------------------------
// Description:
//   returns the number of entries in a HashArray
// Parameters:
//   HashArray ha - the HashArray to examine
// Return value:
//   the number of entries in the HashArray

int HashArrayElts(HashArray ha);

// -----------------------------------------------------------------------------
// HashArray HashArrayPrune
// -----------------------------------------------------------------------------
// Description:
//   prunes a HashArray to only include the last window strings
//   (and all one-character strings, if escape is 0)
// Parameters:
//   HashArray ha - the HashArray to prune
//   int window - the value of WINDOW, i.e. how far back to accept strings
//   int escape - 0 or 1, representing whether -e is set. If escape = 1, all the
//                one character strings are not added to the new table.
//   int curtime - a number representing the current "time", relative to which
//                 the last sent times for the codes will be compared
// Return value:
//   Returns a new HashArray which represents the pruned HashArray

HashArray HashArrayPrune(HashArray ha, int window, int escape, int curtime);
