/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#ifndef ERROR_H

#define ERROR_H

#include <stdio.h>
#include "globals.h"

typedef struct {
	ushort line;
	ushort pos;
	char* msg;
}ErrorInfo;

ErrorInfo get_error();

bool is_error();

void add_error(ushort line, ushort pos, char* msg);

#endif
