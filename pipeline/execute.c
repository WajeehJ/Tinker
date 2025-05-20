#include "execute.h" 
#include "pipe_base.h"
#include "../processor.h"
#include <stdio.h>


void execute_instruction() {
  int vala = registers[execute_stage.first_register]; 
  int valb; 
  if(execute_stage.immediate_used) {
    valb = execute_stage.immediate; 
  } else {
    valb = registers[execute_stage.second_register];
  }

  writeback_stage.val = alu(vala, valb, execute_stage.op); 
}


int alu(int vala, int valb, operation_t op) {
  switch(op) {
    case OP_ADD:
      return vala + valb;
    case OP_SUB:
      return vala - valb;
    case OP_MUL:
      return vala * valb;
    case OP_DIV:
      return vala / valb;
    case OP_AND:
      return vala & valb;
    case OP_OR:
      return vala | valb;
    case OP_NOT:
      return ~vala;  // Assuming valb is ignored for NOT
    case OP_XOR:
      return vala ^ valb;
    case OP_SHFTL:
      return vala << valb;
    case OP_SHFTR:
      return vala >> valb;
    case OP_MOV:
      return valb; 
    case OP_HLT: 
      return 0; 
    default:
      // Optional: handle invalid op
      fprintf(stderr, "Unknown operation\n");
      exit(1);
  }

}