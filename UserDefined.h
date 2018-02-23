
#include <glib.h>
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
   char *    type;                          /**< Identifier type */
   unsigned int     lineNumber;  /**< Line number of the last reference */
   union val        value;       /**< Value of the symbol table element */
}tableEntry;


typedef struct tableEntry_ *entry_p; /**< Declaration of ptr to an entry */


int PrintItem (entry_p theEntry_p);


void SupportPrint (gpointer key_p, gpointer value_p, gpointer user_p);


int PrintTable (GHashTable * theTable_p);


entry_p NewItem (char * varName_p, char * type, unsigned int lineNumber);


int FreeItem (entry_p theEntry_p);


int DestroyTable (GHashTable * theTable_p);
int InsertItem(GHashTable * theTable_p, entry_p theEntry_p);
