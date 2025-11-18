#include <bits/stdc++.h>
using namespace std;

struct Production { string lhs; vector<string> rhs; int id; };

struct LR1Item {
    int prodId;
    int dot;
    string look;
    bool operator<(LR1Item const& o) const {
        if(prodId!=o.prodId) return prodId<o.prodId;
        if(dot!=o.dot) return dot<o.dot;
        return look<o.look;
    }
    bool operator==(LR1Item const& o) const {
        return prodId==o.prodId && dot==o.dot && look==o.look;
    }
};

static const string END_SYM = "$";

struct Action {
    enum Type {SHIFT, REDUCE, ACCEPT, ERROR} type = ERROR;
    int target = -1;
};

vector<Production> prods;
set<string> terminals, nonterminals;
map<string,set<string>> FIRST;

bool isTerminal(const string &sym){ if(sym==END_SYM) return true; return terminals.count(sym); }

void computeFIRST(){
    bool changed=true;
    while(changed){
        changed=false;
        for(auto &p: prods){
            auto &A = p.lhs;
            if(p.rhs.empty()) continue;
            bool allNullable = true;
            for(size_t i=0;i<p.rhs.size();++i){
                string X = p.rhs[i];
                if(isTerminal(X)){
                    if(!FIRST[A].count(X)) { FIRST[A].insert(X); changed=true; }
                    allNullable = false;
                    break;
                } else {
                    for(auto &t: FIRST[X]) if(!FIRST[A].count(t)){ FIRST[A].insert(t); changed=true; }
                    allNullable=false;
                    break;
                }
            }
            if(allNullable){ }
        }
    }
}

set<string> firstOf(const vector<string>& seq, size_t pos){
    set<string> res;
    if(pos>=seq.size()){ res.insert(END_SYM); return res; }
    string X = seq[pos];
    if(isTerminal(X)) res.insert(X);
    else for(auto &t: FIRST[X]) res.insert(t);
    return res;
}

struct State { set<LR1Item> items; };

vector<State> states;
map<set<LR1Item>, int> stateId;

set<LR1Item> closure(set<LR1Item> I){
    bool changed=true;
    while(changed){
        changed=false;
        vector<LR1Item> toAdd;
        for(auto &it: I){
            const Production &p = prods[it.prodId];
            if(it.dot < (int)p.rhs.size()){
                string B = p.rhs[it.dot];
                if(!isTerminal(B)){
                    vector<string> beta;
                    for(size_t k=it.dot+1;k<p.rhs.size();++k) beta.push_back(p.rhs[k]);
                    set<string> lookaheads;
                    if(beta.empty()) lookaheads.insert(it.look);
                    else {
                        auto f = firstOf(beta,0);
                        if(f.count(END_SYM)) { f.erase(END_SYM); f.insert(it.look); }
                        lookaheads = f;
                    }
                    for(auto &q: prods){
                        if(q.lhs==B){
                            for(auto &a: lookaheads){
                                LR1Item ni{q.id,0,a};
                                if(!I.count(ni)){ toAdd.push_back(ni); }
                            }
                        }
                    }
                }
            }
        }
        if(!toAdd.empty()){
            for(auto &x: toAdd) I.insert(x);
            changed=true;
        }
    }
    return I;
}

set<LR1Item> gotoOn(const set<LR1Item>& I, const string &X){
    set<LR1Item> J;
    for(auto &it: I){
        const Production &p=prods[it.prodId];
        if(it.dot < (int)p.rhs.size() && p.rhs[it.dot]==X){
            J.insert(LR1Item{it.prodId, it.dot+1, it.look});
        }
    }
    if(J.empty()) return J;
    return closure(J);
}

map<int,map<string,Action>> ACTION;
map<int,map<string,int>> GOTOtbl;

