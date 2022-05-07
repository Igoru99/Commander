/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#ifndef LOCALS_H

#define LOCALS_H

#include "globals.h"

enum LANGUAGES {
	LANGUAGES_ENG,
	LANGUAGES_RUS
};

enum MESSAGES_TYPE {
	MESSAGES_TYPE_LOAD_SCRIPT_ERROR,
	MESSAGES_TYPE_EXIT,
	MESSAGES_TYPE_HELP,
	MESSAGES_TYPE_LAST_COMMAND,
	MESSAGES_TYPE_RUN_LAST_COMMAND,
	MESSAGES_TYPE_INPUT_ERROR_STR,
	MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL,
};

void set_lang(enum LANGUAGES lang);

const char *const get_msg(enum MESSAGES_TYPE msg_type);

#endif