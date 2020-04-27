CFLAGS = -Wall -Werror
MODULES = token.o lexer.o ast.o parser.o eval.o print.o

main: $(MODULES)
	$(CC) $(CFLAGS) -o main main.c $(MODULES)

clean:
	$(RM) main $(MODULES)
