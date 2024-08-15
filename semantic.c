#include "semantic.h"

#include "hash.h"
#include "utils.h"
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * declarations and usage:
 *   arrays with index
 *   function with param
 *   comparing types on assignment
 *   comparing param/assingment list
 *   missing return type
 */

// Ha quatro tipos de dados para declarações: char, int, float, bool
int SemanticErrors = 0;

typedef enum {
    DECLARED_VAR,
    DECLARED_VECTOR,
    DECLARED_FUNC,
    DECLARED_FUNC_PARAMETER,
} SEMANTIC_NATURE;

void checkSemantic(ASTNode *root) {
    printTreeOLD(root, 0);
    check_and_set_declarations(root);
    // hashPrintNatures();
    check_undeclared_variables(root);
    checkNodes(root, root);
    // checkExpressions(root);
    if (SemanticErrors) {
        printf("Total of %d semantic errors\n", SemanticErrors);
        exit(4);
    }
}

NodeType getExpressionType(ASTNode *node) {
    NodeType l = NODE_EMPTY, r = NODE_EMPTY;
    if (isExpression(node)) {
        if (node->astNodeType == NODE_PARENTHESIS_EXPRESSION)
            return getType(node->children[0]);
        else {
            l = getType(node->children[0]);
            r = getType(node->children[1]);
        }
        if (!(isCompatible(l, r)) && (l && r)) {
            printIncompatibility(node, l, r);
            SemanticErrors++;
            return 0;
        } else
            return checkOpType(node, l);
    }
    printf("Tried get expression but got empty node\n");
    return 0;
}

void printIncompatibility(ASTNode *node, NodeType l, NodeType r) {
    if (l && r) {
        printf("SEMANTIC ERROR! Expression types are not "
               "compatible!\n");
        printf("Evaluating ");
        printNode(node, 0);
        printf("\n  Left: ");
        printNode(node->children[0], 0);
        printf("\n  Right: ");
        printNode(node->children[1], 0);
        printf("\n  Left is %s and right is %s\n", NodeTypeNames[l],
               NodeTypeNames[r]);
    } else {
        printf("SEMANTIC ERROR! Expression types are not "
               "compatible!\n");
        printf("Evaluating ");
        printNode(node, 0);
        printf("\n");
    }
    if (!l) {
        printf("  Couldn't get type for left children: ");
        printNode(node->children[0], 0);
        printf("\n");
        printf("  Right children is ");
        printNode(node->children[1], 0);
        printf(" of type %s\n", NodeTypeNames[r]);
    }
    if (!r) {
        printf("  Couldn't get type for right children: ");
        printNode(node->children[1], 0);
        printf("\n");
        printf("  Left children is ");
        printNode(node->children[0], 0);
        printf(" of type %s\n", NodeTypeNames[l]);
    }
}

NodeType checkOpType(ASTNode *node, NodeType type) {
    // Given an op where the expressions are compatible, returns the op type
    if (node->astNodeType == NODE_PARENTHESIS_EXPRESSION)
        return type;
    if (isCompatible(node->astNodeType, type)) {
        switch (node->astNodeType) {
            case NODE_ADDITION:
            case NODE_SUBTRACTION:
            case NODE_MULTIPLICATION:
            case NODE_DIVISION:
                return type;

            case NODE_LESS_THAN:
            case NODE_GREATER_THAN:
            case NODE_LOGICAL_OR:
            case NODE_LOGICAL_AND:
            case NODE_LESS_THAN_EQUAL:
            case NODE_GREATER_THAN_EQUAL:
            case NODE_EQUAL:
            case NODE_NOT_EQUAL:
                return NODE_KW_BOOL;
            default:
                printf("Default called in checkOpType with node of type %s and "
                       "type %s\n",
                       NodeTypeNames[node->astNodeType], NodeTypeNames[type]);
        }
    } else {
        SemanticErrors++;
        printf("Default called in checkOpType with node of type %s and "
               "type %s\n",
               NodeTypeNames[node->astNodeType], NodeTypeNames[type]);
    }
}

