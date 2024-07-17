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


void printNode(ASTNode* node, int level) {
    if (!node) return;
    if (!node->type) return;

    //printf("%s\n ", NodeTypeNames[node->type]);
    //printIndentation(level);

    switch (node->type) { 
        case NODE_EMPTY:        break;
        case NODE_BLOCK:        break;
        case NODE_KW_CHAR:          printf("char "); break;
        case NODE_KW_READ:          printf(""); break;
        case NODE_KW_INT:           printf("int "); break;
        case NODE_KW_FLOAT:         printf("float "); break;
        case NODE_KW_BOOL:          printf("bool "); break;
        case NODE_LITERAL_INT:      printf("%s",node->value->text); break;
        case NODE_LITERAL_STRING:      printf("%s",node->value->text); break;
        case NODE_TOKEN_IDENTIFIER: printf("%s",node->value->text); break;
        case NODE_LITERAL_CHAR:     printf("%s",node->value->text); break;
        case NODE_LITERAL_REAL:     printf("%s",node->value->text); break;
        case NODE_LITERAL_FALSE:    printf("false");break;
        case NODE_LITERAL_TRUE:     printf("true"); break;
        default: ;
                 break;
    }
}

void printAST(ASTNode* root) {
  printTree(root,0);
}

void printTree(ASTNode* root, int level) {
    if (!root) {
      //printf("No root found");
      return;
    }

    int toAdd= 0 ;
    if (!isListType(root->type)) {
        //printIndentation(level);
    }

    printNode(root,level);

    switch (root->type) {
        case NODE_EMPTY:
            break;

        case NODE_COMMANDS_LIST:
            printTree(root->children[0],level);
            printTree(root->children[1],level);
            break;

        case NODE_PARAM_LIST: // FUNC DECLARATION ARGS
        case NODE_ARGS_LIST: // FUNC CALLRGS
            if (root->children[0]->type == NODE_EMPTY) {
               break;
            } else {
                printTree(root->children[0],level);
                if(root->children[1] != NULL) { 
                    printf(",");
                    printTree(root->children[1],level);
                }
            }
            break;

         case NODE_FUNC_CALL_EMPTY:
            printf("()");
           break;

        case NODE_FUNC_CALL:
            printTree(root->children[0],level);
            printf("(");
            printTree(root->children[1],level);
            printf(")");
            break;

        case NODE_IF_CONTROL:
            printf("if ");
            printf("(");
            printTree(root->children[0],level);
            printf(")\n");
            printTree(root->children[1],level);
            if(root->children[2] != NULL) {
                printf("else ");
                printTree(root->children[2],level);
            }
            break;

        case NODE_KW_PRINT_STRING:
            printf("print ");
            printf("");
            printTree(root->children[0],level);
            printf(";\n");
            printIndentation(level);
            break;

        case NODE_KW_PRINT:
            printf("print ");
            printTree(root->children[0],level);
            printf(" ");
            printTree(root->children[1],level);
            printf(";\n");
            printIndentation(level);

            break;
        case NODE_BLOCK_EMPTY:
            printf("{}");
            break;
        case NODE_BLOCK:
            printIndentation(level);
            printf("{\n");
            level++;
            printIndentation(level);
            printTree(root->children[0],level);
            printf("\n");
            level--;
            printIndentation(level);
            printf("}\n");
            printIndentation(level);
            break;
        case NODE_FUNC_DECLARATION:
            printTree(root->children[0],level);
            printf(" ");
            printTree(root->children[1],level);
            printf("(");
            printTree(root->children[2],level);
            printf(")\n");
            printTree(root->children[3],level);
            break;
        case NODE_VAR_DECLARATION:
            printTree(root->children[0],level);
            printf(" ");
            printTree(root->children[1],level);
            printf(" :  ");
            printTree(root->children[2],level);
            printf(";\n");
            printIndentation(level);
            break;
        case NODE_VECTOR_INT:
        case NODE_VECTOR_TK:
            printTree(root->children[0],level);
            printf("[");
            printTree(root->children[1],level);
            printf("]");
            break;

        case NODE_VECTOR_DECLARATION:
            printTree(root->children[0],level);
            printf(" ");
            printTree(root->children[1],level);
            printf("[");
            printTree(root->children[2],level);
            printf("]");
            printf(";\n");
            printIndentation(level);
            break;

        case NODE_VECTOR_DECLARATION_AND_ASIGN:
            printTree(root->children[0],level);
            printf(" ");
            printTree(root->children[1],level);
            printf("[");
            printTree(root->children[2],level);
            printf("]");
            printf(" : ");
            printTree(root->children[3],level);
            printf(";\n");
            printIndentation(level);
            break;

        case NODE_KW_RETURN:
            printf("return ");
            printTree(root->children[0],level);
            printf(";\n");
            printIndentation(level);
            break;
        case NODE_KW_WHILE:
            printf("while (");
            printTree(root->children[0],level);
            printf(")\n");
            printTree(root->children[1],level);
            break;
        case NODE_KW_READ:
            printf("read ");
            printTree(root->children[0],level);
            printf(" ");
            printTree(root->children[1],level);
            printf(";\n");
            printIndentation(level);
            break;

        case NODE_PARAM:
            printTree(root->children[0],level);
            printf(" ");
            printTree(root->children[1],level);
            break;

        case NODE_ASSIGNMENT:
            printTree(root->children[0],level);
            printf(" = ");
            printTree(root->children[1],level);
            printf(";\n");
            printIndentation(level);
            break;

        case NODE_VALUES_LIST:
              printTree(root->children[0],level);
              printf(" ");
              printTree(root->children[1],level);
              break;

        case NODE_ADDITION:          
        case NODE_SUBTRACTION:       
        case NODE_MULTIPLICATION:    
        case NODE_DIVISION:          
        case NODE_LESS_THAN:         
        case NODE_GREATER_THAN:      
        case NODE_LOGICAL_OR:        
        case NODE_LOGICAL_AND:       
        case NODE_LESS_THAN_EQUAL:   
        case NODE_GREATER_THAN_EQUAL:
        case NODE_EQUAL:             
        case NODE_NOT_EQUAL:         
            printOperation(root,level);
            break;

        default: 
            for (int i = 0; i < MAX_CHILDREN; i++) {
                if (root->children[i]) {
                    printTree(root->children[i], level + toAdd);
                } else break;
          }

    }

}

