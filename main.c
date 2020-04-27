#include <stdio.h>

#include "ast.h"
#include "eval.h"
#include "lexer.h"
#include "parser.h"
#include "print.h"

int main(int argc, char **argv)
{
	for (int i = 1; i < argc; i++) {
		struct lexer l = { argv[i], argv[i], 0 };
		struct ast_expr *e = parse_expr(&l);
		print_ast_expr(e);
		printf("\n= %d\n", eval(e));
		free_ast_expr(e);
	}
}
