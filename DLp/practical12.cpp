#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include <cmath>
#include <iomanip>
using namespace std;

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

bool isNumeric(const string& s) {
    char* end = nullptr;
    strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0';
}

vector<string> tokenize(const string& expr) {
    vector<string> tokens;
    stringstream ss;
    for (size_t i = 0; i < expr.size(); ++i) {
        char c = expr[i];
        if (isspace(c)) continue;
        if (isdigit(c) || c == '.') {
            ss.str(""); ss.clear(); ss << c;
            while (i + 1 < expr.size() && (isdigit(expr[i + 1]) || expr[i + 1] == '.')) ss << expr[++i];
            tokens.push_back(ss.str());
        } else if (isalpha(c)) {
            ss.str(""); ss.clear(); ss << c;
            while (i + 1 < expr.size() && isalnum(expr[i + 1])) ss << expr[++i];
            tokens.push_back(ss.str());
        } else {
            tokens.push_back(string(1, c));
        }
    }
    return tokens;
}

// Convert infix to postfix
vector<string> infixToPostfix(const vector<string>& tokens) {
    stack<string> ops;
    vector<string> output;
    for (const string& token : tokens) {
        if (isNumeric(token) || isalpha(token[0])) {
            output.push_back(token);
        } else if (token == "(") {
            ops.push(token);
        } else if (token == ")") {
            while (!ops.empty() && ops.top() != "(") {
                output.push_back(ops.top()); ops.pop();
            }
            if (!ops.empty()) ops.pop();  // remove '('
        } else if (isOperator(token[0])) {
            while (!ops.empty() && precedence(ops.top()[0]) >= precedence(token[0])) {
                output.push_back(ops.top()); ops.pop();
            }
            ops.push(token);
        }
    }
    while (!ops.empty()) {
        output.push_back(ops.top()); ops.pop();
    }
    return output;
}

// Evaluate postfix with constant folding
vector<string> foldConstants(const vector<string>& postfix) {
    stack<string> s;
    for (const string& token : postfix) {
        if (isNumeric(token) || isalpha(token[0])) {
            s.push(token);
        } else if (isOperator(token[0])) {
            string b = s.top(); s.pop();
            string a = s.top(); s.pop();
            if (isNumeric(a) && isNumeric(b)) {
                double x = stod(a), y = stod(b), result = 0;
                if (token == "+") result = x + y;
                else if (token == "-") result = x - y;
                else if (token == "*") result = x * y;
                else if (token == "/") result = x / y;
                s.push(to_string(result));
            } else {
                s.push(a + " " + token + " " + b);
            }
        }
    }
    return {s.top()};
}

// Optimize expression
string optimizeExpression(const string& expr) {
    auto tokens = tokenize(expr);
    auto postfix = infixToPostfix(tokens);
    auto folded = foldConstants(postfix);
    return folded[0];
}

// MAIN
int main() {
    vector<string> testCases = {
        "5 + x - 3 * 2",
        "2 + 3 * 4 - 1",
        "x + (3 * 5) - 2",
        "( 22 / 7 ) * r * r"
    };

    for (const string& expr : testCases) {
        cout << "Input:    " << expr << endl;
        string result = optimizeExpression(expr);
        cout << "Optimized:" << result << "\n" << endl;
    }

    return 0;
}
