
/*
Allan Francisco Julian Novoa A01361651
Cesar Guadarrama cantu A01364853
Constanza Lievanos Sanchez A01361506

*/
%{
      #include <glib.h>
      #include <string.h>
      #include <stdio.h>
      #include "UserDefined.h"
      #include "types.h"
      extern int lineNum;

  /* Declaramos las Funciones */
void yyerror (GHashTable * theTable_p, const char* const message);
%}


%union {
    char *s;
    float f;
    int i;
}
%parse-param{GHashTable * theTable_p}
/*Declaramos los tokens*/
%token <s>ID
%token SEMI
%token <s>INTEGER
%token <s>FLOAT
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
%token INT_NUM
%token FLOAT_NUM
%type <s> type

%%

//Declaramos la Gramatica
program     : var_dec stmt_seq     { printf ("No errors in the line\n");}
            ;

var_dec     : var_dec single_dec
            |
            ;

single_dec  : type ID SEMI{
                    entry_p      node_p;
                    node_p = malloc(sizeof(entry_p));
                    node_p = NewItem($2, $1, lineNum);
                  g_hash_table_insert(theTable_p, node_p->name_p, node_p);

              }
            ;


type        : INTEGER { $$ = "integer";}
            | FLOAT { $$ = "float";}
            ;

stmt_seq    : stmt_seq stmt
            |
            ;

stmt        : IF exp THEN stmt
            | IF exp THEN stmt ELSE stmt
            | WHILE exp DO stmt
            | variable ASSIGN exp SEMI
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
            ;

simple_exp  : simple_exp PLUS term
            | simple_exp MINUS term
            | term
            ;

term        : term TIMES factor
            | term DIV factor
            | factor
            ;

factor      : LPAREN exp RPAREN
            | INT_NUM
            | FLOAT_NUM
            | variable
            ;

variable    : ID
            ;


%%

/*Incluimos a lex.yy.c*/
#include "lex.yy.c"

/* BISON DOES NOT IMPLEMENT YYERROR, SO DEFINE IT HERE */
void yyerror (GHashTable * theTable_p, const char* const message){
  printf ("%s in line %d\n",message,lineNum);
}

/* BISON DOES NOT DEFINE THE MAIN ENTRY POINT SO DEFINE IT HERE */
int main (){
    GHashTable * theTable_p; //Declaration of the hash table
  /* We create the hash table, using a string as the hash key.
   * We use new full to handle memory allocation through the method FreeItem.
   */
  theTable_p = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, (GDestroyNotify)FreeItem);
  yyparse(theTable_p); //We start parsing.
  PrintTable(theTable_p); //Print the contents of the table.
  DestroyTable(theTable_p); //Finally we eliminate the elements of theTable_p (hash table)
}
