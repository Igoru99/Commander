/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include "tests_lexer.h"

void _print_token(Token* token) {
	switch (token->token_type) {
	case	TOKEN_LOCAL_VAR_NAME:
		printf("TOKEN_LOCAL_VAR_NAME, name:\'%s\'\n", (char*)token->value);
		break;
		case			TOKEN_GLOBAL_VAR_NAME:
			printf("TOKEN_GLOBAL_VAR_NAME, name:\'%s\'\n", (char*)token->value);
			break;
		case			TOKEN_MARK_NAME:
			printf("TOKEN_MARK_NAME, mark:\'%s\'\n", (char*)token->value);
			break;
		case			TOKEN_STR:
			printf("TOKEN_STR, value:\'%s\'\n", (char*)token->value);
			break;
		case			TOKEN_QUOTE_STR:
			printf("TOKEN_QUOTE_STR, value:\'%s\'\n", (char*)token->value);
			break;
		case			TOKEN_NUM:
			printf("TOKEN_NUM, value:\'%d\'\n", (int)*(short*)token->value);
			break;
		case			TOKEN_FLAG:
			printf("TOKEN_FLAG, name:\'%s\'\n", (char*)token->value);
			break;
		case			TOKEN_SEP:
			printf("TOKEN_SEP\n");
			break;
		case		TOKEN_EOF:
			printf("TOKEN_EOF\n");
			break;
		case		TOKEN_OP_EQUALS:
			printf("TOKEN_OP_EQUALS\n");
			break;
		case		TOKEN_OP_ADD:
			printf("TOKEN_OP_ADD\n");
			break;
		case		TOKEN_OP_SUB:
			printf("TOKEN_OP_SUB\n");
			break;
		case		TOKEN_BOOL_OP_EQUALS:
			printf("TOKEN_BOOL_OP_EQUALS\n");
			break;
		case		TOKEN_BOOL_OP_NOT_EQUALS:
			printf("TOKEN_BOOL_OP_NOT_EQUALS\n");
			break;
		case		TOKEN_BOOL_OP_AND:
			printf("TOKEN_BOOL_OP_AND\n");
			break;
		case		TOKEN_BOOL_OP_OR:
			printf("TOKEN_BOOL_OP_OR\n");
			break;
		case		TOKEN_RIGHT_DIR:
			printf("TOKEN_RIGHT_DIR\n");
			break;
		case		TOKEN_DOUBLE_RIGHT_DIR:
			printf("TOKEN_DOUBLE_RIGHT_DIR\n");
			break;
		case		TOKEN_LEFT_BRACKET:
			printf("TOKEN_LEFT_BRACKET\n");
			break;
		case		TOKEN_RIGHT_BRACKET:
			printf("TOKEN_RIGHT_BRACKET\n");
			break;
		case		TOKEN_TO:
			printf("TOKEN_TO\n");
			break;
		case		TOKEN_IF:
			printf("TOKEN_IF\n");
			break;
		case		TOKEN_ELIF:
			printf("TOKEN_ELIF\n");
			break;
		case		TOKEN_ELSE:
			printf("TOKEN_ELSE\n");
			break;
		case		TOKEN_END:
			printf("TOKEN_END\n");
			break;
		case		TOKEN_THEN:
			printf("TOKEN_THEN\n");
			break;
		case		TOKEN_STDOUT:
			printf("TOKEN_STDOUT\n");
			break;
		case		TOKEN_STDIN:
			printf("TOKEN_STDIN\n");
			break;
		case		TOKEN_TRUE:
			printf("TOKEN_TRUE\n");
			break;
		case		TOKEN_FALSE:
			printf("TOKEN_FALSE\n");
			break;
	}
}

void _print_tokens(list* list) {
	node* current_node = list->first_node;
	for (ushort i = 0; i < list->count; i++)
	{
		_print_token(current_node->value);
		current_node = current_node->next_node;
	}
}

void _free_tokens_list(list* list) {
	if (list == NULL)
		return;
	node* current_node = list->first_node;
	for (ushort i = 0; i < list->count; i++)
	{
		free_token(current_node->value);
		current_node = current_node->next_node;
	}
	free_list(list);
}

void test_lexer() {
	list* list = NULL;
	const char* test_1 = "";
	const char* test_2 = ";;;;;;\n";
	const char* test_3 = "\0";
	const char* test_4 = ";-";
	const char* test_5 = "@rwetwer=asfasdfasdf";
	const char* test_6 = "@rwetwer=\'asfasdfasdf\'+werqwrqwrqe";
	const char* test_7 = "@a=-5;if ((@@a)==(-5)) then to mark end;";
	const char* test_8 = "echo \'Hello, world!\'";
	const char* test_9 = "@count = 5\n@number1 = 0\n@number2 = 1\n@counter = 0\n:do\nif @counter != @count then\n@counter = @counter + 1\nif @counter == 1 then\necho @number1\nend\nelif @counter == 2 then\necho @number2\nend\nelse\n@temp = @number1 + @number2\n@number1 = @number2\n@number2 = @temp\nend\nto do\nend\necho End";
	const char* test_10 = "@root = /dirs/;@dirs_count = 0;@index = 0;dc @root > @dirs_count;dl @root > dirs.txt;:do;if @index != @dirs_count then;    getL dirs.txt @index > @dir_name;    contain_number @dir_name > @is_number;    if @is_number then;        echo @dir_name;    end;    @index = @index + 1;    to do;end;";

	init_text_driver_from_stdin(test_1);
	list = tokenize(next_ch_stdin);
	assert("tokenize_test_1_true", list != NULL);
	_free_tokens_list(list);
	list = NULL;

	init_text_driver_from_stdin(test_2);
	list = tokenize(next_ch_stdin);
	assert("tokenize_test_2_true", list != NULL);
	_free_tokens_list(list);
	list = NULL;

	init_text_driver_from_stdin(test_3);
	list = tokenize(next_ch_stdin);
	assert("tokenize_test_3_true", list != NULL);
	_free_tokens_list(list);
	list = NULL;

	init_text_driver_from_stdin(test_4);
	list = tokenize(next_ch_stdin);
	assert("tokenize_test_4_false", list == NULL);
	list = NULL;
	get_error();

	init_text_driver_from_stdin(test_5);
	list = tokenize(next_ch_stdin);
	assert("tokenize_test_5_true", list != NULL);
	_free_tokens_list(list);
	list = NULL;

	init_text_driver_from_stdin(test_6);
	list = tokenize(next_ch_stdin);
	assert("tokenize_test_6_true", list != NULL);
	_free_tokens_list(list);
	list = NULL;

	init_text_driver_from_stdin(test_7);
	list = tokenize(next_ch_stdin);
	assert("tokenize_test_7_true", list != NULL);
	_free_tokens_list(list);
	list = NULL;

	init_text_driver_from_stdin(test_8);
	list = tokenize(next_ch_stdin);
	assert("tokenize_test_8_true", list != NULL);
	_free_tokens_list(list);
	list = NULL;

	init_text_driver_from_stdin(test_9);
	list = tokenize(next_ch_stdin);
	assert("tokenize_test_9_true", list != NULL);
	_free_tokens_list(list);
	list = NULL;

	init_text_driver_from_stdin(test_10);
	list = tokenize(next_ch_stdin);
	assert("tokenize_test_10_true", list != NULL);
	_free_tokens_list(list);
	list = NULL;
}