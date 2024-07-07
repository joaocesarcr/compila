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
    int token;
}

%type <astnode> programa lista_declaracoes declaracao declaracao_variavel declaracao_vetor declaracao_funcao
%type <astnode> lista_parametros parametro tipo valor_inicial valores_iniciais bloco lista_comandos
%type <astnode> comando atribuicao vetor controle_fluxo expressao chamada_funcao lista_chamada

%token <token> KW_CHAR           
%token <token> KW_INT            
%token <token> KW_FLOAT          
%token <token> KW_BOOL           
%token <token> KW_IF             
%token <token> KW_ELSE           
%token <token> KW_WHILE          
%token <token> KW_READ           
%token <token> KW_PRINT          
%token <token> KW_RETURN         
%token <token> OPERATOR_LE       
%token <token> OPERATOR_GE       
%token <token> OPERATOR_EQ       
%token <token> OPERATOR_DIF      
%token <astnode> ','
%token <astnode> ';'
%token <astnode> ':'
%token <astnode> '('
%token <astnode> ')'
%token <astnode> '['
%token <astnode> ']'
%token <astnode> '{'
%token <astnode> '}'
%token <astnode> '='
%token <astnode> '+'
%token <astnode> '-'
%token <astnode> '*'
%token <astnode> '/'
%token <astnode> '%'
%token <astnode> '<'
%token <astnode> '>'
%token <astnode> '&'
%token <astnode> '|'
%token <astnode> '~'
%token <astnode> '.'

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


programa: lista_declaracoes { root = createNode(NODE_PROGRAM, (ASTNode*[]){$1, NULL}, (HASH_NODE*) NULL); }
        ;

lista_declaracoes: declaracao { $$ = createNode(NODE_DECLARATIONS_LIST, (ASTNode*[]){$1, NULL}, NULL); }
                | lista_declaracoes declaracao { $$ = createNode(NODE_DECLARATIONS_LIST, (ASTNode*[]){$1, $2, NULL}, NULL); }
                ;

declaracao: declaracao_variavel ';' { $$ = $1; }
          | declaracao_vetor ';' { $$ = $1; }
          | declaracao_funcao { $$ = $1; }
          ;

declaracao_variavel: tipo TK_IDENTIFIER ':' valor_inicial { $$ = createNode(NODE_VAR_DECLARATION, (ASTNode*[]){$1, $3, NULL}, $2); }
                   ;

declaracao_vetor: tipo TK_IDENTIFIER '[' LIT_INT ']' ':' valores_iniciais { $$ = createNode(NODE_VECTOR_DECLARATION, (ASTNode*[]){$1, createNode(NODE_LITERAL_INT,NULL,$2),createNode(NODE_LITERAL_INT,NULL,$4), $6}, NULL); }
                | tipo TK_IDENTIFIER '[' LIT_INT ']' { $$ = createNode(NODE_VECTOR_DECLARATION, (ASTNode*[]){$1,  createNode(NODE_TOKEN_IDENTIFIER,NULL,$2),createNode(NODE_LITERAL_INT,NULL,$4), NULL}, NULL); }
                ;

declaracao_funcao: tipo TK_IDENTIFIER '(' lista_parametros ')' bloco { $$ = createNode(NODE_FUNC_DECLARATION, (ASTNode*[]){$1, $3, $5, NULL}, $2); }
                 ;

lista_parametros: /* vazio */ { $$ = NULL; }
                | parametro ',' lista_parametros { $$ = createNode(NODE_PARAM_LIST, (ASTNode*[]){$1, $3, NULL}, NULL); }
                | parametro { $$ = createNode(NODE_PARAM_LIST, (ASTNode*[]){$1, NULL}, NULL); }
                ;

parametro: tipo TK_IDENTIFIER { $$ = createNode(NODE_PARAM, (ASTNode*[]){$1, NULL}, $2); }
         ;

tipo: KW_CHAR { $$ = createNode(NODE_KW_CHAR, NULL, NULL); }
    | KW_INT { $$ = createNode(NODE_KW_INT, NULL, NULL); }
    | KW_FLOAT { $$ = createNode(NODE_KW_FLOAT, NULL, NULL); }
    | KW_BOOL { $$ = createNode(NODE_KW_BOOL, NULL, NULL); }
    ;

valor_inicial: LIT_INT { $$ = createNode(NODE_LITERAL_INT, NULL, $1); }
             | LIT_CHAR { $$ = createNode(NODE_LITERAL_CHAR, NULL, $1); }
             | LIT_REAL { $$ = createNode(NODE_LITERAL_CHAR, NULL, $1); }
             | LIT_FALSE { $$ = createNode(NODE_LITERAL_FALSE, NULL, $1); }
             | LIT_TRUE { $$ = createNode(NODE_LITERAL_TRUE, NULL, $1); }
             | LIT_STRING { $$ = createNode(NODE_LITERAL_STRING, NULL, $1); }
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
       | KW_READ tipo TK_IDENTIFIER ';' { $$ = createNode(NODE_KW_READ, (ASTNode*[]){$2, createNode(NODE_TOKEN_IDENTIFIER, NULL, $3), NULL}, NULL); }
       | KW_PRINT tipo expressao ';' { $$ = createNode(NODE_KW_PRINT, (ASTNode*[]){$2, $3, NULL}, NULL); }
       | KW_PRINT LIT_STRING ';' { $$ = createNode(NODE_KW_PRINT, NULL, NULL); }
       | KW_RETURN expressao ';' { $$ = createNode(NODE_KW_RETURN, (ASTNode*[]){$2, NULL}, NULL); }
       | bloco { $$ = $1; }
       | ';' { $$ = NULL; }
       ;

