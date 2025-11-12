%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
void yyerror(const char *s);

int temp_count = 0;
int reduction_count = 0;
int data_type_size = 4;
char* new_temp();
void generate_code(const char *op, const char *arg1, const char *arg2, const char *result);
void print_reduction(const char *msg);

typedef struct {
    char name[50][50];
    int dimensions[50][10];
    int dim_count[50];
    int count;
} ArrayTable;

ArrayTable arrays;

void add_array(const char *name, int *dims, int dim_count);
int get_array_index(const char *name);
char* calculate_offset(const char *array_name, char **indices, int num_indices);

%}

%union {
    int num;
    char *id;
    char *addr;
    struct {
        int dims[10];
        int count;
    } dim_list;
    struct {
        char *indices[10];
        int count;
    } index_list;
    struct {
        char *name;
        char *indices[10];
        int index_count;
    } array_info;
}

%token <num> NUMBER
%token <id> ID TYPE
%token ASSIGN PLUS MINUS MUL DIV POW LPAREN RPAREN LBRACKET RBRACKET COMMA SEMICOLON NEWLINE

%type <addr> expr term factor power primary
%type <dim_list> dimension_list
%type <array_info> array_access

%left PLUS MINUS
%left MUL DIV
%right POW

%%

program:
    /* empty */
    | program statement
    ;

statement:
    array_declaration
    | assignment_statement
    | expression_statement
    | NEWLINE
    ;

array_declaration:
    TYPE ID dimension_list SEMICOLON NEWLINE {
        add_array($2, $3.dims, $3.count);
        free($1);
        free($2);
    }
    ;

dimension_list:
    LBRACKET NUMBER RBRACKET {
        $$.dims[0] = $2;
        $$.count = 1;
    }
    | dimension_list LBRACKET NUMBER RBRACKET {
        $$ = $1;
        $$.dims[$$.count++] = $3;
    }
    ;

assignment_statement:
    array_access ASSIGN expr SEMICOLON NEWLINE {
        char *offset = calculate_offset($1.name, $1.indices, $1.index_count);
        print_reduction("After reduction number");
        
        // Build the array access string with all indices
        char result[200];
        strcpy(result, $1.name);
        for (int i = 0; i < $1.index_count; i++) {
            strcat(result, "[");
            strcat(result, $1.indices[i]);
            strcat(result, "]");
        }
        
        printf("%s = %s\n", result, $3);
        free($1.name);
        for (int i = 0; i < $1.index_count; i++) {
            free($1.indices[i]);
        }
        free(offset);
        free($3);
    }
    | ID ASSIGN expr SEMICOLON NEWLINE {
        print_reduction("After reduction number");
        printf("%s = %s\n", $1, $3);
        free($1);
        free($3);
    }
    ;

expression_statement:
    expr SEMICOLON NEWLINE {
        free($1);
    }
    ;

array_access:
    ID LBRACKET expr RBRACKET {
        $$.name = $1;
        $$.indices[0] = $3;
        $$.index_count = 1;
    }
    | array_access LBRACKET expr RBRACKET {
        $$ = $1;
        $$.indices[$$.index_count++] = $3;
    }
    ;

expr:
    expr PLUS term {
        print_reduction("After reduction number");
        char *temp = new_temp();
        generate_code("+", $1, $3, temp);
        $$ = temp;
        free($1);
        free($3);
    }
    | expr MINUS term {
        print_reduction("After reduction number");
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
        print_reduction("After reduction number");
        char *temp = new_temp();
        generate_code("*", $1, $3, temp);
        $$ = temp;
        free($1);
        free($3);
    }
    | term DIV factor {
        print_reduction("After reduction number");
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
        print_reduction("After reduction number");
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
        print_reduction("After reduction number");
        char *temp = new_temp();
        printf("%s = %d\n", temp, $1);
        $$ = temp;
    }
    | ID {
        $$ = $1;
    }
    | array_access {
        char *offset = calculate_offset($1.name, $1.indices, $1.index_count);
        print_reduction("After reduction number");
        char *temp = new_temp();
        printf("%s = %s[%s]\n", temp, $1.name, offset);
        free($1.name);
        for (int i = 0; i < $1.index_count; i++) {
            free($1.indices[i]);
        }
        free(offset);
        $$ = temp;
    }
    ;

