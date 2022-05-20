/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/


#include "lexer.h"


enum LEXER_STATES {
	LEXER_START,
	LEXER_EOF,

	LEXER_FLAG,
	LEXER_LOCAL_VAR_NAME,
	LEXER_GLOBAL_VAR_NAME,
	LEXER_MARK_NAME,
	LEXER_NUM,
	LEXER_START_STR,
	LEXER_START_QUOTE_STR,
	LEXER_END_QUOTE_STR,

	LEXER_WHITESPACE,
	LEXER_SEP,

	LEXER_LEFT_BRACKET,
	LEXER_RIGHT_BRACKET,

	LEXER_OP_ADD,
	LEXER_OP_SUB,
	LEXER_OP_EQUALS,
	LEXER_BOOL_OP_EQUALS,
	LEXER_OP_NOT,
	LEXER_BOOL_OP_NOT,

	LEXER_OP_AND,
	LEXER_BOOL_OP_AND,
	LEXER_OP_OR,
	LEXER_BOOL_OP_OR,

	LEXER_OP_RIGHT_DIR,
	LEXER_OP_DOUBLE_RIGHT_DIR,
};

enum LEXER_FUNC_RESULT {
	LEXER_FUNC_RESULT_TOKEN,
	LEXER_FUNC_RESULT_CONTINUE
};

typedef struct {
	Token* token;
	enum LEXER_FUNC_RESULT result_type;
}_lexer_func_result;

ushort _line_number = 1;
ushort _line_pos = 1;
ushort _fixed_line_number = 1;
ushort _fixed_line_pos = 1;

char(*_func_ptr)(void) = NULL;
char* _buffer = NULL;

enum LEXER_STATES _lexer_state = LEXER_START;
Token* _stack = NULL;

enum TOKENS _last_token_type;
char _last_ch;

void free_token(Token* token) {
	if (token == NULL)
		return;
	switch (token->token_type) {
	case TOKEN_LOCAL_VAR_NAME:
	case TOKEN_GLOBAL_VAR_NAME:
	case TOKEN_MARK_NAME:
	case TOKEN_STR:
	case TOKEN_QUOTE_STR:
	case TOKEN_NUM:
	case TOKEN_FLAG:
		free(token->value);
	}
	free(token);
}

char _next_ch() {
	if (_func_ptr != NULL)
		return _func_ptr();
	else
		return '\0';
}

Token* _get_keyword_token(enum TOKENS token_type) {
	Token* token = malloc(sizeof(Token));
	if (token == NULL)
	{
		add_error(0, 0, get_msg(MESSAGES_TYPE_ERROR_OUT_MEMORY));
		return NULL;
	}
	token->line_number = _line_number;
	token->line_pos = _line_pos;
	token->value = NULL;
	token->token_type = token_type;
	return token;
}

Token* _get_str_token(enum TOKENS token_type) {
	Token* token = malloc(sizeof(Token));
	if (token == NULL)
	{
		add_error(0, 0, get_msg(MESSAGES_TYPE_ERROR_OUT_MEMORY));
		return NULL;
	}
	token->line_number = _fixed_line_number;
	token->line_pos = _fixed_line_pos;
	if (_buffer == NULL) {
		add_error(0, 0, get_msg(MESSAGES_TYPE_ERROR_OUT_MEMORY));
		free(token);
		return NULL;
	}
	token->value = _buffer;
	_buffer = NULL;
	token->token_type = token_type;
	return token;
}

Token* _convert_to_keyword(Token* token) {
	if (token == NULL)
		return NULL;
	if (strcmp(token->value, "true") == 0)
		token->token_type = TOKEN_TRUE;
	else if (strcmp(token->value, "false") == 0)
		token->token_type = TOKEN_FALSE;
	else if (strcmp(token->value, "to") == 0)
		token->token_type = TOKEN_TO;
	else if (strcmp(token->value, "if") == 0)
		token->token_type = TOKEN_IF;
	else if (strcmp(token->value, "elif") == 0)
		token->token_type = TOKEN_ELIF;
	else if (strcmp(token->value, "else") == 0)
		token->token_type = TOKEN_ELSE;
	else if (strcmp(token->value, "then") == 0)
		token->token_type = TOKEN_THEN;
	else if (strcmp(token->value, "end") == 0)
		token->token_type = TOKEN_END;
	else if (strcmp(token->value, "stdout") == 0)
		token->token_type = TOKEN_STDOUT;
	else if (strcmp(token->value, "stdin") == 0)
		token->token_type = TOKEN_STDIN;
	else
		return token;
	free(token->value);
	return token;
}

Token* _get_num_token() {
	Token* token = malloc(sizeof(Token));
	if (token == NULL)
		return NULL;
	token->line_number = _fixed_line_number;
	token->line_pos = _fixed_line_pos;
	token->value = copy_int(atoi(_buffer));
	if (token->value == NULL) {
		add_error(0, 0, get_msg(MESSAGES_TYPE_ERROR_OUT_MEMORY));
		free(token);
		return NULL;
	}
	_buffer = NULL;
	token->token_type = TOKEN_NUM;
	return token;
}

