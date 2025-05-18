#ifndef PIPE_BASE_H
#define PIPE_BASE_H

#include <stdint.h>

typedef enum {
    ADD,
    ADDI,
    SUB,
    SUBI,
    MUL,
    DIV,
    AND,
    OR,
    XOR,
    NOT,
    SHFTR,
    SHFTRI,
    SHFTL,
    SHFTLI,
    HLT, 
    INVALID_INSTR  // for error handling or unknown instructions
} instruction_t;

typedef enum {
  OP_ADD,
  OP_SUB, 
  OP_MUL,
  OP_DIV,
  OP_AND,
  OP_OR,
  OP_NOT, 
  OP_XOR, 
  OP_SHFTL,
  OP_SHFTR,
  OP_HLT
} operation_t

typedef struct {
  uint32_t program_counter; 
} FetchStage; 

typedef struct {
  uint32_t instruction; 
} DecodeStage; 

typedef struct {
  bool immediate_used;
  uint16_t immediate; 
  uint32_t instruction; 
  uint8_t first_register; 
  uint8_t second_register;
  operation_t op;  
} ExecuteStage; 

typedef struct {
  bool read; 
  bool write; 
  uint32_t memory_address; 
} MemoryStage; 

typedef struct {
  bool write;  //whether or not we will need to writeback
  uint32_t val; //what value that we will writeback
  uint32_t register_to_writeback; //register we need to writeback to 
} WritebackStage; 


extern const char* instr_to_string(instruction_t instr); 

extern FetchStage fetch_stage; 
extern DecodeStage decode_stage; 
extern ExecuteStage execute_stage; 
extern MemoryStage memory_stage; 
extern WritebackStage writeback_stage; 



#endif