# Three-Address Code Generator using YACC

This program generates three-address code (TAC) for arithmetic expressions with proper operator precedence.

## Operator Precedence (Highest to Lowest)
1. Brackets `()` - highest
2. Exponent `^`
3. Multiplication/Division `*`, `/`
4. Addition/Subtraction `+`, `-` - lowest

## Features
- Parses arithmetic expressions with operators: `+`, `-`, `*`, `/`, `^`
- Generates three-address code step by step
- Supports assignment statements
- Uses temporary variables (t0, t1, t2, ...)

## Example
**Input:** `a = (2 + 3) * 4`

**Output:**
```
t0 = 2 + 3
t1 = t0 * 4
a = t1
```

## Compilation Instructions

### Using Make (recommended)
```bash
make
```

### Manual Compilation
```bash
bison -d parser.y
flex lexer.l
gcc -o parser parser.tab.c lex.yy.c -lfl
```

## Running the Program
```bash
./parser
```

Then enter an arithmetic expression when prompted.

## Test Cases

1. `a = (2 + 3) * 4`
2. `b = 10 + 5 * 2`
3. `c = 2 ^ 3 + 1`
4. `d = (5 + 3) / (2 - 1)`

## Clean Up
```bash
make clean
```
