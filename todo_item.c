#include "todo_item.h"

#include <stdio.h>

void
TodoItemToJSON(FILE *f, TodoItem item) {
	// TODO: escape quotes!
	fprintf(f, "{ \"text\": \"%s\", \"done\": %s }", 
		   item.text, 
		   item.done ? "true" : "false");
}

void
TodoItemListToJSON(FILE *f, ArrayList* items) {
	fprintf(f, "[\n");

	for (int i = 0; i < items->len - 1; i++) {
		TodoItem* item = ArrayListGetTodoItem(items, i);
		TodoItemToJSON(f, *item);
		fprintf(f, ",\n");
	}
	TodoItemToJSON(f, *ArrayListGetTodoItem(items, items->len - 1));
	fprintf(f, "\n]\n");
}
