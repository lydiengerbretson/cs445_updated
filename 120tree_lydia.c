// adapted from https://github.com/park2331
// adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/hw2.html

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "120tree_lydia.h"

void print_tree(Treeptr t, int depth)
{
  int i;

  printf("%*s %s: %d\n", depth*2, " ", t->prodrule_name, t->nkids);
  
  for(i=0; i<t->nkids; i++)
  {
	  print_tree(t->kids[i], depth+1);
  }
}

Treeptr create_tree(char* prod_name, int category, int num_kids, ...)
{
	Treeptr T = malloc(sizeof(struct tree) + (num_kids-1)*sizeof(Treeptr)); 
	
	int j;
	va_list args;
	char* prod_name_t = malloc(strlen(prod_name) + 1); 
	strcpy(prod_name_t, prod_name); 
	
	printf("%s\n", prod_name); 
	
	if(T == NULL)
	{
		printf("Tree is out of memory.\n"); 
		exit(1); 
	}
	
	T->prodrule_name = prod_name_t; 
	T->prodrule = category; 
	T->nkids = num_kids; 
	
	

}