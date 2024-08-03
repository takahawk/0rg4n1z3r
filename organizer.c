#include "organizer.h"

#include "w/j50n/json.h"

#define TODO_START_CAPACITY 10

Organizer
O_Alloc() {
	Organizer organizer;
	organizer.todoList = AL_Alloc(sizeof(TodoItem), TODO_START_CAPACITY);
	return organizer;
}

Organizer
O_FromJSON(String json) {
	Organizer o = O_Alloc();
	JSONValue jv;
	JSON_Parse(S_AsBuffer(json), &jv);
	for (int i = 0; i < jv.array.al.len; i++) {
		JSONObject jo = JA_GetObject(i, jv.array);
		O_AddTodo(&o, (TodoItem) {
				.text = S_Copy(JO_GetString("text", jo)),
				.done = JO_GetBoolean("done", jo)
				});
	}

	JV_Free(&jv);
	return o;
}

String
O_ToJSON(Organizer o) {
	JSONArray ja = JA_Alloc();
	for (int i = 0; i < o.todoList.len; i++) {
		TodoItem *todo = O_GetTodo(o, i);
		JSONObject jo = JO_Alloc();
		JO_SetValue(&jo, S_Wrap("text"), (JSONValue) {
					.type = JSON_STRING,
					.str = todo->text
					});
		JO_SetValue(&jo, S_Wrap("done"), (JSONValue) {
					.type = JSON_BOOL,
					.boolean = todo->done
					});
		JA_AddValue(&ja, (JSONValue) {
				.type = JSON_OBJECT,
				.object = jo
				});
	}

	String result = S_From(JV_Stringify((JSONValue) {
			.type = JSON_ARRAY,
			.array = ja
			}));
	JA_Free(&ja);
	return result;
}
