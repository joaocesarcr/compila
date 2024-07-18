%x multiLineComment


%{
#include <stdio.h>

int yylex(void);
int yyerror(char* s);
#include "hash.h"
#include "ast.h"
#include "semantic.c"
#include "y.tab.h"
int running = 1;
int lineNumber = 1;
%}

%%

"/*"                        {BEGIN(multiLineComment);}
<multiLineComment>"*/"      {BEGIN(INITIAL);}
<multiLineComment>\n        {lineNumber++;}
<multiLineComment>.         {}

\n                          {lineNumber++;} 
"//".*                                  

","                         { return yytext[0]; }
";"                         { return yytext[0]; }
":"                         { return yytext[0]; }
"("                         { return yytext[0]; }
")"                         { return yytext[0]; }
"["                         { return yytext[0]; }
"]"                         { return yytext[0]; }
"{"                         { return yytext[0]; }
"}"                         { return yytext[0]; }
"="                         { return yytext[0]; }
"+"                         { return yytext[0]; }
"-"                         { return yytext[0]; }
"*"                         { return yytext[0]; }
"/"                         { return yytext[0]; }
"%"                         { return yytext[0]; }
"<"                         { return yytext[0]; }
">"                         { return yytext[0]; }
"&"                         { return yytext[0]; }
"|"                         { return yytext[0]; }
"~"                         { return yytext[0]; }
"."                         { return yytext[0]; }

"<="                        { return OPERATOR_LE; }
">="                        { return OPERATOR_GE; }
"=="                        { return OPERATOR_EQ; }
"!="                        { return OPERATOR_DIF; }

"bool"                      { return KW_BOOL; }
"char"                      { return KW_CHAR; }
"int"                       { return KW_INT; }
"float"                     { return KW_FLOAT; }
"if"                        { return KW_IF; }
"else"                      { return KW_ELSE; }
"while"                     { return KW_WHILE; }
"read"                      { return KW_READ; }
"print"                     { return KW_PRINT; }
"return"                    { return KW_RETURN; }
"true"                      { return LIT_TRUE; }
"false"                     { return LIT_FALSE; }

[a-zA-Z_][a-zA-Z0-9_]*      { 
                              HASH_NODE* entry = hashInsert(TK_IDENTIFIER, yytext);
                              yylval.hash_node = entry;
                              return TK_IDENTIFIER;
                            }

[0-9]+                      { 
                              HASH_NODE* entry = hashInsert(LIT_INT, yytext);
                              yylval.hash_node = entry;
                              return LIT_INT; 
                            }
'[^'\n]'                    { 
                              HASH_NODE* entry = hashInsert(LIT_CHAR, yytext);
                              yylval.hash_node = entry;
                              return LIT_CHAR; 
                            }

[0-9]+\.[0-9]+              { 
                              HASH_NODE* entry = hashInsert(LIT_REAL, yytext);
                              yylval.hash_node = entry; 
                              return LIT_REAL; 
                            }
[0-9]+\.                    {
                              HASH_NODE* entry = hashInsert(LIT_REAL, yytext);
                              yylval.hash_node = entry;
                              return LIT_REAL; 
                            }
\.[0-9]+                    {
                              HASH_NODE* entry = hashInsert(LIT_REAL, yytext);
                              yylval.hash_node = entry;  
                              return LIT_REAL; 
                            }
\"([^"\n]|\\\")*\"          { 
                              HASH_NODE* entry = hashInsert(LIT_STRING, yytext);
                              yylval.hash_node = entry;
                              return LIT_STRING; 
                            }

[ \t]                       {}

.                           { return TOKEN_ERROR; }

%%
void initMe(void) {
    hashInit();
}
int getLineNumber() {
    return lineNumber;
}

int yywrap() {
    running = 0;
    return 1;
}

int isRunning() {
    return running;
}
#include "y.tab.c"
#include "main.c"
#include "hash.c"
#include "ast.c"
