/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#pragma warning(disable : 4996)
#include "settings.h"

typedef struct {
	char* str;
	short number;
	bool is_error;
}_Value_Result;

char* _get_str() {
	char* str = '\0';
	char c = '\0';
	while ((c = next_ch_file()) != '\'' && c != '\0') {
		str = append_char(str, c);
		if (str == NULL)
			return NULL;
	}
	if (c == '\0')
	{
		if (str != NULL)
			free(str);
		return NULL;
	}
	next_ch_file();
	return str;
}

short _get_number(char start_number) {
	char* str = NULL;
	char c = '\0';
	str = append_char(str, start_number);
	if (str == NULL)
		return 0;
	while ((c = next_ch_file()) != '\n' && c != '\0') {
		str = append_char(str, c);
		if (str == NULL)
			return 0;
	}
	short number = atoi(str);
	free(str);
	return number;
}

char* _get_key() {
	char* str = NULL;
	char c = '\0';
	while ((c = next_ch_file()) != '=' && c != '\0') {
		str = append_char(str, c);
		if (str == NULL)
			return NULL;
	}
	if (c == '\0')
	{
		if (str != NULL)
			free(str);
		return NULL;
	}
	return str;
}

_Value_Result _get_value() {
	_Value_Result result;
	result.is_error = true;
	result.str = NULL;
	result.number = 0;
	char c = next_ch_file();
	if (c == '\'')
	{
		result.str = _get_str();
		if (result.str != NULL) {
			result.is_error = false;
		}
	}
	else if (c != '\0' && c != '\n')
	{
		result.number = _get_number(c);
		result.is_error = false;
	}
	return result;
}

bool _parse_line(dict* dict) {
	char* key = NULL;
	_Value_Result value;
	key = _get_key();
	if (key == NULL)
		return false;
	value = _get_value();
	if (value.is_error) {
		free(key);
		return false;
	}
	bool result = false;
	if (value.str != NULL)
	{
		result = add_sitem(dict, key, value.str);
		free(value.str);
	}
	else
		result = add_iitem(dict, key, value.number);
	free(key);
	return result;
}

dict* _parse_file() {
	dict* dict = init_dict();
	if (dict == NULL)
		return NULL;
	while (_parse_line(dict) != false) {
	}
	return dict;
}

dict* get_settings() {
	const char* path = GLOBAL_VARS_FILE;
	if (init_text_driver_from_file(path, "r") == false)
		return NULL;
	dict* dict = _parse_file();
	return dict;
}

char* _item_to_str(item* item, bool is_last_item) {
	char* valueS = NULL;
	if (item->value_type == DICT_DATATYPE_STRING)
		valueS = item->value;
	if (item == NULL)
		return NULL;
	char* result = NULL;
	for (ushort i = 0; i < strlen(item->key); i++)
	{
		result = append_char(result, item->key[i]);
	}
	result = append_char(result, '=');
	char* value = NULL;
	if (item->value_type == DICT_DATATYPE_STRING)
	{
		value = _strdup(item->value);
	}
	else
	{
		value = malloc(short_len_bytes * sizeof(char));
		if (value == NULL)
		{
			free(result);
			return NULL;
		}
		value = itoa(*(short*)item->value, value, 10);
	}
	if (item->value_type == DICT_DATATYPE_STRING)
		result = append_char(result, '\'');
	for (ushort i = 0; i < strlen(value); i++)
	{
		result = append_char(result, value[i]);
	}
	if (item->value_type == DICT_DATATYPE_STRING)
		result = append_char(result, '\'');
	if(!is_last_item)
	result = append_char(result, '\n');
	result = append_char(result, '\0');
	if (value != NULL)
		free(value);
	return result;
}

bool set_settings(dict* dict) {
	if (dict == NULL)
		return false;
	if (!init_text_driver_from_file(GLOBAL_VARS_FILE, "w"))
		return false;
	ushort count = 0;
	item* i = dict->first_ptr;
	while (count != dict->count) {
		if (!write_string_to_file(_item_to_str(i, count == dict->count - 1)))
			return false;
		i = i->next_ptr;
		count++;
	}
	return true;
}