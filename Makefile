CFLAGS = -Wall -Werror -fPIC
BINARY = main
SHAREDLIB = ccalc.so
MODULES = token.o lexer.o ast.o parser.o eval.o print.o trace.o

main: lib
	$(CC) $(CFLAGS) main.c $(SHAREDLIB) -o $(BINARY)

lib: $(MODULES)
	$(CC) $(CFLAGS) $(MODULES) -shared -o $(SHAREDLIB)

clean:
	$(RM) $(BINARY) $(MODULES) $(SHAREDLIB)
