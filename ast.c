/* Compiladores 2024/01 - Etapa 3
 * João César de Paula Criscolo  - 00304342
 * Prof. Marcelo Johann
 */
#include "ast.h"
#include "hash.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode *createNode(NodeType type, ASTNode *children[MAX_CHILDREN],
                    HASH_NODE *hashNode) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->astNodeType = type;
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
    node->hashNode = hashNode;
    return node;
}

ASTNode **astNullChild() {
    static ASTNode *nullArray[5] = {NULL, NULL, NULL, NULL, NULL};
    return nullArray;
}

void printAST(ASTNode *root) { printTree(root, 0); }
void printNode(ASTNode *node, int level) {
    if (!node)
        return;
    if (!node->astNodeType)
        return;

    if (isExpression(node)) {
        printOp(node);
    } else {

        // printf("%s\n ", NodeTypeNames[node->astNodeType]);
        // printIndentation(level);

        switch (node->astNodeType) {
            case NODE_FUNC_CALL:
                printf("%s(", node->children[0]->hashNode->text);
                printNode(node->children[1], 0);
                printf(")");
                break;
            case NODE_FUNC_CALL_EMPTY:
                printf("%s()", node->children[0]->hashNode->text);
                break;
            case NODE_EMPTY:
            case NODE_BLOCK:
                break;
            case NODE_KW_CHAR:
                printf("char ");
                break;
            case NODE_KW_INT:
                printf("int ");
                break;
            case NODE_KW_FLOAT:
                printf("float ");
                break;
            case NODE_KW_IF:
                printf("if (");
                printNode(node->children[0], 0);
                printf(")");
                break;
            case NODE_KW_IF_ELSE:
                printf("if (");
                printNode(node->children[0], 0);
                printf(")");
                printf("else ");
                printNode(node->children[1], 0);
                printf("");

            case NODE_KW_BOOL:
                printf("bool ");
                break;
            case NODE_VECTOR_INT:
                printf("%s[%d]", node->children[0]->hashNode->text,
                       atoi(node->children[1]->hashNode->text));
                break;
            case NODE_VECTOR_TK:
                printf("%s[%s]", node->children[0]->hashNode->text,
                       node->children[1]->hashNode->text);
                break;
            case NODE_ASSIGNMENT:
                printTree(node->children[0], 0);
                printf(" = ");
                printTree(node->children[1], 0);
                break;

            case NODE_PARENTHESIS_EXPRESSION:
                printf("(");
                printNode(node->children[0], 0);
                printf(")");
                break;
            case NODE_LITERAL_INT:
            case NODE_LITERAL_STRING:
            case NODE_TOKEN_IDENTIFIER:
            case NODE_LITERAL_CHAR:
            case NODE_LITERAL_REAL:
                printf("%s", node->hashNode->text);
                break;
            case NODE_LITERAL_FALSE:
                printf("false");
                break;
            case NODE_LITERAL_TRUE:
                printf("true");
                break;
            case NODE_KW_PRINT_STRING:
                printf("print ");
                printf("");
                printTree(node->children[0], level);
                printf(";\n");
                printIndentation(level);
                break;

            case NODE_KW_PRINT:
                printf("print ");
                printTree(node->children[0], level);
                printf(" ");
                printTree(node->children[1], level);
                printf(";\n");
                printIndentation(level);
                break;
            case NODE_BLOCK_EMPTY:
                printf("{}");
                break;
            case NODE_FUNC_DECLARATION_EMPTY:
                printTree(node->children[0], level);
                printTree(node->children[1], level);
                printf("()\n");
                printTree(node->children[3], level);
                break;

            case NODE_FUNC_DECLARATION:
                printTree(node->children[0], level);
                printf(" ");
                printTree(node->children[1], level);
                printf("(");
                printTree(node->children[2], level);
                printf(")\n");
                printTree(node->children[3], level);
                break;
            case NODE_VAR_DECLARATION:
                printTree(node->children[0], level);
                printf(" ");
                printTree(node->children[1], level);
                printf(" :  ");
                printTree(node->children[2], level);
                printf(";\n");
                printIndentation(level);
                break;
            case NODE_VECTOR_DECLARATION:
                printTree(node->children[0], level);
                printf(" ");
                printTree(node->children[1], level);
                printf("[");
                printTree(node->children[2], level);
                printf("]");
                printf(";\n");
                printIndentation(level);
                break;

            case NODE_VECTOR_DECLARATION_AND_ASIGN:
                printTree(node->children[0], level);
                printf(" ");
                printTree(node->children[1], level);
                printf("[");
                printTree(node->children[2], level);
                printf("]");
                printf(" : ");
                printTree(node->children[3], level);
                printf(";\n");
                printIndentation(level);
                break;

            case NODE_KW_RETURN:
                printf("return ");
                printTree(node->children[0], level);
                printf(";\n");
                printIndentation(level);
                break;
            case NODE_KW_WHILE:
                printf("while (");
                printTree(node->children[0], level);
                printf(")\n");
                printTree(node->children[1], level);
                break;
            case NODE_KW_READ:
                printf("read ");
                printTree(node->children[0], level);
                printf(" ");
                printTree(node->children[1], level);
                printf(";\n");
                printIndentation(level);
                break;

            case NODE_PARAM:
                printTree(node->children[0], level);
                printf(" ");
                printTree(node->children[1], level);
                break;

            case NODE_VALUES_LIST:
                printTree(node->children[0], level);
                printf(" ");
                printTree(node->children[1], level);
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
                printOperation(node, level);
                break;

            default:
                break;
        }
    }
}

