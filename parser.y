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
    Expressao** vetor;
}

%token MAIN PRINT
%token N S U DEF SOMA
%token ABRE_P FECHA_P ABRE_CO FECHA_CO VIRG P_VIRG PA_A PA_F
%token <str> IDENT
%token <num> NUM
%type <expr> expressao
%type <vetor> vet
%type <expr> expressoes




%%

programa:
    lista_definicoes main_bloco
    ;

main_bloco:
    MAIN ABRE_P FECHA_P bloco_principal
    ;

bloco_principal:
    PA_A lista_comandos PA_F
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
    definicao1
    | definicao2
    | lista_definicoes definicao1
    | lista_definicoes definicao2
    ;

definicao1:
    DEF IDENT ABRE_P parametros FECHA_P '=' expressao P_VIRG {
        printf("Função '%s' definida com sucesso.\n", $2);
    }
    ;

definicao2:
    DEF IDENT PA_A bloco PA_F {
        printf("Função '%s' definida com sucesso.\n", $2);
    }
    ;

bloco:
    | IDENT ABRE_P parametros VIRG '0' FECHA_P '=' expressao P_VIRG bloco_2{
        printf("Bloco parte 1\n");
    }
    ;

bloco_2:
    | IDENT ABRE_P parametros FECHA_P '=' expressao P_VIRG{
        printf("Bloco parte 2\n");
    }
    ;


parametros:
    IDENT
    | parametros VIRG IDENT
    ;

expressoes:
    expressao {
        $$ = $1;
        $1->proximo = NULL;
    }
    | expressoes VIRG expressao {
        Expressao* atual = $1;
        while (atual->proximo) atual = atual->proximo;
        atual->proximo = $3;
        $$ = $1;
    }

vet:
    ABRE_CO expressoes FECHA_CO {
        // Cria uma nova expressão do tipo U para armazenar o vetor
        Expressao* e = malloc(sizeof(Expressao));
        e->tipo = T_U;
        e->proj.elementos = malloc(sizeof(Expressao*) * 100); // Limite arbitrário
        e->proj.num_elementos = 0;

        Expressao* atual = $2;
        while (atual) {
            e->proj.elementos[e->proj.num_elementos++] = atual;
            atual = atual->proximo; // Supondo que você tenha uma estrutura de lista ligada
        }

        $$ = e->proj.elementos;
    }
    ;

expressao:
    NUM { $$ = nova_expr_num($1); }
    | IDENT { $$ = nova_expr_ident($1); }
    | N { $$ = nova_expr_n(); }
    | S ABRE_P expressao FECHA_P { $$ = nova_expr_s($3); }
    | U ABRE_P vet VIRG NUM VIRG NUM FECHA_P {
        $$ = nova_expr_u($3, $5, $7);
        
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
