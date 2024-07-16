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
    if (!node->type) return;
    printf("%s\n", NodeTypeNames[node->type]);
    //if (node->type == NODE_TOKEN_IDENTIFIER) printf("%d",node->value->type);
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
        } else break;
    }
}

void printIndentation(int level) {
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
}


ASTNode** astNullChild() {
    static ASTNode* nullArray[5] = {NULL, NULL, NULL, NULL, NULL};
    return nullArray;
}
