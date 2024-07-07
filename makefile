#
# UFRGS - Compiladores B - Marcelo Johann - 2009/2 - Etapa 1
#
# Makefile for single compiler call
# All source files must be included from code embedded in scanner.l
# In our case, you probably need #include "hash.c" at the beginning
# and #include "main.c" in the last part of the scanner.l
#

etapa2: lex.yy.c
	yacc -v -d parser.y 
	gcc -o etapa2 lex.yy.c ast.c hash.c

lex.yy.c: scanner.l
	lex scanner.l

clean:
	rm lex.yy.c etapa2 y.tab.h y.tab.c 

remake: clean etapa2
