#include "ast.h"
#include <stdlib.h>
#include <string.h>

ASTNode* criarNoNumero(int num) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->tipo = NO_NUMERO;
    node->numero = num;
    return node;
}

ASTNode* criarNoPrimitiva(const char* nome, ASTNode* argumento) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->tipo = NO_PRIMITIVA;
    node->primitiva.nome = strdup(nome);
    node->primitiva.argumento = argumento;
    return node;
}

ASTNode* criarNoFuncao(char* nome, ASTNode* params, ASTNode* corpo) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->tipo = NO_FUNCAO;
    node->funcao.nome = strdup(nome);
    node->funcao.params = params;
    node->funcao.corpo = corpo;
    return node;
}

ASTNode* criarNoChamada(char* nome, ASTNode* args) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->tipo = NO_CHAMADA;
    node->chamada.nome = strdup(nome);
    node->chamada.args = args;
    return node;
}

ASTNode* criarNoProjecao(int indice) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->tipo = NO_PROJECAO;
    node->projecao.indice = indice;
    return node;
}

ASTNode* criarNoRecProjecao(int indice) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->tipo = NO_RECPROJECAO;
    node->projecao.indice = indice;
    return node;
}

ASTNode* criarNoRecursao(ASTNode* base, ASTNode* passo) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->tipo = NO_RECURSAO;
    node->recursao.base = base;
    node->recursao.passo = passo;
    return node;
}

ASTNode* criarNoMinimizacaoIlimitada(char* nomeFuncao, ASTNode* args) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->tipo = NO_MU;
    node->mu.nomeFuncao = strdup(nomeFuncao);
    node->mu.args = args;  // args é lista de NO_ARG*
    return node;
}

ASTNode* criarNoMinimizacaoLimitada(char* nomeFuncao, ASTNode* args, ASTNode* limite) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->tipo = NO_MULIM;
    node->mu_lim.nomeFuncao = strdup(nomeFuncao);
    node->mu_lim.args = args;
    node->mu_lim.limite = limite;
    return node;
}

ASTNode* criarNoParam(char* nome) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->tipo = NO_PARAM;
    node->lista.nome = strdup(nome);
    node->lista.proximo = NULL;
    return node;
}

ASTNode* adicionarParam(ASTNode* lista, char* nome) {
    ASTNode* novo = criarNoParam(nome);
    novo->lista.proximo = lista;
    return novo;
}

ASTNode* criarNoArg(ASTNode* expr) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->tipo = NO_ARG;
    node->lista.proximo = NULL;
    // Reutiliza o campo 'nome' para armazenar o nó de expressão
    node->lista.nome = (char*)expr;
    return node;
}

ASTNode* adicionarArg(ASTNode* lista, ASTNode* expr) {
    ASTNode* novo = criarNoArg(expr);
    novo->lista.proximo = lista;
    return novo;
}