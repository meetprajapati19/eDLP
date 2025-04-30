#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <stack>
#include <string>

using namespace std;

map<string, vector<vector<string>>> grammar;
map<string, set<string>> first, follow;
map<string, map<string, vector<string>>> parsingTable;
set<string> terminals, nonTerminals;

void initGrammar() {
    grammar["S"] = {{"A", "B", "C"}, {"D"}};
    grammar["A"] = {{"a"}, {"epsilon"}};
    grammar["B"] = {{"b"}, {"epsilon"}};
    grammar["C"] = {{"(", "S", ")"}, {"c"}};
    grammar["D"] = {{"A", "C"}};

    nonTerminals = {"S", "A", "B", "C", "D"};
    terminals = {"a", "b", "(", ")", "c", "$"};
}

bool isTerminal(const string& symbol) {
    return terminals.find(symbol) != terminals.end();
}

set<string> computeFirst(const string& symbol) {
    if (first.find(symbol) != first.end()) return first[symbol];
    set<string> result;

    if (isTerminal(symbol) || symbol == "epsilon") {
        result.insert(symbol);
        return result;
    }

    for (auto production : grammar[symbol]) {
        for (size_t i = 0; i < production.size(); ++i) {
            set<string> temp = computeFirst(production[i]);
            result.insert(temp.begin(), temp.end());
            if (temp.find("epsilon") == temp.end()) break;
            if (i == production.size() - 1) result.insert("epsilon");
        }
    }
    return first[symbol] = result;
}

set<string> computeFollow(const string& symbol) {
    if (!follow[symbol].empty()) return follow[symbol];
    set<string> result;
    if (symbol == "S") result.insert("$");

    for (auto& rule : grammar) {
        for (auto& production : rule.second) {
            for (size_t i = 0; i < production.size(); ++i) {
                if (production[i] == symbol) {
                    size_t j = i + 1;
                    while (j < production.size()) {
                        set<string> firstNext = computeFirst(production[j]);
                        result.insert(firstNext.begin(), firstNext.end());
                        if (firstNext.find("epsilon") != firstNext.end()) {
                            ++j;
                        } else break;
                    }
                    if (j == production.size() && rule.first != symbol) {
                        set<string> followHead = computeFollow(rule.first);
                        result.insert(followHead.begin(), followHead.end());
                    }
                }
            }
        }
    }

    result.erase("epsilon");
    return follow[symbol] = result;
}

bool constructParsingTable() {
    bool isLL1 = true;
    for (auto& rule : grammar) {
        string nt = rule.first;
        for (auto& production : rule.second) {
            set<string> prodFirst;
            bool hasEpsilon = false;
            for (string sym : production) {
                set<string> symFirst = computeFirst(sym);
                prodFirst.insert(symFirst.begin(), symFirst.end());
                if (symFirst.find("epsilon") == symFirst.end()) break;
                hasEpsilon = true;
            }
            if (prodFirst.find("epsilon") != prodFirst.end()) {
                prodFirst.erase("epsilon");
                set<string> followNT = computeFollow(nt);
                prodFirst.insert(followNT.begin(), followNT.end());
            }
            for (string terminal : prodFirst) {
                if (parsingTable[nt][terminal].empty()) {
                    parsingTable[nt][terminal] = production;
                } else {
                    isLL1 = false; // Conflict detected
                }
            }
        }
    }
    return isLL1;
}

bool validateString(const string& inputStr) {
    stack<string> stk;
    stk.push("$");
    stk.push("S");

    string input = inputStr + "$";
    size_t ip = 0;

    while (!stk.empty()) {
        string top = stk.top(); stk.pop();
        string current(1, input[ip]);

        if (top == "epsilon") continue;
        else if (isTerminal(top) || top == "$") {
            if (top == current) {
                ++ip;
            } else {
                return false;
            }
        } else {
            if (parsingTable[top].find(current) != parsingTable[top].end()) {
                vector<string> prod = parsingTable[top][current];
                for (int i = prod.size() - 1; i >= 0; --i) stk.push(prod[i]);
            } else {
                return false;
            }
        }
    }
    return input[ip] == '$';
}

int main() {
    initGrammar();

    // Compute First and Follow sets
    for (auto& nt : nonTerminals) computeFirst(nt);
    for (auto& nt : nonTerminals) computeFollow(nt);

    // Display First sets
    cout << "\nFirst Sets:" << endl;
    for (auto& f : first) {
        cout << "First(" << f.first << ") = { ";
        for (auto& s : f.second) cout << s << " ";
        cout << "}" << endl;
    }

    // Display Follow sets
    cout << "\nFollow Sets:" << endl;
    for (auto& f : follow) {
        cout << "Follow(" << f.first << ") = { ";
        for (auto& s : f.second) cout << s << " ";
        cout << "}" << endl;
    }

    // Construct Parsing Table
    bool isLL1 = constructParsingTable();

    cout << "\nParsing Table:" << endl;
    for (auto& row : parsingTable) {
        for (auto& col : row.second) {
            cout << "M[" << row.first << ", " << col.first << "] = ";
            for (auto& s : col.second) cout << s << " ";
            cout << endl;
        }
    }

    cout << "\nGrammar is " << (isLL1 ? "LL(1)" : "NOT LL(1)") << endl;

    // Test strings
    vector<string> testStrings = {"abc", "ac", "(abc)", "c", "(ac)", "a", "()", "(ab)", "abcabc", "b"};
    for (auto& str : testStrings) {
        cout << "Input: " << str << " -> ";
        cout << (validateString(str) ? "Valid string" : "Invalid string") << endl;
    }

    return 0;
}