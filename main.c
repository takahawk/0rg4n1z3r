#include <stdio.h>
#include <sys/param.h>

#include <ncurses.h>

#include "todo_item.h"

#define HIGHLIGHTED_PAIR 1

void
printTodoItems(WINDOW *w, TodoItem *todoItems, int len) {
	int curX, curY;
	getyx(w, curY, curX);
	for (int i = 0; i < len; i++) {
		if (curY == i) {
			attron(COLOR_PAIR(HIGHLIGHTED_PAIR));
		}
		mvwprintw(w, i, 0, "[%c] %s", todoItems[i].done ? 'x' : ' ', todoItems[i].text);
		if (curY == i) {
			attroff(COLOR_PAIR(HIGHLIGHTED_PAIR));
		}
	}
}

void
saveTodoItemsToJSON(char *filename, TodoItem *todoItems, int len) {
	FILE *f = fopen(filename, "w");

	if (f == NULL) {
		printf("Failed to open file: %s", filename);
		return;
	}

	TodoItemListToJSON(f, todoItems, len);
	fclose(f);
}

int main() {
	initscr();

	TodoItem todoItems[] = {
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
			.done = 0
		},
		(TodoItem) {
			.text = "Use array list for todo items in there",
			.done = 0
		}
	};
	int todoLen = 9;

	start_color();
	init_pair(HIGHLIGHTED_PAIR, COLOR_BLACK, COLOR_WHITE);
	keypad(stdscr, 1);
	
	int x, y;
	int ch;
	getyx(stdscr, y, x);

	printTodoItems(stdscr, todoItems, todoLen);
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
			todoItems[y].done = !todoItems[y].done;
			break;
		}
		y = MAX(0, (MIN(y, todoLen - 1)));
		wmove(stdscr, y, x);
		printTodoItems(stdscr, todoItems, todoLen);
	}
	endwin();

	saveTodoItemsToJSON("samples/todo.json", todoItems, todoLen);
	return 0;
}


