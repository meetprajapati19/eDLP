#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

// Grammar representation
struct Grammar {
    map<char, vector<string>> productions;
    set<char> non_terminals = {'S', 'A', 'B', 'C', 'D'};
    set<char> terminals = {'a', 'b', '(', ')', 'c'};
    const char epsilon = '@'; // Represent ε
    const char end_marker = '$'; // End of input
};

// Function to compute First sets
void compute_first(const Grammar& grammar, map<char, set<char>>& first) {
    // Initialize First sets
    for (char nt : grammar.non_terminals) {
        first[nt] = {};
    }

    bool changed;
    do {
        changed = false;
        for (const auto& prod : grammar.productions) {
            char nt = prod.first; // Non-terminal
            for (const string& rhs : prod.second) { // Right-hand side
                if (rhs == string(1, grammar.epsilon)) {
                    // If production is ε, add ε to First(nt)
                    if (first[nt].insert(grammar.epsilon).second) {
                        changed = true;
                    }
                } else {
                    bool all_epsilon = true;
                    for (char symbol : rhs) {
                        if (grammar.terminals.count(symbol)) {
                            // If symbol is terminal, add to First(nt)
                            if (first[nt].insert(symbol).second) {
                                changed = true;
                            }
                            all_epsilon = false;
                            break;
                        } else if (grammar.non_terminals.count(symbol)) {
                            // If symbol is non-terminal, add First(symbol) - {ε} to First(nt)
                            for (char f : first[symbol]) {
                                if (f != grammar.epsilon) {
                                    if (first[nt].insert(f).second) {
                                        changed = true;
                                    }
                                }
                            }
                            // If ε not in First(symbol), stop
                            if (first[symbol].count(grammar.epsilon) == 0) {
                                all_epsilon = false;
                                break;
                            }
                        }
                    }
                    // If all symbols derive ε, add ε to First(nt)
                    if (all_epsilon && first[nt].insert(grammar.epsilon).second) {
                        changed = true;
                    }
                }
            }
        }
    } while (changed);
}

// Function to compute Follow sets
void compute_follow(const Grammar& grammar, const map<char, set<char>>& first, map<char, set<char>>& follow) {
    // Initialize Follow sets
    for (char nt : grammar.non_terminals) {
        follow[nt] = {};
    }
    // Add $ to Follow(S) (start symbol)
    follow['S'].insert(grammar.end_marker);

    bool changed;
    do {
        changed = false;
        for (const auto& prod : grammar.productions) {
            char nt = prod.first;
            for (const string& rhs : prod.second) {
                // Process each symbol in the production
                for (size_t i = 0; i < rhs.length(); i++) {
                    char symbol = rhs[i];
                    if (!grammar.non_terminals.count(symbol)) continue; // Skip terminals

                    // Compute First of the string after symbol
                    set<char> next_first;
                    bool all_epsilon = true;
                    for (size_t j = i + 1; j < rhs.length() && all_epsilon; j++) {
                        char next = rhs[j];
                        if (grammar.terminals.count(next)) {
                            next_first.insert(next);
                            all_epsilon = false;
                            break;
                        } else {
                            for (char f : first.at(next)) {
                                if (f != grammar.epsilon) {
                                    next_first.insert(f);
                                }
                            }
                            if (first.at(next).count(grammar.epsilon) == 0) {
                                all_epsilon = false;
                            }
                        }
                    }

                    // Add First of next symbols to Follow(symbol)
                    for (char f : next_first) {
                        if (follow[symbol].insert(f).second) {
                            changed = true;
                        }
                    }

                    // If all subsequent symbols can derive ε, add Follow(nt) to Follow(symbol)
                    if (all_epsilon) {
                        for (char f : follow[nt]) {
                            if (follow[symbol].insert(f).second) {
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
    } while (changed);
}

// Function to print sets
void print_set(const string& prefix, char nt, const set<char>& s, char epsilon) {
    cout << prefix << "(" << nt << ") = {";
    bool first = true;
    for (char c : s) {
        if (!first) cout << ", ";
        if (c == epsilon) cout << "ε";
        else cout << c;
        first = false;
    }
    cout << "}\n";
}

int main() {
    Grammar grammar;
    // Define productions
    grammar.productions['S'] = {"ABC", "D"};
    grammar.productions['A'] = {"a", string(1, grammar.epsilon)};
    grammar.productions['B'] = {"b", string(1, grammar.epsilon)};
    grammar.productions['C'] = {"(S)", "c"};
    grammar.productions['D'] = {"AC"};

    // Compute First and Follow sets
    map<char, set<char>> first, follow;
    compute_first(grammar, first);
    compute_follow(grammar, first, follow);

    // Print First sets
    for (char nt : grammar.non_terminals) {
        print_set("First", nt, first[nt], grammar.epsilon);
    }

    // Print Follow sets
    for (char nt : grammar.non_terminals) {
        print_set("Follow", nt, follow[nt], grammar.epsilon);
    }

    return 0;
}