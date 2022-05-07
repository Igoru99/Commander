/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include <stdio.h>
#include <locale.h>
#include "globals.h"
#include "locals.h"
#include "tests.h"
#include "console.h"
#include "text_driver.h"
#include "script_runner.h"

void main(int argc, char** argv) {
	if (argc == 2) {
		printf("> exec %s", argv[1]);
		bool result = init_text_driver_from_file(argv[1], "r");
		if (result == false)
		{
			printf(get_msg(MESSAGES_TYPE_LOAD_SCRIPT_ERROR));
			printf(get_msg(MESSAGES_TYPE_EXIT));
			getc(stdin);
		}
		else {
			run_script(next_ch_file);
			printf(get_msg(MESSAGES_TYPE_EXIT));
			getc(stdin);
		}
	}
	else {
		char* locale = setlocale(LC_ALL, "");
		printf(HEADER);
		printf(get_msg(MESSAGES_TYPE_HELP));
#ifdef DEBUG
		test_all();
#endif
		start_stdin_listening();
	}
}