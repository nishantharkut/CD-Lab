%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char op;
    int value;
    int left;
    int right;
} Node;

Node dag[50];
int nodeCount = 0;

int addNode(char op, int left, int right, int value) {
    for (int i = 0; i < nodeCount; i++) {
        if (dag[i].op == op && dag[i].left == left && dag[i].right == right && dag[i].value == value)
            return i;
    }
    dag[nodeCount].op = op;
    dag[nodeCount].value = value;
    dag[nodeCount].left = left;
    dag[nodeCount].right = right;
    return nodeCount++;
}

void printDAG() {
    for (int i = 0; i < nodeCount; i++) {
        printf("Node : %d value : ", i);
        if (dag[i].op == 'n')
            printf("%d\n", dag[i].value);
        else
            printf("%c  left child at : %d  right child at : %d\n", dag[i].op, dag[i].left, dag[i].right);
    }
}

int yylex();
void yyerror(char *s) { fprintf(stderr, "%s\n", s); }

%}

%token NUM

%left '+' '-'
%left '*' '/'
%right UMINUS

%%
S   : E { printDAG(); }
    ;

E   : E '+' E { $$ = addNode('+', $1, $3, 0); }
    | E '-' E { $$ = addNode('-', $1, $3, 0); }
    | E '*' E { $$ = addNode('*', $1, $3, 0); }
    | E '/' E { $$ = addNode('/', $1, $3, 0); }
    | '(' E ')' { $$ = $2; }
    | NUM { $$ = addNode('n', -1, -1, $1); }
    | '-' E %prec UMINUS { $$ = addNode('m', $2, -1, 0); }
    ;
%%

int main() {
    yyparse();
    return 0;
}
