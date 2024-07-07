%{
#include "ast.h"
#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode* root;
%}

%union {
    ASTNode* astnode;
    HASH_NODE* symbol;
}

%type <astnode> programa lista_declaracoes declaracao declaracao_variavel declaracao_vetor declaracao_funcao
%type <astnode> lista_parametros parametro tipo valor_inicial valores_iniciais bloco lista_comandos
%type <astnode> comando atribuicao vetor controle_fluxo expressao chamada_funcao lista_chamada

%token KW_CHAR           
%token KW_INT            
%token KW_FLOAT          
%token KW_BOOL           
%token KW_IF             
%token KW_ELSE           
%token KW_WHILE          
%token KW_READ           
%token KW_PRINT          
%token KW_RETURN         
%token OPERATOR_LE       
%token OPERATOR_GE       
%token OPERATOR_EQ       
%token OPERATOR_DIF      

%token <symbol> TK_IDENTIFIER     
%token <symbol> LIT_INT           
%token <symbol> LIT_CHAR          
%token <symbol> LIT_REAL          
%token <symbol> LIT_FALSE         
%token <symbol> LIT_TRUE          
%token <symbol> LIT_STRING        
%token <symbol> TOKEN_ERROR       

%left '+' '-'
%left '*' '/'
%left '<' '>' OPERATOR_DIF OPERATOR_EQ OPERATOR_GE OPERATOR_LE
%left '&' '|'
%left KW_IF
%right LOWER_THAN_ELSE ELSE

%start programa
%%

programa: lista_declaracoes { root = createNode(NODE_PROGRAM, (ASTNode*[]) {$1, NULL}, NULL); }
        ;

lista_declaracoes: declaracao { $$ = createNode(NODE_DECLARATIONS_LIST, (ASTNode*[]){$1, NULL}, NULL); }
                | lista_declaracoes declaracao { $$ = createNode(NODE_DECLARATIONS_LIST, (ASTNode*[]){$1, $2, NULL}, NULL); }
                ;

declaracao: declaracao_variavel ';' { $$ = $1; }
          | declaracao_vetor ';' { $$ = $1; }
          | declaracao_funcao { $$ = $1; }
          ;

declaracao_variavel: tipo TK_IDENTIFIER ':' valor_inicial { $$ = createNode(NODE_VAR_DECLARATION, (ASTNode*[]){$1, createNode(NODE_TYPE, NULL, $2), $4, NULL}, NULL); }
                   ;

declaracao_vetor: tipo TK_IDENTIFIER '[' LIT_INT ']' ':' valores_iniciais { $$ = createNode(NODE_VECTOR_DECLARATION, (ASTNode*[]){$1, createNode(NODE_TYPE, NULL, $2), $4, $7, NULL}, NULL); }
                | tipo TK_IDENTIFIER '[' LIT_INT ']' { $$ = createNode(NODE_VECTOR_DECLARATION, (ASTNode*[]){$1, createNode(NODE_TYPE, NULL, $2), $4, NULL}, NULL); }
                ;

declaracao_funcao: tipo TK_IDENTIFIER '(' lista_parametros ')' bloco { $$ = createNode(NODE_FUNC_DECLARATION, (ASTNode*[]){$1, createNode(NODE_TYPE, NULL, $2), $4, $6, NULL}, NULL); }
                 ;

lista_parametros: /* vazio */ { $$ = NULL; }
                | parametro ',' lista_parametros { $$ = createNode(NODE_PARAM_LIST, (ASTNode*[]){$1, $3, NULL}, NULL); }
                | parametro { $$ = createNode(NODE_PARAM_LIST, (ASTNode*[]){$1, NULL}, NULL); }
                ;

parametro: tipo TK_IDENTIFIER { $$ = createNode(NODE_PARAM, (ASTNode*[]){$1, createNode(NODE_TYPE, NULL, $2), NULL}, NULL); }
         ;

tipo: KW_CHAR { $$ = createNode(NODE_TYPE, NULL, NULL); }
    | KW_INT { $$ = createNode(NODE_TYPE, NULL, NULL); }
    | KW_FLOAT { $$ = createNode(NODE_TYPE, NULL, NULL); }
    | KW_BOOL { $$ = createNode(NODE_TYPE, NULL, NULL); }
    ;

valor_inicial: LIT_INT { $$ = createNode(NODE_INITIAL_VALUE, NULL, $1); }
             | LIT_CHAR { $$ = createNode(NODE_INITIAL_VALUE, NULL, $1); }
             | LIT_REAL { $$ = createNode(NODE_INITIAL_VALUE, NULL, $1); }
             | LIT_FALSE { $$ = createNode(NODE_INITIAL_VALUE, NULL, "false"); }
             | LIT_TRUE { $$ = createNode(NODE_INITIAL_VALUE, NULL, "true"); }
             | LIT_STRING { $$ = createNode(NODE_INITIAL_VALUE, NULL, $1); }
             ;

valores_iniciais: valor_inicial { $$ = createNode(NODE_VALUES_LIST, (ASTNode*[]){$1, NULL}, NULL); }
                | valores_iniciais valor_inicial { $$ = createNode(NODE_VALUES_LIST, (ASTNode*[]){$1, $2, NULL}, NULL); }
                ;

bloco: '{' lista_comandos '}' { $$ = createNode(NODE_BLOCK, (ASTNode*[]){$2, NULL}, NULL); }
     ;

lista_comandos: /* vazio */ { $$ = NULL; }
              | lista_comandos comando { $$ = createNode(NODE_COMMANDS_LIST, (ASTNode*[]){$1, $2, NULL}, NULL); }
              ;

