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

typedef struct ASTNode {
    NodeType type;
    struct ASTNode* children[MAX_CHILDREN];
    // Additional fields can be added as necessary
    
    char* value;  // For identifiers, literals, etc.
} ASTNode;

ASTNode* createNode(NodeType type, ASTNode* children[MAX_CHILDREN], char* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    for (int i = 0; i < MAX_CHILDREN; ++i) {
        node->children[i] = children[i];
    }
    node->value = value ? strdup(value) : NULL;
    return node;
}
