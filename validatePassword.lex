%{
#include <stdio.h>
#include <string.h>
int lowercase = 0, uppercase = 0, digit = 0, symbol = 0;
char password[100];
%}

%%

.  {
    int len = strlen(password);
    if(len < sizeof(password) - 1) {
        password[len] = yytext[0];
        password[len + 1] = '\0';

        if (yytext[0] >= 'a' && yytext[0] <= 'z')
            lowercase = 1;
        else if (yytext[0] >= 'A' && yytext[0] <= 'Z')
            uppercase = 1;
        else if (yytext[0] >= '0' && yytext[0] <= '9')
            digit = 1;
        else if (strchr("*;#$@", yytext[0]))
            symbol = 1;
    }
}

\n  {
    int len = strlen(password);
    if(len >= 9 && len <= 15 && lowercase && uppercase && digit && symbol)
        printf("Valid password\n");
    else
        printf("Invalid password\n");

    // Reset everything for next input
    lowercase = uppercase = digit = symbol = 0;
    password[0] = '\0';
}

%%

int main() {
    password[0] = '\0';
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
