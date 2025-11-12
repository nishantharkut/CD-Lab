%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);


extern char input_buf[1024];
extern int input_len;
%}

%glr-parser
%start input

%%
input
    : S '\n' {
                    if (input_len >= 2 && input_buf[0] == input_buf[input_len-1]) {
              if (input_buf[0] == 'a') printf("Reduce S -> aSa\n");
              else if (input_buf[0] == 'b') printf("Reduce S -> bSb\n");
          } else if (input_len == 1) {
              
              
          } else if (input_len == 0) {
             
          }
          printf("ACCEPTED\n");
          input_len = 0;
      }
    | '\n'   { printf("ACCEPTED\n"); input_len = 0; }
    ;


S
    : 'a' S 'a'
    | 'b' S 'b'
    | 'a'
    | 'b'
    | 
    ;
%%

int main(void) {
    return yyparse();
}

void yyerror(const char *s) {
    printf("REJECTED\n");
}
