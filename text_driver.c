/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#pragma warning(disable : 4996)

#include "text_driver.h"

static char* _str = NULL;
static ushort _pos = 0;
static FILE* _file = NULL;
static int _file_len = 0;

char next_ch_file() {
	char c = '\0';
	if (_file == NULL)
	{
		return c;
	}
	else
	{
		if (_file_len >= MAX_FILE_LEN || (c = getc(_file)) == EOF) {
			free_text_driver_from_file();
			c = '\0';
		}
		else {
			_file_len++;
		}
		return c;
	}
}

char next_ch_stdin() {
	if (_str == NULL)
		return '\0';
	if (_str[_pos] != '\0')
		return _str[_pos++];
	else
		return '\0';
}

bool write_string_to_file(char* str) {
	if (str == NULL || _file == NULL || fputs(str, _file) == EOF)
		return false;
	else
		return true;
}

bool init_text_driver_from_file(const char* path, const char* mode) {
	if (path == NULL || mode == NULL)
		return false;
	if(_file != NULL)
		free_text_driver_from_file();
	_file = fopen(path, mode);
	if (_file == NULL)
		return false;
	return true;
}

bool init_text_driver_from_stdin(char* in) {
	if (in == NULL)
		return false;
	_str = in;
	_pos = 0;
	return true;
}

void free_text_driver_from_file() {
	if (_file != NULL)
	fclose(_file);
	_file = NULL;
	_file_len = 0;
}