#ifndef TODO_ITEM_H_
#define TODO_ITEM_H_

#include <stdio.h>
#include "d4t4-5tructur35/array_list.h"

typedef struct {
	char *text;
	int done;
} TodoItem;

static inline TodoItem*
ArrayListGetTodoItem(ArrayList* al, int i) {
	return (TodoItem*) ArrayListGet(al, i);
}

static inline void
ArrayListAddTodoItem(ArrayList* al, TodoItem tl) {
	ArrayListAdd(al, &tl);
}

void
TodoItemToJSON(FILE*, TodoItem);

void
TodoItemListToJSON(FILE*, ArrayList*);

ArrayList*
TodoItemListFromJSON(FILE*);

#endif
