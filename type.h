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

enum base_types
{
	INT_TYPE = 1,
	CHAR_TYPE = 2, 
	VOID_TYPE = 3, 
	STRING_TYPE = 4,
	DOUBLE_TYPE = 5, 
	OFSTREAM_TYPE = 6
};

int get_base_type(struct tree *);




#endif