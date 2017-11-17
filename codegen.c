#include <stdio.h>
#include <stdlib.h>
#include "tree_lydia.h"
#include "tac.h"
#include "nonterms.h"
#include "symbol_table.h"
#include "120gram_lydia.tab.h"
#include "type.h"
#include "list.h"
#include "utility_func.h"

FILE *output;

void codegen(struct tree * t)
{
   int j; 
   //printf("Opening output.is\n");
   
   
   //int reg; 

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
					//printf("Func init: %s %d %d\n", t->kid[0]->kid[0]->leaf->text, t->kid[0]->kid[0]->leaf->address.region, t->kid[0]->kid[0]->leaf->address.offset); 
					// add to a linked list of addresses
					insert_addr_list(t->kid[0]->kid[0]->leaf->text, t->kid[0]->kid[0]->leaf->address.region,t->kid[0]->kid[0]->leaf->address.offset);
				}
				else
				{
					//printf("Norm init: %s %d %d\n", t->kid[0]->leaf->text, t->kid[0]->leaf->address.region, t->kid[0]->leaf->address.offset);
					// add to a linked list of addresses
					insert_addr_list(t->kid[0]->leaf->text, t->kid[0]->leaf->address.region, t->kid[0]->leaf->address.offset);
				}
			}

			break; 
        case FUNCTION_DEFINITION_1:
		{
            //struct TAC *g;
			//g = gen(R_LOCAL, t->leaf->address, t->kid[0]->leaf->address, t->kid[1]->leaf->address); 
			fprintf(output, "%s:\n", t->kid[1]->kid[0]->leaf->text); 
			//t->code = concat(t->kid[0]->code, t->kid[1]->code);
            //t->code = concat(t->code, g);
			
			break;
		}
		case ASSIGNMENT_EXPRESSION_1:
		{
			if(t->kid[2]->prodrule == POSTFIX_EXPRESSION_2)
			{
				fprintf(output, "call:   %s\n", t->kid[2]->kid[0]->leaf->text); 
			}
			else
			{
			printf("Found assignment exp: %s\n", t->kid[0]->leaf->text); 
			printf("Found assignment exp: %s\n", t->kid[1]->leaf->text); 
			printf("Found assignment exp: %s\n", t->kid[2]->leaf->text); 
			struct TAC_2 *g;
			struct addr *a1, *a2;
			// TODO: Grab region and offset directly from kid[0] and kid[2] instead of looking it up
		    a1 = find_addr_in_list( t->kid[0]->leaf->text); 
		    a2 = find_addr_in_list( t->kid[2]->leaf->text); 
			t->code = concat(t->kid[0]->code, t->kid[2]->code);
		    g = gen_2(O_ASN, a1, a2, NULL);
		    t->code = concat(t->code, g);
			
		    // write to file 
			fprintf(output, "asn:\n"); 
		    fprintf(output, " opcode: %d\n", t->code->opcode); 
			}
			break; 
		}
		case ADDITIVE_EXPRESSION_1:
		{
			printf("Entering code gen for add exp.\n"); 
            struct TAC_2 *g;
			struct addr *a1, *a2, *a3;
            // look up variable address in linked list of addresses

		   a1 = find_addr_in_list(t->kid[0]->leaf->text); 
		   a2 = find_addr_in_list(t->kid[2]->leaf->text); 
		   
		   printf("Found addr: %s %d %d\n", a1->var_name, a1->region, a1->offset); 
		   printf("Found addr: %s %d %d\n", a2->var_name, a2->region, a2->offset); 
		   
		   // need to figure out the dest address
		   // need to figure out t->code
		   t->code = concat(t->kid[0]->code, t->kid[2]->code);
		   g = gen_2(O_ADD, a3, a1, a2);
		   printf("g: %p\n", g); 
		   t->code = concat(t->code, g);
		   printf("t->code: %p\n", t->code); 
		    
		   // write to file 
		   fprintf(output, "add:\n"); 
		   fprintf(output, " opcode: %d\n src1 region: %d\n src2 region: %d\n src1 loc: %d\n src2 loc: %d\n", t->code->opcode, t->code->src1->region, t->code->src2->region, t->code->src1->offset, t->code->src2->offset);
		   // need to write to file only once or else everything
		   // gets messed up 
		  // printf("Printing addr list.\n"); 
		   //print_addr_list(); 
		   //print_icg_list();
		   break;
		}
		default:
		{
			//int i; 
			//t->code = NULL;
            //for(i=0;i<t->nkids;i++)
				//t->code = concat(t->code, t->kid[i]->code);      
			break;
		}
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
			// const 
			size += 8;
			return size;
			//break; 
		case FLOATING: 
			// const 
		    //printf("found a float.\n"); 
			size += 16;
			return size; 
		case STRING:
		    // string 
			return size +=8;
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

