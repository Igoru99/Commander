/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include "generator.h"

static dict* _addresses = NULL;
static list* _tokens_list = NULL;
static node* _current_node = NULL;
static node* _last_node = NULL;
static bool _is_first_node = true;
static ushort _current_construction_pos = 0;

static Token* _next_token() {
	if (_tokens_list == NULL)
		return NULL;
	if (_is_first_node)
	{
		_current_node = _tokens_list->first_node;
		_is_first_node = false;
	}
	else
	{
		_last_node = _current_node;
		_current_node = _current_node->next_node;
	}
	return _current_node == NULL ? NULL : (Token*)_current_node->value;
}

static void _free_construction(Construction* c);

static void _free_operand(Operand* operand) {
	if (operand == NULL)
		return;
	switch (operand->operand_type) {
	case OPERAND_TYPES_INT_CONST:
	case OPERAND_TYPES_STRING_CONST:
	case OPERAND_TYPES_INT_VAR:
	case OPERAND_TYPES_STRING_VAR:
		free(operand->value);
	}
	free(operand);
}

static void _free_arithmetic_expression(ArithmeticExpression* exp) {
	if (exp == NULL)
		return;
	for (ushort i = 0; i < exp->operands_count; i++)
		free(exp->operands + i);
	free(exp);
}

static void _free_logic_expression(LogicExpression* exp) {
	if (exp == NULL)
		return;
	for (ushort i = 0; i < exp->operands_count; i++)
		free(exp->operands + i);
	free(exp);
}

static void _free_set_construction(SetConstruction* set) {
	if (set == NULL)
		return;
	_free_arithmetic_expression(set->value);
	free(set->var_name);
	free(set);
}

static void _free_get_construction(GetConstruction* get) {
	if (get == NULL)
		return;
	free(get->var_name);
	free(get);
}

static void _free_goto_construction(GotoConstruction* goto_c) {
	if (goto_c == NULL)
		return;
	free(goto_c);
}

static void _free_run_construction(RunConstruction* run) {
	if (run == NULL)
		return;
	for (ushort i = 0; i < run->args_count; i++)
		free(run->cmd_args + i);
	free(run->cmd_name);
	free(run);
}

static void _free_logic_construction(LogicConstruction* logic) {
	if (logic == NULL)
		return;
	_free_logic_expression(logic->condition);
	free(logic);
}

static void _free_redirect_construction(RedirectConstruction* rc) {
	if (rc == NULL)
		return;
	_free_construction(rc->input_construction);
	_free_construction(rc->output_construction);
	free(rc);
}

static void _free_construction(Construction* c) {
	if (c == NULL)
		return NULL;
	switch (c->construction_type) {
	case CONSTRUCTION_TYPES_SET:
		_free_set_construction(c);
		break;
	case CONSTRUCTION_TYPES_GET:
		_free_get_construction(c);
		break;
	case CONSTRUCTION_TYPES_GOTO:
		_free_goto_construction(c);
		break;
	case CONSTRUCTION_TYPES_LOGIC:
		_free_logic_construction(c);
		break;
	case CONSTRUCTION_TYPES_RUN:
		_free_run_construction(c);
		break;
	case CONSTRUCTION_TYPES_RIGHT_DIR:
	case CONSTRUCTION_TYPES_RIGHT_DOUBLE_DIR:
		_free_redirect_construction(c);
		break;
	}
	free(c);
}

static void free_script(Script* script) {
	if (script == NULL)
		return NULL;
	for (ushort i = 0; i < script->count; i++)
		_free_construction(script->constructions + i);
	free(script);
}


static Operand* _generate_const_int_operand() {
}

static Operand* _generate_var_int_operand() {
}

static Operand* _generate_const_string_operand() {
}

static Operand* _generate_var_string_operand() {
}

static Operand* _generate_const_true_operand() {
}

static Operand* _generate_const_false_operand() {
}


static Operand* _generate_operand() {
}

static LogicExpression* _generate_logic_expression() {
}

static ArithmeticExpression* _generate_arithmetic_expression() {
}

static GotoConstruction* _generate_goto_construction() {
}

static LogicConstruction* _generate_logic_construction() {

}

static SetConstruction* _generate_set_construction() {

}

static GetConstruction* _generate_get_construction() {

}

static RunConstruction* _generate_run_construction() {

}

static LogicConstruction* _generate_logic_construction(enum LOGIC_CONSTRUCTION_TYPES logic_construction_type) {

}

static RedirectConstruction* _generate_redirect_construction(bool is_double_dir) {

}

