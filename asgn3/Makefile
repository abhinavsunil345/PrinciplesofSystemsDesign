# Makefile for queue.c
CC = clang

CFLAGS=-Wall -Wextra -Werror -Wpedantic

all : queue.h queue.c
	$(CC) $(CFLAGS) -c queue.c

queue.o : queue.h queue.c
	$(CC) $(CFLAGS) -c queue.c

clean :
	rm -f queue.o
