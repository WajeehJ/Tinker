#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdint.h>
#include <stdbool.h>

#define MEMORY_SIZE (512 * 1024)  // 512 KB
extern unsigned char memory[MEMORY_SIZE];  


// registers 
extern uint64_t registers[31]; 

extern uint32_t program_counter; 

extern bool program_finished; 



extern uint32_t read_word(uint32_t addr); 


extern uint32_t write_word(uint32_t addr, uint32_t value);

extern void print_registers(); 



#endif