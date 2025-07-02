#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "decode.h"
#include "../hashmap.h"
#include "pipe_base.h"
#include "../processor.h"


struct hashmap *binary_map; 

int compare_binary_keys(const void *a, const void *b, void *udata) {
    const struct binary_to_comp *ua = a;
    const struct binary_to_comp *ub = b;
    return !(ua->binary == ub->binary); // 0 = equal, non-zero = not equal
}


bool binary_iter(const void *item, void *udata) {
    const struct binary_to_comp *component = item;
    printf("%d (comp=%s) \n", component->binary, instr_to_string(component->instr));
    return true;
}

uint64_t binary_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct binary_to_comp *component = item;
    return hashmap_sip(&component->binary, sizeof(component->binary), seed0, seed1);
}


void initialize_hashmap_decode() {
    binary_map = hashmap_new(sizeof(struct binary_to_comp), 0, 0, 0, 
                              binary_hash, compare_binary_keys, NULL, NULL);

    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b11000, .instr = ADD });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b11001, .instr = ADDI });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b11010, .instr = SUB });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b11011, .instr = SUBI });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b11100, .instr = MUL });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b11101, .instr = DIV });

    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b00000, .instr = AND });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b00001, .instr = OR });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b00010, .instr = XOR });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b00011, .instr = NOT });

    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b00100, .instr = SHFTR });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b00101, .instr = SHFTRI });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b00110, .instr = SHFTL });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b00111, .instr = SHFTLI });

    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b11110, .instr = HLT });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b10001, .instr = MOV });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b10010, .instr = MOVK });

    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b01000, .instr = BR });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b01001, .instr = BRR });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b01010, .instr = BRRI });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b01011, .instr = BRNZ });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b01110, .instr = BRGT });

    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b01100, .instr = CALL });
    hashmap_set(binary_map, &(struct binary_to_comp){ .binary = 0b01101, .instr = RET });

    return 0;

}


void fetch_registers(instruction_t opcode, uint32_t instruction) {

    if(opcode == ADD || opcode == SUB || opcode == MUL || opcode == DIV
    || opcode == AND || opcode == OR || opcode == XOR || opcode == SHFTR 
    || opcode == SHFTL || opcode == MOV) {

        uint8_t store_register = (instruction & 0x07C00000) >> 22; 
        writeback_stage.register_to_writeback = store_register; 

        uint8_t first_register = (instruction & 0x003E0000) >> 17; 
        execute_stage.first_register = first_register; 

        writeback_stage.write = true;  

        execute_stage.immediate_used = false; 
        uint8_t second_register = (instruction & 0x0001F000) >> 12; 
        execute_stage.second_register = second_register; 

    } else if(opcode == ADDI || opcode == SUBI || opcode == SHFTRI || opcode == SHFTLI || opcode == MOVK) {



        uint8_t store_register = (instruction & 0x07C00000) >> 22; 
        writeback_stage.register_to_writeback = store_register; 

        execute_stage.first_register = store_register; 

        writeback_stage.write = true;  

        execute_stage.immediate_used = true; 
        uint16_t immediate = (instruction & 0xFFF); 
        execute_stage.immediate = immediate; 

    } else if(opcode == NOT) {

        uint8_t store_register = (instruction & 0x07C00000) >> 22; 
        writeback_stage.register_to_writeback = store_register; 

        uint8_t first_register = (instruction & 0x003E0000) >> 17; 
        execute_stage.first_register = first_register; 

        writeback_stage.write = true;  

        execute_stage.immediate_used = false; 

    } else if(opcode == BR) {

        writeback_stage.write = false; 
        execute_stage.immediate_used = false; 
        execute_stage.addition_needed = false; 
        execute_stage.stack_flag = false; 

        uint8_t first_register = (instruction & 0x07C00000) >> 22; 
        execute_stage.first_register = first_register;


    } else if(opcode == CALL) { 

        writeback_stage.write = false; 
        execute_stage.immediate_used = false; 
        execute_stage.addition_needed = false; 
        execute_stage.stack_flag = true; 

        uint8_t first_register = (instruction & 0x07C00000) >> 22; 
        execute_stage.first_register = first_register;
    } else if(opcode == BRR) {

        writeback_stage.write = false; 
        execute_stage.immediate_used = false; 
        execute_stage.addition_needed = true; 
        execute_stage.stack_flag = false; 

        uint8_t first_register = (instruction & 0x07C00000) >> 22; 
        execute_stage.first_register = first_register;

    } else if(opcode == BRRI) {

        writeback_stage.write = false; 
        execute_stage.immediate_used = true; 
        execute_stage.addition_needed = true; 
        execute_stage.stack_flag = false; 

        uint8_t first_register = (instruction & 0x07C00000) >> 22; 
        execute_stage.first_register = first_register;

        uint16_t immediate = (instruction & 0xFFF); 
        execute_stage.immediate = immediate; 

    } else if(opcode == BRNZ) {

        writeback_stage.write = false; 
        execute_stage.immediate_used = false;
        execute_stage.addition_needed = false; 
        execute_stage.stack_flag = false;  

        uint8_t first_register = (instruction & 0x07C00000) >> 22; 
        execute_stage.first_register = first_register;

        uint8_t second_register = (instruction & 0x003E0000) >> 17; 
        execute_stage.second_register = second_register; 


    } else if(opcode == BRGT) {

        writeback_stage.write = false; 
        execute_stage.immediate_used = false;
        execute_stage.addition_needed = false; 
        execute_stage.stack_flag = false;  
        
        uint8_t first_register = (instruction & 0x07C00000) >> 22; 
        execute_stage.first_register = first_register; 

        uint8_t second_register = (instruction & 0x003E0000) >> 17; 
        execute_stage.second_register = second_register; 

        uint8_t third_register = (instruction & 0x0001F000) >> 12; 
        execute_stage.third_register = third_register; 


    } else if(opcode == HLT || opcode == RET) {
        writeback_stage.write = false; 
        execute_stage.immediate_used = false; 
    }
}



