#include <stdio.h>
#include "utility_func.h"
#include "symbol_table.h"
#include "nonterms.h"
#include "120gram_lydia.tab.h"

void print_tables(int type)
{
    int i = 0;
    char * temp_name = "";
   if (1) // FUNCTIONS
    {
       for (i = 0; i < TABLE_SIZE ; i++)
       {
         if (local_tables[i])
         {
             if (local_tables[i] == NULL)
             {
                 printf("Null on %d", i);
             }
             else
             {
               printf("Local Table %d: %s\n", i + 1, local_tables[i]->name);
             }
         }
         else
         {
            break;
         }
       }
         

    }
    else if(2) // Classes
    {
    }

}


char * get_func_name(struct tree * t)
{
   char * error = "Crap didn't find name";
   int seenfunc = 0;
   while (1) {
      switch (t->prodrule) {
      case FUNCTION_DEFINITION_1:
        t = t->kid[1];
        break;
      case IDENTIFIER:
        if (seenfunc) 
        {
            return t->leaf->text;
            
        }
        return error ;
      case DIRECT_DECLARATOR_1: // May have to have for class's or other direct_declarers
	 /* now we have seen function syntax, want the ident */
        seenfunc = 1;
        t = t->kid[0];
        break;
      default:
        return error;
	 }
      }
}

char * get_class_name(struct tree *t)
{
     char * error = "Crap didn't find name";
     int classdef = 0;
     while (1)
     {
         switch(t->prodrule)
         {
             case CLASS_SPECIFIER_1:
             {
                t = t->kid[0];
                break;
             }
             case IDENTIFIER:
             {
                if (classdef)
                {
                    return t->leaf->text;
                }
                return error;
             }             
            case CLASS_HEAD_1:
            {
                classdef = 1;
                t = t->kid[1];
                break;
            }
            default:
            {
                return error;
            }
         }
     }
}
