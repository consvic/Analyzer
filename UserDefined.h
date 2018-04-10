
#include <glib.h>
#include <string.h>
#include "types.h"


typedef struct item_{
   char * key;                           /**< Hash table key is a string */
   void * tableEntry;           /**< Pointer to a generic data structure */
}item;


typedef struct item_ *item_p;        /**< Declaration of ptr to an entry */




union val {            /* Note that both values are 32-bits in length */
   int     i_value;                   /**< Interpret it as an integer */
   float   r_value;                      /**< Interpret it as a float */
};


typedef struct tableEntry_{
   char           * name_p;            /**< The name is just the string */
   enum myTypes	type;                          /**< Identifier type */
   unsigned int     lineNumber;  /**< Line number of the last reference */
   union val        value;       /**< Value of the symbol table element */

}tableEntry;


typedef struct tableEntry_ *entry_p; /**< Declaration of ptr to an entry */

/*Method to print an item from the table*/
int PrintItem (entry_p theEntry_p);


void SupportPrint (gpointer key_p, gpointer value_p, gpointer user_p);

/*Print all the table*/
int PrintTable (GHashTable * theTable_p);

/*Insert an item to the table*/
//entry_p NewItem (char * varName_p, char * type, unsigned int lineNumber);

/*Lookup to the symbol table*/
entry_p SymbolLookUp(GHashTable *theTable_p, char *name);

/*Insert an symbol to the table*/
void InsertSymbol(GHashTable *theTable_p, char * name, enum myTypes type,unsigned int lineNumber);

/*Insert an item to the symbl table without the line number*/
void InsertSymbolTemp(GHashTable *theTable_p, char * name, enum myTypes type);

/*Free an item fromthe table*/
int FreeItem (entry_p theEntry_p);

/*Delete the table*/
int DestroyTable (GHashTable * theTable_p);

/*Inserts existing entry to the table*/
int InsertItem(GHashTable * theTable_p, entry_p theEntry_p);

/*Create temporal variable*/
entry_p newTemp(GHashTable *theTable_p);

/*Create constant temporal variable*/
entry_p newTempConstant(GHashTable *theTable_p, union val value, enum myTypes type);

/*Finds an entry and update a symbol to table*/
void SymbolUpdate(GHashTable *theTable_p, char * name, enum myTypes type, union val value);
