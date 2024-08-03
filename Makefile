CC ?= gcc
CFLAGS=-I. -g
LIBS=-ld4t4-5tructur35 -lj50n -lut1l5 -lncurses

HEADER=organizer.h
SRC=organizer.c main.c
OBJ=$(SRC:.c=.o)
APP=0rg4n1z3r

.PHONY: all app check clean

all: app

app: $(APP)

check:
	$(CC) -fsyntax-only $(SRC) $(HEADER)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) 

$(APP): $(OBJ) 
	$(CC) $(if $(USE_SHARED),,-static) -o $@ $^ $(CFLAGS) $(LIBS) 

clean:
	rm -rf $(APP) $(OBJ)
