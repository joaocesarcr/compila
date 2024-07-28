#include "semantic.h"

#include "hash.h"
#include "y.tab.h"

/*
Definições Semânticas:
• Há três tipos diferentes de identificadores:
escalares, vetores, funções
• Há três tipos de dados incompatíveis entre si:
reais, inteiros e booleanos
• Ha cinco tipos de literais: reais, inteiros, , true/false, caracteres, strings
// Literais reais somente podem ser usados em expressões, variáveis, parâmetros
// ou outros processamentos declarados com esse tipo (real)
 */
int SemanticErrors = 0;

typedef enum {
    DECLARED_VAR,
    DECLARED_VECTOR,
    DECLARED_FUNC,
    DECLARED_FUNC_PARAMETER,
} SEMANTIC_NATURE;

// Ha quatro tipos de dados para declarações: char, int, float, bool
typedef enum { INT, BOOL, CHAR, FLOAT } SEMANTIC_TYPE;

void check_semantic(ASTNode *root) {
    check_and_set_declarations(root);
    check_undeclared_variables(root);
    check_operand(root);
    printf("Check usage: \n");
    check_usage(root);
}

int isDeclaration(NodeType type) {
    NodeType lists[] = {
        NODE_FUNC_DECLARATION,
        NODE_VECTOR_DECLARATION,
        NODE_VECTOR_DECLARATION_AND_ASIGN,
        NODE_VAR_DECLARATION,
        NODE_PARAM,
    };
    int s = 5;
    for (int i = 0; i < s; i++)
        if (lists[i] == type)
            return 1;
    return 0;
}

void check_and_set_declarations(ASTNode *node) {
    int i = 0;
    if (node == NULL)
        return;
    if (isDeclaration(node->astNodeType)) {
        if (node->children[1]->hashNode->hashNodeType != TK_IDENTIFIER) {
            printf("SEMANTIC ERROR: %s already declared\n",
                   node->children[1]->hashNode->text);
            SemanticErrors++;
        }
        switch (node->astNodeType) {
            default:
                printf("Error: default option used in "
                       "check_and_set_declarations\n");
                break;
            case NODE_FUNC_DECLARATION:
                node->children[1]->hashNode->hashNodeType = DECLARED_FUNC;
                break;
            case NODE_VECTOR_DECLARATION_AND_ASIGN:
                node->children[1]->hashNode->hashNodeType = DECLARED_VECTOR;
                break;
            case NODE_VECTOR_DECLARATION:
                node->children[1]->hashNode->hashNodeType = DECLARED_VECTOR;
                break;
            case NODE_VAR_DECLARATION:
                node->children[1]->hashNode->hashNodeType = DECLARED_VAR;
                break;
            case NODE_PARAM:
                node->children[1]->hashNode->hashNodeType =
                    DECLARED_FUNC_PARAMETER;
                break;
        }

        node->children[1]->hashNode->nature = node->children[0]->astNodeType;
        if (0)
            printf("Setting nature for %s as %s\n",
                   node->children[1]->hashNode->text,
                   NodeTypeNames[node->children[0]->astNodeType]);
    }
    for (i = 0; i < MAX_CHILDREN; i++) {
        if (node->children[i]) {
            check_and_set_declarations(node->children[i]);
        } else
            break;
    }
}

void check_undeclared_variables(ASTNode *node) {
    if (node == NULL)
        return;

    if (node->astNodeType == NODE_TOKEN_IDENTIFIER) {
        HASH_NODE *symbol = node->hashNode;
        if (!symbol || symbol->hashNodeType == TK_IDENTIFIER) {
            printf("SEMANTIC ERROR: %s undeclared\n", node->hashNode->text);
            SemanticErrors++;
        }
    }
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (node->children[i])
            check_undeclared_variables(node->children[i]);
        else
            break;
    }
}

int isNumber(ASTNode *node) {
    if ((node->astNodeType == NODE_MULTIPLICATION) ||
        (node->astNodeType == NODE_DIVISION) ||
        (node->astNodeType == NODE_ADDITION) ||
        (node->astNodeType == NODE_KW_INT) ||
        (node->astNodeType == NODE_LITERAL_INT) ||
        ((node->astNodeType == NODE_TOKEN_IDENTIFIER) &&
         (node->hashNode->nature == NODE_KW_INT)) ||
        (node->astNodeType == NODE_SUBTRACTION) ||
        ((node->astNodeType == NODE_FUNC_CALL) &&
         (node->children[0]->hashNode->nature == NODE_KW_INT)) ||

        // char e int sao intercambiaveis
        ((node->astNodeType == NODE_TOKEN_IDENTIFIER) &&
         (node->hashNode->nature == NODE_KW_CHAR)) ||
        (node->astNodeType == NODE_KW_CHAR) ||
        (node->astNodeType == NODE_LITERAL_CHAR))
        return 1;
    else
        return 0;
}

