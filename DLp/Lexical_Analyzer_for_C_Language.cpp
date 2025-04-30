#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <regex>
#include <algorithm>

using namespace std;

// Token types
enum TokenType {
    KEYWORD, IDENTIFIER, CONSTANT, STRING, PUNCTUATION, OPERATOR
};

// Token structure
struct Token {
    TokenType type;
    string value;
};

// Lexical Analyzer class
class LexicalAnalyzer {
private:
    set<string> keywords = {
        "int", "char", "return", "void", "if", "else", "for", "while"
    };
    set<char> punctuation = {'(', ')', '{', '}', ';', ','};
    set<string> operators = {"=", "+", "-", "*", "/", "==", "!=", "<", ">", "<=", ">="};
    vector<Token> tokens;
    vector<string> symbol_table;
    vector<string> errors;
    string source_code;

    bool is_keyword(const string& word) {
        return keywords.find(word) != keywords.end();
    }

    bool is_operator(const string& word) {
        return operators.find(word) != operators.end();
    }

    bool is_valid_identifier(const string& word) {
        return regex_match(word, regex("^[a-zA-Z_][a-zA-Z0-9_]*$"));
    }

    void add_to_symbol_table(const string& identifier) {
        if (find(symbol_table.begin(), symbol_table.end(), identifier) == symbol_table.end()) {
            symbol_table.push_back(identifier);
        }
    }

    void tokenize(const string& code) {
        string current;
        bool in_string = false;
        bool in_comment = false;
        bool in_multi_comment = false;

        for (size_t i = 0; i < code.length(); i++) {
            char c = code[i];

            // Handle comments
            if (in_multi_comment) {
                if (c == '*' && i + 1 < code.length() && code[i + 1] == '/') {
                    in_multi_comment = false;
                    i++;
                }
                continue;
            }
            if (in_comment) {
                if (c == '\n') in_comment = false;
                continue;
            }
            if (c == '/' && i + 1 < code.length()) {
                if (code[i + 1] == '/') {
                    in_comment = true;
                    i++;
                    continue;
                }
                if (code[i + 1] == '*') {
                    in_multi_comment = true;
                    i++;
                    continue;
                }
            }

            // Handle strings
            if (c == '\'' && !in_string) {
                in_string = true;
                current += c;
                continue;
            }
            if (in_string) {
                current += c;
                if (c == '\'' && code[i-1] != '\\') {
                    in_string = false;
                    tokens.push_back({STRING, current});
                    current.clear();
                }
                continue;
            }

            // Handle whitespace
            if (isspace(c)) {
                if (!current.empty()) {
                    process_token(current);
                    current.clear();
                }
                continue;
            }

            // Handle punctuation
            if (punctuation.find(c) != punctuation.end()) {
                if (!current.empty()) {
                    process_token(current);
                    current.clear();
                }
                tokens.push_back({PUNCTUATION, string(1, c)});
                continue;
            }

            // Handle operators
            string potential_op = string(1, c);
            if (i + 1 < code.length() && is_operator(potential_op + code[i + 1])) {
                if (!current.empty()) {
                    process_token(current);
                    current.clear();
                }
                tokens.push_back({OPERATOR, potential_op + code[i + 1]});
                i++;
                continue;
            }
            if (is_operator(potential_op)) {
                if (!current.empty()) {
                    process_token(current);
                    current.clear();
                }
                tokens.push_back({OPERATOR, potential_op});
                continue;
            }

            current += c;
        }

        if (!current.empty()) {
            process_token(current);
        }
    }

    void process_token(const string& token) {
        if (is_keyword(token)) {
            tokens.push_back({KEYWORD, token});
        }
        else if (regex_match(token, regex("^[0-9]+$"))) {
            tokens.push_back({CONSTANT, token});
        }
        else if (is_valid_identifier(token)) {
            tokens.push_back({IDENTIFIER, token});
            add_to_symbol_table(token);
        }
        else if (!token.empty() && token != "'" && token != "/*" && token != "*/") {
            errors.push_back(token + " invalid lexeme");
        }
    }

public:
    void analyze(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            source_code += line + "\n";
        }
        file.close();

        tokenize(source_code);
        
        // Print tokens
        cout << "TOKENS\n";
        for (const auto& token : tokens) {
            switch (token.type) {
                case KEYWORD: cout << "Keyword: "; break;
                case IDENTIFIER: cout << "Identifier: "; break;
                case CONSTANT: cout << "Constant: "; break;
                case STRING: cout << "String: "; break;
                case PUNCTUATION: cout << "Punctuation: "; break;
                case OPERATOR: cout << "Operator: "; break;
            }
            cout << token.value << "\n";
        }

        // Print lexical errors
        cout << "\nLEXICAL ERRORS\n";
        if (errors.empty()) {
            cout << "None\n";
        } else {
            for (const auto& error : errors) {
                cout << error << "\n";
            }
        }

        // Print symbol table
        cout << "\nSYMBOL TABLE ENTRIES\n";
        for (size_t i = 0; i < symbol_table.size(); i++) {
            cout << i + 1 << ") " << symbol_table[i] << "\n";
        }
    }
};

int main() {
    LexicalAnalyzer lexer;
    lexer.analyze("p.txt");
    return 0;
}