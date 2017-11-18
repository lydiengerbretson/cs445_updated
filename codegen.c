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
   
   // TODO: POSTFIX_EXPRESSION_2 - Done
   // TODO: Strings...
   // TODO: JUMP_STATEMENT_1 - Done
   // TODO: RElATIONAL_EXPRESSION_1

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
	    case FUNCTION_DEFINITION_1:
		{
           //printf("Found func def!\n"); 
		   fprintf(output, "%s:\n", t->kid[1]->kid[0]->leaf->text); 
			break;
		}
		case INIT_DECLARATOR_1:
			if(t->kid[0] != NULL)
			{
				if(t->kid[0]->kid[0] != NULL)
				{

					// add to a linked list of addresses
					insert_addr_list(t->kid[0]->kid[0]->leaf->text, t->kid[0]->kid[0]->leaf->address.region,t->kid[0]->kid[0]->leaf->address.offset);
				}
				else
				{

					// add to a linked list of addresses
					insert_addr_list(t->kid[0]->leaf->text, t->kid[0]->leaf->address.region, t->kid[0]->leaf->address.offset);
				}
			}

			break; 

		case ASSIGNMENT_EXPRESSION_1:
		{
			if(t->kid[2]->prodrule == POSTFIX_EXPRESSION_2)
			{
				struct TAC_2 *g; 
                struct addr *a1, *a2; 
				int i, pc = 0; 
				a1 = find_addr_in_list(t->kid[0]->leaf->text, 0); 
				a2 = find_addr_in_list(t->kid[2]->kid[0]->leaf->text, 0); 
				int count = 0; 
				// function in type.c
				// TODO: Add to three address code - Done
				count = parameter_count(t->kid[2]);
				count = count - 3; // deducting (, ), and function name
				printf("Count: %d \n", count); 

				t->code = concat(t->kid[2]->kid[0]->code, t->kid[2]->kid[2]->code); 
				g = gen_2(O_CALL, a1, a2, NULL);
				t->code = concat(t->code, g);
				fprintf(output, "asn call: %s, %d, loc: %d\n", t->kid[2]->kid[0]->leaf->text, count, t->code->dest->offset ); 
				for(i=0; i < count; i++)
				{
					fprintf(output, "parm:  loc: %d \n", pc); 
					pc+=8; 
				}
				
			}
			else if(t->kid[2]->prodrule == ADDITIVE_EXPRESSION_1 
			     || t->kid[2]->prodrule == MULTIPLICATIVE_EXPRESSION_1)
			{
 
			struct TAC_2 *g;
			struct addr *a1, *a2, *a3;
			// TODO: Grab region and offset directly from kid[0] and kid[2] instead of looking it up
			// left side of equation 
			a3 = find_addr_in_list(t->kid[0]->leaf->text, t->kid[0]->leaf->category); 
			// right side of equation operands
		    a1 = find_addr_in_list( t->kid[2]->kid[0]->leaf->text, t->kid[2]->kid[0]->leaf->category); 
		    a2 = find_addr_in_list( t->kid[2]->kid[2]->leaf->text, t->kid[2]->kid[2]->leaf->category); 
			t->code = concat(t->kid[0]->code, t->kid[2]->code);
		    g = gen_2(O_ASN, a3, a1, a2);
		    t->code = concat(t->code, g);
			
		    // write to file 
			fprintf(output, "add: "); 
			if(t->code->src1->is_const && t->code->src2->is_const)
			{
				fprintf(output, " loc: %d const: %s const: %s\n", t->code->dest->offset,t->code->src1->var_name, t->code->src2->var_name); 
			}
			else if(t->code->src1->is_const && !t->code->src2->is_const)
			{
				fprintf(output, " loc: %d const: %s loc: %d\n", t->code->dest->offset,t->code->src1->var_name, t->code->src2->offset); 
			}
			else if(!t->code->src1->is_const && t->code->src2->is_const)
			{
				fprintf(output, " loc: %d loc: %d const: %s\n", t->code->dest->offset,t->code->src1->offset, t->code->src2->var_name); 
			}
			else 
			{
				fprintf(output, " loc: %d loc: %d loc: %d\n", t->code->dest->offset,t->code->src1->offset, t->code->src2->offset); 
			}
			}
			else
			{
				// normal assignment expression 
				struct TAC_2 *g;
				struct addr *a1, *a2;
				a1 = find_addr_in_list(t->kid[0]->leaf->text, t->kid[0]->leaf->category); 
				a2 = find_addr_in_list(t->kid[2]->leaf->text, t->kid[2]->leaf->category); 
				
				
				t->code = concat(t->kid[0]->code, t->kid[2]->code); 
				g = gen_2(O_ASN, a1, a2, NULL);
				t->code = concat(t->code, g);
				

				if(t->code->src1->is_const)
				{
					fprintf(output, "asn:  loc: %d const: %s   \n", t->code->dest->offset, t->kid[2]->leaf->text );
				}
				else
				{
					fprintf(output, "asn:  loc: %d loc: %d   \n", t->code->dest->offset, t->code->src1->offset );
				}

			}
			break; 
		}
		case JUMP_STATEMENT_1:
			printf("Found jump statement.\n"); 
			if(t->kid[1]->leaf->category != IDENTIFIER)
			{
				// TODO: Do I need to find a location for constants?
				fprintf(output, "return: const: %s   \n", t->kid[1]->leaf->text );
			}
			else
			{
				struct addr *a1; 
				struct TAC_2 *g;
				a1 = find_addr_in_list(t->kid[1]->leaf->text, t->kid[1]->leaf->category); 
				g = gen_2(O_RET, a1, NULL, NULL);
				t->code = concat(t->code, g);
				fprintf(output, "return: loc: %d  \n", t->code->dest->offset );
			}
			break; 
		case POSTFIX_EXPRESSION_2:
		{
			struct addr *a1; 
			struct TAC_2 *g;
			a1 = find_addr_in_list(t->kid[0]->leaf->text, t->kid[0]->leaf->category); 
			g = gen_2(O_CALL, a1, NULL, NULL);
			t->code = concat(t->code, g);
			
			fprintf(output, "call: loc: %s %d  \n", t->kid[0]->leaf->text, t->code->dest->offset );
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

