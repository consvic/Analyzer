
/*

*Allan Francisco Julian Novoa
*Cesar Guadarrama cantu
*Constanza Lievanos Sanchez

*/

#include "UserDefined.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>




entry_p NewItem (char * varName_p, char * type, unsigned int lineNumber){
                   entry_p ent = malloc(sizeof(entry_p));
                   ent->name_p = varName_p;
                   ent->type = type;
                   ent->lineNumber = lineNumber;
                   return ent;
                 }

int PrintTable (GHashTable * theTable_p){
  g_hash_table_foreach(theTable_p, (GHFunc)SupportPrint, NULL);
  return(EXIT_SUCCESS);
}

void SupportPrint (gpointer key_p, gpointer value_p, gpointer user_p){
  PrintItem(value_p);
}

int PrintItem (entry_p theEntry_p){
  printf("Name: %s -- Type: %s\n",theEntry_p->name_p,theEntry_p->type);
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
