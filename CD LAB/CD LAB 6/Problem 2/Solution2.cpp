#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class SLRParser {
public:
    struct Production {
        char left;
        string right;
    };

private:
    map<char, vector<string>> productions;
    vector<char> terminals, nonTerminals;
    char startSymbol{};
    char augmentedStart{};

    map<char, set<char>> first, follow;

    vector<vector<pair<char, string>>> itemsets;

    map<pair<int, char>, int> gotoTable;
    map<pair<int, char>, vector<string>> actionTable;

    void computeFirst(char symbol, set<char>& out, set<char>& processing);
    void computeFollow(char symbol, set<char>& out, set<char>& processing);
    vector<pair<char, string>> closure(const vector<pair<char, string>>& kernel);
    void buildItemSets();
    void buildParsingTable();
    static bool isNonTerminal(char c) { return isupper(static_cast<unsigned char>(c)) != 0; }
    static bool isTerminalChar(char c) { return !isNonTerminal(c) && !isspace(static_cast<unsigned char>(c)); }
    static bool itemsetEqual(const vector<pair<char, string>>& a,
                             const vector<pair<char, string>>& b) {
        if (a.size() != b.size()) return false;
        set<pair<char, string>> sa(a.begin(), a.end());
        set<pair<char, string>> sb(b.begin(), b.end());
        return sa == sb;
    }

public:
    SLRParser(const vector<Production>& prods, char start);
    void buildParser();
    void displayResults() const;
};

SLRParser::SLRParser(const vector<Production>& prods, char start) : startSymbol(start) {
    augmentedStart = '#';
    for (const auto& p : prods) {
        productions[p.left].push_back(p.right);
        if (find(nonTerminals.begin(), nonTerminals.end(), p.left) == nonTerminals.end())
            nonTerminals.push_back(p.left);
        for (char c : p.right) {
            if (isNonTerminal(c)) {
                if (find(nonTerminals.begin(), nonTerminals.end(), c) == nonTerminals.end())
                    nonTerminals.push_back(c);
            } else if (isTerminalChar(c)) {
                if (find(terminals.begin(), terminals.end(), c) == terminals.end())
                    terminals.push_back(c);
            }
        }
    }
    productions[augmentedStart].push_back(string(1, startSymbol));
    if (find(nonTerminals.begin(), nonTerminals.end(), augmentedStart) == nonTerminals.end())
        nonTerminals.insert(nonTerminals.begin(), augmentedStart);
    if (find(terminals.begin(), terminals.end(), '$') == terminals.end())
        terminals.push_back('$');
}

void SLRParser::computeFirst(char symbol, set<char>& out, set<char>& processing) {
    if (find(terminals.begin(), terminals.end(), symbol) != terminals.end()) { out.insert(symbol); return; }
    auto itMemo = first.find(symbol);
    if (itMemo != first.end()) { out.insert(itMemo->second.begin(), itMemo->second.end()); return; }
    if (processing.count(symbol)) return;
    processing.insert(symbol);
    for (const auto& rhs : productions[symbol]) {
        if (rhs.empty()) {
            continue;
        }
        for (size_t i = 0; i < rhs.size(); ++i) {
            char X = rhs[i];
            if (X == symbol) {
                break;
            }
            set<char> temp;
            computeFirst(X, temp, processing);
            for (char t : temp) out.insert(t);
            break;
        }
    }
    processing.erase(symbol);
    first[symbol] = out;
}

void SLRParser::computeFollow(char symbol, set<char>& out, set<char>& processing) {
    if (processing.count(symbol)) return;
    processing.insert(symbol);

    if (symbol == startSymbol) out.insert('$');

    for (const auto& [A, rights] : productions) {
        for (const auto& rhs : rights) {
            for (size_t i = 0; i < rhs.size(); ++i) {
                if (rhs[i] != symbol) continue;
                if (i + 1 < rhs.size()) {
                    char B = rhs[i + 1];
                    set<char> fb; set<char> procEmpty;
                    computeFirst(B, fb, procEmpty);
                    for (char t : fb) out.insert(t);
                } else {
                    if (A != symbol) {
                        set<char> fA;
                        computeFollow(A, fA, processing);
                        out.insert(fA.begin(), fA.end());
                    }
                }
            }
        }
    }
    follow[symbol] = out;
}

vector<pair<char, string>> SLRParser::closure(const vector<pair<char, string>>& kernel) {
    vector<pair<char, string>> res = kernel;
    bool changed;
    do {
        changed = false;
        vector<pair<char, string>> added;
        for (const auto& item : res) {
            const auto& left = item.first; (void)left;
            const auto& dotted = item.second;
            size_t dot = dotted.find('.');
            if (dot == string::npos || dot + 1 >= dotted.size()) continue;
            char X = dotted[dot + 1];
            if (isNonTerminal(X)) {
                for (const auto& prod : productions[X]) {
                    pair<char, string> newItem{X, string(".") + prod};
                    if (find(res.begin(), res.end(), newItem) == res.end() &&
                        find(added.begin(), added.end(), newItem) == added.end()) {
                        added.push_back(newItem);
                        changed = true;
                    }
                }
            }
        }
        res.insert(res.end(), added.begin(), added.end());
    } while (changed);
    return res;
}

