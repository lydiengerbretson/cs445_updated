//#include "token.h"

// structure adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/hw2.html
typedef struct tree {
	
   char* prodrule_name; 
   int prodrule;
   int nkids;
   struct tree *kid[9];
   struct token *leaf;
} * Treeptr;

Treeptr create_tree(char*, int,  ...); 
void print_tree(Treeptr t, int r); 
