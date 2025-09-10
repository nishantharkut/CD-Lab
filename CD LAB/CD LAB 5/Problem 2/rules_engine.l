%option noyywrap
%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//strings ending with 00
int rule_a(const char *s){ int n=strlen(s); return n>=2 && s[n-1]=='0' && s[n-2]=='0'; }

//strings with the three consecutive 222's
int rule_b(const char *s){ return strstr(s,"222") != NULL; }

//set of all strings such that every block of five consecutive symbols contains at least two 5’s.
int rule_c(const char *s){
    int n=strlen(s);
    if(n < 5) return 0; /* requires at least one block of length 5 */
    for(int i=0;i<=n-5;i++){
        int cnt=0;
        for(int j=0;j<5;j++) if(s[i+j]=='5') cnt++;
        if(cnt < 2) return 0;
    }
    return 1;
}

//The set of all strings beginning with a 1 which, interpreted as the binary
//representation of an integer, is congruent to zero modulo 5.
int rule_d(const char *s){
    int n=strlen(s);
    if(n<1 || s[0] != '1') return 0;
    for(int i=0;i<n;i++) if(s[i] != '0' && s[i] != '1') return 0;
    int v=0;
    for(int i=0;i<n;i++) v = (v*2 + (s[i]-'0')) % 5;
    return v == 0;
}

//The set of all strings such that the 10th symbol from the right end is 1.
int rule_e(const char *s){
    int n=strlen(s);
    if(n < 10) return 0;
    return s[n-10] == '1';
}

// The set of all four digits numbers whose sum is 9
int rule_f(const char *s){
    if(strlen(s) != 4) return 0;
    int sum=0;
    for(int i=0;i<4;i++) sum += s[i]-'0';
    return sum == 9;
}

// The set of all four digital numbers, whose individual digits are in ascending order
// from left to right.
int rule_g(const char *s){
    if(strlen(s) != 4) return 0;
    return s[0] < s[1] && s[1] < s[2] && s[2] < s[3];
}

void classify(const char *tok){
    if(rule_a(tok)) printf("%s match rule a\n", tok);
    else if(rule_b(tok)) printf("%s match rule b\n", tok);
    else if(rule_c(tok)) printf("%s match rule c\n", tok);
    else if(rule_d(tok)) printf("%s match rule d\n", tok);
    else if(rule_e(tok)) printf("%s match rule e\n", tok);
    else if(rule_f(tok)) printf("%s match rule f\n", tok);
    else if(rule_g(tok)) printf("%s match rule g\n", tok);
    else printf("%s : no rule matched\n", tok);
}
%}

%%

[0-9]+    { classify(yytext); }
[ \t\r\n]+    { /* skip whitespace */ }
.         { printf("%s : invalid token\n", yytext); }

%%

int main(int argc, char **argv){
    yylex();
    return 0;
}
