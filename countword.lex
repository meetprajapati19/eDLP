%{
#include <stdio.h>

int charCount = 0;
int wordCount = 0;
int lineCount = 0;
%}

%%
\n               { charCount++; lineCount++; }
[ \t\r]+         { charCount += yyleng; } // whitespace other than newline
[A-Za-z0-9_]+    { wordCount++; charCount += yyleng; } // words (basic alphanumerics)
.                { charCount += yyleng; } // any other single character
%%

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    yyin = file;
    yylex();

    printf("Characters : %d\n", charCount);
    printf("Words : %d\n", wordCount);
    printf("Line : %d\n", lineCount);

    fclose(file);
    return 0;
}

int yywrap() {
    return 1;
}

