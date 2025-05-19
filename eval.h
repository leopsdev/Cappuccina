#ifndef EVAL_H
#define EVAL_H

#include "ast.h"

// Tipos de primitivas
typedef int (*PrimitivaFunc)(int* args, int num_args);

// Estrutura para funções built-in
typedef struct {
    const char* nome;
    PrimitivaFunc funcao;
} FuncaoBuiltIn;

// Funções de avaliação
int avaliar(ASTNode* node, int* args, int num_args);
int avaliarChamada(const char* nome, ASTNode* args_node, int* args, int num_args);

// Registro de funções do usuário
void registrarFuncao(ASTNode* node);


// Primitivas built-in
int prim_zero(int* args, int num_args);
int prim_succ(int* args, int num_args);
int prim_proj(int* args, int num_args, int indice);
int prim_recproj(int* args, int num_args, int indice);
int prim_iszero(int* args, int num_args);

// Função para inicializar as built-ins
void inicializarBuiltIns();


#endif