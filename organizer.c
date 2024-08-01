#include "organizer.h"

#define TODO_START_CAPACITY 10

Organizer
O_Alloc() {
	Organizer organizer;
	organizer.todoList = AL_Alloc(sizeof(TodoItem, TODO_START_CAPACITY));
	return organizer;
}
