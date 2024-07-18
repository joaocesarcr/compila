#include "semantic.h"

#include "hash.h"
#include "y.tab.h"

int SemanticErrors = 0;

typedef enum {
    DECLARED_VAR,
} NodeTypes;
void check_and_set_declarations(ASTNode *node) {
    int i = 0;
    if (node == NULL)
        return;
    switch (node->type) {
        default:
            break;
        case NODE_FUNC_DECLARATION:
        case NODE_VECTOR_DECLARATION_AND_ASIGN:
        case NODE_VECTOR_DECLARATION:
        case NODE_VAR_DECLARATION:
            if (node->children[1]) {
                if (node->children[1]->value->type != TK_IDENTIFIER) {
                    printf("SEMANTIC ERROR: %s already declared\n",
                           node->children[1]->value->text);
                    SemanticErrors++;
                } else
                    node->children[1]->value->type = node->children[1]->type;
            }
            break;
    }
    for (i = 0; i < MAX_CHILDREN; i++) {
        if (node->children[i]) {
            check_and_set_declarations(node->children[i]);
        } else
            break;
    }
}
void check_undeclared() {}
