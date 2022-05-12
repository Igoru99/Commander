/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#ifndef LIST_H

#define LIST_H
#include <stdlib.h>
#include <limits.h>
#include "globals.h"

typedef struct {
	void* next_node;
	void* value;
}node;

typedef struct {
	void* first_node;
	ushort count;
}list;

list* init_list();

void free_list(list* list);

node* add_item(list* list, void* value);

#endif