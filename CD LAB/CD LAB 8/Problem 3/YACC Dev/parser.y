%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%union {
  int ival;
}

%token <ival> NUM
%type  <ival> expr

%left '+' '-'
%left '*' '/'

%%

input:
  | input line
  ;

line:
    expr '\n'    { printf("= %d\n", $1); }
  | '\n'        
  ;

expr:
    expr '+' expr   { $$ = $1 + $3; }
  | expr '-' expr   { $$ = $1 - $3; }
  | expr '*' expr   { $$ = $1 * $3; }
  | expr '/' expr   { if ($3 == 0) { yyerror("division by zero"); $$ = 0; } else $$ = $1 / $3; }
  | '(' expr ')'    { $$ = $2; }
  | NUM             { $$ = $1; }
  ;

%%

void yyerror(const char *s){
  fprintf(stderr, "Error: %s\n", s);
}

int main(void){
  printf("Enter expressions (one per line). Ctrl+D to exit.\n");
  return yyparse();
}
