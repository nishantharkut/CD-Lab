

#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> grammar;
set<char> nonTerminals, terminals;


bool isNonTerminal(char c) {
    return c >= 'A' && c <= 'Z';
}

map<char, set<char>> computeLast(){
    map<char, set<char>> LAST;
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto &p : grammar) {
            char A = p.first;
            for (const string &prod : p.second) {
                if (prod == "#" || prod.empty()) {
                    if (LAST[A].insert('#').second) changed = true;
                    continue;
                }
                for (int i = prod.size() - 1; i >= 0; --i) {
                    char sym = prod[i];
                    if (!isNonTerminal(sym)) {
                        if (LAST[A].insert(sym).second) changed = true;
                        break;
                    } else {
                        for (char x : LAST[sym]) {
                            if (x != '#') {
                                if (LAST[A].insert(x).second) changed = true;
                            }
                        }
                        if (LAST[sym].count('#')) {
                            if (i == 0) {
                                if (LAST[A].insert('#').second) changed = true;
                            }
                            continue;
                        } else {
                            break;
                        }
                    }
                }
            }
        }
    }
    return LAST;
}

static set<char> lastOfString(const string &alpha, const map<char, set<char>> &LAST) {
    set<char> out;
    if (alpha.empty() || alpha == "#") {
        out.insert('#');
        return out;
    }
    bool nullableSuffix = true;
    for (int i = static_cast<int>(alpha.size()) - 1; i >= 0; --i) {
        char X = alpha[i];
        if (!isNonTerminal(X)) {
            out.insert(X);
            nullableSuffix = false;
            break;
        } else {
            auto it = LAST.find(X);
            if (it != LAST.end()) {
                for (char t : it->second) if (t != '#') out.insert(t);
                if (it->second.count('#')) {
                    nullableSuffix = true;
                    continue;
                } else {
                    nullableSuffix = false;
                    break;
                }
            } else {
                nullableSuffix = false;
                break;
            }
        }
    }
    if (nullableSuffix) out.insert('#');
    return out;
}

map<char, set<char>> computePRECEDE(const map<char, set<char>>& LAST, char startSym) {
    map<char, set<char>> PRECEDE;
    for (char A : nonTerminals) PRECEDE[A];
    if (startSym) PRECEDE[startSym].insert('$');
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto &p : grammar) {
            char X = p.first;
            for (const string &rhs : p.second) {
                for (size_t i = 0; i < rhs.size(); ++i) {
                    char A = rhs[i];
                    if (!isNonTerminal(A)) continue;
                    string prefix = rhs.substr(0, i);
                    set<char> lastPref = lastOfString(prefix, LAST);
                    for (char t : lastPref) if (t != '#') {
                        if (PRECEDE[A].insert(t).second) changed = true;
                    }
                    if (lastPref.count('#')) {
                        for (char t : PRECEDE[X]) {
                            if (PRECEDE[A].insert(t).second) changed = true;
                        }
                    }
                }
            }
        }
    }
    return PRECEDE;
}

static bool chooseProduction(char A, char a,
                             const map<char, set<char>> &LAST,
                             const map<char, set<char>> &PRECEDE,
                             string &chosen) {
    vector<string> candidates;
    auto git = grammar.find(A);
    if (git == grammar.end()) return false;
    for (const string &rhs : git->second) {
        set<char> lastAlpha = lastOfString(rhs, LAST);
        bool select = false;
        if (lastAlpha.count(a)) select = true;
        else if (lastAlpha.count('#') && PRECEDE.at(A).count(a)) select = true;
        if (select) candidates.push_back(rhs);
    }
    if (candidates.empty()) return false;
    chosen = candidates.front();
    return true;
}

bool RR1Parser(const string &input, char start, const map<char, set<char>> &LAST, const map<char, set<char>> &PRECEDE){
    string st;
    st.push_back(start);
    int ip = static_cast<int>(input.size()) - 1;

    while (!st.empty()) {
        char X = st.back();
        char a = (ip >= 0) ? input[ip] : '$';

        if (!isNonTerminal(X)) {
            if (X == a) { st.pop_back(); ip--; }
            else return false;
        } else {
            string prod;
            if (!chooseProduction(X, a, LAST, PRECEDE, prod)) return false;
            st.pop_back();
            if (!(prod == "#" || prod.empty())) {
                for (size_t i = 0; i < prod.size(); ++i) st.push_back(prod[i]);
            }
        }
    }
    return ip == 0;
}


int main(){
    int n;
    cout << "enter the number of productions\n";
    cin >> n;
    cin.ignore();
    
    char start = 0;
    for (int i = 0; i<n; i++){
        string line;
        cout << "enter the production " << i+1 << " (eg. S-> AbXy|#) : ";
        getline(cin, line);
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        char lhs = line[0];
        nonTerminals.insert(lhs);
        if (i == 0) start = lhs;
        size_t pos = line.find("->");
        if (pos == string::npos){
            cout << "invalid production format.\n";
            return 1;
        }
        string rhs = line.substr(pos+2);
        stringstream ss(rhs);
        string prod;
       while (getline(ss, prod, '|')){
            grammar[lhs].push_back(prod);
            for (char c : prod) {
                if (!isNonTerminal(c) && c != '#') terminals.insert(c);
            }
        }
    }

    auto LAST = computeLast();
    cout << "\nLAST sets:\n";
    for (char nt : nonTerminals) {
        cout << "LAST(" << nt << ") = { ";
        for (char t : LAST[nt]) {
            if (t == '#') cout << "ε ";
            else cout << t << " ";
        }
        cout << "}\n";
    }

    auto PRECEDE = computePRECEDE(LAST, start);
    cout << "\nPRECEDE sets:\n";
    for (char nt : nonTerminals) {
        cout << "PRECEDE(" << nt << ") = { ";
        for (char t : PRECEDE[nt]) {
            if (t == '$') cout << "$ ";
            else cout << t << " ";
        }
        cout << "}\n";
    }


    string input;
    cout << "enter the input string for the given grammar : ";
    cin >> input;
    input = "$" + input;

    if (RR1Parser(input, start, LAST, PRECEDE)){
        cout << "string accepted\n";
    }
    else {
        cout << "string not accepted\n";
    }
    return 0;
}