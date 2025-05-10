#include <stdio.h>
#include <stdlib.h> 
#include "object_compiler.h"

unsigned char memory[512 * 1000];


int main(int argc, char *argv[]) {


  if(argc < 2) {
    printf("Please pass in a file"); 
    return -1; 
  }

  //generate the object file
  initialize_hashmap();
  parse_file(argv[1]); 

  
  return 0; 
}