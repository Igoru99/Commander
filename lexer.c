/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

/*
#include "lexer.h"

ushort _line_number = 1;
ushort _line_pos = 1;

char(*_func_ptr)(void) = NULL;
char _current_ch = '\0';

bool _is_number(char c) {
	return c >= '0' && c <= '9' ? true : false;
}

bool is_letter(char c) {
	switch (c) {
		return true;
	default:
		return false;
	}
}

void free_token(Token* token) {
	if (token == NULL)
		return;
	switch (token->token_type) {
	case TOKEN_STR:
	case TOKEN_NUM:
	case TOKEN_FLAG:
		free(token->value);
		break;
	}
}

void free_tokens(Token* tokens, ushort count) {
	if (tokens == NULL)
		return;
	for (ushort i = 0; i < count; i++)
		free_token(tokens + i);
	free(tokens);
}

void free_TokenizedData(TokenizedData* data) {
	if (data == NULL)
		return;
	free_tokens(data->tokens, data->count);
	free(data);
}

char _next_ch() {
	if (_func_ptr != NULL)
		return _func_ptr();
	else
		return '\0';
}

Token* get_keyword_token(enum TOKENS token_type) {
	Token* token = malloc(sizeof(Token));
	if (token == NULL)
		return NULL;
	token->line_number = _line_number;
	token->line_pos = _line_pos;
	token->value = NULL;
	token->token_type = token_type;
	return token;
}


Token* next_token() {
	while (true) {
		switch (_current_ch = _next_ch()) {
		case '\0':
			return get_keyword_token(TOKEN_EOF);
		case '\n':
			_line_pos = 1;
			_line_number++;
		case ';':
			return get_keyword_token(TOKEN_SEP);
		case '$':
			return get_keyword_token(TOKEN_GLOBAL);
		case '@':
			return get_keyword_token(TOKEN_GET);
		case ' ':
			break;
		case ':':
			return get_name_token();
		case '\'':
			return get_str_token();
		case '+':
			return get_keyword_token(TOKEN_OP_ADD);
		case '-':
			return get_keyword_token(TOKEN_OP_SUB);

		default:
			if (is_letter(_current_ch))
			{
				Token* token = get_name_token();
				if (token == NULL)
					return NULL;

			}
			else if (_is_number(_current_ch))
				return get_num_token();
			else {
				err = get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL);
				return NULL;
			}
		}
		_line_pos++;
	}
}

void init_state() {
	err = NULL;
	_line_number = 1;
	_line_pos = 1;
	is_err = false;
	_current_ch = '\0';
	_func_ptr = NULL;
}

TokenizedData* init_data(ushort count) {
	TokenizedData* data = malloc(sizeof(TokenizedData));
	if (data == NULL)
	{
		return NULL;
	}
	data->tokens = malloc(count*sizeof(Token));
	if (data->tokens == NULL) {
		free(data);
		return NULL;
	}
	data->count = count;
	return data;
}

TokenizedData* tokenize(char(*next_ch_fn(void))) {
	if (next_ch_fn == NULL)
		return NULL;
	_func_ptr = next_ch_fn;
	init_state();
	Token* data[USHRT_MAX] = { NULL };
	ushort pos = 0;
	Token* current_token = NULL;
	while ((current_token = next_token()) != NULL && current_token->token_type != TOKEN_EOF) {
		data[pos++] = current_token;
	}
	if (is_err == true) {
		for (ushort i = 0; i < USHRT_MAX; i++)
			free_token(data + i);
		return NULL;
	}
	else {
		TokenizedData* tokens = init_data(pos + 1);
		if (data == NULL) {
			for (ushort i = 0; i < USHRT_MAX; i++)
				free_token(data + i);
			return NULL;
		}
		for (ushort i = 0; i < pos + 1; i++) {
			tokens->tokens[i] = *data[i];
		}
		return tokens;
	}
}
*/