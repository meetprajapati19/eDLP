#include <iostream>
#include <string>

using namespace std;

class RecursiveDescentParser {
private:
    string input;
    size_t index;

    // Helper function to get current character
    char current_char() {
        if (index < input.length()) {
            return input[index];
        }
        return '\0'; // End of input
    }

    // Helper function to advance to next character
    void advance() {
        if (index < input.length()) {
            index++;
        }
    }

    // S → ( L ) | a
    bool parse_S() {
        if (current_char() == 'a') {
            advance();
            return true;
        }
        if (current_char() == '(') {
            advance();
            if (parse_L()) {
                if (current_char() == ')') {
                    advance();
                    return true;
                }
            }
            return false;
        }
        return false;
    }

    // L → S L’
    bool parse_L() {
        if (parse_S()) {
            return parse_L_prime();
        }
        return false;
    }

    // L’ → , S L’ | ε
    bool parse_L_prime() {
        if (current_char() == ',') {
            advance();
            if (parse_S()) {
                return parse_L_prime();
            }
            return false;
        }
        return true; // ε (empty string)
    }

public:
    RecursiveDescentParser(const string& str) : input(str), index(0) {}

    bool validate() {
        bool result = parse_S();
        // Check if entire input is consumed
        return result && index == input.length();
    }
};

int main() {
    string input;
    cout << "Enter input string: ";
    getline(cin, input);

    RecursiveDescentParser parser(input);
    if (parser.validate()) {
        cout << "Valid string\n";
    } else {
        cout << "Invalid string\n";
    }

    return 0;
}