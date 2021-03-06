#ifndef TYPE_H
#define TYPE_H

#include "symbol_table.h"
#include "tree_lydia.h"
#include "list.h"

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
	OFSTREAM_TYPE = 6,
	PTR_TYPE = 7,
	BOOL_TYPE = 8, 
	CLASS_TYPE = 9,
	ARRAY_TYPE = 10
};

int get_base_type(struct tree *);
void type_add_check(struct tree *, char *, int);
void type_mod_check(struct tree *, char *, int);
void type_shift_check(struct tree *, char *);
void type_relation_check(struct tree *, char *);
void type_switch_check(struct tree *, char *);
void type_shift_check_1(struct tree *);
void type_assign_check(struct tree *, char *);
void type_return_check(struct tree *, char *, int); 
void type_class_member_check(struct tree *, char *); 
void type_class_member_check_relation(struct tree *); 
void type_class_member_func_check(struct tree *, int); 
void type_class_member_jump_check(struct tree *, int); 
void type_class_member_jump_2_check(struct tree *, int );
int parameter_count(struct tree *t);

int find_type_in_list_icg(struct tree *t);




#endif