_lexer_func_result _end_construction(enum LEXER_STATES lexer_state, enum TOKENS token_type) {
	_lexer_func_result _result;
	_result.result_type = LEXER_FUNC_RESULT_TOKEN;

	switch (_lexer_state) {
	case LEXER_START:
	case LEXER_END_QUOTE_STR:
	case LEXER_WHITESPACE:
	case LEXER_SEP:
	case LEXER_RIGHT_BRACKET:
		_lexer_state = lexer_state;
		_result.token = _get_keyword_token(token_type);
		return _result;
	case LEXER_FLAG:
		_lexer_state = lexer_state;
		_stack = _get_keyword_token(token_type);
		_result.token = _get_str_token(TOKEN_FLAG);
		return _result;
	case LEXER_LOCAL_VAR_NAME:
		_lexer_state = lexer_state;
		_stack = _get_keyword_token(token_type);
		_result.token = _get_str_token(TOKEN_LOCAL_VAR_NAME);
		return _result;
	case LEXER_GLOBAL_VAR_NAME:
		_lexer_state = lexer_state;
		_stack = _get_keyword_token(token_type);
		_result.token = _get_str_token(TOKEN_GLOBAL_VAR_NAME);
		return _result;
	case LEXER_MARK_NAME:
		_lexer_state = lexer_state;
		_stack = _get_keyword_token(token_type);
		_result.token = _get_str_token(TOKEN_MARK_NAME);
		return _result;
	case LEXER_NUM:
		_lexer_state = lexer_state;
		_stack = _get_keyword_token(token_type);
		_result.token = _get_num_token();
		return _result;
	case LEXER_START_STR:
		_lexer_state = lexer_state;
		_stack = _get_keyword_token(token_type);
		_result.token = _convert_to_keyword(_get_str_token(TOKEN_STR));
		return _result;
	default:
		if (token_type == TOKEN_EOF)
			add_error(_line_number, _line_pos+1, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_EOF));
		else
			add_error(_line_number, _line_pos+1, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		_result.token = NULL;
		return _result;
	}
}

_lexer_func_result _process_ch_EOF() {
	return _end_construction(LEXER_EOF, TOKEN_EOF);
}

_lexer_func_result _process_ch_sep(char sep) {
	if (sep == '\n') {
		_line_pos = 1;
		_line_number++;
	}
	if (_lexer_state == LEXER_START_QUOTE_STR) {
		_buffer = append_char(_buffer, sep);
		_line_pos++;
		_lexer_func_result _result;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	}
	return _end_construction(LEXER_SEP, TOKEN_SEP);
}

_lexer_func_result _process_ch_var_directive() {
	_lexer_func_result _result;
	_result.result_type = LEXER_FUNC_RESULT_TOKEN;

	switch (_lexer_state) {
	case LEXER_OP_EQUALS:
		_fixed_line_number = _line_number;
		_fixed_line_pos = _line_pos;
		_buffer = append_char(_buffer, '@');
		_line_pos++;
		_lexer_state = LEXER_LOCAL_VAR_NAME;
		_result.token = _get_keyword_token(TOKEN_OP_EQUALS);
		return _result;
	case LEXER_OP_RIGHT_DIR:
		_fixed_line_number = _line_number;
		_fixed_line_pos = _line_pos;
		_buffer = append_char(_buffer, '@');
		_line_pos++;
		_lexer_state = LEXER_LOCAL_VAR_NAME;
		_result.token = _get_keyword_token(TOKEN_RIGHT_DIR);
		return _result;
	case LEXER_START:
	case LEXER_WHITESPACE:
	case LEXER_SEP:
	case LEXER_LEFT_BRACKET:
	case LEXER_OP_ADD:
	case LEXER_OP_SUB:
	case LEXER_BOOL_OP_EQUALS:
	case LEXER_BOOL_OP_NOT:
	case LEXER_BOOL_OP_AND:
	case LEXER_BOOL_OP_OR:
	case LEXER_OP_DOUBLE_RIGHT_DIR:
		_fixed_line_number = _line_number;
		_fixed_line_pos = _line_pos;
		_buffer = append_char(_buffer, '@');
		_line_pos++;
		_lexer_state = LEXER_LOCAL_VAR_NAME;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	case LEXER_LOCAL_VAR_NAME:
		_buffer = append_char(_buffer, '@');
		_line_pos++;
		_lexer_state = LEXER_GLOBAL_VAR_NAME;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	case LEXER_START_STR:
	case LEXER_START_QUOTE_STR:
		_buffer = append_char(_buffer, '@');
		_line_pos++;
		_lexer_func_result _result;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	default:
		add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		_result.token = NULL;
		return _result;
	}
}

