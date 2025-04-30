#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string pattern;
    std::string input;

    std::cout << "Enter regular expression: ";
    std::getline(std::cin, pattern);

    std::cout << "Enter string to validate: ";
    std::getline(std::cin, input);

    try {
        std::regex re(pattern);
        if (std::regex_match(input, re)) {
            std::cout << "Valid String\n";
        } else {
            std::cout << "Invalid String\n";
        }
    } catch (const std::regex_error& e) {
        std::cout << "Invalid regular expression: " << e.what() << "\n";
    }

    return 0;
}
