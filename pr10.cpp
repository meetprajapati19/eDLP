#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <cmath>
#include <cctype>
#include <stdexcept>

using namespace std;

// Operator precedence
int precedence(char op) {
    if(op == '^') return 3;
    if(op == '*' || op == '/') return 2;
    if(op == '+' || op == '-') return 1;
    return 0;
}

// Apply an operator to two operands
double applyOp(double a, double b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if(b == 0) throw runtime_error("Division by zero");
            return a / b;
        case '^': return pow(a, b);
    }
    return 0;
}

// Check if character is an operator
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// Evaluate the expression using stacks (Bottom-Up Parsing)
double evaluate(const string& expr) {
    stack<double> values;
    stack<char> ops;
    size_t i = 0;

    while(i < expr.length()) {
        if(isspace(expr[i])) {
            i++;
            continue;
        }

        if(expr[i] == '(') {
            ops.push(expr[i]);
            i++;
        }
        else if(isdigit(expr[i]) || expr[i] == '.') {
            string numStr;
            while(i < expr.length() && (isdigit(expr[i]) || expr[i] == '.'))
                numStr += expr[i++];
            values.push(stod(numStr));
        }
        else if(expr[i] == ')') {
            while(!ops.empty() && ops.top() != '(') {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(a, b, op));
            }
            if(ops.empty()) throw runtime_error("Mismatched parentheses");
            ops.pop(); // pop '('
            i++;
        }
        else if(isOperator(expr[i])) {
            char currentOp = expr[i];
            while(!ops.empty() && precedence(ops.top()) >= precedence(currentOp) && currentOp != '^') {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(a, b, op));
            }
            ops.push(currentOp);
            i++;
        }
        else {
            throw runtime_error("Invalid character in expression");
        }
    }

    while(!ops.empty()) {
        if(values.size() < 2) throw runtime_error("Invalid expression");
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOp(a, b, op));
    }

    if(values.size() != 1) throw runtime_error("Invalid expression");
    return values.top();
}

int main() {
    string input;
    cout << "Enter arithmetic expression: ";
    getline(cin, input);

    try {
        double result = evaluate(input);
        cout << "Result: " << result << endl;
    } catch(const exception& e) {
        cout << "Invalid expression" << endl;
    }

    return 0;
}
