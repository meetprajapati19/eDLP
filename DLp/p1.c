#include <stdio.h>
#include <string.h>

int main() {
    char n[15];
    int i = 0;
    printf("Enter a string: ");
    scanf("%s", n);

    while (n[i] == 'a') {
        i++;
    }

    if (n[i] == 'b' && n[i + 1] == 'b' && n[i + 2] == '\0') {
        printf("Valid");
    } else {
        printf("Invalid.\n");
    }

    return 0;
}
