%{
#include "y.tab.h"
%}

%%

"i"     { return IF; }
"t"     { return THEN; }
"e"     { return ELSE; }
"a"     { return A; }
"b"     { return B; }
[ \t\n] ;      // Ignore whitespace
.       { return yytext[0]; }

%%
int yywrap() { return 1; }
