#include <ncurses.h>

#include "todo_item.h"

void
printTodoItems(WINDOW *w, TodoItem *todoItems, int len) {
	for (int i = 0; i < len; i++) {
		mvwprintw(w, i, 0, todoItems[i].text);
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
			.done = 0
		}
	};
	printTodoItems(stdscr, todoItems, 2);
	getch();
	endwin();
	return 0;
}


