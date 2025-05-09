#include <object_compiler.h> 


int main(int argc, char *argv[]) {
  int memory[512 * 1000] = malloc(512 * 1000); 

  if(argc < 2) {
    printf("Please pass in a file"); 
    return -1; 
  }
  parse_file(argv[1]); 
  return 0; 
}