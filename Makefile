CC=gcc
CFLAGS=-I. -g

0rg4n1z3r: main.o todo_item.o d4t4-5tructur35/array_list.o
	$(CC) -o 0rg4n1z3r main.o todo_item.o d4t4-5tructur35/array_list.o -g -lncurses
