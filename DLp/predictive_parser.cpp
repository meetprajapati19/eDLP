#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <stack>
#include <iomanip>

using namespace std;

// Grammar representation
struct Grammar {
    map<char, vector<string>> productions;
    set<char> non_terminals = {'S', 'A', 'B', 'C', 'D'};
    set<char> terminals = {'a', 'b', '(', ')', 'c'};
    const char epsilon = '@'; // Represent ε
    const char end_marker = '$'; // End of input

    // First sets from Practical 7
    map<char, set<char>> first = {
        {'S', {'a', 'b', '(', 'c'}},
        {'A', {'a', '@'}},
        {'B', {'b', '@'}},
        {'C', {'(', 'c'}},
        {'D', {'a', '('}}
    };

    // Follow sets from Practical 7
    map<char, set<char>> follow = {
        {'S', {')', '$'}},
        {'A', {'b', '(', ')', '$'}},
        {'B', {'c', ')', '$'}},
        {'C', {')', '$'}},
        {'D', {')', '$'}}
    };

    Grammar() {
        // Define productions
        productions['S'] = {"ABC", "D"};
        productions['A'] = {"a", string(1, epsilon)};
        productions['B'] = {"b", string(1, epsilon)};
        productions['C'] = {"(S)", "c"};
        productions['D'] = {"AC"};
    }
};

// Predictive parsing table
using ParseTable = map<pair<char, char>, string>;

void print_parse_table(const ParseTable& table, const Grammar& grammar) {
    const int col_width = 10;
    cout << "\nPredictive Parsing Table\n";
    cout << string(7 + (grammar.terminals.size() + 1) * col_width, '=') << "\n";
    cout << setw(5) << "NT" << " |";
    for (char t : grammar.terminals) {
        cout << setw(col_width - 1) << t << "|";
    }
    cout << setw(col_width - 1) << grammar.end_marker << "|\n";
    cout << string(7 + (grammar.terminals.size() + 1) * col_width, '-') << "\n";

    for (char nt : grammar.non_terminals) {
        cout << setw(5) << nt << " |";
        for (char t : grammar.terminals) {
            auto key = make_pair(nt, t);
            string entry = table.count(key) ? table.at(key) : "";
            if (entry == string(1, grammar.epsilon)) entry = "ε";
            cout << setw(col_width - 1) << left << entry << "|";
        }
        auto key = make_pair(nt, grammar.end_marker);
        string entry = table.count(key) ? table.at(key) : "";
        if (entry == string(1, grammar.epsilon)) entry = "ε";
        cout << setw(col_width - 1) << left << entry << "|\n";
    }
    cout << string(7 + (grammar.terminals.size() + 1) * col_width, '=') << "\n\n";
}

bool construct_parse_table(const Grammar& grammar, ParseTable& table) {
    bool is_ll1 = true;

    for (const auto& prod : grammar.productions) {
        char nt = prod.first;
        for (const string& rhs : prod.second) {
            // Compute First of rhs
            set<char> first_set;
            bool all_epsilon = true;
            for (char symbol : rhs) {
                if (grammar.terminals.count(symbol)) {
                    first_set.insert(symbol);
                    all_epsilon = false;
                    break;
                } else if (grammar.non_terminals.count(symbol)) {
                    for (char f : grammar.first.at(symbol)) {
                        if (f != grammar.epsilon) {
                            first_set.insert(f);
                        }
                    }
                    if (grammar.first.at(symbol).count(grammar.epsilon) == 0) {
                        all_epsilon = false;
                        break;
                    }
                }
            }

            // Add entries for First(rhs)
            for (char t : first_set) {
                auto key = make_pair(nt, t);
                if (table.count(key)) {
                    is_ll1 = false; // Multiple entries
                }
                table[key] = rhs;
            }

            // If rhs can derive ε, add entries for Follow(nt)
            if (all_epsilon || rhs == string(1, grammar.epsilon)) {
                for (char t : grammar.follow.at(nt)) {
                    auto key = make_pair(nt, t);
                    if (table.count(key)) {
                        is_ll1 = false; // Multiple entries
                    }
                    table[key] = rhs;
                }
            }
        }
    }

    return is_ll1;
}

bool validate_string(const string& input, const Grammar& grammar, const ParseTable& table) {
    stack<char> stk;
    stk.push(grammar.end_marker);
    stk.push('S'); // Start symbol
    size_t pos = 0;

    // Add end marker to input
    string padded_input = input + grammar.end_marker;

    while (!stk.empty()) {
        char top = stk.top();
        char current = pos < padded_input.length() ? padded_input[pos] : grammar.end_marker;

        if (top == current) {
            stk.pop();
            pos++;
            continue;
        }

        if (grammar.terminals.count(top) || top == grammar.end_marker) {
            return false; // Mismatch
        }

        // Look up parsing table
        auto key = make_pair(top, current);
        if (!table.count(key)) {
            return false; // No production
        }

        string production = table.at(key);
        stk.pop();

        // Push production in reverse order (if not ε)
        if (production != string(1, grammar.epsilon)) {
            for (int i = production.length() - 1; i >= 0; i--) {
                stk.push(production[i]);
            }
        }
    }

    return pos == padded_input.length();
}

int main() {
    Grammar grammar;
    ParseTable table;

    // Construct parsing table
    bool is_ll1 = construct_parse_table(grammar, table);

    // Print parsing table
    print_parse_table(table, grammar);

    // Output LL(1) status
    cout << "Grammar Analysis: The grammar is " << (is_ll1 ? "" : "not ") << "LL(1)\n\n";

    // If LL(1), validate input string
    if (is_ll1) {
        string input;
        cout << "Enter input string: ";
        getline(cin, input);

        // Remove trailing newline if present
        if (!input.empty() && input.back() == '\n') {
            input.pop_back();
        }

        // Validate string
        cout << "String Validation Result: ";
        if (validate_string(input, grammar, table)) {
            cout << "Valid string\n";
        } else {
            cout << "Invalid string\n";
        }
    }

    return 0;
}