CC=gcc
CFLAGS=-I. -Wall -Werror -g
DEPS=maze_maker.h
SRCS=main.c windows_dir.c get_args.c
OBJS=$(SRCS:.c=.o)
EXEC=maze_maker.exe

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	del $(OBJS) $(EXEC)
