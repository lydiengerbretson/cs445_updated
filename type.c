#include "type.h"
#include "120gram_lydia.tab.h"
#include "nonterms.h"

// function adapted from https://github.com/westrope/wookie-chomp/blob/hw3/Hw2/prodrule.c
int get_base_type( struct tree *t)
{

	if(strcmp(t->leaf->text, "string") == 0)
		return STRING_TYPE;
	if(strcmp(t->leaf->text, "ofstream") == 0)
		return OFSTREAM_TYPE;
	if(strcmp(t->leaf->text, "LYDIA") == 0)
		return 1234; // testing purposes

	switch(t->prodrule)
	{
		
	case INT:
		return INT_TYPE;
	case DOUBLE:
		return DOUBLE_TYPE;
	case CHAR:
		return CHAR_TYPE;
	case VOID:
		return VOID_TYPE;
	case BOOL:
		return BOOL_TYPE;
	default:
		return 0;
	}

  
}

void type_add_check(struct tree *t, char *table_name)
{
	int k;
	int type1; 
	int type2;

	// find type of left side of expression 
	type1 = find_type_in_list(t->kid[0]->leaf->text, table_name);
	if(type1 != INT_TYPE && type1 != DOUBLE_TYPE)
	{
		printf("ERROR!!\n");
	}
    printf("Type of assigner %s : %d\n", t->kid[0]->leaf->text, type1);
	for(k=0; k<t->kid[2]->nkids; k++)
	{
		// only works for two operands on right side
		if(t->kid[2]->kid[k]->prodrule == 258)
		{
			// check and compare these types to assigner
			type2 = find_type_in_list(t->kid[2]->kid[k]->leaf->text, table_name);
			// possibly be recursive 
			printf("Variables in add/mult expr: %s %d\n", t->kid[2]->kid[k]->leaf->text, type2);
			if(type2 != type1)
			{
				semanticerror("Types do not match.", t);
			}
		}
	}


}

void type_relation_check(struct tree *t, char *table_name)
{
	int k;
	int type1; 
	int type2;

	// find type of left side of expression 
    type1 = find_type_in_list(t->kid[2]->kid[0]->leaf->text, table_name);
	printf("Checking bool/int: %s, %d\n", t->kid[2]->kid[0]->leaf->text, type1);
	type2 = find_type_in_list(t->kid[2]->kid[2]->leaf->text, table_name);
	printf("Checking bool/int: %s, %d\n", t->kid[2]->kid[2]->leaf->text, type2);
    if(type1 != INT_TYPE 
	&& type1 != BOOL_TYPE
	&& type1 != DOUBLE_TYPE
	&& type2 != INT_TYPE
	&& type2 != BOOL_TYPE
	&& type2 != DOUBLE_TYPE)
	{
		semanticerror("ERROR, you need a bool/int type!!\n", t);
	}
	if(type1 == INT_TYPE && t->kid[2]->kid[2]->leaf->category != INTEGER)
	{
		// error
		semanticerror("Ints: Types do not match.", t);
	}	
    else if(type1 == BOOL_TYPE 
	     && type2 != BOOL_TYPE 
		 && t->kid[2]->kid[2]->leaf->category != TRUE
         && t->kid[2]->kid[2]->leaf->category != FALSE)
    {
	   // error
	   semanticerror("Bools: Types do not match.", t);
    }
    else if(type1 == DOUBLE_TYPE 
	     && type2 != DOUBLE_TYPE 
		 && t->kid[2]->kid[2]->leaf->category != FLOATING
         && t->kid[2]->kid[2]->leaf->category != DOUBLE)
    {
	   // error
	   semanticerror("Doubles: Types do not match.", t);
    }		
	else if(type1 != type2 
	     && t->kid[2]->kid[2]->leaf->category != INTEGER 
	     && t->kid[2]->kid[2]->leaf->category != TRUE
         && t->kid[2]->kid[2]->leaf->category != FALSE
		 && t->kid[2]->kid[2]->leaf->category != FLOATING
         && t->kid[2]->kid[2]->leaf->category != DOUBLE)
	{
		semanticerror("Types do not match.", t);
	}
	


}