_lexer_func_result _process_ch_colon() {
	_lexer_func_result _result;
	_result.result_type = LEXER_FUNC_RESULT_TOKEN;

	switch (_lexer_state) {
	case LEXER_START:
	case LEXER_WHITESPACE:
	case LEXER_SEP:
		_fixed_line_number = _line_number;
		_fixed_line_pos = _line_pos;
		_buffer = append_char(_buffer, ':');
		_line_pos++;
		_lexer_state = LEXER_MARK_NAME;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	case LEXER_START_STR:
	case LEXER_START_QUOTE_STR:
		_buffer = append_char(_buffer, ':');
		_line_pos++;
		_lexer_func_result _result;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	default:
		add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		_result.token = NULL;
		return _result;
	}
}

_lexer_func_result _process_ch_quote() {
	_lexer_func_result _result;
	_result.result_type = LEXER_FUNC_RESULT_TOKEN;

	switch (_lexer_state) {
	case LEXER_OP_EQUALS:
		_lexer_state = LEXER_START_QUOTE_STR;
		_result.token = _get_keyword_token(TOKEN_OP_EQUALS);
		_line_pos++;
		return _result;
	case LEXER_OP_RIGHT_DIR:
		_lexer_state = LEXER_START_QUOTE_STR;
		_result.token = _get_keyword_token(TOKEN_RIGHT_DIR);
		_line_pos++;
		return _result;
	case LEXER_START:
	case LEXER_WHITESPACE:
	case LEXER_SEP:
	case LEXER_OP_ADD:
	case LEXER_OP_SUB:
	case LEXER_BOOL_OP_EQUALS:
	case LEXER_BOOL_OP_NOT:
	case LEXER_BOOL_OP_AND:
	case LEXER_BOOL_OP_OR:
	case LEXER_OP_DOUBLE_RIGHT_DIR:
		_fixed_line_number = _line_number;
		_fixed_line_pos = _line_pos;
		_lexer_state = LEXER_START_QUOTE_STR;
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	case LEXER_START_QUOTE_STR:
		_lexer_state = LEXER_END_QUOTE_STR;
		_result.token = _convert_to_keyword(_get_str_token(TOKEN_STR));
		_line_pos++;
		return _result;
	default:
		add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		_result.token = NULL;
		return _result;
	}
}

_lexer_func_result _process_ch_whitespace() {
	_lexer_func_result _result;
	_result.result_type = LEXER_FUNC_RESULT_TOKEN;

	switch (_lexer_state) {
	case LEXER_OP_EQUALS:
		_lexer_state = LEXER_WHITESPACE;
		_result.token = _get_keyword_token(TOKEN_OP_EQUALS);
		_line_pos++;
		return _result;
	case LEXER_OP_RIGHT_DIR:
		_lexer_state = LEXER_WHITESPACE;
		_result.token = _get_keyword_token(TOKEN_RIGHT_DIR);
		_line_pos++;
		return _result;
	case LEXER_START:
	case LEXER_WHITESPACE:
	case LEXER_SEP:
	case LEXER_LEFT_BRACKET:
	case LEXER_RIGHT_BRACKET:
	case LEXER_OP_ADD:
	case LEXER_OP_SUB:
	case LEXER_BOOL_OP_EQUALS:
	case LEXER_BOOL_OP_NOT:
	case LEXER_BOOL_OP_AND:
	case LEXER_BOOL_OP_OR:
	case LEXER_OP_DOUBLE_RIGHT_DIR:
		_lexer_state = LEXER_WHITESPACE;
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	case LEXER_FLAG:
		_lexer_state = LEXER_WHITESPACE;
		_result.token = _get_str_token(TOKEN_FLAG);
		_line_pos++;
		return _result;
	case LEXER_LOCAL_VAR_NAME:
		_lexer_state = LEXER_WHITESPACE;
		_result.token = _get_str_token(TOKEN_LOCAL_VAR_NAME);
		_line_pos++;
		return _result;
	case LEXER_GLOBAL_VAR_NAME:
		_lexer_state = LEXER_WHITESPACE;
		_result.token = _get_str_token(TOKEN_GLOBAL_VAR_NAME);
		_line_pos++;
		return _result;
	case LEXER_MARK_NAME:
		_lexer_state = LEXER_WHITESPACE;
		_result.token = _get_str_token(TOKEN_MARK_NAME);
		_line_pos++;
		return _result;
	case LEXER_NUM:
		_lexer_state = LEXER_WHITESPACE;
		_result.token = _get_num_token();
		_line_pos++;
		return _result;
	case LEXER_START_STR:
		_lexer_state = LEXER_WHITESPACE;
		_result.token = _convert_to_keyword(_get_str_token(TOKEN_STR));
		_line_pos++;
		return _result;
	case LEXER_END_QUOTE_STR:
		_lexer_state = LEXER_WHITESPACE;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		_line_pos++;
		return _result;
	case LEXER_START_QUOTE_STR:
		_buffer = append_char(_buffer, ' ');
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		_line_pos++;
		return _result;
	default:
		add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		_result.token = NULL;
		return _result;
	}
}

