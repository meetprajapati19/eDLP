%{
#include <stdio.h>
#include <string.h>
%}

%%
[Cc][Hh][Aa][Rr][Uu][Ss][Aa][Tt]     { printf("university"); }  // Match "charusat" in any case
.|\n                                { printf("%s", yytext); }   // Print all other characters as is
%%

int main() {
    printf("Enter a string:\n");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
