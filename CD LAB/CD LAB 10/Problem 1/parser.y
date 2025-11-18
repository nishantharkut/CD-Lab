%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
void yyerror(const char *s);

int temp_count = 0;
char* new_temp();
void generate_code(const char *op, const char *arg1, const char *arg2, const char *result);

%}

%union {
    int num;
    char *id;
    char *addr;
}

%token <num> NUMBER
%token <id> ID
%token ASSIGN PLUS MINUS MUL DIV POW LPAREN RPAREN NEWLINE

%type <addr> expr term factor power primary

%left PLUS MINUS
%left MUL DIV
%right POW

%%

program:
    /* empty */
    | program statement
    ;

statement:
    ID ASSIGN expr NEWLINE {
        printf("%s = %s\n", $1, $3);
        free($1);
        free($3);
    }
    | expr NEWLINE {
        printf("Result = %s\n", $1);
        free($1);
    }
    | NEWLINE
    ;

expr:
    expr PLUS term {
        char *temp = new_temp();
        generate_code("+", $1, $3, temp);
        $$ = temp;
        free($1);
        free($3);
    }
    | expr MINUS term {
        char *temp = new_temp();
        generate_code("-", $1, $3, temp);
        $$ = temp;
        free($1);
        free($3);
    }
    | term {
        $$ = $1;
    }
    ;

term:
    term MUL factor {
        char *temp = new_temp();
        generate_code("*", $1, $3, temp);
        $$ = temp;
        free($1);
        free($3);
    }
    | term DIV factor {
        char *temp = new_temp();
        generate_code("/", $1, $3, temp);
        $$ = temp;
        free($1);
        free($3);
    }
    | factor {
        $$ = $1;
    }
    ;

factor:
    power POW factor {
        char *temp = new_temp();
        generate_code("^", $1, $3, temp);
        $$ = temp;
        free($1);
        free($3);
    }
    | power {
        $$ = $1;
    }
    ;

power:
    primary {
        $$ = $1;
    }
    ;

primary:
    LPAREN expr RPAREN {
        $$ = $2;
    }
    | NUMBER {
        char *temp = new_temp();
        printf("%s = %d;\n", temp, $1);
        $$ = temp;
    }
    | ID {
        $$ = $1;
    }
    ;

%%

char* new_temp() {
    char *temp = (char*)malloc(20);
    sprintf(temp, "t%d", temp_count++);
    return temp;
}

void generate_code(const char *op, const char *arg1, const char *arg2, const char *result) {
    printf("%s = %s %s %s;\n", result, arg1, op, arg2);
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Enter an expression (e.g., a = (2 + 3) * 4):\n");
    printf("I/p: ");
    yyparse();
    return 0;
}
