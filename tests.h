/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#ifndef TESTS_H

#define TESTS_H
#include <stdio.h>
#include "globals.h"
#include "test.h"
#include "tests_dict.h"
#include "tests_text_driver.h"
#include "tests_settings.h"
#include "tests_error.h"
#include "tests_utilites.h"

void test_all();

ushort get_test_count();

ushort get_true_test_count();

#endif