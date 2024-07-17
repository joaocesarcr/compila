%{
/* Compiladores 2024/01 - Etapa 3
 * João César de Paula Criscolo  - 00304342
 * Prof. Marcelo Johann
*/

#include "ast.h"
#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode* root;
extern HASH_NODE* hashFind(char* text);
%}

%union {
    ASTNode* astnode;
    HASH_NODE* hash_node;
    char* data;
    int token;
}

%type <astnode> programa lista_declaracoes declaracao declaracao_variavel declaracao_vetor declaracao_funcao
%type <astnode> lista_parametros parametro tipo valor_inicial valores_iniciais bloco lista_comandos
%type <astnode> comando atribuicao vetor controle_fluxo controle_fluxo_if expressao chamada_funcao lista_chamada

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

%token <hash_node> TK_IDENTIFIER     
%token <hash_node> LIT_INT           
%token <hash_node> LIT_CHAR          
%token <hash_node> LIT_REAL          
%token <hash_node> LIT_FALSE         
%token <hash_node> LIT_TRUE          
%token <hash_node> LIT_STRING        
%token <hash_node> TOKEN_ERROR       

%left '+' '-'
%left '*' '/'
%left '<' '>' OPERATOR_DIF OPERATOR_EQ OPERATOR_GE OPERATOR_LE
%left '&' '|'
%left KW_IF
%right LOWER_THAN_ELSE ELSE

%start programa
%%


programa: lista_declaracoes { root = createNode(NODE_PROGRAM, (ASTNode*[]){$1, NULL}, NULL); }
        ;

lista_declaracoes: declaracao { $$ = createNode(NODE_DECLARATIONS_LIST, (ASTNode*[]){$1, NULL}, NULL); }
                | lista_declaracoes declaracao { $$ = createNode(NODE_DECLARATIONS_LIST, (ASTNode*[]){$1, $2, NULL}, NULL); }
                ;

declaracao: declaracao_variavel ';' { $$ = $1; }
          | declaracao_vetor ';' { $$ = $1; }
          | declaracao_funcao { $$ = $1; }
          ;

declaracao_variavel: tipo TK_IDENTIFIER ':' valor_inicial { $$ = createNode(
                                                                     NODE_VAR_DECLARATION,
                                                                     (ASTNode*[]) {
                                                                         $1,
                                                                         createNode(NODE_TOKEN_IDENTIFIER,astNullChild(),$2),
                                                                         $4,
                                                                         NULL,
                                                                         NULL
                                                                      },
                                                                      NULL); }
                   ;


declaracao_vetor: tipo TK_IDENTIFIER '[' LIT_INT ']' ':' valores_iniciais {
                $$ = createNode(NODE_VECTOR_DECLARATION_AND_ASIGN,
                                (ASTNode*[]){ 
                                    $1,
                                    createNode(NODE_TOKEN_IDENTIFIER,astNullChild(),$2),
                                    createNode(NODE_LITERAL_INT,astNullChild(),$4),
                                    $7,
                                    NULL,
                                },
                                NULL);
                              }
                | tipo TK_IDENTIFIER '[' LIT_INT ']' {
                $$ = createNode(NODE_VECTOR_DECLARATION,
                                   (ASTNode*[]){ 
                                         $1,
                                         createNode(NODE_TOKEN_IDENTIFIER,astNullChild(),$2),
                                         createNode(NODE_LITERAL_INT,astNullChild(),$4),
                                         NULL,
                                         NULL,
                                    },
                                    NULL);
                }
                ;

declaracao_funcao: tipo TK_IDENTIFIER '(' lista_parametros ')' bloco { $$ = createNode(
                                                                                   NODE_FUNC_DECLARATION,
                                                                                   (ASTNode*[]){
                                                                                        $1,
                                                                                        createNode(NODE_TOKEN_IDENTIFIER,astNullChild(),$2),
                                                                                        $4,
                                                                                        $6,
                                                                                        NULL},
                                                                                   NULL); }
                 | tipo TK_IDENTIFIER '(' ')' bloco {$$ = createNode(
                                                              NODE_FUNC_DECLARATION,
                                                               (ASTNode*[]){
                                                                    $1,
                                                                    createNode(NODE_TOKEN_IDENTIFIER,astNullChild(),$2),
                                                                    createNode(NODE_PARAM_LIST_EMPTY,astNullChild(),NULL),
                                                                    $5,
                                                                    NULL},
                                                               NULL); }

                 ;


parametro: tipo TK_IDENTIFIER { $$ = createNode(NODE_PARAM, (ASTNode*[]){
         $1,
         createNode(NODE_TOKEN_IDENTIFIER,astNullChild(),$2),NULL,NULL,NULL},NULL); }
         ;

tipo: KW_CHAR { $$ = createNode(NODE_KW_CHAR, NULL, NULL); }
    | KW_FLOAT { $$ = createNode(NODE_KW_FLOAT, NULL, NULL); }
    | KW_BOOL { $$ = createNode(NODE_KW_BOOL, NULL, NULL); }
    | KW_INT { $$ = createNode(NODE_KW_INT,NULL, NULL); }
    ;

valor_inicial: LIT_INT { $$ = createNode(NODE_LITERAL_INT,       astNullChild(),$1); }
             | LIT_CHAR { $$ = createNode(NODE_LITERAL_CHAR,     astNullChild(),$1); }
             | LIT_REAL { $$ = createNode(NODE_LITERAL_CHAR,     astNullChild(),$1); }
             | LIT_FALSE { $$ = createNode(NODE_LITERAL_FALSE,   astNullChild(),$1); }
             | LIT_TRUE { $$ = createNode(NODE_LITERAL_TRUE,     astNullChild(),$1); }
             | LIT_STRING { $$ = createNode(NODE_LITERAL_STRING, astNullChild(),$1); }
             ;

