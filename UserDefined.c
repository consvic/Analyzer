
/*

*Allan Francisco Julian Novoa
*Cesar Guadarrama cantu
*Constanza Lievanos Sanchez

*/

#include "UserDefined.h"
#include <stdio.h>
#include <stdlib.h>

	/* Prints the information of each node in the symbol table*/
int PrintItem(entry_p my_item){
	printf("Name: %s\nType: %d\n", my_item->name,my_item->type);
	if(my_item->type == integer)
		printf("%d\n",my_item->value.i_value );
	else
		printf("%f\n",my_item->value.r_value );
	return EXIT_SUCCESS;
}


int PrintSymbol(gpointer key,gpointer value, gpointer data){
		/* Print the entry and a division */
      PrintItem(value);
      printf("----------------------------\n");
      return EXIT_SUCCESS;
}

int PrintTable(GHashTable * my_table){
		/* Print a fancy title for the table */
        printf("/////////////////////////////////\n");
	    printf("Tabla de Simbolos:\n");
	    printf("/////////////////////////////////\n");
		/* Print each of the entries with the function defined in glib */
	g_hash_table_foreach(my_table,(GHFunc)PrintSymbol,NULL);
	return EXIT_SUCCESS;
}

	/* Creates the entry and then inserts it in the table */
void SymInsert(GHashTable *myTable, char * name, enum myTypes type){
	entry_p node = malloc(sizeof(struct tableEntry_));
	node->name = strdup(name);
	node->type = type;

	/* Initialize every variable as 0*/
	if(type == real)
		node->value.r_value = 0.0;
	else
		node->value.i_value = 0;
	g_hash_table_insert(myTable,node->name,node);
}

	/* Looks for an entry in the symbol table using the name as a key 	*/
	/* Returns NULL if nothing is found 								*/
entry_p SymLookUp(GHashTable *myTable, char *name){
	entry_p item 		= malloc(sizeof(entry_p));
    entry_p symEntry 	= g_hash_table_lookup(myTable,name);

    /* Duplicate the node to avoid changing the information of the nodes */
    if(symEntry != NULL){
    	item->name 		= symEntry->name;	//The name is never modified, so theres no need to duplicate it
	    item->value 	= symEntry->value;
	    item->type 		= symEntry->type; 
	    return item;	
    }
    return NULL;       
}
	/* Frees the string used in each of the entries */
void FreeItem(gpointer  my_entry){
	free(((entry_p)my_entry)->name);
}
	/* Finds an entry using the name as the key */
	/* Then, it modifies the values in the structure to Update the node */
void SymUpdate(GHashTable *myTable, char * name, enum myTypes type, union val value){
	entry_p node = g_hash_table_lookup(myTable,name);
	if(node != NULL){
		node->type = type;
		node->value = value;	
	}
}

	/* Looks for a temporal variable in the symbol table in the for t0, t1, t2, ... */
	/* When the temporal is not found, it is created and added to the symbol table  */
	/* the pointer to the structure added to the symbol table is returned			*/

entry_p newTemp(GHashTable *myTable){
	char * temp = malloc(sizeof(char *));
	char * a = malloc(sizeof(char *));
	int i = 0;	
	do{
		strcpy(temp,"t");
		snprintf(a,sizeof(char *),"%d",i);
		strcat(temp,a);		
		i++;
	}while(SymLookUp(myTable,temp)!=NULL);
	SymInsert(myTable,temp,integer);
	return SymLookUp(myTable,temp);
}
	
	/* Same process as the newTemp method, but this fills in the attributes */
entry_p newTempCons(GHashTable *myTable, union val value, enum myTypes type){
	char * temp = malloc(sizeof(char *));
	char * a = malloc(sizeof(char *));
	int i = 0;	
	do{
		strcpy(temp,"t");
		snprintf(a,sizeof(char *),"%d",i);
		strcat(temp,a);		
		i++;
	}while(SymLookUp(myTable,temp)!=NULL);
	SymInsert(myTable,temp,integer);
	SymUpdate(myTable,temp,type,value);
	return SymLookUp(myTable,temp);
}

	/* Generate and return the structure used to represent each of the instructions*/