void checkExpressions(ASTNode *node) {
    int i;
    for (i = 0; i < MAX_CHILDREN; i++) {
        if (node->children[i]) {
            if (isExpression(node->children[i])) {
                getExpressionType(node->children[i]);
            } else
                checkExpressions(node->children[i]);
        }
    }
}

void check_and_set_declarations(ASTNode *node) {
    int i = 0;
    if (node == NULL)
        return;
    if (isDeclaration(node->astNodeType)) {
        if (node->children[1]->hashNode->hashNodeType != TK_IDENTIFIER) {
            printf("SEMANTIC ERROR: %s already declared as %s\n",
                   node->children[1]->hashNode->text,
                   NodeTypeNames[node->children[1]->hashNode->nature]);
            SemanticErrors++;
        }
        switch (node->astNodeType) {
            default:
                printf("Error: default option used in "
                       "check_and_set_declarations\n");
                break;
            case NODE_FUNC_DECLARATION:
            case NODE_FUNC_DECLARATION_EMPTY:
                node->children[1]->hashNode->hashNodeType = DECLARED_FUNC;
                break;
            case NODE_VECTOR_DECLARATION_AND_ASIGN:
                node->children[1]->hashNode->hashNodeType = DECLARED_VECTOR;
                break;
            case NODE_VECTOR_DECLARATION:
                node->children[1]->hashNode->hashNodeType = DECLARED_VECTOR;
                break;
            case NODE_VAR_DECLARATION:
                node->children[1]->hashNode->hashNodeType = DECLARED_VAR;
                break;
            case NODE_PARAM:
                node->children[1]->hashNode->hashNodeType =
                    DECLARED_FUNC_PARAMETER;
                break;
        }

        node->children[1]->hashNode->nature = node->children[0]->astNodeType;
        if (0)
            printf("Setting nature for %s as %s\n",
                   node->children[1]->hashNode->text,
                   NodeTypeNames[node->children[0]->astNodeType]);
    }
    switch (node->astNodeType) {
        default:
            break;
        case NODE_LITERAL_INT:
            node->hashNode->nature = NODE_KW_INT;
            break;
        case NODE_LITERAL_CHAR:
            node->hashNode->nature = NODE_KW_CHAR;
            break;
        case NODE_LITERAL_REAL:
            node->hashNode->nature = NODE_KW_FLOAT;
            break;
        case NODE_LITERAL_FALSE:
        case NODE_LITERAL_TRUE:
            // node->hashNode->nature = KW_BOOL;
            break;
    }
    for (i = 0; i < MAX_CHILDREN; i++) {
        if (node->children[i]) {
            check_and_set_declarations(node->children[i]);
        } else
            break;
    }
}

void check_undeclared_variables(ASTNode *node) {
    if (node == NULL)
        return;

    if (node->astNodeType == NODE_TOKEN_IDENTIFIER) {
        HASH_NODE *symbol = node->hashNode;
        if (!symbol) {
            printf("SEMANTIC ERROR: %s without hashNode\n",
                   node->hashNode->text);
            SemanticErrors++;
        }
        if (symbol->hashNodeType == TK_IDENTIFIER) {
            printf("SEMANTIC ERROR: %s undeclared\n", node->hashNode->text);
            SemanticErrors++;
        }
    }
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (node->children[i])
            check_undeclared_variables(node->children[i]);
        else
            break;
    }
}

int checkFunctionCall(ASTNode *node, ASTNode *declaration) {
    // Returns 1 if the call satisfies the declaration
    // else return 0
    // node: NODE_FUNC_CALL
    // declaration: NODE_FUNC_DECLARATION

    // Check if both are empty
    if (node->astNodeType == NODE_FUNC_CALL_EMPTY) {
        if (declaration->astNodeType == NODE_FUNC_DECLARATION_EMPTY)
            return 1;
        else {
            SemanticErrors++;
            printf("Function %s called without parameters\n",
                   node->children[0]->hashNode->text);
            return 0;
        }
    } else if (declaration->astNodeType == NODE_FUNC_DECLARATION_EMPTY) {
        SemanticErrors++;
        printf("Function %s without parameters called with parameters\n",
               node->children[0]->hashNode->text);
        return 0;
    }

    // Check if the func call has the same number of parameters
    HASH_NODE *tkid = node->children[0]->hashNode;
    int qttDef;
    if ((node->astNodeType == NODE_VECTOR_DECLARATION) ||
        node->astNodeType == NODE_VECTOR_DECLARATION_AND_ASIGN) {
        qttDef = atoi(declaration->children[2]->hashNode->text);
    } else {
        qttDef = getParamQtt(node->children[1], 0);
    }
    int qttCal = getParamQtt(node->children[1], 0);
    if (qttCal != qttDef) {
        printf("SEMANTIC ERROR: %s call must have %d parameters. Called "
               "with %d\n",
               tkid->text, qttDef, qttCal);
    }

    checkFuncParamType(node->children[1], declaration->children[2],
                       declaration->children[1]->hashNode, 0);
    // Check if the type of each list element is compatible with definition
    // checkListType(node->children[1],
    // declaration->children[0]->astNodeType, 0,
    // declaration->children[1]->hashNode);
}

