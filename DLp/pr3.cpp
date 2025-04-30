#include <iostream>
#include <fstream>
#include <cctype>
#include <unordered_set>
#include <vector>
#include <regex>

using namespace std;


unordered_set<string> keywords = {"int", "char", "return", "if", "else", "while", "for", "float", "double"};

unordered_set<char> operators = {'+', '-', '*', '/', '=', '%', '>', '<', '!'};

unordered_set<char> punctuation = {';', ',', '(', ')', '{', '}', '[', ']'};
bool isKeyword(const string& word) {
    return keywords.find(word) != keywords.end();
}
bool isIdentifier(const string& word) {
    return regex_match(word, regex("[a-zA-Z_][a-zA-Z0-9_]*"));
}
bool isConstant(const string& word) {
    return regex_match(word, regex("[0-9]+"));
}
void lexa(const string& fm) {
    ifstream file(fm);
    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return;
    }

    string token;
    char ch;
    vector<string> symbolTable;
    vector<string> errors;

    cout << "TOKENS\n";
    while (file.get(ch)) {
        // Ignore whitespace
        if (isspace(ch)) continue;

        // Remove comments
        if (ch == '/') {
            char nextChar;
            if (file.get(nextChar)) {
                if (nextChar == '/') {
                    while (file.get(ch) && ch != '\n');
                    continue;
                } else if (nextChar == '*') {
                    while (file.get(ch)) {
                        if (ch == '*') {
                            if (file.get(nextChar) && nextChar == '/') break;
                        }
                    }
                    continue;
                } else {
                    file.unget();
                }
            }
        }

        // Identify punctuation
        if (punctuation.find(ch) != punctuation.end()) {
            cout << "Punctuation: " << ch << endl;
            continue;
        }

        // Identify operators
        if (operators.find(ch) != operators.end()) {
            cout << "Operator: " << ch << endl;
            continue;
        }
        if (isalnum(ch) || ch == '_') {
            token = ch;
            while (file.get(ch) && (isalnum(ch) || ch == '_')) {
                token += ch;
            }
            file.unget();

            if (isKeyword(token)) {
                cout << "Keyword: " << token << endl;
            } else if (isConstant(token)) {
                cout << "Constant: " << token << endl;
            } else if (isIdentifier(token)) {
                cout << "Identifier: " << token << endl;
                symbolTable.push_back(token);
            } else {
                errors.push_back(token + " invalid lexeme");
            }
            continue;
        }

        if (ch == '\'') {
            token = "";
            while (file.get(ch) && ch != '\'') {
                token += ch;
            }
            cout << "String: '" << token << "'" << endl;
            continue;
        }
    }
    
    file.close();
    if (!errors.empty()) {
        cout << "\nLEXICAL ERRORS" << endl;
        for (const auto& err : errors) {
            cout << err << endl;
        }
    }

    // Display symbol table
    if (!symbolTable.empty()) {
        cout << "\nSYMBOL TABLE ENTRIES" << endl;
        int index = 1;
        for (const auto& id : symbolTable) {
            cout << index++ << ") " << id << endl;
        }
    }
}

int main() {
    string fm = "practical3.txt"; 
    lexa(fm);
    return 0;
}