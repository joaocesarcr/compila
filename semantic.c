#include "semantic.h"

#include "hash.h"
#include "y.tab.h"

int SemanticErrors = 0;

typedef enum {
    DECLARED_VAR,
    DECLARED_VECTOR,
    DECLARED_FUNC,
    DECLARED_FUNC_PARAMETER,
} SEMANTIC_NATURE;

typedef enum {
    INT,
    BOOL,
} SEMANTIC_TYPE;

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
    if (isDeclaration(node->type)) {
        if (node->children[1]->value->type != TK_IDENTIFIER) {
            printf("SEMANTIC ERROR: %s already declared\n",
                   node->children[1]->value->text);
            SemanticErrors++;
        }
        switch (node->type) {
            default:
                break;
            case NODE_FUNC_DECLARATION:
                node->children[1]->value->type = DECLARED_FUNC;
                node->children[1]->value->nature = DECLARED_FUNC;
            case NODE_VECTOR_DECLARATION_AND_ASIGN:
                node->children[1]->value->type = DECLARED_VECTOR;
            case NODE_VECTOR_DECLARATION:
                node->children[1]->value->type = DECLARED_VECTOR;
            case NODE_VAR_DECLARATION:
                node->children[1]->value->type = DECLARED_VAR;
            case NODE_PARAM:
                node->children[1]->value->type = DECLARED_FUNC_PARAMETER;
        }
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

    if (node->type == NODE_TOKEN_IDENTIFIER) {
        HASH_NODE *symbol = node->value;
        if (!symbol || symbol->type == TK_IDENTIFIER) {
            printf("SEMANTIC ERROR: %s undeclared\n", node->value->text);
            SemanticErrors++;
        }
    }
    for (int i = 0; i < MAX_CHILDREN; i++) {
        check_undeclared_variables(node->children[i]);
    }
}

void check_operand(ASTNode *node) {
    int i;

    if (node == NULL)
        return;
    switch (node->type) {
        default:
            break;
        case NODE_ADDITION:
        case NODE_SUBTRACTION:
        case NODE_DIVISION:
        case NODE_MULTIPLICATION:
            if (! 
                    (node->children[0]->type == NODE_MULTIPLICATION) ||
                    (node->children[0]->type == NODE_DIVISION) ||
                    (node->children[0]->type == NODE_ADDITION) ||
                    (node->children[0]->type == NODE_SUBTRACTION) ||
                    (node->children[0]->type == NODE_FUNC_CALL) ||
    }
    node->children[1]->value->type = DECLARED_FUNC;
    node->children[1]->value->nature = DECLARED_FUNC;
    case NODE_VECTOR_DECLARATION_AND_ASIGN:
        node->children[1]->value->type = DECLARED_VECTOR;
    case NODE_VECTOR_DECLARATION:
        node->children[1]->value->type = DECLARED_VECTOR;
    case NODE_VAR_DECLARATION:
        node->children[1]->value->type = DECLARED_VAR;
    case NODE_PARAM:
        node->children[1]->value->type = DECLARED_FUNC_PARAMETER;
}
}
void check_variable_usage(ASTNode *node) {
    if (node == NULL)
        return;
    switch (node->type) {
        case NODE_ASSIGNMENT:
            // Check left side
            if (node->children[1]->type == NODE_TOKEN_IDENTIFIER) {
                HASH_NODE *symbol = hashFind(node->children[0]->value->text);
                if (symbol->nature != NODE_VAR_DECLARATION) {
                    printf("SEMANTIC ERROR: %s is not a scalar\n",
                           node->children[0]->value->text);
                    SemanticErrors++;
                }
            }
            // Check right side
            check_expression(node->children[1]);
            break;
        case NODE_VECTOR_INT:
        case NODE_VECTOR_TK:
            // Check vector usage
            if (node->children[0]->type == NODE_TOKEN_IDENTIFIER) {
                HASH_NODE *symbol = hashFind(node->children[0]->value->text);
                if (symbol->nature != NODE_VECTOR_DECLARATION) {
                    printf("SEMANTIC ERROR: %s is not a vector\n",
                           node->children[0]->value->text);
                    SemanticErrors++;
                }
            }
            check_expression(node->children[1]); // Check index
            break;
        case NODE_FUNC_CALL:
            // Check function usage
            if (node->children[0]->type == NODE_TOKEN_IDENTIFIER) {
                HASH_NODE *symbol = hashFind(node->children[0]->value->text);
                if (symbol->nature != NODE_FUNC_DECLARATION) {
                    printf("SEMANTIC ERROR: %s is not a function\n",
                           node->children[0]->value->text);
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
