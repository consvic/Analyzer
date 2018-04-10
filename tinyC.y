%{

  /*
  Allan Francisco Julian Novoa A01361651
  Cesar Guadarrama cantu A01364853
  Constanza Lievanos Sanchez A01361015

  */
#include <glib.h>
#include <string.h>
#include <stdio.h>
#include "UserDefined.h"
#include "types.h"
extern int lineNum;

  /* Declaramos las Funciones */
void yyerror (GHashTable * theTable_p, const char* const message);

void typeError();

void cohersion();
%}


%union {
    char *s;
    float f;
    int i;
    entry_p  symTab;
}
%parse-param{GHashTable * theTable_p}
/*Declaramos los tokens*/
%token <s>ID
%token SEMI
%token <i>INTEGER
%token <f>FLOAT
%token COMA
%token IF
%token THEN
%token ELSE
%token WHILE
%token DO
%token ASSIGN
%token WRITE
%token READ
%token LPAREN
%token RPAREN
%token LBRACE
%token RBRACE
%token LT
%token GT
%token LE
%token GE
%token EQ
%token NE
%token LTEQ
%token PLUS
%token MINUS
%token TIMES
%token DIV
%type <i> type
%type <symTab> variable factor term simple_exp exp stmt_seq block stmt

%left MINUS PLUS
%left TIMES DIV

%%

//Declaramos la Gramatica
program     : var_dec stmt_seq     { printf ("No errors in the line\n");}
            ;

var_dec     : var_dec single_dec
            |
            ;

single_dec  : type ID SEMI
                                  {
              												if(SymbolLookUp(theTable_p,$2)!=NULL) {
              												printf("\nWarning! In line %d: Variable %s already defined\n",lineNum,$2 );
              												} else {

              												  InsertSymbol(theTable_p,$2,$1,lineNum);
              														/*entry_p      node_p;
              														node_p = malloc(sizeof(entry_p));
              														node_p = NewItem($2, $1, lineNum);
              														g_hash_table_insert(theTable_p, node_p->name_p, node_p);*/
              												}
              										}
            ;

type        : INTEGER 					{ $$ = integer;}
            | FLOAT 					  { $$ = real;}
            ;

stmt_seq    : stmt_seq stmt
            |
            ;

stmt        : IF exp THEN stmt
            | IF exp THEN stmt ELSE stmt
            | WHILE exp DO stmt
            | variable ASSIGN exp SEMI
                                        {
                                            printf("Linea %d con tipos %d %d\n",lineNum, $1->type, $3->type);

                                                if(($1->type == real) && ($3->type == integer)) {
                                                    /*Aqui se hace la cohersion*/
                                                    printf("\nInfo. Coercion performed at line %d passing integer to float\n",lineNum );
                                                    $3->type = real;
                                                      /*Pegar codigo*/
                                                } else  if(($1->type == real) && ($3->type == real)) {
                                                            /*Pegar codigo*/
                                                         } else  if(($1->type == integer)&& ($3->type == integer)) {
                                                                /*Pegar codigo*/
                                                             } else {
                                                                 typeError();
                                                             }


                                        }
            | READ LPAREN variable RPAREN SEMI
            | WRITE LPAREN exp RPAREN SEMI
            | block
            ;

block       : LBRACE stmt_seq RBRACE
            ;

exp         : simple_exp LT simple_exp
            | simple_exp EQ simple_exp
            | simple_exp GT simple_exp
            | simple_exp GE simple_exp
            | simple_exp LE simple_exp
            | simple_exp NE simple_exp
            | simple_exp
                                      {
                  											$$ = $1;
                  										}
            ;