int checkFuncParamType(ASTNode *call, ASTNode *def, HASH_NODE *tkid,
                       int index) {
    // checks if the function call satisfies its definition
    if ((!def->children[0]) && (!(call->children[0]))) {
        return 1;
    }
    if (((def->children[0]) && (!(call->children[0]))) ||
        ((!def->children[0]) && (call->children[0]))) {
        printf("SEMANTIC ERROR: func call size doesnt match with def\n");
        return 0;
    }

    if (isCompatible(getType(def->children[0]), getType(call->children[0]))) {
        if (def->children[1] && call->children[1]) {
            checkFuncParamType(call->children[1], def->children[1], tkid,
                               index + 1);
        }
        return 1;
    } else {
        printf("SEMANTIC ERROR: func %s param %d expected to be of type ",
               tkid->text, index);
        printf("%s, but found %s instead\n",
               NodeTypeNames[getType(def->children[0])],
               NodeTypeNames[getType(call->children[0])]);
        return 0;
    }
}

int checkVectorTypes(ASTNode *node, NodeType type, int index,
                     HASH_NODE *declaration) {
    // percore a lita e verifica se os elementos functionam com o tipo
    // node: inicio da lista de parametros
    // type: tipo esperado
    // index: posicao atual no vetor\
    // declaration: endereco do tkid na tabela hash
    int printDebug = 1;
    if (node == NULL) {
        if (printDebug)
            printf("checkListTyp NULL node\n");
        return 1;
    }

    if (node->children[1]) { // Se nao final da lista, compara e chama
        int nodeType = getType(node->children[0]);
        if (node->children[0]->astNodeType == NODE_TOKEN_IDENTIFIER) {
            nodeType = node->children[0]->hashNode->nature;
            if (printDebug)
                printf("checking type for %s[%d]. comparing %s of type %s "
                       "with "
                       "expected %s \n",
                       declaration->text, index,
                       node->children[0]->hashNode->text,
                       NodeTypeNames[nodeType], NodeTypeNames[type]);

        } else {
            nodeType = node->children[0]->astNodeType;
            if (printDebug)
                printf("checking type for %s[%d]. comparing type %s with "
                       "expected %s \n",
                       declaration->text, index, NodeTypeNames[nodeType],
                       NodeTypeNames[type]);
        }

        if (isCompatible(nodeType, type)) {
            if (printDebug)
                printf("  %s is compatible with %s\n", NodeTypeNames[nodeType],
                       NodeTypeNames[type]);

            return checkVectorTypes(node->children[1], type, index + 1,
                                    declaration);
        } else {
            printf("SEMANTIC ERROR: %s[%d] expected to be %s, but "
                   "got %s instead\n",
                   declaration->text, index, NodeTypeNames[type],
                   NodeTypeNames[node->children[0]->astNodeType]);
            SemanticErrors++;
            return 0;
        }
    } else { // Se final de lista, compara 0
        // return (isCompatible(node->children[0]->hashNode->nature, type));
        if (isCompatible(node->children[0]->hashNode->nature, type)) {
            return 1;
        } else {
            if (node->children[0] && 0) {
                printf("SEMANTIC ERROR: %s[%d] expected to be "
                       "%s, but "
                       "got %s instead\n",
                       declaration->text, index, NodeTypeNames[type],
                       NodeTypeNames[node->children[0]->astNodeType]);

                SemanticErrors++;
                return 0;
            }
        }
    }
}

