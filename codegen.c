#include <stdio.h>
#include "tree_lydia.h"
#include "tac.h"
#include "nonterms.h"
#include "symbol_table.h"

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
            struct instr *g;
			// create places
			// t->address = .place
			// t->address[0] = .place
			// t->address[1] = .place
			// create TAC using OP_ADD
			printf("concatenating %s with %s\n", t->kid[0]->leaf->text, t->kid[1]->leaf->text);

            g = gen(O_ADD, t->address,
            t->kid[0]->address, t->kid[1]->address);
			
			t->code = concat(t->kid[0]->code, t->kid[1]->code);
            t->code = concat(t->code, g);
			//print_icg_list(t->code, g);
			printf("%d\n", t->code->opcode); 
			printf("%d\n", t->code->dest.region);
			printf("%d\n", g->dest.region);
			break;
		}
		default:
			//printf("Default.\n");
			break;
	}


}
