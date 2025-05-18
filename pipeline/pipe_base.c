#include "pipe_base.h"


FetchStage fetch_stage; 
DecodeStage decode_stage; 
ExecuteStage execute_stage; 
MemoryStage memory_stage; 
WritebackStage writeback_stage; 

const char* instr_to_string(instruction_t instr) {
    switch (instr) {
        case ADD: return "add";
        case ADDI: return "addi";
        case SUB: return "sub";
        case SUBI: return "subi";
        case MUL: return "mul";
        case DIV: return "div";
        case AND: return "and";
        case OR: return "or";
        case XOR: return "xor";
        case NOT: return "not";
        case SHFTR: return "shftr";
        case SHFTRI: return "shftri";
        case SHFTL: return "shftl";
        case SHFTLI: return "shftli";
        default: return "invalid";
    }
}
