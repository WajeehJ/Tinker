#ifndef OBJECTCOMPILER_H
#define OBJECTCOMPILER_H


//translate a "component" basically a part of an instruction 
//into its corresponding binary 
struct comp_to_binary {
  char *comp; 
  uint8_t binary; 
};

int initialize_hashmap();

int parse_file(const char *file_name);

#endif