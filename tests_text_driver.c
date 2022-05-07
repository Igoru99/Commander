/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#pragma warning(disable : 4996)
#include "tests_text_driver.h"

bool _create_test_file_1() {
	FILE* file = fopen(TEST_FILE_1, "w");
	if (file == NULL)
		return false;
	if (fputs("1", file) == EOF)
		return false;
	fclose(file);
	return true;
}

bool _create_test_file_2() {
	FILE* file = fopen(TEST_FILE_2, "w");
	if (file == NULL)
		return false;
	if (fputs("0", file) == EOF)
		return false;
	fclose(file);
	return true;
}

bool _create_test_files() {
	return _create_test_file_1() && _create_test_file_2();
}

bool _remove_test_files() {
	return remove(TEST_FILE_1) == 0 && remove(TEST_FILE_2) == 0 && remove(TEST_FILE_3) == 0;
}

void test_text_driver() {
	if (!_create_test_files())
	{
		warning("Creating test files error");
		return;
	}

	assert("next_ch_stdin_null_str", next_ch_stdin() == '\0');
	assert("init_text_driver_from_file_null_path", !init_text_driver_from_file(NULL, "r"));
	assert("init_text_driver_from_file_null_path_and_null_mode", !init_text_driver_from_file(NULL, NULL));
	assert("init_text_driver_from_file_invalid_path", !init_text_driver_from_file("\\\\\\\\", "r"));

	assert("init_text_driver_from_file_get_file", init_text_driver_from_file(TEST_FILE_1, "r"));

	assert("next_ch_file_get_char_1", next_ch_file() == '1');
	assert("next_ch_file_get_EOF_file", next_ch_file() == '\0');

	assert("init_text_driver_from_stdin_null_str", !init_text_driver_from_stdin(NULL));
	assert("init_text_driver_from_stdin_empty_str", init_text_driver_from_stdin("")  && next_ch_stdin() == '\0');

	assert("init_text_driver_from_stdin_full_str", init_text_driver_from_stdin("1"));
	assert("next_ch_stdin_char_1", next_ch_stdin() == '1');
	assert("next_ch_stdin_char_2", next_ch_stdin() == '\0');

	assert("init_text_driver_from_file_get_file_2", init_text_driver_from_file(TEST_FILE_2, "r"));
	free_text_driver_from_file();
	assert("free_file", next_ch_file() == '\0');

	assert("init_text_driver_from_file_create_file", init_text_driver_from_file(TEST_FILE_3, "w"));
	assert("write_line_text_driver", write_string_to_file("z"));
	free_text_driver_from_file();
	assert("free_file", next_ch_file() == '\0');

	assert("init_text_driver_from_file_get_new_file", init_text_driver_from_file(TEST_FILE_3, "r"));
	assert("next_ch_new_file_get_char_1", next_ch_file() == 'z');
	assert("next_ch_new_file_get_EOF_file", next_ch_file() == '\0');
	if(!_remove_test_files())
		warning("Deleting test files error");
}