#ifndef ORGANIZER_H_
#define ORGANIZER_H_

#include <stdbool.h>

#include "w/d4t4-5tructur35/array_list.h"
#include "w/d4t4-5tructur35/string_.h"

typedef struct {
	String text;
	bool done;
} TodoItem;

typedef struct {
	ArrayList todoList;
} Organizer;

Organizer
O_Alloc();

static inline void
O_AddTodo(Organizer*, TodoItem);

static inline TodoItem*
O_GetTodo(Organizer, int);

static inline void
O_AddTodo(Organizer *o, TodoItem ti) {
	AL_Add(o->todoList, &ti);
}

static inline TodoItem*
O_GetTodo(Organizer o, int i) {
	return (TodoItem *) AL_Get(o.todoList, i);
}


#endif
