#ifndef UTILITY_FUNC_H
#define UTILITY_FUNC_H
#include "symbol_table.h"

#define TABLE_SIZE 1000

SymbolTable local_tables[TABLE_SIZE];
SymbolTable class_tables[TABLE_SIZE];

void print_tables(int type);
char * get_func_name(struct tree *, int *is_class);
char * get_class_name(struct tree *);
int get_declarator_type(struct tree *);

void check_class_members_post(struct tree *);
void check_class_members_state(struct tree *);
void check_class_members_jump(struct tree *t);

#endif