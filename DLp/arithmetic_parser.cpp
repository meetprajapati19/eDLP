#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cmath>
#include <stdexcept>

using namespace std;

// Token types
enum TokenType { NUMBER, PLUS, MINUS, MULT, DIV, EXP, LPAREN, RPAREN, END, ERROR };

// Token structure
struct Token {
    TokenType type;
    double value; // For NUMBER tokens
    char symbol;  // For operator tokens
};

// Parser class
class ExpressionEvaluator {
private:
    string input;
    size_t pos;
    stack<Token> tokenStack;
    stack<double> valueStack;

    // Get next token
    Token getNextToken() {
        while (pos < input.length() && isspace(input[pos])) pos++;
        
        if (pos >= input.length()) return {END, 0, '\0'};

        char c = input[pos];
        
        // Number (integer or decimal)
        if (isdigit(c) || c == '.') {
            string numStr;
            bool hasDot = (c == '.');
            numStr += c;
            pos++;
            
            while (pos < input.length() && (isdigit(input[pos]) || input[pos] == '.')) {
                if (input[pos] == '.') {
                    if (hasDot) return {ERROR, 0, '\0'};
                    hasDot = true;
                }
                numStr += input[pos];
                pos++;
            }
            
            try {
                return {NUMBER, stod(numStr), '\0'};
            } catch (...) {
                return {ERROR, 0, '\0'};
            }
        }
        
        pos++;
        switch (c) {
            case '+': return {PLUS, 0, '+'};
            case '-': return {MINUS, 0, '-'};
            case '*': return {MULT, 0, '*'};
            case '/': return {DIV, 0, '/'};
            case '^': return {EXP, 0, '^'};
            case '(': return {LPAREN, 0, '('};
            case ')': return {RPAREN, 0, ')'};
            default: return {ERROR, 0, '\0'};
        }
    }

    // Check precedence for shift-reduce parsing
    int precedence(TokenType type) {
        switch (type) {
            case EXP: return 3;
            case MULT:
            case DIV: return 2;
            case PLUS:
            case MINUS: return 1;
            default: return 0;
        }
    }

    // Apply semantic rule
    void applyRule(TokenType type) {
        double val1, val2;
        switch (type) {
            case PLUS:
                val2 = valueStack.top(); valueStack.pop();
                val1 = valueStack.top(); valueStack.pop();
                valueStack.push(val1 + val2);
                break;
            case MINUS:
                val2 = valueStack.top(); valueStack.pop();
                val1 = valueStack.top(); valueStack.pop();
                valueStack.push(val1 - val2);
                break;
            case MULT:
                val2 = valueStack.top(); valueStack.pop();
                val1 = valueStack.top(); valueStack.pop();
                valueStack.push(val1 * val2);
                break;
            case DIV:
                val2 = valueStack.top(); valueStack.pop();
                if (val2 == 0) throw runtime_error("Division by zero");
                val1 = valueStack.top(); valueStack.pop();
                valueStack.push(val1 / val2);
                break;
            case EXP:
                val2 = valueStack.top(); valueStack.pop();
                val1 = valueStack.top(); valueStack.pop();
                valueStack.push(pow(val1, val2));
                break;
        }
    }

public:
    ExpressionEvaluator(const string& expr) : input(expr), pos(0) {}

    double evaluate() {
        TokenType lastOp = END;
        bool expectNumber = true;

        while (true) {
            Token token = getNextToken();
            
            if (token.type == ERROR) {
                throw runtime_error("Invalid expression");
            }

            if (token.type == END) {
                while (!tokenStack.empty()) {
                    if (tokenStack.top().type == LPAREN) {
                        throw runtime_error("Invalid expression: unmatched parenthesis");
                    }
                    applyRule(tokenStack.top().type);
                    tokenStack.pop();
                }
                if (valueStack.empty()) throw runtime_error("Invalid expression");
                return valueStack.top();
            }

            if (token.type == NUMBER) {
                if (!expectNumber) throw runtime_error("Invalid expression");
                valueStack.push(token.value);
                expectNumber = false;
            } else if (token.type == LPAREN) {
                tokenStack.push(token);
                expectNumber = true;
            } else if (token.type == RPAREN) {
                while (!tokenStack.empty() && tokenStack.top().type != LPAREN) {
                    applyRule(tokenStack.top().type);
                    tokenStack.pop();
                }
                if (tokenStack.empty()) throw runtime_error("Invalid expression: unmatched parenthesis");
                tokenStack.pop(); // Pop LPAREN
                expectNumber = false;
            } else { // Operator
                if (expectNumber) throw runtime_error("Invalid expression");
                while (!tokenStack.empty() && tokenStack.top().type != LPAREN &&
                       precedence(tokenStack.top().type) >= precedence(token.type)) {
                    applyRule(tokenStack.top().type);
                    tokenStack.pop();
                }
                tokenStack.push(token);
                expectNumber = true;
            }
        }
    }
};

int main() {
    string input;
    cout << "Enter arithmetic expression: ";
    getline(cin, input);

    try {
        ExpressionEvaluator evaluator(input);
        double result = evaluator.evaluate();
        cout << "Result: " << result << endl;
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }

    return 0;
}