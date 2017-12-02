%{

#include <stdlib.h>
#include "yacc.tab.h"

%}

%%
[a-zA-Z]+               {printf("Shift ID\n"); yylval.string = yytext; return ID;}
0|[1-9][0-9]*           {printf("Shift NUMBER\n"); yylval.number = atoi(yytext); return NUMBER;}
\+                      {printf("Shift PLUS\n"); return PLUS;}
\-                      {printf("Shift MINUS\n"); return MINUS;}
\*                      {printf("Shift MULTIPLY\n"); return MULTIPLY;}
\/                      {printf("Shift DIVIDE\n"); return DIVIDE;}
\;                      {printf("Shift SEMICOLON\n"); return SEMICOLON;}
[ \t]                   // Skip whitespace and tab
\n
%%