operation_t decide_operation(instruction_t opcode) {
    operation_t op; 
    switch(opcode) {
        case ADD:
        case ADDI:
            op = OP_ADD;
            break;
        case SUB:
        case SUBI:
            op = OP_SUB;
            break;
        case MUL:
            op = OP_MUL;
            break;
        case DIV:
            op = OP_DIV;
            break;
        case AND:
            op = OP_AND;
            break;
        case OR:
            op = OP_OR;
            break;
        case XOR:
            op = OP_XOR;
            break;
        case NOT:
            op = OP_NOT;
            break;
        case SHFTR:
        case SHFTRI:
            op = OP_SHFTR;
            break;
        case SHFTL:
        case SHFTLI:
            op = OP_SHFTL;
            break;
        case MOV:
        case MOVK:
            op = OP_MOV; 
            break; 
        case HLT:
            op = OP_HLT; 
            break; 
        case BR: 
        case BRR: 
        case BRRI: 
        case CALL:
            op = OP_BR; 
            break;
        case BRNZ: 
            op = OP_BRNZ; 
            break; 
        case BRGT: 
            op = OP_BRGT; 
            break; 
        case RET: 
            op = OP_RET;
            break; 
        case INVALID_INSTR:
        default:
            fprintf(stderr, "Invalid instruction\n");
            exit(1);
    }

    return op; 
}

void decode_instruction() {



  uint32_t instruction = decode_stage.instruction; 
  struct binary_to_comp *binary_test; 
  //get opcode 
  uint8_t binary_opcode = (instruction & 0xF8000000) >> 27; 
  binary_test = hashmap_get(binary_map, &(struct binary_to_comp){ .binary=binary_opcode });


  instruction_t opcode = binary_test->instr; 

  //now that we have opcode, let's implement the decoding of instructions 

  //get registers 

  fetch_registers(opcode, instruction); 

  //decide operation for thsi instruction 

  operation_t op = decide_operation(opcode);
  execute_stage.op = op;  



  if(op == OP_HLT) {
    program_finished = true; 
  }

}

