#include <stdio.h>
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
    printf("%s (binary=%s)\n", component->comp, component->binary);
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
    const char *binary_values[] = {
        "00001", "00010", "00011", "00100", "00101", "00110", "00111", "01000", "01001", "01010",
        "01011", "01100", "01101", "01110", "01111", "10000", "10001", "10010", "10011", "10100",
        "10101", "10110", "10111", "11000", "11001", "11010", "11011", "11100", "11101", "11110"
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

  hashmap_set(map, &(struct comp_to_binary){ .comp="add", .binary="00011000" }); 
  hashmap_set(map, &(struct comp_to_binary){ .comp="addi", .binary="00011001" });
  hashmap_set(map, &(struct comp_to_binary){ .comp="sub", .binary="00011010" });  

}

char *create_instruction(char *str) {
  char *token; 

  token = strtok(str, " "); 

  struct comp_to_binary *comp_test; 
  while (token != NULL) {
    
    // Check if the last character is a comma
    size_t len = strlen(token);
    if (len > 0 && token[len - 1] == ',') {
      token[len - 1] = '\0';  // Remove the comma
    }

    comp_test = hashmap_get(map, &(struct comp_to_binary){ .comp=token });
    printf("%s\n", comp_test->binary);

    // Get the next token
    token = strtok(NULL, " ");


  }

  return ""; 

}



//purpose of this file is to build a object file that can be executed 
int parse_file(const char* filename) {


  FILE *fptr = fopen(filename, "r");

  struct comp_to_binary *comp_test; 


  //read every line from the file 
  while(fgets((char *)memory, 100, fptr)) {

    //delete new line character 
    memory[strcspn(memory, "\n")] = '\0';

    create_instruction(memory);
  }



  fclose(fptr);
  
  

  
  return 0; 
}