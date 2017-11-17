// adapted from https://github.com/park2331
// adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/hw2.html

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "tree_lydia.h"
#include "token.h"
#include "symbol_table.h"

Treeptr create_tree(char* prod_name, int category, int num_kids, ...)
{
    va_list args;
	struct tree *T = malloc(sizeof(struct tree)); // possibly cast this malloc 
	
	//struct TAC_2 *code = calloc(1, sizeof(struct TAC*)); 
	//struct addr *address = calloc(1, sizeof(struct addr*));
	int j;
	
	if(T == NULL)
	{
		printf("Tree is out of memory.\n"); 
		exit(1); 
	}
	// product rule name 
	//char* prod_name_t = (char*)calloc(1, 2048);
	char *prod_name_t = strdup(prod_name); 
	T->prodrule_name = prod_name_t; 
	// product rule number 
	T->prodrule = category; 
	// number of kids
	T->nkids = num_kids; 

	// add any children to tree
	if(num_kids > 0)
	{
		// initialize variable argument list 
		va_start(args, num_kids); 
		
		for(j=0; j < num_kids; j++)
		{
			// assign child to value of current argument in variable list 
			T->kid[j] = va_arg(args, struct tree *); 
		}
		
		va_end(args); 
	}
	
	return T; 	

}

void print_tree(struct tree *treeptr, int depth)
{
  int j;
  
  if(!treeptr)
  {
      // do nothing
  }
  else 
  { 
	  if (treeptr->nkids == 0)
	  {
		  printf("%*s LEAF: \"%s\": %d region: %d\n", depth*2, " ", 
				 treeptr->leaf->text, treeptr->leaf->category, treeptr->leaf->address.region); 
	  }
	  else
	  {
		printf("%*s KID: %s: %d\n", depth*2, " ", treeptr->prodrule_name, treeptr->nkids);
 
		for(j=0; j<treeptr->nkids; j++)
		{
			print_tree(treeptr->kid[j], depth+1);
		}
	  }
}

}


