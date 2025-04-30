#include <iostream>
#include <string>

using namespace std;

string input;
size_t index = 0;

bool S();
bool L();
bool L_();
bool match(char c);
string removeSpaces(string str);

int main() {
    cout << "Enter the string to validate: ";
    getline(cin, input);

    input = removeSpaces(input); 
    index = 0;

    if (S() && index == input.length()) {
        cout << "Valid string" << endl;
    } else {
        cout << "Invalid string" << endl;
    }

    return 0;
}

// Grammar rule: S → ( L ) | a
bool S() {
    if (match('a')) { // S → a
        return true;
    } else if (match('(')) { // S → ( L )
        if (L()) {
            return match(')');
        }
    }
    return false;
}

// Grammar rule: L → S L'
bool L() {
    if (S()) {
        return L_();
    }
    return false;
}

// Grammar rule: L' → , S L' | ε
bool L_() {
    if (match(',')) {
        if (S()) {
            return L_();
        }
        return false;
    }
    return true; // ε transition (empty string allowed)
}

// Checks if the current character matches the expected one
bool match(char c) {
    if (index < input.length() && input[index] == c) {
        index++;
        return true;
    }
    return false;
}

// Removes spaces from the input string
string removeSpaces(string str) {
    string result;
    for (char c : str) {
        if (c != ' ') {
            result += c;
        }
    }
    return result;
}





#include <iostream>
#include <cctype>
#include <string>

using namespace std;

string input;
size_t index = 0;

bool E();
bool E_prime();
bool T();
bool T_prime();
bool F();
bool match(char c);
void skipSpaces();

bool E() { // E -> T E'
    if (T()) {
        return E_prime();
    }
    return false;
}

bool E_prime() { // E' -> * T E' | / T E' | ε
    skipSpaces();
    if (match('*') || match('/')) {
        if (T()) {
            return E_prime();
        }
        return false;
    }
    return true; // ε transition
}

bool T() { // T -> F T'
    if (F()) {
        return T_prime();
    }
    return false;
}

bool T_prime() { // T' -> + F T' | - F T' | ε
    skipSpaces();
    if (match('+') || match('-')) {
        if (F()) {
            return T_prime();
        }
        return false;
    }
    return true; // ε transition
}

bool F() { // F -> ( E ) | NUMBER
    skipSpaces();
    if (match('(')) {
        if (E() && match(')')) {
            return true;
        }
        return false;
    }

    // Check if the current character is a digit (NUMBER)
    if (isdigit(input[index])) {
        while (isdigit(input[index])) { // Consume all digits of NUMBER
            index++;
        }
        return true;
    }

    return false;
}

// Match function to compare current character with expected one
bool match(char c) {
    if (index < input.length() && input[index] == c) {
        index++;
        return true;
    }
    return false;
}

void skipSpaces() {
    while (index < input.length() && isspace(input[index])) {
        index++;
    }
}

int main() {
    cout << "Enter the expression to validate: ";
    getline(cin, input);

    index = 0; // Reset index
    skipSpaces(); // Handle leading spaces

    if (E() && index == input.length()) {
        cout << "Valid expression" << endl;
    } else {
        cout << "Invalid expression" << endl;
    }

    return 0;
}
