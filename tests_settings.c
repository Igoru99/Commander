/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include "tests_settings.h"

void test_settings() {
	dict* dict = NULL;

	remove(GLOBAL_VARS_FILE);

	assert("get_settings_not_exists_file_vars", get_settings() == NULL);

	assert("set_settings_null_dict", !set_settings(dict));

	dict = init_dict();
	if (dict != NULL) {
		add_sitem(dict, "key1", "value1");
		add_iitem(dict, "key2", 0);
		assert("set_settings_dict", set_settings(dict));

		free_dict(dict);
		dict = get_settings();
		assert("get_settings_dict", dict != NULL && dict->count == 2 && find_item(dict, "key1") != NULL && strcmp((char*)find_item(dict, "key1")->value, "value1") == 0 && find_item(dict, "key2") != NULL && *(short*)find_item(dict, "key2")->value == 0);
		free_dict(dict);
		if (remove(GLOBAL_VARS_FILE) != 0)
			warning("Deleting test global vars settings file error");
	}
	else {
		warning("Initializing dict error");
	}
}