void check_usage(ASTNode *node) {
    int i;

    if (node == NULL) {
        printf("Null node\n");
        return;
    }
    // printf("Node: %s\n", NodeTypeNames[node->astNodeType]);

    switch (node->astNodeType) {
        default:
            break;
        case NODE_VECTOR_INT:
            if ((node->children[0]->hashNode->hashNodeType !=
                 DECLARED_VECTOR)) {
                SemanticErrors++;
                printf("SEMANTIC ERROR: incorrect vector usage. Indexes should "
                       "only "
                       "be used on vectors\n");
                printIdentifiersTypeNature(node->children[0],
                                           node->children[1]);
            }
            break;
        case NODE_VECTOR_TK:
            if ((node->children[0]->hashNode->hashNodeType !=
                 DECLARED_VECTOR)) {
                SemanticErrors++;
                printf("SEMANTIC ERROR: incorrect vector usage. Index should "
                       "be used on vectors\n");
                printIdentifiersTypeNature(node->children[0],
                                           node->children[1]);
            }

            if (!(isNumber(node->children[1]))) {
                SemanticErrors++;
                printf("SEMANTIC ERROR: incorrect vector usage. Index must be "
                       "int compatible\n");
                printIdentifiersTypeNature(node->children[0],
                                           node->children[1]);
            }
            break;
    }
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (node->children[i])
            check_usage(node->children[i]);
        else
            break;
    }
}
void check_operand(ASTNode *node) {
    int i;

    if (node == NULL)
        return;
    switch (node->astNodeType) {
        default:
            break;
        case NODE_ADDITION:
        case NODE_SUBTRACTION:
        case NODE_DIVISION:
        case NODE_MULTIPLICATION:
            if (!(isNumber(node->children[0]) && isNumber(node->children[1]))) {
                printf("SEMANTIC ERROR: operation error. Invalid operands\n");
                printIdentifiersTypeNature(node->children[0],
                                           node->children[1]);
                SemanticErrors++;
            }
            break;
    }
    for (int i = 0; i < MAX_CHILDREN; i++) {
        check_operand(node->children[i]);
    }
}
void printIdentifiersTypeNature(ASTNode *node, ASTNode *otherNode) {
    printf("    Identifiers: %s , %s \n", node->hashNode->text,
           otherNode->hashNode->text);
    printf("    astNode type: %s , %s \n", NodeTypeNames[node->astNodeType],
           NodeTypeNames[otherNode->astNodeType]);

    printf("    Natures: %s , %s\n", NodeTypeNames[node->hashNode->nature],
           NodeTypeNames[otherNode->hashNode->nature]);
}
void check_variable_usage(ASTNode *node) {
    if (node == NULL)
        return;
    switch (node->astNodeType) {
        case NODE_ASSIGNMENT:
            // Check left side
            if (node->children[1]->astNodeType == NODE_TOKEN_IDENTIFIER) {
                HASH_NODE *symbol = hashFind(node->children[0]->hashNode->text);
                if (symbol->nature != NODE_VAR_DECLARATION) {
                    printf("SEMANTIC ERROR: %s is not a scalar\n",
                           node->children[0]->hashNode->text);
                    SemanticErrors++;
                }
            }
            // Check right side
            check_expression(node->children[1]);
            break;
        case NODE_VECTOR_INT:
        case NODE_VECTOR_TK:
            // Check vector usage
            if (node->children[0]->astNodeType == NODE_TOKEN_IDENTIFIER) {
                HASH_NODE *symbol = hashFind(node->children[0]->hashNode->text);
                if (symbol->nature != NODE_VECTOR_DECLARATION) {
                    printf("SEMANTIC ERROR: %s is not a vector\n",
                           node->children[0]->hashNode->text);
                    SemanticErrors++;
                }
            }
            check_expression(node->children[1]); // Check index
            break;
        case NODE_FUNC_CALL:
            // Check function usage
            if (node->children[0]->astNodeType == NODE_TOKEN_IDENTIFIER) {
                HASH_NODE *symbol = hashFind(node->children[0]->hashNode->text);
                if (symbol->nature != NODE_FUNC_DECLARATION) {
                    printf("SEMANTIC ERROR: %s is not a function\n",
                           node->children[0]->hashNode->text);
                    SemanticErrors++;
                }
            }
            check_arguments(node->children[1]); // Check arguments
            break;
        default:
            break;
    }
    for (int i = 0; i < MAX_CHILDREN; i++) {
        check_variable_usage(node->children[i]);
    }
}

void check_expression(ASTNode *node) {
    if (node == NULL)
        return;
    // Verifique o tipo de dados nas expressões aqui
    // ...
}

void check_arguments(ASTNode *node) {
    if (node == NULL)
        return;
    // Verifique os argumentos das chamadas de função aqui
    // ...
}

void semantic_check(ASTNode *root) {
    check_and_set_declarations(root);
    check_undeclared_variables(root);
    check_variable_usage(root);
    // Adicione mais verificações conforme necessário
}

void check_undeclared() {}
