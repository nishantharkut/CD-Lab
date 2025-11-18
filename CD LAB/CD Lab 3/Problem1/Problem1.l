%{
#include <cstdio>
%}

INT     [0-9]+
FLOAT   [0-9]+\.[0-9]+

%%
{FLOAT}     { printf("%s is a floating-point number\n", yytext); }
{INT}       { printf("%s is not a floating-point number\n", yytext); }
[ \t\r]+    
\n          
.           
%%

int yywrap(void){ return 1; }
int main(void){
    printf("Enter Number(s):\n");
    yylex();
    return 0;
}