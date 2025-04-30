#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
#include <iomanip>
using namespace std;

struct Quadruple {
    string op;
    string arg1;
    string arg2;
    string result;
};

vector<string> tokens;
vector<Quadruple> quadruples;
int index = 0, tempCount = 1;

// Function to tokenize the input
void tokenize(const string& expr) {
    stringstream ss;
    for (char ch : expr) {
        if (isspace(ch)) continue;
        if (isdigit(ch) || ch == '.') {
            ss << ch;
        } else {
            if (!ss.str().empty()) {
                tokens.push_back(ss.str());
                ss.str("");
            }
            tokens.emplace_back(1, ch);
        }
    }
    if (!ss.str().empty()) tokens.push_back(ss.str());
}

string getToken() {
    return index < tokens.size() ? tokens[index] : "";
}

string consume() {
    return tokens[index++];
}

string factor();

string term() {
    string result = factor();
    while (getToken() == "*" || getToken() == "/") {
        string op = consume();
        string right = factor();
        string temp = "t" + to_string(tempCount++);
        quadruples.push_back({op, result, right, temp});
        result = temp;
    }
    return result;
}

string expression() {
    string result = term();
    while (getToken() == "+" || getToken() == "-") {
        string op = consume();
        string right = term();
        string temp = "t" + to_string(tempCount++);
        quadruples.push_back({op, result, right, temp});
        result = temp;
    }
    return result;
}

string factor() {
    string tok = getToken();
    if (tok == "(") {
        consume();
        string result = expression();
        if (consume() != ")") {
            throw runtime_error("Missing closing parenthesis");
        }
        return result;
    } else if (isdigit(tok[0]) || tok.find('.') != string::npos) {
        return consume();
    } else {
        throw runtime_error("Unexpected token: " + tok);
    }
}

void printQuadruples() {
    cout << left << setw(10) << "Operator" << setw(10) << "Operand 1"
         << setw(10) << "Operand 2" << "Result" << endl;
    for (auto& quad : quadruples) {
        cout << setw(10) << quad.op << setw(10) << quad.arg1
             << setw(10) << quad.arg2 << quad.result << endl;
    }
}

int main() {
    vector<string> expressions = {
        "9 + 42 * 8",
        "5 + 6 - 3",
        "7 - ( 8 * 2 )",
        "( 9 - 3 ) + ( 5 * 4 / 2 )",
        "(3 + 5 * 2 - 8) / 4 - 2 + 6",
        "86 / 2 / 3"
    };

    for (const auto& expr : expressions) {
        cout << "\nInput: " << expr << endl;
        tokens.clear();
        quadruples.clear();
        index = 0;
        tempCount = 1;

        try {
            tokenize(expr);
            expression();
            printQuadruples();
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    return 0;
}