void checkNodes(ASTNode *node, ASTNode *root) {
    // Check all function calls for correct usage;
    int i;
    if (node == NULL)
        return;
    if ((node->astNodeType == NODE_IF_CONTROL) ||
        (node->astNodeType == NODE_KW_WHILE)) {
        NodeType c = getType(node->children[0]);
        if (!isCompatible(c, NODE_KW_BOOL)) {
            printf("SEMANTIC ERROR: ");
            printNode(node, 0);
            printf("\n  ");
            printf(
                "If expected expression of type bool, but found %s instead \n",
                NodeTypeNames[c]);
        }
    }
    if ((node->astNodeType == NODE_FUNC_DECLARATION) ||
        (node->astNodeType == NODE_FUNC_DECLARATION_EMPTY)) {
        checkFunctionReturn(node, node, node->children[0]->astNodeType);
    }

    if ((node->astNodeType == NODE_FUNC_CALL) ||
        (node->astNodeType == NODE_FUNC_CALL_EMPTY)) {
        ASTNode *declaration =
            findFunctionDeclaration(root, node->children[0]->hashNode);
        if (declaration) {
            checkFunctionCall(node, declaration);
        }

        else {
            printf("Function \"%s\" was not defined\n",
                   node->children[0]->astNodeType == NODE_TOKEN_IDENTIFIER
                       ? node->children[0]->hashNode->text
                       : NodeTypeNames[node->children[0]->astNodeType]);
        }
    }
    if ((node->astNodeType == NODE_VECTOR_INT) ||
        (node->astNodeType == NODE_VECTOR_TK)) {
        checkVectorUsage(node);
    }

    if (node->astNodeType == NODE_VECTOR_DECLARATION_AND_ASIGN) {
        int paramQtt = getParamQtt(node->children[3], 0);
        // printf("VECTOR DECLARED WITH SIZE int cnv %d\n",
        //       atoi(node->children[2]->hashNode->text));
        // printf("VECTOR DECLARED WITH SIZE %s\n",
        //      node->children[2]->hashNode->text);
        int declarationQtt = atoi(node->children[2]->hashNode->text);
        if (paramQtt != declarationQtt) {
            printf("SEMANTIC ERROR: trying to assign %d params to %d params "
                   "vector\n",
                   paramQtt, declarationQtt);
            SemanticErrors++;
        }
        checkVectorTypes(node->children[3], node->children[0]->astNodeType, 0,
                         node->children[1]->hashNode);
    }

    if (node->astNodeType == NODE_ASSIGNMENT) {
        NodeType c0 = getType(node->children[0]);
        NodeType c1 = getType(node->children[1]);
        if (!isCompatible(c0, c1)) {
            printf("Node assignment error!\n");
            printNode(node, 0);
            printf("\n");
            printIncompatibility(node, c0, c1);
        }
    }
    for (i = 0; i < MAX_CHILDREN; i++) {
        if (node->children[i]) {
            // Check if function was called with parameters
            ASTNode *child = node->children[i];
            if (child->astNodeType == NODE_TOKEN_IDENTIFIER) {
                if (child->hashNode->hashNodeType == DECLARED_FUNC) {
                    if (!((node->astNodeType == NODE_FUNC_CALL) ||
                          (node->astNodeType == NODE_FUNC_DECLARATION) ||
                          (node->astNodeType == NODE_FUNC_DECLARATION_EMPTY) ||
                          (node->astNodeType == NODE_FUNC_CALL_EMPTY))) {
                        printf("SEMANTIC ERROR: %s is a function\n",
                               child->hashNode->text);
                        printf("Called function from node of type %s\n",
                               NodeTypeNames[node->astNodeType]);
                        SemanticErrors++;
                    }
                } else if (child->hashNode->hashNodeType == DECLARED_VECTOR) {
                    // Check if vector was called as vector
                    if (!((node->astNodeType ==
                           NODE_VECTOR_DECLARATION_AND_ASIGN) ||
                          (node->astNodeType == NODE_VECTOR_DECLARATION) ||
                          (node->astNodeType == NODE_VECTOR_INT) ||
                          (node->astNodeType == NODE_VECTOR_TK))) {
                        printf("SEMANTIC ERROR: %s is an array and can "
                               "only be "
                               "used with indexes\n",
                               child->hashNode->text);
                        SemanticErrors++;
                    }
                }

                // Check if function is called with parameters
            }
            checkNodes(node->children[i], root);
        }
    }
}
ASTNode *checkFunctionReturn(ASTNode *node, ASTNode *funcRoot, NodeType type) {
    // node: First call: NODE_FUNC_DECLARATION or
    // NODE_FUNC_DECLARATION_EMPTY Return: the node of the return of the
    // function declared in node
    if (node == NULL) {
        return NULL;
    }

    if (node->astNodeType == NODE_KW_RETURN) {
        // printf("Here checking %s and %s\n", NodeTypeNames[type],
        // NodeTypeNames[getType(node)]);
        NodeType c0Type = getType(node->children[0]);
        if (!(isCompatible(c0Type, type))) {
            printf("SEMANTIC ERROR: func %s should return %s. Got ",
                   funcRoot->children[1]->hashNode->text,
                   NodeTypeNames[funcRoot->children[0]->astNodeType]);
            printNode(node->children[0], 0);
            printf(" of type %s instead\n", NodeTypeNames[c0Type]);

            SemanticErrors++;
        }
    }

    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (node->children[i]) {
            ASTNode *funcReturn =
                checkFunctionReturn(node->children[i], funcRoot, type);
        }
    }
}

