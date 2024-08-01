#ifndef HASH_HEADER
#define HASH_HEADER

#include <stdio.h>
#define HASH_SIZE 997

typedef enum {
    SYMBOL_VAR,
    SYMBOL_LABEL,
} SymbolTypes;
typedef struct hash_node {
    int hashNodeType;
    int nature;
    char *text;
    struct hash_node *next;
} HASH_NODE;

void hashInit(void);
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(int hashNodeType, char *text);
void hashPrint(void);
void hashPrintNatures();
HASH_NODE *makeTemp(void);
HASH_NODE *makeLabel(void);
#endif
