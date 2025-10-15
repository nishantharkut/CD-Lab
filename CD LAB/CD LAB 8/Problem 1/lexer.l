%option noyywrap
%{
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>
%}


%%
[ \t\r]+         
[01]+(\.[01]+)?  {
                    yylval.s = strdup(yytext);
                    return BINARY;
                 }
\n               { return 0; }   
.                
%%
