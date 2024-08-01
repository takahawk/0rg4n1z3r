CC ?= gcc
CFLAGS=-I. -g
LIBS=-ld4t4-5tructur35 -lncurses

SRC=todo_item.c main.c
OBJ=$(SRC:.c=.o)
APP=0rg4n1z3r

.PHONY: all app clean

all: app

app: $(APP)

%.o: %.c
	$(CC) -o $@ $< $(CFLAGS) $(LIBS)

$(APP): $(OBJ) 
	$(CC) $(if $(USE_SHARED),,-static) -o $@ $(CFLAGS) $(LIBS) 

clean:
	rm -rf $(APP) $(OBJ)
