#include "symbol_table.h"

void insert_sym_list(char *s, char *t, int typ); 
bool find_sym_in_list(char *s, char *t); 
void print_syms_in_list(char *t);
int find_type_in_list(char *s, char *t);
void parm_entry_in_list(char *t, char *p, int typ); 
int check_syms_in_list( char *t, int type); 