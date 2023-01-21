# Makefile for memory.c
CC = clang

CFLAGS = -Wall -Wextra -Werror -Wpedantic

memory : memory.o
	clang -std=c17 -Wall -o memory memory.o

all : memory.o
	clang -std=c17 -Wall -o memory memory.o

memory.o : memory.c
	clang -std=c17 -Wall -c memory.c

clean :
	rm -f memory memory.o
