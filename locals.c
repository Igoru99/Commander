/*
* Writen by Dozhdikov Igor (igoru99@gmail.com)
* License: GNU GPL v2.0
* (C) 2022 year.
*/

#include "locals.h"

const char* const _MESSAGES_ENG[] = {
	{"Loading script error.\n"},
	{"Tap <Enter> to exit.\n"},
	{"Enter \'help'\ for more details.\n"},
	{"Last command: %s\n"},
	{"Enter \'+\' and tap <Enter> for runnning this command.\n"},
	{"Input entry error.\n"},
	{"Unexcepted symbol.\n"},
	{"Not enough memory.\n"},
	{"Unexcepted end of file.\n"},
};


const char* const _MESSAGES_RUS[] = {
	{"��� �������� ������� ��������� ������.\n"},
	{"������� <Enter> ��� ������.\n"},
	{"������� \'help'\ ��� ��������� �������.\n"},
	{"���������� �������: %s\n"},
	{"��� ���������� ���� ������� ������� \'+\' � ������� <Enter>.\n"},
	{"��� ����� ������ ��������� ������.\n"},
	{"����������� ������.\n"},
	{"������������ ������.\n"},
	{"����������� ����� �����.\n"},
};

enum LANGUAGES _current_lang = DEFAULT_LANG;

void set_lang(enum LANGUAGES lang) {
	_current_lang = lang;
}

const char *const get_msg(enum MESSAGES_TYPE msg_type) {
	switch (_current_lang) {
	case LANGUAGES_ENG:
		return _MESSAGES_ENG[msg_type];
	case LANGUAGES_RUS:
		return _MESSAGES_RUS[msg_type];
	default:
		return "Checking language error.\n";
	}
}