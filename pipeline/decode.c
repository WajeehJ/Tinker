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
    return ua->binary == ub->binary;
}

bool binary_iter(const void *item, void *udata) {
    const struct binary_to_comp *component = item;
    printf("%d (comp=%s) \n", component->binary, instr_to_string(component->instr));
    return true;
}

uint64_t binary_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct binary_to_comp *component = item;
    return hashmap_sip(instr_to_string(component->instr), strlen(instr_to_string(component->instr)), seed0, seed1);
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

    return 0;

}


void fetch_registers(instruction_t opcode, uint32_t instruction) {

    if(opcode == ADD || opcode == SUB || opcode == MUL || opcode == DIV
    opcode == AND || opcode == OR || opcode == XOR || opcode == SHFTR 
    || opcode == SHFTL) {

        uint8_t store_register = (instruction & 0x07C00000) >> 22; 
        writeback_stage.register_to_writeback = store_register; 

        uint8_t first_register = (instruction & 0x003E0000) >> 17; 
        execute_stage.first_register = first_register; 

        writeback_stage.write = true;  

        execute_stage.immediate_used = false; 
        uint8_t second_register = (instruction & 0x0001F000) >> 12; 
        execute_stage.second_register = second_register; 

    } else if(opcode == ADDI || opcode == SUBI || opcode == SHFTR || opcode == SHFTI) {

        uint8_t store_register = (instruction & 0x07C00000) >> 22; 
        writeback_stage.register_to_writeback = store_register; 

        uint8_t first_register = (instruction & 0x003E0000) >> 17; 
        execute_stage.first_register = first_register; 

        writeback_stage.write = true;  

        execute_stage.immediate_used = true; 
        uint16_t immediate = (instruction & 0xFFF); 
        execute_stage.immedate = immediate; 

    } else if(opcode == NOT) {

        uint8_t store_register = (instruction & 0x07C00000) >> 22; 
        writeback_stage.register_to_writeback = store_register; 

        uint8_t first_register = (instruction & 0x003E0000) >> 17; 
        execute_stage.first_register = first_register; 

        writeback_stage.write = true;  

        execute_stage.immediate_used = false; 

    } else if(opcode == HLT) {
        writeback_stage.write = false; 
        execute_stage.immedate_used = false; 
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
        case HLT:
            op = OP_HLT; 
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
  execute_instruction.op = op;  

  if(op == HLT) {
    program_finished = true; 
  }

}

