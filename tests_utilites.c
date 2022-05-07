/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include "tests_utilites.h"

void test_utilites() {
	short old_short = 5;
	short* new_short = copy_int(old_short);
	assert("copy_int", new_short != &old_short && *new_short == old_short);
	free(new_short);
	char* old_str = "I am human";
	char* new_str = copy_str(old_str);
	assert("copy_str", new_str != old_str && strcmp(old_str, new_str) == 0);
	free(new_str);
	new_str = NULL;
	new_str = append_char(new_str, '0');
	assert("append_char_to_null_str", strcmp(new_str, "0") == 0);
	new_str = append_char(new_str, 'q');
	assert("append_char_to_full_str", strcmp(new_str, "0q") == 0);
	free(new_str);
}