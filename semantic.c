#include "semantic.h"

#include "hash.h"
#include "y.tab.h"
#include <stdlib.h>

/*
 * declarations and usage:
 *   arrays with index
 *   function with param
 *   comparing types on assignment
 *   comparing param/assingment list
 *   missing return type
 */
int SemanticErrors = 0;

typedef enum {
    DECLARED_VAR,
    DECLARED_VECTOR,
    DECLARED_FUNC,
    DECLARED_FUNC_PARAMETER,
} SEMANTIC_NATURE;

// Ha quatro tipos de dados para declarações: char, int, float, bool
typedef enum { INT, BOOL, CHAR, FLOAT } SEMANTIC_TYPE;

void check_semantic(ASTNode *root) {
    check_and_set_declarations(root);
    check_undeclared_variables(root);
    check_operand(root);
    check_usage(root);
    checkNodes(root, root);
    if (SemanticErrors) {
        printf("Total of %d semantic errors\n", SemanticErrors);
        exit(4);
    }
}

int isDeclaration(NodeType type) {
    NodeType lists[] = {
        NODE_FUNC_DECLARATION,   NODE_FUNC_DECLARATION_EMPTY,
        NODE_VECTOR_DECLARATION, NODE_VECTOR_DECLARATION_AND_ASIGN,
        NODE_VAR_DECLARATION,    NODE_PARAM,
    };
    int s = 6;
    for (int i = 0; i < s; i++)
        if (lists[i] == type)
            return 1;
    return 0;
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
        printf(
            "SEMANTIC ERROR: %s call must have %d parameters. Called with %d\n",
            tkid->text, qttDef, qttCal);
    }

    checkFuncParamType(node->children[1], declaration->children[2],
                       declaration->children[1]->hashNode, 0);
    // Check if the type of each list element is compatible with definition
    // checkListType(node->children[1], declaration->children[0]->astNodeType,
    // 0, declaration->children[1]->hashNode);
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
    int printDebug = 0;
    if (node == NULL) {
        if (printDebug)
            printf("checkListTyp NULL node\n");
        return 1;
    }

    if (node->children[1]) { // Se nao final da lista, compara e chama
        int nodeType;
        if (node->children[0]->astNodeType == NODE_TOKEN_IDENTIFIER) {
            nodeType = node->children[0]->hashNode->nature;
            if (printDebug)
                printf("checking type for %s[%d]. comparing %s of type %s with "
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
        return (isCompatible(node->children[0]->hashNode->nature, type));
        /*
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
        */
    }
}

void checkNodes(ASTNode *node, ASTNode *root) {
    // Check all function calls for correct usage;
    int i;
    if (node == NULL)
        return;
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
        if (!isCompatible(getType(node->children[0]),
                          getType(node->children[1]))) {
            printf("SEMANTIC ERROR: trying to assign ");
            printNode(node->children[0], 0);
            printf(" = ");
            printNode(node->children[1], 0);
            printf(" of type %s and type %s which are not compatible\n",
                   NodeTypeNames[getType(node->children[0])],
                   NodeTypeNames[getType(node->children[1])]);
            SemanticErrors++;
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
                          (node->astNodeType == NODE_VECTOR_TK)))
                        printf("SEMANTIC ERROR: %s is an array and can only be "
                               "used with indexes\n",
                               child->hashNode->text);
                    SemanticErrors++;
                }

                // Check if function is called with parameters
            }
            checkNodes(node->children[i], root);
        }
    }
}
ASTNode *checkFunctionReturn(ASTNode *node, ASTNode *funcRoot, NodeType type) {
    // node: First call: NODE_FUNC_DECLARATION or NODE_FUNC_DECLARATION_EMPTY
    // Return: the node of the return of the function declared in node
    if (node == NULL) {
        return NULL;
    }

    if (node->astNodeType == NODE_KW_RETURN) {
        if (!(isCompatible(getType(node->children[0]), type))) {
            printf("SEMANTIC ERROR: func %s should return %s. Got %s instead\n",
                   funcRoot->children[1]->hashNode->text,
                   NodeTypeNames[funcRoot->children[0]->astNodeType],
                   NodeTypeNames[getType(node->children[0])]);
            SemanticErrors++;
        }
    }

    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (node->children[i]) {
            ASTNode *funcReturn =
                checkFunctionReturn(node->children[i], funcRoot, type);
        }

        return NULL;
    }
}

