# Compiler Design Lab - Complete Implementation Guide

This repository contains comprehensive implementations of compiler design concepts through 10 progressive laboratory assignments. Each lab focuses on different phases and techniques of compiler construction.

##  Table of Contents
- [Overview](#overview)
- [Lab Assignments](#lab-assignments)
- [Technologies Used](#technologies-used)
- [Installation & Setup](#installation--setup)
- [Directory Structure](#directory-structure)

---

## Overview

This collection represents a complete journey through compiler design, from basic lexical analysis to advanced code generation techniques. The labs progressively build understanding of:
- Lexical Analysis (Tokenization)
- Finite Automata & Regular Expressions
- Syntax Analysis (Parsing)
- Grammar Analysis
- Syntax-Directed Translation
- Code Generation & Optimization

---

## Lab Assignments

### Lab 1: Introduction to Compiler Tools
**Objective:** Familiarization with compiler construction tools and basic concepts

**Topics Covered:**
- Introduction to Lex and Yacc
- Basic compiler phases
- Understanding tokenization

**Files:** `CD LAB/CD LAB 1/`

---

### Lab 2: Lexical Analysis Fundamentals
**Objective:** Implement lexical analyzers using Lex and understand finite automata concepts

#### Problem 1: Lexical Analyzer for Mini Programming Language
- **Tool:** Lex/Flex
- **Description:** Tokenizes a simple programming language with keywords, identifiers, operators, and punctuation
- **Keywords Recognized:** `Prog`, `Integer`, `Begin`, `read`, `write`, `if`, `then`, `else`, `endif`, `while`, `do`, `endwhile`, `end`
- **Features:**
  - Identifies 18 different token types
  - Tracks line numbers
  - Recognizes operators: `+`, `-`, `*`, `/`, `:=`, `<`, `>`, `<=`, `>=`, `==`
  - Handles semicolons, commas, and whitespace
- **File:** `lab2.L`

#### Problem 2: Epsilon (ε)-Closure Computation
- **Tool:** C++
- **Description:** Computes epsilon closures for NFA states
- **Concepts:**
  - Non-deterministic Finite Automata (NFA)
  - Epsilon transitions
  - Closure operations using DFS
- **File:** `problem 2.cpp`

#### Problem 3: Identifier Validation
- **Tool:** C++
- **Description:** Validates identifiers based on predefined rules
- **File:** `problem 3.cpp`

**Files:** `CD LAB/CD LAB 2/`

---

### Lab 3: Advanced Lexical Analysis
**Objective:** Master pattern matching and token classification

#### Problem 1: Floating-Point Number Detector
- **Tool:** Lex
- **Description:** Distinguishes between integers and floating-point numbers
- **Pattern Recognition:**
  - Integer: `[0-9]+`
  - Float: `[0-9]+\.[0-9]+`
- **File:** `Problem1/Problem1.l`

#### Problem 2: C Language Lexical Analyzer
- **Tool:** Lex
- **Description:** Complete lexical analyzer for C language tokens
- **Categorizes:**
  - Keywords (30+ C keywords)
  - Identifiers
  - Constants
  - Operators (`:=`, `<=`, `>=`, `==`, arithmetic, relational)
  - Punctuation symbols
  - Invalid tokens
- **Statistics Tracking:**
  - Count of each token type
  - Total tokens processed
- **File:** `Problem2/problem2.l`

#### Problem 3: Whitespace Normalizer
- **Tool:** Lex
- **Description:** Removes extra whitespace and normalizes spacing in text
- **Functionality:**
  - Collapses multiple spaces/tabs/newlines into single space
  - Preserves non-whitespace characters
- **File:** `Problem3/Problem3.l`

#### Problem 4: Advanced Pattern Recognition
- **Tool:** C++
- **Description:** Custom pattern matching implementation
- **File:** `Problem4/problem4.cpp`

**Files:** `CD LAB/CD Lab 3/`

---

### Lab 4: Text Processing with Lex
**Objective:** Apply lexical analysis to practical text transformation tasks

#### Question 1: Whitespace Compactor
- **Tool:** Lex
- **Description:** Compresses consecutive whitespace into single space
- **File:** `Question 1/ques1.l`

#### Question 2: Pig Latin Translator
- **Tool:** Lex
- **Description:** Converts English text to Pig Latin
- **Rules:**
  - Words starting with vowels: append "ay"
  - Words starting with consonants: move first letter to end and append "ay"
- **Example:** "hello" → "ellohay", "apple" → "appleay"
- **File:** `Question 2/ques2.l`

#### Question 3: Custom Text Processor
- **Tool:** C++
- **Description:** Advanced text processing algorithms
- **Files:** `Question 3/ques3.cpp`, `ques3copy.cpp`

#### Question 4: Additional Text Analysis
- **Tool:** C++
- **Description:** Specialized text analysis and transformation
- **File:** `Question 4/ques4.cpp`

**Files:** `CD LAB/CD LAB 4/`

---

### Lab 5: Parsing Techniques
**Objective:** Implement parsers and understand grammar analysis

#### Problem 1: Recursive Descent Parsing
- **Tool:** C++
- **Description:** Implements recursive descent parser with backtracking
- **Grammar:** 
  - `S → cAd`
  - `A → ab | a`
- **Features:**
  - Tracks function calls
  - Counts backtracks
  - Performance metrics
- **Input Examples:**
  - "cabd" → ACCEPT
  - "cad" → ACCEPT
  - "cab" → REJECT
- **Files:** 
  - `Problem 1/parsing_a.cpp` (Version A)
  - `Problem 1/parsing_b.cpp` (Version B)

#### Problem 2: Rules Engine with Regular Expressions
- **Tool:** Lex
- **Description:** Implements multiple complex pattern matching rules
- **Rules:**
  - **Rule A:** Strings ending with "00"
  - **Rule B:** Strings containing three consecutive 2's ("222")
  - **Rule C:** Every block of 5 consecutive symbols contains ≥2 5's
  - **Rule D:** Binary strings starting with 1, divisible by 5
  - **Rule E:** 10th symbol from right is '1'
  - **Rule F:** 4-digit numbers with digit sum = 9
  - **Rule G:** 4-digit numbers with digits in ascending order
- **File:** `Problem 2/rules_engine.l`

#### Problem 3: Matrix Operations Parser
- **Tool:** Lex
- **Description:** Lexical analysis for matrix operations
- **Features:**
  - Recognizes matrix notation
  - Handles multi-dimensional arrays
  - Processes matrix expressions
- **File:** `Problem 3/matrix_3.l`

#### Problem 4: Grammar Analysis
- **Tool:** C++
- **Description:** Grammar parsing and validation
- **File:** `Problem 4/p4.cpp`

**Files:** `CD LAB/CD LAB 5/`

---

### Lab 6: FIRST and LAST Sets Computation
**Objective:** Understand grammar analysis fundamentals for parser construction

#### Problem 1: LAST Set Computation
- **Tool:** C++
- **Description:** Computes LAST sets for context-free grammar
- **Algorithm:**
  - Iterative fixed-point computation
  - Handles epsilon (ε) productions
  - Works with arbitrary CFGs
- **Concepts:**
  - Bottom-up parsing preparation
  - Follow set computation foundation
  - Grammar analysis
- **Applications:**
  - LR parser construction
  - Error recovery in parsing
- **File:** `Problem 1/solution.cpp`

#### Problem 2: FIRST Set Computation
- **Tool:** C++
- **Description:** Computes FIRST sets for context-free grammar
- **Features:**
  - Handles nullable non-terminals
  - Supports epsilon productions
  - Iterative algorithm until convergence
- **Use Cases:**
  - LL parser table construction
  - Predictive parsing
  - Grammar validation
- **File:** `Problem 2/Solution2.cpp`

**Files:** `CD LAB/CD LAB 6/`

---

### Lab 7: LR Parser Construction
**Objective:** Implement LR(0) and LR(1) parsing techniques

#### Problem 1: LR(0) Parser with Automaton
- **Tool:** C + Lex
- **Description:** Complete LR(0) parser implementation
- **Components:**
  - LR(0) item construction
  - Canonical collection of LR(0) items
  - DFA state generation
  - Parsing table construction
- **Grammar Support:**
  - Handles augmented grammars
  - Computes closures and goto functions
  - Detects shift-reduce and reduce-reduce conflicts
- **Files:**
  - `Problem 1/parser.c` (Parser implementation)
  - `Problem 1/lexer.l` (Lexical analyzer)

#### Problem 2: LR(1) Parser
- **Tool:** C++
- **Description:** LR(1) parser with lookahead
- **Features:**
  - More powerful than LR(0)
  - Handles a broader class of grammars
  - Resolves conflicts using lookahead
- **File:** `Problem 2/lr1_program2.cpp`

**Files:** `CD LAB/CD LAB 7/`

---

### Lab 8: Yacc/Bison - Syntax Directed Translation
**Objective:** Master Yacc for parser generation and implement calculators

#### Problem 1: Binary to Decimal Converter
- **Tools:** Bison + Flex
- **Description:** Converts binary numbers (including decimals) to decimal
- **Features:**
  - Supports integer and fractional binary numbers
  - Handles binary point (e.g., "101.11")
  - Accurate floating-point conversion
- **Algorithm:**
  - Integer part: multiply by 2, add digit
  - Fractional part: divide by 2^position
- **Files:**
  - `Problem 1/parser.y` (Bison grammar)
  - `Problem 1/lexer.l` (Flex tokenizer)

#### Problem 2: Arithmetic Expression Parser
- **Tools:** Bison + Flex
- **Description:** Full-featured arithmetic calculator
- **Operators Supported:**
  - Addition (`+`), Subtraction (`-`)
  - Multiplication (`*`), Division (`/`)
  - Exponentiation (`^`)
  - Parentheses for precedence
- **Features:**
  - Proper operator precedence
  - Left/Right associativity
  - Error handling
- **Files:**
  - `Problem 2/parser.y`
  - `Problem 2/lexer.l`

#### Problem 3: Advanced Expression Evaluator
- **Implementation:** Both C++ and Yacc versions provided

##### CPP Solution
- **Tool:** C++
- **Description:** Custom expression evaluator
- **File:** `Problem 3/CPP Solution/solution.cpp`

##### Yacc Version
- **Tools:** Bison + Flex
- **Description:** Parser-based evaluator with SDT
- **Files:**
  - `Problem 3/YACC Dev/parser.y`
  - `Problem 3/YACC Dev/lexer.l`

**Files:** `CD LAB/CD LAB 8/`

---

### Lab 9: Intermediate Code Generation
**Objective:** Generate intermediate representations (DAG, Three-Address Code)

#### Problem 1: Directed Acyclic Graph (DAG) Generation
- **Tools:** Bison + Flex
- **Description:** Constructs DAG for expression optimization
- **Features:**
  - Common sub-expression elimination
  - Node sharing for identical operations
  - Optimized expression representation
- **Output Format:**
  - Node number
  - Operation/value
  - Left and right child pointers
- **Example:**
  - Input: `a + b + a`
  - DAG eliminates redundant computation of `a`
- **Files:**
  - `Problem 1/problem.y`
  - `Problem 1/lexer.l`

#### Problem 2: Palindrome Checker
- **Tools:** Bison + Flex
- **Description:** Syntax-directed palindrome verification
- **Features:**
  - Recognizes palindromic strings
  - Grammar-based validation
  - Immediate feedback
- **Files:**
  - `Problem 2/palindrome.y`
  - `Problem 2/lexer.l`

**Files:** `CD LAB/CD LAB 9/`

---

### Lab 10: Three-Address Code Generation
**Objective:** Generate optimized intermediate code for complex expressions

#### Problem 1: Three-Address Code Generator
- **Tools:** Bison + Flex
- **Description:** Generates TAC for arithmetic expressions with proper precedence
- **Operator Precedence (High → Low):**
  1. Parentheses `()`
  2. Exponentiation `^`
  3. Multiplication/Division `*`, `/`
  4. Addition/Subtraction `+`, `-`
- **Features:**
  - Temporary variable generation (t0, t1, t2, ...)
  - Handles assignment statements
  - Step-by-step code generation
- **Example:**
  ```
  Input:  a = (2 + 3) * 4
  Output:
    t0 = 2 + 3
    t1 = t0 * 4
    a = t1
  ```
- **Files:**
  - `Problem 1/parser.y`
  - `Problem 1/lexer.l`
  - `Problem 1/Makefile`
  - `Problem 1/README.md` (Detailed documentation)

#### Problem 2: Array Three-Address Code Generator
- **Tools:** Bison + Flex
- **Description:** Extended TAC generator for array operations
- **Features:**
  - Handles multi-dimensional arrays
  - Array indexing: `A[i][j]`, `B[k]`
  - Array declarations with dimensions
  - Address calculation for array elements
- **Input Format:**
  - Data type size
  - Number of arrays with dimensions
  - Expression with array operations
- **Example:**
  ```
  Input:
    4              (size of int)
    1              (1 array)
    2              (2 dimensions)
    3 3            (3x3 array)
    A[i][j] = B[k] + 5;
  
  Output:
    After reduction number 1
    t0 = B[k]
    After reduction number 2
    t1 = 5
    After reduction number 3
    t2 = t0 + t1
    After reduction number 4
    A[i][j] = t2
  ```
- **Files:**
  - `Problem 2/parser.y`
  - `Problem 2/lexer.l`
  - `Problem 2/Makefile`
  - `Problem 2/README.md`

**Files:** `CD LAB/CD LAB 10/`

---

## Technologies Used

### Programming Languages
- **C** - Low-level parser implementations
- **C++** - Algorithm implementations, data structures
- **Lex/Flex** - Lexical analyzer generation
- **Yacc/Bison** - Parser generation

### Tools & Utilities
- **GNU Compiler Collection (GCC)** - C/C++ compilation
- **Make** - Build automation
- **Git** - Version control

### Compiler Construction Tools
- **Flex** - Fast lexical analyzer generator
- **Bison** - GNU parser generator (Yacc replacement)

---

## Installation & Setup

### Prerequisites

#### For Linux/WSL:
```bash
# Install Flex and Bison
sudo apt-get update
sudo apt-get install flex bison gcc g++ make

# Verify installations
flex --version
bison --version
gcc --version
```

#### For Windows:
1. Install MinGW-w64 or MSYS2
2. Install Flex and Bison through package manager
3. Add to system PATH

#### For macOS:
```bash
# Using Homebrew
brew install flex bison gcc make

# Add to PATH if needed
export PATH="/usr/local/opt/bison/bin:$PATH"
export PATH="/usr/local/opt/flex/bin:$PATH"
```

### Compilation Instructions

#### For Lex Programs:
```bash
flex filename.l
gcc lex.yy.c -o output -lfl
./output < input.txt
```

#### For Yacc/Bison Programs:
```bash
bison -d parser.y
flex lexer.l
gcc parser.tab.c lex.yy.c -o parser -lfl
./parser
```

#### For C++ Programs:
```bash
g++ -o program filename.cpp
./program
```

#### Using Makefiles:
Most labs with complex builds include Makefiles:
```bash
make          # Compile
./parser      # Run
make clean    # Clean build artifacts
```

---

## Directory Structure

```
CD LAB/
├── CD LAB 1/           # Introduction
├── CD LAB 2/           # Lexical Analysis Basics
│   ├── lab2.L
│   ├── problem 2.cpp
│   └── problem 3.cpp
├── CD Lab 3/           # Advanced Lexical Analysis
│   ├── Problem1/       # Float detector
│   ├── Problem2/       # C lexer
│   ├── Problem3/       # Whitespace normalizer
│   └── Problem4/
├── CD LAB 4/           # Text Processing
│   ├── Question 1/     # Whitespace compactor
│   ├── Question 2/     # Pig Latin
│   ├── Question 3/
│   └── Question 4/
├── CD LAB 5/           # Parsing
│   ├── Problem 1/      # Recursive descent
│   ├── Problem 2/      # Rules engine
│   ├── Problem 3/      # Matrix parser
│   └── Problem 4/
├── CD LAB 6/           # FIRST/LAST Sets
│   ├── Problem 1/      # LAST computation
│   └── Problem 2/      # FIRST computation
├── CD LAB 7/           # LR Parsing
│   ├── Problem 1/      # LR(0) parser
│   └── Problem 2/      # LR(1) parser
├── CD LAB 8/           # Yacc Basics
│   ├── Problem 1/      # Binary converter
│   ├── Problem 2/      # Calculator
│   └── Problem 3/      # Expression evaluator
├── CD LAB 9/           # Intermediate Code
│   ├── Problem 1/      # DAG generation
│   └── Problem 2/      # Palindrome checker
└── CD LAB 10/          # Code Generation
    ├── Problem 1/      # Three-address code
    └── Problem 2/      # Array TAC
```

---

## Key Concepts Covered

### Phase 1: Lexical Analysis
- Token recognition and classification
- Regular expressions and pattern matching
- Finite automata (NFA, DFA, ε-closure)
- Lex/Flex programming

### Phase 2: Syntax Analysis
- Context-free grammars
- Parsing techniques (recursive descent, LR)
- FIRST and FOLLOW sets
- Parse tree construction
- Conflict resolution

### Phase 3: Semantic Analysis
- Syntax-directed translation
- Attribute grammars
- Type checking foundations

### Phase 4: Intermediate Code Generation
- Three-address code
- Directed Acyclic Graph (DAG)
- Temporary variable management
- Expression optimization

### Phase 5: Code Optimization (Introductory)
- Common sub-expression elimination
- Array address calculation
- Reduction tracking

---

## Common Commands Reference

### Flex (Lex)
```bash
flex file.l                    # Generate lex.yy.c
gcc lex.yy.c -o prog -lfl     # Compile with flex library
./prog < input.txt            # Run with input file
```

### Bison (Yacc)
```bash
bison -d parser.y             # Generate parser.tab.c and parser.tab.h
flex lexer.l                  # Generate lex.yy.c
gcc parser.tab.c lex.yy.c -o parser -lfl
./parser                      # Run interactively
```

### C++ Compilation
```bash
g++ -o program file.cpp       # Compile
./program                     # Run
g++ -std=c++11 file.cpp       # Compile with C++11
```

### Debugging
```bash
bison -v parser.y             # Generate parser.output (verbose)
flex -d lexer.l               # Debug mode for lexer
gcc -g file.c                 # Compile with debug symbols
gdb ./program                 # Use GDB debugger
```

---

## Learning Path Recommendation

1. **Start with Lab 2-3**: Master lexical analysis with Lex
2. **Progress to Lab 4**: Apply lexical concepts to practical problems
3. **Move to Lab 5-6**: Understand parsing fundamentals and grammar analysis
4. **Advance to Lab 7**: Implement LR parsing techniques
5. **Master Lab 8**: Learn Yacc/Bison for automatic parser generation
6. **Complete Lab 9-10**: Understand intermediate code generation and optimization

---

## Additional Resources

### Recommended Reading
- **"Compilers: Principles, Techniques, and Tools"** by Aho, Lam, Sethi, Ullman (Dragon Book)
- **Flex & Bison** by John Levine
- **"Modern Compiler Implementation in C"** by Andrew Appel

### Online Resources
- [GNU Bison Manual](https://www.gnu.org/software/bison/manual/)
- [Flex Documentation](https://github.com/westes/flex)
- [Compiler Design Tutorial - GeeksforGeeks](https://www.geeksforgeeks.org/compiler-design-tutorials/)

---

## Contributing

This repository is maintained as an academic project. For improvements or corrections:
1. Fork the repository
2. Create a feature branch
3. Submit a pull request with detailed description

---

## License

This project is created for educational purposes as part of the Compiler Design course curriculum.

---

## Author

**Course:** Compiler Design (5th Semester)  
**Repository:** CD-Lab  
**Last Updated:** November 2025

---

## Notes

- All programs have been tested on Linux/WSL environment
- Input files are provided where applicable
- Makefiles are included for complex builds
- Output examples are provided in individual README files within labs
- Some labs contain multiple solution approaches for comparison

---

**Happy Learning! 🚀**

For questions or issues, please open an issue in the repository.
