#include "organizer.h"

#define TODO_START_CAPACITY 10

Organizer
O_Alloc() {
	Organizer organizer;
	organizer.todoList = AL_Alloc(sizeof(TodoItem, TODO_START_CAPACITY));
	return organizer;
}

Organizer
O_FromJSON(String json) {
	// TODO: impl
}

String
O_ToJSON(Organizer o) {
	JSONArray ja = JA_Alloc();
	for (int i = 0; i < o.todoList.len; i++) {
		TodoItem todo = O_GetTodo(o.todoList, i);
		JSONObject jo = JO_Alloc();
		JA_SetValue(&jo, S_Wrap("text", (JSONValue) {
					.type = JSON_STRING,
					.str = todo.text
					}));
		JA_SetValue(&jo, S_Wrap("done", (JSONValue) {
					.type = JSON_BOOL,
					.boolean = todo.done
					}));
		JO_AddValue(&ja, (JSONValue) {
				.type = JSON_OBJECT,
				.object = jo
				});
	}

	Strinb result = JV_Stringify((JSONValue) {
			.type = JSON_ARRAY,
			.array = ja
			};
	JA_Free(&ja);
	return result;
}
