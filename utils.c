#include "utils.h"
#include "ast.h"
#include "hash.h"
int isExpression(ASTNode *node) {
    NodeType expressions[] = {
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
        NODE_PARENTHESIS_EXPRESSION,
    };
    int size = 13;
    for (int i = 0; i < size; i++) {
        if (node->astNodeType == expressions[i])
            return 1;
    }
    return 0;
}
int isDeclaration(NodeType type) {
    NodeType lists[] = {
        NODE_FUNC_DECLARATION,   NODE_FUNC_DECLARATION_EMPTY,
        NODE_VECTOR_DECLARATION, NODE_VECTOR_DECLARATION_AND_ASIGN,
        NODE_VAR_DECLARATION,    NODE_PARAM,
    };
    int s = 6;
    for (int i = 0; i < s; i++)
        if (lists[i] == type)
            return 1;
    return 0;
}
int isNumberType(NodeType type) {
    if ((type == NODE_MULTIPLICATION) || (type == NODE_DIVISION) ||
        (type == NODE_ADDITION) || (type == NODE_SUBTRACTION) ||
        (type == NODE_KW_INT) || (type == NODE_LITERAL_INT) ||
        // char e int sao intercambiaveis
        (type == NODE_KW_CHAR) || (type == NODE_LITERAL_CHAR))
        return 1;
    else
        return 0;
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
        ((node->astNodeType == NODE_VECTOR_TK) &&
         (node->children[0]->hashNode->nature == NODE_KW_INT)) ||
        ((node->astNodeType == NODE_VECTOR_INT) &&
         (node->children[0]->hashNode->nature == NODE_KW_INT)) ||

        ((node->astNodeType == NODE_FUNC_CALL) &&
         (node->children[0]->hashNode->nature == NODE_KW_INT)) ||

        // char e int sao intercambiaveis
        ((node->astNodeType == NODE_FUNC_CALL) &&
         (node->children[0]->hashNode->nature == NODE_KW_CHAR)) ||

        ((node->astNodeType == NODE_TOKEN_IDENTIFIER) &&
         (node->hashNode->nature == NODE_KW_CHAR)) ||
        (node->astNodeType == NODE_KW_CHAR) ||
        (node->astNodeType == NODE_LITERAL_CHAR))
        return 1;
    else
        return 0;
}
int isKW(NodeType astNodeType) {
    NodeType lists[] = {
        NODE_KW_CHAR, NODE_KW_READ, NODE_KW_INT, NODE_KW_FLOAT, NODE_KW_BOOL,
    };
    int s = 5;
    for (int i = 0; i < s; i++)
        if (lists[i] == astNodeType)
            return 1;
    return 0;
}
int isListType(NodeType astNodeType) {
    NodeType lists[] = {
        NODE_DECLARATIONS_LIST, NODE_EMPTY,         NODE_PARAM_LIST,
        NODE_VALUES_LIST,       NODE_COMMANDS_LIST, NODE_ARGS_LIST,
    };
    int s = 9;
    for (int i = 0; i < s; i++)
        if (lists[i] == astNodeType)
            return 1;
    return 0;
}
int isLitType(NodeType astNodeType) {
    static const NodeType lists[] = {
        NODE_EMPTY,         NODE_TOKEN_IDENTIFIER, NODE_LITERAL_INT,
        NODE_LITERAL_CHAR,  NODE_LITERAL_REAL,     NODE_LITERAL_STRING,
        NODE_LITERAL_FALSE, NODE_LITERAL_TRUE,

    };
    size_t size = sizeof(lists) / sizeof(lists[0]);

    for (size_t i = 0; i < size; i++) {
        if (lists[i] == astNodeType) {
            return 1;
        }
    }
    return 0;
}
void printIndentation(int level) {
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
}
int isOP(NodeType astNodeType) {
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
    size_t size = sizeof(lists) / sizeof(lists[0]);

    for (size_t i = 0; i < size; i++) {
        if (lists[i] == astNodeType) {
            return 1;
        }
    }
    return 0;
}
