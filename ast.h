#ifndef AST_HEADER
#define AST_HEADER

#include "hash.h"

#define MAX_CHILDREN 5
typedef enum {
    NODE_PROGRAM,
    NODE_DECLARATIONS_LIST,
    NODE_DECLARATION,
    NODE_VAR_DECLARATION,
    NODE_VECTOR_DECLARATION,
    NODE_VECTOR_DECLARATION_AND_ASIGN,
    NODE_FUNC_DECLARATION,
    NODE_FUNC_DECLARATION_EMPTY,
    NODE_PARAM_LIST,
    NODE_PARAM_LIST_EMPTY,
    NODE_PARAM,
    NODE_INITIAL_VALUE,
    NODE_VALUES_LIST,
    NODE_BLOCK,
    NODE_BLOCK_EMPTY,
    NODE_COMMANDS_LIST,
    NODE_COMMAND,
    NODE_ASSIGNMENT,
    NODE_VECTOR_INT,
    NODE_VECTOR_TK,
    NODE_CONTROL_FLOW,
    NODE_EXPRESSION,
    NODE_FUNC_CALL,
    NODE_FUNC_CALL_EMPTY,
    NODE_ARGS_LIST,
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
    NODE_LITERAL_INT,
    NODE_LITERAL_CHAR,
    NODE_LITERAL_REAL,
    NODE_LITERAL_STRING,
    NODE_LITERAL_FALSE,
    NODE_LITERAL_TRUE,
    NODE_TOKEN_ERROR,
    NODE_TOKEN_IDENTIFIER,
    NODE_TYPE,
    NODE_KW_CHAR,
    NODE_KW_READ,
    NODE_KW_INT,
    NODE_KW_FLOAT,
    NODE_KW_BOOL,
    NODE_KW_PRINT,
    NODE_KW_PRINT_STRING,
    NODE_KW_RETURN,
    NODE_KW_IF,
    NODE_IF_CONTROL,
    NODE_ELSE,
    NODE_KW_IF_ELSE,
    NODE_KW_WHILE,
    NODE_EMPTY,
    NODE_PARENTHESIS_EXPRESSION,
    NODE_VAR,
} NodeType;

typedef struct ASTNode_s {
    NodeType astNodeType;
    struct ASTNode_s *children[MAX_CHILDREN];
    // Additional fields can be added as necessary
    HASH_NODE *hashNode; // For identifiers, literals, etc.
} ASTNode;

ASTNode *createNode(NodeType type, ASTNode *children[MAX_CHILDREN],
                    HASH_NODE *hashNode);
int isListType(NodeType astNodeType);
int isKW(NodeType astNodeType);
int isLitType(NodeType astNodeType);
int isOP(NodeType astNodeType);
ASTNode **astNullChild();

void printOp(ASTNode *node);
void printNode(ASTNode *node, int level);
void printTree(ASTNode *root, int level);
void printOperation(ASTNode *node, int level);
void printAST(ASTNode *root);
void printIndentation(int level);

void printNodeOLD(ASTNode *node);
void printTreeOLD(ASTNode *root, int level);
void printArgList(ASTNode *node);

const char *NodeTypeNames[] = {
    "NODE_PROGRAM",
    "NODE_DECLARATIONS_LIST",
    "NODE_DECLARATION",
    "NODE_VAR_DECLARATION",
    "NODE_VECTOR_DECLARATION",
    "NODE_VECTOR_DECLARATION_AND_ASIGN",
    "NODE_FUNC_DECLARATION",
    "NODE_FUNC_DECLARATION_EMPTY",
    "NODE_PARAM_LIST",
    "NODE_PARAM_LIST_EMPTY",
    "NODE_PARAM",
    "NODE_INITIAL_VALUE",
    "NODE_VALUES_LIST",
    "NODE_BLOCK",
    "NODE_BLOCK_EMPTY",
    "NODE_COMMANDS_LIST",
    "NODE_COMMAND",
    "NODE_ASSIGNMENT",
    "NODE_VECTOR_INT",
    "NODE_VECTOR_TK",
    "NODE_CONTROL_FLOW",
    "NODE_EXPRESSION",
    "NODE_FUNC_CALL",
    "NODE_FUNC_CALL_EMPTY",
    "NODE_ARGS_LIST",
    "NODE_ADDITION",
    "NODE_SUBTRACTION",
    "NODE_MULTIPLICATION",
    "NODE_DIVISION",
    "NODE_LESS_THAN",
    "NODE_GREATER_THAN",
    "NODE_LOGICAL_OR",
    "NODE_LOGICAL_AND",
    "NODE_LESS_THAN_EQUAL",
    "NODE_GREATER_THAN_EQUAL",
    "NODE_EQUAL",
    "NODE_NOT_EQUAL",
    "NODE_LITERAL_INT",
    "NODE_LITERAL_CHAR",
    "NODE_LITERAL_REAL",
    "NODE_LITERAL_STRING",
    "NODE_LITERAL_FALSE",
    "NODE_LITERAL_TRUE",
    "NODE_TOKEN_ERROR",
    "NODE_TOKEN_IDENTIFIER",
    "NODE_TYPE",
    "NODE_KW_CHAR",
    "NODE_KW_READ",
    "NODE_KW_INT",
    "NODE_KW_FLOAT",
    "NODE_KW_BOOL",
    "NODE_KW_PRINT",
    "NODE_KW_PRINT_STRING",
    "NODE_KW_RETURN",
    "NODE_KW_IF",
    "NODE_IF_CONTROL",
    "NODE_ELSE",
    "NODE_KW_IF_ELSE",
    "NODE_KW_WHILE",
    "NODE_EMPTY",
    "NODE_PARENTHESIS_EXPRESSION",
    "NODE_VAR",
};
#endif
