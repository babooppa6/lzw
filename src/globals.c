/*
globals.c
contains implementation code for functions which are used by multiple files
in the LZW project

by Geoffrey Litt
*/

int bitsToRepresent(int codemax){
  int n = 0;

  while((1 << ++n) < codemax);

  return n;
}