void SLRParser::buildItemSets() {
    vector<pair<char, string>> startKernel{{augmentedStart, string(".") + productions[augmentedStart][0]}};
    itemsets.push_back(closure(startKernel));

    for (size_t i = 0; i < itemsets.size(); ++i) {
        map<char, vector<pair<char, string>>> groups;
        for (const auto& [A, dotted] : itemsets[i]) {
            size_t dot = dotted.find('.');
            if (dot == string::npos || dot + 1 >= dotted.size()) continue;
            char X = dotted[dot + 1];
            string moved = dotted;
            swap(moved[dot], moved[dot + 1]);
            groups[X].push_back({A, moved});
        }
        for (const auto& [X, kernel] : groups) {
            auto J = closure(kernel);
            int foundIdx = -1;
            for (size_t k = 0; k < itemsets.size(); ++k) {
                if (itemsetEqual(itemsets[k], J)) { foundIdx = static_cast<int>(k); break; }
            }
            if (foundIdx == -1) {
                foundIdx = static_cast<int>(itemsets.size());
                itemsets.push_back(J);
            }
            gotoTable[{static_cast<int>(i), X}] = foundIdx;
        }
    }
}

void SLRParser::buildParsingTable() {
    for (size_t i = 0; i < itemsets.size(); ++i) {
        for (const auto& [A, dotted] : itemsets[i]) {
            size_t dot = dotted.find('.');
            if (dot + 1 == dotted.size()) {
                if (A == augmentedStart && dotted == productions[augmentedStart][0] + string(".")) {
                    actionTable[{static_cast<int>(i), '$'}].push_back("accept");
                } else {
                    string rhs = dotted.substr(0, dotted.size() - 1);
                    for (char a : follow[A]) {
                        ostringstream ss; ss << "reduce " << A << "->" << rhs;
                        actionTable[{static_cast<int>(i), a}].push_back(ss.str());
                    }
                }
            } else {
                char X = dotted[dot + 1];
                if (find(terminals.begin(), terminals.end(), X) != terminals.end()) {
                    auto itGo = gotoTable.find({static_cast<int>(i), X});
                    if (itGo != gotoTable.end()) {
                        int to = itGo->second;
                        ostringstream ss; ss << "shift " << to;
                        actionTable[{static_cast<int>(i), X}].push_back(ss.str());
                    }
                }
            }
        }
        for (char B : nonTerminals) {
            auto it = gotoTable.find({static_cast<int>(i), B});
            if (it != gotoTable.end()) {
                ostringstream ss; ss << it->second;
                actionTable[{static_cast<int>(i), B}].push_back(ss.str());
            }
        }
    }
}

void SLRParser::buildParser() {
    for (char A : nonTerminals) {
        set<char> s, proc; computeFirst(A, s, proc);
    }
    for (char A : nonTerminals) {
        set<char> s, proc; computeFollow(A, s, proc);
    }
    buildItemSets();
    buildParsingTable();
}

void SLRParser::displayResults() const {
    cout << "FIRST sets:\n";
    for (const auto& [A, S] : first) {
        cout << A << ": {";
        for (auto it = S.begin(); it != S.end(); ++it) { if (it != S.begin()) cout << ", "; cout << *it; }
        cout << "}\n";
    }
    cout << "\nFOLLOW sets:\n";
    for (const auto& [A, S] : follow) {
        cout << A << ": {";
        for (auto it = S.begin(); it != S.end(); ++it) { if (it != S.begin()) cout << ", "; cout << *it; }
        cout << "}\n";
    }

    cout << "\nItem Sets:\n";
    for (size_t i = 0; i < itemsets.size(); ++i) {
        cout << "I" << i << ":\n";
        for (const auto& [A, dotted] : itemsets[i]) cout << "  " << A << " -> " << dotted << "\n";
        cout << "\n";
    }

    cout << "Parsing Table (terminals then nonterminals):\n ";
    for (char a : terminals) {
        cout << a << "\t";
    }
    for (char A : nonTerminals) {
        cout << A << "\t";
    }
    cout << "\n";
    for (size_t i = 0; i < itemsets.size(); ++i) {
        cout << i << " ";
        for (char s : terminals) {
            auto it = actionTable.find({static_cast<int>(i), s});
            if (it != actionTable.end()) { for (const auto& act : it->second) cout << act << ' '; }
            cout << "\t";
        }
        for (char A : nonTerminals) {
            auto it = actionTable.find({static_cast<int>(i), A});
            if (it != actionTable.end()) { for (const auto& act : it->second) cout << act << ' '; }
            cout << "\t";
        }
        cout << "\n";
    }
}

int main() {
    vector<SLRParser::Production> prods = {
        {'S', "E"},
        {'E', "E+T"},
        {'E', "T"},
        {'T', "T*F"},
        {'T', "F"},
        {'F', "(E)"},
        {'F', "id"}
    };
    SLRParser parser(prods, 'S');
    parser.buildParser();
    parser.displayResults();
    return 0;
}
