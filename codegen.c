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

		case ADDITIVE_EXPRESSION_1:
		{
			printf("Entering code gen for add exp.\n"); 
            struct TAC *g;
			// create places
			// t->address = .place
			// t->address[0] = .place
			// t->address[1] = .place
			// create TAC using OP_ADD
            //t->place = new_temp_addr(t->kid[0]);
			printf("Place is: %d\n", t->place); 
            g = gen(O_ADD, t->address,
            t->kid[0]->address, t->kid[1]->address);
			
			t->code = concat(t->kid[0]->code, t->kid[1]->code);
            t->code = concat(t->code, g);
			//print_icg_list(t->code, g);
			printf("%d\n", t->code->opcode); 
			//printf("%d\n", t->code->dest.region);
			//printf("%d\n", g->dest.region);
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
