
/*

*Allan Francisco Julian Novoa
*Cesar Guadarrama cantu
*Constanza Lievanos Sanchez

*/

#include "UserDefined.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>




/*entry_p NewItem (char * varName_p, char * type, unsigned int lineNumber){
                   entry_p ent = malloc(sizeof(entry_p));
                   ent->name_p = varName_p;
                   ent->type = type;
                   ent->lineNumber = lineNumber;
                   return ent;
                 }*/


void InsertSymbol(GHashTable *theTable_p, char * name, enum myTypes type,unsigned int lineNumber){
    entry_p node = malloc(sizeof(struct tableEntry_));
	node->name_p = name;
	node->type = type;
    node->lineNumber = lineNumber;

	/* Initialize every variable as 0*/
	if(type == real)
		node->value.r_value = 0.0;
	else
		node->value.i_value = 0;
	g_hash_table_insert(theTable_p,node->name_p,node);
}

void InsertSymbolTemp(GHashTable *theTable_p, char * name, enum myTypes type){
    entry_p node = malloc(sizeof(struct tableEntry_));
	node->name_p = name;
	node->type = type;

	/* Initialize every variable as 0*/
	if(type == real)
		node->value.r_value = 0.0;
	else
		node->value.i_value = 0;
	g_hash_table_insert(theTable_p,node->name_p,node);
}

int PrintTable (GHashTable * theTable_p){
  g_hash_table_foreach(theTable_p, (GHFunc)SupportPrint, NULL);
  return(EXIT_SUCCESS);
}

void SupportPrint (gpointer key_p, gpointer value_p, gpointer user_p){
  PrintItem(value_p);
}

int PrintItem (entry_p theEntry_p){
  printf("Name: %s -- Type: %d\n",theEntry_p->name_p,theEntry_p->type);
  return 1;
}

int InsertItem(GHashTable * theTable_p, entry_p theEntry_p){
  g_hash_table_insert(theTable_p, theEntry_p->name_p, theEntry_p);
  return(EXIT_SUCCESS);
}

int FreeItem (entry_p theEntry_p){

  free(theEntry_p);
  return(EXIT_SUCCESS);
}

int DestroyTable (GHashTable * theTable_p){
  g_hash_table_destroy(theTable_p);
  return(EXIT_SUCCESS);
}

entry_p SymbolLookUp(GHashTable *theTable_p, char *name){
    entry_p item = malloc(sizeof(entry_p));
    entry_p symEntry = g_hash_table_lookup(theTable_p,name);


    if(symEntry!= NULL){

      item->name_p 		= symEntry->name_p;
	    item->value 	= symEntry->value;
	    item->type 		= symEntry->type;
	    return item;
    }
    return NULL;
}

void SymbolUpdate(GHashTable *theTable_p, char * name, enum myTypes type, union val value){
	entry_p node = g_hash_table_lookup(theTable_p,name);
	if(node != NULL){
		node->type = type;
		node->value = value;
	}
}

entry_p newTemp(GHashTable *theTable_p){
	char * temp = malloc(sizeof(char *));
	char * c = malloc(sizeof(char *));
	int i = 0;
	do{
		strcpy(temp,"t");
		snprintf(c,sizeof(char *),"%d",i);
		strcat(temp,c);
		i++;
	}while(SymbolLookUp(theTable_p,temp) != NULL);
	InsertSymbolTemp(theTable_p,temp,integer);
	return SymbolLookUp(theTable_p,temp);
}

entry_p newTempConstant(GHashTable *theTable_p, union val value, enum myTypes type){
	char * temp = malloc(sizeof(char *));
	char * c = malloc(sizeof(char *));
	int i = 0;
	do{
		strcpy(temp,"t");
		snprintf(c,sizeof(char *),"%d",i);
		strcat(temp,c);
		i++;
	}while(SymbolLookUp(theTable_p,temp)!=NULL);

	InsertSymbolTemp(theTable_p,temp,integer);
	SymbolUpdate(theTable_p,temp,type,value);
	return SymbolLookUp(theTable_p,temp);
}
