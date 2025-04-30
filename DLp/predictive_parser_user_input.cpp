#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <stack>
#include <iomanip>
#include <sstream>

using namespace std;

// Grammar representation
struct Grammar {
    map<char, vector<string>> productions;
    set<char> non_terminals;
    set<char> terminals;
    map<char, set<char>> first;
    map<char, set<char>> follow;
    char epsilon = '@'; // Represent ε
    char end_marker = '$'; // End of input
    char start_symbol;

    void input_grammar() {
        string line;

        // Input non-terminals
        cout << "Enter non-terminals (single characters, space-separated, end with newline): ";
        getline(cin, line);
        istringstream iss_nt(line);
        char nt;
        while (iss_nt >> nt) {
            non_terminals.insert(nt);
        }
        start_symbol = *non_terminals.begin(); // Assume first non-terminal is start symbol

        // Input terminals
        cout << "Enter terminals (single characters, space-separated, end with newline): ";
        getline(cin, line);
        istringstream iss_t(line);
        char t;
        while (iss_t >> t) {
            terminals.insert(t);
        }

        // Input productions
        cout << "Enter number of productions: ";
        int num_prods;
        cin >> num_prods;
        cin.ignore(); // Clear newline
        cout << "Enter productions (format: X -> rhs, ε as @, | for alternatives, one per line):\n";
        for (int i = 0; i < num_prods; i++) {
            getline(cin, line);
            istringstream iss_p(line);
            char lhs;
            string arrow, rhs;
            iss_p >> lhs >> arrow;
            getline(iss_p, rhs);
            // Split rhs by '|' for alternatives
            istringstream iss_rhs(rhs);
            string prod;
            while (getline(iss_rhs, prod, '|')) {
                // Trim leading/trailing spaces
                prod.erase(0, prod.find_first_not_of(" "));
                prod.erase(prod.find_last_not_of(" ") + 1);
                if (prod.empty()) prod = string(1, epsilon);
                productions[lhs].push_back(prod);
            }
        }

        // Ask if user wants to provide First/Follow sets
        cout << "Do you want to provide First and Follow sets? (y/n): ";
        char choice;
        cin >> choice;
        cin.ignore();
        if (choice == 'y' || choice == 'Y') {
            // Input First sets
            cout << "Enter First sets (format: First(X) = {a b c}, one per non-terminal):\n";
            for (char nt : non_terminals) {
                cout << "First(" << nt << ") = ";
                getline(cin, line);
                istringstream iss_f(line);
                char c;
                first[nt] = {};
                while (iss_f >> c) {
                    if (c == '@') c = epsilon;
                    first[nt].insert(c);
                }
            }

            // Input Follow sets
            cout << "Enter Follow sets (format: Follow(X) = {a b c}, one per non-terminal):\n";
            for (char nt : non_terminals) {
                cout << "Follow(" << nt << ") = ";
                getline(cin, line);
                istringstream iss_f(line);
                char c;
                follow[nt] = {};
                while (iss_f >> c) {
                    if (c == '@') c = epsilon;
                    if (c == '$') c = end_marker;
                    follow[nt].insert(c);
                }
            }
        } else {
            compute_first();
            compute_follow();
        }
    }

    void compute_first() {
        for (char nt : non_terminals) {
            first[nt] = {};
        }

        bool changed;
        do {
            changed = false;
            for (const auto& prod : productions) {
                char nt = prod.first;
                for (const string& rhs : prod.second) {
                    if (rhs == string(1, epsilon)) {
                        if (first[nt].insert(epsilon).second) {
                            changed = true;
                        }
                    } else {
                        bool all_epsilon = true;
                        for (char symbol : rhs) {
                            if (terminals.count(symbol)) {
                                if (first[nt].insert(symbol).second) {
                                    changed = true;
                                }
                                all_epsilon = false;
                                break;
                            } else if (non_terminals.count(symbol)) {
                                for (char f : first[symbol]) {
                                    if (f != epsilon) {
                                        if (first[nt].insert(f).second) {
                                            changed = true;
                                        }
                                    }
                                }
                                if (first[symbol].count(epsilon) == 0) {
                                    all_epsilon = false;
                                    break;
                                }
                            }
                        }
                        if (all_epsilon && first[nt].insert(epsilon).second) {
                            changed = true;
                        }
                    }
                }
            }
        } while (changed);
    }

