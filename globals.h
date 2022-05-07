/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#ifndef GLOBALS_H

#define GLOBALS_H

/*Header*/

#define NAME "Commander"
#define VERSION "1.0.0"
#define HEADER NAME##" "##VERSION##"\n"

/*Languages*/

#define DEFAULT_LANG LANGUAGES_ENG

/*Running mode*/

#define DEBUG
//#define RELEASE

/*Data type macros*/

#define ushort unsigned short
#define short_len_bytes 7
#define true 1
#define false 0
#define bool char

/*Constrains*/

#define MAX_CMD_HISTORY 15 // Max command's count in history
#define MAX_STDIN_LEN 100 // Max stdin lenght
#define MAX_FILE_LEN 65535 // Max file's lenght
#define MAX_VAR_NAME 100// Max variable name

/*Files*/

#define GLOBAL_VARS_FILE "vars.kini" // Global variables file

#endif