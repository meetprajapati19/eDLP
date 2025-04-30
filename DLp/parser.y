%{
#include <stdio.h>
#include <stdlib.h>
extern int yylex();
extern char *yytext;
int valid = 0; // Flag to track parsing success
void yyerror(const char *s);
%}

%token I E_TOKEN T A B

%%
start: S { valid = 1; } // Set valid flag on successful parse

S: I E_TOKEN T S S_prime
 | A
 ;

S_prime: E_TOKEN S
 | /* epsilon */
 ;

E_TOKEN: B
 ;
%%

void yyerror(const char *s) {
    // Errors handled in main
}

int main() {
    printf("Enter input string: ");
    yyparse();
    if (valid) {
        printf("Valid string\n");
    } else {
        printf("Invalid string\n");
    }
    return 0;
}