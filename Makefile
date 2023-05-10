CC=gcc
CFLAGS=-I. -Wall -Werror -g
DEPS=maze_maker.h windows_dir.h get_args.h queue.h imager.h
SRCS=maze_maker.c main.c windows_dir.c get_args.c queue.c imager.c
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
