#include <iostream>
#include <stack>
#include <vector>
#include <sstream>
#include <cctype>
#include <map>
#include <cmath>

using namespace std;

bool isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

int precedence(char op) {
    if(op == '^') return 3;
    if(op == '*' || op == '/') return 2;
    if(op == '+' || op == '-') return 1;
    return 0;
}

bool isVariable(const string& s) {
    return isalpha(s[0]);
}

bool isNumber(const string& s) {
    istringstream iss(s);
    double d;
    return (iss >> d) && (iss.eof());
}

// Evaluate a simple binary operation between two numeric strings
string evaluate(const string& a, const string& b, char op) {
    double d1 = stod(a);
    double d2 = stod(b);
    double result = 0;

    switch(op) {
        case '+': result = d1 + d2; break;
        case '-': result = d1 - d2; break;
        case '*': result = d1 * d2; break;
        case '/': result = d1 / d2; break;
        case '^': result = pow(d1, d2); break;
    }

    ostringstream oss;
    oss << result;
    return oss.str();
}

// Convert infix to postfix with constant folding
vector<string> constantFold(const string& expr) {
    stack<char> opStack;
    stack<string> valStack;
    string token, result;
    vector<string> output;
    
    for (size_t i = 0; i < expr.length(); ++i) {
        char ch = expr[i];
        
        if (isspace(ch)) continue;
        
        // Collect multi-character tokens
        if (isalnum(ch) || ch == '.') {
            token += ch;
            if (i == expr.size() - 1 || (!isalnum(expr[i + 1]) && expr[i + 1] != '.')) {
                valStack.push(token);
                token.clear();
            }
        } 
        else if (ch == '(') {
            opStack.push(ch);
        } 
        else if (ch == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                char op = opStack.top(); opStack.pop();
                string b = valStack.top(); valStack.pop();
                string a = valStack.top(); valStack.pop();

                if (isNumber(a) && isNumber(b)) {
                    valStack.push(evaluate(a, b, op));
                } else {
                    valStack.push("(" + a + " " + op + " " + b + ")");
                }
            }
            if (!opStack.empty()) opStack.pop(); // pop '('
        } 
        else if (isOperator(ch)) {
            while (!opStack.empty() && precedence(opStack.top()) >= precedence(ch)) {
                char op = opStack.top(); opStack.pop();
                string b = valStack.top(); valStack.pop();
                string a = valStack.top(); valStack.pop();

                if (isNumber(a) && isNumber(b)) {
                    valStack.push(evaluate(a, b, op));
                } else {
                    valStack.push("(" + a + " " + op + " " + b + ")");
                }
            }
            opStack.push(ch);
        }
    }

    while (!opStack.empty()) {
        char op = opStack.top(); opStack.pop();
        string b = valStack.top(); valStack.pop();
        string a = valStack.top(); valStack.pop();

        if (isNumber(a) && isNumber(b)) {
            valStack.push(evaluate(a, b, op));
        } else {
            valStack.push("(" + a + " " + op + " " + b + ")");
        }
    }

    // Remove outer parentheses if not needed
    result = valStack.top();
    if (result[0] == '(' && result[result.size()-1] == ')') {
        result = result.substr(1, result.size()-2);
    }
    
    output.push_back(result);
    return output;
}

int main() {
    string input;
    cout << "Enter arithmetic expression: ";
    getline(cin, input);

    vector<string> optimized = constantFold(input);

    cout << "Optimized Expression: " << optimized[0] << endl;

    return 0;
}
