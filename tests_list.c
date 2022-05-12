/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include "tests_list.h"

void test_list() {
	list* list = init_list();
	char* item_1 = "item1";
	char* item_2 = "item2";
	if (list != NULL) {
		assert("init_list_true", list->count == 0 && list->first_node == NULL);
		assert("init_list_true", list->count == 0 && list->first_node == NULL);
		assert("add_item_1_list", add_item(list, item_1) != NULL && list->count == 1 && list->first_node != NULL);
		assert("add_item_2_list", add_item(list, item_2) != NULL && list->count == 2 && ((node*)list->first_node)->next_node != NULL);
		free_list(list);
		assert("free_list", 1);
	}
	else {
		warning("Initializing list error");
	}

}