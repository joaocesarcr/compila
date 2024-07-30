/* Compiladores 2024/01 - Etapa 3
 * João César de Paula Criscolo  - 00304342
 * Prof. Marcelo Johann
 */

#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>

// lex.yy.h
int yylex();
extern char *yytext;
extern FILE *yyin;

int isRunning(void);
void initMe(void);

int main(int argc, char **argv) {
    hashInit();
    if (argc)
        yyin = fopen(argv[1], "r");
    yyparse();
    // hashPrint();
    printTreeOLD(root, 0);

    exit(0);
}
