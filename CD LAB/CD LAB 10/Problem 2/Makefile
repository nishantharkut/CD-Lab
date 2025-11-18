CC = gcc
YACC = bison
LEX = flex

all: parser

parser.tab.c parser.tab.h: parser.y
	$(YACC) -d parser.y

lex.yy.c: lexer.l parser.tab.h
	$(LEX) lexer.l

parser: lex.yy.c parser.tab.c
	$(CC) -o parser parser.tab.c lex.yy.c

clean:
	rm -f parser parser.tab.c parser.tab.h lex.yy.c *.exe

.PHONY: all clean
