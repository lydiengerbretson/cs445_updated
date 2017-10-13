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

// adapted from https://github.com/park2331/compiler

// creates 'type' entry; init memory and store name 
Entry new_entry(char* n) {

  Entry e = calloc(1, sizeof(Entry));
  TypeTable t = calloc(1, sizeof(TypeTable));

 // e->name = strdup(n);
 //e->name = malloc(strlen(n) +1); // nope this doesn't help LOL
 e->name = strdup(n); 


  // initialize type
  e->entrytype = t;
  
  //e->entrytype->basetype = type; 
  
  return e;
}


// Initialize new table for use, initialize memory and store name. 
SymbolTable new_table(char* n) {

 // possibly change to calloc instead of malloc: Done
 
  SymbolTable t = calloc(1, sizeof(SymbolTable));
  t->entry[0] = calloc(10000, sizeof(Entry));
  //t->entry[10000] = calloc(10000 , sizeof(Entry));

  t->name = strdup(n);
  
  return t;
}


// init mem for entry, stores name, inits table field of entry struct. 
// returns entry pointer.
Entry new_scope(char *n) {

  Entry e = new_entry(n);
  
  e->name = strdup(n);
  e->entrytable = new_table(n);

  return e;
 
}


// insert entry; takes entry struct as param 
void insert(Entry e, SymbolTable t) {

   int key = get_key(e->name);
  
   insert_sym_list(e->name, t->name);
   
   t->entry[key] = e;
		
   fprintf( stdout , "%s was INSERTED into scope: %s at location %d \n" , e->name , t->name, key);

    // need to handle collisions!!
	// insert symbol after walking through a linked list symbol table entries
	
}

void insert_sym_list(char *s, char *t)
{
    struct entry *new_node = (struct entry *)malloc(sizeof(struct entry));
    struct entry *curr, *temp;
	
    //new_node->sym_table_name = malloc(strlen(t)+1); 
	new_node->sym_table_name = strdup(t); 
	
	//new_node->name = malloc(strlen(s)+1); 
	new_node->name = strdup(s); 
	
	new_node->next = NULL; 
	
	if(start == NULL)
	{
		// insert first node
		//printf("**Inserting first node: %s\n", new_node->name); 
		start = new_node; 
		curr = new_node;
	}
	else
	{
		temp = start; 
		while(temp->next != NULL)
		{
			temp = temp->next; 
		}
		//printf("**Inserting the rest: %s\n", new_node->name); 
		temp->next = new_node; 
	}
	
}

bool find_sym_in_list(char *s, char *t)
{
	struct entry *temp;
	temp = start; 
	
	while(temp != NULL)
	{
		if(strcmp(s, temp->name) == 0 && strcmp(temp->sym_table_name, t) ==0)
		{
		//printf("**Printing the wanted symbol: %s\n", temp->name); 
		return true; 
		}
		temp = temp->next;
		
	}
	return false;
		
}


// inserts entry by name 
void insert_sym(char* n, SymbolTable t) {
 
  Entry e = new_entry(n);
  
  insert(e, t);
  
}

// function creates and inserts a scope table into table t 
void insert_scope(char* n, SymbolTable t) {

  Entry e = new_scope(n);

  int key = get_key(e->name);

  insert(e, t);

  fprintf( stdout , "%s was INSERTED into scope: %s.\n" , e->name , t->name );
  
}

// return entry at specific key n
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

  if (t->entry[key] ) 
  {
    //fprintf(stdout, "Entry: \"%s\" found in table \"%s\" at location %d\n", n, t->name , key );
    return true;
    
  } 
  else 
  {

    //fprintf(stdout, "Entry: \"%s\" NOT found in table \"%s\"\n", n, t->name );
    return false;
  }
    
  
}

// TODO: Add function parameter scoping: Done. 
// TODO: Add function name scoping/definitions: Done.
// TODO: Add global scoping: Done 
// TODO: Add semantic error for redeclarations: Done.
// TODO: Add semantic error for undeclarations: Kinda done. Add more cases.
// TODO: Multiple variable declarations on the same line: Done
// TODO: Implement array declarations: Done.
// TODO: Implement class function scoping: Kinda done. Need to create class function tables?
// TODO: Fix ant.cpp/ant.h problem: Done.
// TODO: Fix #include string or hash problem: Done.
// TODO: Implement cout, cin, endl: Kinda done.
// TODO: Fix function prototypes, insert it into global table: Done

