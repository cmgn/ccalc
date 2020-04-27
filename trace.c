#include <stdio.h>

#include "ast.h"
#include "print.h"
#include "trace.h"
#include "eval.h"

static void internal_trace_ast_expr(struct ast_expr *root, struct ast_expr *expr)
{
	if (expr->tag == AST_EXPR_CONSTANT) {
		return;
	}
	internal_trace_ast_expr(root, expr->binary_op.left);
	internal_trace_ast_expr(root, expr->binary_op.right);
	int result = eval(expr);
	free_ast_expr(expr->binary_op.left);
	free_ast_expr(expr->binary_op.right);
	*expr = (struct ast_expr) {
		.tag = AST_EXPR_CONSTANT,
		.constant = (struct ast_expr_constant) {
			.value = result,
		},
	};
	print_ast_expr(root);
	printf("\n");
}

void trace_ast_expr(struct ast_expr *expr)
{
	print_ast_expr(expr);
	printf("\n");
	internal_trace_ast_expr(expr, expr);
}
