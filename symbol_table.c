#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "symbol_table.h"
#include "hash.h"
#include "nonterms.h"
#include "120gram_lydia.tab.h"
#include "utility_func.h"
#include "list.h"

extern SymbolTable CLASSTABLE; 
extern SymbolTable FUNCTION_TABLE; 
extern SymbolTable GLOBAL_TABLE; 


// adapted from https://github.com/park2331/compiler

// creates 'type' entry; init memory and store name 
Entry new_entry(char *n, int typ) {

  Entry e = calloc(1, sizeof(Entry));

  e->name = (char*)malloc(strlen(n)+1); // sometimes there is problems with this... so weird!!
  e->name = strdup(n);
  
  // add type to entry
  e->typ = typ;
  
  
  return e;
}


// Initialize new table for use, initialize memory and store name. 
SymbolTable new_table(char* n) {

 // possibly change to calloc instead of malloc: Done
 
  SymbolTable t = (SymbolTable)calloc(1, sizeof(SymbolTable));
  t->entry[0] = (Entry)calloc(10000, sizeof(Entry));
  //t->entry[0]->name = (char*)malloc(strlen(n)+1);

  t->name = (char*)malloc(strlen(n)+1); // this helps sometimes
  t->name = strdup(n);
  //strncpy(t->name, n, strlen(n)+1);
  
  return t;
}


// insert entry; takes entry struct as param 
void insert(Entry e, SymbolTable t, int typ) {

   int key = get_key(e->name);
  
   insert_sym_list(e->name, t->name, typ);
   
   t->entry[key] = e;
		
   fprintf( stdout , "%s was INSERTED into scope: %s with type %d \n" , e->name , t->name, typ);

	
}

// inserts entry by name 
void insert_sym(char* n, SymbolTable t, int typ) {
 
  Entry e = new_entry(n, typ);
  
  insert(e, t, typ);
  
}


