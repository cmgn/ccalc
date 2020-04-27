#include <stdlib.h>

#include "ast.h"

const char binary_op_tag_string[] = {
	'+',
	'-',
	'*',
	'/',
	'%',
	'^',
};

void free_ast_expr(struct ast_expr *e)
{
	switch (e->tag) {
	case AST_EXPR_CONSTANT:
		free(e);
		break;
	case AST_EXPR_BINARY_OP:
		free_ast_expr(e->binary_op.left);
		free_ast_expr(e->binary_op.right);
		free(e);
		break;
	}
}
