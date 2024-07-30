#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include "ast.h"
#include "hash.h"

extern int SemanticErrors;

void check_expression(ASTNode *node);
void check_arguments(ASTNode *node);
void check_undeclared_variables(ASTNode *node);
void check_operand(ASTNode *node);
void check_usage(ASTNode *node);
void check_and_set_declarations(ASTNode *node);
void check_undeclared();
int setParamQtt(ASTNode *node, int qtt);
int checkListType(ASTNode *node, NodeType type, int index,
                  HASH_NODE *declaration);
NodeType getType(ASTNode *node);
void checkFunctions(ASTNode *node, ASTNode *root);
int checkFunctionCall(ASTNode *node, ASTNode *root);
int isCompatible(NodeType fType, NodeType sType);
ASTNode *findFunctionDeclaration(ASTNode *node, HASH_NODE *tokenHashNode);
void setExpressionValue(ASTNode *node, ASTNode *parent);
void printIdentifiersTypeNature(ASTNode *node, ASTNode *otherNode);
void printOpError(ASTNode *node, char op);

#endif /*                                                                          \
• variáveis redeclaradas X                                                      \
• anotar tipo (natureza) nas tabela hash X                                       \
• anotar tipo de dado (dataType) na tabela hash X                                \
• variáveis não-declaradas X                                                   \
                                                                                 \ \
• verificar natureza, se:                                                        \
    - escalares são usados como escalares                                         \
    - vetores são usados como vetores                                             \
    - funções são usadas como funções                                         \
    - não esqueça de verificar no lado direito (expressões) e no lado           \
esquerdo (atribuições)                                                           \
                                                                                 \ \
• verificar tipos de dados nas expressões                                       \
                                                                                 \ \
• verificar argumentos de chamada de função versus parâmetros:                \
    - não pode haver menos argumentos                                             \
    - não pode haver mais argumentos                                              \
    - os tipos devem ser compatíveis (não iguais, lembre-se)                     \
                                                                                 \ \
• verificar o tamanho e a lista de inicialização do vetor;                     \
• verificar tipo do valor de retorno da função                                 \
• verificar índices dos vetores (não pode ser booleano, não pode ser real),   \
tanto na expressão quanto na atribuição                                         \
 */
