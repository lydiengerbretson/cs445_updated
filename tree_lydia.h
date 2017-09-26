#ifndef TREE_LYDIA_H_
#define TREE_LYDIA_H_

#include "token.h"

// https://github.com/westrope/wookie-chomp/blob/hw3/Hw2/tree.h
typedef struct tree{
  int category;
  int nkids; 
  union {
    struct leaf {
      char *lexeme;
      int val;
    } t;
    struct nonleaf {
      struct tree *child[9];
    } nt;
  } u;
} tree;


void print_tree(struct tree *np, int j); 
struct tree * create_tree_node(int category, int nkds);
struct tree * create_leaf( int category, char *text);
struct tree * create_tree(char * name, int category, int nkids, ...);

#endif /* TREE_LYDIA_H_ */
