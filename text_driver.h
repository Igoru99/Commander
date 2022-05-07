/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#ifndef TEXT_DRIVER_H

#define TEXT_DRIVER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"

bool init_text_driver_from_file(const char* path, const char* mode);

bool init_text_driver_from_stdin(char* in);

char next_ch_file();

char next_ch_stdin();

bool write_string_to_file(char* str);

void free_text_driver_from_file();

#endif