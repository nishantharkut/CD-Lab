%option noyywrap
%{
#include "problem.tab.h"
#include <stdlib.h>
#include <stdio.h>

%}

%%
[0-9]+          { yylval = atoi(yytext); return NUM; }
[ \t\r\n]+    
[+\-*/()]      { return yytext[0]; }
.               {
	unsigned char b1 = (unsigned char)yytext[0];
	if (b1 == 0xE2) {
		int b2 = input();
		int b3 = input();
		if (b2 == 0x80 && (b3 == 0x93 || b3 == 0x94)) {
			return '-';
		}
		
		if (b3 != EOF) unput(b3);
		if (b2 != EOF) unput(b2);
		
	}
    
}
%%
