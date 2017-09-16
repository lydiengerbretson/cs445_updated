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
	Treeptr T = malloc(sizeof(struct tree) + (num_kids-1)*sizeof(Treeptr)); 
	
	int j;
	// type that holds variable arguments

	// product rule name 
	char* prod_name_t = strdup(prod_name); 
	//strcpy(prod_name_t, prod_name); 
	
	//printf("%s\n", prod_name); // testing purposes
	
	if(T == NULL)
	{
		printf("Tree is out of memory.\n"); 
		exit(1); 
	}
	// product rule name 
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
		  //printf("exit 0\n"); 
		  printf("%*s  Leaf: %d  %s\n", depth*2, " ", 
				treeptr->leaf->category, treeptr->leaf->text); 
	  }
	  else
	  {
		printf("%*s  Kid: %s: %d\n", depth*2, " ", treeptr->prodrule_name, treeptr->nkids);
		//printf("exit 2\n"); 
		for(j=0; j<treeptr->nkids; j++)
		{
			//printf("exit 3\n"); 
			print_tree(treeptr->kid[j], depth+1);

		}
	  }
}
}

