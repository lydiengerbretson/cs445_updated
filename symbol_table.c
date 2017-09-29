#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "symbol_table.h"
#include "hash.h"
#include "nonterms.h"
#include "120gram_lydia.tab.h"

// adapted from https://github.com/park2331/compiler

/* Creates 'type' entry; init memory and store name */
Entry new_entry(char* n) {

  Entry e = calloc(1, sizeof(Entry));
  TypeTable t = calloc(1, sizeof(TypeTable));

  e->name = strdup(n);

  /* Initialize type */
  e->entrytype = t;
  //e->entrytype->basetype = type; 
  
  return e;
}


// Initialize new table for use, initialize memory and store name. 
SymbolTable new_table(char* n) {

  int i;
  SymbolTable t = calloc(1, sizeof(SymbolTable));
  
  t->entry[10000] = calloc(10000, sizeof(Entry));

  t->name = strdup(n);

  return t;
}


/* Init mem for entry, stores name, inits table field of entry struct. 
 * Returns entry pointer.
 */
Entry new_scope(char *n) {

  Entry e = new_entry(n);
  
  e->name = strdup(n);
  e->entrytable = new_table(n);

  return e;
 
}


// Insert entry; takes entry struct as param 
void insert(Entry e, SymbolTable t) {

  int key = get_key(e->name);

  t->entry[key] = e;

  fprintf( stdout , "%s was INSERTED into scope: %s.\n" , e->name , t->name);
  
}


/* Inserts entry by name */
void insert_sym(char* n, SymbolTable t) {

  Entry e = new_entry(n);

  insert(e, t);
  
}

/* Function creates and inserts a scope table into table t*/
void insert_scope(char* n, SymbolTable t) {

  Entry e = new_scope(n);

  int key = get_key(e->name);

  insert(e, t);

  fprintf( stdout , "%s was INSERTED into scope: %s.\n" , e->name , t->name );
  
}

Entry get_entry( char *n , SymbolTable t ) {

  return t->entry[get_key(n)];
}


/* Return entry */
SymbolTable get_scope(char *n, SymbolTable t) {
  
  int key = get_key(n);
  
  return t->entry[key]->entrytable;
  
}

bool lookup(char *n, SymbolTable t) {

  int key = get_key(n);
  

  if (t->entry[key]) {

   // fprintf(stdout, "Entry: \"%s\" found in table \"%s\" at location %d\n", n, t->name , key );
    return true;
    
  } else {

    // fprintf(stdout, "Entry: \"%s\" NOT found in table \"%s\"\n", n, t->name );
    return false;
    
  }
  
}

struct tree * handle_funcdef( struct tree *t, SymbolTable scope ) {

  int i;

  if ( t->nkids == 0 ) {

    if ( t->leaf);
      return t;
  
  } else if( t->nkids > 0 ) {

    for(i=0; i < t->nkids; i++) {

      return populate_symbol_table( t->kid[i] , scope );

    }

  } else {

    return NULL;

  }

}

struct tree * populate_symbol_table( struct tree *t , SymbolTable scope ) {

  int i,j, key;
  struct tree * temp;
  Entry local;

  if ( !t ) {
    return NULL;

  } else if (t->nkids == 0 ) 
  {

    if (t->prodrule == IDENTIFIER) 
	{
	 // printf("---IDENTIFIER----\n"); 
      insert_sym(t->leaf->text , scope);
      
      return t;

    }
	else 
	{
      return NULL;
    }
  } else if( t->nkids > 0 ) 
  {

    key = get_key( t->prodrule_name );
    //printf( "%s = %d\n",t->prodrule_name, key );

// TODO: Add function parameter scoping: Kinda done. 
// TODO: Fix function name scoping/definitions: Kinda done.
// TODO: Add semantic error for redeclarations

    switch(t->prodrule) {

    case FUNCTION_DEFINITION_1:   	
	  printf("\n------FUNCTION------\n"); 
      local = new_scope("global");//t->kid[0]->leaf->text
	  handle_funcdef(t->kid[1], local->entrytable);
      for (j=0; j < t->nkids; j++) 
	  {
		// insert into local symbol table 
		populate_symbol_table( t->kid[j] , local->entrytable );     
		 
      }
     // insert_sym( local->entrytable->name, scope ); // apparently this is not needed 
	   //populate_params(t->kid[1]);
	   //check_declared(t->kid[1])
	  break; 
	  
    case DIRECT_DECLARATOR_1:
	//case DECLARATOR_1:
      printf("\n ------DECLARED VARIABLES------\n");
	
      scope->name = strdup(t->kid[0]->leaf->text);
	  //break; // when break is commented, parameters show up  

    default:
     //printf(" ------DEFAULT------\n");
      for (i=0; i < t->nkids; i++) 
	  { 
        //printf("Calling default pop table...\n"); 
	    populate_symbol_table( t->kid[i] , scope );
      }
      break;
    }

  } else {
    return NULL;
  }
};

void populate_params(struct tree *t)
{
	int i; 
	if(t==NULL)
	{
		return; 
	}
	
	switch(t->prodrule)
	{
		case DIRECT_DECLARATOR_1:
		return; 
		default:
			for(i=0; i < t->nkids; i++)
			{
				populate_params(t->kid[i]); 
			}
	}
}

void check_declared(struct tree * t)
{


}
