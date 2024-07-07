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
    NODE_FUNC_DECLARATION,
    NODE_PARAM_LIST,
    NODE_PARAM,
    NODE_TYPE,
    NODE_INITIAL_VALUE,
    NODE_VALUES_LIST,
    NODE_BLOCK,
    NODE_COMMANDS_LIST,
    NODE_COMMAND,
    NODE_ASSIGNMENT,
    NODE_VECTOR,
    NODE_CONTROL_FLOW,
    NODE_EXPRESSION,
    NODE_FUNC_CALL,
    NODE_ARGS_LIST
} NodeType;

typedef struct ASTNode_s {
    NodeType type;
    struct ASTNode_s* children[MAX_CHILDREN];
    // Additional fields can be added as necessary
    HASH_NODE value;  // For identifiers, literals, etc.
} ASTNode;

ASTNode* createNode(NodeType type, ASTNode* children[MAX_CHILDREN], HASH_NODE value);

#endif