// overall populate_symbol_table layout adapted from https://github.com/park2331/compiler/blob/master/tomorrow/symtab.c
// this function populates symbol tables for classes, functions, and globals
struct tree * populate_symbol_table( struct tree *t , SymbolTable scope ) {

  int i,j, key;
  struct tree * temp;
  Entry local;
  char *lex;
  int class_func = 0; // 1 for class method, 0 otherwise
  static int nlocaltables = 0;
  static int nclasstables = 0;
  SymbolTable func_table = NULL;
  SymbolTable class_table = NULL;
  char * func_name = NULL;
  char * class_name = NULL;
  static bool direct_declare = false;
  int declarator = 0;
  
  if ( !t ) {
    return NULL;

  } else if (t->nkids == 0 ) 
  {

   if (!direct_declare) // This fixes foo was instered into scope foo problem
    {

        if (t->prodrule == IDENTIFIER) 
        {
		  // get type and check type before inserting
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
	  {
		//  semanticerror("Redeclared variable.", t); 
	  }
	  // get type and check type before inserting
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
      func_name = get_func_name(t, &class_func);
	  if (func_name == NULL)
	  {
		  printf("Error, function has no name\n");
	  }
	  func_table = new_table(func_name);
	  scope = func_table;
	  // get type and check type before inserting
	  if (class_func == 0)
	  {		        
		// not a class method
        insert_sym(func_name, GLOBAL_TABLE); // Insert into global table 
		local_tables[nlocaltables++] = func_table;
	  }
	  else
	  {
		 insert_sym(func_name, scope); // Insert into global table 
		 class_tables[nclasstables++] = func_table;		
	  }
      
      for (j=0; j < t->nkids; j++) 
	  {
		// insert into local function symbol table 
		populate_symbol_table( t->kid[j] , scope);   		
		 
      }

	  break; 
	case ASSIGNMENT_EXPRESSION_1:
	    // checking the left hand side of assignment expression
		if(t->kid[2]->prodrule == ADDITIVE_EXPRESSION_1)
		{
			printf("found additive expression!\n"); 
			// only finds the first thing in the additive expression
			checkundeclared(t->kid[2]->kid[0], scope); 
		}
		else
		{
	     checkundeclared(t->kid[0], scope); 
		}
		break;		
    case POSTFIX_EXPRESSION_1:
	     // checking if a function has been declared in the global scope
		 if(strcmp(t->kid[0]->leaf->text, "printf") != 0 && (strcmp(t->kid[0]->leaf->text, "scanf")) != 0 ) // slightly cheating here
	     checkundeclared(t->kid[0], GLOBAL_TABLE); 
		 break; 		
    case JUMP_STATEMENT_1:
	    // checking the return value 
		checkundeclared(t->kid[1], scope); // this doesn't work with returning class.member
		break;		
	case SELECTION_STATEMENT_1:
	    // checking inside if and switch statements
	    checkundeclared(t->kid[2], scope); // this doesn't work with returning class.member
		break;
	  
    case DIRECT_DECLARATOR_1:
	case DIRECT_DECLARATOR_5:
    case DIRECT_DECLARATOR_6:
	case DIRECT_DECLARATOR_7:
      direct_declare = true;
      printf("\n ------DECLARED VARIABLES------\n");
	  
	  for (j=0; j < t->nkids; j++) 
	  {
		// insert into local current symbol table 
		// this will create new scope i.e. SymbolTable for each new function 
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


// check redeclared variables
// if they are in symbol table, then they are already declared
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
			  // lookup checks for symbol in correct scoping
			  // find_sym_in_list just double checks if lookup
              // did its job and scans to find if symbol is 
			  // in a linked list of entries. 
			  // because lookup is stupid sometimes... :/
			 if(lookup(t->leaf->text, ST) && find_sym_in_list(t->leaf->text, ST->name))
			  {
				   
				  // this need to be only init declarators
				 //printf("\n DOUBLE: %s in SCOPE: %s, %d \n", t->leaf->text, ST->name, t->leaf->category); 
				 semanticerror("Redeclared variable:", t); 
			  }
		  }
	  }
	  else
	  {	  

		for(j=0; j<t->nkids; j++)
		{
			checkredeclared(t->kid[j], ST);
		}
		
	  }

  }
}

// check undeclared variables
// if they are not in symbol table, then they are not declared
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
			  
			 if(!lookup(t->leaf->text, ST) && !find_sym_in_list(t->leaf->text, ST->name) && !find_sym_in_list(t->leaf->text, "global_table"))
			  {
				 semanticerror("Undeclared variable:", t); 
			  }
		  }
	  }
	  else
	  {	  

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


