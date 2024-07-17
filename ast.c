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

    if (!isListType(node->type)) { 
        printf("%s", NodeTypeNames[node->type]);
        switch (node->type) { 
            case NODE_TOKEN_IDENTIFIER: printf(": %s",node->value->text); break;
            case NODE_LITERAL_INT:      printf(": %s",node->value->text); break;
            case NODE_LITERAL_CHAR:     printf(": %s",node->value->text); break;
            case NODE_LITERAL_REAL:     printf(": %s",node->value->text); break;
            case NODE_LITERAL_STRING:   printf(": %s",node->value->text); break;
            case NODE_LITERAL_FALSE:    printf(": false");break;
            case NODE_LITERAL_TRUE:     printf(": true"); break;
            case NODE_DECLARATIONS_LIST:     printf(": true"); break;
            default: ;
        }
        printf("\n");
    }
}

void printProgram(ASTNode* node) {
    switch (node->type) { 
        case NODE_TOKEN_IDENTIFIER: printf(": %s",node->value->text); break;
        case NODE_LITERAL_INT:      printf(": %s",node->value->text); break;
        case NODE_LITERAL_CHAR:     printf(": %s",node->value->text); break;
        case NODE_LITERAL_REAL:     printf(": %s",node->value->text); break;
        case NODE_LITERAL_STRING:   printf(": %s",node->value->text); break;
        case NODE_LITERAL_FALSE:    printf(": false");break;
        case NODE_LITERAL_TRUE:     printf(": true"); break;
        case NODE_DECLARATIONS_LIST:     printf(": true"); break;
        default: ;
    }
    printf("\n");


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

    int toAdd= 1;
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (root->children[i]) {
            if (isListType(root->children[i]->type)) toAdd =0;
            printTree(root->children[i], level + toAdd);
            toAdd = 1;
        } else break;
    }
}

int isListType(NodeType type) {
  NodeType lists[] = {NODE_DECLARATIONS_LIST,
                   NODE_PARAM_LIST,
                   NODE_VALUES_LIST,
                   NODE_COMMANDS_LIST,
                   NODE_ARGS_LIST,
  };
  int s = 5;
  for (int i = 0; i < s; i++) {
      if (lists[i] == type) return 1;
  }
  return 0;
  
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
