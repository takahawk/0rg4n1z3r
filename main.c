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

ArrayList*
loadTodoItemsFromJSON(char *filename) {
	FILE *f = fopen(filename, "rb");

	if (f == NULL) {
		printf("Failed to open file: %s", filename);
		return NULL;
	}

	ArrayList* list = TodoItemListFromJSON(f);
	fclose(f);
	return list;
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

	ArrayList *todoItems = loadTodoItemsFromJSON("samples/todo.json");
	if (todoItems == NULL) {
		return -1;
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
		y = MAX(0, (MIN(y, todoItems->len - 1)));
		wmove(stdscr, y, x);
		printTodoItems(stdscr, todoItems);
	}
	endwin();

	saveTodoItemsToJSON("samples/todo.json", todoItems);
	FreeArrayList(todoItems);
	return 0;
}


