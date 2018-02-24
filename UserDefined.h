
#include <glib.h>
#include "types.h"

/**
*
* @struct item
*
* @brief basic element for the table entry
*
* The user defined data structure has a @c string with the key
* and a @c pointer to the table entry
*
*/
typedef struct item_{
   char * key;                           /**< Hash table key is a string */
   void * tableEntry;           /**< Pointer to a generic data structure */
}item;

/**
*
* @typedef item_p
*
* @brief declare a pointer to the @c item @c structure
*
*/
typedef struct item_ *item_p;        /**< Declaration of ptr to an entry */

/**
*
* @union val
*
* @brief Defines which value of the symbol table is being used
*
* The @c val union defines the possible values for the elements in the
* symbol table (int or float)
*
*/
union val {            /* Note that both values are 32-bits in length */
   int     i_value;                   /**< Interpret it as an integer */
   float   r_value;                      /**< Interpret it as a float */
};

/**
*
* @struct tableEntry
*
* @brief This is the user-defined symbol table entry.
*
* The @c TableEntry is the user-defined structure that describes a symbol table
* entry. Each entry has the following fields:
*
* The @c name_p is a string holding the name of the variable. This may be
*    different from the hash key.
*
* The @c type indicates if the variable is integer or float.
*
* The @c lineNumber is the line number where the variable was defined.
*
* The @c value is a union of all possible values (integer/float). Not space
*    efficient if smaller types are allowed.
*
*/
typedef struct tableEntry_{
   char * name_p;            /**< The name is just the string */
   char * type;                          /**< Identifier type */
   unsigned int lineNumber;  /**< Line number of the last reference */
   union val value;       /**< Value of the symbol table element */
}tableEntry;

/**
*
* @typedef entry_p
*
* @brief pointer to the @c tableEntry @c structure
*
*/
typedef struct tableEntry_ *entry_p; /**< Declaration of ptr to an entry */

/**
*
* @brief Prints the contents of the symbol table.
*
* @b PrintItem will print each field of the table.
*
* @param  theEntry_p is a pointer to a user-defined structure element.
*
* @return @c EXIT_SUCCESS the item was printed with no
*         problems, otherwise returns @c EXIT_FAILURE
*
* @code
*  printf("Name: %s -- Type: %s\n",theEntry_p->name_p,theEntry_p->type);
*  return 1;
* @endcode
*
*/
int PrintItem (entry_p theEntry_p);

/**
*
* @brief Gets the key, value and data pointers from the @c g_hash_foreach
* and calls PrintItem for each element.
*
* @b SupportPrint is a support function that captures the key, value and
* data pointers from @c g_hash_foreach and in turn calls @c PrintItem to
* print each entry from the hash table.
*
* @param  key_p pointer to the key
* @param  value_p pointer to the value
* @param  user_p pointer to the user-defined data
* @return @c void
*
* @code
*  PrintItem(value_p);
* @endcode
*
*/
void SupportPrint (gpointer key_p, gpointer value_p, gpointer user_p);

/**
*
* @brief Prints all the elements of a table.
*
* @b PrintTable will print all the elements of @p theTable_p. It calls
* the user-defined function SuuportPrint which handles the format of the data
* portion of the items in the table.
*
* @param  theTable_p pointer to the table to be printed.
* @return @c EXIT_SUCCESS if the table was traversed with no
*         problems, otherwise return @c EXIT_FAILURE.
*
* @code
*   g_hash_table_foreach(theTable_p, (GHFunc)SupportPrint, NULL);
*   return(EXIT_SUCCESS);
* @endcode
*
*/
int PrintTable (GHashTable * theTable_p);

/**
*
* @brief Allocates memory to a node of the user-defined data structure.
*
* @b NewItem will allocate space for the @c varName_p string and set the
* fields of the @c tableEntry structure.
*
* @param  varName_p is the input string which is copied by NewItem.
* @param  type is the variable type (integer or real).
* @param  lineNumber line where the variable was declared.
*
* @return @c entry_p pointer to the memory block that holds the newly created
*         symbol table entry.
*
* @code
*   entry_p ent = malloc(sizeof(entry_p));
*   ent->name_p = varName_p;
*   ent->type = type;
*   ent->lineNumber = lineNumber;
*   return ent;
* @endcode
*
*/
entry_p NewItem (char * varName_p, char * type, unsigned int lineNumber);

/**
*
* @brief De-allocates memory assigned to user-defined data structure.
*
* @b FreeItem will de-allocate the @c string inside the user-defined data
* structure @c tableEntry.
*
* @param  theEntry_p is a pointer to the user-defined data structure.
* @return @c EXIT_SUCCESS if the item was de-allocated with no
*         problems, otherwise return @c EXIT_FAILURE.
*
* @code
*  free(theEntry_p);
*  return(EXIT_SUCCESS);
* @endcode
*
*/
int FreeItem (entry_p theEntry_p);

/**
*
* @brief De-allocates memory assigned to each user-defined data structure in
*        the hash table.
*
* @b DestroyTable will de-allocate the user-defined data structure. It also
*    deallocates memory for the hash table.
*
* @param  theTable_p is a pointer to the hast table.
* @return @c EXIT_SUCCESS if the list was de-allocated with no problems,
*         otherwise return @c EXIT_FAILURE
*
* @code
*   g_hash_table_destroy(theTable_p);
*   return(EXIT_SUCCESS);
* @endcode
*
*/
int DestroyTable (GHashTable * theTable_p);

/**
*
* @brief Inserts theEntry_p entry item to the hash table theTable_p
*
* @b InsertItem will allocate space for the item @c theEntry_p into the hash table
*    @c theTable_p
*
* @param theTable_p is a pointer to the hast table.
* @param theEntry_p is a pointer to the user-defined data structure.
* @return @c EXIT_SUCCESS if the list was de-allocated with no problems,
*         otherwise return @c EXIT_FAILURE
*
*
* @code
*   g_hash_table_insert(theTable_p, theEntry_p->name_p, theEntry_p);
*   return(EXIT_SUCCESS);
* @endcode
*
*/
int InsertItem(GHashTable * theTable_p, entry_p theEntry_p);
