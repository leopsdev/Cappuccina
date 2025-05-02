%{
#include <stdio.h>
#include "parser.tab.h"
%}

%option noyywrap

%%
main                { return MAIN; }
print               { return PRINT; }
n                   { return N; }
s                   { return S; }
u                   { return U; }
def                 { return DEF; }
soma                { return SOMA; }
[a-zA-Z_][a-zA-Z0-9_]*   { yylval.str = strdup(yytext); return IDENT; }
[0-9]+              { return NUM; }
"("                 { return ABRE_P; }
")"                 { return FECHA_P; }
"["                 { return ABRE_CO; }
"]"                 { return FECHA_CO; }
","                 { return VIRG; }
";"                 { return P_VIRG; }
[ \t\n]+            ; // ignora espaços

.                   { return *yytext; }
%%