_lexer_func_result _process_ch_add() {
	_lexer_func_result _result;
	_result.result_type = LEXER_FUNC_RESULT_TOKEN;

	switch (_lexer_state) {
	case LEXER_OP_EQUALS:
		_lexer_state = LEXER_OP_ADD;
		_stack = _get_keyword_token(TOKEN_OP_ADD);
		_result.token = _get_str_token(TOKEN_OP_EQUALS);
		_line_pos++;
		return _result;
	case LEXER_LOCAL_VAR_NAME:
		_lexer_state = LEXER_OP_ADD;
		_stack = _get_keyword_token(TOKEN_OP_ADD);
		_result.token = _get_str_token(TOKEN_LOCAL_VAR_NAME);
		_line_pos++;
		return _result;
	case LEXER_GLOBAL_VAR_NAME:
		_lexer_state = LEXER_OP_ADD;
		_stack = _get_keyword_token(TOKEN_OP_ADD);
		_result.token = _get_str_token(TOKEN_GLOBAL_VAR_NAME);
		_line_pos++;
		return _result;
	case LEXER_NUM:
		_lexer_state = LEXER_OP_ADD;
		_stack = _get_keyword_token(TOKEN_OP_ADD);
		_result.token = _get_num_token();
		_line_pos++;
		return _result;
	case LEXER_START_STR:
		_lexer_state = LEXER_OP_ADD;
		_stack = _get_keyword_token(TOKEN_OP_ADD);
		_result.token = _convert_to_keyword(_get_str_token(TOKEN_STR));
		_line_pos++;
		return _result;
	case LEXER_START_QUOTE_STR:
		_buffer = append_char(_buffer, '+');
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		_line_pos++;
		return _result;
	case LEXER_WHITESPACE:
	case LEXER_SEP:
	case LEXER_LEFT_BRACKET:
	case LEXER_RIGHT_BRACKET:
	case LEXER_BOOL_OP_AND:
	case LEXER_BOOL_OP_EQUALS:
	case LEXER_BOOL_OP_NOT:
	case LEXER_BOOL_OP_OR:
	case LEXER_END_QUOTE_STR:
		_lexer_state = LEXER_OP_ADD;
		_result.token = _get_keyword_token(TOKEN_OP_ADD);
		_line_pos++;
		return _result;
	default:
		add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		_result.token = NULL;
		return _result;
	}
}

_lexer_func_result _process_ch_sub() {
	_lexer_func_result _result;
	_result.result_type = LEXER_FUNC_RESULT_TOKEN;

	switch (_lexer_state) {
	case LEXER_OP_EQUALS:
		_lexer_state = LEXER_OP_SUB;
		_stack = _get_keyword_token(TOKEN_OP_SUB);
		_result.token = _get_keyword_token(TOKEN_OP_EQUALS);
		_line_pos++;
		return _result;
	case LEXER_LOCAL_VAR_NAME:
		_lexer_state = LEXER_OP_SUB;
		_stack = _get_keyword_token(TOKEN_OP_SUB);
		_result.token = _get_str_token(TOKEN_LOCAL_VAR_NAME);
		_line_pos++;
		return _result;
	case LEXER_GLOBAL_VAR_NAME:
		_lexer_state = LEXER_OP_SUB;
		_stack = _get_keyword_token(TOKEN_OP_SUB);
		_result.token = _get_str_token(TOKEN_GLOBAL_VAR_NAME);
		_line_pos++;
		return _result;
	case LEXER_NUM:
		_lexer_state = LEXER_OP_SUB;
		_stack = _get_keyword_token(TOKEN_OP_SUB);
		_result.token = _get_num_token();
		_line_pos++;
		return _result;
	case LEXER_START_STR:
	case LEXER_START_QUOTE_STR:
		_buffer = append_char(_buffer, '-');
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		_line_pos++;
		return _result;
	case LEXER_WHITESPACE:
		if (_last_token_type == TOKEN_STR) {
			_lexer_state = LEXER_FLAG;
			_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
			_line_pos++;
			return _result;
		}
		else {
			add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
			_result.token = NULL;
			return _result;
		}
	case LEXER_SEP:
	case LEXER_LEFT_BRACKET:
	case LEXER_RIGHT_BRACKET:
	case LEXER_BOOL_OP_AND:
	case LEXER_BOOL_OP_EQUALS:
	case LEXER_BOOL_OP_NOT:
	case LEXER_BOOL_OP_OR:
		_lexer_state = LEXER_OP_SUB;
		_result.token = _get_keyword_token(TOKEN_OP_SUB);
		_line_pos++;
		return _result;
	default:
		add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		_result.token = NULL;
		return _result;
	}
}

