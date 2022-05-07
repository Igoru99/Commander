/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#ifndef TEST_H

#define TEST_H
#include <stdio.h>
#include "globals.h"

#define TEST_FILE_1 "test.txt"
#define TEST_FILE_2 "test0.txt"
#define TEST_FILE_3 "test1.txt"

void assert(const char* const test_name, bool expression);

void warning(const char* const msg);

bool is_warning();

ushort get_test_count();

ushort get_true_test_count();

#endif