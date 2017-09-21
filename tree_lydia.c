// adapted from https://github.com/park2331
// adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/hw2.html

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "tree_lydia.h"
#include "token.h"

Treeptr create_tree(char* prod_name, int category, int num_kids, ...)
{
    va_list args;
	struct tree *T = malloc(sizeof(struct tree));
	int j;
	
	if(T == NULL)
	{
		printf("Tree is out of memory.\n"); 
		exit(1); 
	}
	// product rule name 
	char* prod_name_t = strdup(prod_name); 
	T->prodrule_name = prod_name_t; 
	// product rule number 
	T->prodrule = category; // add as a parameter
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
	  //printf("exit 1\n"); 
  }
  else 
  { 
	  if (treeptr->nkids == 0)
	  {
		  printf("%*s  Leaf: \"%s\"  %d\n", depth*2, " ", 
				 treeptr->leaf->text, treeptr->leaf->category); 
	  }
	  else
	  {
		printf("%*s  Kid: %s: %d\n", depth*2, " ", treeptr->prodrule_name, treeptr->nkids);
 
		for(j=0; j<treeptr->nkids; j++)
		{
			print_tree(treeptr->kid[j], depth+1);
		}
	  }
}

}

void free_tree(struct tree *treeptr)
{
	int idx = 0; 
	if(treeptr != NULL)
	{ 
		treeptr->prodrule = 0; 
		treeptr->nkids = 0; 
		//printf("freeing %s \n", treeptr->prodrule_name); 
		free(treeptr->prodrule_name); 
		treeptr->prodrule_name = NULL;
		
		/*treeptr->leaf->lineno = 1; 
		treeptr->leaf->ival = 0; 
		treeptr->leaf->fval = 0; 
		
		free(treeptr->leaf->text); 
		treeptr->leaf->text = NULL; 
		free(treeptr->leaf->filename); 
		treeptr->leaf->filename = NULL;
		free(treeptr->leaf->sval); 
		treeptr->leaf->sval = NULL; */
		
		while(idx < 9)
		{
			free_tree(treeptr->kid[idx]); 
			treeptr->kid[idx] = NULL; 
			idx++; 
		}
	}
}

