%code requires {
    #include "aux.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include "aux.h"

int yylex(void);
void yyerror(const char *s);

%}

%union {
    char* str;
    int num;
    Expressao* expr;
    
}

%token MAIN PRINT
%token N S U DEF SOMA
%token ABRE_P FECHA_P ABRE_CO FECHA_CO VIRG P_VIRG
%token <str> IDENT
%token <num> NUM
%type <expr> expressao


%%

programa:
    lista_definicoes main_bloco
    ;

main_bloco:
    MAIN ABRE_P FECHA_P bloco_principal
    ;

bloco_principal:
    '{' lista_comandos '}'
    ;

lista_comandos:
    comando
    | lista_comandos comando
    ;

comando:
    PRINT ABRE_P expressao FECHA_P P_VIRG {
        printf("Tipo da expressão: %d\n", $3->tipo);
        int resultado = avaliar($3);
        printf("Resultado: %d\n", resultado);
    }
    ;

lista_definicoes:
    definicao
    | lista_definicoes definicao
    ;

definicao:
    DEF IDENT ABRE_P parametros FECHA_P '=' expressao P_VIRG {
        printf("Função '%s' definida com sucesso.\n", $2);
    }
    ;


parametros:
    IDENT
    | parametros VIRG IDENT
    ;

expressao:
    NUM { $$ = nova_expr_num($1); }
    | IDENT { $$ = nova_expr_ident($1); }
    | N { $$ = nova_expr_n(); }
    | S ABRE_P expressao FECHA_P { $$ = nova_expr_s($3); }
    | U ABRE_P expressao VIRG expressao FECHA_P {
        $$ = nova_expr_u($3, $5);
    }
    | SOMA ABRE_P expressao VIRG expressao FECHA_P {
        $$ = nova_expr_soma($3, $5);
    }
    ;

%%


void yyerror(const char *s) {
    fprintf(stderr, "Erro: %s\n", s);
}

int main(void) {
    printf("Digite uma definição de função:\n");
    return yyparse();
}
