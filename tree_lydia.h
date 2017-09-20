#ifndef TREE_LYDIA_H_
#define TREE_LYDIA_H_

#include "token.h"

// structure adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/hw2.html
typedef struct tree {
	
   char* prodrule_name; 
   int prodrule;
   int nkids;
   struct tree *kid[9];
   struct token *leaf;
} * Treeptr;


void print_tree(Treeptr, int); 

Treeptr create_tree(char*, int, int, ...); 

void free_tree(struct tree *treeptr); 

#endif /* TREE_LYDIA_H_ */
