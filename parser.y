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
%token TK_IDENTIFIER     
%token LIT_INT           
%token LIT_CHAR          
%token LIT_REAL          
%token LIT_FALSE         
%token LIT_TRUE          
%token LIT_STRING        
%token TOKEN_ERROR       

%left '+' '-'
%left '*' '/'
%left '<' '>' OPERATOR_DIF OPERATOR_EQ OPERATOR_GE OPERATOR_LE
%left '&' '|'
%left KW_IF


%% 
programa: lista_declaracoes
        ;

lista_declaracoes: declaracao
                | lista_declaracoes declaracao
                ;

declaracao: declaracao_variavel ';'
          | declaracao_vetor ';'
          | declaracao_funcao
          ;

declaracao_variavel: tipo TK_IDENTIFIER ':' valor_inicial
           ;

declaracao_vetor: tipo TK_IDENTIFIER '[' LIT_INT ']' ':' valores_iniciais
          | tipo TK_IDENTIFIER '[' LIT_INT ']'
          ;

declaracao_funcao: tipo TK_IDENTIFIER '(' lista_parametros ')' bloco
                  ;

              
lista_parametros: 
                | lista_parametros ',' parametro
                | parametro
                ;


parametro: tipo TK_IDENTIFIER
         ;

tipo: KW_CHAR
    | KW_INT
    | KW_FLOAT
    | KW_BOOL
    ;

valor_inicial: LIT_INT
             | LIT_CHAR
             | LIT_REAL
             | LIT_FALSE
             | LIT_TRUE
             | LIT_STRING
             ;

valores_iniciais: valor_inicial
                | valores_iniciais valor_inicial
                ;

bloco: '{' lista_comandos '}'
     ;

lista_comandos: /* vazio */
              | lista_comandos comando
              ;

comando: atribuicao
       | controle_fluxo
       | KW_READ tipo TK_IDENTIFIER ';'
       | KW_PRINT tipo expressao ';'
       | KW_PRINT LIT_STRING ';'
       | KW_RETURN expressao ';'
       | bloco
       | ';'
       ;

atribuicao: TK_IDENTIFIER '=' expressao ';'
          | vetor '=' expressao ';'
          ;

vetor: TK_IDENTIFIER'['LIT_INT']'
     | TK_IDENTIFIER'['TK_IDENTIFIER']'
     ;

controle_fluxo: KW_IF '(' expressao ')' comando
              | KW_IF '(' expressao ')' comando KW_ELSE comando
              | KW_WHILE '(' expressao ')' comando
              ;
expressao: expressao '+' expressao
         | expressao '-' expressao
         | expressao '*' expressao
         | expressao '/' expressao
         | expressao '<' expressao
         | expressao '>' expressao
         | expressao '|' expressao
         | expressao '&' expressao
         | expressao OPERATOR_LE expressao
         | expressao OPERATOR_GE expressao
         | expressao OPERATOR_EQ expressao
         | expressao OPERATOR_DIF expressao
         | '(' expressao ')'
         | TK_IDENTIFIER
         | vetor
         | LIT_INT
         | LIT_CHAR
         | LIT_REAL
         | LIT_FALSE
         | LIT_TRUE
         | chamada_funcao
         ;

chamada_funcao: TK_IDENTIFIER '(' lista_chamada ')' 
              ;

lista_chamada:
             | expressao ',' lista_chamada
             | expressao
             ;


%% 
int yyerror() {
  fprintf(stderr,"Syntax error in line %d\n", getLineNumber());
  exit(3);
  return 1;
}
