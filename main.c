/* Compiladores 2024/01 - Etapa 2
 * João César de Paula Criscolo  - 00304342
 * Prof. Marcelo Johann
 */

#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"

//lex.yy.h
int yylex();
extern char *yytext;
extern FILE *yyin;

int isRunning(void);
void initMe(void);

int main(int argc, char** argv) {
  hashInit();
  if (argc) 
      yyin = fopen(argv[1], "r");
  printf("mds");
  yyparse();
  hashPrint();
  printf("Printa\n");
  printf("Printa ok \n");
  printf("Parse OK\n");
  printAST(root);
  printf("Compilação OK\n");

  exit(0);
  }

