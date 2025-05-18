#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>  // for uint32_t
#include "object_compiler.h"
#include "./pipeline/decode.h"
#include "./pipeline/fetch.h"
#include "./pipeline/execute.h"
#include "./pipeline/writeback.h"
#include "./pipeline/pipe_base.h"
#include "processor.h" 

unsigned char memory[MEMORY_SIZE];
uint64_t registers[31];

uint32_t read_word(uint32_t addr) {
    if (addr + 3 >= MEMORY_SIZE) return -1;
    return (memory[addr] << 24) |
           (memory[addr + 1] << 16) |
           (memory[addr + 2] << 8) |
           (memory[addr + 3]);
}

uint32_t write_word(uint32_t addr, uint32_t value) {
    if (addr + 3 >= MEMORY_SIZE) return -1;
    memory[addr]     = (value >> 24) & 0xFF;
    memory[addr + 1] = (value >> 16) & 0xFF;
    memory[addr + 2] = (value >> 8)  & 0xFF;
    memory[addr + 3] = value & 0xFF;
    return 0; 
}


int main(int argc, char *argv[]) {


  if(argc < 2) {
    printf("Please pass in a file"); 
    return -1; 
  }

  //generate the object file
  initialize_hashmap();
  parse_file(argv[1]);


  program_counter = 0x1000;
  program_finished = false; 

  //pipeline 
  while(!program_finished) {
    fetch_instruction(); 
    decode_instruction();
    execute_instruction();  
    writeback(); 
  }




  
  return 0; 
}