_lexer_func_result _process_ch_num(char c) {
	_lexer_func_result _result;
	_result.result_type = LEXER_FUNC_RESULT_TOKEN;

	switch (_lexer_state) {
	case LEXER_OP_EQUALS:
		_fixed_line_number = _line_number;
		_fixed_line_pos = _line_pos;
		_lexer_state = LEXER_NUM;
		_buffer = append_char(_buffer, c);
		_result.token = _get_keyword_token(TOKEN_OP_EQUALS);
		_line_pos++;
		return _result;
	case LEXER_NUM:
		if (strcmp(_buffer, "0") == 0) {
			add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
			_result.token = NULL;
			return _result;
		}
		else {
			_fixed_line_number = _line_number;
			_fixed_line_pos = _line_pos;
			_buffer = append_char(_buffer, c);
			_line_pos++;
			_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
			return _result;
		}
	case LEXER_START:
	case LEXER_WHITESPACE:
	case LEXER_SEP:
	case LEXER_LEFT_BRACKET:
	case LEXER_RIGHT_BRACKET:
	case LEXER_OP_ADD:
	case LEXER_OP_SUB:
	case LEXER_BOOL_OP_EQUALS:
	case LEXER_BOOL_OP_NOT:
	case LEXER_BOOL_OP_AND:
	case LEXER_BOOL_OP_OR:
		_fixed_line_number = _line_number;
		_fixed_line_pos = _line_pos;
		_lexer_state = LEXER_NUM;
		_buffer = append_char(_buffer, c);
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	case LEXER_START_STR:
	case LEXER_START_QUOTE_STR:
	case LEXER_LOCAL_VAR_NAME:
	case LEXER_MARK_NAME:
		_buffer = append_char(_buffer, c);
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	case LEXER_FLAG:
		if (_last_ch != '-') {
			_buffer = append_char(_buffer, c);
			_line_pos++;
			_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
			return _result;
		}
		else {
			add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
			_result.token = NULL;
			return _result;
		}
	default:
		add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		_result.token = NULL;
		return _result;
	}
}

_lexer_func_result _process_ch_letter(char c) {
	_lexer_func_result _result;
	_result.result_type = LEXER_FUNC_RESULT_TOKEN;

	switch (_lexer_state) {
	case LEXER_OP_EQUALS:
		_fixed_line_number = _line_number;
		_fixed_line_pos = _line_pos;
		_lexer_state = LEXER_START_STR;
		_buffer = append_char(_buffer, c);
		_result.token = _get_keyword_token(TOKEN_OP_EQUALS);
		_line_pos++;
		return _result;
	case LEXER_START:
	case LEXER_SEP:
	case LEXER_WHITESPACE:
	case LEXER_OP_ADD:
	case LEXER_OP_SUB:
	case LEXER_BOOL_OP_EQUALS:
	case LEXER_BOOL_OP_NOT:
	case LEXER_BOOL_OP_AND:
	case LEXER_BOOL_OP_OR:
	case LEXER_OP_RIGHT_DIR:
	case LEXER_OP_DOUBLE_RIGHT_DIR:
	case LEXER_LEFT_BRACKET:
		_fixed_line_number = _line_number;
		_fixed_line_pos = _line_pos;
		_lexer_state = LEXER_START_STR;
		_buffer = append_char(_buffer, c);
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	case LEXER_FLAG:
	case LEXER_LOCAL_VAR_NAME:
	case LEXER_GLOBAL_VAR_NAME:
	case LEXER_MARK_NAME:
		_buffer = append_char(_buffer, c);
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	case LEXER_START_STR:
	case LEXER_START_QUOTE_STR:
		_buffer = append_char(_buffer, c);
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	default:
		add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		_result.token = NULL;
		return _result;
	}
}

_lexer_func_result _process_ch_ascii_symbol(char c) {
	_lexer_func_result _result;
	_result.result_type = LEXER_FUNC_RESULT_TOKEN;

	switch (_lexer_state) {
	case LEXER_OP_EQUALS:
		_fixed_line_number = _line_number;
		_fixed_line_pos = _line_pos;
		_lexer_state = LEXER_START_STR;
		_buffer = append_char(_buffer, c);
		_result.token = _get_keyword_token(TOKEN_OP_EQUALS);
		_line_pos++;
		return _result;
	case LEXER_START:
	case LEXER_SEP:
	case LEXER_WHITESPACE:
	case LEXER_OP_ADD:
	case LEXER_OP_SUB:
	case LEXER_BOOL_OP_EQUALS:
	case LEXER_BOOL_OP_NOT:
	case LEXER_BOOL_OP_AND:
	case LEXER_BOOL_OP_OR:
	case LEXER_OP_RIGHT_DIR:
	case LEXER_OP_DOUBLE_RIGHT_DIR:
	case LEXER_LEFT_BRACKET:
		_fixed_line_number = _line_number;
		_fixed_line_pos = _line_pos;
		_lexer_state = LEXER_START_STR;
		_buffer = append_char(_buffer, c);
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	case LEXER_START_STR:
	case LEXER_START_QUOTE_STR:
		_buffer = append_char(_buffer, c);
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	default:
		add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		_result.token = NULL;
		return _result;
	}
}

