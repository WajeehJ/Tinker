#include <stdio.h>
#include "processor.h" 

//purpose of this file is to build a object file that can be executed 
int parse_file(const char* filename) {


  FILE *fptr = fopen(filename, "r");

  // Store the content of the file
  char myString[100];

  // Read the content and print it
  while(fgets(myString, 100, fptr)) {
    printf("%s\n", myString);
  }

  fclose(fptr);
  
  

  
  return 0; 
}