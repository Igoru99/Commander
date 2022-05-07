/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#pragma warning(disable : 4996)
#include "console.h"

// index 0 - current cmd
char _cmd_history[MAX_CMD_HISTORY][MAX_STDIN_LEN] = { '\0' };
ushort _next_pos = 0;

void _add_cmd_to_history(char* script) {
	for (ushort i = MAX_CMD_HISTORY - 1; i > 0; i--)
		strcpy(_cmd_history[i], _cmd_history[i - 1]);
	strcpy(_cmd_history[0], script);
	_next_pos = 0;
}

void start_stdin_listening() {
	while (true) {
		printf("> ");
		char* input[MAX_STDIN_LEN] = { '\0' };
		if (gets(input) == NULL)
		{
			printf(get_msg(MESSAGES_TYPE_INPUT_ERROR_STR));
			continue;
		}
		if (input[0] == '\0') {
			printf(get_msg(MESSAGES_TYPE_LAST_COMMAND), _cmd_history[_next_pos]);
			printf(get_msg(MESSAGES_TYPE_RUN_LAST_COMMAND));
			_next_pos = (_next_pos + 1) % MAX_CMD_HISTORY;
			continue;
		}
		else if (input[0] == '+') {
			ushort current_pos = (_next_pos - 1) % MAX_CMD_HISTORY;
			strcpy(input, _cmd_history[current_pos]);
			printf("> %s\n", _cmd_history[current_pos]);
		}
		_add_cmd_to_history(input);
		run_script(input);
	}
}