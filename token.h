#ifndef _TOKEN_H
#define _TOKEN_H

enum token_tag {
	TOKEN_ERROR,
	TOKEN_EOF,
	TOKEN_INTEGER,
	TOKEN_ADD,
	TOKEN_SUB,
	TOKEN_MUL,
	TOKEN_DIV,
	TOKEN_MOD,
	TOKEN_POW,
	TOKEN_LEFT_BRACKET,
	TOKEN_RIGHT_BRACKET,
};

struct token {
	enum token_tag tag;
	union {
		int int_value;
		char *str_value;
	};
};

extern const char *token_tag_string[];

#endif
