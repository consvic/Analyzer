%{

  /*
  Allan Francisco Julian Novoa A01361651
  Cesar Guadarrama cantu A01364853
  Constanza Lievanos Sanchez A01361015


  */
#include <glib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "UserDefined.h"
#include "types.h"
extern int lineNum;

/*
Variables para saber si hubo error de que no se delcaro una variable
*/
int x = 0;

/*
*   Arreglo en donde se guarda el codigo
*/
GPtrArray *          code;

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
    GPtrArray *	list;
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

%type <i> type m
%type <symTab> variable factor term simple_exp exp stmt_seq block stmt
%type <list> n


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

              												}
              										}
            ;

type        : INTEGER 					{ $$ = integer;}
            | FLOAT 					  { $$ = real;}
            ;

stmt_seq    : stmt_seq stmt m {
                                                   $$ = malloc(sizeof(entry_p));

                                                      GPtrArray * next = $2->myLists->list_next;
                                                      //$$->list_next = cloneList(myLists->list_next);
                                                    //  backPatch(code,$2->list_next,$3);

                            }

            |                   {

                                }
            ;

stmt        : IF exp THEN m stmt              {
                                                    //  $$ = malloc(sizeof(entry_p));
                                                      //backPatch(code,$2->list_true,$4);
                                                      //$$->list_next = mergeList($2->list_false,$5->list_next);
                                            }
            | IF exp THEN m stmt n m stmt     {
                                                    //  $$ = malloc(sizeof(entry_p));
                                                      //backPatch(code,$2->list_true,$4);
                                                      //backPatch(code,$2->list_false,$7);
                                                      //$$->list_next=mergeList($5->list_next,mergeList($6,$8->list_next));
                                            }
            | WHILE m exp DO m stmt            {
                                                      //backPatch(code,$3->list_true,$5);
                                                    //  $$ = malloc(sizeof(entry_p));
                                                      //$$->list_next = cloneList($3->list_false);
                                                      union result res;
                                                      res.address = $2;
                                                      g_ptr_array_add(code,newQuad("jump",res,NULL,NULL));
                                            }
            | variable ASSIGN exp SEMI
                                        {

                                            printf("Linea %d con tipos %d %d\n",lineNum, $1->type, $3->type);

                                                if(($1->type == real) && ($3->type == integer)) {
                                                    /*Aqui se hace la cohersion*/
                                                    cohersion();
                                                    $3->type = real;
                                                } else  if(($1->type == real) && ($3->type == real)) {
                                                         } else  if(($1->type == integer)&& ($3->type == integer)) {
                                                             } else {
                                                                 typeError();
                                                             }

                                                      union result res;
                                                      res.entry = $1;
                                                      g_ptr_array_add(code,newQuad("assign",res,$3,NULL));
                                                      $$ = malloc(sizeof(entry_p));

                                                      theLists_p myLists = malloc(sizeof(theLists_p));
                                                      myLists->list_next = g_ptr_array_new();
                                                      $$->myLists = myLists;
                                        }
            | READ LPAREN variable RPAREN SEMI  {
                                                      union result resWrite;
                                                      resWrite.entry = $3;
                                                      g_ptr_array_add(code,newQuad("read",resWrite,NULL,NULL));
                                                      $$ = malloc(sizeof(entry_p));
                                                      theLists_p myLists = malloc(sizeof(theLists_p));
                                                      myLists->list_next = g_ptr_array_new();
                                                      $$->myLists = myLists;
                                                }
            | WRITE LPAREN exp RPAREN SEMI      {
                                                      union result resRead;
                                                      resRead.entry = $3;
                                                      g_ptr_array_add(code,newQuad("write",resRead,NULL,NULL));
                                                      $$ = malloc(sizeof(entry_p));
                                                      theLists_p myLists = malloc(sizeof(theLists_p));
                                                      myLists->list_next = g_ptr_array_new();
                                                      $$->myLists = myLists;
                                                }
            | block                             {
                                                    $$ = $1;
                                                }
            ;

m           :                                   {
                                                    $$ = code->len;
                                                }
            ;

n           : ELSE 					            {

									           $$ = newList(code->len);
                                               union result res;
									           res.address = 0;/* Any address is ok, it will be replaced during backpatch*/
									           g_ptr_array_add(code,newQuad("jump",res,NULL,NULL));

								                }
            ;


block       : LBRACE stmt_seq RBRACE            {
                                                      $$ = $2;
                                                }
            ;

