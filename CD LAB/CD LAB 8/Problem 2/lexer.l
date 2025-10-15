%option noyywrap
%{
#include "parser.tab.h"
#include <stdio.h>
%}

%%
[ \t\r]+         ;         
[0-9]+            { printf("%s", yytext); return NUMBER; }
"("               { return '('; }
")"               { return ')'; }
[\+\-\*\/\^]      { return yytext[0]; }
\n                { return '\n'; }
.                
%%
