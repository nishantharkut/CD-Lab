%{
#include <stdio.h>
%}

%%
^[ \t]*[a-zA-Z]([ \t]*[+\-*/][ \t]*[a-zA-Z])*[ \t]*(\n|$)    { printf("It is a Valid expression\n"); }
^[^\n]*(\n|$)                                 { printf("It is not a Valid Expression\n"); }
%%

int yywrap (void){
    return 1;
}

int main(void){
    yylex();
    return 0;
}


