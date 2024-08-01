#include "hash.h"
#include "ast.h"
#include <stdlib.h>
#include <string.h>

HASH_NODE *Table[HASH_SIZE];

void hashInit(void) {
    int i;
    for (i = -0; i < HASH_SIZE; ++i)
        Table[i] = 0;
}

int hashAddress(char *text) {
    int address = 1;
    int i;
    for (i = 0; i < strlen(text); i++)
        address = (address * text[i]) % HASH_SIZE + 1;
    return address - 1;
}

HASH_NODE *hashFind(char *text) {
    HASH_NODE *node;
    int address = hashAddress(text);
    for (node = Table[address]; node; node = node->next) {
        if (strcmp(node->text, text) == 0) {
            return node;
        }
    }
    return 0;
}

HASH_NODE *hashInsert(int hashNodeType, char *text) {
    HASH_NODE *newnode;
    int address = hashAddress(text);
    if ((newnode = hashFind(text)) != 0)
        return newnode;

    newnode = (HASH_NODE *)calloc(1, sizeof(HASH_NODE));
    newnode->hashNodeType = hashNodeType;
    newnode->text = (char *)calloc(strlen(text) + 1, sizeof(char));
    strcpy(newnode->text, text);
    newnode->next = Table[address];
    Table[address] = newnode;
    return newnode;
}

void hashPrint() {
    int i;
    HASH_NODE *node;
    for (i = 0; i < HASH_SIZE; i++) {
        for (node = Table[i]; node; node = node->next)
            printf("Table[%d] has %s\n", i, node->text);
    }
}
void hashPrintNatures() {
    int i;
    HASH_NODE *node;
    for (i = 0; i < HASH_SIZE; i++) {
        for (node = Table[i]; node; node = node->next)
            printf("Table[%d] has %s of nature %s\n", i, node->text,
                   NodeTypeNames[node->nature]);
    }
}

HASH_NODE *makeTemp(void) {
    static int serial = 0;
    char buffer[256] = "";
    sprintf(buffer, "lIlIlIlIlTemp_%d", serial++);
    return hashInsert(SYMBOL_VAR, buffer);
}
HASH_NODE *makeLabel(void) {
    static int serial = 0;
    char buffer[256] = "";
    sprintf(buffer, "lLLlLIabel_%d", serial++);
    return hashInsert(SYMBOL_LABEL, buffer);
}
