/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include "tests.h"

void test_all() {
	printf("-> Running tests...\n");
	test_error();
	test_dict();
	test_utilites();
	test_settings();
	test_text_driver();
	test_lexer();
	test_list();
	ushort max_count = get_test_count();
	ushort count = get_true_test_count();
	printf("-> performed %u tests from %u.\n", count, max_count);
	if (count != max_count)
		printf("-> Tests failed.\n");
	else if(!is_warning())
		printf("-> Tests passed successfully.\n");
	else
		printf("-> Tests passed with warnings.\n");
}