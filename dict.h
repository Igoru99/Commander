/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#ifndef DICT_H

#define DICT_H
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "globals.h"
#include "utilites.h"

enum DICT_DATATYPES {
	DICT_DATATYPE_SHORT,
	DICT_DATATYPE_STRING
};

typedef struct{
	void* next_ptr;
	void* value;
	const char* key;
	enum DICT_DATATYPES value_type;
}item;

typedef struct {
	item* first_ptr;
	item* last_ptr;
	ushort count;
} dict;

dict* init_dict();

void free_dict(dict* dict);

item* find_item(dict* dict, const char* key);

item* add_iitem(dict* dict, const char* key, short value);

item* add_sitem(dict* dict, const char* key, char* value);

bool remove_item(dict* dict, const char* key);

#endif