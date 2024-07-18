#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include "ast.h"
#include "hash.h"

extern int SemanticErrors;

void check_and_set_declarations(ASTNode *node);
void check_undeclared();

#endif
