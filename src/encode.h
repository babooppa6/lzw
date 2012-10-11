/*
encode.h
contains function declaration for the LZW encoder

by Geoffrey Litt
*/

// -----------------------------------------------------------------------------
// void encode
// -----------------------------------------------------------------------------
// Description:
//  compresses a bytestream from stdin using the LZW algorithm,
//  outputs the compressed bytestream to stdout
// Parameters:
//   Options* opt - a pointer to an options struct containing parameters for
//                  the program's operation

void encode(Options* opt);