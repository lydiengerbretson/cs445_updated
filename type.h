#include "symbol_table.h"

#ifndef TYPE_H
#define TYPE_H

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

#endif /*TYPE_H*/