quad_p newQuad(char * op, union result res, entry_p arg1, entry_p arg2){
	quad_p myQuad = malloc(sizeof(quad_p));
	myQuad->op = strdup(op);
	myQuad->result = res;
	myQuad->arg1 = arg1;	//Can be null
	myQuad->arg2 = arg2;	//Can be null
	return myQuad;
}

	/* Generates an array where the address is added */
GPtrArray * newList(int add){
	GPtrArray * myList = g_ptr_array_new();	
	g_ptr_array_add(myList,(gpointer)(long)add);
	return myList;
}
	
	/* Takes the addresses in list, looks up each addres in the code */
	/* and replaces whatever it is in the res.addres with the given  */
	/* parameter */
void backPatch(GPtrArray * code, GPtrArray * list, int add){
	int i;
	for(i=0;i<list->len;i++){
		long index = (long)g_ptr_array_index(list,i);
		quad_p quad = g_ptr_array_index(code,index);		
		union result res;
		res.address = add;
		quad->result = res;		
	}
}

	/* Combine two lists and return the new list generated */
GPtrArray * mergeList(GPtrArray * list1, GPtrArray * list2){
	GPtrArray * newList = g_ptr_array_new();
	int i;
	long * entry = malloc(sizeof(long));
	for (i=0; i < list1->len; i++){
		entry = (long *)g_ptr_array_index(list1,i);
		g_ptr_array_add(newList,(gpointer)entry);
	}

	for (i = 0; i < list2->len; i++){
		entry = (long *)g_ptr_array_index(list2,i);
		g_ptr_array_add(newList,(gpointer)entry);
	}
	return newList;
}

	/* Copy an existing list in a new space in memory */
GPtrArray * cloneList(GPtrArray * list){
	GPtrArray * newList = g_ptr_array_new();
	int i;
	long * entry = malloc(sizeof(long));
	for (i=0; i < list->len; i++){
		entry = (long *) g_ptr_array_index(list,i);
		g_ptr_array_add(newList,(gpointer)entry);
	}
	return newList;
}

	/* Debug function to print each quad */
void PrintQuad(quad_p myQuad){
	printf("Op: %s ", myQuad->op);
	if(strcmp(myQuad->op,"jump")==0){
		printf("Address: %d ",myQuad->result.address);
	}else if(strcmp(myQuad->op,"LT")==0||strcmp(myQuad->op,"GT")==0||strcmp(myQuad->op,"EQ")==0){
		printf("Address: %d ",myQuad->result.address);
		printf("Arg1 %s ",(myQuad->arg1)->name );
		printf("Arg2 %s ",(myQuad->arg2)->name );
	}else if(strcmp(myQuad->op,"assign")==0){
		printf("Dest %s ",myQuad->result.entry->name );
		printf("Arg1 %s ",(myQuad->arg1)->name );
	}else if(strcmp(myQuad->op,"write")==0||strcmp(myQuad->op,"read")==0){
		printf("Dest %s ",myQuad->result.entry->name );
	}else{
		printf("Dest %s ",myQuad->result.entry->name );
		printf("Arg1 %s ",(myQuad->arg1)->name );
		printf("Arg2 %s ",(myQuad->arg2)->name );
	}
	printf("\n");
}

int PrintCodeHelper(gpointer data, gpointer user_data){
	PrintQuad(data);
	printf("----------------------------\n");
    return EXIT_SUCCESS;
}
	/* Print all the code using the foreach method of g_ptr_array */
int PrintCode(GPtrArray *code){
    printf("/////////////////////////////////\n");
	printf("       Codigo Generado       \n");
    printf("/////////////////////////////////\n");
	g_ptr_array_foreach(code,(GFunc)PrintCodeHelper,NULL);
	return EXIT_SUCCESS;
}


	/* Print the list, for debugging pruposes*/
void PrintInt(long add){
	printf(" %ld ",add);
}

int PrintListHelper(gpointer data, gpointer user_data){
	PrintInt((long)data);
	printf("\n");
	return EXIT_SUCCESS;
}

