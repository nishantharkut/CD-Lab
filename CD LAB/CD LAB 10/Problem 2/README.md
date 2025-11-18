# Three-Address Code Generator for Arrays using YACC

This program extends the basic three-address code generator to handle arrays and multi-dimensional arrays.

## Features
- Handles array declarations with dimensions
- Supports multi-dimensional array indexing (e.g., A[i][j])
- Generates three-address code for array operations
- Tracks reductions for array expressions using temporary variables (t1, t2, etc.)

## Supported Operations
- Array declarations: `int A[3][3];`
- Array indexing: `A[i][j]`, `B[k]`, etc.
- Array assignments with expressions: `A[i][j] = B[k] + 5;`
- All arithmetic operators: `+`, `-`, `*`, `/`, `^`

## Input Format
1. Size of data type (e.g., 4 for int)
2. Number of arrays
3. For each array:
   - Number of dimensions
   - Size of each dimension
4. Expression ending with semicolon

## Example

**Input:**
```
4
1
2
3 3
A[i][j] = B[k] + 5;
```

**Output:**
```
After reduction number 1
t0 = B[k]
After reduction number 2
t1 = 5
After reduction number 3
t2 = t0 + t1
After reduction number 4
A[i][j] = t2
```

## Compilation

### Using Make
```bash
make
```

### Manual Compilation
```bash
bison -d parser.y
flex lexer.l
gcc -o parser parser.tab.c lex.yy.c
```

## Running the Program
```bash
./parser < input.txt
```

Or run interactively:
```bash
./parser
```

## How It Works
1. The parser reads array declarations and stores them in an array table
2. When parsing expressions, each array access is reduced to a temporary variable
3. Arithmetic operations are performed on temporaries
4. Each reduction is numbered and displayed
5. Final assignment shows the complete three-address code

## Clean Up
```bash
make clean
```
