/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#ifndef SETTINGS_H

#define SETTINGS_H

#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "dict.h"
#include "utilites.h"
#include "text_driver.h"

dict* get_settings();

bool set_settings(dict* dict);

#endif