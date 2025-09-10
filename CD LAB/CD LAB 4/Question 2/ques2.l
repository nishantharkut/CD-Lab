%{
#include <stdio.h>
#include <ctype.h>
%}

%option noyywrap

%%

[A-Za-z]+   {
                char *yytext_copy = yytext;
                int len = yyleng;
                char first = yytext_copy[0];
                if (strchr("AEIOUaeiou", first)) {
                    printf("%say", yytext_copy);
                } else {    
                    printf("%s%cay", yytext_copy+1, first);
                }
            }

.           { ECHO; }

%%

int main(void) {
    yylex();
    return 0;
}