void addAction(int st, const string &a, Action act){ auto &slot = ACTION[st][a]; if(slot.type==Action::ERROR){ slot=act; return; } if(slot.type!=act.type || slot.target!=act.target){ cerr << "Conflict in state "<<st<<" on symbol '"<<a<<"'\n"; } }

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    prods.push_back({"S'", {"S"}, 0});
    prods.push_back({"S", {"L","=","R"}, 1});
    prods.push_back({"S", {"R"}, 2});
    prods.push_back({"L", {"*","R"}, 3});
    prods.push_back({"L", {"id"}, 4});
    prods.push_back({"R", {"L"}, 5});

    nonterminals = {"S'","S","L","R"};
    terminals = {"id","*","=",END_SYM};

    for(auto &t: terminals) FIRST[t].insert(t);
    computeFIRST();

    LR1Item start{0,0,END_SYM};
    set<LR1Item> I0 = closure({start});
    states.push_back({I0});
    stateId[I0]=0;

    queue<int> q; q.push(0);
    while(!q.empty()){
        int s=q.front(); q.pop();
        set<string> nextSyms;
        for(auto &it: states[s].items){
            const Production &p=prods[it.prodId];
            if(it.dot < (int)p.rhs.size()) nextSyms.insert(p.rhs[it.dot]);
        }
        for(auto &X: nextSyms){
            auto J = gotoOn(states[s].items,X);
            if(J.empty()) continue;
            if(!stateId.count(J)){
                int nid = states.size();
                states.push_back({J});
                stateId[J]=nid;
                q.push(nid);
            }
            int to = stateId[J];
            if(isTerminal(X)){
                Action a; a.type=Action::SHIFT; a.target=to; addAction(s,X,a);
            } else {
                GOTOtbl[s][X]=to;
            }
        }
    }

    for(size_t s=0;s<states.size();++s){
        for(auto &it: states[s].items){
            const Production &p=prods[it.prodId];
            if(it.dot == (int)p.rhs.size()){
                if(p.lhs=="S'" && it.look==END_SYM){
                    Action a; a.type=Action::ACCEPT; addAction(s,END_SYM,a);
                } else {
                    Action a; a.type=Action::REDUCE; a.target=it.prodId; addAction((int)s,it.look,a);
                }
            }
        }
    }

    cout << "Canonical LR(1) Item Sets ("<<states.size()<<" states)\n";
    for(size_t i=0;i<states.size();++i){
        cout << "State "<<i<<":\n";
        for(auto &it: states[i].items){
            const Production &p=prods[it.prodId];
            cout << "  ("<<it.prodId<<") "<<p.lhs<<" -> ";
            for(int k=0;k<(int)p.rhs.size();++k){
                if(k==it.dot) cout << ".";
                cout << p.rhs[k] << ' ';
            }
            if(it.dot == (int)p.rhs.size()) cout << ".";
            cout << ", "<<it.look<<"\n";
        }
        cout << "\n";
    }

    vector<string> termOrder = {"id","*","=",END_SYM};
    vector<string> nonTermOrder = {"S","L","R"};

    cout << "ACTION/GOTO Table:\n";
    cout << left << setw(7) << "State";
    for(auto &t: termOrder) cout << setw(10) << t;
    for(auto &nt: nonTermOrder) cout << setw(7) << nt;
    cout << "\n";
    for(size_t s=0;s<states.size();++s){
        cout << setw(7) << s;
        for(auto &t: termOrder){
            string cell="";
            auto itA = ACTION[s].find(t);
            if(itA!=ACTION[s].end()){
                if(itA->second.type==Action::SHIFT) cell = "s"+to_string(itA->second.target);
                else if(itA->second.type==Action::REDUCE) cell = "r"+to_string(itA->second.target);
                else if(itA->second.type==Action::ACCEPT) cell = "acc";
            }
            cout << setw(10) << cell;
        }
        for(auto &nt: nonTermOrder){
            string cell="";
            auto itG = GOTOtbl[s].find(nt);
            if(itG!=GOTOtbl[s].end()) cell = to_string(itG->second);
            cout << setw(7) << cell;
        }
        cout << "\n";
    }

    cerr << "Enter token sequence (e.g., id = id): ";
    string line; getline(cin,line);
    if(line.empty()) line = "id = id";
    vector<string> input; {
        stringstream ss(line); string tok; while(ss>>tok) input.push_back(tok);
    }
    if(input.empty()) { cerr << "No tokens given.\n"; return 0; }
    if(input.back()!=END_SYM) input.push_back(END_SYM);

    vector<int> stStack={0};
    vector<string> symStack;
    size_t ip=0;

    cout << "\nParse trace for: "; for(auto &t: input) cout << t << ' '; cout << "\n";
    cout << left << setw(30) << "StateStack" << setw(25) << "Symbols" << setw(15) << "Input" << "Action" << "\n";

    while(true){
        int s = stStack.back();
        string a = input[ip];
        Action act = ACTION[s][a];
        auto formatStack=[&](const vector<int>&v){ string r; for(size_t i=0;i<v.size();++i){ r+=to_string(v[i]); if(i+1<v.size()) r+=' '; } return r; };
        auto formatSyms=[&](const vector<string>&v){ string r; for(size_t i=0;i<v.size();++i){ r+=v[i]; if(i+1<v.size()) r+=' '; } return r; };
        string remaining; for(size_t k=ip;k<input.size();++k){ remaining+=input[k]; if(k+1<input.size()) remaining+=' '; }
        string actStr;
        if(act.type==Action::SHIFT){ actStr = "shift "+to_string(act.target); }
        else if(act.type==Action::REDUCE){
            const Production &rp = prods[act.target];
            actStr = "reduce ("+to_string(rp.id)+") "+rp.lhs+"->"; for(auto &x: rp.rhs) actStr+=x; if(rp.rhs.empty()) actStr+="ε"; }
        else if(act.type==Action::ACCEPT){ actStr = "accept"; }
        else actStr = "error";
        cout << setw(30) << formatStack(stStack) << setw(25) << formatSyms(symStack) << setw(15) << remaining << actStr << "\n";

        if(act.type==Action::SHIFT){
            symStack.push_back(a);
            stStack.push_back(act.target);
            ++ip;
        } else if(act.type==Action::REDUCE){
            const Production &rp = prods[act.target];
            for(size_t k=0;k<rp.rhs.size();++k){ stStack.pop_back(); symStack.pop_back(); }
            int tstate = stStack.back();
            int to = GOTOtbl[tstate][rp.lhs];
            symStack.push_back(rp.lhs);
            stStack.push_back(to);
        } else if(act.type==Action::ACCEPT){
            cout << "\nInput accepted.\n";
            break;
        } else {
            cout << "\nParse error at token '"<<a<<"'\n"; break; }
    }
}
