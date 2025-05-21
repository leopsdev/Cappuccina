#include "eval.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define INT_MAX 1000000000

// Tabela de funções do usuário
typedef struct {
    char* nome;
    ASTNode* params;
    ASTNode* corpo;
} FuncaoUsuario;

FuncaoUsuario* funcoesUsuario = NULL;
int numFuncoesUsuario = 0;

// Tabela de funções built-in
FuncaoBuiltIn builtIns[] = {
    {"zero", prim_zero},
    {"succ", prim_succ},
    {"iszero", prim_iszero},
    // proj e recproj são tratadas separadamente
};
int numBuiltIns = sizeof(builtIns) / sizeof(builtIns[0]);

// Implementação das primitivas
int prim_zero(int* args, int num_args) { return 0; }

int prim_succ(int* args, int num_args) {
    if (num_args < 1) {
        fprintf(stderr, "Erro: succ requer 1 argumento\n");
        return 0;
    }
    return args[0] + 1;
}

int prim_iszero(int* args, int num_args) {
    return (args[0] == 0) ? 1 : 0;
}

int prim_proj(int* args, int num_args, int indice) {
    if (indice < 1 || indice > num_args) {
        fprintf(stderr, "Erro: índice de projeção inválido\n");
        return 0;
    }
    return args[num_args - indice];
}

int prim_recproj(int* args, int num_args, int indice) {
    // indice: 1-indexado
    // args: [x_{n}-1, h(...), x1, x2, ..., x_{n-1}]
    if (indice < 1 || indice > num_args) {
        fprintf(stderr, "Erro: recproj(%d) inválido\n", indice);
        return 0;
    }
    return args[indice-1];
}

// Avaliação genérica
int avaliar(ASTNode* node, int* args, int num_args) {
    //printf("Tipo %d\n", node->tipo);
    switch (node->tipo) {
        case NO_NUMERO:
            return node->numero;

        case NO_PROJECAO:
            return prim_proj(args, num_args, node->projecao.indice);

        case NO_RECPROJECAO:
            return prim_recproj(args, num_args, node->projecao.indice);

        case NO_RECURSAO: {
            int y = args[0];  // y é o PRIMEIRO argumento (args[0])
            int* x_args = &args[1];  // x1, x2,... são os argumentos restantes
            int n_x_args = num_args - 1;

            if (y == 0) {
                // Caso base: avalia apenas com x1,...,xn
                return avaliar(node->recursao.base, x_args, n_x_args);
            } else {
                // Vetor especial para recproj: [ (y-1), h(...), x1, ..., xn ]
                int recursive_args[n_x_args + 2];
                
                // recproj(1) = (y-1)
                recursive_args[0] = y - 1;
                
                // recproj(2) = h(x1,...,xn, y-1)
                int recurse_args[num_args];
                recurse_args[0] = y - 1;  // Novo y
                memcpy(&recurse_args[1], x_args, n_x_args * sizeof(int));  // Copia x's
                recursive_args[1] = avaliar(node, recurse_args, num_args);
                
                // Copia x1,...,xn para recproj(3+)
                memcpy(&recursive_args[2], x_args, n_x_args * sizeof(int));
                
                // Avalia o passo recursivo
                return avaliar(node->recursao.passo, recursive_args, n_x_args + 2);
            }
        }

        case NO_PRIMITIVA:
            if (strcmp(node->primitiva.nome, "zero") == 0) {
                return 0;
            } else if (strcmp(node->primitiva.nome, "succ") == 0) {
                int valor = avaliar(node->primitiva.argumento, args, num_args);
                return valor + 1;
            } else if (strcmp(node->primitiva.nome, "iszero") == 0) {
                int valor = avaliar(node->primitiva.argumento, args, num_args);
                return (valor == 0) ? 1 : 0;
            } else {
                fprintf(stderr, "Primitiva desconhecida: %s\n", node->primitiva.nome);
                exit(1);
            }
        
        case NO_MU: {
            const char* nomeFuncao = node->mu.nomeFuncao;
            int x_args[16];
            int n = 0;
            ASTNode* atual = node->mu.args;
            while (atual != NULL) {
                x_args[n++] = avaliar(atual, args, num_args);
                atual = atual->lista.proximo;
            }

            for (int z = 0; z < INT_MAX; z++) {
                int args_completos[17];
                memcpy(args_completos, x_args, n * sizeof(int));
                args_completos[n] = z;

                if (avaliarChamada(nomeFuncao, NULL, args_completos, n + 1) == 0) {
                    return z;
                }
            }

            fprintf(stderr, "Erro: minimização ilimitada não encontrou valor\n");
            return 0;
        }
        
        case NO_MULIM: {
            const char* nomeFuncao = node->mu_lim.nomeFuncao;

            int limite = avaliar(node->mu_lim.limite, args, num_args);

            int x_args[16];
            int n = 0;
            ASTNode* atual = node->mu_lim.args;
            while (atual != NULL) {
                x_args[n++] = avaliar(atual, args, num_args);
                atual = atual->lista.proximo;
            }

            for (int z = 0; z <= limite; z++) {
                int args_completos[17];
                memcpy(args_completos, x_args, n * sizeof(int));
                args_completos[n] = z;

                if (avaliarChamada(nomeFuncao, NULL, args_completos, n + 1) == 0) {
                    return z;
                }
            }
            fprintf(stderr, "Erro: minimização limitada não encontrou valor <= %d\n", limite);
            return 0;
        }

        case NO_CHAMADA:
            return avaliarChamada(node->chamada.nome, node->chamada.args, args, num_args);

        default:
            fprintf(stderr, "Erro: nó de AST desconhecido\n");
            return 0;
    }
}

int avaliarChamada(const char* nome, ASTNode* args_node, int* args, int num_args) {
    // 1. Verifica built-ins
    for (int i = 0; i < numBuiltIns; i++) {
        if (strcmp(nome, builtIns[i].nome) == 0) {
            int args_avaliados[16];
            int n = 0;
            while (args_node != NULL) {
                args_avaliados[n++] = avaliar((ASTNode*)args_node->lista.nome, args, num_args);
                args_node = args_node->lista.proximo;
            }
            return builtIns[i].funcao(args_avaliados, n);
        }
    }

    // 2. Verifica funções do usuário
    for (int i = 0; i < numFuncoesUsuario; i++) {
        if (strcmp(nome, funcoesUsuario[i].nome) == 0) {
            int args_avaliados[16];
            int n = 0;
            while (args_node != NULL) {
                args_avaliados[n++] = avaliar((ASTNode*)args_node->lista.nome, args, num_args);
                args_node = args_node->lista.proximo;
            }
            return avaliar(funcoesUsuario[i].corpo, args_avaliados, n);
        }
    }

    fprintf(stderr, "Erro: função '%s' não definida\n", nome);
    return 0;
}


void registrarFuncao(ASTNode* node) {
    if (node->tipo != NO_FUNCAO) return;
    funcoesUsuario = realloc(funcoesUsuario, (numFuncoesUsuario + 1) * sizeof(FuncaoUsuario));
    funcoesUsuario[numFuncoesUsuario].nome = strdup(node->funcao.nome);
    funcoesUsuario[numFuncoesUsuario].params = node->funcao.params;
    funcoesUsuario[numFuncoesUsuario].corpo = node->funcao.corpo;
    numFuncoesUsuario++;
}