void PrintList(GPtrArray * list,char * name){
	printf(" \nList %s\n",name);
	printf("=======================\n");
	g_ptr_array_foreach(list,(GFunc)PrintListHelper,NULL);
	printf("=======================\n");
}

	/* Interprete the given code using the given symbol table */
void interprete(GHashTable * my_table,GPtrArray *code){
	int i = 0;
	char * com;
	entry_p add,t1,t2;	
	union result res;	
	    printf("/////////////////////////////////\n");
	    printf("       Interprete         \n");
	    printf("/////////////////////////////////\n");

	//Here starts the interpeter from line 0 of the array of the code
	while(i<code->len){
		quad_p quad = g_ptr_array_index(code,i);							//We get the quad from the array 
		com = strdup(quad->op);												//Set the value of the operation to com
		if(strcmp(com,"assign")==0){
			add = g_hash_table_lookup(my_table,quad->result.entry->name);	//Get the variable that is going to change

			t1  = g_hash_table_lookup(my_table,quad->arg1->name);			//Get the variable that has the value to assign
			if(add->type == 1){												//*********************************************//
				if(t1->type == 1)											//							 			       //
					add->value.r_value = t1->value.r_value;					//				   Do Cohersion				   //
				else{														//				   When needed				   //
					add->value.r_value = (float)t1->value.i_value;			//						 					   //
				}															//*********************************************//
			}else{
				add->value.i_value = t1->value.i_value;						//Assign the value of t1 to the value of add
			}
		}
		if(strcmp(com,"sum")==0){
			add = g_hash_table_lookup(my_table,quad->result.entry->name);	//Get the variable to assign the addition

			t1  = g_hash_table_lookup(my_table,quad->arg1->name);			//Get the first value of the operation
			t2  = g_hash_table_lookup(my_table,quad->arg2->name);			//Get the second value of the operation

			//Check the which type are the variables and do cohersion if is necessary
			if(add->type == 1){
				if ((t1->type == 1)&&t2->type == 1){								//Real=Real+Real
					add->value.r_value = t1->value.r_value+t2->value.r_value;
				}else{
					if ((t1->type != 1)&&t2->type == 1){							//Real=Int+Real
						add->value.r_value = t1->value.i_value+t2->value.r_value;
					}else{
						if ((t1->type == 1)&&t2->type != 1){						//Real=Real+Int
							add->value.r_value = t1->value.r_value+t2->value.i_value;
						}else{														//Real=Int+Int
							add->value.r_value = t1->value.i_value+t2->value.i_value;
						}
					}
				}
			}else{
				if ((t1->type == 1)&&(t2->type == 1)){								//Int=Real+Real
						add->value.i_value = t1->value.r_value+t2->value.r_value;
				}else{																//Int=Int+Real
					if ((t1->type != 1)&&t2->type == 1){
						add->value.i_value = t1->value.i_value+t2->value.r_value;
					}else{															//Int=Real+Int
						if ((t1->type == 1)&&t2->type != 1){
						add->value.i_value = t1->value.r_value+t2->value.i_value;
						}else{														//Int=Int+Int
							add->value.i_value = t1->value.i_value+t2->value.i_value;
						}
					}
				}
			}
		}
		if(strcmp(com,"minus")==0){
			add = g_hash_table_lookup(my_table,quad->result.entry->name);	//Get the variable to assign the addition

			t1  = g_hash_table_lookup(my_table,quad->arg1->name);			//Get the first value of the operation
			t2  = g_hash_table_lookup(my_table,quad->arg2->name);			//Get the second value of the operation

			if(add->type == 1){
				if ((t1->type == 1)&&t2->type == 1){								//Real=Real-Real
					add->value.r_value = t1->value.r_value-t2->value.r_value;
				}else{
					if ((t1->type != 1)&&t2->type == 1){							//Real=Int-Real
						add->value.r_value = t1->value.i_value-t2->value.r_value;
					}else{
						if ((t1->type == 1)&&t2->type != 1){						//Real=Real-Int
							add->value.r_value = t1->value.r_value-t2->value.i_value;
						}else{														//Real=Int-Int
							add->value.r_value = t1->value.i_value-t2->value.i_value;
						}
					}
				}
			}else{
				if ((t1->type == 1)&&(t2->type == 1)){								//Int=Real-Real
						add->value.i_value = t1->value.r_value-t2->value.r_value;
				}else{
					if ((t1->type != 1)&&t2->type == 1){							//Int=Int-Real
						add->value.i_value = t1->value.i_value-t2->value.r_value;
					}else{
						if ((t1->type == 1)&&t2->type != 1){						//Int=Real-Int
						add->value.i_value = t1->value.r_value-t2->value.i_value;
						}else{														//Int=Int-Int
							add->value.i_value = t1->value.i_value-t2->value.i_value;
						}
					}
				}
			}
		}
		if(strcmp(com,"mult")==0){
			add = g_hash_table_lookup(my_table,quad->result.entry->name);	//Get the variable to assign the addition

			t1  = g_hash_table_lookup(my_table,quad->arg1->name);			//Get the first value of the operation
			t2  = g_hash_table_lookup(my_table,quad->arg2->name);			//Get the second value of the operation

			
			if(add->type == 1){
				if ((t1->type == 1)&&t2->type == 1){								//Real=Real*Real
					add->value.r_value = t1->value.r_value*t2->value.r_value;
				}else{
					if ((t1->type != 1)&&t2->type == 1){							//Real=Int*Real
						add->value.r_value = t1->value.i_value*t2->value.r_value;
					}else{
						if ((t1->type == 1)&&t2->type != 1){						//Real=Real*Int
							add->value.r_value = t1->value.r_value*t2->value.i_value;
						}else{														//Real=Int*Int
							add->value.r_value = t1->value.i_value*t2->value.i_value;
						}
					}
				}
			}else{
				if ((t1->type == 1)&&(t2->type == 1)){								//Real=Real*Real
						add->value.i_value = t1->value.r_value*t2->value.r_value;
				}else{					
					if ((t1->type != 1)&&t2->type == 1){							//Real=Int*Real
						add->value.i_value = t1->value.i_value*t2->value.r_value;
					}else{
						if ((t1->type == 1)&&t2->type != 1){						//Real=Real*Int
						add->value.i_value = t1->value.r_value*t2->value.i_value;
						}else{														//Real=Int*Int
							add->value.i_value = t1->value.i_value*t2->value.i_value;
						}
					}
				}
			}
		}
		if(strcmp(com,"div")==0){
			add = g_hash_table_lookup(my_table,quad->result.entry->name);	//Get the variable to assign the addition

			t1  = g_hash_table_lookup(my_table,quad->arg1->name);			//Get the first value of the operation
			t2  = g_hash_table_lookup(my_table,quad->arg2->name);			//Get the second value of the operation

			if(add->type == 1){
				if ((t1->type == 1)&&t2->type == 1){								//Real=Real/Real
					add->value.r_value = t1->value.r_value/t2->value.r_value;
				}else{
					if ((t1->type != 1)&&t2->type == 1){							//Real=Int/Real
						add->value.r_value = t1->value.i_value/t2->value.r_value;
					}else{
						if ((t1->type == 1)&&t2->type != 1){						//Real=Real/Int
							add->value.r_value = t1->value.r_value/t2->value.i_value;
						}else{														//Real=Int/Int
							add->value.r_value = t1->value.i_value/t2->value.i_value;
						}
					}
				}
			}else{
				if ((t1->type == 1)&&(t2->type == 1)){								//Int=Real/Real
						add->value.i_value = t1->value.r_value/t2->value.r_value;
				}else{
					if ((t1->type != 1)&&t2->type == 1){							//Int=Int/Real
						add->value.i_value = t1->value.i_value/t2->value.r_value;
					}else{
						if ((t1->type == 1)&&t2->type != 1){						//Int=Real/Int
							add->value.i_value = t1->value.r_value/t2->value.i_value;
						}else{														//Int=Int/Int
							add->value.i_value = t1->value.i_value/t2->value.i_value;
						}
					}
				}
			}
		}
		if(strcmp(com,"read")==0){
			add = g_hash_table_lookup(my_table,quad->result.entry->name);	//Get the variable where the value will be stored
			printf("\nEnter value for %s ",add->name);				//Custom instruction for the final user

			/* Store as an int o as a float depending on the type*/
			if(add->type == integer){
				int myVar;
				scanf("%d",&myVar);
				add->value.i_value = myVar;
			}else{
				float myVar;
				scanf("%f",&myVar);
				add->value.r_value = myVar;
			}
		}
		if(strcmp(com,"write")==0){
			add = g_hash_table_lookup(my_table,quad->result.entry->name);	//Get the variable to print

			printf("\n%s := ",add->name);									//Print for the final user
			/* Print in the correct format depending on the type */
			if(add->type==integer)
				printf("%d\n",add->value.i_value );
			else				
				printf("%f\n",add->value.r_value );
		}
		if(strcmp(com,"LT")==0){	
			t1  = g_hash_table_lookup(my_table,quad->arg1->name);		//Get the first value of the relation
			t2  = g_hash_table_lookup(my_table,quad->arg2->name);		//Get the second value of the relation
			//Compare the values and change the values and change the line of the code if it is true 
			if ((t1->type == 1)&&t2->type == 1){
					if((int)t1->value.r_value < (int)t2->value.r_value)
						i=quad->result.address-1;
				}else{
					if ((t1->type != 1)&&t2->type == 1){
						if(t1->value.i_value < (int)t2->value.r_value)
							i=quad->result.address-1;
					}else{
						if ((t1->type == 1)&&t2->type != 1){
							if((int)t1->value.r_value < t2->value.i_value)
								i=quad->result.address-1;
						}else{
							if(t1->value.i_value < t2->value.i_value)
								i=quad->result.address-1;
						}
					}
				}
			
		}
		if(strcmp(com,"EQ")==0){
			t1  = g_hash_table_lookup(my_table,quad->arg1->name);		//Get the first value of the relation
			t2  = g_hash_table_lookup(my_table,quad->arg2->name);		//Get the second value of the relation
			//Compare the values and change the values and change the line of the code if it is true
			if ((t1->type == 1)&&t2->type == 1){
					if((int)t1->value.r_value == (int)t2->value.r_value)
						i=quad->result.address-1;
				}else{
					if ((t1->type != 1)&&t2->type == 1){
						if(t1->value.i_value == (int)t2->value.r_value)
							i=quad->result.address-1;
					}else{
						if ((t1->type == 1)&&t2->type != 1){
							if((int)t1->value.r_value == t2->value.i_value)
								i=quad->result.address-1;
						}else{
							if(t1->value.i_value == t2->value.i_value)
								i=quad->result.address-1;
						}
					}
				}
		}
		if(strcmp(com,"GT")==0){
			t1  = g_hash_table_lookup(my_table,quad->arg1->name);		//Get the first value of the relation
			t2  = g_hash_table_lookup(my_table,quad->arg2->name);		//Get the second value of the relation
			//Compare the values and change the values and change the line of the code if it is true
			if ((t1->type == 1)&&t2->type == 1){
					if((int)t1->value.r_value > (int)t2->value.r_value)
						i=quad->result.address-1;
				}else{
					if ((t1->type != 1)&&t2->type == 1){
						if(t1->value.i_value > (int)t2->value.r_value)
							i=quad->result.address-1;
					}else{
						if ((t1->type == 1)&&t2->type != 1){
							if((int)t1->value.r_value > t2->value.i_value)
								i=quad->result.address-1;
						}else{
							if(t1->value.i_value > t2->value.i_value)
								i=quad->result.address-1;
						}
					}
				}
		}
		if(strcmp(com,"jump")==0){
			//Change the line that we are going to read next 
			i=quad->result.address-1; // add - 1 because i is augmented at the end of the while
		}
		i++;
	}
	    printf("/////////////////////////////////\n");
	    printf("       Fin del interprete        \n");
	    printf("/////////////////////////////////\n");
}