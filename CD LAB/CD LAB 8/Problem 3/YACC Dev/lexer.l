%{
#include "parser.tab.h"  
%}

%%

[ \t]+           ;                /* skip spaces */
[0-9]+           { yylval.ival = atoi(yytext); return NUM; }
\n               { return '\n'; }
"+"              { return '+'; }
"-"              { return '-'; }
"*"              { return '*'; }
"/"              { return '/'; }
"("              { return '('; }
")"              { return ')'; }
.                { return yytext[0]; }  /* any other single char token */

%%
int yywrap(void){ return 1; }
