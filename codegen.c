#include <stdio.h>
#include "tree_lydia.h"
#include "tac.h"
#include "nonterms.h"
#include "symbol_table.h"
#include "120gram_lydia.tab.h"
#include "type.h"
#include "list.h"
#include "utility_func.h"


void codegen(struct tree * t)
{
   int j; 
   int reg; 

   if (t==NULL) 
   {
	   return;
   }
   else 
   { 
	  if (t->nkids == 0)
	  {
		  //printf("LEAF: \"%s\": %d Label: %d\n", 
				// t->leaf->text, t->leaf->category, t->label); 
	  }
	  else
	  {
		//printf("KID: %s: %d Label: %d\n", t->prodrule_name, t->nkids, t->label);
 
		for(j=0; j<t->nkids; j++)
		{
			codegen(t->kid[j]);
		}
	  }
    }
	
	switch(t->prodrule)
	{
		case INIT_DECLARATOR_1:
			if(t->kid[0] != NULL)
			{
				if(t->kid[0]->kid[0] != NULL)
				{
					printf("Func init: %s %d\n", t->kid[0]->kid[0]->leaf->text, t->kid[0]->kid[0]->leaf->address.region); 
				}
				else
				{
					printf("Norm init: %s %d\n", t->kid[0]->leaf->text, t->kid[0]->leaf->address.region);
				}
			}

			break; 
        case FUNCTION_DEFINITION_1:
		{
            struct TAC *g;
			//g = gen(R_LOCAL, t->leaf->address, t->kid[0]->leaf->address, t->kid[1]->leaf->address); 
			printf("Found func def: %s\n", t->kid[1]->kid[0]->leaf->text); 
			//t->code = concat(t->kid[0]->code, t->kid[1]->code);
            //t->code = concat(t->code, g);
			
			break;
		}
		case ADDITIVE_EXPRESSION_1:
		{
			printf("Entering code gen for add exp.\n"); 
            struct TAC *g;
			int reg;
            
            //g = gen(O_ADD, t->leaf->address,
            //t->kid[0]->leaf->address, t->kid[1]->leaf->address);
            
			//t->code = concat(t->kid[0]->code, t->kid[1]->code);

           // t->code = concat(t->code, g);
			


			break;
		}
		default:
			//printf("Default.\n");
			break;
	}


}

int new_temp_addr(struct tree *t, int type)
{
	static int size = 0;
	int j;  
	
	switch (t->leaf->category)
	{
		case INTEGER: 
		    //printf("found an int.\n"); 
			size += 8;
			return size;
			//break; 
		case FLOATING: 
		    //printf("found a float.\n"); 
			size += 16;
			return size; 
			//break; 
		case IDENTIFIER:
			//type = find_type_in_list_icg(t);
			//printf("found an id: %s %d \n", t->leaf->text, type); 
			switch(type)
			{
				case 0: // nothing 
					return 0;
				case 1: // integer
					return size += 8;
				case 2: // char
					return size += 1; 			
				case 5: // floating
					return size += 16;
				default:
				    size = 0;
					return size;
			}
			break; 
		default:
			if(t->nkids > 0)
			{
			   for(j=0; j<t->nkids; j++)
		       { 
					size = new_temp_addr(t->kid[j], type);
		       }
			}
			break;
		
	}
	
	return size; 
}
