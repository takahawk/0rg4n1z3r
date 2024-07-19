#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>

#include <ncurses.h>

#include "todo_item.h"

#define HIGHLIGHTED_PAIR 1

typedef enum {
	TODO_LIST,
	ADD_NEW_TODO,
	SHUTTING_DOWN
} AppState;

void
printTodoItems(WINDOW *w, ArrayList *todoItems) {
	int curX, curY;
	getyx(w, curY, curX);
	for (int i = 0; i < todoItems->len; i++) {
		TodoItem *item = ArrayListGetTodoItem(todoItems, i);
		if (curY == i) {
			attron(COLOR_PAIR(HIGHLIGHTED_PAIR));
		}
		move(i, 0);
		clrtoeol();
		move(curY, curX);
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

AppState
todoList(ArrayList*);

AppState
addNewTodo(ArrayList*);

int main() {
	initscr();

	ArrayList *todoItems = loadTodoItemsFromJSON("samples/todo.json");
	if (todoItems == NULL) {
		return -1;
	}

	start_color();
	noecho();
	init_pair(HIGHLIGHTED_PAIR, COLOR_BLACK, COLOR_WHITE);
	keypad(stdscr, 1);
	
	AppState state = TODO_LIST;
	int x, y;
	int ch;

	while (state != SHUTTING_DOWN) {
		switch (state) {
		case TODO_LIST:
			state = todoList(todoItems);
			break;
		case ADD_NEW_TODO:
			state = addNewTodo(todoItems);
			break;
		}
	}
	endwin();

	saveTodoItemsToJSON("samples/todo.json", todoItems);
	FreeArrayList(todoItems);
	return 0;
}


AppState
todoList(ArrayList *todoItems) {
	wclear(stdscr);
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
		case 't':
			if (y > 0) {
				ArrayListSwap(todoItems, y, y - 1);
			}
			break;
		case 'a':
			return ADD_NEW_TODO;
		}
		y = MAX(0, (MIN(y, todoItems->len - 1)));
		wmove(stdscr, y, x);
		wrefresh(stdscr);
		printTodoItems(stdscr, todoItems);
	}

	return SHUTTING_DOWN;
}

AppState
addNewTodo(ArrayList *todoItems) {
	wclear(stdscr);

	int ch;
	int x, y;
	int i = 0;
	// TODO: use custom resizable buffers!
	char* buffer = malloc(1337); 

	getyx(stdscr, y, x);

	mvwprintw(stdscr, y, x, "Who do you need to do: %s", buffer);
	while ((ch = getch()) != 'q') {
		switch(ch) {
		case '\n':
		case KEY_ENTER:
			TodoItem item = {
				.text = buffer,
				.done = false
			};
			ArrayListAdd(todoItems, &item);
			goto end;
		case KEY_BACKSPACE:
			if (i == 0) {
				continue;
			}
			buffer[--i] = '\0';
			break;
		default:
			buffer[i++] = ch;
			buffer[i] = '\0';
		}
		wclear(stdscr);
		mvwprintw(stdscr, y, x, "Who do you need to do: %s", buffer);
	}
end:
	return TODO_LIST;
}
