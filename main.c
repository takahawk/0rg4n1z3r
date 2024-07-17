#include <ncurses.h>

int main() {
	initscr();
	mvwprintw(stdscr, 0, 0, "TODO: TODO list");
	getch();
	endwin();
	return 0;
}
