#include <stdio.h>
#include "tree_lydia.h"
#include "tac.h"

void codegen(struct tree * t)
{
   int i;
   if (t==NULL) return;

   /*
    * this is a post-order traversal, so visit children first
    */
   for(i=0;i<t->nkids;i++)
      codegen(t->kid[i]);

   /*
    * back from children, consider what we have to do with
    * this node. The main thing we have to do, one way or
    * another, is assign t->code
    */
   switch (t->label) {
   case O_ADD: {
      struct instr *g;
      t->code = concat(t->kid[0]->code, t->kid[1]->code);
	  // assuming that you already allocated t->address and labels as needed
      g = gen(O_ADD, t->address,
              t->kid[0]->address, t->kid[1]->address);
      t->code = concat(t->code, g);
      break;
      }
   /*
    * ... really, a bazillion cases, up to one for each
    * production rule (in the worst case)
    */
   default:
      /* default is: concatenate our children's code */
      t->code = NULL;
      for(i=0;i<t->nkids;i++)
         t->code = concat(t->code, t->kid[i]->code);
   }
}
