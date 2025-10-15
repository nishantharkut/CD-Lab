%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double bin_to_dec(char *s);
void yyerror(char *s);
int yylex(void);
%}

%union {
  char *s;
}

%token <s> BINARY

%%
input:
    BINARY {
        double val = bin_to_dec($1);
        if (fabs(val - round(val)) < 1e-9)
            printf("%.0f\n", val);
        else
            printf("%.6f\n", val);
        free($1);
    }
;
%%

int main(void){
    while (1) {
        printf("Enter the binary number: ");
        if (yyparse() != 0) break;
    }
    return 0;
}

void yyerror(const char *s){
    fprintf(stderr, "Parse error: %s\n", s);
}

double bin_to_dec(const char *s){
    double val = 0.0;
    const char *p = s;

    while (*p && *p != '.') {
        if (*p == '0' || *p == '1')
            val = val * 2.0 + (*p - '0');
        p++;
    }

    if (*p == '.') {
        p++;
        double weight = 0.5;
        while (*p) {
            if (*p == '0' || *p == '1') {
                val += weight * (*p - '0');
                weight *= 0.5;
            }
            p++;
        }
    }

    return val;
}
