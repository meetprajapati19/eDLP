#include <iostream>
#include <stack>
#include <vector>
#include <cctype>
#include <sstream>
#include <iomanip>

using namespace std;

struct Quadruple {
    string op;
    string arg1;
    string arg2;
    string result;
};

vector<Quadruple> quadruples;
int tempCount = 1;

// Get a new temporary variable name
string newTemp() {
    return "t" + to_string(tempCount++);
}

// Check operator precedence
int precedence(char op) {
    if(op == '+' || op == '-') return 1;
    if(op == '*' || op == '/') return 2;
    return 0;
}

// Convert infix to postfix using Shunting Yard Algorithm
vector<string> infixToPostfix(const string& expr) {
    stack<char> ops;
    vector<string> postfix;
    string num;

    for(size_t i = 0; i < expr.length(); ++i) {
        char ch = expr[i];

        if(isspace(ch)) continue;

        if(isdigit(ch) || ch == '.') {
            num += ch;
            if(i == expr.length() - 1 || (!isdigit(expr[i + 1]) && expr[i + 1] != '.')) {
                postfix.push_back(num);
                num = "";
            }
        }
        else if(ch == '(') {
            ops.push(ch);
        }
        else if(ch == ')') {
            while(!ops.empty() && ops.top() != '(') {
                postfix.push_back(string(1, ops.top()));
                ops.pop();
            }
            if(!ops.empty()) ops.pop(); // Pop '('
        }
        else { // Operator
            while(!ops.empty() && precedence(ops.top()) >= precedence(ch)) {
                postfix.push_back(string(1, ops.top()));
                ops.pop();
            }
            ops.push(ch);
        }
    }

    while(!ops.empty()) {
        postfix.push_back(string(1, ops.top()));
        ops.pop();
    }

    return postfix;
}

// Generate quadruples from postfix expression
string generateQuadruples(const vector<string>& postfix) {
    stack<string> s;

    for(const string& token : postfix) {
        if(token == "+" || token == "-" || token == "*" || token == "/") {
            string b = s.top(); s.pop();
            string a = s.top(); s.pop();
            string t = newTemp();
            quadruples.push_back({token, a, b, t});
            s.push(t);
        } else {
            s.push(token);
        }
    }

    return s.top(); // Final result
}

void printQuadruples() {
    cout << "\nQuadruple Table:\n";
    cout << left << setw(10) << "Op"
         << setw(10) << "Arg1"
         << setw(10) << "Arg2"
         << setw(10) << "Result" << endl;

    for(const auto& q : quadruples) {
        cout << setw(10) << q.op
             << setw(10) << q.arg1
             << setw(10) << q.arg2
             << setw(10) << q.result << endl;
    }
}

int main() {
    string input;
    cout << "Enter arithmetic expression: ";
    getline(cin, input);

    try {
        auto postfix = infixToPostfix(input);
        generateQuadruples(postfix);
        printQuadruples();
    } catch (...) {
        cout << "Invalid expression!" << endl;
    }

    return 0;
}
