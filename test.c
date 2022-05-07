/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include "test.h"

ushort _COUNT = 0;
ushort _MAX_COUNT = 0;
bool _is_warning = false;

void assert(const char *const test_name, bool expression) {
	if (expression == false) {
		printf("[ERROR] Test '%s' failed.\n", test_name);
	}
	else {
		printf("[SUCCESS] Test '%s' passed.\n", test_name);
		_COUNT++;
	}
	_MAX_COUNT++;
}

ushort get_test_count() {
	return _MAX_COUNT;
}

ushort get_true_test_count() {
	return _COUNT;
}

void warning(const char* const msg) {
	printf("[WARNING] %s.\n", msg);
	_is_warning = true;
}

bool is_warning() {
	return _is_warning;
}