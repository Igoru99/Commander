/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include "utilites.h"

short* copy_int(short value) {
	short* ptr = malloc(sizeof(short));
	if (ptr == NULL)
		return NULL;
	*ptr = value;
	return ptr;
}

char* copy_str(char* value) {
	return _strdup(value);
}

char* append_char(char* string, char c)
{
	ushort new_len = 0;
	if (string == NULL)
		new_len = 2;
	else
		new_len = strlen(string) + 2;
	char* result = malloc(new_len * sizeof(char));
	if (result != NULL)
	{
		for (ushort i = 0; i < new_len - 2; i++)
			result[i] = string[i];
		result[new_len - 2] = c;
		result[new_len - 1] = '\0';
		if (string != NULL)
			free(string);
	}
	else {
		if (string != NULL)
			free(string);
	}
	return result;
}