// return entry at specific key n
Entry get_entry( char *n , SymbolTable t ) {

  return t->entry[get_key(n)];
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
// TODO: Work on class.member handling: Done
// TODO: Work on class constructor members (not undeclared)!

// overall populate_symbol_table layout adapted from https://github.com/park2331/compiler/blob/master/tomorrow/symtab.c
// this function populates symbol tables for classes, functions, and globals
struct tree * populate_symbol_table( struct tree *t , SymbolTable scope ) {

  int i,j,k, key;
  int type;
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
          insert_sym(t->leaf->text, scope, 0);
          
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
		
	case SIMPLE_DECLARATION_1:
		type = get_base_type(t->kid[0]);
		populate_init_decls(t->kid[1], scope, type);
		break; 

	case CLASS_SPECIFIER_1:
      printf("\n------CLASS------\n"); 
      class_name = get_class_name(t);
	  if(lookup(class_name, GLOBAL_TABLE))
	  {
		  semanticerror("Redeclared variable.", t); 
	  }
	  // get type and check type before inserting
      insert_sym(class_name, GLOBAL_TABLE, 0); // Insert into global table
      
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
		// get base type of function (which is return type)
	    type = get_base_type(t->kid[0]);
		// inserting type into tree node
		t->kid[1]->kid[0]->typ = type;
		//printf("Printing function leaf node: %s, type: %d \n", t->kid[1]->kid[0]->leaf->text, t->kid[1]->kid[0]->typ);
		printf("This function: %s, type: %d\n", func_name, type);
        insert_sym(func_name, GLOBAL_TABLE, type); // Insert into global table 
		local_tables[nlocaltables++] = func_table;
	  }
	  else
	  {
		 insert_sym(func_name, scope, 0); // Insert into global table 
		 class_tables[nclasstables++] = func_table;		
	  }
      
      for (j=0; j < t->nkids; j++) 
	  {
		// insert into local function symbol table 
		populate_symbol_table( t->kid[j] , scope);   		
		 
      }

	  break; 
	case ASSIGNMENT_EXPRESSION_1:
	    if(t->kid[0]->prodrule == POSTFIX_EXPRESSION_4)
		{
			check_class_members_post(t);
			// check type
		}
	    // checking the right hand side of assignment expression
		else if(t->kid[2]->prodrule == POSTFIX_EXPRESSION_1)
		{
			checkundeclared(t->kid[2]->kid[0], GLOBAL_TABLE); 
			// check type
		}
		else if(t->kid[2]->prodrule == ADDITIVE_EXPRESSION_1 
		     || t->kid[2]->prodrule == MULTIPLICATIVE_EXPRESSION_1)
		{
			// only finds the first thing in the additive expression
			checkundeclared(t->kid[2]->kid[0], scope); 
			// check types 
			printf("Find and check types!\n");
			type_add_check(t, scope->name);

		}
       
		else 
		{
			 // checking left hand side of assignment expression
			checkundeclared(t->kid[0], scope); 
			 // checking right hand side of assignment expression
			checkundeclared(t->kid[2], scope);
			// check type
		}
		break;	
		
    case POSTFIX_EXPRESSION_1:
	     // checking if a class function has been declared in the class scope
		 if(t->kid[0]->prodrule == POSTFIX_EXPRESSION_4)
		 {
			 check_class_members_post(t);
			 break; // does this need to be here?
		 }
		 // checking if a function has been declared in the global scope
		 if(t->kid[0] != NULL || t->kid[0]->leaf->text != NULL)
		 {
			if(strcmp(t->kid[0]->leaf->text, "printf") != 0 && (strcmp(t->kid[0]->leaf->text, "scanf") != 0 )) // slightly cheating here
				checkundeclared(t->kid[0], GLOBAL_TABLE); 
		 }
		 break; 	

    case JUMP_STATEMENT_1:
	    // checking the return value 
		if(t->kid[1]->prodrule == POSTFIX_EXPRESSION_1)
		{
			if(t->kid[1]->kid[0]->prodrule == POSTFIX_EXPRESSION_4)
			{
				// somehow need to check if this has been declared in class scope
				check_class_members_jump(t);
			}
			else
			{
				// check undeclared for regular postfix
				checkundeclared(t->kid[1], scope);

			}
		}
		else
		{
			checkundeclared(t->kid[1], scope);
		}
		
		break;
	case SELECTION_STATEMENT_1:
	    // checking inside if and switch statements
		if(t->kid[2]->prodrule == POSTFIX_EXPRESSION_4)
		{
			// helper function to check if variable is declared in class scope
			// defined in utility_func.c
			check_class_members_state(t);
		}
		else
		{
			checkundeclared(t->kid[2], scope); 
		}
		//break;
	case ITERATION_STATEMENT_1:
	    // checking inside if and switch statements
		if(t->kid[2]->prodrule == POSTFIX_EXPRESSION_4)
		{
			// helper function to check if variable is declared in class scope
			// defined in utility_func.c
			check_class_members_state(t);
		}
		else
		{
			checkundeclared(t->kid[2], scope); 
		}
		//break;
		// do not need a break statement so that 
		// the rest of the variables can be processed within the iteration statement scope
    case DIRECT_DECLARATOR_1:
	case DIRECT_DECLARATOR_5:
    case DIRECT_DECLARATOR_6:
	case DIRECT_DECLARATOR_7:
      direct_declare = true;
      //printf("\n ------DECLARED VARIABLES------\n");
	  
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
			  
			 if(!lookup(t->leaf->text, ST)&& !find_sym_in_list(t->leaf->text, ST->name) && !find_sym_in_list(t->leaf->text, "global_table"))
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


void populate_init_decls(struct tree *t, SymbolTable scope, int type)
{
	switch(t->prodrule)
	{
		case INIT_DECLARATOR_1:
		case INIT_DECLARATOR_LIST_1:
		    checkredeclared(t->kid[0], scope);
			populate_init_decls(t->kid[0], scope, type);
			break;
		case DIRECT_DECLARATOR_4:
			populate_init_decls(t->kid[0], scope, type);
			break;
		case IDENTIFIER:
		    t->typ = type;
			//printf("Inserting %s as type %d\n", t->leaf->text, t->typ);
			insert_sym(t->leaf->text, scope, type);
			break;
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


