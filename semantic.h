#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include "ast.h"
#include "hash.h"

extern int SemanticErrors;

void checkSemantic(ASTNode *root);
void check_and_set_declarations(ASTNode *node);
void check_undeclared_variables(ASTNode *node);
void checkVectorUsage(ASTNode *node);
void checkNodes(ASTNode *node, ASTNode *root);

void printIncompatibility(ASTNode *node, NodeType l, NodeType r);
NodeType checkOpType(ASTNode *node, NodeType type);
NodeType checkExpressionType(ASTNode *expression);
void checkExpressions(ASTNode *node);
int getParamQtt(ASTNode *node, int qtt);
NodeType getType(ASTNode *node);
ASTNode *checkFunctionReturn(ASTNode *node, ASTNode *funcRoot, NodeType type);

int checkFunctionCall(ASTNode *node, ASTNode *root);
int isCompatible(NodeType fType, NodeType sType);
int checkFuncParamType(ASTNode *call, ASTNode *def, HASH_NODE *tkid, int index);

ASTNode *findFunctionDeclaration(ASTNode *node, HASH_NODE *tokenHashNode);

void printIdentifiersTypeNature(ASTNode *node, ASTNode *otherNode);
void printOpError(ASTNode *node);

#endif
