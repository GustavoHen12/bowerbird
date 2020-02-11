CC=gcc
CFLAGS= -ansi -Wall -Wextra -lncurses -O3
DEPS = lib_tela.h lib_dados 

all: lib_tela.o lib_dados.o 
	$(CC) -o main main.c lib_tela.o lib_dados.o $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


