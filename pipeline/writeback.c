#include "writeback.h" 
#include "../processor.h"
#include "pipebase.h"


void writeback() {
  if(writeback_stage.write) {
    registers[writeback_stage.register_to_writeback] = writeback_stage.val; 
  }
}





