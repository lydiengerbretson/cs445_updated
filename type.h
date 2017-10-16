#ifndef TYPE_H
#define TYPE_H

#include "symbol_table.h"
#include "tree_lydia.h"

// structure that holds type information 
typedef struct type {

  int basetype;
  
  union {
    
    struct arraytype {
      
      int size;
      struct entry *type;
      
    } arr;

    struct functiontype {

      struct entry *type;

    } func;

    struct classtype {
      struct entry *type;

    } cls;

  };

} * typeptr;

int get_type(struct tree *);



#endif