    void compute_follow() {
        for (char nt : non_terminals) {
            follow[nt] = {};
        }
        follow[start_symbol].insert(end_marker);

        bool changed;
        do {
            changed = false;
            for (const auto& prod : productions) {
                char nt = prod.first;
                for (const string& rhs : prod.second) {
                    for (size_t i = 0; i < rhs.length(); i++) {
                        char symbol = rhs[i];
                        if (!non_terminals.count(symbol)) continue;

                        set<char> next_first;
                        bool all_epsilon = true;
                        for (size_t j = i + 1; j < rhs.length() && all_epsilon; j++) {
                            char next = rhs[j];
                            if (terminals.count(next)) {
                                next_first.insert(next);
                                all_epsilon = false;
                                break;
                            } else {
                                for (char f : first.at(next)) {
                                    if (f != epsilon) {
                                        next_first.insert(f);
                                    }
                                }
                                if (first.at(next).count(epsilon) == 0) {
                                    all_epsilon = false;
                                }
                            }
                        }

                        for (char f : next_first) {
                            if (follow[symbol].insert(f).second) {
                                changed = true;
                            }
                        }

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
};

// Predictive parsing table
using ParseTable = map<pair<char, char>, string>;

void print_parse_table(const ParseTable& table, const Grammar& grammar) {
    const int col_width = 10;
    set<char> all_terminals = grammar.terminals;
    all_terminals.insert(grammar.end_marker);

    cout << "\nPredictive Parsing Table\n";
    cout << string(7 + all_terminals.size() * col_width, '=') << "\n";
    cout << setw(5) << "NT" << " |";
    for (char t : all_terminals) {
        cout << setw(col_width - 1) << t << "|";
    }
    cout << "\n";
    cout << string(7 + all_terminals.size() * col_width, '-') << "\n";

    for (char nt : grammar.non_terminals) {
        cout << setw(5) << nt << " |";
        for (char t : all_terminals) {
            auto key = make_pair(nt, t);
            string entry = table.count(key) ? table.at(key) : "";
            if (entry == string(1, grammar.epsilon)) entry = "ε";
            cout << setw(col_width - 1) << left << entry << "|";
        }
        cout << "\n";
    }
    cout << string(7 + all_terminals.size() * col_width, '=') << "\n\n";
}

bool construct_parse_table(const Grammar& grammar, ParseTable& table) {
    bool is_ll1 = true;

    for (const auto& prod : grammar.productions) {
        char nt = prod.first;
        for (const string& rhs : prod.second) {
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

            for (char t : first_set) {
                auto key = make_pair(nt, t);
                if (table.count(key)) {
                    is_ll1 = false;
                }
                table[key] = rhs;
            }

            if (all_epsilon || rhs == string(1, grammar.epsilon)) {
                for (char t : grammar.follow.at(nt)) {
                    auto key = make_pair(nt, t);
                    if (table.count(key)) {
                        is_ll1 = false;
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
    stk.push(grammar.start_symbol);
    size_t pos = 0;

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
            return false;
        }

        auto key = make_pair(top, current);
        if (!table.count(key)) {
            return false;
        }

        string production = table.at(key);
        stk.pop();

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
    grammar.input_grammar();

    ParseTable table;
    bool is_ll1 = construct_parse_table(grammar, table);

    print_parse_table(table, grammar);

    cout << "Grammar Analysis: The grammar is " << (is_ll1 ? "" : "not ") << "LL(1)\n\n";

    if (is_ll1) {
        string input;
        cout << "Enter input string: ";
        getline(cin, input);

        if (!input.empty() && input.back() == '\n') {
            input.pop_back();
        }

        cout << "String Validation Result: ";
        if (validate_string(input, grammar, table)) {
            cout << "Valid string\n";
        } else {
            cout << "Invalid string\n";
        }
    }

    return 0;
}