#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

map<char, vector<string>> grammar = {
    {'S', {"ABC", "D"}},
    {'A', {"a", "#"}},  // Using '#' as epsilon
    {'B', {"b", "#"}},
    {'C', {"(S)", "c"}},
    {'D', {"AC"}}
};

map<char, set<char>> first_sets;
map<char, set<char>> follow_sets;
char start_symbol = 'S';

// Function to compute First set
set<char> compute_first(char symbol) {
    if (first_sets.count(symbol) && !first_sets[symbol].empty())
        return first_sets[symbol];

    set<char> first;
    if (!isupper(symbol)) {
        first.insert(symbol);
    } else {
        for (const string &production : grammar[symbol]) {
            bool contains_epsilon = true;
            for (char sub_symbol : production) {
                set<char> sub_first = compute_first(sub_symbol);
                first.insert(sub_first.begin(), sub_first.end());
                first.erase('#');  // Remove epsilon
                
                if (sub_first.find('#') == sub_first.end()) {
                    contains_epsilon = false;
                    break;
                }
            }
            if (contains_epsilon) {
                first.insert('#');  // Add epsilon if all symbols can be epsilon
            }
        }
    }
    first_sets[symbol] = first;
    return first;
}

// Function to compute Follow set
void compute_follow() {
    follow_sets[start_symbol].insert('$');  // End marker for start symbol
    bool changed = true;

    while (changed) {
        changed = false;
        for (const auto &entry : grammar) {
            char lhs = entry.first;
            for (const string &production : entry.second) {
                set<char> trailer = follow_sets[lhs];

                for (auto it = production.rbegin(); it != production.rend(); ++it) {
                    char symbol = *it;
                    if (isupper(symbol)) {
                        size_t old_size = follow_sets[symbol].size();
                        follow_sets[symbol].insert(trailer.begin(), trailer.end());

                        if (first_sets[symbol].count('#')) {
                            set<char> temp = first_sets[symbol];
                            temp.erase('#');
                            trailer.insert(temp.begin(), temp.end());
                        } else {
                            trailer = first_sets[symbol];
                        }

                        if (follow_sets[symbol].size() > old_size) {
                            changed = true;
                        }
                    } else {
                        trailer = {symbol};
                    }
                }
            }
        }
    }
}

// Function to print First and Follow sets for only left-side non-terminals
void print_sets() {
    for (const auto &entry : grammar) {
        char non_terminal = entry.first;
        cout << "First(" << non_terminal << ") = { ";
        for (char c : first_sets[non_terminal]) cout << c << " ";
        cout << "}\n";
    }
    cout << "\n";
    for (const auto &entry : grammar) {
        char non_terminal = entry.first;
        cout << "Follow(" << non_terminal << ") = { ";
        for (char c : follow_sets[non_terminal]) cout << c << " ";
        cout << "}\n";
    }
}

int main() {
    for (const auto &entry : grammar) {
        compute_first(entry.first);
    }
    compute_follow();
    print_sets();
    return 0;
}

