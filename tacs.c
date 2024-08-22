#include "tacs.h"
#include "ast.h"
#include "hash.h"
#include "semantic.h"
#include <stdlib.h>

TAC *makeIfThen(TAC *c0, TAC *c1);
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
    if (tac->tacType == TAC_SYMBOL)
        return;
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

            /*
        case NODE_FUNC_CALL:
        case NODE_FUNC_CALL_EMPTY:
            // JUMP TO FUNCTION
            // mas e o return?
            // return tacGenFunc(code[0], code[1]);
            result = tacCreate(TAC_FUNC_CALL,makeTemp(),node->hashNode,
            break;
            */

        case NODE_KW_RETURN:
            result = makeReturn();

        case NODE_KW_READ:
            result = tacCreate(TAC_READ, node->children[1]->hashNode, 0, 0);
            break;
        case NODE_FUNC_DECLARATION:
        case NODE_FUNC_DECLARATION_EMPTY:
            result = makeFuncDec(code[0], code[1], code[2], code[3],
                                 node->children[1]->hashNode);
            break;

        case NODE_KW_PRINT:
        case NODE_KW_PRINT_STRING:
            result = tacCreate(TAC_PRINT,
                               node->children[1] ? node->children[1]->hashNode
                                                 : node->children[0]->hashNode,
                               0, 0);

            break;

        case NODE_KW_IF:
            result = makeIfThen(code[0], code[1]);
            break;
        case NODE_KW_IF_ELSE:
            result = makeIfThenElse(code[0], code[1], code[2]);
            break;
        case NODE_KW_WHILE:
            result = makeWhile(code[0], code[1]);
            break;
        case NODE_LITERAL_INT:
        case NODE_LITERAL_REAL:
        case NODE_LITERAL_CHAR:
        case NODE_LITERAL_TRUE:
        case NODE_LITERAL_FALSE:
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

TAC *makeIfThen(TAC *c0, TAC *c1) {
    // c0: expression to be evaluated
    // c1: block
    TAC *jumptac = 0;
    TAC *labeltac = 0;

    HASH_NODE *newlabel = makeLabel();
    jumptac = tacCreate(TAC_JUMP_ZERO, newlabel, c0->res ? c0->res : 0, 0);
    labeltac = tacCreate(TAC_LABEL, newlabel, 0, 0);

    jumptac->prev = c0;
    labeltac->prev = c1;
    return tacJoin(jumptac, labeltac);
    /*
     C0
     TAC(TAC_JUMP_ZERO,jumptac,true,0); c0
     C1
     TAC(TAC_LABEL,newlabel,0,0);
     */
}
TAC *makeIfThenElse(TAC *c0, TAC *c1, TAC *c2) {
    // c0: expression to be evaluated
    // c1: true block
    // c2: block

    TAC *jumptac = 0;
    TAC *jumpElseEND = 0;
    TAC *labeltac = 0;
    TAC *elseENDtac = 0;

    HASH_NODE *newlabel = 0;
    HASH_NODE *elseEND = 0;

    newlabel = makeLabel();
    elseEND = makeLabel();

    labeltac = tacCreate(TAC_LABEL, newlabel, 0, 0);
    elseENDtac = tacCreate(TAC_LABEL, elseEND, 0, 0);

    jumptac = tacCreate(TAC_JUMP_ZERO, newlabel, c0->res ? c0->res : 0, 0);
    jumpElseEND = tacCreate(TAC_JMP, elseEND, 0, 0);

    jumptac->prev = c0;
    labeltac->prev = jumpElseEND;
    elseENDtac->prev = c2;
    jumpElseEND->prev = c1;

    return tacJoin(tacJoin(jumptac, labeltac), elseENDtac);
    /*
       C0
       TAC(TAC_JUMP_ZERO,jumpElseStart,false,0);
       C1
       TAC(TAC_JMP,elseEnd,0,0);
       TAC(TAC_LABEL,elseStart,0,0);
       C2
       TAC(TAC_LABEL,elseEnd,0,0);
    */
}

