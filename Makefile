# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Output executable name
TARGET = tinker_executable

# Source files
SRCS = processor.c object_compiler.c

# Object files (automatically generated from source files)
OBJS = $(SRCS:.c=.o)

# Default rule: build the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object and executable files
clean:
	rm -f $(OBJS) $(TARGET)
