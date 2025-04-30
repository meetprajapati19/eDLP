%{
#include <stdio.h>
#include <string.h>

/* Token types */
#define KEYWORD 1
#define IDENTIFIER 2
#define CONSTANT 3
#define STRING 4
#define PUNCTUATION 5
#define OPERATOR 6

/* Global variables for error tracking */
char error_buffer[1000][100];
int error_count = 0;

/* Function to add lexical errors */
void add_error(const char *lexeme) {
    snprintf(error_buffer[error_count], 100, "%s invalid lexeme", lexeme);
    error_count++;
}

/* Function to print token */
void print_token(int type, const char *value) {
    switch (type) {
        case KEYWORD: printf("Keyword: %s\n", value); break;
        case IDENTIFIER: printf("Identifier: %s\n", value); break;
        case CONSTANT: printf("Constant: %s\n", value); break;
        case STRING: printf("String: %s\n", value); break;
        case PUNCTUATION: printf("Punctuation: %s\n", value); break;
        case OPERATOR: printf("Operator: %s\n", value); break;
    }
}
%}

%option noyywrap
%option yylineno

/* Definitions */
DIGIT       [0-9]
LETTER      [a-zA-Z_]
WHITESPACE  [ \t\n]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*
CONSTANT    {DIGIT}+
STRING      '([^'\\]|\\.)*'
PUNCTUATION [();,{}]
OPERATOR    [=+\-*/%]|==|!=|<=|>=|<|>
KEYWORD     int|char|return|if|else|for|while|void

/* States for comments */
%x SINGLE_COMMENT
%x MULTI_COMMENT

%%
{WHITESPACE}+           { /* Ignore whitespace */ }

"//"                    { BEGIN(SINGLE_COMMENT); }
<SINGLE_COMMENT>.       { /* Ignore single-line comment content */ }
<SINGLE_COMMENT>\n      { BEGIN(INITIAL); }

"/*"                    { BEGIN(MULTI_COMMENT); }
<MULTI_COMMENT>.|\n     { /* Ignore multi-line comment content */ }
<MULTI_COMMENT>"*/"     { BEGIN(INITIAL); }

{KEYWORD}               { print_token(KEYWORD, yytext); }
{IDENTIFIER}            { 
    if (yytext[0] >= '0' && yytext[0] <= '9') {
        add_error(yytext);
    } else {
        print_token(IDENTIFIER, yytext);
    }
}
{CONSTANT}              { print_token(CONSTANT, yytext); }
{STRING}                { print_token(STRING, yytext); }
{PUNCTUATION}           { print_token(PUNCTUATION, yytext); }
{OPERATOR}              { print_token(OPERATOR, yytext); }

.                       { add_error(yytext); }

%%

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("Cannot open file %s\n", argv[1]);
        return 1;
    }

    yyin = file;
    printf("TOKENS\n");
    yylex();

    printf("\nLEXICAL ERRORS\n");
    if (error_count == 0) {
        printf("None\n");
    } else {
        for (int i = 0; i < error_count; i++) {
            printf("%s\n", error_buffer[i]);
        }
    }

    fclose(file);
    return 0;
}