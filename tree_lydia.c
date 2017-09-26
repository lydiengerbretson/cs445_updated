// adapted from https://github.com/park2331
// adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/hw2.html
// adapted from https://github.com/westrope/wookie-chomp/blob/hw3/tree.c

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "tree_lydia.h"
#include "token.h"

struct tree * create_tree_node(int category, int nkids){
  struct tree *p = (struct tree*)calloc(1, sizeof(struct tree));
  if (p == NULL) {
    printf("Tree node is out of memory.\n");
    exit(-1);
  }
  p->category = category;
  p->nkids = nkids; 
  return p;
}

struct tree * create_leaf(int category, char *text)
{
   struct tree * T = create_tree_node(category, 0);
   T->u.t.lexeme = strdup(text);
   return T;
}


struct tree * create_tree(char *name, int category, int nkids, ...)
{
   int i;
   va_list arg_list;
   // create tree node, add category and number of kids to it to it 
   struct tree *T = create_tree_node(category, nkids);
   va_start(arg_list, nkids);
   for(i=0; i<nkids; i++) 
   {
      T->u.nt.child[i] = va_arg(arg_list, struct tree *);
   }
   va_end(arg_list);

   return T;
}

void print_tree(struct tree *treeptr, int j)
{

   if (treeptr == NULL) { warn("NULL tree pointer"); return; }
   int k;
   for(k = 0; k < j; k++) printf("  ");
   printf("%d  %s \n", treeptr->category, treeptr->u.t.lexeme);
   
   if(treeptr->category >= 1000){
     int i;
     for(i = 0; treeptr->u.nt.child[i] != NULL; i++)
	 {
       print_tree(treeptr->u.nt.child[i], j+1);
     }
}
}

