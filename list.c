/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include "list.h"

list* init_list() {
	list* new_list = malloc(sizeof(list));
	if (new_list == NULL)
		return NULL;
	new_list->first_node = NULL;
	new_list->count = 0;
	return new_list;
}

void free_list(list* list) {
	if (list == NULL)
		return;
	node* next_node = list->first_node;
	node* temp = NULL;
	for (ushort i = 0; i < list->count; i++) {
		temp = next_node;
		next_node = next_node->next_node;
		free(temp);
	}
	free(list);
}

node* add_item(list* list, void* value) {
	if (list == NULL || value == NULL || list->count == USHRT_MAX)
		return NULL;
	node* last_node = list->first_node;
	for (ushort i = 0; i < list->count-1; i++)
		last_node = last_node->next_node;
	node* new_node = malloc(sizeof(node));
	if (new_node == NULL)
		return NULL;
	new_node->value = value;
	if (list->count == 0) {
		list->first_node = new_node;
	}
	else {
		last_node->next_node = new_node;
	}
	list->count++;
	return new_node;
}