CC=gcc
CFLAGS=-I.

0rg4n1z3r: main.o todo_item.o
	$(CC) -o 0rg4n1z3r main.o todo_item.o -lncurses
