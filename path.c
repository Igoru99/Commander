/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include "path.h"

static const char* _cur_dir = "C://";

const char* get_cur_dir() {
	return _cur_dir;
}

bool set_cur_dir(const char* path) {
	if(path != NULL)
	_cur_dir = path;
}