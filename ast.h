#ifndef AST_H
#define AST_H

typedef enum {
    NO_NUMERO,
    NO_FUNCAO,
    NO_CHAMADA,
    NO_PROJECAO,
    NO_RECPROJECAO,
    NO_RECURSAO,
    NO_MINIMIZACAO,
    NO_PRIMITIVA,
    NO_PARAM,
    NO_ARG
} TipoNo;

typedef struct ASTNode {
    TipoNo tipo;
    union {
        // Para números
        int numero;
        
        // Para funções (def nome(params) = corpo)
        struct {
            char* nome;
            struct ASTNode* params;
            struct ASTNode* corpo;
        } funcao;
        
        // Para chamadas (nome(args))
        struct {
            char* nome;
            struct ASTNode* args;
        } chamada;
        
        // Para projeções (proj(i))
        struct {
            int indice;
        } projecao;
        
        // Para recursão (rec(base, passo))
        struct {
            struct ASTNode* base;
            struct ASTNode* passo;
        } recursao;
        
        // Para minimização (mu(pred) ou mu_lim(lim, pred))
        struct {
            char* predicado;
            struct ASTNode* limite;  // NULL se for mu ilimitado
        } minimizacao;

        struct {
            char* nome;               // "zero", "succ" ou "iszero"
            struct ASTNode* argumento; // NULL para zero()
        } primitiva;
        
        // Para listas (params ou args)
        struct {
            char* nome;
            struct ASTNode* proximo;
        } lista;
    };
} ASTNode;

// Funções de criação de nós
ASTNode* criarNoNumero(int num);
ASTNode* criarNoFuncao(char* nome, ASTNode* params, ASTNode* corpo);
ASTNode* criarNoChamada(char* nome, ASTNode* args);
ASTNode* criarNoProjecao(int indice);
ASTNode* criarNoRecProjecao(int indice);
ASTNode* criarNoRecursao(ASTNode* base, ASTNode* passo);
ASTNode* criarNoMinimizacao(char* predicado, ASTNode* limite);
ASTNode* criarNoSoma(ASTNode* esq, ASTNode* dir);
ASTNode* criarNoParam(char* nome);
ASTNode* adicionarParam(ASTNode* lista, char* nome);
ASTNode* criarNoArg(ASTNode* expr);
ASTNode* adicionarArg(ASTNode* lista, ASTNode* expr);
ASTNode* criarNoPrimitiva(const char* nome, ASTNode* argumento);


#endif