%%

void add_array(const char *name, int *dims, int dim_count) {
    strcpy(arrays.name[arrays.count], name);
    for (int i = 0; i < dim_count; i++) {
        arrays.dimensions[arrays.count][i] = dims[i];
    }
    arrays.dim_count[arrays.count] = dim_count;
    arrays.count++;
}

int get_array_index(const char *name) {
    for (int i = 0; i < arrays.count; i++) {
        if (strcmp(arrays.name[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

char* new_temp() {
    char *temp = (char*)malloc(20);
    sprintf(temp, "t%d", temp_count++);
    return temp;
}

void generate_code(const char *op, const char *arg1, const char *arg2, const char *result) {
    printf("%s = %s %s %s\n", result, arg1, op, arg2);
}

void print_reduction(const char *msg) {
    reduction_count++;
    printf("%s %d\n", msg, reduction_count);
}

char* calculate_offset(const char *array_name, char **indices, int num_indices) {
    int arr_idx = get_array_index(array_name);
    if (arr_idx == -1) {
        // Array not in table, just return first index
        if (num_indices > 0) {
            return strdup(indices[0]);
        }
        return strdup("0");
    }
    
    if (num_indices == 1) {
        // 1D array: offset = index * dimension_size * data_type_size
        int dim_size = (arr_idx >= 0 && arrays.dim_count[arr_idx] > 0) ? arrays.dimensions[arr_idx][0] : 1;
        int multiplier = dim_size * data_type_size;
        char mult_str[20];
        sprintf(mult_str, "%d", multiplier);
        print_reduction("After reduction number");
        char *temp = new_temp();
        printf("%s = %s * %s\n", temp, indices[0], mult_str);
        return temp;
    }
    
    // Multi-dimensional array
    // For A[i][j] with dimensions [3][3]:
    // offset = (i * 3 + j) * data_type_size
    char *result = strdup(indices[0]);
    
    for (int i = 1; i < num_indices; i++) {
        // Multiply by dimension size
        int dim_size = arrays.dimensions[arr_idx][i];
        char dim_str[20];
        sprintf(dim_str, "%d", dim_size);
        
        print_reduction("After reduction number");
        char *temp1 = new_temp();
        printf("%s = %s * %s\n", temp1, result, dim_str);
        free(result);
        
        print_reduction("After reduction number");
        char *temp2 = new_temp();
        printf("%s = %s + %s\n", temp2, temp1, indices[i]);
        free(temp1);
        
        result = temp2;
    }
    
    // Multiply by data type size
    char size_str[20];
    sprintf(size_str, "%d", data_type_size);
    print_reduction("After reduction number");
    char *final_temp = new_temp();
    printf("%s = %s * %s\n", final_temp, result, size_str);
    free(result);
    
    return final_temp;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    arrays.count = 0;
    
    printf("Enter size of data type :\n");
    scanf("%d", &data_type_size);
    getchar(); // consume newline
    
    printf("Enter no of arrays :\n");
    int num_arrays;
    scanf("%d", &num_arrays);
    getchar(); // consume newline
    
    // Predefined array names
    char predefined_names[10][50] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};
    
    for (int i = 0; i < num_arrays; i++) {
        printf("Enter no of dimension of %d array :\n", i + 1);
        int num_dims;
        scanf("%d", &num_dims);
        getchar(); // consume newline
        
        printf("Enter dimensions of %d array :\n", i + 1);
        int dims[10];
        for (int j = 0; j < num_dims; j++) {
            scanf("%d", &dims[j]);
        }
        getchar(); // consume newline
        
        add_array(predefined_names[i], dims, num_dims);
    }
    
    printf("\nEnter Expression ending with Semicolon\n");
    yyparse();
    return 0;
}
