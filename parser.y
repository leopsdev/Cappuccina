%{
/*** Cabeçalho C ***/
#include "ast.h"
#include "eval.h"
#include <stdio.h>
#include <stdlib.h>

// Variável global para a raiz da AST
ASTNode* raiz_ast = NULL;

// Protótipos
void yyerror(const char* s);
int yylex();
%}

%union {
    int num;
    char* id;
    ASTNode* node;
}

%token DEF REC MU MU_LIM PROJ RECPROJ ZERO SUCC ISZERO
%token IGUAL ABRE_PAR FECHA_PAR VIRGULA
%token <num> NUMERO
%token <id> ID_FUNCAO PREDICADO

%type <node> programa funcao expressao params args

%start programa

%%

/*** Gramática ***/
programa:
    funcao                    { raiz_ast = $1; }
    | programa funcao         { $$ = $1; registrarFuncao($2); }
    ;

funcao:
    DEF ID_FUNCAO ABRE_PAR params FECHA_PAR IGUAL expressao {
        $$ = criarNoFuncao($2, $4, $7);
    }
    ;

params:
    ID_FUNCAO                { $$ = criarNoParam($1); }
    | params VIRGULA ID_FUNCAO { $$ = adicionarParam($1, $3); }
    ;

expressao:
    NUMERO                   { $$ = criarNoNumero($1); }
    | ID_FUNCAO ABRE_PAR args FECHA_PAR { $$ = criarNoChamada($1, $3); }
    | PROJ ABRE_PAR NUMERO FECHA_PAR { $$ = criarNoProjecao($3); }
    | RECPROJ ABRE_PAR NUMERO FECHA_PAR { $$ = criarNoRecProjecao($3); }
    | REC ABRE_PAR expressao VIRGULA expressao FECHA_PAR {
        $$ = criarNoRecursao($3, $5);
    }
    | MU ABRE_PAR ID_FUNCAO VIRGULA args FECHA_PAR {
        $$ = criarNoMinimizacaoIlimitada($3, $5);
    }
    | MU_LIM ABRE_PAR expressao VIRGULA ID_FUNCAO VIRGULA args FECHA_PAR {
        $$ = criarNoMinimizacaoLimitada($5, $7, $3);
    }
    | ABRE_PAR expressao FECHA_PAR { $$ = $2; }
    | ZERO ABRE_PAR FECHA_PAR           { $$ = criarNoPrimitiva("zero", NULL); }
    | SUCC ABRE_PAR expressao FECHA_PAR { $$ = criarNoPrimitiva("succ", $3); }
    | ISZERO ABRE_PAR expressao FECHA_PAR { $$ = criarNoPrimitiva("iszero", $3); }
    ;

args:
    expressao                { $$ = criarNoArg($1); }
    | args VIRGULA expressao { $$ = adicionarArg($1, $3); }
    ;

%%

/*** Funções Auxiliares ***/
extern int yylineno;
extern char* yytext;

void yyerror(const char *s) {
    fprintf(stderr, "Erro de sintaxe na linha %d: %s\n", yylineno, s);
    fprintf(stderr, "Erro de sintaxe: %s no token '%s'\n", s, yytext);
}
