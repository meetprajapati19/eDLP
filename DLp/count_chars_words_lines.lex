%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int char_count = 0, word_count = 0, line_count = 0;
int whitespace_count = 0, tab_count = 0, special_char_count = 0;
int keyword_count = 0, operator_count = 0, identifier_count = 0, constant_count = 0, punctuation_count = 0,l=0;

// C keywords array
char *keywords[] = {"int", "float", "char", "double", "if", "else", "while", "for", "return", "void", "switch", "case", "break", "continue", "struct", "union", "typedef", "long", "short", "signed", "unsigned", "const", "static", "extern", "sizeof", "volatile"};
#define NUM_KEYWORDS 24

int is_keyword(char *word) {
    for (int i = 0; i < NUM_KEYWORDS; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}
%}

%%
[a-zA-Z_][a-zA-Z0-9_]*  {
    if (is_keyword(yytext)) {  // If it's a keyword
        keyword_count++;       // Count as a keyword
    } else {                   // Otherwise, it's an identifier
        identifier_count++;    // Count as an identifier
    }
    word_count++;              // Count it as a word
    char_count += yyleng;      // Count characters
}

['"a-zA-Z_][a-zA-Z0-9_]*['"]  {
    if (is_keyword(yytext)) {  // If it's a keyword
        keyword_count++;       // Count as a keyword
    } else {                   // Otherwise, it's an identifier
        identifier_count++;    // Count as an identifier
    }
    word_count++;              // Count it as a word
    char_count += yyleng;      // Count characters
}

[0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)? { 
    constant_count++; 
    char_count += yyleng; 
}

\".*\"  { 
    constant_count++; 
    char_count += yyleng; 
}



[\+\-\*/=<>!&|^%]  {
    operator_count++; 
    char_count += yyleng; 
}

[{}();,]  { 
    punctuation_count++; 
    char_count += yyleng; 
}

[ \t]  { 
    char_count += yyleng; 
    whitespace_count++; 
    if (yytext[0] == '\t') tab_count++; 
}
\n  { 
    line_count++; 
    char_count++; 
}
[@$%^&]  { 
    special_char_count++; 
    char_count++; 
}

%%

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    yyin = file;
    yylex();
    fclose(file);

    // Print the counts
    printf("Characters: %d\n", char_count);
    printf("Words: %d\n", word_count);
    printf("Lines: %d\n", line_count);
    printf("Whitespaces: %d\n", whitespace_count);
    printf("Tabs: %d\n", tab_count);
    printf("Special Characters: %d\n", special_char_count);
    printf("Keywords: %d\n", keyword_count);
    printf("Operators: %d\n", operator_count);
    printf("Identifiers: %d\n", identifier_count);
    printf("Constants: %d\n", constant_count);
    printf("Punctuation: %d\n", punctuation_count);
    printf("Punctuation: %d\n", l);

    return 0;
}

int yywrap() {
    return 1;
}
