/*
 * Three Address Code - skeleton for CS 445
 * Adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/tac.c
 */
 
#include <stdio.h>
#include <stdlib.h>
#include "tac.h"

struct TAC_2 *gen_2(int op, struct addr *a1, struct addr *a2, struct addr *a3)
{
  struct TAC_2 *rv = malloc(sizeof (struct TAC_2));
  if (rv == NULL) {
     fprintf(stderr, "out of memory\n");
     exit(4);
     }
  rv->opcode = op;
  rv->dest = a1;
  rv->src1 = a2;
  rv->src2 = a3;
  rv->next = NULL;

  return rv;
}

struct TAC_2 *copylist(struct TAC_2 *l)
{
   if (l == NULL) return NULL;
   // copy first TAC_2uction 
   struct TAC_2 *lcopy = gen_2(l->opcode, l->dest, l->src1, l->src2);
   // copy the rest of the TAC_2uctions 
   lcopy->next = copylist(l->next);
   return lcopy;
}

struct TAC_2 *append(struct TAC_2 *l1, struct TAC_2 *l2)
{
	// walk down the end of l1 
   if (l1 == NULL) 
   {
	   printf("*Appending to list \n"); 
	   return l2;
   }
   struct TAC_2 *ltmp = l1;
   //struct TAC_2 *ltmp = start_TAC;
   while(ltmp->next != NULL) 
   {
	   printf("***Appending to list.\n"); 
	   ltmp = ltmp->next;
   }
    printf("*Appending to list.\n"); 
   ltmp->next = l2;
   return l1;
}

struct TAC_2 *concat(struct TAC_2 *l1, struct TAC_2 *l2)
{
	// append l2 at the end of a copy of l1 (for a paranoid person)
	// leaving the children's linked list intact 
	// may not necessary? This is for debugging
   return append(copylist(l1), l2);
}

void print_icg_list()
{
	struct TAC_2 *temp; 
	if(start_addr == NULL)
	{
        return; 
	}
	else
	{
		temp = start_TAC; 
		while(temp->next != NULL)
		{
		    printf("***Printing list.\n"); 
			temp = temp->next; 
		}
		
	}

}

void insert_addr_list(char *s, int region, int offset)
{

    struct addr *new_node = calloc(1, sizeof(struct addr *));
	
	struct addr  *temp;
    new_node->var_name = strdup(s); 
	new_node->region = region;	
	new_node->offset = offset;
	new_node->next = NULL; 
	
	if(start_addr == NULL)
	{

		start_addr = new_node; 
		//curr = new_node;
	}
	else
	{
		temp = start_addr; 
		while(temp->next != NULL)
		{
			temp = temp->next; 
		}
		//printf("**Inserting the rest: %s\n", new_node->name); 
		temp->next = new_node; 
	}
	

}

void print_addr_list()
{

	struct addr *temp;
	temp = start_addr; 
	
	while(temp != NULL)
	{

		printf(" Name: %s Region: %d Offset: %d\n", temp->var_name, temp->region, temp->offset); 
		//return true; 
		temp = temp->next;
		
	}
	//return false;
		
}

struct addr* find_addr_in_list(char *var_name, int cat)
{
	struct addr *temp;
	temp = start_addr; 
	static int temp_region = 0; 
	
		// TODO: Need to know what region it is in!
		// Possibly look it up in symbol table instead
	while(temp != NULL)
	{
		if(strcmp(var_name, temp->var_name) == 0 )
		{
		//printf("**Printing the wanted symbol: %s type: %d\n", temp->name, temp->typ); 
		temp_region = temp->region; 
		return temp; 
		}
		temp = temp->next;
		//printf("What is the current region? %d\n", temp->region); 
		
		
	}
	if(temp == NULL)
	{
		printf("Maybe a constant!!\n"); 
		// create new temp node w/ constant info ?? 
		// add this new node to list and then return it?? 
		struct addr *new_node = calloc(1, sizeof(struct addr *));
	
        new_node->var_name = strdup("const"); 
	    new_node->region = temp_region;	
		switch(cat)
		{
			case 259:
				new_node->offset = 8;
				break; 
			case 260: 
				new_node->offset = 16;
				break; 
			default:
				new_node->offset = 0; 
				break; 
		}
	    
	    new_node->next = NULL; 
		temp = new_node; 
	}
	return temp;
}


