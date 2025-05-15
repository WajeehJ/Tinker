#include <stdio.h>
#include <stdint.h>
#include "decode.h"
#include "../hashmap.h"


struct hashmap *map; 

int compare_comps(const void *a, const void *b, void *udata) {
    const struct binary_to_comp *ua = a;
    const struct binary_to_comp *ub = b;
    return ua->binary == ub->binary;
}

bool comp_iter(const void *item, void *udata) {
    const struct binary_to_comp *component = item;
    printf("%d (comp=%s) \n", component->binary, component->comp);
    return true;
}

uint64_t comp_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct binary_to_comp *component = item;
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
        hashmap_set(map, &(struct binary_to_comp){ .binary = binary_values[i], .comp = registers[i] });
    }
}

void initialize_hashmap_decode() {

}

void decode_instruction(uint32_t instruction) {
  //get opcode 

}

