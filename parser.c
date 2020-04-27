#include <stdlib.h>

#include "ast.h"
#include "lexer.h"
#include "parser.h"

// constant:
//   INTEGER
static struct ast_expr *parse_constant(struct lexer *l)
{
	struct token t;
	next_token(l, &t);
	if (l->err) {
		return 0;
	}
	if (t.tag != TOKEN_INTEGER) {
		l->err = "expected integer";
		return 0;
	}
	struct ast_expr *e = malloc(sizeof(*e));
	if (!e) {
		l->err = "failed to allocate";
		return 0;
	}
	e->tag = AST_EXPR_CONSTANT;
	e->constant.value = t.int_value;
	return e;
}

// bracketed:
//   constant
//   '(' expr ')'
static struct ast_expr *parse_bracketed(struct lexer *l)
{
	struct token t;
	peek_token(l, &t);
	if (l->err) {
		return 0;
	}
	if (t.tag != TOKEN_LEFT_BRACKET) {
		return parse_constant(l);
	}
	next_token(l, &t);
	struct ast_expr *expr = parse_expr(l);
	if (!expr) {
		return 0;
	}
	next_token(l, &t);
	if (t.tag != TOKEN_RIGHT_BRACKET) {
		l->err = "expected right bracket";
		free_ast_expr(expr);
		return 0;
	}
	return expr;
}

// negation:
//   bracketed
//   '-' negation
static struct ast_expr *parse_negation(struct lexer *l)
{
	struct token t;
	peek_token(l, &t);
	if (l->err) {
		return 0;
	}
	if (t.tag != TOKEN_SUB) {
		return parse_bracketed(l);
	}
	next_token(l, &t);
	struct ast_expr *negation = parse_negation(l);
	if (!negation) {
		return 0;
	}
	struct ast_expr *top = malloc(sizeof(*top));
	if (!top) {
		l->err = "failed to allocate";
		free_ast_expr(negation);
		return 0;
	}
	struct ast_expr *left = malloc(sizeof(*left));
	if (!left) {
		l->err = "failed to allocate";
		free_ast_expr(negation);
		// Not yet initialised.
		free(top);
		return 0;
	}
	top->tag = AST_EXPR_BINARY_OP;
	top->binary_op.tag = AST_BINARY_OP_MUL;
	top->binary_op.left = left;
	top->binary_op.right = negation;
	left->tag = AST_EXPR_CONSTANT;
	left->constant.value = -1;
	return top;
}

// exponentiation
//   negation
//   negation '^' exponentiation
static struct ast_expr *parse_exponentiation(struct lexer *l)
{
	struct ast_expr *left = parse_negation(l);
	struct token t;
	peek_token(l, &t);
	if (t.tag != TOKEN_POW) {
		return left;
	}
	next_token(l, &t);
	struct ast_expr *right = parse_exponentiation(l);
	if (!right) {
		free_ast_expr(left);
		return 0;
	}
	struct ast_expr *binop = malloc(sizeof(*binop));
	if (!binop) {
		l->err = "failed to allocate";
		free_ast_expr(left);
		free_ast_expr(right);
		return 0;
	}
	binop->tag = AST_EXPR_BINARY_OP;
	binop->binary_op.tag = AST_BINARY_OP_POW;
	binop->binary_op.left = left;
	binop->binary_op.right = right;
	return binop;
}

// TODO(cmgn): @CLEANUP: reduce the duplication between the
// parse_{product,sum} functions.

// product:
//   exponentiation
//   product '*' exponentiation
//   product '/' exponentiation
//   product '%' exponentiation
static struct ast_expr *parse_product(struct lexer *l)
{
	struct ast_expr *left = parse_exponentiation(l);
	if (!left) {
		return 0;
	}
	for (;;) {
		struct token op;
		peek_token(l, &op);
		if (l->err) {
			free_ast_expr(left);
			return 0;
		}
		if (op.tag != TOKEN_MUL && op.tag != TOKEN_DIV && op.tag != TOKEN_MOD) {
			return left;
		}
		next_token(l, &op);
		struct ast_expr *right = parse_exponentiation(l);
		if (!right) {
			free_ast_expr(left);
			return 0;
		}
		struct ast_expr *binop = malloc(sizeof(*binop));
		if (!binop) {
			l->err = "failed to allocate";
			free_ast_expr(left);
			free_ast_expr(right);
			return 0;
		}
		binop->tag = AST_EXPR_BINARY_OP;
		switch (op.tag) {
		case TOKEN_MUL:
			binop->binary_op.tag = AST_BINARY_OP_MUL;
			break;
		case TOKEN_DIV:
			binop->binary_op.tag = AST_BINARY_OP_DIV;
			break;
		case TOKEN_MOD:
			binop->binary_op.tag = AST_BINARY_OP_MOD;
			break;
		default:
			// Unreachable.
			break;
		}
		binop->binary_op.left = left;
		binop->binary_op.right = right;
		left = binop;
	}
}

// sum:
//   product
//   sum '+' product
//   sum '-' product
static struct ast_expr *parse_sum(struct lexer *l)
{
	struct ast_expr *left = parse_product(l);
	if (!left) {
		return 0;
	}
	for (;;) {
		struct token op;
		peek_token(l, &op);
		if (l->err) {
			free_ast_expr(left);
			return 0;
		}
		if (op.tag != TOKEN_ADD && op.tag != TOKEN_SUB) {
			return left;
		}
		next_token(l, &op);
		struct ast_expr *right = parse_product(l);
		if (!right) {
			free_ast_expr(left);
			return 0;
		}
		struct ast_expr *binop = malloc(sizeof(*binop));
		if (!binop) {
			l->err = "failed to allocate";
			free_ast_expr(left);
			free_ast_expr(right);
			return 0;
		}
		binop->tag = AST_EXPR_BINARY_OP;
		binop->binary_op.tag = (op.tag == TOKEN_ADD)
			? AST_BINARY_OP_ADD
			: AST_BINARY_OP_SUB;
		binop->binary_op.left = left;
		binop->binary_op.right = right;
		left = binop;
	}
}

extern struct ast_expr *parse_expr(struct lexer *l)
{
	return parse_sum(l);
}
