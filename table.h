#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Type name table.
 * Replace this with a hash table, when you get a chance.
 */
struct typenametable_entry {
   char *name;
   int category;
   struct typenametable_entry *next;
   } *HEAD;
   

int typenametable_insert(char *s, int cat);

int typenametable_lookup(char *s); 

void typenametable_clear(struct typenametable_entry *type); 