static Construction* _generate_get_or_redirect_construction() {
	if (_next_token() == NULL)
	{
		add_error(0, 0, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_EOF));
		return NULL;
	}
	switch (((Token*)_current_node)->token_type) {
	case TOKEN_DOUBLE_RIGHT_DIR:
		return _generate_extern_construction(_generate_redirect_construction(true), CONSTRUCTION_TYPES_RIGHT_DOUBLE_DIR);
	case TOKEN_RIGHT_DIR:
		return _generate_extern_construction(_generate_redirect_construction(false), CONSTRUCTION_TYPES_RIGHT_DIR);
	case TOKEN_SEP:
	case TOKEN_END:
		// get value construction or run construction
	default:
		add_error(((Token*)_current_node->value)->line_number, ((Token*)_current_node->value)->line_number, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
		return NULL;
	}
}

static Construction* _generate_end_construction() {

}

static Construction* _generate_extern_construction(Construction* inner_construction, enum CONSTRUCTION_TYPES construction_type) {
	Construction* extern_c = malloc(sizeof(Construction));
	if (extern_c == NULL) {
		add_error(0, 0, get_msg(MESSAGES_TYPE_ERROR_OUT_MEMORY));
		return NULL;
	}
	extern_c->value = inner_construction;
	extern_c->construction_type = construction_type;
	return extern_c;
}

static Construction* _generate_construction() {
	if (_next_token() == NULL)
	{
		add_error(0, 0, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_EOF));
		return NULL;
	}
	while (true) {
		switch (((Token*)_current_node)->token_type) {
		case TOKEN_LOCAL_VAR_NAME:
		case TOKEN_GLOBAL_VAR_NAME:
		case TOKEN_STR:
		case TOKEN_QUOTE_STR:
		case TOKEN_STDIN:
			return _generate_get_or_redirect_construction();
		case TOKEN_MARK_NAME:
			add_iitem(_addresses, (char*)((Token*)_current_node->value)->value, (short)_current_construction_pos);
			continue;
		case TOKEN_SEP:
			continue;
		case TOKEN_EOF:
			return _generate_extern_construction(_generate_end_construction(), CONSTRUCTION_TYPES_END);
		case TOKEN_TO:
			return _generate_extern_construction(_generate_goto_construction(), CONSTRUCTION_TYPES_GOTO);
		case TOKEN_IF:
			return _generate_extern_construction(_generate_logic_construction(LOGIC_CONSTRUCTION_TYPES_IF), CONSTRUCTION_TYPES_LOGIC);
		case TOKEN_ELIF:
			return _generate_extern_construction(_generate_logic_construction(LOGIC_CONSTRUCTION_TYPES_ELIF), CONSTRUCTION_TYPES_LOGIC);
		case TOKEN_ELSE:
			return _generate_extern_construction(_generate_logic_construction(LOGIC_CONSTRUCTION_TYPES_ELSE), CONSTRUCTION_TYPES_LOGIC);
		default:
			add_error(((Token*)_current_node->value)->line_number, ((Token*)_current_node->value)->line_number, get_msg(MESSAGES_TYPE_ERROR_UNEXCEPTED_SYMBOL));
			return NULL;
		}
	}
}

Script* _do_first_cycle(list* tokens_list) {
	if (tokens_list == NULL)
		return NULL;
	_current_node = NULL;
	_is_first_node = true;
	_tokens_list = tokens_list;
	_current_construction_pos = 0;
	list* constructions_list = init_list();

	if (constructions_list == NULL)
	{
		add_error(0, 0, get_msg(MESSAGES_TYPE_ERROR_OUT_MEMORY));
		return NULL;
	}

	Construction* current_construction = NULL;

	while ((current_construction = _generate_construction()) != NULL && current_construction->construction_type != CONSTRUCTION_TYPES_END) {
		add_item(constructions_list, current_construction);
	}

	if (is_error()) {
		node* current_node = constructions_list->first_node;
		for (ushort i = 0; i < constructions_list->count; i++)
		{
			_free_construction(current_node->value);
			current_node = current_node->next_node;
		}
		free_list(constructions_list);
		return NULL;
	}
	else {
		add_item(constructions_list, current_construction);
		return constructions_list;
	}
}

Script* _do_second_cycle(Script* script) {
	if (script == NULL)
		return NULL;
	_addresses = NULL;
}

Script* generate(list* tokens_list) {
	return _do_second_cycle(_do_first_cycle(tokens_list));
}