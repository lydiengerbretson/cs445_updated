#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "symbol_table.h"
#include "hash.h"
#include "nonterms.h"
#include "120gram_lydia.tab.h"

extern SymbolTable CLASSTABLE; 
extern SymbolTable FUNCTION_TABLE; 
extern SymbolTable GLOBAL_TABLE; 
extern SymbolTable PARAM_TABLE; 

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

    fprintf( stdout , "%s was INSERTED into scope: %s at location %d \n" , e->name , t->name, key);
  
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

   //fprintf(stdout, "Entry: \"%s\" found in table \"%s\" at location %d\n", n, t->name , key );
    return true;
    
  } else {

    //fprintf(stdout, "Entry: \"%s\" NOT found in table \"%s\"\n", n, t->name );
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
  char *lex;


  static bool direct_declare = false;
  
  if ( !t ) {
    return NULL;

  } else if (t->nkids == 0 ) 
  {

   if (!direct_declare) // This fixes foo was instered into scope foo problem
    {
        if (t->prodrule == IDENTIFIER) 
        {

		  if(lookup(t->leaf->text, FUNCTION_TABLE))
		  {
			  printf("Already in symbol table %s : %s \n",  scope->name, t->leaf->text); 
			  //semanticerror("Error: redeclared variable", t); 
		  }
		  
          insert_sym(t->leaf->text , scope);
		  //checkdeclared(t, scope); 
          
          return t;

        }
        else 
        {
          return NULL;
        }
        
       
    }
  else
    {
        direct_declare = false;
    }
  } else if( t->nkids > 0 ) 
  {

    key = get_key( t->prodrule_name );

// TODO: Add function parameter scoping: Kinda done. 
// TODO: Add function name scoping/definitions: Kinda done.
// TODO: Add global scoping: Done 
// TODO: Add semantic error for redeclarations: 
// TODO: Add semantic error for undeclarations: 
// TODO: Multiple variable declarations on the same line: Done

    if (direct_declare){direct_declare=false;};
    switch(t->prodrule) {

	case CLASS_HEAD_1:
	case MEMBER_SPECIFICATION_1:
	for (j=0; j < t->nkids; j++) 
	  {
		// insert into class symbol table 
		populate_symbol_table( t->kid[j] , CLASSTABLE );   
			 
      }
	break; 
    case FUNCTION_DEFINITION_1:   

	  printf("\n------FUNCTION------\n"); 
	  handle_funcdef(t->kid[1], GLOBAL_TABLE); 
      for (j=0; j < t->nkids; j++) 
	  {
		// insert into local function symbol table 
		populate_symbol_table( t->kid[j] , FUNCTION_TABLE );   		
		 
      }
	  printf("Checking variables in scope: %s\n", FUNCTION_TABLE->name); 
	  for(j=0; j < t->nkids; j++)
	  {
		  checkdeclared(t->kid[j], FUNCTION_TABLE);  
	  }	
	  break; 
	case ASSIGNMENT_EXPRESSION_1:
		break;
    case JUMP_STATEMENT_1:
		break;
    case DIRECT_DECLARATOR_1:
      direct_declare = true;
      printf("\n ------DECLARED VARIABLES------\n");
	  for (j=0; j < t->nkids; j++) 
	  {
		// insert into local current symbol table 
		// need to create new scope i.e. SymbolTable for each new function? 
		// this doesn't fix the issue

		FUNCTION_TABLE->name = strdup(t->kid[0]->leaf->text); 
		populate_symbol_table( t->kid[j] , FUNCTION_TABLE );     
		 
      }	
	  
      break;
	  
	case PARAMETER_DECLARATION_1:
	 // printf("Found a parameter!\n");
	  for (j=0; j < t->nkids; j++) 
	  {
		// insert into local parameter symbol table 
		populate_symbol_table( t->kid[j] , FUNCTION_TABLE );     
		 
      }		
       break; 

    default:
     
      for (i=0; i < t->nkids; i++) 
	  {  
	    populate_symbol_table( t->kid[i] , scope );
      }
     
      break;
    }
}

	else
	{
		return NULL; 
	}

  };


// adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/semantic.c
void checkdeclared(struct tree * t, SymbolTable ST)
{
  int j; 

  //if(strcmp(ST->name, "yay") == 0)
  //{
  if(!t)
  {
      // do nothing
  }
  else 
  { 
    // lookup variable somewhere in here
	  if (t->nkids == 0)
	  {
		  printf("	LEAF within symbol table %s: \"%s\": %d\n",  
				 ST->name, t->leaf->text, t->leaf->category); 
	  }
	  else
	  {
		printf("	KID within symbol table %s: %s: %d\n", ST->name, t->prodrule_name, t->nkids);
 
		for(j=0; j<t->nkids; j++)
		{
			checkdeclared(t->kid[j], ST);
		}
	  }
}
  //}
}



// adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/symt.c
void semanticerror(char *s, struct tree *n)
{
   while (n && (n->nkids > 0)) n=n->kid[0];
   if (n) {
     fprintf(stderr, "%s:%d: ", n->leaf->filename, n->leaf->lineno);
   }
  fprintf(stderr, "%s", s);
  if (n && n->prodrule == IDENTIFIER) fprintf(stderr, " %s", n->leaf->text);
  fprintf(stderr, "\n");
  exit(3); 
}


