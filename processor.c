#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>  // for uint32_t
#include "object_compiler.h"
#include "./pipeline/decode.h"

unsigned char memory[512 * 1000];


int main(int argc, char *argv[]) {


  if(argc < 2) {
    printf("Please pass in a file"); 
    return -1; 
  }

  //generate the object file
  initialize_hashmap();
  parse_file(argv[1]);


  char object_filename[256];           // make sure buffer is large enough

  snprintf(object_filename, sizeof(object_filename), "%s.obj", argv[1]);

  //read a file 32 bytes at a time 
  FILE *file = fopen(object_filename, "rb");  // Open file in binary mode
  if (!file) {
    perror("Failed to open file");
    return 1;
  }

  uint32_t buffer;  // Buffer to hold 32-bit chunks

  while (fread(&buffer, sizeof(uint32_t), 1, file) == 1) {
    // Process the 32-bit chunk
  }

  if (!feof(file)) {
    // If loop exited not because of EOF, print error
    perror("File read error");
  }

  fclose(file);



  
  return 0; 
}