_lexer_func_result _process_ch_not() {
	_lexer_func_result _result;
	_result.result_type = LEXER_FUNC_RESULT_TOKEN;

	switch (_lexer_state) {
	case LEXER_LOCAL_VAR_NAME:
	case LEXER_GLOBAL_VAR_NAME:
	case LEXER_NUM:
	case LEXER_END_QUOTE_STR:
	case LEXER_WHITESPACE:
	case LEXER_RIGHT_BRACKET:
		_lexer_state = LEXER_BOOL_OP_NOT;
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	case LEXER_START_STR:
	case LEXER_START_QUOTE_STR:
		_buffer = append_char(_buffer, '!');
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	default:
		add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		_result.token = NULL;
		return _result;
	}
}

_lexer_func_result _process_ch_equals() {
	_lexer_func_result _result;
	_result.result_type = LEXER_FUNC_RESULT_TOKEN;

	switch (_lexer_state) {
	case LEXER_BOOL_OP_NOT:
		_lexer_state = LEXER_BOOL_OP_NOT;
		_result.token = _get_keyword_token(TOKEN_BOOL_OP_NOT_EQUALS);
		_line_pos++;
		return _result;
	case LEXER_OP_EQUALS:
		_lexer_state = LEXER_BOOL_OP_EQUALS;
		_result.token = _get_keyword_token(TOKEN_BOOL_OP_EQUALS);
		_line_pos++;
		return _result;
	case LEXER_LOCAL_VAR_NAME:
		_lexer_state = LEXER_OP_EQUALS;
		_result.token = _get_str_token(TOKEN_LOCAL_VAR_NAME);
		_line_pos++;
		return _result;
	case LEXER_GLOBAL_VAR_NAME:
		_lexer_state = LEXER_OP_EQUALS;
		_result.token = _get_str_token(TOKEN_GLOBAL_VAR_NAME);
		_line_pos++;
		return _result;
	case LEXER_NUM:
		_lexer_state = LEXER_OP_EQUALS;
		_result.token = _get_num_token();
		_line_pos++;
		return _result;
	case LEXER_END_QUOTE_STR:
	case LEXER_WHITESPACE:
	case LEXER_RIGHT_BRACKET:
		_lexer_state = LEXER_OP_EQUALS;
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	case LEXER_START_STR:
	case LEXER_START_QUOTE_STR:
		_buffer = append_char(_buffer, '=');
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	default:
		add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		_result.token = NULL;
		return _result;
	}
}

_lexer_func_result _process_ch_right_dir() {
	_lexer_func_result _result;
	_result.result_type = LEXER_FUNC_RESULT_TOKEN;

	switch (_lexer_state) {
	case LEXER_LOCAL_VAR_NAME:
		_lexer_state = LEXER_OP_RIGHT_DIR;
		_result.token = _get_str_token(TOKEN_LOCAL_VAR_NAME);
		_line_pos++;
		return _result;
	case LEXER_GLOBAL_VAR_NAME:
		_lexer_state = LEXER_OP_RIGHT_DIR;
		_result.token = _get_str_token(TOKEN_GLOBAL_VAR_NAME);
		_line_pos++;
		return _result;
	case LEXER_END_QUOTE_STR:
	case LEXER_WHITESPACE:
		_lexer_state = LEXER_OP_RIGHT_DIR;
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	case LEXER_START_STR:
	case LEXER_START_QUOTE_STR:
		_buffer = append_char(_buffer, '>');
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	case LEXER_OP_RIGHT_DIR:
		_lexer_state = LEXER_OP_DOUBLE_RIGHT_DIR;
		_result.token = _get_keyword_token(TOKEN_DOUBLE_RIGHT_DIR);
		_line_pos++;
		return _result;
	default:
		add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		_result.token = NULL;
		return _result;
	}
}

_lexer_func_result _process_ch_and() {
	_lexer_func_result _result;
	_result.result_type = LEXER_FUNC_RESULT_TOKEN;

	switch (_lexer_state) {
	case LEXER_LOCAL_VAR_NAME:
		_lexer_state = LEXER_OP_AND;
		_result.token = _get_str_token(TOKEN_LOCAL_VAR_NAME);
		_line_pos++;
		return _result;
	case LEXER_GLOBAL_VAR_NAME:
		_lexer_state = LEXER_OP_AND;
		_result.token = _get_str_token(TOKEN_GLOBAL_VAR_NAME);
		_line_pos++;
		return _result;
	case LEXER_NUM:
		_lexer_state = LEXER_OP_AND;
		_result.token = _get_num_token();
		_line_pos++;
		return _result;
	case LEXER_WHITESPACE:
	case LEXER_RIGHT_BRACKET:
	case LEXER_END_QUOTE_STR:
		_lexer_state = LEXER_OP_AND;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		_line_pos++;
		return _result;
	case LEXER_START_STR:
	case LEXER_START_QUOTE_STR:
		_buffer = append_char(_buffer, '&');
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	case LEXER_OP_AND:
		_lexer_state = LEXER_BOOL_OP_AND;
		_result.token = _get_keyword_token(TOKEN_BOOL_OP_AND);
		_line_pos++;
		return _result;
	default:
		add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		_result.token = NULL;
		return _result;
	}
}