exp         : simple_exp LT simple_exp          {
                                                      $$ = malloc(sizeof(entry_p));
                                                      $$->type = integer;
                                                      theLists_p myLists = malloc(sizeof(theLists_p));
                                                      myLists->list_true = NULL;
                                                      myLists->list_false = NULL;
                                                      myLists->list_next = NULL;
                                                    myLists->list_true = newList(code->len);
                                                    myLists->list_false = newList(code->len+1);
                                                    $$->myLists = myLists;

                                                      /* Place the "code" generated in the array that represents the memory */
                                                      union result res;
                                                      res.address = 0;/* Any address is ok, it will be replaced during backpatch*/
                                                      g_ptr_array_add(code,newQuad("LT",res,$1,$3));

                                                      union result res2;
                                                      res2.address = 0;/* Any address is ok, it will be replaced during backpatch*/
                                                      g_ptr_array_add(code,newQuad("jump",res2,NULL,NULL));
                                                }
            | simple_exp EQ simple_exp          {
                                                      $$ = malloc(sizeof(entry_p));
                                                      $$->type = integer;
                                                      theLists_p myLists = malloc(sizeof(theLists_p));
                                                      myLists->list_true = NULL;
                                                      myLists->list_false = NULL;
                                                      myLists->list_next = NULL;
                                                    myLists->list_true = newList(code->len);
                                                    myLists->list_false = newList(code->len+1);
                                                    $$->myLists = myLists;

                                                  //   $$->list_true = newList(code->len);
                                                      //$$->list_false = newList(code->len+1);

                                                      /* Place the "code" generated in the array that represents the memory */
                                                      union result res;
                                                      res.address = 0;/* Any address is ok, it will be replaced during backpatch*/
                                                      g_ptr_array_add(code,newQuad("EQ",res,$1,$3));

                                                      union result res2;
                                                      res2.address = 0;/* Any address is ok, it will be replaced during backpatch*/
                                                      g_ptr_array_add(code,newQuad("jump",res2,NULL,NULL));
                                                }
            | simple_exp GT simple_exp          {
                                                      $$ = malloc(sizeof(entry_p));
                                                      $$->type = integer;

                                                      $$ = malloc(sizeof(entry_p));
                                                      $$->type = integer;
                                                      theLists_p myLists = malloc(sizeof(theLists_p));
                                                      myLists->list_true = NULL;
                                                      myLists->list_false = NULL;
                                                      myLists->list_next = NULL;
                                                    myLists->list_true = newList(code->len);
                                                    myLists->list_false = newList(code->len+1);
                                                    $$->myLists = myLists;

                                                      /* Place the "code" generated in the array that represents the memory */
                                                      union result res;
                                                      res.address = 0; /* Any address is ok, it will be replaced during backpatch*/
                                                      g_ptr_array_add(code,newQuad("GT",res,$1,$3));

                                                      union result res2;
                                                      res2.address = 0; /* Any address is ok, it will be replaced during backpatch*/
                                                      g_ptr_array_add(code,newQuad("jump",res2,NULL,NULL));
                                                }
            | simple_exp
                                                {
                  											$$ = $1;
                  						        }
            ;

simple_exp  : simple_exp PLUS term
                                          {
                                                      $$ = newTemp(theTable_p);
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
                                                      union result res;
                                                      res.entry = $$;
                                                      g_ptr_array_add(code,newQuad("sum",res,$1,$3));
                                          }

            | simple_exp MINUS term
                                          {
                                                      $$ = newTemp(theTable_p);
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
                                                      union result res;
                                                      res.entry = $$;
                                                      g_ptr_array_add(code,newQuad("minus",res,$1,$3));


                                          }
            | term
                                          {
                    												$$ = $1;
                                          }
            ;

term        : term TIMES factor
                                          {
                                                      $$ = newTemp(theTable_p);
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
                                                        union result res;
                                                      res.entry = $$;
                                                      g_ptr_array_add(code,newQuad("mult",res,$1,$3));

                                          }
            | term DIV factor
                                          {
                                                      $$ = newTemp(theTable_p);
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
                                                                  printf("HOLI");
                                                                  printf("\nInfo. Coercion performed at line %d passing integer to float\n",lineNum );
                                                                  $$->type = real;
                                                            }
                                                      }
                                                     union result res;
                                                    res.entry = $$;
                                                    g_ptr_array_add(code,newQuad("div",res,$1,$3));

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
                                                    x = 1;
                                                    printf("Error! In line %d: Undeclared variable %s\n",lineNum,$1);
                                              }else{
                                                    $$ = node;
                                              }
                                        }
            ;


%%

/*Incluimos a lex.yy.c*/
#include "lex.yy.c"
#include<ctype.h>

/* BISON DOES NOT IMPLEMENT YYERROR, SO DEFINE IT HERE */
void yyerror (GHashTable * theTable_p, const char* const message){
  printf ("%s in line %d\n",message,lineNum);
}
void typeError(){
    printf ("Type Error in line %d\n",lineNum);
}

void cohersion(){
    printf ("We use cohersion here in line %d\n",lineNum);
}

/* BISON DOES NOT DEFINE THE MAIN ENTRY POINT SO DEFINE IT HERE */
int main (){
  GHashTable * theTable_p;
  theTable_p = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, (GDestroyNotify)FreeItem);
  code = g_ptr_array_new();
  yyparse(theTable_p);
  if(x != 1)
  {
    PrintTable(theTable_p);
    //PrintCode(code);
  }

  DestroyTable(theTable_p);
}
