CC=gcc
CFLAGS=-I.

0rg4n1z3r: main.o
	$(CC) -o 0rg4n1z3r main.o -lncurses