void printTree(ASTNode *root, int level) {
    if (!root) {
        // printf("No root found");
        return;
    }

    int toAdd = 0;

    //    printNode(root, level);

    switch (root->astNodeType) {
        case NODE_EMPTY:
            break;

        case NODE_COMMANDS_LIST:
            printTree(root->children[0], level);
            printTree(root->children[1], level);
            break;

        case NODE_PARAM_LIST: // FUNC DECLARATION ARGS
        case NODE_ARGS_LIST:  // FUNC CALLRGS
            printArgList(root);
            break;
            if (root->children[0]->astNodeType == NODE_EMPTY) {
                break;
            } else {
                printTree(root->children[0], level);
                if (root->children[1] != NULL) {
                    printf(",");
                    printTree(root->children[1], level);
                }
            }
            break;
        case NODE_BLOCK:
            printIndentation(level);
            printf("{\n");
            level++;
            printIndentation(level);
            printTree(root->children[0], level);
            printf("\n");
            level--;
            printIndentation(level);
            printf("}\n");
            printIndentation(level);
            break;

        case NODE_FUNC_CALL:
            printf("%s(", root->children[0]->hashNode->text);
            printArgList(root->children[1]);
            printf(")");
            break;
        case NODE_FUNC_CALL_EMPTY:
            printf("%s()", root->children[0]->hashNode->text);
            break;
        case NODE_KW_CHAR:
            printf("char ");
            break;
        case NODE_KW_INT:
            printf("int ");
            break;
        case NODE_KW_FLOAT:
            printf("float ");
            break;
        case NODE_KW_IF:
            printf("if (");
            printNode(root->children[0], level);
            printf(")");
            printTree(root->children[1], level);
            break;

        case NODE_KW_IF_ELSE:
            printf("if (");
            printNode(root->children[0], level);
            printf(")");
            printTree(root->children[1], level);

            printf("else ");
            printTree(root->children[2], level);
            break;

        case NODE_KW_BOOL:
            printf("bool ");
            break;
        case NODE_VECTOR_INT:
            printf("%s[%d]", root->children[0]->hashNode->text,
                   atoi(root->children[1]->hashNode->text));
            break;
        case NODE_VECTOR_TK:
            printf("%s[%s]", root->children[0]->hashNode->text,
                   root->children[1]->hashNode->text);
            break;
        case NODE_ASSIGNMENT:
            printIndentation(level);
            printTree(root->children[0], 0);
            printf(" = ");
            printTree(root->children[1], 0);
            printf(";\n");
            break;

        case NODE_PARENTHESIS_EXPRESSION:
            printf("(");
            printNode(root->children[0], 0);
            printf(")");
            break;
        case NODE_LITERAL_INT:
        case NODE_LITERAL_STRING:
        case NODE_TOKEN_IDENTIFIER:
        case NODE_LITERAL_CHAR:
        case NODE_LITERAL_REAL:
            printf("%s", root->hashNode->text);
            break;
        case NODE_LITERAL_FALSE:
            printf("false");
            break;
        case NODE_LITERAL_TRUE:
            printf("true");
            break;
        case NODE_KW_PRINT_STRING:
            printf("print ");
            printf("");
            printTree(root->children[0], level);
            printf(";\n");
            printIndentation(level);
            break;

        case NODE_KW_PRINT:
            printf("print ");
            printTree(root->children[0], level);
            printf(" ");
            printTree(root->children[1], level);
            printf(";\n");
            printIndentation(level);

            break;
        case NODE_BLOCK_EMPTY:
            printf("{}");
            break;
        case NODE_FUNC_DECLARATION_EMPTY:
            printTree(root->children[0], level);
            printf(" ");
            printTree(root->children[1], level);
            printf("(");
            printf(")\n");
            printTree(root->children[3], level);
            break;

        case NODE_FUNC_DECLARATION:
            printTree(root->children[0], level);
            printf(" ");
            printTree(root->children[1], level);
            printf("(");
            printTree(root->children[2], level);
            printf(")\n");
            printTree(root->children[3], level);
            break;
        case NODE_VAR_DECLARATION:
            printTree(root->children[0], level);
            printf(" ");
            printTree(root->children[1], level);
            printf(":");
            printTree(root->children[2], level);
            printf(";\n");
            printIndentation(level);
            break;
        case NODE_VECTOR_DECLARATION:
            printTree(root->children[0], level);
            printf(" ");
            printTree(root->children[1], level);
            printf("[");
            printTree(root->children[2], level);
            printf("]");
            printf(";\n");
            printIndentation(level);
            break;

        case NODE_VECTOR_DECLARATION_AND_ASIGN:
            printTree(root->children[0], level);
            printf(" ");
            printTree(root->children[1], level);
            printf("[");
            printTree(root->children[2], level);
            printf("]");
            printf(" : ");
            printTree(root->children[3], level);
            printf(";\n");
            printIndentation(level);
            break;

        case NODE_KW_RETURN:
            printf("return ");
            printTree(root->children[0], level);
            printf(";\n");
            printIndentation(level);
            break;
        case NODE_KW_WHILE:
            printf("while (");
            printTree(root->children[0], level);
            printf(")\n");
            printTree(root->children[1], level);
            break;
        case NODE_KW_READ:
            printf("read ");
            printTree(root->children[0], level);
            printf(" ");
            printTree(root->children[1], level);
            printf(";\n");
            printIndentation(level);
            break;

        case NODE_PARAM:
            printTree(root->children[0], level);
            printf(" ");
            printTree(root->children[1], level);
            break;

        case NODE_VALUES_LIST:
            printTree(root->children[0], level);
            printf(" ");
            printTree(root->children[1], level);
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
            printOperation(root, level);
            break;

        default:
            for (int i = 0; i < MAX_CHILDREN; i++) {
                if (root->children[i]) {
                    printTree(root->children[i], level + toAdd);
                } else
                    break;
            }
    }
}

