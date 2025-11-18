%option noyywrap
%{
#include "palindrome.tab.h"
#include <stdio.h>
char input_buf[1024];
int input_len = 0;
%}

%%
a       { printf("Shift a\n"); 
          if (input_len < (int)sizeof(input_buf)-1) 
              input_buf[input_len++] = 'a'; 
          return 'a'; 
        }
b       { printf("Shift b\n"); 
          if (input_len < (int)sizeof(input_buf)-1) 
              input_buf[input_len++] = 'b'; 
          return 'b'; 
        }
\n      { return '\n'; }
[ \t\r]+
.       
%%
