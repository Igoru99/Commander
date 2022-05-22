/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include "lexer.h"

static ushort line_number = 1;
static ushort line_pos = 0;
static ushort fixed_line_number = 1;
static ushort fixed_line_pos = 0;
static list* tokens_list = NULL;
static char(*func_ptr)(void) = NULL;
static char current_ch = '\0';
static char* buffer = NULL;

static bool is_sep(char c) {
	if (c == ';' || c == '\n')
		return true;
	else
		return false;
}

static bool is_number(char c) {
	if (c >= '0' && c <= '9')
		return true;
	else
		return false;
}

static bool is_letter(char c) {
	switch (c) {
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
		return true;
	default:
		return false;
	}
}

static void append_char_to_buffer(char c) {
	buffer = append_char(buffer, c);
}

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
		free(token->value);
	}
	free(token);
}

static char next_ch() {
	if (func_ptr != NULL)
		current_ch = func_ptr();
	else
		current_ch = '\0';
	line_pos++;
	return current_ch;
}

static Token* get_keyword_token(enum TOKENS token_type) {
	Token* token = malloc(sizeof(Token));
	if (token == NULL)
	{
		add_error(0, 0, get_msg(MESSAGES_TYPE_ERROR_OUT_MEMORY));
		return NULL;
	}
	token->line_number = line_number;
	token->line_pos = line_pos;
	token->value = NULL;
	token->token_type = token_type;
	return token;
}

static Token* get_str_token(enum TOKENS token_type) {
	Token* token = malloc(sizeof(Token));
	if (token == NULL)
	{
		add_error(0, 0, get_msg(MESSAGES_TYPE_ERROR_OUT_MEMORY));
		return NULL;
	}
	token->line_number = fixed_line_number;
	token->line_pos = fixed_line_pos;
	if (buffer == NULL) {
		add_error(0, 0, get_msg(MESSAGES_TYPE_ERROR_OUT_MEMORY));
		free(token);
		return NULL;
	}
	token->value = buffer;
	buffer = NULL;
	token->token_type = token_type;
	return token;
}

static Token* convert_to_keyword(Token* token) {
	if (token == NULL)
		return NULL;
	if (strcmp(token->value, "to") == 0)
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
	else
		return token;
	free(token->value);
	return token;
}

static Token* get_num_token() {
	Token* token = malloc(sizeof(Token));
	if (token == NULL)
		return NULL;
	token->line_number = fixed_line_number;
	token->line_pos = fixed_line_pos;
	token->value = copy_int(atoi(buffer));
	if (token->value == NULL) {
		add_error(0, 0, get_msg(MESSAGES_TYPE_ERROR_OUT_MEMORY));
		free(token);
		return NULL;
	}
	buffer = NULL;
	token->token_type = TOKEN_NUM;
	return token;
}

static void process_num() {
	if (is_error())
	{
		next_ch();
		return;
	}
	fixed_line_number = line_number;
	fixed_line_pos = line_pos;
	append_char_to_buffer(current_ch);
	while (is_number(current_ch = next_ch())) {
		append_char_to_buffer(current_ch);
	}
	add_item(tokens_list, get_num_token());
}

static void check_num() {
	if (is_error())
	{
		next_ch();
		return;
	}
	fixed_line_number = line_number;
	fixed_line_pos = line_pos;
	if (is_number(next_ch())) {
		add_error(line_number, line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		return;
	}
	else {
		append_char_to_buffer('0');
		add_item(tokens_list, get_num_token());
	}
}

static void process_str() {
	if (is_error())
	{
		next_ch();
		return;
	}
	fixed_line_number = line_number;
	fixed_line_pos = line_pos;
	append_char_to_buffer(current_ch);
	while ((current_ch = next_ch()) != ' ' && !is_sep(current_ch) && current_ch != '\0') {
		append_char_to_buffer(current_ch);
	}
	add_item(tokens_list, convert_to_keyword(get_str_token(TOKEN_STR)));
}

static void process_quote_str() {
	if (is_error())
	{
		next_ch();
		return;
	}
	fixed_line_number = line_number;
	fixed_line_pos = line_pos;
	append_char_to_buffer(current_ch);
	while ((current_ch = next_ch()) != '\'') {
		if (current_ch == '\0') {
			add_error(line_number, line_pos, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_EOF));
			next_ch();
			return;
		}
		else if (current_ch == '\n') {
			line_number++;
			line_pos = 0;
		}
		append_char_to_buffer(current_ch);
	}
	add_item(tokens_list, get_str_token(TOKEN_QUOTE_STR));
	next_ch();
}

static void process_op(enum TOKENS token_op_type) {
	if (is_error())
	{
		next_ch();
		return;
	}
	add_item(tokens_list, get_keyword_token(token_op_type));
	next_ch();
}

static void process_name(enum TOKENS token_type) {
	if (is_error())
	{
		next_ch();
		return;
	}
	fixed_line_number = line_number;
	fixed_line_pos = line_pos;
	append_char_to_buffer(current_ch);
	while (is_letter(current_ch = next_ch()) || is_number(current_ch)) {
		append_char_to_buffer(current_ch);
	}
	add_item(tokens_list, get_str_token(token_type));
}

static void process_sep() {
	if (is_error())
	{
		next_ch();
		return;
	}
	add_item(tokens_list, get_keyword_token(TOKEN_SEP));
	next_ch();
}

static void init_start_lexer_state(char(*_func_ptr)(void)) {
	line_number = 1;
	line_pos = 0;
	fixed_line_number = 1;
	fixed_line_pos = 0;
	func_ptr = _func_ptr;
	current_ch = '\0';
	tokens_list = init_list();
	if (tokens_list == NULL)
		add_error(0, 0, get_msg(MESSAGES_TYPE_ERROR_OUT_MEMORY));
}

static void fill_tokens_list() {
	next_ch();
	while (true) {
		if (current_ch == ' ')
			next_ch();
		else if (current_ch == '\n') {
			line_number++;
			line_pos = 0;
			next_ch();
		}
		else if (current_ch == ';')
			process_sep();
		else if (current_ch == '+')
			process_op(TOKEN_OP_ADD);
		else if (current_ch == '-')
			process_op(TOKEN_OP_SUB);
		else if (current_ch == '=')
			process_op(TOKEN_OP_EQUALS);
		else if (current_ch == '!')
			process_op(TOKEN_OP_NOT);
		else if (current_ch == ':')
			process_name(TOKEN_MARK_NAME);
		else if (current_ch == '@')
			process_name(TOKEN_LOCAL_VAR_NAME);
		else if (current_ch == '$')
			process_name(TOKEN_GLOBAL_VAR_NAME);
		else if (current_ch == '>')
			process_op(TOKEN_RIGHT_DIR);
		else if (current_ch == '<')
			process_op(TOKEN_LEFT_DIR);
		else if (current_ch != '0' && is_number(current_ch))
			process_num();
		else if (current_ch == '0')
			check_num();
		else if (current_ch == '\'')
			process_quote_str();
		else if (current_ch == '\0')
			return;
		else if (current_ch == '(')
			process_op(TOKEN_LEFT_BRACKET);
		else if (current_ch == ')')
			process_op(TOKEN_RIGHT_BRACKET);
		else
		{
			process_str();
		}
	}
}

list* pack_tokens_list() {
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
		add_item(tokens_list, get_keyword_token(TOKEN_EOF));
		return tokens_list;
	}
}

list* tokenize(char(*next_ch_fn(void))) {
	init_start_lexer_state(next_ch_fn);
	fill_tokens_list();
	return pack_tokens_list();
}