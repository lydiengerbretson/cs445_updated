#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>
#include <stdio.h>
#include "hash.h"
#include "tree_lydia.h"
#include "type.h"

// adapted from https://github.com/park2331/compiler

struct entry;
struct type;
struct table;

// structure for hash table 
typedef struct entry {

  char *name;
  char *sym_table_name;
  int typ;
  
  struct type *entrytype;
  struct table *entrytable;
  
  struct entry *next; 

} * Entry;

Entry start;


// hash table structure that holds name and scope info.
struct table {

  char *name;

  Entry entry[10000];
    
};

typedef struct table  * SymbolTable;


// symbol table and entry table function declarations
SymbolTable new_table( char* );
Entry new_entry( char* , int);
Entry get_entry( char* , SymbolTable );
void insert( Entry, SymbolTable, int);
void insert_sym( char* , SymbolTable, int);
SymbolTable get_scope(char* , SymbolTable );
bool lookup( char *, SymbolTable );
void semanticerror(char *s, struct tree *n); 
char * alloc(int n);

void checkredeclared(struct tree *, SymbolTable); 
void checkundeclared(struct tree *, SymbolTable); 

struct tree * populate_symbol_table( struct tree *, SymbolTable );
struct tree * handle_funcdef( struct tree * , SymbolTable );
void populate_init_decls(struct tree *, SymbolTable, int);
#endif
