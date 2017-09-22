#include "table.h"
#include "120gram_lydia.tab.h"

int typenametable_insert(char *s, int cat)
{
   struct typenametable_entry *tmp = HEAD;

   while (tmp) 
   {
	  //if(tmp->name != NULL)
	  //{
      if (tmp->name && !strcmp(s, tmp->name)) {
         //fprintf(stderr,
	    //"warning, %s is already in the typename table as a %d\n",
	   // s, tmp->category );
         return tmp->category;
         }
      tmp = tmp->next;
	  //}
   }
   tmp = calloc(1, sizeof (struct typenametable_entry));
   //if(!tmp) { fprintf(stderr, "tnti out of memory\n"); return -1; }
   tmp->name = strdup(s);
   tmp->category = cat;
   tmp->next = HEAD;
   HEAD = tmp;
}

int typenametable_lookup(char *s)
{
   struct typenametable_entry *tmp = HEAD;
   while (tmp) {
	   printf("s: %s\n", s);
	   fflush(stdout); 
	   printf("tmp->name: %s \n", tmp->name); 
	   fflush(stdout); 
      if (tmp->name && !strcmp(s, tmp->name)) {
        // fprintf(stderr,
	    //"warning, %s is already in the typename table as a %d\n",
	   // s, tmp->category );
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
		printf("clearing type %s \n", (char*)type->name); 
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
		printf("type name table is cleared.\n"); 
	}
}
