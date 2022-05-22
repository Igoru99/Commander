/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include "dict.h"

/*Create and destroy dicts*/

dict* init_dict() {
	dict* new_dict = malloc(sizeof(dict));
	if (new_dict == NULL)
		return NULL;
	new_dict->first_ptr = NULL;
	new_dict->last_ptr = NULL;
	new_dict->count = 0;
	return new_dict;
}

static void _free_item(item* current_item) {
	free(current_item->value);
	free(current_item->key);
	free(current_item);
}

void free_dict(dict* dict) {
	if (dict == NULL)
		return;
	if (dict->count != 0) {
		item* current_item = dict->first_ptr;
		item* next_item = current_item->next_ptr;
		while (true) {
			_free_item(current_item);
			dict->count--;
			if (dict->count == 0)
				break;
			current_item = next_item;
			next_item = next_item->next_ptr;
		}
	}
	free(dict);
}

/*Find item*/

item* find_item(dict* dict, const char* key) {
	if (dict == NULL || key == NULL || dict->count == 0)
		return NULL;
	int count = 0;
	item* i = dict->first_ptr;
	while (count != dict->count) {
		if (strcmp(i->key, key) == 0) {
			return i;
		}
		i = i->next_ptr;
		count++;
	}
	return NULL;
}

/*Add item*/

static item* _init_item(const char* key, void* value, enum DICT_DATATYPES data_type) {
	item* new_pair = malloc(sizeof(item));
	if (new_pair == NULL)
		return NULL;
	new_pair->key = key;
	new_pair->value = value;
	new_pair->next_ptr = NULL;
	new_pair->value_type = data_type;
	return new_pair;
}

static void _insert_item(dict* dict, item* new_pair) {
	if (dict->count == 0) {
		dict->first_ptr = new_pair;
		dict->last_ptr = new_pair;
		new_pair->next_ptr = new_pair;
	}
	else {
		new_pair->next_ptr = dict->first_ptr;
		dict->last_ptr->next_ptr = new_pair;
		dict->last_ptr = new_pair;
	}
	dict->count++;
}

item* add_iitem(dict* dict, const char* key, short value) {
	if (dict == NULL || key == NULL || dict->count == USHRT_MAX)
		return NULL;
	char* new_key = copy_str(key);
	if (new_key == NULL)
		return NULL;
	short* new_value = copy_int(value);
	if (new_value == NULL)
	{
		free(new_key);
		return NULL;
	}
	item* new_pair = _init_item(new_key, new_value, DICT_DATATYPE_SHORT);
	if (new_pair != NULL) {
		_insert_item(dict, new_pair);
	}
	else {
		free(new_key);
		free(new_value);
	}
	return new_pair;
}

item* add_sitem(dict* dict, const char* key, char* value) {
	if (dict == NULL || key == NULL || dict->count == USHRT_MAX)
		return NULL;
	char* new_key = copy_str(key);
	if (new_key == NULL)
		return NULL;
	char* new_value = copy_str(value);
	if (new_value == NULL)
	{
		free(new_key);
		return NULL;
	}
	item* new_pair = _init_item(new_key, new_value, DICT_DATATYPE_STRING);
	if (new_pair != NULL) {
		_insert_item(dict, new_pair);
	}
	else {
		free(new_key);
		free(new_value);
	}
	return new_pair;
}

/*Remove item*/

bool remove_item(dict* dict, const char* key) {
	if (dict == NULL || key == NULL || dict->count == 0)
		return false;
	if (dict->count == 1) {
		if(strcmp(dict->first_ptr->key, key) == 0){
			_free_item(dict->first_ptr);
			dict->first_ptr = NULL;
			dict->last_ptr = NULL;
			dict->count = 0;
			return true;
		}
		return false;
	}
	int count = 0;
	item* current_item = dict->first_ptr;
	item* last_item = dict->last_ptr;
	while (count != dict->count) {
		if (strcmp(current_item->key, key) == 0) {
			if (current_item == dict->first_ptr)
				dict->first_ptr = current_item->next_ptr;
			if (current_item == dict->last_ptr)
				dict->last_ptr = last_item;
			last_item->next_ptr = current_item->next_ptr;
			_free_item(current_item);
			dict->count--;
			return true;
		}
		last_item = current_item;
		current_item = current_item->next_ptr;
		count++;
	}
	return false;
}