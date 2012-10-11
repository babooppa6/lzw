/*
main.c
Contains the main interface to the LZW encoder/decoder program
Parses arguments and hands off execution to the appropriate function

by Geoffrey Litt
*/

#include "globals.h"
#include "encode.h"
#include "decode.h"

void parseArguments(int argc, char** argv, Options *opt);

// -----------------------------------------------------------------------------
// int main
// -----------------------------------------------------------------------------
// Description:
//   handles the execution of the program at a high level
// Parameters:
//   int argc - number of command line arguments
//   char* argv[] - array of strings representing command line arguments
// Return values:
//   0 - indicates successful completion of the program

int main(int argc, char* argv[]){
  Options opt = {.decode = 0, .maxbits = 12, .prune = 0, .escape = 0};
  parseArguments(argc, argv, &opt);

  if(opt.decode){
    decode();
  }
  else{
    encode(&opt);
  }

  return 0;
}

// -----------------------------------------------------------------------------
// void parseArguments
// -----------------------------------------------------------------------------
// Description
//   validates/parses command line arguments
// Parameters:
//   int argc - number of command line arguments
//   char* argv[] - array of strings representing command line arguments
//   Options *opt - a pointer to an Options struct, which the parsed
//     options will be inserted into
// External state:
//   modifies fields in an Options struct passed in with *opt

void parseArguments(int argc, char** argv, Options *opt){
  int i;
  long j;
  char* execname = malloc(7*sizeof(char)); //6 chars for encode/decode + null

  for(i = 0; i < 6; i++){
    execname[i] = argv[0][strlen(argv[0]) - 6 + i];
  }
  execname[6] = '\0';

  //no arguments parsing if called as decode
  if(!strcmp(execname, "decode")){
    if(argc > 1){
      fprintf(stderr, "Error: no arguments should be passed to decode.\n");
      exit(EXIT_FAILURE);
    }
    else{
      opt->decode = 1;
    }
  }

  else if(!strcmp(execname, "encode")){
    for(i = 1; i < argc; i++){

      //handle the -m flag
      //increment i to look at the argument after the flag
      if(!strcmp(argv[i], "-m")){
        if(argc > ++i && (j = strtol(argv[i], 0, 10)) > 0){
          if(j <= (CHAR_BIT) || j > (3*CHAR_BIT)){
            j = 12;
          }
          opt->maxbits = (int)j;
        }
        else{
          fprintf(stderr, "Error: MAXBITS must be a positive integer.\n");
          exit(EXIT_FAILURE);
        }
      }

      //handle the -p flag
      //increment i to look at the argument after the flag
      else if(!strcmp(argv[i], "-p")){
        if(argc > ++i && (j = strtol(argv[i], 0, 10)) > 0){
          opt->prune = (int)j;
        }
        else{
          fprintf(stderr, "Error: WINDOW must be a positive integer.\n");
          exit(EXIT_FAILURE);
        }
      }

      //handle -e flag
      else if(!strcmp(argv[i], "-e")){
        opt->escape = 1;
      }

      else{
        fprintf(stderr, "Error: invalid option %s specified.\n", argv[i]);
        exit(EXIT_FAILURE);
      }
    }
  }

  else{
    //should never happen, but why not handle the error gracefully
    fprintf(stderr, "Error: invalid executable name. Must be encode or decode");
    exit(EXIT_FAILURE);
  }

  free(execname);
}
