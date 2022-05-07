/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include "tests_dict.h"

void test_dict() {
	dict* dict = init_dict();
	assert("create_dict", dict != NULL);
	char* test_key_null = NULL;
	char* test_key_1 = "key1";
	char* test_key_2 = "key2";
	assert("remove_item_empty_dict", !remove_item(dict, test_key_1));
	assert("find_item_empty_dict", find_item(dict, test_key_1) == NULL);
	assert("add_null_key_dict_i", !add_iitem(dict, test_key_null, 0));
	assert("add_test_item_1_dict", add_iitem(dict, test_key_1, 0) != NULL && dict->count == 1);
	assert("find_item_1_dict", find_item(dict, test_key_1) != NULL && *(short*)find_item(dict, test_key_1)->value == 0);
	assert("not_find_item_2_dict", find_item(dict, test_key_2) == NULL);
	assert("add_test_item_2_dict", add_sitem(dict, test_key_2, "1") != NULL && dict->count == 2);
	assert("find_item_2_dict", find_item(dict, test_key_2) != NULL && strcmp((char*)find_item(dict, test_key_2)->value, "1") == 0);
	assert("remove_item_1_dict", remove_item(dict, test_key_1) && dict->count == 1);
	assert("find_item_2_dict", find_item(dict, test_key_2) != NULL && strcmp((char*)find_item(dict, test_key_2)->value, "1") == 0);
	assert("remove_item_2_dict", remove_item(dict, test_key_2) && dict->count == 0);
	free_dict(dict);
}