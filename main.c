#include <stdio.h>
#include <sys/param.h>

#include <ncurses.h>

#include "todo_item.h"

#define HIGHLIGHTED_PAIR 1

void
printTodoItems(WINDOW *w, ArrayList *todoItems) {
	int curX, curY;
	getyx(w, curY, curX);
	for (int i = 0; i < todoItems->len; i++) {
		TodoItem *item = ArrayListGetTodoItem(todoItems, i);
		if (curY == i) {
			attron(COLOR_PAIR(HIGHLIGHTED_PAIR));
		}
		mvwprintw(w, i, 0, "[%c] %s", item->done ? 'x' : ' ', item->text);
		if (curY == i) {
			attroff(COLOR_PAIR(HIGHLIGHTED_PAIR));
		}
	}
}

void
saveTodoItemsToJSON(char *filename, ArrayList *todoItems) {
	FILE *f = fopen(filename, "w");

	if (f == NULL) {
		printf("Failed to open file: %s", filename);
		return;
	}

	TodoItemListToJSON(f, todoItems);
	fclose(f);
}

int main() {
	initscr();

	ArrayList *todoItems = AllocArrayList(sizeof(TodoItem), 10);

	TodoItem todoItemsLegacy[] = {
		(TodoItem) {
			.text = "Add todo item structure",
			.done = 1
		},
		(TodoItem) {
			.text = "Add rendering with a checkbox",
			.done = 1
		},
		(TodoItem) {
			.text = "Add highlighting of selected point",
			.done = 1
		},
		(TodoItem) {
			.text = "Cursor moves",
			.done = 1
		},
		(TodoItem) {
			.text = "Implement toggling checkbox",
			.done = 1
		},
		(TodoItem) {
			.text = "Implement saving to JSON (simple version)",
			.done = 1
		},
		(TodoItem) {
			.text = "Implement loading from JSON (simple version)",
			.done = 0
		},
		(TodoItem) {
			.text = "Create separate repo for data structures",
			.done = 1
		},
		(TodoItem) {
			.text = "Use array list for todo items",
			.done = 1
		},
		(TodoItem) {
			.text = "Implement swapping adjacent elements",
			.done = 0
		},
		(TodoItem) {
			.text = "Implement adding new todo item",
			.done = 0
		}
	};
	int todoLen = 11;

	for (int i = 0; i < todoLen; i++) {
		ArrayListAddTodoItem(todoItems, todoItemsLegacy[i]);
	}

	start_color();
	init_pair(HIGHLIGHTED_PAIR, COLOR_BLACK, COLOR_WHITE);
	keypad(stdscr, 1);
	
	int x, y;
	int ch;
	getyx(stdscr, y, x);

	printTodoItems(stdscr, todoItems);
	while ((ch = getch()) != 'q') {
		switch(ch) {
		case KEY_UP:
		case 'k':
			y--;
			break;
		case KEY_DOWN:
		case 'j':
			y++;
			break;
		case ' ':
			TodoItem *item = ArrayListGetTodoItem(todoItems, y);
			item->done = !item->done;
			break;
		}
		y = MAX(0, (MIN(y, todoLen - 1)));
		wmove(stdscr, y, x);
		printTodoItems(stdscr, todoItems);
	}
	endwin();

	saveTodoItemsToJSON("samples/todo.json", todoItems);
	FreeArrayList(todoItems);
	return 0;
}