void printOperation(ASTNode *node, int level) {
    printTree(node->children[0], level);
    switch (node->astNodeType) {
        case NODE_ADDITION:
            printf(" + ");
            break;
        case NODE_SUBTRACTION:
            printf(" - ");
            break;
        case NODE_MULTIPLICATION:
            printf(" * ");
            break;
        case NODE_DIVISION:
            printf(" / ");
            break;
        case NODE_LESS_THAN:
            printf(" < ");
            break;
        case NODE_GREATER_THAN:
            printf(" > ");
            break;
        case NODE_LOGICAL_OR:
            printf(" | ");
            break;
        case NODE_LOGICAL_AND:
            printf(" && ");
            break;
        case NODE_LESS_THAN_EQUAL:
            printf(" <= ");
            break;
        case NODE_GREATER_THAN_EQUAL:
            printf(" >= ");
            break;
        case NODE_EQUAL:
            printf(" == ");
            break;
        case NODE_NOT_EQUAL:
            printf(" != ");
            break;
        default:
            printf("ERRO PRINT OPERATION! CHamando algo errado");
    }
    printTree(node->children[1], level);
    printf("");
}

void printNodeOLD(ASTNode *node) {
    if (!node)
        return;

    printf("%p ", node);
    switch (node->astNodeType) {
        case NODE_FUNC_CALL:
            printf("%s(", node->children[0]->hashNode
                              ? node->children[0]->hashNode->text
                              : "NULL");

            if (node->children[1])
                printArgList(node->children[1]);
            printf(")\n");
            break;
        case NODE_FUNC_CALL_EMPTY:
            printf("%s()\n", node->children[0]->hashNode
                                 ? node->children[0]->hashNode->text
                                 : "NULL");
            break;
        case NODE_LITERAL_INT:
            printf("LITERAL_INT: %s\n",
                   node->hashNode ? node->hashNode->text : "NULL");
            break;
        case NODE_LITERAL_CHAR:
            printf("LITERAL_CHAR: %s\n",
                   node->hashNode ? node->hashNode->text : "NULL");
            break;
        case NODE_LITERAL_REAL:
            printf("LITERAL_REAL: %s\n",
                   node->hashNode ? node->hashNode->text : "NULL");
            break;
        case NODE_LITERAL_STRING:
            printf("LITERAL_STRING: %s\n",
                   node->hashNode ? node->hashNode->text : "NULL");
            break;
        case NODE_TOKEN_IDENTIFIER:
            printf("TOKEN_IDENTIFIER: %s\n",
                   node->hashNode ? node->hashNode->text : "NULL");
            break;
            /*
        case NODE_LITERAL_TRUE:
            printf("true\n");
            break;
        case NODE_LITERAL_FALSE:
            printf("true\n");
            break;
            */

        default:
            printf("NodeType: %s\n", NodeTypeNames[node->astNodeType]);
            break;
    }
}

