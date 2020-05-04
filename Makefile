CFLAGS = -Wall -Werror -O3
BINARY = main
MODULES = token.o lexer.o ast.o parser.o eval.o print.o trace.o

main: $(MODULES)
	$(CC) $(CFLAGS) $(MODULES) main.c -o $(BINARY)

clean:
	$(RM) $(BINARY) $(MODULES) $(SHAREDLIB)
