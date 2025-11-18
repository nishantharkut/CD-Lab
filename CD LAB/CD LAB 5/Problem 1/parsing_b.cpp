#include <bits/stdc++.h>
using namespace std;

string s;
int n;
long long calls = 0, backtracks = 0;

bool match(char ch, int &pos) {
    if (pos < n && s[pos] == ch) { pos++; return true; }
    return false;
}

bool parseA(int &pos) {
    calls++;
    int save = pos;
    if (match('a', pos)) {
        return true;
    }
    pos = save; backtracks++;
    if (match('a', pos) && match('b', pos)) {
        return true;
    }
    pos = save;
    return false;
}

bool parseS(int &pos) {
    calls++;
    int save = pos;
    if (!match('c', pos)) { pos = save; return false; }
    if (!parseA(pos)) { pos = save; return false; }
    if (!match('d', pos)) { pos = save; return false; }
    return true;
}

int main() {
    while (getline(cin, s)) {
        if (s.size() && s.back() == '\r') s.pop_back();
        n = s.size();
        calls = backtracks = 0;
        int pos = 0;
        bool ok = parseS(pos) && pos == n;
        cout << "Input: \"" << s << "\" => " << (ok ? "ACCEPT" : "REJECT") << "\n";
        cout << "calls: " << calls << "  backtracks: " << backtracks << "\n\n";
    }
    return 0;
}
