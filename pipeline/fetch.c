#include <stdint.h> 
#include "../processor.h" 
#include "pipe_base.h"



uint32_t fetch_instruction() {
  decode_stage.instruction = read_word(program_counter); 
  program_counter += 4; 
}