%{
#include <stdio.h>
%}

%%

charusat   { printf("university"); }
.          { printf("%s", yytext); }

%%

int main() {
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
