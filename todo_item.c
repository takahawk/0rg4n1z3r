#include "todo_item.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

// TODO: move to some util library

static int
removeWhitespaces(char *buffer, int len) {
	int toSkip;
	for (int i = 0; i < len; i++) {
		switch (buffer[i]) {
		case ' ':
		case '\n':
		case '\t':
			toSkip++;
			break;
		default:
			if (toSkip != 0)
				buffer[i - toSkip] = buffer[i];
		}
	}

	if (toSkip != 0)
		buffer[len - toSkip] = '\0';

	return toSkip;
}

static char*
readFile(FILE *f, int *len) {
	// TODO: use custom buffer with length
	char* buffer;

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
	// TODO: implement some parsing combinator to refactor?
	// 0. read file to string
	ArrayList* result = AllocArrayList(sizeof(TodoItem), 10);	
	int len;
	char *json = readFile(f, &len);
	int i = 0;

	len -= removeWhitespaces(json, len);
	//TODO: impl rest

}
