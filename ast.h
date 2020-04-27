#ifndef _AST_H
#define _AST_H

struct ast_expr;
struct ast_expr_constant;
struct ast_expr_binary_op;

enum ast_expr_tag {
	AST_EXPR_CONSTANT,
	AST_EXPR_BINARY_OP,
};

enum ast_binary_op_tag {
	AST_BINARY_OP_ADD,
	AST_BINARY_OP_SUB,
	AST_BINARY_OP_MUL,
	AST_BINARY_OP_DIV,
	AST_BINARY_OP_MOD,
	AST_BINARY_OP_POW,
};

extern const char binary_op_tag_string[];

struct ast_expr_constant {
	int value;
};

struct ast_expr_binary_op {
	enum ast_binary_op_tag tag;
	struct ast_expr *left;
	struct ast_expr *right;
};

struct ast_expr {
	enum ast_expr_tag tag;
	union {
		struct ast_expr_constant constant;
		struct ast_expr_binary_op binary_op;
	};
};

extern void free_ast_expr(struct ast_expr *);

#endif