_lexer_func_result _process_ch_or() {
	_lexer_func_result _result;
	_result.result_type = LEXER_FUNC_RESULT_TOKEN;

	switch (_lexer_state) {
	case LEXER_LOCAL_VAR_NAME:
		_lexer_state = LEXER_OP_OR;
		_result.token = _get_str_token(TOKEN_LOCAL_VAR_NAME);
		_line_pos++;
		return _result;
	case LEXER_GLOBAL_VAR_NAME:
		_lexer_state = LEXER_OP_OR;
		_result.token = _get_str_token(TOKEN_GLOBAL_VAR_NAME);
		_line_pos++;
		return _result;
	case LEXER_NUM:
		_lexer_state = LEXER_OP_OR;
		_result.token = _get_num_token();
		_line_pos++;
		return _result;
	case LEXER_WHITESPACE:
	case LEXER_RIGHT_BRACKET:
	case LEXER_END_QUOTE_STR:
		_lexer_state = LEXER_OP_OR;
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	case LEXER_START_STR:
	case LEXER_START_QUOTE_STR:
		_buffer = append_char(_buffer, '|');
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	case LEXER_OP_OR:
		_lexer_state = LEXER_BOOL_OP_OR;
		_result.token = _get_keyword_token(TOKEN_BOOL_OP_OR);
		_line_pos++;
		return _result;
	default:
		add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		_result.token = NULL;
		return _result;
	}
}

_lexer_func_result _process_ch_left_bracket() {
	_lexer_func_result _result;
	_result.result_type = LEXER_FUNC_RESULT_TOKEN;

	switch (_lexer_state) {
	case LEXER_START:
	case LEXER_WHITESPACE:
	case LEXER_SEP:
	case LEXER_LEFT_BRACKET:
	case LEXER_OP_ADD:
	case LEXER_OP_SUB:
	case LEXER_OP_EQUALS:
	case LEXER_BOOL_OP_EQUALS:
	case LEXER_BOOL_OP_NOT:
	case LEXER_BOOL_OP_AND:
	case LEXER_BOOL_OP_OR:
		_lexer_state = LEXER_LEFT_BRACKET;
		_result.token = _get_keyword_token(TOKEN_LEFT_BRACKET);
		_line_pos++;
		return _result;
	case LEXER_START_STR:
	case LEXER_START_QUOTE_STR:
		_buffer = append_char(_buffer, '(');
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	default:
		add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		_result.token = NULL;
		return _result;
	}
}

