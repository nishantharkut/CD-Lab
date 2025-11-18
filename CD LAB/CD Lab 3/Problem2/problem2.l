%{
#include <stdio.h>
int Total_Tokens = 0;
int NumberOfKeywords = 0;
int NumberOfIdentifiers = 0;
int NumberOfConstants = 0;
int NumberOfOperators = 0;
int NumberOfInvalidToken = 0;
int NumberOfPunctuations = 0;
%}

KEYWORD (auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while)
IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*
CONSTANT [0-9]+
OPERATOR (:=|<=|>=|==|[+\-*/<>=%])
PUNCTUATION [\(\)\;\,\{\}\[\]\.]
INVALID [0-9a-zA-Z_]+
%%

{KEYWORD}       { printf("%s is a keyword\n", yytext); NumberOfKeywords++; Total_Tokens++; }
{IDENTIFIER}    { printf("%s is an identifier\n", yytext); NumberOfIdentifiers++; Total_Tokens++; }
{CONSTANT}      { printf("%s is a constant\n", yytext); NumberOfConstants++; Total_Tokens++; }
{OPERATOR}      { printf("%s is an operator\n", yytext); NumberOfOperators++; Total_Tokens++; }
{PUNCTUATION}   { printf("%s is a punctuation\n", yytext); NumberOfPunctuations++; Total_Tokens++; }
{INVALID}      { printf("%s is an invalid token\n", yytext); NumberOfInvalidToken++;}
[ \t\r\n]+     
.               { printf("%s is unknown\n", yytext); Total_Tokens++; }

%%

int yywrap(void) { return 1; }

int main(void){
    printf("Lexical Output:\n");
    yylex();
    printf("Number of Keywords : %d\n", NumberOfKeywords);
    printf("Number of Identifiers : %d\n", NumberOfIdentifiers);
    printf("Number of Constants : %d\n", NumberOfConstants);
    printf("Number of Operators : %d\n", NumberOfOperators);
    printf("Number of Punctuations : %d\n", NumberOfPunctuations);
    printf("Number of Invalid Tokens : %d\n", NumberOfInvalidToken);
    printf("Total Tokens : %d\n", Total_Tokens);
    
    return 0;
}