#include "todo_item.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

// TODO: move to some util library
static char*
readFileToString(char *filename, int *len) {
	// TODO: use custom buffer with length
	char* buffer;

	FILE *f = fopen(filename, "rb");

	if (!f) {
		// TODO: print error
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	*len = ftell(f);
	fseek(f, 0, SEEK_SET);

	buffer = malloc(*len);

	int fd = fileno(f);

	void *mapping = mmap(0, *len, PROT_READ, MAP_PRIVATE, fd, 0);
	memcpy(buffer, mapping, *len);
	munmap(mapping, *len);
	fclose(f);

	return buffer;
}

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

ArrayList*
TodoItemListFromJSON(FILE* f) {

}
