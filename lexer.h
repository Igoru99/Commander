/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#ifndef LEXER_H

#define LEXER_H

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include "globals.h"
#include "locals.h"

enum TOKENS {
	TOKEN_NAME,
	TOKEN_STR,
	TOKEN_NUM,
	TOKEN_SEP,
	TOKEN_EOF,
	TOKEN_GLOBAL,
	TOKEN_OP_EQUALS,
	TOKEN_OP_ADD,
	TOKEN_OP_SUB,
	TOKEN_BOOL_OP_EQUALS,
	TOKEN_BOOL_OP_NOT_EQUALS,
	TOKEN_BOOL_OP_AND,
	TOKEN_BOOL_OP_OR,
	TOKEN_GET,
	TOKEN_SET,
	TOKEN_COLON,
	TOKEN_LEFT_DIR,
	TOKEN_DOUBLE_LEFT_DIR,
	TOKEN_RIGHT_DIR,
	TOKEN_DOUBLE_RIGHT_DIR,
	TOKEN_FLAG,
	TOKEN_LEFT_BRACKET,
	TOKEN_RIGHT_BRACKET
};

typedef struct {
	void* value;
	ushort line_pos;
	ushort line_number;
	enum TOKENS token_type;
} Token;

typedef struct {
	Token* tokens;
	ushort count;
}TokenizedData;

TokenizedData tokenize(char(*next_ch_fn()));

#endif
