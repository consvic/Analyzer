/*
Allan Francisco Julian Novoa A01361651
Cesar Guadarrama cantu A01364853
Constanza lievanos Sanchez A01361506

*/


%{
	#include <stdlib.h>
	int lineNum = 1;  //Variable para saber en que linea se cometio el error
%}


	/*

	Declaramos la Variables
  */

d [0-9]
leter [a-zA-Z_]



%%

[\n\r]+ 						  { lineNum++; }
[ \t]+							  {		       }



"/*"[^*]*"*"+([^*/][^*]*[*]+)*[/]  {          }
"/*"							   {          }
"//".* 							   {          }


	/*Palabras Reservadas*/
"int" 							  { yylval.s = strdup(yytext);return INTEGER;}
"float" 						  { yylval.s = strdup(yytext);return FLOAT;}
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
"<" 							  { return LT;}
">" 							  { return GT;}
"<=" 							  { return GE;}
">=" 							  { return LE;}
"=" 							  { return EQ;}
"!=" 							  { return NE;}

	/* Signos Aritmeticos */
"+" 							  { return PLUS; }
"-" 							  { return MINUS; }
"*" 							  { return TIMES; }
"/" 							  { return DIV; }

":=" 							  { return ASSIGN; }


	/* Gramaticas para numeros enteros y otro para decimales */

-?(({d}*\.{d}+)([eE][+-]?{d}+)?)  { return FLOAT_NUM; }
-?({d}+)						  { return INT_NUM; }


	/* Gramatica para identificadores */
({leter})({leter}|{d})* 		  {yylval.s = strdup(yytext);
return ID;}


	/* Gramatica para "Strings" */
\"[^*]*\"|\'[^*]*\'				  {
									printf("Token: String Value: "),
									printf("%s\n",yytext);
								  }

	/* Si el simbolo no es reconocido, lo tomara como error*/
. 								  { printf("Error: Symbol not recognized\t%d in line %d\n",*yytext,lineNum); }
%%
