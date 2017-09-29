
#include <stdbool.h>
#include <stdio.h>
#include "hash.h"
#include "tree_lydia.h"

// adapted from https://github.com/park2331/compiler

struct entry;
struct type;
struct table;

// structure for hash table 
typedef struct entry {

  char *name;
  
  struct type *entrytype;
  struct table *entrytable;

} * Entry;



// hash table structure that holds name and scope info.
typedef struct table {

  char *name;
  Entry entry[];
    
} * SymbolTable;


// structure that holds type information 
typedef struct type {

  int basetype;
  
  union {
    
    struct arraytype {
      
      int size;
      struct entry *type;
      
    } a;

    struct functiontype {

      struct entry *type;

    } f;

    struct classtype {
      struct entry *type;

    } c;

  };

} * TypeTable;

// symbol table and entry table function declarations
SymbolTable new_table( char* );
Entry new_entry( char* );
Entry new_scope( char* );
Entry get_entry( char* , SymbolTable );
void insert( Entry, SymbolTable );
void insert_sym( char* , SymbolTable );
SymbolTable get_scope(char* , SymbolTable );
bool lookup( char *, SymbolTable );
void semanticerror(char *s, struct tree *n); 

void populate_params(struct tree *); 
void check_declared(struct tree *); 

struct tree * populate_symbol_table( struct tree *, SymbolTable );
struct tree * handle_funcdef( struct tree * , SymbolTable );