_lexer_func_result _process_ch_right_bracket() {
	_lexer_func_result _result;
	_result.result_type = LEXER_FUNC_RESULT_TOKEN;

	switch (_lexer_state) {
	case LEXER_LOCAL_VAR_NAME:
		_lexer_state = LEXER_RIGHT_BRACKET;
		_stack = _get_keyword_token(TOKEN_RIGHT_BRACKET);
		_result.token = _get_str_token(TOKEN_LOCAL_VAR_NAME);
		_line_pos++;
		return _result;
	case LEXER_GLOBAL_VAR_NAME:
		_lexer_state = LEXER_RIGHT_BRACKET;
		_stack = _get_keyword_token(TOKEN_RIGHT_BRACKET);
		_result.token = _get_str_token(TOKEN_GLOBAL_VAR_NAME);
		_line_pos++;
		return _result;
	case LEXER_NUM:
		_lexer_state = LEXER_RIGHT_BRACKET;
		_stack = _get_keyword_token(TOKEN_RIGHT_BRACKET);
		_result.token = _get_num_token();
		_line_pos++;
		return _result;
	case LEXER_END_QUOTE_STR:
	case LEXER_WHITESPACE:
	case LEXER_RIGHT_BRACKET:
		_lexer_state = LEXER_RIGHT_BRACKET;
		_result.token = _get_keyword_token(TOKEN_RIGHT_BRACKET);
		_line_pos++;
		return _result;
	case	LEXER_START_STR:
	case	LEXER_START_QUOTE_STR:
		_buffer = append_char(_buffer, ')');
		_line_pos++;
		_result.result_type = LEXER_FUNC_RESULT_CONTINUE;
		return _result;
	default:
		add_error(_line_number, _line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		_result.token = NULL;
		return _result;
	}
}

Token* next_token() {

	_lexer_func_result result;
	Token* temp;
	char c;

	if (_stack != NULL) {
		temp = _stack;
		_stack = NULL;
		_last_token_type = temp->token_type;
		return temp;
	}

	while (true) {
		switch (c = _next_ch()) {

		case '\0':

			result = _process_ch_EOF();
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}

		case '\n':
		case ';':

			result = _process_ch_sep(c);
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				_last_token_type = result.token->token_type;
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}

		case '@':

			result = _process_ch_var_directive();
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				_last_token_type = result.token->token_type;
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}

		case ' ':

			result = _process_ch_whitespace();
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				_last_token_type = result.token->token_type;
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}

		case ':':

			result = _process_ch_colon();
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				_last_token_type = result.token->token_type;
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}

		case '\'':

			result = _process_ch_quote();
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				_last_token_type = result.token->token_type;
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}

		case '+':

			result = _process_ch_add();
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				_last_token_type = result.token->token_type;
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}

		case '-':

			result = _process_ch_sub();
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				_last_token_type = result.token->token_type;
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}

		case '!':

			result = _process_ch_not();
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				_last_token_type = result.token->token_type;
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}

		case '=':

			result = _process_ch_equals();
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				_last_token_type = result.token->token_type;
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}

		case '(':

			result = _process_ch_left_bracket();
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				_last_token_type = result.token->token_type;
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}

		case ')':

			result = _process_ch_right_bracket();
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				_last_token_type = result.token->token_type;
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}

		case '>':

			result = _process_ch_right_dir();
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				_last_token_type = result.token->token_type;
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}

		case '&':

			result = _process_ch_and();
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				_last_token_type = result.token->token_type;
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}

		case '|':

			result = _process_ch_or();
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				_last_token_type = result.token->token_type;
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':

			result = _process_ch_num(c);
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				_last_token_type = result.token->token_type;
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}

		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
		case 'à':
		case 'á':
		case 'â':
		case 'ã':
		case 'ä':
		case 'å':
		case '¸':
		case 'æ':
		case 'ç':
		case 'è':
		case 'ê':
		case 'ë':
		case 'ì':
		case 'í':
		case 'î':
		case 'ï':
		case 'ð':
		case 'ñ':
		case 'ò':
		case 'ó':
		case 'ô':
		case 'õ':
		case 'ö':
		case '÷':
		case 'ø':
		case 'ù':
		case 'ú':
		case 'û':
		case 'ü':
		case 'ý':
		case 'þ':
		case 'ÿ':
		case 'À':
		case 'Á':
		case 'Â':
		case 'Ã':
		case 'Ä':
		case 'Å':
		case '¨':
		case 'Æ':
		case 'Ç':
		case 'È':
		case 'Ê':
		case 'Ë':
		case 'Ì':
		case 'Í':
		case 'Î':
		case 'Ï':
		case 'Ð':
		case 'Ñ':
		case 'Ò':
		case 'Ó':
		case 'Ô':
		case 'Õ':
		case 'Ö':
		case '×':
		case 'Ø':
		case 'Ù':
		case 'Ú':
		case 'Û':
		case 'Ü':
		case 'Ý':
		case 'Þ':
		case 'ß':
		case '_':

			result = _process_ch_letter(c);
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				_last_token_type = result.token->token_type;
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}

		default:

			result = _process_ch_ascii_symbol(c);
			switch (result.result_type) {
			case LEXER_FUNC_RESULT_TOKEN:
				_last_token_type = result.token->token_type;
				return result.token;
			case LEXER_FUNC_RESULT_CONTINUE:
				continue;
			}
		}
		_last_ch = c;
	}
}

void _init_start_lexer_state(char(*func_ptr)(void)) {
	_line_number = 1;
	_line_pos = 1;
	_fixed_line_number = 1;
	_fixed_line_pos = 1;
	_buffer = NULL;
	_func_ptr = func_ptr;
	_lexer_state = LEXER_START;
	_stack = NULL;
	_last_token_type = TOKEN_EOF;
	_last_ch = '\0';
}

list* tokenize(char(*next_ch_fn(void))) {

	if (next_ch_fn == NULL)
		return NULL;

	_init_start_lexer_state(next_ch_fn);

	list* tokens_list = init_list();
	if (tokens_list == NULL)
	{
		add_error(0, 0, get_msg(MESSAGES_TYPE_ERROR_OUT_MEMORY));
		return NULL;
	}

	Token* current_token = NULL;

	while ((current_token = next_token()) != NULL && current_token->token_type != TOKEN_EOF) {
		add_item(tokens_list, current_token);
	}

	if (is_error()) {
		node* current_node = tokens_list->first_node;
		for (ushort i = 0; i < tokens_list->count; i++)
		{
			free_token(current_node->value);
			current_node = current_node->next_node;
		}
		free_list(tokens_list);
		return NULL;
	}
	else {
		add_item(tokens_list, current_token);
		return tokens_list;
	}
}