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
			.done = 0
		},
	};
	int todoLen = 5;

	start_color();
	init_pair(HIGHLIGHTED_PAIR, COLOR_BLACK, COLOR_WHITE);
	keypad(stdscr, 1);
	
	int x, y;
	int ch;
	getyx(stdscr, y, x);

	printTodoItems(stdscr, todoItems, todoLen);
	while ((ch = getch()) != 'q') {
		switch(ch) {
		case KEY_DOWN:
		case 'k':
			y--;
			break;
		case KEY_UP:
		case 'j':
			y++;
			break;
		}
		y = MAX(0, (MIN(y, todoLen - 1)));
		wmove(stdscr, y, x);
		printTodoItems(stdscr, todoItems, todoLen);
	}
	endwin();
	return 0;
}