void printTreeOLD(ASTNode *root, int level) {
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
void printOp(ASTNode *node) {
    if (node->astNodeType == NODE_PARENTHESIS_EXPRESSION) {
        printf("(");
        printNode(node->children[0], 0);
        printf(")");
    } else {
        printNode(node->children[0], 0);
        switch (node->astNodeType) {
            default:
                printf("Missed printOp value: %s\n",
                       NodeTypeNames[node->astNodeType]);
            case NODE_ADDITION:
                printf(" + ");
                break;
            case NODE_SUBTRACTION:
                printf(" - ");
                break;
            case NODE_MULTIPLICATION:
                printf(" * ");
                break;
            case NODE_DIVISION:
                printf(" / ");
                break;
            case NODE_LESS_THAN:
                printf(" < ");
                break;
            case NODE_GREATER_THAN:
                printf(" > ");
                break;
            case NODE_LOGICAL_OR:
                printf(" | ");
                break;
            case NODE_LOGICAL_AND:
                printf(" & ");
                break;
            case NODE_LESS_THAN_EQUAL:
                printf(" <= ");
                break;
            case NODE_GREATER_THAN_EQUAL:
                printf(" >= ");
                break;
            case NODE_EQUAL:
                printf(" == ");
                break;
            case NODE_NOT_EQUAL:
                printf(" != ");
                break;
        }
        printNode(node->children[1], 0);
    }
}

void printArgList(ASTNode *node) {
    if ((node->astNodeType != NODE_ARGS_LIST) &&
        (node->astNodeType != NODE_PARAM_LIST)) {
        printf("ERROR. Trying to print ArgList but node is not "
               "NODE_ARGS_LIST. Got %s instead\n",
               NodeTypeNames[node->astNodeType]);
    }
    printNode(node->children[0], 0);
    if (node->children[1]) {
        printf(",");
        printArgList(node->children[1]);
    }
}
