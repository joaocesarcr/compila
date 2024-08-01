#include "tacs.h"
#include "ast.h"
#include "hash.h"
#include "semantic.h"
#include <stdlib.h>

TAC *tacCreate(TacType type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2) {
    TAC *newtac = 0;
    newtac = (TAC *)calloc(1, sizeof(TAC));
    newtac->tacType = type;
    newtac->res = res;
    newtac->op1 = op1;
    newtac->op2 = op2;
    newtac->prev = 0;
    newtac->next = 0;
    return newtac;
}

void tacPrint(TAC *tac) {
    if (!tac)
        return;
    // if (tac->tacType == TAC_SYMBOL) return;
    // printf("type%d", tac->tacType);
    fprintf(stderr, "TAC(");
    fprintf(stderr, "%s", TacTypeNames[tac->tacType]);
    fprintf(stderr, ",%s", (tac->res) ? tac->res->text : "0");
    fprintf(stderr, ",%s", (tac->op1) ? tac->op1->text : "0");

    fprintf(stderr, ",%s", (tac->op2) ? tac->op2->text : "0");
    fprintf(stderr, ");\n");
}
void tacPrintBackwards(TAC *tac) {
    if (!tac)
        return;

    else {
        tacPrintBackwards(tac->prev);
        tacPrint(tac);
    }
}

TAC *tacJoin(TAC *l1, TAC *l2) {
    TAC *point;
    if (!l1)
        return l2;
    if (!l2)
        return l1;
    for (point = l2; point->prev != 0; point = point->prev)
        ;
    point->prev = l1;
    return l2;
}
// CODE GEN
TAC *generateCode(ASTNode *node) {
    int i;
    TAC *result = 0;
    TAC *code[MAX_CHILDREN];
    if (!node)
        return NULL;
    for (i = 0; i < MAX_CHILDREN; i++)
        code[i] = generateCode(node->children[i]);

    switch (node->astNodeType) {
        default:
            result =
                tacJoin(tacJoin(tacJoin(code[0], code[1]), code[2]), code[3]);
            break;

        case NODE_LITERAL_INT:
        case NODE_TOKEN_IDENTIFIER:
            result = tacCreate(TAC_SYMBOL, node->hashNode, 0, 0);
            break;
        case NODE_ASSIGNMENT:
            result = tacJoin(code[1],
                             tacCreate(TAC_COPY, node->children[0]->hashNode,
                                       code[1] ? code[1]->res : 0, 0));
            break;
        case NODE_ADDITION:
            return tacGenOp(TAC_ADD, code[0], code[1]);
            break;
        case NODE_SUBTRACTION:
            return tacGenOp(TAC_SUB, code[0], code[1]);
            break;
        case NODE_DIVISION:
            return tacGenOp(TAC_DIVIDE, code[0], code[1]);
            break;
        case NODE_MULTIPLICATION:
            return tacGenOp(TAC_MULTIPLY, code[0], code[1]);
            break;
        case NODE_LESS_THAN:
            return tacGenOp(TAC_LESS_THAN, code[0], code[1]);
            break;
        case NODE_GREATER_THAN:
            return tacGenOp(TAC_GREATER_THAN, code[0], code[1]);
            break;
        case NODE_LOGICAL_OR:
            return tacGenOp(TAC_LOGICAL_OR, code[0], code[1]);
            break;
        case NODE_LOGICAL_AND:
            return tacGenOp(TAC_LOGICAL_AND, code[0], code[1]);
            break;
        case NODE_LESS_THAN_EQUAL:
            return tacGenOp(TAC_LESS_THAN_EQUAL, code[0], code[1]);
            break;
        case NODE_GREATER_THAN_EQUAL:
            return tacGenOp(TAC_GREATER_THAN_EQUAL, code[0], code[1]);
            break;
        case NODE_EQUAL:
            return tacGenOp(TAC_EQUAL, code[0], code[1]);
            break;
        case NODE_NOT_EQUAL:
            return tacGenOp(TAC_NOT_EQUAL, code[0], code[1]);
            break;
    }

    return result;
}

TAC *tacGenOp(TacType OP, TAC *c0, TAC *c1) {
    return tacJoin(tacJoin(c0, c1), tacCreate(OP, makeTemp(), c0 ? c0->res : 0,
                                              c1 ? c1->res : 0));
}
/*
 *result = tacJoin(tacJoin(code[0], code[1]),
                             tacCreate(TAC_ADD, makeTemp(),
                                       code[0] ? code[0]->res : 0,
                                       code[1] ? code[1]->res : 0));
*/