ASTNode *findFunctionDeclaration(ASTNode *node, HASH_NODE *tokenHashNode) {
    if (node == NULL) {
        return NULL;
    }

    if ((node->astNodeType == NODE_FUNC_DECLARATION) ||
        (node->astNodeType == NODE_FUNC_DECLARATION_EMPTY)) {
        if (strcmp(node->children[1]->hashNode->text, tokenHashNode->text) ==
            0) {
            return node;
        }
    }

    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (node->children[i]) {
            ASTNode *funcDeclaration =
                findFunctionDeclaration(node->children[i], tokenHashNode);
            if (funcDeclaration != NULL) {
                return funcDeclaration;
            }
        }
    }

    return NULL;
}

void checkVectorUsage(ASTNode *node) {
    // Check vector usage
    int i;

    if (node == NULL) {
        return;
    }
    // printf("Node: %s\n", NodeTypeNames[node->astNodeType]);

    switch (node->astNodeType) {
        default:
            break;
        case NODE_VECTOR_INT:
            if ((node->children[0]->hashNode->hashNodeType !=
                 DECLARED_VECTOR)) {
                SemanticErrors++;
                printf("SEMANTIC ERROR: %s[%s] incorrect vector usage. "
                       "Indexes "
                       "should "
                       "only "
                       "be used on vectors\n",
                       node->children[0]->hashNode->text,
                       node->children[1]->hashNode->text);
                printIdentifiersTypeNature(node->children[0],
                                           node->children[1]);
            }
            break;
        case NODE_VECTOR_TK:
            if ((node->children[0]->hashNode->hashNodeType !=
                 DECLARED_VECTOR)) {
                SemanticErrors++;
                printf("SEMANTIC ERROR: %s[%s] incorrect vector usage. "
                       "Indexes "
                       "should "
                       "only "
                       "be used on vectors\n",
                       node->children[0]->hashNode->text,
                       node->children[1]->hashNode->text);
                printIdentifiersTypeNature(node->children[0],
                                           node->children[1]);
            }

            if (!(isNumber(node->children[1]))) {
                SemanticErrors++;
                printf("SEMANTIC ERROR: %s[%s] incorrect vector usage. Index "
                       "must be "
                       "int compatible \n",
                       node->children[0]->hashNode->text,
                       node->children[1]->hashNode->text);
                printIdentifiersTypeNature(node->children[0],
                                           node->children[1]);
            }
            break;
    }
}

int getParamQtt(ASTNode *node, int qtt) {
    if (node == NULL) {
        return qtt + 1;
    }

    if (node->children[1]) {
        if ((node->children[1]->astNodeType == NODE_ARGS_LIST) ||
            (node->children[1]->astNodeType == NODE_VALUES_LIST) ||
            (node->children[1]->astNodeType == NODE_PARAM_LIST)) {
            return getParamQtt(node->children[1], qtt + 1);
        }
    } else
        return qtt + 1;
}

