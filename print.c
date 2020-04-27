#include <stdio.h>

#include "ast.h"
#include "print.h"

#define NO_ASSOC 0
#define LEFT_ASSOC 1
#define RIGHT_ASSOC 2

static int expr_assoc(struct ast_expr *expr)
{
	if (expr->tag == AST_EXPR_CONSTANT) {
		return NO_ASSOC;
	}
	// For now, the only right associative operator is ^.
	if (expr->binary_op.tag == AST_BINARY_OP_POW) {
		return RIGHT_ASSOC;
	}
	return LEFT_ASSOC;
}

static int expr_precedence(struct ast_expr *expr)
{
	if (expr->tag == AST_EXPR_CONSTANT) {
		return 1000;
	}
	switch (expr->binary_op.tag) {
	case AST_BINARY_OP_ADD:
	case AST_BINARY_OP_SUB:
		return 100;
	case AST_BINARY_OP_MUL:
	case AST_BINARY_OP_DIV:
	case AST_BINARY_OP_MOD:
		return 200;
	case AST_BINARY_OP_POW:
		return 300;
	default:
		// Unreachable.
		return 0;
	}
}

void print_ast_expr(struct ast_expr *expr)
{
	if (expr->tag == AST_EXPR_CONSTANT) {
		printf("%d", expr->constant.value);
		return;
	}
	int prec = expr_precedence(expr);
	int lprec = expr_precedence(expr->binary_op.left);
	int lassoc = expr_assoc(expr->binary_op.left);
	if (lprec < prec || (lprec == prec && lassoc == RIGHT_ASSOC)) {
		printf("(");
		print_ast_expr(expr->binary_op.left);
		printf(")");
	} else {
		print_ast_expr(expr->binary_op.left);
	}
	printf(" %c ", binary_op_tag_string[expr->binary_op.tag]);
	int rprec = expr_precedence(expr->binary_op.right);
	int rassoc = expr_assoc(expr->binary_op.right);
	if (rprec < prec || (rprec == prec && rassoc == LEFT_ASSOC)) {
		printf("(");
		print_ast_expr(expr->binary_op.right);
		printf(")");
	} else {
		print_ast_expr(expr->binary_op.right);
	}
}
