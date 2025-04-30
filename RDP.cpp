#include <iostream>
#include <string>
using namespace std;

class RDPParser {
    string input;
    int pos;

public:
    RDPParser(const string& str) {
        input = "";
        for (char ch : str) {
            if (ch != ' ') input += ch;  // remove spaces
        }
        pos = 0;
    }

    bool parse() {
        bool isValid = S();
        return isValid && pos == input.length();
    }

private:
    // S → ( L ) | a
    bool S() {
        if (match('(')) {
            if (L()) {
                if (match(')')) {
                    return true;
                }
            }
            return false;
        }
        else if (match('a')) {
            return true;
        }
        return false;
    }

    // L → S L’
    bool L() {
        if (S()) {
            return Ldash();
        }
        return false;
    }

    // L’ → , S L’ | ε
    bool Ldash() {
        if (match(',')) {
            if (S()) {
                return Ldash();  // recursive
            }
            return false;
        }
        return true;  // epsilon production
    }

    // Match current character with expected
    bool match(char expected) {
        if (pos < input.length() && input[pos] == expected) {
            pos++;
            return true;
        }
        return false;
    }
};

int main() {
    string input;
    cout << "Enter a string to validate: ";
    getline(cin, input);

    RDPParser parser(input);
    if (parser.parse()) {
        cout << "Valid string" << endl;
    } else {
        cout << "Invalid string" << endl;
    }

    return 0;
}
