#ifndef DECODE_H
#define DECODE_H

#include "pipe_base.h"

struct binary_to_comp {
  uint8_t binary; 
  instruction_t instr; 
};


extern void decode_instruction(uint32_t instruction);


#endif