#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "processor.h" 
#include "hashmap.h"
#include "object_compiler.h"


struct hashmap *map; 

int compare_comps(const void *a, const void *b, void *udata) {
    const struct comp_to_binary *ua = a;
    const struct comp_to_binary *ub = b;
    return strcmp(ua->comp, ub->comp);
}

bool comp_iter(const void *item, void *udata) {
    const struct comp_to_binary *component = item;
    printf("%s (binary=%d)\n", component->comp, component->binary);
    return true;
}

uint64_t comp_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct comp_to_binary *component = item;
    return hashmap_sip(component->comp, strlen(component->comp), seed0, seed1);
}

void add_registers_to_map(void *map) {

    // Array of register names
    const char *registers[] = {
        "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10",
        "r11", "r12", "r13", "r14", "r15", "r16", "r17", "r18", "r19", "r20",
        "r21", "r22", "r23", "r24", "r25", "r26", "r27", "r28", "r29", "r30"
    };

    // Array of corresponding binary representations
    const int binary_values[] = {
      0b00001, 0b00010, 0b00011, 0b00100, 0b00101, 0b00110, 0b00111, 0b01000, 0b01001, 0b01010,
      0b01011, 0b01100, 0b01101, 0b01110, 0b01111, 0b10000, 0b10001, 0b10010, 0b10011, 0b10100,
      0b10101, 0b10110, 0b10111, 0b11000, 0b11001, 0b11010, 0b11011, 0b11100, 0b11101, 0b11110
    };

    // Loop through registers and add to hashmap
    for (int i = 0; i < 30; i++) {
        hashmap_set(map, &(struct comp_to_binary){ .comp = registers[i], .binary = binary_values[i] });
    }
}

//creates a hashmap that intializes all of the components
//mapped to their binary representations

int initialize_hashmap() {
  map = hashmap_new(sizeof(struct comp_to_binary), 0, 0, 0, 
                                     comp_hash, compare_comps, NULL, NULL);

  add_registers_to_map(map);

  //Basic Math Instructions
  hashmap_set(map, &(struct comp_to_binary){ .comp="add", .binary=0b11000 }); 
  hashmap_set(map, &(struct comp_to_binary){ .comp="addi", .binary=0b11001 });
  hashmap_set(map, &(struct comp_to_binary){ .comp="sub", .binary=0b11010 });  
  hashmap_set(map, &(struct comp_to_binary){ .comp="subi", .binary=0b11011 }); 
  hashmap_set(map, &(struct comp_to_binary){ .comp="mul", .binary=0b11100 }); 
  hashmap_set(map, &(struct comp_to_binary){ .comp="div", .binary=0b11101 }); 

  // Logic Instructions
  hashmap_set(map, &(struct comp_to_binary){ .comp="and", .binary=0b00000 }); 
  hashmap_set(map, &(struct comp_to_binary){ .comp="or", .binary=0b00001 }); 
  hashmap_set(map, &(struct comp_to_binary){ .comp="xor", .binary=0b00010 }); 
  hashmap_set(map, &(struct comp_to_binary){ .comp="not", .binary=0b00011 }); 
  hashmap_set(map, &(struct comp_to_binary){ .comp="shftr", .binary=0b00100 }); 
  hashmap_set(map, &(struct comp_to_binary){ .comp="shftri", .binary=0b00101 }); 
  hashmap_set(map, &(struct comp_to_binary){ .comp="shftl", .binary=0b00110 }); 
  hashmap_set(map, &(struct comp_to_binary){ .comp="shftli", .binary=0b00111 }); 

}


bool is_valid_12bit_unsigned(long val) {
    return val >= 0 && val < 4096;
}

bool parse_literal(const char *token, int *out_val) {
    char *endptr;

    if (strncmp(token, "0b", 2) == 0 || strncmp(token, "0B", 2) == 0) {
        // Binary (manual parse)
        int val = 0;
        for (const char *p = token + 2; *p; p++) {
            if (*p != '0' && *p != '1') return false;
            val = (val << 1) | (*p - '0');
        }
        if (val > 2047) val -= 4096; // convert to signed 12-bit if needed
        *out_val = val;
        return is_valid_12bit_unsigned(val);
    } else {
        // Hex or decimal (auto-detected base)
        int val = strtol(token, &endptr, 0);
        if (*endptr != '\0') return false;
        *out_val = val;
        return is_valid_12bit_unsigned(val);
    }
}

void printBinary(int n) {
    for (int i = 31; i >= 0; i--) {
        int bit = (n >> i) & 1;
        printf("%d", bit);
    }
    printf("\n");
}