TAC *makeWhile(TAC *c0, TAC *c1) {
    // c0: expression to be evaluated
    // c1: block

    TAC *tacJmpZ = 0;
    TAC *tacJmpWhileStart = 0;
    TAC *tacLabelWhileStart = 0;
    TAC *tacLabelWhileEnd = 0;

    HASH_NODE *whileStart = 0;
    HASH_NODE *whileEnd = 0;

    whileEnd = makeLabel();
    whileStart = makeLabel();

    tacLabelWhileStart = tacCreate(TAC_LABEL, whileStart, 0, 0);
    tacLabelWhileEnd = tacCreate(TAC_LABEL, whileEnd, 0, 0);
    tacJmpZ = tacCreate(TAC_JUMP_ZERO, whileEnd, c0->res ? c0->res : 0, 0);
    tacJmpWhileStart = tacCreate(TAC_JMP, whileStart, 0, 0);

    // tacLabelWhileStart->prev = c1;
    tacJmpZ->prev = c0;
    tacJmpWhileStart->prev = c1;
    // tacLabelWhileEnd->prev = tacJmpWhileStart;
    return tacJoin(
        tacJoin(tacJoin(tacLabelWhileStart, tacJmpZ), tacJmpWhileStart),
        tacLabelWhileEnd);

} /*
  TAC(TAC_LABEL,labelWhileStart,0,0); // hashWhileStartLabelLabel
  C0
  TAC(TAC_JUMP_ZERO,labelWhileEnd,true,0) // jumpEndWhile
  C1
  TAC(TAC_JMP,labelWhileStart,0,0) // jumpStartWhile
  TAC(TAC_LABEL,labelWhileEnd,0,0);// labelWhileEnd
  */

TAC *makeFuncCall(TAC *c0, TAC *c1) {
    //
}
TAC *makeFuncDec(TAC *c0, TAC *c1, TAC *c2, TAC *c3, HASH_NODE *name) {
    TAC *beginFun = 0;
    TAC *endFun = 0;

    HASH_NODE *beginFunNode = 0;
    HASH_NODE *endFunNode = 0;

    beginFunNode = makeLabel();
    endFunNode = makeLabel();

    beginFun = tacCreate(TAC_FUNC_BEGIN, beginFunNode, name, 0);
    endFun = tacCreate(TAC_FUNC_END, endFunNode, 0, 0);
    return tacJoin(tacJoin(beginFun, c3), endFun);
}

/*
    TAC(TAC_FUNC_CALL,labelX,0,0);

    TAC(TAC_FUNC_BEGIN,lLLlLIabel_3,main,0);
    TAC(TAC_LABEL,lLLlLIabel_2,0,0);
    TAC(TAC_DIVIDE,lIlIlIlIlTemp_0,n,2);
    TAC(TAC_LESS_THAN_EQUAL,lIlIlIlIlTemp_1,i,lIlIlIlIlTemp_0);
    TAC(TAC_JUMP_ZERO,lLLlLIabel_1,lIlIlIlIlTemp_1,0);
    TAC(TAC_MULTIPLY,lIlIlIlIlTemp_2,i,i);
    TAC(TAC_EQUAL,lIlIlIlIlTemp_3,lIlIlIlIlTemp_2,16);
    TAC(TAC_JUMP_ZERO,lLLlLIabel_0,lIlIlIlIlTemp_3,0);
    TAC(TAC_LABEL,lLLlLIabel_0,0,0);
    TAC(TAC_ADD,lIlIlIlIlTemp_4,i,1);
    TAC(TAC_COPY,i,lIlIlIlIlTemp_4,0);
    TAC(TAC_JMP,lLLlLIabel_2,0,0);
    TAC(TAC_LABEL,lLLlLIabel_1,0,0);
    TAC(TAC_RET, returnvar, oqvemdpsdereturn,0);

    TAC(TAC_COPY, labelX, oqvemdpsdereturn,0);
    TAC(TAC_JMP,afterFuncCall,0,0);

    TAC(TAC_FUNC_END,lLLlLIabel_4,0,0);
 */
