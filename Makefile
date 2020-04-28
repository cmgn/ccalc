CFLAGS = -Wall -Werror -fPIC -O3
BINARY = main
SHAREDLIB = ccalc.so
MODULES = token.o lexer.o ast.o parser.o eval.o print.o trace.o
RPATH = .

main: lib
	$(CC) $(CFLAGS) -Wl,-rpath=$(RPATH) main.c $(SHAREDLIB) -o $(BINARY)

lib: $(MODULES)
	$(CC) $(CFLAGS) $(MODULES) -shared -o $(SHAREDLIB)

clean:
	$(RM) $(BINARY) $(MODULES) $(SHAREDLIB)
