#ifndef _LEXER_H
#define _LEXER_H

#include "token.h"

struct lexer {
	const char *src;
	const char *pos;
	const char *err;
};

extern void next_token(struct lexer *l, struct token *t);
extern void peek_token(struct lexer *l, struct token *t);

#endif
