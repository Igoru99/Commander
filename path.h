/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#ifndef PATH_H

#define PATH_H

#include <stdio.h>
#include "globals.h"

const char* get_cur_dir();

bool set_cur_dir(const char* path);

#endif