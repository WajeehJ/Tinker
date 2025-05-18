# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -Iinclude

# Output executable
TARGET = tinker_executable

# Root-level source/object files
SRCS = processor.c object_compiler.c hashmap.c
OBJS = $(SRCS:.c=.o)

# Subdirectory object files
PIPELINE_OBJS = pipeline/decode.o pipeline/fetch.o pipeline/pipe_base.o pipeline/writeback.o pipeline/execute.o

# Default rule
all: pipeline $(TARGET)

# Link everything into the final executable
$(TARGET): $(OBJS) $(PIPELINE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile root-level .c files into .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Trigger subdirectory build
pipeline:
	$(MAKE) -C pipeline

# Clean everything
clean:
	$(MAKE) -C pipeline clean
	rm -f $(OBJS) $(TARGET)