valores_iniciais: valor_inicial { $$ = $1 ;}
                | valor_inicial  valores_iniciais{ $$ = createNode(NODE_VALUES_LIST, (ASTNode*[]){
                                                                                          $1,
                                                                                          $2,
                                                                                          NULL,
                                                                                          NULL,
                                                                                          NULL
                                                                                      },
                                                                                          NULL); }
                ;

bloco: '{' lista_comandos '}' { $$ = createNode(NODE_BLOCK, (ASTNode*[]){$2, NULL,NULL,NULL,NULL}, NULL); }
     | '{' '}'  { $$ = createNode(NODE_BLOCK_EMPTY, astNullChild(), NULL); }
     ;


comando: atribuicao { $$ = $1; }
       | controle_fluxo { $$ = $1; }
       | KW_READ tipo TK_IDENTIFIER ';' { $$ = createNode(NODE_KW_READ, (ASTNode*[]){$2, createNode(NODE_TOKEN_IDENTIFIER, astNullChild(),$3), NULL}, NULL); }
       | KW_PRINT LIT_STRING ';'     { $$ = createNode(NODE_KW_PRINT_STRING, (ASTNode*[]){createNode(NODE_LITERAL_STRING, astNullChild(), $2), NULL}, NULL); }
       | KW_PRINT tipo expressao ';' { $$ = createNode(NODE_KW_PRINT, (ASTNode*[]){$2, $3, NULL}, NULL); }
       | KW_RETURN expressao ';'     { $$ = createNode(NODE_KW_RETURN, (ASTNode*[]){$2, NULL}, NULL); }
       | bloco { $$ = $1; }
       | ';' { $$ = NULL; }
       ;

atribuicao: TK_IDENTIFIER '=' expressao ';' { $$ = createNode(
                                                      NODE_ASSIGNMENT,
                                                      (ASTNode*[]){
                                                          createNode(NODE_TOKEN_IDENTIFIER, NULL, $1),
                                                          $3,
                                                          NULL,
                                                          NULL,
                                                          NULL
                                                      },
                                                      NULL);
                                            }
          | vetor '=' expressao ';' { $$ = createNode(NODE_ASSIGNMENT, (ASTNode*[]){$1, $3, NULL},NULL); }
          ;

vetor: TK_IDENTIFIER '[' LIT_INT ']' { $$ = createNode(NODE_VECTOR_INT, (ASTNode*[]){createNode(NODE_TOKEN_IDENTIFIER, astNullChild(),$1),
                                                                                     createNode(NODE_LITERAL_INT, astNullChild(), $3),NULL,NULL,NULL}, NULL); }
     | TK_IDENTIFIER '[' TK_IDENTIFIER ']' { $$ = createNode(NODE_VECTOR_TK, (ASTNode*[]){ 
                                                                                 createNode(NODE_TOKEN_IDENTIFIER, astNullChild(),$1),
                                                                                 createNode(NODE_TOKEN_IDENTIFIER, astNullChild(),$3),
                                                                                 NULL,NULL,NULL},
                                                                               NULL); }
     ;

controle_fluxo: KW_IF '(' expressao ')' comando controle_fluxo_if { $$ = createNode(NODE_IF_CONTROL,(ASTNode*[]){$3, $5, $6,NULL,NULL},NULL); }
    | KW_WHILE '(' expressao ')' bloco { $$ = createNode(NODE_KW_WHILE, (ASTNode*[]){$3, $5, NULL}, NULL); }
    ;

controle_fluxo_if: { $$ = NULL;}
    | KW_ELSE comando { $$ = $2; }
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
         | TK_IDENTIFIER {
               //printf("\nValue of $1: %s\n", $1->text);
               $$ = createNode(NODE_TOKEN_IDENTIFIER, astNullChild(), $1); 
           }
         | vetor { $$ = $1; }
         | LIT_INT { $$ = createNode(NODE_LITERAL_INT,     astNullChild(),  $1); }
         | LIT_CHAR { $$ = createNode(NODE_LITERAL_CHAR,   astNullChild(),  $1); }
         | LIT_REAL { $$ = createNode(NODE_LITERAL_REAL,   astNullChild(),  $1); }
         | LIT_FALSE { $$ = createNode(NODE_LITERAL_FALSE, NULL,  NULL); }
         | LIT_TRUE { $$ = createNode(NODE_LITERAL_TRUE,   NULL,  NULL); }
         | chamada_funcao { $$ = $1; }
         ;

              ;

lista_comandos: comando { $$ = $1;}
              | comando lista_comandos { $$ = createNode(NODE_COMMANDS_LIST, (ASTNode*[]){$1, $2, NULL}, NULL); }
              ;


lista_chamada: /* empty */ { $$ = NULL; }
             | expressao ',' lista_chamada { $$ = createNode(NODE_ARGS_LIST, (ASTNode*[]){$1, $3, NULL}, NULL); }
             | expressao { $$ = createNode(NODE_ARGS_LIST, (ASTNode*[]){$1, NULL}, NULL); }
             ;

lista_parametros: parametro ',' lista_parametros { $$ = createNode(NODE_PARAM_LIST, (ASTNode*[]){$1, $3, NULL,NULL,NULL}, NULL); }
                | parametro { $$ = $1;}
                ;


chamada_funcao: TK_IDENTIFIER '(' lista_chamada ')' { $$ = createNode(NODE_FUNC_CALL, (ASTNode*[]){createNode(NODE_TOKEN_IDENTIFIER, astNullChild(),$1), $3, NULL}, NULL); }

%% 
int yyerror() {
  fprintf(stderr,"Syntax error in line %d\n", getLineNumber());
  exit(3);
  return 1;
}
