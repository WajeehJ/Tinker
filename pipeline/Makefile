# pipeline/Makefile

CC = gcc
CFLAGS = -I../include -Wall -Wextra -O2

# Source and object files
SRCS = decode.c fetch.c execute.c memory.c pipe_base.c writeback.c
OBJS = $(SRCS:.c=.o)

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
