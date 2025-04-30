#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

map<string, vector<vector<string>>> grammar;
map<string, set<string>> firstSet;
map<string, set<string>> followSet;
set<string> nonTerminals, terminals;
string startSymbol = "S";

// Utility to check if a symbol is a terminal
bool isTerminal(const string &symbol) {
    return !(symbol[0] >= 'A' && symbol[0] <= 'Z');
}

// Compute FIRST set
set<string> computeFirst(const string &symbol) {
    if (firstSet.count(symbol)) return firstSet[symbol];

    set<string> result;

    // Terminal or ε
    if (isTerminal(symbol)) {
        result.insert(symbol);
        return result;
    }

    for (auto production : grammar[symbol]) {
        for (int i = 0; i < production.size(); ++i) {
            string sym = production[i];
            set<string> temp = computeFirst(sym);
            result.insert(temp.begin(), temp.end());
            if (temp.find("ε") == temp.end()) break;
            if (i == production.size() - 1)
                result.insert("ε");
        }
    }

    firstSet[symbol] = result;
    return result;
}

// Compute FOLLOW set
void computeFollow(const string &symbol) {
    if (symbol == startSymbol)
        followSet[symbol].insert("$");

    for (auto &rule : grammar) {
        for (auto &prod : rule.second) {
            for (int i = 0; i < prod.size(); ++i) {
                if (prod[i] == symbol) {
                    int j = i + 1;
                    bool epsilonFound = false;

                    while (j < prod.size()) {
                        set<string> firstNext = computeFirst(prod[j]);

                        for (auto &f : firstNext)
                            if (f != "ε")
                                followSet[symbol].insert(f);

                        if (firstNext.find("ε") != firstNext.end()) {
                            epsilonFound = true;
                            j++;
                        } else {
                            epsilonFound = false;
                            break;
                        }
                    }

                    if (j == prod.size() || epsilonFound) {
                        if (rule.first != symbol) {
                            computeFollow(rule.first);
                            followSet[symbol].insert(followSet[rule.first].begin(),
                                                     followSet[rule.first].end());
                        }
                    }
                }
            }
        }
    }
}

// Display FIRST or FOLLOW sets
void displaySet(const map<string, set<string>> &setMap, const string &title) {
    cout << title << ":\n";
    for (auto &entry : setMap) {
        cout << title << "(" << entry.first << ") = { ";
        for (auto it = entry.second.begin(); it != entry.second.end(); ++it) {
            if (it != entry.second.begin()) cout << ", ";
            cout << *it;
        }
        cout << " }\n";
    }
}

int main() {
    // Hardcoded Grammar
    grammar["S"] = {{"A", "B", "C"}, {"D"}};
    grammar["A"] = {{"a"}, {"ε"}};
    grammar["B"] = {{"b"}, {"ε"}};
    grammar["C"] = {{"(", "S", ")"}, {"c"}};
    grammar["D"] = {{"A", "C"}};

    nonTerminals = {"S", "A", "B", "C", "D"};
    terminals = {"a", "b", "c", "(", ")", "ε"};

    // Compute FIRST
    for (const auto &nt : nonTerminals)
        computeFirst(nt);

    // Compute FOLLOW
    for (const auto &nt : nonTerminals)
        computeFollow(nt);

    // Display results
    displaySet(firstSet, "First");
    displaySet(followSet, "Follow");

    return 0;
}
