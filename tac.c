/*
 * Three Address Code - skeleton for CS 445
 * Adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/tac.c
 */
 
#include <stdio.h>
#include <stdlib.h>
#include "tac.h"

struct TAC *gen(int op, struct addr a1, struct addr a2, struct addr a3)
{
  struct TAC *rv = malloc(sizeof (struct TAC));
  if (rv == NULL) {
     fprintf(stderr, "out of memory\n");
     exit(4);
     }
  rv->opcode = op;
  rv->dest = a1;
  rv->src1 = a2;
  rv->src2 = a3;
  rv->next = NULL;
  return rv;
}

struct TAC *copylist(struct TAC *l)
{
   if (l == NULL) return NULL;
   // copy first TACuction 
   struct TAC *lcopy = gen(l->opcode, l->dest, l->src1, l->src2);
   // copy the rest of the TACuctions 
   lcopy->next = copylist(l->next);
   return lcopy;
}

struct TAC *append(struct TAC *l1, struct TAC *l2)
{
	// walk down the end of l1 
   if (l1 == NULL) return l2;
   struct TAC *ltmp = l1;
   while(ltmp->next != NULL) ltmp = ltmp->next;
   ltmp->next = l2;
   return l1;
}

struct TAC *concat(struct TAC *l1, struct TAC *l2)
{
	// append l2 at the end of a copy of l1 (for a paranoid person)
	// leaving the children's linked list intact 
	// may not necessary? This is for debugging
   return append(copylist(l1), l2);
}

void print_icg_list(struct TAC *l1, struct TAC *l2)
{
	// walk down the end of l1 
   if (l1 == NULL) 
   {
	   printf("l1 is null.\n");
	   return;
   }
   struct TAC *ltmp = l1;
   if(ltmp->next == NULL)
   {
	   printf("ltmp is null.\n");
   }
   while(ltmp->next != NULL) 
   {
	   printf("Printing list.\n"); 
	   ltmp = ltmp->next;
   }

}
