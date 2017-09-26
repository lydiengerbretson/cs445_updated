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
  
  return e;
}


/* Initialize new table for use; init mem and store name. */
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

  fprintf( stdout , "%s was INSERTED into %s at location %d.\n" , e->name , t->name , key );
  
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

  fprintf( stdout , "%s was INSERTED into scope %s at location %d.\n" , e->name , t->name , key );
  
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

    fprintf(stdout, "Entry: \"%s\" found in table \"%s\" at location %d\n", n, t->name , key );
    return true;
    
  } else {

    fprintf(stdout, "Entry: \"%s\" NOT found in table \"%s\"\n", n, t->name );
    return false;
    
  }
  
}

struct tree * handlefuncdef( struct tree *t, SymbolTable scope ) {

  int i;

  if ( t->nkids == 0 ) {

    if ( t->u.t.lexeme);
      return t;
  
  } else if( t->nkids > 0 ) {

    for(i=0; i < t->nkids; i++) {

      return populate_symbol_table( t->u.nt.child[i] , scope );

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

  } else if (t->nkids == 0 ) {

    if (t->category == IDENTIFIER) {
      insert_sym(t->u.t.lexeme , scope);
      
      return t;

    } else {
      return NULL;
    }
  } else if( t->nkids > 0 ) {

    key = get_key( t->u.t.lexeme );
    printf( "%s = %d\n",t->u.t.lexeme, key );

    switch(key) {


    case FUNCTION_DEFINITION_1: 
      local = new_scope("local");
      for (j=0; j < t->nkids; j++) 
	  {
		populate_symbol_table( t->u.nt.child[j] , local->entrytable );
      }
	  // make type 
      insert_sym( local->entrytable->name, scope );

    case DIRECT_DECLARATOR_1:

     // scope->name = strdup(t->child[0]->leaf->text);
	 scope->name = strdup(t->u.t.lexeme); 
     // make type??
      break;

    default:

      for (i=0; i < t->nkids; i++) 
	  {
	    populate_symbol_table( t->u.nt.child[i] , scope );
      }
      break;
    }

  } else {
    return NULL;
  }
};


