#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "ast.h"
#include "lexer.h"
#include "trace.h"
#include "parser.h"

void handle_sigfpe(int sigid)
{
	printf("terminating: floating point exception\n");
	exit(1);
}

int main(int argc, char **argv)
{
	if (signal(SIGFPE, handle_sigfpe) < 0) {
		perror("did not initialise floating point signal handler");
		return 1;
	}
	for (int i = 1; i < argc; i++) {
		struct lexer l = { argv[i], argv[i], 0 };
		struct ast_expr *e = parse_expr(&l);
		if (l.err) {
			printf("%s\n", l.err);
			continue;
		}
		trace_ast_expr(e);
		free_ast_expr(e);
	}
}
