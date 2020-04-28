#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "print.h"
#include "trace.h"
#include "eval.h"

static struct ast_expr *copy_ast_expr(struct ast_expr *expr)
{
	// TODO(cmgn): Just terminating the program when malloc fails would probably
	// be easier?
	struct ast_expr *copy = malloc(sizeof(*copy));
	if (!copy) {
		goto free_nothing;
	}
	*copy = *expr;
	if (expr->tag == AST_EXPR_CONSTANT) {
		return copy;
	}
	copy->binary_op.left = copy_ast_expr(expr->binary_op.left);
	if (!copy->binary_op.left) {
		goto free_copy;
	}
	copy->binary_op.right = copy_ast_expr(expr->binary_op.right);
	if (!copy->binary_op.right) {
		goto free_copy_left;
	}
	return copy;

free_copy_left:
	free_ast_expr(copy->binary_op.left);
free_copy:
	free(copy);
free_nothing:
	return 0;
}

static void internal_trace_ast_expr(struct ast_expr *root, struct ast_expr *expr)
{
	if (expr->tag == AST_EXPR_CONSTANT) {
		return;
	}
	internal_trace_ast_expr(root, expr->binary_op.left);
	internal_trace_ast_expr(root, expr->binary_op.right);
	int result = eval_ast_expr(expr);
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
	struct ast_expr *copy = copy_ast_expr(expr);
	if (!copy) {
		printf("could not trace: malloc failed\n");
		return;
	}
	print_ast_expr(copy);
	printf("\n");
	internal_trace_ast_expr(copy, copy);
	free_ast_expr(copy);
}
