#
# UFRGS - Compiladores B - Marcelo Johann - 2009/2 - Etapa 1
#
# Makefile for single compiler call
# All source files must be included from code embedded in scanner.l
# In our case, you probably need #include "hash.c" at the beginning
# and #include "main.c" in the last part of the scanner.l
#

etapa5: lex.yy.c
	yacc -v -d parser.y 
	gcc -o etapa5 lex.yy.c -v -g

lex.yy.c: scanner.l
	lex scanner.l 

clean:
	rm -rf lex.yy.c etapa5 y.tab.h y.tab.c ast.o y.output etapa5.dSYM/ *.o

remake: clean etapa5
