/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include "vm.h"

static word MEMORY[MAX_MEMORY_SIZE];
static ushort PC;
static ushort SP;

static session* _current_session;
static word stack_head;

static bool stack_pop() {
	if (SP != (ushort)MAX_MEMORY_SIZE-1) {
		stack_head = MEMORY[SP++];
		return true;
	}
	else {
		return false;
	}
}

static bool stack_push(word new_word) {
	if (SP > (ushort)MAX_MEMORY_SIZE - (ushort)MAX_STACK_SIZE) {
		MEMORY[SP--] = new_word;
		stack_head = new_word;
		return true;
	}
	else {
		return false;
	}
}

static item* get_var(const char* name) {
	if (name[1] == '@') {
		return find_item(_current_session->global_vars, name);
	}
	else {
		return find_item(_current_session->local_vars, name);
	}
}

static bool run_cmd() {
	word temp1;
	word temp2;
	item* temp_item1;
	item* temp_item2;
	while (true) {
		switch (MEMORY[PC].value) {
		case CMD_NOP:
			PC++;
			continue;
		case CMD_PUSH:
			PC++;
			if (stack_push(MEMORY[PC])) {
				PC++;
				continue;
			}
			else {
				return false;
			}
		case CMD_DROP:
			if (!stack_pop()) {
				return false;
			};
			PC++;
			continue;
		case CMD_SWAP:
			if (!stack_pop()) {
				return false;
			}
			temp1 = stack_head;
			if (!stack_pop()) {
				return false;
			}
			temp2 = stack_head;
			stack_push(temp1);
			stack_push(temp2);
			PC++;
			continue;
		case CMD_DUP:
			if (!stack_push(stack_head)) {
				return false;
			}
			PC++;
			continue;
		case CMD_ADD:
			if (!stack_pop()) {
				return false;
			}
			temp1 = stack_head;
			if (!stack_pop()) {
				return false;
			}
			temp2 = stack_head;
			if (temp1.word_type == 1) {
				if (temp2.word_type == 1) {
					temp2.value += temp1.value;
				}
				else if (temp2.word_type == 2) {
					temp_item2 = get_var(temp2.str);
					if (temp_item2 == NULL) {
						return false;
					}
					temp2.value = temp1.value + *(short*)temp_item2->value;
				}
				else {
					return false;
				}
				temp2.word_type = 1;
				stack_push(temp2);
				PC++;
				continue;
			}
			else if (temp1.word_type == 2) {
				if (temp2.word_type == 1) {
					temp_item1 = get_var(temp1.str);
					if (temp_item1 == NULL) {
						return false;
					}
					temp2.value += *(short*)temp_item1->value;
				}
				else if (temp2.word_type == 2) {
					temp_item1 = get_var(temp1.str);
					if (temp_item1 == NULL) {
						return false;
					}
					temp_item2 = get_var(temp2.str);
					if (temp_item2 == NULL) {
						return false;
					}
					temp2.value = *(short*)temp_item1->value + *(short*)temp_item2->value;
				}
				else {
					return false;
				}
				temp2.word_type = 1;
				stack_push(temp2);
				PC++;
				continue;
			}
			else if (temp1.word_type == 3) {
				if (temp2.word_type == 3) {
					temp_item1 = get_var(temp1.str);
					temp_item2 = get_var(temp2.str);
					if (temp_item1 == NULL || temp_item2 == NULL) {
						return false;
					}
					temp2.str = strcat(temp_item1->value, temp_item2->value);
				}
				else if (temp2.word_type == 4) {
					temp_item1 = get_var(temp1.str);
					if (temp_item1 == NULL) {
						return false;
					}
					temp2.str = strcat(temp_item1->value, temp2.str);
				}
				else {
					return false;
				}
				temp2.word_type = 4;
				stack_push(temp2);
				PC++;
				continue;
			}
			else if (temp1.word_type == 4) {
				if (temp2.word_type == 3) {
					temp_item2 = get_var(temp2.str);
					if (temp_item2 == NULL) {
						return false;
					}
					temp2.str = strcat(temp1.str, temp_item2->value);
				}
				else if (temp2.word_type == 4) {
					temp2.str = strcat(temp1.str, temp2.str);
				}
				else {
					return false;
				}
				temp2.word_type = 4;
				stack_push(temp2);
				PC++;
				continue;
			}
			switch (temp1.word_type) {
			case 1:
				
			case 2:
				
			case 3:
				temp_item1 = get_var(temp1.str);
				temp_item2 = get_var(temp2.str);
				if (temp_item1 == NULL || temp_item2 == NULL) {
					return false;
				}
				temp1.word_type = 5;
				temp1.str = strcat(temp_item1->value, temp_item2->value);
				stack_push(temp1);
				PC++;
				continue;
			case 4:

			}
		case CMD_SUB:
		case CMD_STOP:
		case CMD_NEG:
		case CMD_OVER:
		case CMD_GOTO:
		case CMD_SAVE:
		case CMD_LOAD:
		case CMD_IN:
		case CMD_OUT:
		case CMD_IFEQ:
		case CMD_IFNE:
		case CMD_IFLE:
		case CMD_IFLT:
		case CMD_IFGE:
		case CMD_IFGT:
		case CMD_RUN:
		case CMD_RF:
		case CMD_WFN:
		case CMD_WFA:
		default:

		}
	}
}

bool run_program(word* code, session* current_session) {
	if (code == NULL || current_session == NULL)
		return false;
}