simple_exp  : simple_exp PLUS term
                                          {

                                                      if($1->type == real){
                                                            if($3->type==real){
                                                                  $$->type = real;
                                                            }
                                                            else{
                                                            	/* Coercion */
                                                            	printf("\nInfo. Coercion performed at line %d passing integer to float\n",lineNum );
                                                                  $$->type = real;
                                                            }
                                                      }
                                                      else{
                                                            if($3->type==real){
                                                            	/* Coercion */
                                                            	printf("\nInfo. Coercion performed at line %d passing integer to float\n",lineNum );
                                                              $$->type = real;
                                                            }
                                                            else{
                                                                  $$->type = integer;
                                                            }
                                                      }
                                          }

            | simple_exp MINUS term
                                          {

                                                      if($1->type == real){
                                                            if($3->type==real){
                                                                  $$->type = real;
                                                            }
                                                            else{
                                                            	/* Coercion */
                                                            	printf("\nInfo. Coercion performed at line %d passing integer to float\n",lineNum );
                                                                $$->type = real;
                                                            }
                                                      }
                                                      else{
                                                            if($3->type==real){
                                                            	/* Coercion */
                                                            	printf("\nInfo. Coercion performed at line %d passing integer to float\n",lineNum );
                                                              $$->type = real;
                                                            }
                                                            else{
                                                                  $$->type = integer;
                                                            }
                                                      }


                                          }
            | term
                                          {
                    												$$ = $1;
                    											}
            ;

term        : term TIMES factor
                                          {
                                                      if($1->type == real){
                                                            if($3->type==real){
                                                                  $$->type = real;
                                                            }
                                                            else{
                                                            	/* Coercion */
                                                            	printf("\nInfo. Coercion performed at line %d passing integer to float\n",lineNum );
                                                                $$->type = real;
                                                            }
                                                      }
                                                      else{
                                                            if($3->type==real){
                                                            	/* Coercion */
                                                            	printf("\nInfo. Coercion performed at line %d passing integer to float\n",lineNum );
                                                              $$->type = real;
                                                            }
                                                            else{
                                                                  $$->type = integer;
                                                            }
                                                      }


                                          }
            | term DIV factor
                                          {

                                                      if($1->type == real){
                                                            if($3->type==real){
                                                                  $$->type = real;
                                                            }
                                                            else{
                                                            	/* Coercion */
                                                            	printf("\nInfo. Coercion performed at line %d passing integer to float\n",lineNum );
                                                              $$->type = real;
                                                            }
                                                      }
                                                      else{
                                                            if($3->type==real){
                                                            	/* Coercion */
                                                            	printf("\nInfo. Coercion performed at line %d passing integer to float\n",lineNum );
                                                              $$->type = real;
                                                            }
                                                            else{
                                                                  printf("\nInfo. Coercion performed at line %d passing integer to float\n",lineNum );
                                                                  $$->type = real;
                                                            }
                                                      }


                                          }
			      | factor
                                          {
                    												$$ = $1;
                    											}
            ;

factor      : LPAREN exp RPAREN
                                          {
                    												$$ = $2;
                    											}
            | INTEGER
                                          {
                    												/* Add constants to the symbol table to ease implementation */
                    												union val value;
                    												value.i_value = $1;
                    												$$ = newTempConstant(theTable_p,value,integer);
                    											}
            | FLOAT
                                          {
                    												/* Add constants to the symbol table to ease implementation */
                    												union val value;
                    												value.r_value = $1;
                    												$$ = newTempConstant(theTable_p,value,real);
                    											}
            | variable
                                          {
                    												$$ = $1;
                    											}
            ;

variable    : ID
                                        {
                                              /* Check if the variable is in the symbol table */
                                              entry_p node = SymbolLookUp(theTable_p,$1);
                                              if(node == NULL){
                                                    printf("\nWarning! In line %d: Undeclared variable %s\n",lineNum,$1);
                                              }else{
                                                    $$ = node;
                                              }
                                        }
            ;


%%

/*Incluimos a lex.yy.c*/
#include "lex.yy.c"

/* BISON DOES NOT IMPLEMENT YYERROR, SO DEFINE IT HERE */
void yyerror (GHashTable * theTable_p, const char* const message){
  printf ("%s in line %d\n",message,lineNum);
}
void typeError(){
    printf ("Type Error in line %d\n",lineNum);
}

/* BISON DOES NOT DEFINE THE MAIN ENTRY POINT SO DEFINE IT HERE */
int main (){
  GHashTable * theTable_p;
  theTable_p = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, (GDestroyNotify)FreeItem);
  yyparse(theTable_p);
  PrintTable(theTable_p);
  DestroyTable(theTable_p);
}
