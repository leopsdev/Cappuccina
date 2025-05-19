%{
#include "ast.h"
#include "parser.tab.h"  
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* copiarString(const char* texto) {
    char* str = (char*) malloc(strlen(texto) + 1);
    strcpy(str, texto);
    return str;
}
%}

DIGITO   [0-9]
LETRA    [a-zA-Z]
ID_FUN   [a-z][a-z0-9_]*     
PRED     [A-Z][A-Z0-9_]*     
ESPACO   [ \t\n]

%%

"def"       { return DEF; }
"rec"       { return REC; }
"mu"        { return MU; }
"mu_lim"    { return MU_LIM; }
"proj"      { return PROJ; }
"recproj"   { return RECPROJ; }
"zero"      { return ZERO; }
"succ"      { return SUCC; }
"iszero"    { return ISZERO; }

{ID_FUN}    { yylval.id = copiarString(yytext); return ID_FUNCAO; }
{PRED}      { yylval.id = copiarString(yytext); return PREDICADO; }

{DIGITO}+   { yylval.num = atoi(yytext); return NUMERO; }

"="         { return IGUAL; }
"("         { return ABRE_PAR; }
")"         { return FECHA_PAR; }
","         { return VIRGULA; }

{ESPACO}+   { /* Ignorar espaços em branco */ }

.           { fprintf(stderr, "Caractere inválido: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}