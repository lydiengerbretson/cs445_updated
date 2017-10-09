#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "symbol_table.h"
#include "hash.h"
#include "nonterms.h"
#include "120gram_lydia.tab.h"
#include "utility_func.h"

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


  SymbolTable t = malloc(sizeof(SymbolTable));

  t->entry[8741 ] = malloc(8741   * sizeof(Entry));

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
  static int nlocaltables = 0;
  static int nclasstables = 0;
  SymbolTable func_table = NULL;
  SymbolTable class_table = NULL;
  char * func_name = NULL;
  char * class_name = NULL;
  static bool direct_declare = false;
  static bool class_declare = false; 
  
  if ( !t ) {
    return NULL;

  } else if (t->nkids == 0 ) 
  {

   if (!direct_declare) // This fixes foo was instered into scope foo problem
    {

        if (t->prodrule == IDENTIFIER) 
        {

          insert_sym(t->leaf->text, scope);
          
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
	
	// this is where i check if a variable is redeclared
	case INIT_DECLARATOR_1:
        // will exit with error if redeclared
		checkredeclared(t->kid[0], scope); 
		populate_symbol_table(t->kid[0], scope); 
		break; 

	case CLASS_SPECIFIER_1:
      printf("\n------CLASS------\n"); 
      class_name = get_class_name(t);
	  if(lookup(class_name, GLOBAL_TABLE))
		  semanticerror("Redeclared variable.", t); 
      insert_sym(class_name, GLOBAL_TABLE); // Insert into global table
      
      class_table = new_table(class_name);
      class_tables[nclasstables++] = class_table;
      scope = class_table;
      for (j=0; j < t->nkids; j++) 
	  {
		// insert into local function symbol table 
		populate_symbol_table( t->kid[j] , scope);   		
		 
      }
	  break; 
    case FUNCTION_DEFINITION_1:   

	  printf("\n------FUNCTION------\n"); 
      func_name = get_func_name(t);
	  if(lookup(func_name, GLOBAL_TABLE))
		  semanticerror("Redeclared variable.", t); 
      insert_sym(func_name, GLOBAL_TABLE); // Insert into global table
	  //handle_funcdef(t->kid[1], GLOBAL_TABLE);       
      func_table = new_table(func_name);
      local_tables[nlocaltables++] = func_table;
      scope = func_table;
      for (j=0; j < t->nkids; j++) 
	  {
		// insert into local function symbol table 
		populate_symbol_table( t->kid[j] , scope);   		
		 
      }

	  break; 
	case ASSIGNMENT_EXPRESSION_1:
	    // will exit if it finds an undeclared variable
	    checkundeclared(t->kid[0], scope); 
		populate_symbol_table(t->kid[0], scope); 
		break;
		
    case JUMP_STATEMENT_1:
	 // will exit if it finds an undeclared variable
		//checkundeclared(t->kid[1], scope); 
		//populate_symbol_table(t->kid[1], scope); 
		break;
	  
    case DIRECT_DECLARATOR_1:
      direct_declare = true;
      printf("\n ------DECLARED VARIABLES------\n");
	  
	  for (j=0; j < t->nkids; j++) 
	  {
		// insert into local current symbol table 
		// need to create new scope i.e. SymbolTable for each new function? 
		// this doesn't fix the issue

		populate_symbol_table( t->kid[j] , scope );     
		 
      }	
	  
      break;
	case PARAMETER_DECLARATION_1:

	  for (j=0; j < t->nkids; j++) 
	  {
		// insert into local parameter symbol table 
		populate_symbol_table( t->kid[j] , scope);     
		 
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
void checkredeclared(struct tree * t, SymbolTable ST)
{
  int j; 

  if(!t)
  {
      // do nothing
  }
  else 
  { 
    // lookup variable somewhere in here

	  if (t->nkids == 0)
	  {
		  if(t->leaf->category == IDENTIFIER)
		  {
			  //printf("	LEAF within symbol table %s: \"%s\": %d at lineno: %d\n",  
			  //ST->name, t->leaf->text, t->leaf->category, t->leaf->lineno); 
			  
			 if(lookup(t->leaf->text, ST))
			  {
				  // this need to be only init declarators
				  printf("DOUBLE: %s \n", t->leaf->text); 
				 //semanticerror("Redeclared variable:", t); 
			  }
		  }
	  }
	  else
	  {	  

		//printf("	KID [%d] within symbol table %s: %s: %d %d \n", j, ST->name, t->prodrule_name, t->nkids, t->prodrule);
		for(j=0; j<t->nkids; j++)
		{
			checkredeclared(t->kid[j], ST);
		}
		
	  }

  }
}

// adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/semantic.c
void checkundeclared(struct tree * t, SymbolTable ST)
{
  int j; 

  if(!t)
  {
      // do nothing
  }
  else 
  { 
    // lookup variable somewhere in here

	  if (t->nkids == 0)
	  {
		  if(t->leaf->category == IDENTIFIER)
		  {
			  //printf("	LEAF within symbol table %s: \"%s\": %d at lineno: %d\n",  
			  //ST->name, t->leaf->text, t->leaf->category, t->leaf->lineno); 
			  
			 if(!lookup(t->leaf->text, ST))
			  {
				  // this need to be only init declarators
				 semanticerror("Undeclared variable:", t); 
			  }
		  }
	  }
	  else
	  {	  

		//printf("	KID [%d] within symbol table %s: %s: %d %d \n", j, ST->name, t->prodrule_name, t->nkids, t->prodrule);
		for(j=0; j<t->nkids; j++)
		{
			checkundeclared(t->kid[j], ST);
		}
		
	  }

  }
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


