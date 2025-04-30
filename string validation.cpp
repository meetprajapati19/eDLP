#include <stdio.h>
#include <string.h>

int isValid(char str[]) {
    int i = 0;

    // Accept any number of 'a's
    while (str[i] == 'a') {
        i++;
    }

    // After a's, we must have exactly two b's and nothing else
    if (str[i] == 'b' && str[i + 1] == 'b' && str[i + 2] == '\0') {
        return 1; // Valid
    }

    return 0; // Invalid
}

int main() {
    char input[100];
    printf("Enter a string: ");
    fgets(input, sizeof(input), stdin);

    // Remove newline character if present
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    if (isValid(input)) {
        printf("Valid String\n");
    } else {
        printf("Invalid String\n");
    }

    return 0;
}
