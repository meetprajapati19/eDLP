%{
#include <stdio.h>
%}

%%

[0-9]+     { printf("%s\n", yytext); }   // Match and print each number
.|\n       ;                             // Ignore all other characters

%%

int main() {
    printf("Enter a string: ");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