//needed this function when hashmap def contained char pointers
//realized that it was pointless, we can just store at uint8_t 
//regardless, kept for safekeeping
void binary_parse(char *token,  int *out_val) {
  int val = 0;
  for (const char *p = token; *p; p++) {
    if (*p != '0' && *p != '1') return false;
      val = (val << 1) | (*p - '0');
    }

  *out_val = val;
}



void create_instruction(char *str, int *instruction_val) {
  char *token; 


  //this token must be a opcode 
  token = strtok(str, " ");


  if(strcmp(token, "add") == 0 || strcmp(token, "sub") == 0) {

    struct comp_to_binary *comp_test; 
    int comp_binary; 

    int shift_val = 27; 

    for(int i = 0; i < 4; i++) {

      if(token == NULL) {
        printf("Error: Invalid Instruction - too few arguments");
        *instruction_val = 0; 
        return; 
      }
      size_t len = strlen(token);
      if (len > 0 && token[len - 1] == ',') {
        token[len - 1] = '\0';  // Remove the comma
      }

      comp_test = hashmap_get(map, &(struct comp_to_binary){ .comp=token });
      if(comp_test == NULL) {
        printf("Error: Unrecognized Instruction Component %s\n", token);
        *instruction_val = 0;
        return;  
      }
      *instruction_val = *instruction_val | (comp_test->binary << shift_val); 

      shift_val -= 5; 

      // Get the next token
      token = strtok(NULL, " ");

    }

    if(token != NULL) {
      printf("Error: Invalid Instruction - too many arguments");
      *instruction_val = 0;  
      return; 
    }

  } else if(strcmp(token, "addi") == 0) {

    struct comp_to_binary *comp_test; 
    int shift_val = 27; 

    comp_test = hashmap_get(map, &(struct comp_to_binary){ .comp=token });
    if(comp_test == NULL) {
      printf("Error: Unrecognized Instruction Component %s\n", token);
      *instruction_val = 0;
      return;  
    }

    *instruction_val = *instruction_val | (comp_test->binary << shift_val); 
    shift_val -= 5; 

    token = strtok(NULL, " "); 


    if(token == NULL) {
      printf("Invalid Instruction - too few arguments"); 
      *instruction_val = 0; 
      return; 
    }

    size_t len = strlen(token);
    if (len > 0 && token[len - 1] == ',') {
      token[len - 1] = '\0';  // Remove the comma
    }

    comp_test = hashmap_get(map, &(struct comp_to_binary){ .comp=token });
    if(comp_test == NULL) {
      printf("Error: Unrecognized Instruction Component %s\n", token);
      *instruction_val = 0;
      return;  
    }

    *instruction_val = *instruction_val | (comp_test->binary << shift_val); 

    token = strtok(NULL, " "); 

    if(token == NULL) {
      printf("Invalid Instruction - too few arguments"); 
      *instruction_val = 0; 
      return; 
    }

    int literal; 

    if(parse_literal(token, &literal)) {
      *instruction_val = *instruction_val | (literal); 
    } else {
      printf("Error: Unsupported Literal"); 
      *instruction_val = 0; 
      return; 
    }


  } else {
    printf("Error: Unsupported Instruction"); 
    *instruction_val = 0; 
    return; 
  }
  

}


int write_int_to_object_file(const char *filename, int32_t value) {
    FILE *f = fopen(filename, "ab"); // append in binary mode
    if (!f) {
        perror("Failed to open file");
        return -1;
    }

    size_t written = fwrite(&value, sizeof(value), 1, f);
    fclose(f);

    return written == 1 ? 0 : -1;
}



//purpose of this file is to build a object file that can be executed 
int parse_file(const char* filename) {
  char object_filename[256];           // make sure buffer is large enough

  snprintf(object_filename, sizeof(object_filename), "%s.obj", filename);
  FILE *f = fopen(object_filename, "wb"); // "wb" = write binary
  if (!f) {
    perror("Failed to create file");
    return 1;
  }






  FILE *fptr = fopen(filename, "r");

  struct comp_to_binary *comp_test; 


  //read every line from the file 
  while(fgets((char *)memory, 100, fptr)) {

    //delete new line character 
    memory[strcspn(memory, "\n")] = '\0';
    int instruction_val = 0; 
    create_instruction(memory, &instruction_val);
    printBinary(instruction_val);
    if(instruction_val == 0) {
      printf("true");
      return; 
    }

    if (write_int_to_object_file(object_filename, instruction_val) != 0) {
        fprintf(stderr, "Failed to write instruction\n");
        return 1;
    }
  }



  fclose(fptr);
  fclose(f);
  
  

  
  return 0; 
}