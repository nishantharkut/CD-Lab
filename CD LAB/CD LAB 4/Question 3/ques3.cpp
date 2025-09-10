#include <bits/stdc++.h>
using namespace std;

string trim(const string &s){
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a==string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b-a+1);
}

vector<string> splitAlternatives(string rhs){
    string t;
    for(char c: rhs) if(c!=' ' && c!='\t') t.push_back(c);
    vector<string> out;
    string cur;
    for(char c: t){
        if(c=='|'){ out.push_back(cur); cur.clear(); }
        else cur.push_back(c);
    }
    if(!cur.empty()) out.push_back(cur);
    return out;
}

int main(){
    // ios::sync_with_stdio(false);
    // cin.tie(nullptr);
    int n;
    cout<<"Enter number of productions: ";
    if(!(cin>>n)) return 0;
    string line;
    getline(cin, line);
    cout<<"Use # for epsilon.\n";

    map<char, vector<string>> grammar;
    set<char> nonTerminals, terminals;
    char start = 0;

    for(int i=0;i<n;i++){
        getline(cin, line);
        line = trim(line);
        if(line.empty()){ i--; continue; }
        size_t arrow = line.find("->");
        if(arrow==string::npos){ cerr<<"Bad production: "<<line<<"\n"; return 1; }
        string L = trim(line.substr(0, arrow));
        string R = line.substr(arrow+2);
        if(L.empty()){ cerr<<"Bad LHS: "<<line<<"\n"; return 1; }
        char lhs = L[0];
        if(i==0) start = lhs;
        nonTerminals.insert(lhs);
        auto alts = splitAlternatives(R);
        for(auto &alt: alts){
            grammar[lhs].push_back(alt);
            for(char c: alt){
                if(isupper((unsigned char)c)) nonTerminals.insert(c);
                else if(c!='#') terminals.insert(c);
            }
        }
    }

    map<char, set<char>> FIRST;
    bool changed = true;
    while(changed){
        changed = false;
        for(char A: nonTerminals){
            for(auto &prod: grammar[A]){
                if(prod == "#"){
                    if(FIRST[A].insert('#').second) changed = true;
                    continue;
                }
                bool allEps = true;
                for(char sym: prod){
                    if(!isupper((unsigned char)sym)){
                        if(FIRST[A].insert(sym).second) changed = true;
                        allEps = false;
                        break;
                    } else {
                        for(char t: FIRST[sym]){
                            if(t != '#' && FIRST[A].insert(t).second) changed = true;
                        }
                        if(FIRST[sym].count('#')) {
                        } else {
                            allEps = false;
                            break;
                        }
                    }
                }
                if(allEps){
                    if(FIRST[A].insert('#').second) changed = true;
                }
            }
        }
    }

    map<char, set<char>> FOLLOW;
    if(start) FOLLOW[start].insert('$');
    changed = true;
    while(changed){
        changed = false;
        for(char A: nonTerminals){
            for(auto &prod: grammar[A]){
                int L = (int)prod.size();
                for(int i=0;i<L;i++){
                    char B = prod[i];
                    if(!isupper((unsigned char)B)) continue;
                    bool betaAllEps = true;
                    if(i+1 < L){
                        for(int j=i+1;j<L;j++){
                            char sym = prod[j];
                            if(!isupper((unsigned char)sym)){
                                if(FOLLOW[B].insert(sym).second) changed = true;
                                betaAllEps = false;
                                break;
                            } else {
                                for(char t: FIRST[sym]){
                                    if(t != '#' && FOLLOW[B].insert(t).second) changed = true;
                                }
                                if(FIRST[sym].count('#')) {
                                
                                } else {
                                    betaAllEps = false;
                                    break;
                                }
                            }
                        }
                    } else betaAllEps = true;

                    if(betaAllEps){
                        for(char t: FOLLOW[A]){
                            if(FOLLOW[B].insert(t).second) changed = true;
                        }
                    }
                }
            }
        }
    }

    vector<char> nts(nonTerminals.begin(), nonTerminals.end());
    sort(nts.begin(), nts.end());
    cout<<"\nFIRST sets:\n";
    for(char A: nts){
        cout<<"FIRST("<<A<<") = { ";
        vector<string> elems;
        for(char x: FIRST[A]){
            if(x == '#') elems.push_back("ε");
            else { string s; s.push_back(x); elems.push_back(s); }
        }
        sort(elems.begin(), elems.end());
        for(size_t i=0;i<elems.size();++i){
            if(i) cout<<", ";
            cout<<elems[i];
        }
        cout<<" }\n";
    }

    cout<<"\nFOLLOW sets:\n";
    for(char A: nts){
        cout<<"FOLLOW("<<A<<") = { ";
        vector<string> elems;
        for(char x: FOLLOW[A]){
            if(x == '#') elems.push_back("ε");
            else { string s; s.push_back(x); elems.push_back(s); }
        }
        sort(elems.begin(), elems.end());
        for(size_t i=0;i<elems.size();++i){
            if(i) cout<<", ";
            cout<<elems[i];
        }
        cout<<" }\n";
    }

    return 0;
}
