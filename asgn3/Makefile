# Makefile for queue.c
CC = clang

CFLAGS=-Wall -Wextra -Werror -pedantic

all : queue.h queue.c
	clang -Wall -c queue.c

Queue.o : queue.h queue.c
	clang -Wall -c queue.c

clean :
	rm -f queue.o
