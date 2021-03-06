
#include <glib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

enum myTypes {integer, real};			/* Types to be used. Boolean = integer*/

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
*
* The @c value is a union of all possible values (integer/float). Not space
*    efficient if smaller types are allowed.
*
*/
typedef struct tableEntry_{
    char *          name;            /**< The name is just the string */
    enum myTypes	type;                          /**< Identifier type */
    union val        value;       /**< Value of the symbol table element */
    GPtrArray *		list_true;
	GPtrArray *		list_false;
	GPtrArray *		list_next;

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
int PrintItem(entry_p my_item);

/**
*
* @brief Gets the key, value and data pointers from the @c g_hash_foreach
* and calls PrintItem for each element.
*
* @b PrintSymbol is a support function that captures the key, value and
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
int PrintSymbol(gpointer key,gpointer value, gpointer data);

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
int PrintTable(GHashTable * my_table);

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
//entry_p NewItem (char * varName_p, char * type, unsigned int lineNumber);

/**
*
* @brief Lookup item by name to the symbol table
*
* @b SymbolLookUp will check in the hash table if the item exists
*
* @param theTable_p is a pointer to the hast table.
* @param name is a string with the name of the item.
* @return @c the entry on the table if the item is found and NULL if not found
*
*
* @code
*   entry_p item = malloc(sizeof(entry_p));
*   entry_p symEntry = g_hash_table_lookup(theTable_p,name);
*
*   if(symEntry!= NULL){
*
*      item->name_p 		= symEntry->name_p;
*      item->value 	= symEntry->value;
*      item->type 		= symEntry->type;
*      return item;
*    }
*    return NULL;
* @endcode
*
*/
entry_p SymLookUp(GHashTable *myTable, char *name);

/**
*
* @brief Allocates memory to a node of the user-defined data structure.
*
* @b InsertSymbol will allocate space for the @c name string and set the
* fields of the @c tableEntry structure.
*
* @param theTable_p is a pointer to the hast table.
* @param  name is the input string of the name.
* @param  type is the variable type (integer or real).
* @param  lineNumber line where the variable was declared.
* @return @c void
*
* @code
*    entry_p node = malloc(sizeof(struct tableEntry_));
*    node->name_p = name;
*    node->type = type;
*    node->lineNumber = lineNumber;
*    if(type == real)
*    node->value.r_value = 0.0;
*    else
*    node->value.i_value = 0;
*    g_hash_table_insert(theTable_p,node->name_p,node);
* @endcode
*
*/
void SymInsert(GHashTable *myTable, char * name, enum myTypes type);

/**
*
* @brief Allocates memory to a node of the user-defined data structure.
*
* @b InsertSymbolTemp will allocate space for the @c varName_p string and set the
* fields of the @c tableEntry structure without the line number.
*
* @param theTable_p is a pointer to the hast table.
* @param  name is the input string of the name.
* @param  type is the variable type (integer or real).
* @return @c void
*
* @code
*    entry_p node = malloc(sizeof(struct tableEntry_));
*    node->name_p = name;
*    node->type = type;
*    if(type == real)
*    node->value.r_value = 0.0;
*    else
*    node->value.i_value = 0;
*    g_hash_table_insert(theTable_p,node->name_p,node);
* @endcode
*
*/
void InsertSymbolTemp(GHashTable *theTable_p, char * name, enum myTypes type);

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
void FreeItem(gpointer my_entry);

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

/**
*
* @brief Create temporal empty variable to the hash table
*
* @b newTemp will create a temporal variable into the hash table
*
* @param theTable_p is a pointer to the hast table.
*
* @return returns the created entry_p
*
* @code
*   char * temp = malloc(sizeof(char *));
*   char * c = malloc(sizeof(char *));
*   int i = 0;
*   do{
*     strcpy(temp,"t");
*     snprintf(c,sizeof(char *),"%d",i);
*     strcat(temp,c);
*     i++;
*   }while(SymbolLookUp(theTable_p,temp) != NULL);
*   InsertSymbolTemp(theTable_p,temp,integer);
*   return SymbolLookUp(theTable_p,temp);
* @endcode
*
*/
entry_p newTemp(GHashTable *myTable);

/**
*
* @brief Create temporal constant variable to the hash table
*
* @b newTempConstant will create a temporal variable into the hash table
*
* @param theTable_p is a pointer to the hast table.
* @param value is the value of the constant
* @param type is the variable type (integer or real).
*
* @return returns the created entry_p
*
* @code
*   char * temp = malloc(sizeof(char *));
*   char * c = malloc(sizeof(char *));
*   int i = 0;
*   do{
*     strcpy(temp,"t");
*     snprintf(c,sizeof(char *),"%d",i);
*     strcat(temp,c);
*     i++;
*   }while(SymbolLookUp(theTable_p,temp) != NULL);
*   SymbolUpdate(theTable_p,temp,type,value);
*   InsertSymbolTemp(theTable_p,temp,integer);
*   return SymbolLookUp(theTable_p,temp);
* @endcode
*
*/
entry_p newTempCons(GHashTable *myTable, union val value, enum myTypes type);

/**
*
* @brief Updates value and type of a symbol in the table
*
* @b SymbolUpdate will update the value and type of an entry
*
* @param theTable_p is a pointer to the hast table.
* @param  name is the input string of the name.
* @param type is the variable type (integer or real).
* @param value is the value of the constant
* @return @c void
*
* @code
*   entry_p node = g_hash_table_lookup(theTable_p,name);
*   if(node != NULL){
*   node->type = type;
*   node->value = value;
* @endcode
*
*/

void SymUpdate(GHashTable *myTable, char * name, enum myTypes type, union val value);
/**
*
* @union result
*
* @brief Defines the address
*
* The @c result union defines tthe address of the quad
*
*/
union result{
	int 	address;
	entry_p entry;
};
/**
*
* @struct quad
*
* @brief This is the quad structure
*
* The @c quad is where the code will be store. Each entry has the following fields:
*
* The @c op is a string holding the opperation.
*
* The @c result indicates the union result of the wuad
*
* 
*/

typedef struct quad_{
	char *			op;
	union result	result;
	entry_p 		arg1;
	entry_p			arg2;
}quad;

/**
*
* @typedef quad_
*
* @brief pointer to the @c quad @c structure
*
*/
typedef struct quad_ * quad_p;
/**
*
* @brief Inserts a new quad to the quad structure
*
* 
*
* @param char * op is the operation
* @param result is the union result
* @param arg1 is the pointer to the first Argument
* @param arg2 is the pointer to the second Argument
*
*@code
*quad_p newQuad(char * op, union result res, entry_p arg1, entry_p arg2){
*	quad_p myQuad = malloc(sizeof(quad_p));
*	myQuad->op = strdup(op);
*	myQuad->result = res;
*	myQuad->arg1 = arg1;	//Can be null
*	myQuad->arg2 = arg2;	//Can be null
*	return myQuad;
*  }
* @endCode
*
*
*/
quad_p newQuad(char * op, union result res, entry_p arg1, entry_p arg2);
/**
*
* @typedef newList
*
* @brief Lista de todas Las direcciones
*
*/
GPtrArray * newList(int add);
/**
*
* @typedef cloneList
*
* @brief Copia de las lsitas de direcciones
*
*/
GPtrArray * cloneList(GPtrArray * list);
/**
*
* @typedef mergeList
*
* @brief Lista que combina las lista 1 y dos

*
*/
GPtrArray * mergeList(GPtrArray * list1, GPtrArray * list2);
/**
*
* @brief Generates Backpatch
*
* 
*
* @param GPtrArray * code receives the code
* @param list receives the list of address
*
*
*@code
*void backPatch(GPtrArray * code, GPtrArray * list, int add){
*	int i;
*	for(i=0;i<list->len;i++){
*		long index = (long)g_ptr_array_index(list,i);
*		quad_p quad = g_ptr_array_index(code,index);		
*		union result res;
*		res.address = add;
*		quad->result = res;		
*	}
*}
*
* @endCode
*
*
*/
void backPatch(GPtrArray * code, GPtrArray * list, int add);
/**
*
* @typedef PrintQuad
*
* @brief Prints all the quads

*
*/
void PrintQuad(quad_p myQuad);								
/**
*
* @typedef PrintCodeHelper
*
* @brief prints the data from the quad

*
*/															
int PrintCodeHelper(gpointer data, gpointer user_data);		
/**
*
* @typedef PrintCode
*
* @brief Prints the array of code

*
*/															
int PrintCode(GPtrArray *code);	
/**
*
* @typedef interprete
*
* @brief function that acts as an interprete of the code generation
*
* @param GHashTable * theTable_p receives the symbol table
* @param GPtrArray * code receives the code generated
*/	
void interprete(GHashTable * theTable_p,GPtrArray *code);
