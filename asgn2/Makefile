# Makefile for httpserver.c
CC = clang

CFLAGS = -Wall -Wextra -Werror -pedantic

httpserver : httpserver.o
	clang -Wall -o httpserver httpserver.o asgn2_helper_funcs.a

all : httpserver.o
	clang -Wall -o httpserver httpserver.o asgn2_helper_funcs.a

httpserver.o : httpserver.c
	clang -Wall -c httpserver.c

clean :
	rm -f httpserver httpserver.o
