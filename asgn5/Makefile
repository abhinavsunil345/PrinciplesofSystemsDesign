# Makefile for cacher.c
CC = clang

CFLAGS = -Wall -Wextra -Werror -pedantic

cacher : cacher.o
	clang -std=c17 -Wall -o cacher cacher.o

all : cacher.o
	clang -std=c17 -Wall -o cacher cacher.o

Cacher.o : cacher.c
	clang -std=c17 -Wall -c cacher.c

clean :
	rm -f cacher cacher.o
