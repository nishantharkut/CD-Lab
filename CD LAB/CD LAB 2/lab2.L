%{
#include <stdio.h>
#include <string.h>

int line_no = 1;
%}

DIGIT       [0-9]+
ID          [a-zA-Z_][a-zA-Z0-9_]*
OP          (\+|\-|\*|\/|:=|<|>|<=|>=|==)
SEMI        ;
COMMA       ,
WS          [ \t]+
NEWLINE     \n

%%

"Prog"              { printf("Regex: \"Prog\"\t\tToken: 1 (PROGRAM)\n"); }
"Integer"           { printf("Regex: \"Integer\"\tToken: 2 (INTEGER)\n"); }
"Begin"             { printf("Regex: \"Begin\"\t\tToken: 3 (BEGIN)\n"); }
"read"              { printf("Regex: \"read\"\t\tToken: 4 (READ)\n"); }
"write"             { printf("Regex: \"write\"\t\tToken: 5 (WRITE)\n"); }
"if"                { printf("Regex: \"if\"\t\tToken: 6 (IF)\n"); }
"then"              { printf("Regex: \"then\"\t\tToken: 7 (THEN)\n"); }
"else"              { printf("Regex: \"else\"\t\tToken: 8 (ELSE)\n"); }
"endif"             { printf("Regex: \"endif\"\tToken: 9 (ENDIF)\n"); }
"while"             { printf("Regex: \"while\"\tToken: 10 (WHILE)\n"); }
"do"                { printf("Regex: \"do\"\t\tToken: 11 (DO)\n"); }
"endwhile"          { printf("Regex: \"endwhile\"\tToken: 12 (ENDWHILE)\n"); }
"end"               { printf("Regex: \"end\"\t\tToken: 13 (END)\n"); }

{DIGIT}             { printf("Regex: [0-9]+\t\tToken: 14 (NUMBER) -> %s\n", yytext); }
{ID}                { printf("Regex: [a-zA-Z_][a-zA-Z0-9_]*\tToken: 15 (IDENTIFIER) -> %s\n", yytext); }
{OP}                { printf("Regex: OPERATOR\t\tToken: 16 (OPERATOR) -> %s\n", yytext); }
{SEMI}              { printf("Regex: ;\t\tToken: 17 (SEMICOLON)\n"); }
{COMMA}             { printf("Regex: ,\t\tToken: 18 (COMMA)\n"); }

{WS}                { /* ignore spaces */ }
{NEWLINE}           { line_no++; }

.                   { printf("Unknown symbol: %s at line %d\n", yytext, line_no); }

%%

int yywrap(void) {
    return 1;
}

int main() {
    printf("Lexical Analysis Output:\n");
    yylex();
    return 0;
}
