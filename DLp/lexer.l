%{
#include <stdio.h>
#include "y.tab.h"
%}

%option noyywrap
%option yylineno

%%
i           { return I; }
e           { return E_TOKEN; }
t           { return T; }
a           { return A; }
b           { return B; }
\n          { return 0; } // End of input
[ \t]       { /* Ignore whitespace */ }
.           { return -1; } // Invalid character
%%

int yyerror(const char *s) {
    return -1;
}