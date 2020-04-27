#include "ast.h"
#include "eval.h"

static int ipow(int x, int y)
{
	int r = 1;
	while (y-- > 0) {
		r *= x;
	}
	return r;
}

int eval(struct ast_expr *e)
{
	switch (e->tag) {
	case AST_EXPR_CONSTANT:
		return e->constant.value;
	case AST_EXPR_BINARY_OP: {
		int l = eval(e->binary_op.left);
		int r = eval(e->binary_op.right);
		switch (e->binary_op.tag) {
		case AST_BINARY_OP_ADD:
			return l + r;
		case AST_BINARY_OP_SUB:
			return l - r;
		case AST_BINARY_OP_MUL:
			return l * r;
		case AST_BINARY_OP_DIV:
			return l / r;
		case AST_BINARY_OP_MOD:
			return l % r;
		case AST_BINARY_OP_POW:
			return ipow(l, r);
		}
	}
	}
	// Unreachable.
	return 0;
}
