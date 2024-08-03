#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>

#include <ncurses.h>

#include "w/ut1l5/files.h"

#include "organizer.h"

#define HIGHLIGHTED_PAIR 1

typedef enum {
	TODO_LIST,
	ADD_NEW_TODO,
	SHUTTING_DOWN
} AppState;

void
printTodoItems(WINDOW *w, Organizer *o) {
	int curX, curY;
	getyx(w, curY, curX);
	for (int i = 0; i < o->todoList.len; i++) {
		TodoItem *item = O_GetTodo(*o, i);
		if (curY == i) {
			attron(COLOR_PAIR(HIGHLIGHTED_PAIR));
		}
		move(i, 0);
		clrtoeol();
		move(curY, curX);
		mvwprintw(w, i, 0, "[%c] %s", item->done ? 'x' : ' ', item->text.str);
		if (curY == i) {
			attroff(COLOR_PAIR(HIGHLIGHTED_PAIR));
		}
	}
}


AppState
todoList(Organizer*);

AppState
addNewTodo(Organizer*);

int main() {
	initscr();

	String json = S_From(uF_ReadFileByName("samples/todo.json"));
	Organizer o = O_FromJSON(json);
	S_Free(&json);

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
			state = todoList(&o);
			break;
		case ADD_NEW_TODO:
			state = addNewTodo(&o);
			break;
		case SHUTTING_DOWN:
			break;
		}
	}
	endwin();

	uF_WriteToFileByName("samples/todo.json", S_AsBuffer(O_ToJSON(o)));
	return 0;
}


AppState
todoList(Organizer *o) {
	wclear(stdscr);
	int x, y;
	int ch;
	getyx(stdscr, y, x);
	printTodoItems(stdscr, o);
	while ((ch = getch()) != 'q') {
		TodoItem *item;
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
			item = O_GetTodo(*o, y); 
			item->done = !item->done;
			break;
		case 't':
			if (y > 0) {
				AL_Swap(&o->todoList, y, y - 1);
			}
			break;
		case 'a':
			return ADD_NEW_TODO;
		}
		y = MAX(0, (MIN(y, o->todoList.len - 1)));
		wmove(stdscr, y, x);
		wrefresh(stdscr);
		printTodoItems(stdscr, o); 

	}
	return SHUTTING_DOWN;
}

AppState
addNewTodo(Organizer *o) {
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
				.text = S_From(buffer),
				.done = false
			};
			O_AddTodo(o, item);
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
