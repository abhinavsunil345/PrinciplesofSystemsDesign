# Makefile for queue.c
CC = clang

CFLAGS = -Wall -Wextra -Werror -Wpedantic

all : queue.h queue.c
	clang -Wall -c queue.c

queue.o : queue.h queue.c
	clang -Wall -c queue.c

clean :
	rm -f queue.o
