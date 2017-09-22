#include "table.h"
#include "120gram_lydia.tab.h"

// typenametable functions adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/120pp/120lex.l
int typenametable_insert(char *s, int cat)
{
   struct typenametable_entry *tmp = HEAD;

   while (tmp) 
   {
      if (tmp->name && !strcmp(s, tmp->name)) {
         return tmp->category;
         }
      tmp = tmp->next;
   }
   tmp = calloc(1, sizeof (struct typenametable_entry));
   tmp->name = strdup(s);
   tmp->category = cat;
   tmp->next = HEAD;
   HEAD = tmp;
}

int typenametable_lookup(char *s)
{
   struct typenametable_entry *tmp = HEAD;
   while (tmp) {
      if (tmp->name && !strcmp(s, tmp->name)) {
         return tmp->category;
         }
      tmp = tmp->next;
   }
   return IDENTIFIER;
}

// function help from Tovah Whitesell (SEL) 
void typenametable_clear(struct typenametable_entry *type)
{ 
	if (type->next != NULL)
	{
		free(type->name); 
		type->name = NULL; 
		type->category = 0; 
		typenametable_clear(type->next); 
		free(type->next); 
		type->next = NULL;
		
	}
	else
	{
		free(type->name); 
		type->name = NULL; 
		type->category = 0;  
	}
}