atribuicao: TK_IDENTIFIER '=' expressao ';' { $$ = createNode(NODE_ASSIGNMENT, (ASTNode*[]){createNode(NODE_TOKEN_IDENTIFIER, NULL, $1), $3, NULL}, NULL); }
          | vetor '=' expressao ';' { $$ = createNode(NODE_ASSIGNMENT, (ASTNode*[]){$1, $3, NULL},NULL); }
          ;

vetor: TK_IDENTIFIER '[' LIT_INT ']' { $$ = createNode(NODE_VECTOR, (ASTNode*[]){createNode(NODE_TYPE, NULL, $1), createNode(NODE_INITIAL_VALUE, NULL, $3), NULL}, NULL); }
     | TK_IDENTIFIER '[' TK_IDENTIFIER ']' { $$ = createNode(NODE_VECTOR, (ASTNode*[]){createNode(NODE_TYPE, NULL, $1), createNode(NODE_TYPE, NULL, $3), NULL}, NULL); }
     ;

controle_fluxo: KW_IF '(' expressao ')' comando { $$ = createNode(NODE_KW_IF, (ASTNode*[]){$3, $5, NULL},NULL); }
              | KW_IF '(' expressao ')' comando KW_ELSE comando { $$ = createNode(NODE_KW_IF_ELSE, (ASTNode*[]){$3, $5, $7, NULL}, NULL); }
              | KW_WHILE '(' expressao ')' comando { $$ = createNode(NODE_KW_WHILE, (ASTNode*[]){$3, $5, NULL}, NULL); }
              ;

expressao: expressao '+' expressao { $$ = createNode(NODE_ADDITION, (ASTNode*[]){$1, $3, NULL}, NULL); }
         | expressao '-' expressao { $$ = createNode(NODE_SUBTRACTION, (ASTNode*[]){$1, $3, NULL}, NULL); }
         | expressao '*' expressao { $$ = createNode(NODE_MULTIPLICATION, (ASTNode*[]){$1, $3, NULL}, NULL); }
         | expressao '/' expressao { $$ = createNode(NODE_DIVISION, (ASTNode*[]){$1, $3, NULL}, NULL); }
         | expressao '<' expressao { $$ = createNode(NODE_LESS_THAN, (ASTNode*[]){$1, $3, NULL}, NULL); }
         | expressao '>' expressao { $$ = createNode(NODE_GREATER_THAN, (ASTNode*[]){$1, $3, NULL}, NULL); }
         | expressao '|' expressao { $$ = createNode(NODE_LOGICAL_OR, (ASTNode*[]){$1, $3, NULL}, NULL); }
         | expressao '&' expressao { $$ = createNode(NODE_LOGICAL_AND, (ASTNode*[]){$1, $3, NULL}, NULL); }
         | expressao OPERATOR_LE expressao { $$ = createNode(NODE_LESS_THAN_EQUAL, (ASTNode*[]){$1, $3, NULL}, NULL); }
         | expressao OPERATOR_GE expressao { $$ = createNode(NODE_GREATER_THAN_EQUAL, (ASTNode*[]){$1, $3, NULL}, NULL); } 
         | expressao OPERATOR_EQ expressao { $$ = createNode(NODE_EQUAL, (ASTNode*[]){$1, $3, NULL}, NULL); }
         | expressao OPERATOR_DIF expressao { $$ = createNode(NODE_NOT_EQUAL, (ASTNode*[]){$1, $3, NULL}, NULL); }
         | '(' expressao ')' { $$ = $2;  }
         | TK_IDENTIFIER { $$ = createNode(NODE_EXPRESSION, NULL, $1); }
         | vetor { $$ = $1; }
         | LIT_INT { $$ = createNode(NODE_EXPRESSION, NULL, $1); }
         | LIT_CHAR { $$ = createNode(NODE_EXPRESSION, NULL, $1); }
         | LIT_REAL { $$ = createNode(NODE_EXPRESSION, NULL, $1); }
         | LIT_FALSE { $$ = createNode(NODE_EXPRESSION, NULL, $1); }
         | LIT_TRUE { $$ = createNode(NODE_EXPRESSION, NULL, $1); }
         | chamada_funcao { $$ = $1; }
         ;

chamada_funcao: TK_IDENTIFIER '(' lista_chamada ')' { $$ = createNode(NODE_FUNC_CALL, (ASTNode*[]){createNode(NODE_TYPE, NULL, $1), $3, NULL}, NULL); }
              ;

lista_chamada: /* empty */ { $$ = NULL; }
             | expressao ',' lista_chamada { $$ = createNode(NODE_ARGS_LIST, (ASTNode*[]){$1, $3, NULL}, NULL); }
             ;
%% 
int yyerror() {
  fprintf(stderr,"Syntax error in line %d\n", getLineNumber());
  exit(3);
  return 1;
}
