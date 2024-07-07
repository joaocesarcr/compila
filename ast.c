#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "hash.h"

ASTNode* createNode(NodeType type, ASTNode* children[MAX_CHILDREN], HASH_NODE* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->type = type;
    for (int i = 0; i < MAX_CHILDREN; ++i) {
        node->children[i] = children[i];
    }
    node->value = *value;
    return node;
}
