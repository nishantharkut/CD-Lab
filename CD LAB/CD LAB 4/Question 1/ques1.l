%{
#include <stdio.h>
int spaced = 1;
%}

%option noyywrap

%%
[ \t\n]+     { if (!spaced) { putchar(' '); spaced = 1; } }
.            { ECHO; spaced = 0; }
%%

int main(void) {
    yylex();
    return 0;
}
