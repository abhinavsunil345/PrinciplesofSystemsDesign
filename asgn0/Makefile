# Makefile for hello.c
CC = clang

CFLAGS = -Wall -Wextra -Werror -Wpedantic

hello : hello.o
	clang -std=c17 -Wall -o hello hello.o

all : hello.o
	clang -std=c17 -Wall -o hello hello.o

Hello.o : hello.c
	clang -std=c17 -Wall -c hello.c

clean :
	rm -f hello hello.o