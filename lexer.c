#include <ctype.h>

#include "lexer.h"

static int scan_integer(const char **s)
{
	int n = 0;
	while (isdigit(**s)) {
		n = n*10 + **s - '0';
		(*s)++;
	}
	return n;
}

static enum token_tag operator_type(char c)
{
	struct operator_pair {
		char key;
		enum token_tag tag;
	};
	static struct operator_pair pairs[] = {
		{'+', TOKEN_ADD},
		{'-', TOKEN_SUB},
		{'*', TOKEN_MUL},
		{'/', TOKEN_DIV},
		{'%', TOKEN_MOD},
		{'^', TOKEN_POW},
		{'(', TOKEN_LEFT_BRACKET},
		{')', TOKEN_RIGHT_BRACKET},
	};
	for (int i = 0; i < sizeof(pairs)/sizeof(*pairs); i++) {
		if (pairs[i].key == c) {
			return pairs[i].tag;
		}
	}
	return TOKEN_ERROR;
}

void next_token(struct lexer *l, struct token *t)
{
	for (;;) {
		char c = *l->pos++;
		if (isspace(c)) {
			continue;
		}
		if (!c) {
			t->tag = TOKEN_EOF;
			return;
		}
		if (isdigit(c)) {
			t->tag = TOKEN_INTEGER;
			l->pos--;
			t->int_value = scan_integer(&l->pos);
			return;
		}
		if (operator_type(c) != TOKEN_ERROR) {
			t->tag = operator_type(c);
			return;
		}
		// If we've exhaused all of our options we move the pointer
		// back over the character we couldn't handle, and report
		// an error.
		l->pos--;
		l->err = "unexpected character";
		return;
	}
}

void peek_token(struct lexer *l, struct token *t)
{
	const char *pos = l->pos;
	next_token(l, t);
	if (l->err) {
		return;
	}
	l->pos = pos;
}
