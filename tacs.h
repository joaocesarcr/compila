#ifndef TACS_HEADER
#define TACS_HEADER

#include "ast.h"
#include "hash.h"
typedef enum {
    TAC_JMP,
    TAC_TRUE,
    TAC_FALSE,
    TAC_JUMP_ELSE,
    TAC_JUMP_ZERO,
    TAC_LABEL,
    TAC_SYMBOL,
    TAC_ADD,
    TAC_SUB,
    TAC_MULTIPLY,
    TAC_DIVIDE,
    TAC_LESS_THAN,
    TAC_GREATER_THAN,
    TAC_LOGICAL_OR,
    TAC_LOGICAL_AND,
    TAC_LESS_THAN_EQUAL,
    TAC_GREATER_THAN_EQUAL,
    TAC_EQUAL,
    TAC_NOT_EQUAL,
    TAC_COPY,
    TAC_PRINT,
    TAC_FUNC_BEGIN,
    TAC_FUNC_END,
    TAC_READ,
    TAC_RETURN,
    TAC_FUNC_CALL,
} TacType;

const char *TacTypeNames[] = {
    "TAC_JMP",
    "TAC_TRUE",
    "TAC_FALSE",
    "TAC_JUMP_ELSE",
    "TAC_JUMP_ZERO",
    "TAC_LABEL",
    "TAC_SYMBOL",
    "TAC_ADD",
    "TAC_SUB",
    "TAC_MULTIPLY",
    "TAC_DIVIDE",
    "TAC_LESS_THAN",
    "TAC_GREATER_THAN",
    "TAC_LOGICAL_OR",
    "TAC_LOGICAL_AND",
    "TAC_LESS_THAN_EQUAL",
    "TAC_GREATER_THAN_EQUAL",
    "TAC_EQUAL",
    "TAC_NOT_EQUAL",
    "TAC_COPY",
    "TAC_PRINT",
    "TAC_FUNC_BEGIN",
    "TAC_FUNC_END",
    "TAC_READ",
    "TAC_RETURN",
    "TAC_FUNC_CALL",
};

typedef struct sTac_node {
    TacType tacType;
    struct sTac_node *prev;
    struct sTac_node *next;
    HASH_NODE *res;
    HASH_NODE *op1;
    HASH_NODE *op2;
} TAC;

TAC *tacCreate(TacType type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2);

void tacPrint(TAC *tac);
void tacPrintBackwards(TAC *tac);

// CODE GEN
TAC *generateCode(ASTNode *node);
TAC *tacGenOp(TacType OP, TAC *c1, TAC *c2);
TAC *makeIfThenElse(TAC *c0, TAC *c1, TAC *c2);
TAC *makeWhile(TAC *c0, TAC *c1);
TAC *makeFuncDec(TAC *c0, TAC *c1, TAC *c2, TAC *c3, HASH_NODE *name);
TAC *makeFuncCall(TAC *var, HASH_NODE *node);
#endif
