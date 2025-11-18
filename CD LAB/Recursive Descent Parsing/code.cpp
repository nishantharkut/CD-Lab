// Very small Recursive Descent Parser (exam-friendly)
// Grammar:
//   E  -> T E'
//   E' -> + T E' | epsilon
//   T  -> F T'
//   T' -> * F T' | epsilon
//   F  -> ( E ) | id
//
// id here is a single letter or digit (e.g. a, b, x, 1, 2) for simplicity.
//
// How to use: copy this to the exam sheet, compile/run, give an input like:
//   a + b * (c + d)
// It prints "OK" if the whole input is parsed, else "Error".

#include <bits/stdc++.h>
using namespace std;

string s;   // input string (spaces allowed)
int i_pos;   // current position in s

// return current character (skipping spaces). If end, return '\0'.
char peek() {
    while (i_pos < (int)s.size() && isspace((unsigned char)s[i_pos])) ++i_pos;
    if (i_pos >= (int)s.size()) return '\0';
    return s[i_pos];
}

// if current (skipping spaces) equals c, consume it and return true; else false.
bool consume_if(char c) {
    if (peek() == c) { ++i_pos; return true; }
    return false;
}

// Forward declarations for grammar functions (they are tiny)
bool parseE();
bool parseEP();
bool parseT();
bool parseTP();
bool parseF();

// E -> T E'
bool parseE() {
    if (!parseT()) return false;
    if (!parseEP()) return false;
    return true;
}

// E' -> + T E' | epsilon
bool parseEP() {
    if (consume_if('+')) {         // if there's a '+', take alternative "+ T E'"
        if (!parseT()) return false;
        if (!parseEP()) return false;
        return true;
    }
    // else epsilon: okay without consuming input
    return true;
}

// T -> F T'
bool parseT() {
    if (!parseF()) return false;
    if (!parseTP()) return false;
    return true;
}

// T' -> * F T' | epsilon
bool parseTP() {
    if (consume_if('*')) {
        if (!parseF()) return false;
        if (!parseTP()) return false;
        return true;
    }
    // epsilon
    return true;
}

// F -> ( E ) | id
bool parseF() {
    if (consume_if('(')) {
        if (!parseE()) return false;
        if (!consume_if(')')) return false;
        return true;
    }
    // id = single letter or digit
    char c = peek();
    if ( (c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') ) {
        ++i_pos; // consume id
        return true;
    }
    return false; // no alternative matched
}

int main() {
    // Read a whole line (expression) including spaces
    getline(cin, s);
    i_pos = 0;

    bool ok = parseE();

    // after parsing, skip remaining spaces and check if we've consumed whole input
    while (i_pos < (int)s.size() && isspace((unsigned char)s[i_pos])) ++i_pos;
    if (ok && i_pos == (int)s.size()) cout << "OK\n";
    else cout << "Error\n";
    return 0;
}
