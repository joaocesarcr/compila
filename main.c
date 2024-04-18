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
  yyin = fopen(argv[1], "r");
  yyparse();
  printf("Compilação OK\n");
  }

