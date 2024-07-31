#ifndef UTILS_HEADER
#define UTILS_HEADER

#include "ast.h"
#include "hash.h"

void printIndentation(int level);
int isExpression(ASTNode *node);
int isDeclaration(NodeType type);
int isNumberType(NodeType type);
int isNumber(ASTNode *node);
int isKW(NodeType astNodeType);
int isListType(NodeType astNodeType);
int isLitType(NodeType astNodeType);
int isOP(NodeType astNodeType);
#endif
