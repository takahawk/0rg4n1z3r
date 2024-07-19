#include "todo_item.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "d4t4-5tructur35/array_list.h"

// TODO: move to some util library

static int
removeUnquotedWhitespaces(char *buffer, int len) {
	int toSkip = 0;
	int quoted = 0;
	for (int i = 0; i < len; i++) {
		switch (buffer[i]) {
		case ' ':
		case '\n':
		case '\t':
			if (!quoted) {
				toSkip++;
				continue;
			}
			break;
		case '\"':
			quoted = !quoted;
		default:
		}
		if (toSkip != 0)
			buffer[i - toSkip] = buffer[i];
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
	// read file to string
	ArrayList* result = AllocArrayList(sizeof(TodoItem), 10);	
	int len;
	char *json = readFile(f, &len);

	// remove whitespace to simplify the rest
	len -= removeUnquotedWhitespaces(json, len);

	if (json[0] != '[') {
		// TODO: log error
		goto error;
	}

	int i = 1;
	while (json[i] == '{') {
		i++;
		int textSet = 0, doneSet = 0;
		TodoItem item;

		do {
			if (json[i++] != '\"') {
				// TODO: log error
				goto error;
			}
			if (strncmp(json + i, "text", 4) == 0) {
				i += 4; // text
				if (json[i++] != '\"') {
					goto error;
				}	
				if (json[i++] != ':') {
					goto error;
				}
				if (json[i++] != '\"') {
					goto error;
				}
				
				int j = 0;
				while (json[i + j] != '\"')
					// we don't expect newline here, check?
					j++;
				
				// TODO: memory-leak! add destructor to TodoItem
				item.text = malloc(j + 1);
				memcpy(item.text, json + i, j);
				item.text[j] = '\0';
				i += j + 1;
				textSet = 1;
			} else if (strncmp(json + i, "done", 4) == 0) {
				i += 4; // done
				if (json[i++] != '\"') {
					goto error;
				}	
				if (json[i++] != ':') {
					goto error;
				}
				
				if (strncmp(json + i, "true", 4) == 0) {
					item.done = 1;
					i += 4;
				} else if (strncmp(json + i, "false", 5) == 0) {
					item.done = 0;
					i += 5;
				}
				doneSet = 1;
			}
				
			if (textSet && doneSet) {
				if (json[i++] != '}') {
					goto error;
				}
				break;
			} 

			if (json[i++] != ',') {
				goto error;
			}
			continue;
		} while(1);

		
		ArrayListAdd(result, &item);
		if (json[i] == ',')
			i++;
	}

	if (json[i] != ']') {
		goto error;
	}

	free(json);
	return result;
error:
	free(json);
	free(result);
	return NULL;
}