NodeType getType(ASTNode *node) {
    switch (node->astNodeType) {
        default:
            return node->astNodeType;
        case NODE_PARAM:
            return getType(node->children[0]);

        case NODE_VECTOR_TK:
        case NODE_VECTOR_INT:
            return node->children[0]->hashNode->nature;
        case NODE_FUNC_CALL:
        case NODE_FUNC_CALL_EMPTY:
            return node->children[0]->hashNode->nature;
        case NODE_TOKEN_IDENTIFIER:
            return node->hashNode->nature;
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
int isNumberType(NodeType type) {
    if ((type == NODE_MULTIPLICATION) || (type == NODE_DIVISION) ||
        (type == NODE_ADDITION) || (type == NODE_SUBTRACTION) ||
        (type == NODE_KW_INT) || (type == NODE_LITERAL_INT) ||
        // char e int sao intercambiaveis
        (type == NODE_KW_CHAR) || (type == NODE_LITERAL_CHAR))
        return 1;
    else
        return 0;
}

int isNumber(ASTNode *node) {
    if ((node->astNodeType == NODE_MULTIPLICATION) ||
        (node->astNodeType == NODE_DIVISION) ||
        (node->astNodeType == NODE_ADDITION) ||
        (node->astNodeType == NODE_KW_INT) ||
        (node->astNodeType == NODE_LITERAL_INT) ||
        ((node->astNodeType == NODE_TOKEN_IDENTIFIER) &&
         (node->hashNode->nature == NODE_KW_INT)) ||
        (node->astNodeType == NODE_SUBTRACTION) ||
        ((node->astNodeType == NODE_FUNC_CALL) &&
         (node->children[0]->hashNode->nature == NODE_KW_INT)) ||

        // char e int sao intercambiaveis
        ((node->astNodeType == NODE_TOKEN_IDENTIFIER) &&
         (node->hashNode->nature == NODE_KW_CHAR)) ||
        (node->astNodeType == NODE_KW_CHAR) ||
        (node->astNodeType == NODE_LITERAL_CHAR))
        return 1;
    else
        return 0;
}

void check_usage(ASTNode *node) {
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

    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (node->children[i])
            check_usage(node->children[i]);
        else
            break;
    }
}

int isCompatible(NodeType fType, NodeType sType) {
    if (fType == sType) {
        return 1;
    }
    if (isNumberType(fType) && isNumberType(sType)) {
        return 1;
    }
    int printDebug = 0;
    switch (fType) {
        default:
            if (printDebug)
                printf("isCompatible default called with types %s and %s\n",
                       NodeTypeNames[fType], NodeTypeNames[sType]);
            return 0;
            break;
        case NODE_KW_BOOL:
        case NODE_LITERAL_STRING:
            return 0;
            break;
        case NODE_VECTOR_TK:
        case NODE_VECTOR_INT:
            if ((sType == NODE_VECTOR_INT) || (sType == NODE_VECTOR_TK) ||
                (sType == NODE_KW_CHAR))
                return 1;

        case NODE_KW_INT:
            if ((sType == NODE_KW_CHAR) || (sType == NODE_VECTOR_TK) ||
                (sType == NODE_VECTOR_INT))
                return 1;
            else
                return 0;
            break;
        case NODE_KW_FLOAT:
            // Float is only compatible with float
            return 0;
            break;
        case NODE_KW_CHAR:
            if (sType == NODE_KW_INT)
                return 1;
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

void check_operand(ASTNode *node) {
    int i;

    if (node == NULL)
        return;
    char *op;
    switch (node->astNodeType) {
        default:
            break;
        case NODE_ADDITION:
            printOpError(node, '+');
            break;
        case NODE_SUBTRACTION:
            printOpError(node, '-');
            break;
        case NODE_DIVISION:
            printOpError(node, '/');
            break;
        case NODE_MULTIPLICATION:
            printOpError(node, '*');
            break;
    }
    for (int i = 0; i < MAX_CHILDREN; i++) {
        check_operand(node->children[i]);
    }
}

void printOpError(ASTNode *node, char op) {
    if (!(isNumber(node->children[0]) && isNumber(node->children[1]))) {
        printf("SEMANTIC ERROR: %s %c %s error. Invalid operands\n",
               node->children[0]->hashNode->text, op,
               node->children[1]->hashNode->text);
        printIdentifiersTypeNature(node->children[0], node->children[1]);
        SemanticErrors++;
    }
}
void printIdentifiersTypeNature(ASTNode *node, ASTNode *otherNode) {
    printf("    Identifiers: %s , %s \n", node->hashNode->text,
           otherNode->hashNode->text);
    printf("    astNode type: %s , %s \n", NodeTypeNames[node->astNodeType],
           NodeTypeNames[otherNode->astNodeType]);

    printf("    Natures: %s , %s\n", NodeTypeNames[node->hashNode->nature],
           NodeTypeNames[otherNode->hashNode->nature]);
}
void check_variable_usage(ASTNode *node) {
    if (node == NULL)
        return;
    switch (node->astNodeType) {
        case NODE_ASSIGNMENT:
            // Check left side
            if (node->children[1]->astNodeType == NODE_TOKEN_IDENTIFIER) {
                HASH_NODE *symbol = hashFind(node->children[0]->hashNode->text);
                if (symbol->nature != NODE_VAR_DECLARATION) {
                    printf("SEMANTIC ERROR: %s is not a scalar\n",
                           node->children[0]->hashNode->text);
                    SemanticErrors++;
                }
            }
            // Check right side
            check_expression(node->children[1]);
            break;
        case NODE_VECTOR_INT:
        case NODE_VECTOR_TK:
            // Check vector usage
            if (node->children[0]->astNodeType == NODE_TOKEN_IDENTIFIER) {
                HASH_NODE *symbol = hashFind(node->children[0]->hashNode->text);
                if (symbol->nature != NODE_VECTOR_DECLARATION) {
                    printf("SEMANTIC ERROR: %s is not a vector\n",
                           node->children[0]->hashNode->text);
                    SemanticErrors++;
                }
            }
            check_expression(node->children[1]); // Check index
            break;
        case NODE_FUNC_CALL:
            // Check function usage
            if (node->children[0]->astNodeType == NODE_TOKEN_IDENTIFIER) {
                HASH_NODE *symbol = hashFind(node->children[0]->hashNode->text);
                if (symbol->nature != NODE_FUNC_DECLARATION) {
                    printf("SEMANTIC ERROR: %s is not a function\n",
                           node->children[0]->hashNode->text);
                    SemanticErrors++;
                }
            }
            check_arguments(node->children[1]); // Check arguments
            break;
        default:
            break;
    }
    for (int i = 0; i < MAX_CHILDREN; i++) {
        check_variable_usage(node->children[i]);
    }
}

void check_expression(ASTNode *node) {
    if (node == NULL)
        return;
    // Verifique o tipo de dados nas expressões aqui
    // ...
}

void check_arguments(ASTNode *node) {
    if (node == NULL)
        return;
    // Verifique os argumentos das chamadas de função aqui
    // ...
}
void semantic_check(ASTNode *root) {
    check_and_set_declarations(root);
    check_undeclared_variables(root);
    check_variable_usage(root);
    // Adicione mais verificações conforme necessário
}

void check_undeclared() {}
