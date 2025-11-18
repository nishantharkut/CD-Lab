#include <bits/stdc++.h>
using namespace std;
using Str = string;

vector<pair<Str, vector<Str>>> productions;
set<Str> nonterminals;
set<Str> terminals;
map<Str, set<Str>> FIRSTVT, LASTVT;
map<Str, map<Str, char>> table;

static inline vector<Str> split_ws(const string &s){
    vector<Str> out;
    string cur;
    istringstream iss(s);
    while(iss >> cur) out.push_back(cur);
    return out;
}

bool looksLikeNonTerminal(const Str &s){
    if(s.empty()) return false;
    for(char c : s) if(!(c>='A' && c<='Z')) return false;
    return true;
}

bool isTerminal(const Str &s){
    if(s == "$") return true;
    return terminals.count(s) > 0;
}
bool isNonTerminal(const Str &s){
    return nonterminals.count(s) > 0;
}

void readGrammarFromUser(){
    cout << "Enter productions (one per line). Tokens must be space-separated.\n";
    cout << "Examples:\n  E -> E + E\n  E -> ( E )\n  E -> i\nWhen done, type a single line: END\n\n";
    string line;
    while(true){
        cout << "> ";
        if(!getline(cin, line)) break;
        if(line.size()==0) continue;
        if(line == "END") break;
        vector<Str> parts = split_ws(line);
        if(parts.size() < 3){
            cerr << "Ignoring malformed line (need at least LHS -> RHS): " << line << "\n";
            continue;
        }
        int arrow = -1;
        for(size_t i=0;i<parts.size();i++){
            if(parts[i] == "->" || parts[i] == "=>") { arrow = (int)i; break; }
        }
        if(arrow == -1 || arrow == 0 || arrow == (int)parts.size()-1){
            cerr << "Ignoring malformed production (missing -> or bad placement): " << line << "\n";
            continue;
        }
        Str LHS = parts[0];
        nonterminals.insert(LHS);
        vector<Str> rhs;
        for(size_t i = arrow+1; i < parts.size(); ++i){
            rhs.push_back(parts[i]);
        }
        vector<vector<Str>> alternatives;
        vector<Str> cur;
        for(auto &tok : rhs){
            if(tok == "|"){
                if(!cur.empty()){
                    alternatives.push_back(cur);
                    cur.clear();
                }
            } else {
                cur.push_back(tok);
            }
        }
        if(!cur.empty()) alternatives.push_back(cur);
        for(auto &alt : alternatives){
            if(alt.empty()){
                cerr << "Warning: empty RHS (epsilon) ignored for LHS " << LHS << "\n";
                continue;
            }
            productions.push_back({LHS, alt});
        }
    }
    for(auto &prod : productions){
        for(auto &tok : prod.second){
            if(!looksLikeNonTerminal(tok)){
                terminals.insert(tok);
            } else {
                nonterminals.insert(tok);
            }
        }
    }
    terminals.insert("$");
}

void computeFIRSTVT(){
    FIRSTVT.clear();
    for(auto &A : nonterminals) FIRSTVT[A] = {};
    bool changed = true;
    while(changed){
        changed = false;
        for(auto &prod : productions){
            Str A = prod.first;
            auto &beta = prod.second;
            if(beta.empty()) continue;
            if(isTerminal(beta[0]) || !looksLikeNonTerminal(beta[0])){
                if(FIRSTVT[A].insert(beta[0]).second) changed = true;
            }
            if(isNonTerminal(beta[0]) && beta.size() >= 2 && (isTerminal(beta[1]) || !looksLikeNonTerminal(beta[1]))){
                if(FIRSTVT[A].insert(beta[1]).second) changed = true;
            }
            if(isNonTerminal(beta[0])){
                for(auto &t : FIRSTVT[beta[0]]){
                    if(FIRSTVT[A].insert(t).second) changed = true;
                }
            }
        }
    }
}

void computeLASTVT(){
    LASTVT.clear();
    for(auto &A : nonterminals) LASTVT[A] = {};
    bool changed = true;
    while(changed){
        changed = false;
        for(auto &prod : productions){
            Str A = prod.first;
            auto &beta = prod.second;
            if(beta.empty()) continue;
            if(isTerminal(beta.back()) || !looksLikeNonTerminal(beta.back())){
                if(LASTVT[A].insert(beta.back()).second) changed = true;
            }
            if(isNonTerminal(beta.back()) && beta.size() >= 2 && (isTerminal(beta[beta.size()-2]) || !looksLikeNonTerminal(beta[beta.size()-2]))){
                if(LASTVT[A].insert(beta[beta.size()-2]).second) changed = true;
            }
            if(isNonTerminal(beta.back())){
                for(auto &t : LASTVT[beta.back()]){
                    if(LASTVT[A].insert(t).second) changed = true;
                }
            }
        }
    }
}

void initTable(){
    table.clear();
    for(auto &a : terminals){
        for(auto &b : terminals){
            table[a][b] = ' ';
        }
    }
    for(auto &prod : productions){
        auto &alpha = prod.second;
        int n = (int)alpha.size();
        for(int i=0;i+1<n;i++){
            Str X = alpha[i], Y = alpha[i+1];
            bool Xterm = !(isNonTerminal(X));
            bool Yterm = !(isNonTerminal(Y));
            if(Xterm && Yterm){
                table[X][Y] = '=';
            }
            if(Xterm && isNonTerminal(Y)){
                for(auto &t : FIRSTVT[Y]){
                    if(table[X][t] == ' ') table[X][t] = '<';
                }
            }
            if(isNonTerminal(X) && Yterm){
                for(auto &t : LASTVT[X]){
                    if(table[t][Y] == ' ') table[t][Y] = '>';
                }
            }
        }
    }
    for(auto &t : terminals){
        if(t != "$"){
            table["$"][t] = '<';
            table[t]["$"] = '>';
        }
    }
    table["$"]["$"] = '=';
}

