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
    if (children) {
        for (int i = 0; i < MAX_CHILDREN; ++i) {
            if (children[i] == NULL) {
                // Set all subsequent children to NULL
                while (i < MAX_CHILDREN) {
                    node->children[i] = NULL;
                    i++;
                }
                break;
            }
            node->children[i] = children[i];
        }
    }
    node->value = value;
    return node;
}


void printNode(ASTNode* node) {
    if (!node) return;

    switch (node->type) {
        case NODE_LITERAL_INT:
            printf("LITERAL_INT: %s\n", node->value ? node->value->text : "NULL");
            break;
        case NODE_LITERAL_CHAR:
            printf("LITERAL_CHAR: %s\n", node->value ? node->value->text : "NULL");
            break;
        case NODE_LITERAL_REAL:
            printf("LITERAL_REAL: %s\n", node->value ? node->value->text : "NULL");
            break;
        case NODE_LITERAL_STRING:
            printf("LITERAL_STRING: %s\n", node->value ? node->value->text : "NULL");
            break;
        // Add cases for other node types if necessary
        default:
            printf("NodeType: %s\n", NodeTypeNames[node->type]);
            break;
    }
}

void printAST(ASTNode* root) {
  printTree(root,0);
}

void printTree(ASTNode* root, int level) {
    if (!root) {
      printf("No root found");
      return;
    }
    printIndentation(level);
    printNode(root);

    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (root->children[i]) {
            printTree(root->children[i], level + 1);
        }
    }
}

void printIndentation(int level) {
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
}


