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
TodoItemListToJSON(FILE *f, TodoItem* items, int len) {
	fprintf(f, "[\n");

	for (int i = 0; i < len - 1; i++) {
		TodoItemToJSON(f, items[i]);
		fprintf(f, ",\n");
	}
	TodoItemToJSON(f, items[len - 1]);
	fprintf(f, "\n]\n");
}
