#ifndef _PARSER_H
#define _PARSER_H

#include "ast.h"
#include "lexer.h"

extern struct ast_expr *parse_expr(struct lexer *l);

#endif
