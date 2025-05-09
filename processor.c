#include <stdio.h>
#include <stdlib.h> 
#include "object_compiler.h"


int main(int argc, char *argv[]) {

  //initialize memory of the processor 
  int memory[512 * 1000];

  if(argc < 2) {
    printf("Please pass in a file"); 
    return -1; 
  }

  //generate the object file
  parse_file(argv[1]); 

  
  return 0; 
}