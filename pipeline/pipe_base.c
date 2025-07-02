#include "pipe_base.h"
#include "../processor.h"


FetchStage fetch_stage = {0}; 
DecodeStage decode_stage = {0}; 
ExecuteStage execute_stage = {0}; 
MemoryStage memory_stage = {0}; 
WritebackStage writeback_stage = {0}; 

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
        case BR: return "br"; 
        case BRR: return "brr"; 
        case BRRI: return "brri"; 
        case BRNZ: return "brnz"; 
        case CALL: return "call"; 
        case MOV: return "mov"; 
        case MOVK: return "movk"; 
        case HLT: return "hlt"; 
        case RET: return "ret"; 
        case BRGT: return "brgt"; 
        default: return "invalid";
    }
}

const char* operation_to_string(operation_t op) {
    switch (op) {
        case OP_ADD:   return "ADD";
        case OP_SUB:   return "SUB";
        case OP_MUL:   return "MUL";
        case OP_DIV:   return "DIV";
        case OP_AND:   return "AND";
        case OP_OR:    return "OR";
        case OP_NOT:   return "NOT";
        case OP_XOR:   return "XOR";
        case OP_SHFTL: return "SHFTL";
        case OP_SHFTR: return "SHFTR";
        case OP_HLT:   return "HLT";
        case OP_MOV:   return "MOV";
        case OP_BR:    return "BR";
        case OP_BRNZ:  return "BRNZ";
        case OP_RET:   return "RET"; 
        case OP_BRGT:  return "BRGT"; 
        default:       return "UNKNOWN";
    }
}



void print_pipeline() {
    printf("===== Pipeline Registers =====\n");

    // Fetch Stage
    printf("Fetch Stage:\n");
    printf("  Program Counter: 0x%08X\n", program_counter);

    // Decode Stage
    printf("Decode Stage:\n");
    printf("  Instruction:     0x%08X\n", decode_stage.instruction);

    // Execute Stage
    printf("Execute Stage:\n");
    printf("  Instruction:     0x%08X\n", execute_stage.instruction);
    printf("  Op:              %s\n", operation_to_string(execute_stage.op));
    printf("  First Reg:       %u\n", execute_stage.first_register);
    printf("  Second Reg:      %u\n", execute_stage.second_register);
    printf("  Immediate Used:  %s\n", execute_stage.immediate_used ? "true" : "false");
    printf("  Immediate:       0x%04X\n", execute_stage.immediate);

    // Memory Stage
    printf("Memory Stage:\n");
    printf("  Read:            %s\n", memory_stage.read ? "true" : "false");
    printf("  Write:           %s\n", memory_stage.write ? "true" : "false");
    printf("  Mem Address:     0x%08X\n", memory_stage.memory_address);

    // Writeback Stage
    printf("Writeback Stage:\n");
    printf("  Write:           %s\n", writeback_stage.write ? "true" : "false");
    printf("  Value:           0x%08X\n", writeback_stage.val);
    printf("  Dest Register:   %u\n", writeback_stage.register_to_writeback);

    printf("==============================\n");
}
