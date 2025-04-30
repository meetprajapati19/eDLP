#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <cctype>

using namespace std;

// Structure for a quadruple
struct Quadruple {
    string op;
    string op1;
    string op2;
    string result;
};

// Parser class
class Parser {
private:
    string input;
    size_t pos;
    vector<Quadruple> quadruples;
    int temp_count;

    // Generate a new temporary variable
    string new_temp() {
        return "t" + to_string(temp_count++);
    }

    // Convert double to string, removing .0 for integers
    string to_string(double val) {
        ostringstream oss;
        if (val == (int)val) {
            oss << (int)val;
        } else {
            oss << fixed << setprecision(2) << val;
        }
        return oss.str();
    }

    // Add a quadruple
    void add_quadruple(const string& op, const string& op1, const string& op2, const string& result) {
        quadruples.push_back({op, op1, op2, result});
    }

    // Get current character
    char current() {
        return pos < input.length() ? input[pos] : '\0';
    }

    // Move to next character
    void next() {
        if (pos < input.length()) pos++;
    }

    // Skip whitespace
    void skip_whitespace() {
        while (pos < input.length() && isspace(input[pos])) {
            next();
        }
    }

    // Parse a number (integer or decimal)
    bool parse_number(double& value, string& token) {
        skip_whitespace();
        size_t start = pos;
        string num_str;
        bool has_dot = false;

        while (pos < input.length() && (isdigit(input[pos]) || input[pos] == '.')) {
            if (input[pos] == '.') {
                if (has_dot) return false; // Multiple dots
                has_dot = true;
            }
            num_str += input[pos];
            next();
        }

        if (num_str.empty() || num_str == ".") return false;

        try {
            value = stod(num_str);
            token = to_string(value);
            return true;
        } catch (...) {
            return false;
        }
    }

    // Parse F → (E) | digit
    string parse_F() {
        skip_whitespace();
        if (current() == '(') {
            next(); // Consume '('
            string result = parse_E();
            if (current() != ')') {
                throw string("Invalid expression: missing ')'");
            }
            next(); // Consume ')'
            return result;
        } else {
            double value;
            string token;
            if (!parse_number(value, token)) {
                throw string("Invalid expression: expected number");
            }
            return token;
        }
    }

    // Parse T → T * F | T / F | F
    string parse_T() {
        string left = parse_F();
        skip_whitespace();
        while (current() == '*' || current() == '/') {
            char op = current();
            next(); // Consume '*' or '/'
            string right = parse_F();
            string temp = new_temp();
            add_quadruple(string(1, op), left, right, temp);
            left = temp;
            skip_whitespace();
        }
        return left;
    }

    // Parse E → E + T | E – T | T
    string parse_E() {
        string left = parse_T();
        skip_whitespace();
        while (current() == '+' || current() == '-') {
            char op = current();
            next(); // Consume '+' or '-'
            string right = parse_T();
            string temp = new_temp();
            add_quadruple(string(1, op), left, right, temp);
            left = temp;
            skip_whitespace();
        }
        return left;
    }

public:
    Parser(const string& expr) : input(expr), pos(0), temp_count(1) {}

    void parse() {
        try {
            parse_E();
            skip_whitespace();
            if (pos < input.length()) {
                throw string("Invalid expression: extra characters");
            }
            print_quadruples();
        } catch (const string& msg) {
            cout << msg << endl;
        }
    }

    void print_quadruples() {
        if (quadruples.empty()) return;
        cout << left << setw(10) << "Operator" << setw(11) << "Operand 1" << setw(11) << "Operand 2" << "Result" << endl;
        for (const auto& q : quadruples) {
            cout << left << setw(10) << q.op << setw(11) << q.op1 << setw(11) << q.op2 << q.result << endl;
        }
    }
};

int main() {
    string input;
    cout << "Enter arithmetic expression: ";
    getline(cin, input);

    // Remove trailing newline if present
    if (!input.empty() && input.back() == '\n') {
        input.pop_back();
    }

    Parser parser(input);
    parser.parse();

    return 0;
}