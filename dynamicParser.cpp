#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

// Type aliases
using Rule = vector<string>;
using Grammar = unordered_map<string, vector<Rule>>;

class DynamicParser {
    Grammar grammar;
    vector<string> tokens;
    int pos;

public:
    DynamicParser(Grammar g, const vector<string>& inputTokens)
        : grammar(g), tokens(inputTokens), pos(0) {}

    bool parse(const string& startSymbol) {
        bool success = parseRule(startSymbol);
        return success && pos == tokens.size();
    }

private:
    bool parseRule(const string& symbol) {
        // Terminal case
        if (grammar.find(symbol) == grammar.end()) {
            if (pos < tokens.size() && tokens[pos] == symbol) {
                pos++;
                return true;
            }
            return false;
        }

        // Non-terminal case
        for (auto& rule : grammar[symbol]) {
            int savedPos = pos;
            bool success = true;

            for (auto& token : rule) {
                if (token == "ε") continue; // handle epsilon (empty string)
                if (!parseRule(token)) {
                    success = false;
                    break;
                }
            }

            if (success) return true;
            pos = savedPos; // backtrack
        }

        return false;
    }
};

// Utility function to split string by space
vector<string> split(const string& line) {
    stringstream ss(line);
    string token;
    vector<string> tokens;
    while (ss >> token)
        tokens.push_back(token);
    return tokens;
}

int main() {
    // Define grammar for:
    // S → ( L ) | a
    // L → S L'
    // L' → , S L' | ε

    Grammar grammar;

    grammar["S"] = {{"(", "L", ")"}, {"a"}};
    grammar["L"] = {{"S", "L'"}};
    grammar["L'"] = {{",", "S", "L'"}, {"ε"}}; // ε represents empty string

    cout << "Enter string to validate (space-separated tokens, e.g. ( a ) or ( a , a )):\n";
    string input;
    getline(cin, input);
    vector<string> inputTokens = split(input);

    DynamicParser parser(grammar, inputTokens);
    if (parser.parse("S")) {
        cout << "Valid string" << endl;
    } else {
        cout << "Invalid string" << endl;
    }

    return 0;
}

