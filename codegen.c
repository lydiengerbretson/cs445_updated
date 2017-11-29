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
FILE *asm_output; 

void codegen(struct tree * t)
{
   int j; 
   
   // TODO: POSTFIX_EXPRESSION_2 - Done
   // TODO: Strings...
   // TODO: More functions and parameters
   // TODO: cout << "hello world" << endl; - Done
   // TODO: JUMP_STATEMENT_1 - Done
   // TODO: RElATIONAL_EXPRESSION_1 - Done
   // TODO: While loops - Done
   // TODO: Classes

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
		   fprintf(output, ".code: %s listed above. \n", t->kid[1]->kid[0]->leaf->text); 
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
				//printf("Count: %d \n", count); 

				t->code = concat(t->kid[2]->kid[0]->code, t->kid[2]->kid[2]->code); 
				g = gen_2(O_CALL, a1, a2, NULL);
				t->code = concat(t->code, g);
				for(i=0; i < count; i++)
				{
					fprintf(output, "	param	loc: %d \n", pc); 
					pc+=8; 
				}
				fprintf(output, "	call    %s, %d, loc: %d\n", t->kid[2]->kid[0]->leaf->text, count, t->code->dest->offset ); 

				
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

			
		    // write to file 
			if(t->kid[2]->prodrule == MULTIPLICATIVE_EXPRESSION_1)
			{
				fprintf(output, "	mul"); 
				t->code = concat(t->kid[0]->code, t->kid[2]->code);
				g = gen_2(O_MUL, a3, a1, a2);
				t->code = concat(t->code, g);
			}
			else
			{
				fprintf(output, "	add"); 
				t->code = concat(t->kid[0]->code, t->kid[2]->code);
				g = gen_2(O_ADD, a3, a1, a2);
				t->code = concat(t->code, g);
			}
			if(t->code->src1->is_const && t->code->src2->is_const)
			{
				fprintf(output, "	loc: %d const: %s const: %s\n", t->code->dest->offset,t->code->src1->var_name, t->code->src2->var_name); 
			}
			else if(t->code->src1->is_const && !t->code->src2->is_const)
			{
				fprintf(output, "	loc %d const: %s loc: %d\n", t->code->dest->offset,t->code->src1->var_name, t->code->src2->offset); 
			}
			else if(!t->code->src1->is_const && t->code->src2->is_const)
			{
				fprintf(output, "	loc: %d loc: %d const: %s\n", t->code->dest->offset,t->code->src1->offset, t->code->src2->var_name); 
			}
			else 
			{
				fprintf(output, "	loc: %d loc: %d loc: %d\n", t->code->dest->offset,t->code->src1->offset, t->code->src2->offset); 
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
					fprintf(output, "	asn     loc: %d const: %s   \n", t->code->dest->offset, t->kid[2]->leaf->text );
				}
				else
				{
					fprintf(output, "	asn     loc: %d loc: %d   \n", t->code->dest->offset, t->code->src1->offset );
				}

			}
			break; 
		}
		case JUMP_STATEMENT_1:
			//printf("Found jump statement.\n"); 
			if(t->kid[1]->leaf->category != IDENTIFIER)
			{
				// TODO: Do I need to find a location for constants?
				fprintf(output, "	ret     const: %s   \n", t->kid[1]->leaf->text );
			}
			else
			{
				struct addr *a1; 
				struct TAC_2 *g;
				a1 = find_addr_in_list(t->kid[1]->leaf->text, t->kid[1]->leaf->category); 
				g = gen_2(O_RET, a1, NULL, NULL);
				t->code = concat(t->code, g);
				fprintf(output, "	ret     loc: %d  \n", t->code->dest->offset );
			}
			break; 
		case POSTFIX_EXPRESSION_2:
		{
			struct addr *a1; 
			struct TAC_2 *g;
			a1 = find_addr_in_list(t->kid[0]->leaf->text, t->kid[0]->leaf->category); 
			g = gen_2(O_CALL, a1, NULL, NULL);
			t->code = concat(t->code, g);
			fprintf(output, "	call    %s, loc: %d  \n", t->kid[0]->leaf->text, t->code->dest->offset );
			break; 
		}
		case SHIFT_EXPRESSION_1:
		{
			struct addr *a1; 
			struct TAC_2 *g;
			if(t->kid[2]->leaf->category == IDENTIFIER)
			{
				if(strcmp(t->kid[2]->leaf->text, "endl") != 0)
				{
				// location
				a1 = find_addr_in_list(t->kid[2]->leaf->text, t->kid[2]->leaf->category); 
				g = gen_2(O_SHIFT, a1, NULL, NULL);
				t->code = concat(t->code, g);
				fprintf(output, "	shift   loc: %d  \n",  t->code->dest->offset );
				}
			}
			else 
			{
				// constant 
				a1 = find_addr_in_list(t->kid[2]->leaf->text, t->kid[2]->leaf->category); 
				g = gen_2(O_SHIFT, a1, NULL, NULL);
				t->code = concat(t->code, g);
				fprintf(output, "	shift   const: %s loc: %d  \n", t->kid[2]->leaf->text, t->code->dest->offset );
			}
			break; 
		}
		case SELECTION_STATEMENT_1:
		case ITERATION_STATEMENT_1:
		{
			struct addr *a1; 
			struct TAC_2 *g;
			
			if(t->kid[2]->prodrule == EQUALITY_EXPRESSION_1
			|| t->kid[2]->prodrule == RELATIONAL_EXPRESSION_1)
			{
               //BLT,...	
			   //if x rop y then goto L 	
			   //binary conditional jump to L 
			    struct addr *a2;
				a1 = find_addr_in_list(t->kid[2]->kid[0]->leaf->text, t->kid[2]->kid[0]->leaf->category); 
				a2 = find_addr_in_list(t->kid[2]->kid[2]->leaf->text, t->kid[2]->kid[2]->leaf->category); 
				struct addr *L1 = malloc(sizeof(struct addr)); 
				L1->var_name = "label_1";
				L1->region = R_LABEL;
				L1->offset = 0; 
				L1->next = NULL; 
				g = gen_2(O_BLT, L1, a1, a2); 
				t->code = concat(t->code, g); 
				fprintf(output, "	blt     loc: %d, goto: %d \n", t->code->dest->offset, t->code->dest->region );
			}
			else if(t->kid[2]->prodrule == EQUALITY_EXPRESSION_2)
			{
				// inequality expression
				// BNIF	if !x then goto L 	
				// unary negative conditional jump to L 
				struct addr *a2;
				a1 = find_addr_in_list(t->kid[2]->kid[0]->leaf->text, t->kid[2]->kid[0]->leaf->category); 
				a2 = find_addr_in_list(t->kid[2]->kid[2]->leaf->text, t->kid[2]->kid[2]->leaf->category); 
				struct addr *L1 = malloc(sizeof(struct addr)); 
				L1->var_name = "label_1";
				L1->region = R_LABEL;
				L1->offset = 0; 
				L1->next = NULL; 
				g = gen_2(O_BNIF, L1, a1, a2); 
				t->code = concat(t->code, g); 
				fprintf(output, "	bnif    loc: %d, goto: %d \n", t->code->dest->offset, t->code->dest->region );
			}
			else
			{
				// BIF	if x then goto L 	
				// unary conditional jump to L 
				a1 = find_addr_in_list(t->kid[2]->leaf->text, t->kid[2]->leaf->category); 
				struct addr *L1 = malloc(sizeof(struct addr)); 
				L1->var_name = "label_1";
				L1->region = R_LABEL;
				L1->offset = 0; 
				L1->next = NULL; 
				g = gen_2(O_BIF, L1, a1, NULL); 
				t->code = concat(t->code, g); 
				fprintf(output, "	bif     %s, loc: %d, goto: %d \n", t->kid[2]->leaf->text, t->code->dest->offset, t->code->dest->region );
			}
			
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

void finalgen(struct tree *t)
{
   int j; 
   int a;
   if (t==NULL) 
   {
	   return;
   }
   else 
   { 
	  if (t->nkids == 0)
	  {
          // do nothing
	  }
	  else
	  {
 
		for(j=0; j<t->nkids; j++)
		{
			finalgen(t->kid[j]);
		}
	  }
    }
	// final code generation here 
	if(t->code)
	{
		switch(t->code->opcode)
		{	
			case O_ADD:
				/*printf("Found ADD: \n"); 
				// write to .s file
				printf(" t->code->dest->region: %d\n t->code->dest->offset: %d\n t->code->dest->var_name: %s\n", t->code->dest->region, t->code->dest->offset,t->code->dest->var_name);  
				printf(" t->code->src1->region: %d\n t->code->src1->offset: %d\n t->code->src1->var_name: %s\n", t->code->src1->region, t->code->src1->offset,t->code->src1->var_name); 
				printf(" t->code->src2->region: %d\n t->code->src2->offset: %d\n t->code->src2->var_name: %s\n", t->code->src2->region, t->code->src2->offset,t->code->src2->var_name); */
				
			    // local variables
				// TODO: global variables
				fprintf(asm_output, "---ADDITION---\n"); 
				if(t->code->dest->region == R_LOCAL)
				{
					// move first operand to a register
					if(t->code->src1->is_const)
					{
						// make float an integer
						// type "promote" it to an integer... 
						a = atoi( t->code->src1->var_name); 
						fprintf(asm_output, "\t movl    $%d, %%eax\n", a);
						//printf("%s \n", t->code->src1->var_name);

					}
					else
					{
						fprintf(asm_output, "\t movl    -%d(%%rbp), %%eax\n", t->code->src1->offset);
					}

					// move second operand to a register 
					if(t->code->src2->is_const)
					{
						// make float an integer
						// type "promote" it to an integer... 
						a = atoi( t->code->src2->var_name); 
						fprintf(asm_output, "\t movl    $%d, %%edx\n", a);
					}
					else
					{
						fprintf(asm_output, "\t movl    -%d(%%rbp), %%edx\n", t->code->src2->offset);
					}
					// not using leal..
					fprintf(asm_output, "\t addl    %%edx, %%eax\n");
					fprintf(asm_output, "\t movl    %%eax, -%d(%%rbp)\n", t->code->dest->offset);
				}
				if(t->code->dest->region == R_GLOBAL)
				{
					// do something
				}
				break; 
			case O_ASN:
				printf("Found ASN: \n"); 
				// write to .s file
				//movl -4(%rbp), %eax
                //movl %eax, -8(%rbp) 
				printf(" t->code->dest->region: %d\n t->code->dest->offset: %d\n t->code->dest->var_name: %s\n t->code->src1->var_name: %s\n", t->code->dest->region, t->code->dest->offset,t->code->dest->var_name, t->code->src1->var_name); 
				fprintf(asm_output, "---ASSIGNMENT---\n"); 
				if(t->code->src1->is_const)
				{
					
					if(t->kid[2]->leaf->category == CHARACTER)
					{
						// change character to ascii :)
						int ascii;
                        ascii = (int)t->kid[2]->leaf->text[1]; 
						fprintf(asm_output, "\t movl    $%d, %%eax\n", ascii);
					}
					else
					{
						// then it is a integer or float 
						a = atoi( t->code->src1->var_name); 
						fprintf(asm_output, "\t movl    $%d, %%eax\n", a);
					}

				}
				else
				{
					fprintf(asm_output, "\t movl    -%d(%%rbp), %%eax\n", t->code->src1->offset);
					
				}
				fprintf(asm_output, "\t movl    %%eax, -%d(%%rbp)\n", t->code->dest->offset); 
				break;
			case O_SHIFT:
			    //         movl    $_ZSt4cout, %edi
                //         call    _ZNSolsEi
                //         movl    $_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
                //         movq    %rax, %rdi
                //         call    _ZNSolsEPFRSoS_E
				// if the thing that is being outputted is an integer... 
				fprintf(asm_output, "---SHIFT: %s \n", t->code->dest->var_name); 
				fprintf(asm_output, "\t movl    $_ZSt4cout, %%edi\n");
				fprintf(asm_output, "\t call    _ZNSolsEi\n"); 
				break;
			case O_CALL:
				printf("Found CALL: \n"); 
				// write to .s file 
				//printf(" t->code->dest->region: %d\n t->code->dest->offset: %d\n t->code->dest->var_name: %s\n", t->code->dest->region, t->code->dest->offset,t->code->dest->var_name); 
				break;
			case O_BIF: 
				printf("Found BIF: \n"); 
				// write to .s file
				//printf(" t->code->dest->region: %d\n t->code->dest->offset: %d\n t->code->dest->var_name: %s\n", t->code->dest->region, t->code->dest->offset,t->code->dest->var_name); 
				break;
			default: 
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
		case CHARACTER: 
			size += 1; // should this be 5??
			return size; 
		case INTEGER: 
		    //printf("found an int.\n"); 
			// const 
			size += 4;
			return size;
			//break; 
		case FLOATING: 
			// const 
		    //printf("found a float.\n"); 
			size += 8;
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
					return size += 4;
				case 2: // char
					return size += 1; 			
				case 5: // floating
					return size += 8;
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

