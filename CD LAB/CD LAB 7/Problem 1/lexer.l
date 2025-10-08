%option noyywrap
%{
/* token codes must match parser.c */
#define ID 1
#define EQ 2
#define AST 3
#define DOLLAR 4
#include <string.h>
%}

%%

[ \t\r\n]+               ;                   /* skip whitespace */
"="                      { return EQ; }
"*"                      { return AST; }
"$"                      { return DOLLAR; }
[A-Za-z_][A-Za-z0-9_]*   { return ID; }

.                        { /* skip unknown */ }

%%
