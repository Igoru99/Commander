/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include "tests_error.h"

void test_error() {
	ErrorInfo err = get_error();
	assert("get_empty_error", err.msg == NULL);
	assert("is_empty_error", !is_error());
	add_error(5, 1, "error");
	assert("is_full_error", is_error());
	err = get_error();
	assert("get_full_error", strcmp(err.msg, "error") == 0 && err.line == 5 && err.pos == 1);
}