#include <bits/stdc++.h>

#define MAX_STATES 20

int nfa[MAX_STATES][MAX_STATES];
int closure[MAX_STATES];         
int visited[MAX_STATES];         
int n;                           

void epsilon_closure(int state) {
    if (!visited[state]) {
        visited[state] = 1;
        closure[state] = 1;
        for (int next = 0; next < n; next++) {
            if (nfa[state][next]) {
                epsilon_closure(next);
            }
        }
    }
}

int main() {
    int i, j;

    printf("Enter number of states: ");
    scanf("%d", &n);

    printf("Enter ε-transition table (1 if ε-transition exists, else 0):\n");
    printf("Row = from state, Column = to state\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &nfa[i][j]);
        }
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            visited[j] = 0;
            closure[j] = 0;
        }

        epsilon_closure(i);

        printf("ε-closure(q%d) = { ", i);
        for (j = 0; j < n; j++) {
            if (closure[j]) {
                printf("q%d ", j);
            }
        }
        printf("}\n");
    }

    return 0;
}
