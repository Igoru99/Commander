/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include "error.h"

static ErrorInfo _err;

ErrorInfo get_error() {
	ErrorInfo cur_err = _err;
	_err.line = 0;
	_err.pos = 0;
	_err.msg = NULL;
	return cur_err;
}

bool is_error() {
	if (_err.msg == NULL)
		return false;
	else
		return true;
}

void add_error(ushort line, ushort pos, char* msg){
	_err.line = line;
	_err.pos = pos;
	_err.msg = msg;
}