comando: atribuicao { $$ = $1; }
       | controle_fluxo { $$ = $1; }
       | KW_READ tipo TK_IDENTIFIER ';' { $$ = createNode(NODE_COMMAND, (ASTNode*[]){$2, createNode(NODE_TYPE, NULL, $3), NULL}, NULL); }
       | KW_PRINT tipo expressao ';' { $$ = createNode(NODE_COMMAND, (ASTNode*[]){$2, $3, NULL}, NULL); }
       | KW_PRINT LIT_STRING ';' { $$ = createNode(NODE_COMMAND, NULL, $2); }
       | KW_RETURN expressao ';' { $$ = createNode(NODE_COMMAND, (ASTNode*[]){$2, NULL}, NULL); }
       | bloco { $$ = $1; }
       | ';' { $$ = NULL; }
       ;

atribuicao: TK_IDENTIFIER '=' expressao ';' { $$ = createNode(NODE_ASSIGNMENT, (ASTNode*[]){createNode(NODE_TYPE, NULL, $1), $3, NULL}, NULL); }
          | vetor '=' expressao ';' { $$ = createNode(NODE_ASSIGNMENT, (ASTNode*[]){$1, $3, NULL}, NULL); }
          ;

vetor: TK_IDENTIFIER '[' LIT_INT ']' { $$ = createNode(NODE_VECTOR, (ASTNode*[]){createNode(NODE_TYPE, NULL, $1), createNode(NODE_INITIAL_VALUE, NULL, $3), NULL}, NULL); }
     | TK_IDENTIFIER '[' TK_IDENTIFIER ']' { $$ = createNode(NODE_VECTOR, (ASTNode*[]){createNode(NODE_TYPE, NULL, $1), createNode(NODE_TYPE, NULL, $3), NULL}, NULL); }
     ;

controle_fluxo: KW_IF '(' expressao ')' comando %prec LOWER_THAN_ELSE { $$ = createNode(NODE_CONTROL_FLOW, (ASTNode*[]){$3, $5, NULL}, NULL); }
              | KW_IF '(' expressao ')' comando KW_ELSE comando %prec ELSE { $$ = createNode(NODE_CONTROL_FLOW, (ASTNode*[]){$3, $5, $7, NULL}, NULL); }
              | KW_WHILE '(' expressao ')' comando { $$ = createNode(NODE_CONTROL_FLOW, (ASTNode*[]){$3, $5, NULL}, NULL); }
              ;

expressao: expressao '+' expressao { $$ = createNode(NODE_EXPRESSION, (ASTNode*[]){$1, $3, NULL}, "+"); }
         | expressao '-' expressao { $$ = createNode(NODE_EXPRESSION, (ASTNode*[]){$1, $3, NULL}, "-"); }
         | expressao '*' expressao { $$ = createNode(NODE_EXPRESSION, (ASTNode*[]){$1, $3, NULL}, "*"); }
         | expressao '/' expressao { $$ = createNode(NODE_EXPRESSION, (ASTNode*[]){$1, $3, NULL}, "/"); }
         | expressao '<' expressao { $$ = createNode(NODE_EXPRESSION, (ASTNode*[]){$1, $3, NULL}, "<"); }
         | expressao '>' expressao { $$ = createNode(NODE_EXPRESSION, (ASTNode*[]){$1, $3, NULL}, ">"); }
         | expressao '|' expressao { $$ = createNode(NODE_EXPRESSION, (ASTNode*[]){$1, $3, NULL}, "|"); }
         | expressao '&' expressao { $$ = createNode(NODE_EXPRESSION, (ASTNode*[]){$1, $3, NULL}, "&"); }
         | expressao OPERATOR_LE expressao { $$ = createNode(NODE_EXPRESSION, (ASTNode*[]){$1, $3, NULL}, "<="); }
         | expressao OPERATOR_GE expressao { $$ = createNode(NODE_EXPRESSION, (ASTNode*[]){$1, $3, NULL}, ">="); } 
         | expressao OPERATOR_EQ expressao { $$ = createNode(NODE_EXPRESSION, (ASTNode*[]){$1, $3, NULL}, "=="); }
         | expressao OPERATOR_DIF expressao { $$ = createNode(NODE_EXPRESSION, (ASTNode*[]){$1, $3, NULL}, "!="); }
         | '(' expressao ')' { $$ = $2;  }
         | TK_IDENTIFIER { $$ = createNode(NODE_EXPRESSION, NULL, $1); }
         | vetor { $$ = $1; }
         | LIT_INT { $$ = createNode(NODE_EXPRESSION, NULL, $1); }
         | LIT_CHAR { $$ = createNode(NODE_EXPRESSION, NULL, $1); }
         | LIT_REAL { $$ = createNode(NODE_EXPRESSION, NULL, $1); }
         | LIT_FALSE { $$ = createNode(NODE_EXPRESSION, NULL, "false"); }
         | LIT_TRUE { $$ = createNode(NODE_EXPRESSION, NULL, "true"); }
         | chamada_funcao { $$ = $1; }
         ;

chamada_funcao: TK_IDENTIFIER '(' lista_chamada ')' { $$ = createNode(NODE_FUNC_CALL, (ASTNode*[]){createNode(NODE_TYPE, NULL, $1), $3, NULL}, NULL); }
              ;

lista_chamada: /* empty */ { $$ = NULL; }
             | expressao ',' lista_chamada { $$ = createNode(NODE_ARGS_LIST, (ASTNode*[]){$1, $3, NULL}, NULL); }
             | expressao { $$ = createNode(NODE_ARGS_LIST, (ASTNode*[]){$1, NULL}, NULL); }
             ;
%% 
int yyerror() {
  fprintf(stderr,"Syntax error in line %d\n", getLineNumber());
  exit(3);
  return 1;
}
