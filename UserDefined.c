
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
  //node->lineNumber = 0;
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

/*int FreeItem (entry_p theEntry_p){

  free(theEntry_p);
  return(EXIT_SUCCESS);
}*/
void FreeItem(gpointer  my_entry){
	free(((entry_p)my_entry)->name_p);
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
  free(entry);
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
  free(entry);
  return newList;
}

void PrintQuad(quad_p myQuad){
	printf("Op: %s ", myQuad->op);
	if(strcmp(myQuad->op,"jump")==0){
		printf("Address: %d ",myQuad->result.address);
	}else if(strcmp(myQuad->op,"LT")==0||strcmp(myQuad->op,"GT")==0||strcmp(myQuad->op,"EQ")==0){
		printf("Address: %d ",myQuad->result.address);
		printf("Arg1 %s ",(myQuad->arg1)->name_p );
		printf("Arg2 %s ",(myQuad->arg2)->name_p );
	}else if(strcmp(myQuad->op,"assign")==0){
		printf("Dest %s ",myQuad->result.entry->name_p );
		printf("Arg1 %s ",(myQuad->arg1)->name_p );
	}else if(strcmp(myQuad->op,"write")==0||strcmp(myQuad->op,"read")==0){
		printf("Dest %s ",myQuad->result.entry->name_p );
	}else{
		printf("Dest %s ",myQuad->result.entry->name_p );
		printf("Arg1 %s ",(myQuad->arg1)->name_p );
		printf("Arg2 %s ",(myQuad->arg2)->name_p );
	}
	printf("\n");
}

int PrintCodeHelper(gpointer data, gpointer user_data){
	PrintQuad(data);
	printf("----------------------------\n");
    return EXIT_SUCCESS;
}

/*
Print The code
*/
int PrintCode(GPtrArray *code){
	printf("       Code generated       \n");
	printf("================================\n");
	g_ptr_array_foreach(code,(GFunc)PrintCodeHelper,NULL);
	return EXIT_SUCCESS;
}

/*
* Interprete del Codigo Generado
*
*/
void interprete(GHashTable * theTable_p,GPtrArray *code){
	int i = 0;
	char * com;
	entry_p add,t1,t2;
	union result res;
	printf("************************************\n");
	printf("        INTERPRETACION         \n");
	printf("************************************\n");

	//Here starts the interpeter from line 0 of the array of the code
	while(i<code->len){
		quad_p quad = g_ptr_array_index(code,i);							//We get the quad from the array
		com = strdup(quad->op);												//Set the value of the operation to com
		if(strcmp(com,"assign")==0){
			add = g_hash_table_lookup(theTable_p,quad->result.entry->name_p);	//Get the variable that is going to change

			t1  = g_hash_table_lookup(theTable_p,quad->arg1->name_p);			//Get the variable that has the value to assign
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
			add = g_hash_table_lookup(theTable_p,quad->result.entry->name_p);	//Get the variable to assign the addition

			t1  = g_hash_table_lookup(theTable_p,quad->arg1->name_p);			//Get the first value of the operation
			t2  = g_hash_table_lookup(theTable_p,quad->arg2->name_p);			//Get the second value of the operation

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
			add = g_hash_table_lookup(theTable_p,quad->result.entry->name_p);	//Get the variable to assign the addition

			t1  = g_hash_table_lookup(theTable_p,quad->arg1->name_p);			//Get the first value of the operation
			t2  = g_hash_table_lookup(theTable_p,quad->arg2->name_p);			//Get the second value of the operation

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
			add = g_hash_table_lookup(theTable_p,quad->result.entry->name_p);	//Get the variable to assign the addition

			t1  = g_hash_table_lookup(theTable_p,quad->arg1->name_p);			//Get the first value of the operation
			t2  = g_hash_table_lookup(theTable_p,quad->arg2->name_p);			//Get the second value of the operation


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
			add = g_hash_table_lookup(theTable_p,quad->result.entry->name_p);	//Get the variable to assign the addition

			t1  = g_hash_table_lookup(theTable_p,quad->arg1->name_p);			//Get the first value of the operation
			t2  = g_hash_table_lookup(theTable_p,quad->arg2->name_p);			//Get the second value of the operation

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
			add = g_hash_table_lookup(theTable_p,quad->result.entry->name_p);	//Get the variable where the value will be stored
			printf("\nEnter value for %s ",add->name_p);				//Custom instruction for the final user

			/* Store as an int o as a float depending on the type*/
			if(add->type == integer){
				int myVar;
				if(scanf("%d",&myVar) == 1){
                    add->value.i_value = myVar;
                }
				else{
                    printf("Error al leer un int");
                }
			}else{
				float myVar;
				if(scanf("%f",&myVar)== 1.0){
                    add->value.r_value = myVar;
                }
                else{
                     printf("Error al leer un Flotante");
                }

			}
		}
		if(strcmp(com,"write")==0){
			add = g_hash_table_lookup(theTable_p,quad->result.entry->name_p);	//Get the variable to print

			printf("\n%s := ",add->name_p);									//Print for the final user
			/* Print in the correct format depending on the type */
			if(add->type==integer)
				printf("%d\n",add->value.i_value );
			else
				printf("%f\n",add->value.r_value );
		}
		if(strcmp(com,"LT")==0){
			t1  = g_hash_table_lookup(theTable_p,quad->arg1->name_p);		//Get the first value of the relation
			t2  = g_hash_table_lookup(theTable_p,quad->arg2->name_p);		//Get the second value of the relation
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
			t1  = g_hash_table_lookup(theTable_p,quad->arg1->name_p);		//Get the first value of the relation
			t2  = g_hash_table_lookup(theTable_p,quad->arg2->name_p);		//Get the second value of the relation
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
			t1  = g_hash_table_lookup(theTable_p,quad->arg1->name_p);		//Get the first value of the relation
			t2  = g_hash_table_lookup(theTable_p,quad->arg2->name_p);		//Get the second value of the relation
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
	printf("**************************************\n");
	printf("      Fin de Codigo         \n");
	printf("**************************************\n");
}
