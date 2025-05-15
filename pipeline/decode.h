#ifndef DECODE_H
#define DECODE_H

struct binary_to_comp {
  uint8_t binary; 
  char *comp; 
};


extern void decode_instruction(uint32_t instruction);


#endif