#include "execute.h" 
#include "pipe_base.h"
#include "../processor.h"
#include <stdio.h>


void execute_instruction() {

  uint64_t vala; 
  uint64_t valb; 


  if(execute_stage.op == OP_HLT) {
    return; 
  }

  if(execute_stage.op == OP_RET) {
    program_counter = read_word(registers[31]-8); 
    return;
  }

  if(execute_stage.op == OP_BR) {
    if(execute_stage.addition_needed) {
      vala = program_counter; 
      if(execute_stage.immediate_used) {
        valb = execute_stage.immediate; 
      } else {
        valb = registers[execute_stage.first_register]; 
      }
      program_counter = alu(vala, valb, OP_ADD); 
    } else {
      if(execute_stage.stack_flag) {
        write_word(registers[31] - 8, program_counter); 
      } 
      program_counter = registers[execute_stage.first_register];
    }
    return; 
  } else if(execute_stage.op == OP_BRNZ) {
    if(registers[execute_stage.second_register] != 0) {
      program_counter = registers[execute_stage.first_register];
    }
    return; 
  } else if(execute_stage.op == OP_BRGT) {
    if((int64_t)registers[execute_stage.second_register] > (int64_t)registers[execute_stage.third_register]) {
      program_counter = registers[execute_stage.first_register]; 
    }
    return; 
  }



  vala = registers[execute_stage.first_register]; 
  if(execute_stage.immediate_used) {
    valb = execute_stage.immediate; 
  } else {
    valb = registers[execute_stage.second_register];
  }

  if(execute_stage.op == OP_MOV) {
    writeback_stage.val = valb; 
  } else { 
    writeback_stage.val = alu(vala, valb, execute_stage.op); 
  }
}


int alu(uint64_t vala, uint64_t valb, operation_t op) {
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
    default:
      // Optional: handle invalid op
      fprintf(stderr, "Unknown operation\n");
      exit(1);
  }

}