void printOperation(ASTNode* node,int level) {
    printTree(node->children[0],level);
    switch (node->type) {
      case NODE_ADDITION:           printf(" + "); break;
      case NODE_SUBTRACTION:        printf(" - "); break;
      case NODE_MULTIPLICATION:     printf(" * "); break;
      case NODE_DIVISION:           printf(" / "); break;
      case NODE_LESS_THAN:          printf(" < "); break;
      case NODE_GREATER_THAN:       printf(" > "); break;
      case NODE_LOGICAL_OR:         printf(" || "); break;
      case NODE_LOGICAL_AND:        printf(" && "); break;
      case NODE_LESS_THAN_EQUAL:    printf(" <= "); break;
      case NODE_GREATER_THAN_EQUAL: printf(" >= "); break;
      case NODE_EQUAL:              printf(" == "); break;
      case NODE_NOT_EQUAL:          printf(" != "); break;
      default: printf("ERRO PRINT OPERATION! CHamando algo errado");
    }
    printTree(node->children[1],level);
    printf("");
}

int isOP(NodeType type) {
    int s = 27;
    NodeType lists[] = {
        NODE_ASSIGNMENT,
        NODE_PARAM,
        NODE_KW_IF,
        NODE_KW_IF_ELSE,
        NODE_KW_WHILE,
        NODE_VECTOR_INT,
        NODE_VECTOR_TK,
        NODE_VAR_DECLARATION,
        NODE_KW_PRINT,
        NODE_KW_PRINT_STRING,
        NODE_VECTOR_DECLARATION,
        NODE_VECTOR_DECLARATION_AND_ASIGN,
        NODE_KW_RETURN,
        NODE_FUNC_DECLARATION,
        NODE_ADDITION,
        NODE_SUBTRACTION,
        NODE_MULTIPLICATION,
        NODE_DIVISION,
        NODE_LESS_THAN,
        NODE_GREATER_THAN,
        NODE_LOGICAL_OR,
        NODE_LOGICAL_AND,
        NODE_LESS_THAN_EQUAL,
        NODE_GREATER_THAN_EQUAL,
        NODE_EQUAL,
        NODE_NOT_EQUAL,
    };
    for (int i = 0; i < s; i++) {
      if (lists[i] == type) return 1;
    }
    return 0;
}

int isKW(NodeType type) {
    NodeType lists[] = {
        NODE_KW_CHAR,
        NODE_KW_READ,
        NODE_KW_INT,
        NODE_KW_FLOAT,
        NODE_KW_BOOL,
    };
    int s = 5;
    for (int i = 0; i < s; i++) if (lists[i] == type) return 1;
    return 0;
}

int isListType(NodeType type) {
    NodeType lists[] = {
        NODE_DECLARATIONS_LIST,
        NODE_EMPTY,
        NODE_PARAM_LIST,
        NODE_VALUES_LIST,
        NODE_COMMANDS_LIST,
        NODE_ARGS_LIST,
    };
    int s = 9;
    for (int i = 0; i < s; i++) if (lists[i] == type) return 1;
    return 0;
}

int isLitType(NodeType type) {
    NodeType lists[] = {
        NODE_EMPTY,
        NODE_TOKEN_IDENTIFIER,
        NODE_LITERAL_INT,
        NODE_LITERAL_CHAR,
        NODE_LITERAL_REAL,
        NODE_LITERAL_STRING,
        NODE_LITERAL_FALSE,
        NODE_LITERAL_TRUE,

    };
    int s = 9;
    for (int i = 0; i < s; i++) if (lists[i] == type) return 1;
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

void printNodeOLD(ASTNode* node) {
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


void printTreeOLD(ASTNode* root, int level) {
    if (!root) {
      printf("No root found");
      return;
    }
    printIndentation(level);
    printNodeOLD(root);

    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (root->children[i]) {
            printTreeOLD(root->children[i], level + 1);
        }
    }
}
