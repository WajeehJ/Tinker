#ifndef DECODE_H
#define DECODE_H

#include "pipe_base.h"

struct binary_to_comp {
  uint8_t binary; 
  instruction_t instr; 
};

extern void initialize_hashmap_decode();
extern void decode_instruction();


#endif