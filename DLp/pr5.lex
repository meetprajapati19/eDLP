%{
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int is_keyword(char *word);
%}

%%

"auto"|"break"|"case"|"char"|"const"|"continue"|"default"|"do"|"double"|"else"|"enum"|"extern"|"float"|"for"|"goto"|"if"|"inline"|"int"|"long"|"register"|"restrict"|"return"|"short"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"void"|"volatile"|"while"  { printf("Keyword: %s\n", yytext); }

[a-zA-Z_][a-zA-Z0-9_]*   { printf("Identifier: %s\n", yytext); }
[0-9]+                    { printf("Constant: %s\n", yytext); }
"'".[^']*"'"             { printf("String: %s\n", yytext); }
[{}()\[\],;]              { printf("Punctuation: %s\n", yytext); }
[=+\-*/%<>!&|]            { printf("Operator: %s\n", yytext); }
"//".*                   { /* Ignore single-line comments */ }
"/*"([^*]|\*+[^*/])*"*/" { /* Ignore multi-line comments */ }

[ \t\n]+                  { /* Ignore whitespace */ }
.                         { printf("Lexical Error: %s\n", yytext); }

%%

int main() {
    char filename[100];
    printf("Enter C source file name: ");
    scanf("%s", filename);
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    yyin = file;
    yylex();
    fclose(file);
    return 0;
}

int yywrap() {
    return 1;
}
