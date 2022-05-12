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
#include "utilites.h"
#include "error.h"
#include "list.h"

enum TOKENS {
	TOKEN_LOCAL_VAR_NAME,
	TOKEN_GLOBAL_VAR_NAME,
	TOKEN_MARK_NAME,
	TOKEN_STR,
	TOKEN_QUOTE_STR,
	TOKEN_NUM,
	TOKEN_FLAG,

	TOKEN_SEP,
	TOKEN_EOF,

	TOKEN_OP_EQUALS,
	TOKEN_OP_ADD,
	TOKEN_OP_SUB,
	TOKEN_BOOL_OP_EQUALS,
	TOKEN_BOOL_OP_NOT_EQUALS,
	TOKEN_BOOL_OP_AND,
	TOKEN_BOOL_OP_OR,
	TOKEN_RIGHT_DIR,
	TOKEN_DOUBLE_RIGHT_DIR,
	TOKEN_LEFT_BRACKET,
	TOKEN_RIGHT_BRACKET,

	TOKEN_TO,
	TOKEN_IF,
	TOKEN_ELIF,
	TOKEN_ELSE,
	TOKEN_END,
	TOKEN_THEN,
	TOKEN_STDOUT,
	TOKEN_STDIN,
	TOKEN_TRUE,
	TOKEN_FALSE
};

typedef struct {
	void* value;
	ushort line_pos;
	ushort line_number;
	enum TOKENS token_type;
} Token;

list* tokenize(char(*next_ch_fn()));

#endif