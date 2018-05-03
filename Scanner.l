/*
Allan Francisco Julian Novoa A01361651
Cesar Guadarrama cantu A01364853
Constanza lievanos Sanchez A01361015

*/


%{
	#include <stdlib.h>
	#include <string.h>  
	#include <glib.h>
	
%}


	/*

	Declaramos la Variables
  */

d [0-9]
leter [a-zA-Z_]



%%

[\n] 						  { yylineno = yylineno +1; }
[ \t\r]+							  {		       }



"/*"[^*]*"*"+([^*/][^*]*[*]+)*[/]  {          }
"/*"							   {          }
"//".* 							   {          }


	/*Palabras Reservadas*/
"int" 							  { return INTEGER;}
"float" 						  { return FLOAT;}
"if" 							  { return IF;}
"then"							  { return THEN;}
"else" 							  { return ELSE;}
"while" 						  { return WHILE;}
"read" 							  { return READ;}
"write" 					      { return WRITE;}
"do"							  { return DO; }

	/*Signos de puntuacion*/
"," 							  { return COMA;}
";" 							  { return SEMI;}
"(" 							  { return LPAREN;}
")"  							  { return RPAREN;}
"{" 							  { return LBRACE; }
"}" 							  { return RBRACE;}

	/*Tokens de Comparacion*/
"<" 		{ return LT;}
">" 		{ return GT;}
"<=" 		{ return GT;}
">=" 		{ return LT;}
"=" 		{ return EQ;}
"!=" 		{ return EQ;}
 
	/* Signos Aritmeticos */
"+" 							  { return PLUS; }
"-" 							  { return MINUS; }
"*" 							  { return TIMES; }
"/" 							  { return DIV; }

":=" 							  { return ASSIGN; }


	/* Gramaticas para numeros enteros y otro para decimales */

-?(({d}*\.{d}+)([eE][+-]?{d}+)?) 	 { 	yylval.float_value = atof (yytext);
										return FLOAT_NUM; }
-?({d}+)							 { 	yylval.integer_value = atoi (yytext);
										return INT_NUM; }


	/* Gramatica para identificadores */
({leter})({leter}|{d})* 			 { 	yylval.string_value = strdup(yytext);
										return ID;}


	/* Gramatica para "Strings" */
\"[^*]*\"|\'[^*]*\'				  {
									printf("Token: String Value: "),
									printf("%s\n",yytext);
								  }

	/* Any other character not recognized */
. 									{return yytext[0];}
%%
