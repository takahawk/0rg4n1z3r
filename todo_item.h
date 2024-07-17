#ifndef TODO_ITEM_H_
#define TODO_ITEM_H_

#include <stdio.h>

typedef struct {
	char *text;
	int done;
} TodoItem;

void
TodoItemToJSON(FILE*, TodoItem);

void
TodoItemListToJSON(FILE*, TodoItem*, int);

#endif
