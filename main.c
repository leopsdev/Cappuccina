#include <stdio.h>
#include "ast.h"
#include "parser.tab.h"
#include "eval.h"

extern ASTNode* raiz_ast;

int main() {
    if (yyparse() == 0) {
        registrarFuncao(raiz_ast);  // <-- REGISTRA A FUNÇÃO
        int res = avaliarChamada("main", NULL, NULL, 0);
        printf("Resultado: %d\n", res);
    }
    return 0;
}
