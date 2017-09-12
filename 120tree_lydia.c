// adapted from https://github.com/park2331
// adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/hw2.html

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "120tree_lydia.h"

Treeptr create_tree(char* prod_name, int num_kids, ...)
{
	Treeptr T = malloc(sizeof(struct tree) + (num_kids-1)*sizeof(Treeptr)); 
	
	int j;
	// type that holds variable arguments
	va_list args;
	// product rule name 
	char* prod_name_t = strdup(prod_name); 
	//strcpy(prod_name_t, prod_name); 
	
	printf("%s\n", prod_name); 
	
	if(T == NULL)
	{
		printf("Tree is out of memory.\n"); 
		exit(1); 
	}
	// product rule name 
	T->prodrule_name = prod_name_t; 
	// product rule number 
	// T->prodrule = category; // ????
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

void print_tree(Treeptr t, int depth)
{
  int i;
  /*if(t == NULL)
  {
	  printf("Empty tree.\n"); 
	  exit(1); 
  } */
  
  if(!t)
  {
      // do nothing
	  printf("exit 1\n"); 
  }
  else
  {
	printf("%*s %s: %d\n", depth*2, " ", t->prodrule_name, t->nkids);
    printf("exit 2\n"); 
	for(i=0; i<t->nkids; i++)
	{
		printf("exit 3\n"); 
		print_tree(t->kid[i], depth+1);

	}
  }
}