void setExpressionValue(ASTNode *node, ASTNode *parent) {
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (node->children[i])
            // node->hashNodesetExpressionValue(node->children[i], node);
            i++;
        else
            break;
    }
    // if node is expression
    // node.type = setExpressionValue
    switch (node->astNodeType) {
        default:
            break;

        case NODE_ADDITION:
        case NODE_SUBTRACTION:
        case NODE_MULTIPLICATION:
        case NODE_DIVISION:
        case NODE_LESS_THAN:
        case NODE_GREATER_THAN:
        case NODE_LOGICAL_OR:
        case NODE_LOGICAL_AND:
            break;
        case NODE_EXPRESSION:
            // if ((node->children[0]->hashNode->hashNodeType !=
            break;
    }
}

void printOpError(ASTNode *node) {
    NodeType c0, c1;
    c0 = getType(node->children[0]);
    c1 = getType(node->children[1]);
    if (c0 && c1) {
        if (!(isCompatible(c0, c1))) {
            printf("SEMANTIC ERROR: ");
            printOp(node);
            printf(" error. Invalid operands for : ");
            printf("%s, %s\n", NodeTypeNames[getType(node->children[0])],
                   NodeTypeNames[getType(node->children[1])]);
            // printIdentifiersTypeNature(node->children[0], node->children[1]);
            SemanticErrors++;
        }
    }
}
void printIdentifiersTypeNature(ASTNode *node, ASTNode *otherNode) {
    printf("    Identifiers: ");
    printNode(node, 0);
    printf(" , ");
    printNode(otherNode, 0);
    printf("\n");
    printf("    astNode type: %s , %s \n", NodeTypeNames[node->astNodeType],
           NodeTypeNames[otherNode->astNodeType]);

    printf("    Natures: %s , %s\n", NodeTypeNames[node->hashNode->nature],
           NodeTypeNames[otherNode->hashNode->nature]);
}
int isCompatible(NodeType fType, NodeType sType) {
    // if (isNumberType(fType) && isNumberType(sType)) {
    //     return 1;
    // }
    int vReturn = 0;
    int printDebug = 1;
    if (fType == NODE_LITERAL_CHAR) {
        fType = NODE_KW_CHAR;
    }
    if (fType == NODE_LITERAL_INT) {
        fType = NODE_KW_INT;
    }
    if (fType == NODE_LITERAL_REAL) {
        fType = NODE_KW_FLOAT;
    }
    if (sType == NODE_LITERAL_CHAR) {
        sType = NODE_KW_CHAR;
    }
    if (sType == NODE_LITERAL_INT) {
        sType = NODE_KW_INT;
    }
    if (sType == NODE_LITERAL_REAL) {
        sType = NODE_KW_FLOAT;
    }

    if (fType == sType) {
        vReturn = 1;
    } else {
        if (printDebug)
            printf("isCompatible called with types %s and %s.",
                   NodeTypeNames[fType], NodeTypeNames[sType]);

        switch (fType) {
            default:
                if (printDebug)
                    printf("isCompatible default called with types %s and %s\n",
                           NodeTypeNames[fType], NodeTypeNames[sType]);
                return 0;
                break;

            case NODE_LITERAL_REAL:
                if (sType == NODE_KW_FLOAT)
                    vReturn = 1;
            case NODE_LITERAL_INT:
                if ((sType == NODE_KW_INT) || (sType == NODE_KW_CHAR))
                    vReturn = 1;
                break;
            case NODE_LITERAL_CHAR:
                if ((sType == NODE_KW_INT) || (sType == NODE_KW_CHAR))
                    vReturn = 1;
                break;

            case NODE_ADDITION:
            case NODE_SUBTRACTION:
            case NODE_MULTIPLICATION:
            case NODE_DIVISION:
                switch (sType) {
                    case NODE_KW_INT:
                    case NODE_KW_FLOAT:
                    case NODE_KW_CHAR:
                        vReturn = 1;
                        break;
                    default:
                        vReturn = 0;
                        break;
                }

                break;
            case NODE_LESS_THAN:
            case NODE_GREATER_THAN:
            case NODE_LOGICAL_OR:
            case NODE_LOGICAL_AND:
            case NODE_LESS_THAN_EQUAL:
            case NODE_GREATER_THAN_EQUAL:
            case NODE_EQUAL:
            case NODE_NOT_EQUAL:
                switch (sType) {
                    case NODE_KW_INT:
                    case NODE_KW_FLOAT:
                    case NODE_KW_CHAR:
                    case NODE_KW_BOOL:
                        vReturn = 1;
                        break;
                    default:
                        vReturn = 0;
                        break;
                }

            case NODE_LITERAL_TRUE:
            case NODE_LITERAL_FALSE:
            case NODE_KW_BOOL:
                vReturn =
                    ((sType == NODE_LITERAL_FALSE) || (sType == NODE_KW_BOOL) ||
                     (sType == NODE_LITERAL_TRUE));

                break;

            case NODE_LITERAL_STRING:
                vReturn = 0;
                break;

            case NODE_VECTOR_TK:
                vReturn =
                    ((sType == NODE_VECTOR_INT) || (sType == NODE_VECTOR_TK) ||
                     (sType == NODE_VECTOR_TK) || (sType == NODE_KW_CHAR));

                break;
            case NODE_VECTOR_INT:
                vReturn =
                    ((sType == NODE_VECTOR_INT) || (sType == NODE_VECTOR_TK) ||
                     (sType == NODE_KW_CHAR));
                break;

            case NODE_KW_INT:
                vReturn =
                    ((sType == NODE_KW_CHAR) || (sType == NODE_VECTOR_TK) ||
                     (sType == NODE_VECTOR_INT));
                break;
            case NODE_KW_FLOAT:
                vReturn = (sType == NODE_LITERAL_REAL);
                // Float is only compatible with float
                vReturn = 0;
                break;
            case NODE_KW_CHAR:
                if (sType == NODE_KW_INT)
                    vReturn = 1;
                break;
        }
        if (printDebug)
            printf("Returning %d, %s\n", vReturn, vReturn ? "true" : "false");
    }
}
NodeType getType(ASTNode *node) {
    if (isLitType(node->astNodeType)) {
        // printf("Chegou aqui com node %s\n",
        // NodeTypeNames[node->astNodeType]);
        if ((node->astNodeType == NODE_LITERAL_FALSE) ||
            (node->astNodeType == NODE_LITERAL_TRUE)) {
            return NODE_KW_BOOL;
        } else
            return node->hashNode->nature;
    } else if (isExpression(node))
        return getExpressionType(node);

    switch (node->astNodeType) {
        default:
            printf("getType defaulted with type %s\n",
                   NodeTypeNames[node->astNodeType]);
            return node->astNodeType;
        case NODE_KW_RETURN:
            return getType(node->children[0]);
        case NODE_KW_INT:
            return NODE_KW_INT;
        case NODE_KW_CHAR:
            return NODE_KW_CHAR;
        case NODE_KW_FLOAT:
            return NODE_KW_FLOAT;
        case NODE_KW_BOOL:
            return NODE_KW_BOOL;

        case NODE_PARAM:
            return getType(node->children[0]);

        case NODE_LITERAL_INT:
            return NODE_KW_INT;
        case NODE_LITERAL_CHAR:
            return NODE_KW_CHAR;
        case NODE_LITERAL_FALSE:
        case NODE_LITERAL_TRUE:
            return NODE_KW_BOOL;
        case NODE_LITERAL_REAL:
            return NODE_KW_FLOAT;

        case NODE_VECTOR_TK:
        case NODE_VECTOR_INT:
            return node->children[0]->hashNode->nature;
        case NODE_FUNC_CALL:
        case NODE_FUNC_CALL_EMPTY:
            return node->children[0]->hashNode->nature;
        case NODE_TOKEN_IDENTIFIER:
            return node->hashNode->nature;
        case NODE_PARENTHESIS_EXPRESSION:
            printf("chegou aqui! children = ");
            printNode(node->children[0], 0);
            printf("\n");
            return getType(node->children[0]);
    }
}
