%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *s){ fprintf(stderr,"Error: %s\n", s); }
%}

/* precedence */
%left '+' '-'
%left '*' '/'
%right '^'

%token NUMBER

%%
input:
  | input line
  ;

line:
    expr '\n'   { putchar('\n'); }  
  | '\n'        
  ;
  
expr:
    expr '+' expr  { putchar('+'); }
  | expr '-' expr  { putchar('-'); }
  | expr '*' expr  { putchar('*'); }
  | expr '/' expr  { putchar('/'); }
  | expr '^' expr  { putchar('^'); }
  | '(' expr ')' 
  | NUMBER         
  ;
%%
int main(void){
    return yyparse();
}