void printSetMap(const map<Str, set<Str>> &mp, const string &name){
    cout << name << ":\n";
    for(auto &p : mp){
        cout << "  " << p.first << " : { ";
        for(auto &s : p.second) cout << s << " ";
        cout << "}\n";
    }
}

void printTable(){
    vector<Str> cols(terminals.begin(), terminals.end());
    sort(cols.begin(), cols.end());
    cout << "\nOperator-Precedence Table:\n      ";
    for(auto &c : cols) cout << setw(6) << c;
    cout << "\n";
    for(auto &r : cols){
        cout << setw(6) << r;
        for(auto &c : cols){
            char v = table[r][c];
            if(v == ' ') cout << setw(6) << ".";
            else cout << setw(6) << v;
        }
        cout << "\n";
    }
}

int findRightmostTerminal(const vector<Str> &stack){
    for(int i = (int)stack.size()-1; i >= 0; --i){
        if(isTerminal(stack[i]) || stack[i] == "$") return i;
        if(!looksLikeNonTerminal(stack[i])) return i;
    }
    return -1;
}
int findPrevTerminal(const vector<Str> &stack, int startIdx){
    for(int i = startIdx; i >= 0; --i){
        if(isTerminal(stack[i]) || stack[i] == "$") return i;
        if(!looksLikeNonTerminal(stack[i])) return i;
    }
    return -1;
}

bool sequenceEqualsVec(const vector<Str> &a, const vector<Str> &b){
    if(a.size() != b.size()) return false;
    for(size_t i=0;i<a.size();++i) if(a[i] != b[i]) return false;
    return true;
}

bool parseInput(const vector<Str> &inputTokens){
    vector<Str> st;
    st.push_back("$");
    vector<Str> input = inputTokens;
    if(input.empty() || input.back() != "$") input.push_back("$");
    int ip = 0;

    cout << "\nParsing steps (stack \t\t input \t\t action):\n";
    while(true){
        string sstack;
        for(auto &x : st) sstack += x + " ";
        string sin;
        for(int k=ip;k<(int)input.size();k++) sin += input[k] + " ";
        cout << setw(30) << sstack << "\t" << setw(20) << sin << "\t";

        int idxR = findRightmostTerminal(st);
        if(idxR == -1){
            cout << "ERROR: no terminal on stack\n";
            return false;
        }
        Str a = st[idxR];
        Str b = input[ip];
        char rel = table[a][b];
        if(rel == '<' || rel == '='){
            cout << "shift " << b << "\n";
            st.push_back(b);
            ip++;
            if(b == "$" && idxR == 0 && st.size()==2){
                cout << setw(30) << (st[0] + " " + st[1]) << "\t" << setw(20) << "$" << "\t" << "accept\n";
                return true;
            }
        } else if(rel == '>'){
            int pos = idxR;
            int leftTermPos = -1;
            while(true){
                int prev = findPrevTerminal(st, pos-1);
                if(prev == -1){
                    leftTermPos = 0;
                    break;
                }
                char r = table[ st[prev] ][ st[pos] ];
                if(r == '<'){
                    leftTermPos = prev;
                    break;
                } else {
                    pos = prev;
                }
            }
            int l = leftTermPos + 1;
            int r = (int)st.size() - 1;
            vector<Str> handle(st.begin()+l, st.end());
            bool reduced = false;
            for(auto &prod : productions){
                if(sequenceEqualsVec(handle, prod.second)){
                    cout << "reduce by " << prod.first << " -> ";
                    for(auto &token : prod.second) cout << token << " ";
                    cout << "\n";
                    for(size_t k=0;k<handle.size();++k) st.pop_back();
                    st.push_back(prod.first);
                    reduced = true;
                    break;
                }
            }
            if(!reduced){
                cout << "ERROR: no production matches handle: ";
                for(auto &t : handle) cout << t << " ";
                cout << "\n";
                return false;
            }
        } else {
            cout << "ERROR: undefined precedence between '"<< a <<"' and '" << b << "'\n";
            return false;
        }

        if(st.size()==2 && st[0]=="$" && isNonTerminal(st[1]) && ip == (int)input.size()-1 && input[ip]=="$"){
            cout << setw(30) << (st[0] + " " + st[1]) << "\t" << setw(20) << "$" << "\t" << "accept\n";
            return true;
        }
    }
    return false;
}

int main(){
    readGrammarFromUser();
    if(productions.empty()){
        cerr << "No productions read. Exiting.\n";
        return 0;
    }
    cout << "\nProductions read:\n";
    for(auto &p : productions){
        cout << "  " << p.first << " -> ";
        for(auto &t : p.second) cout << t << " ";
        cout << "\n";
    }

    computeFIRSTVT();
    computeLASTVT();
    initTable();

    printSetMap(FIRSTVT, "FIRSTVT");
    printSetMap(LASTVT, "LASTVT");
    printTable();

    cout << "\nEnter input token stream (space-separated tokens). Use $ as end marker or program will append it. Example: i + i * i $\n> ";
    string inp;
    if(!getline(cin, inp)){
        cerr << "No input stream provided. Exiting.\n";
        return 0;
    }
    vector<Str> tokens = split_ws(inp);
    if(tokens.empty()){
        cerr << "Empty input stream. Exiting.\n";
        return 0;
    }
    bool ok = parseInput(tokens);
    cout << "\nParsing " << (ok? "succeeded" : "failed") << ".\n";
    return 0;
}
