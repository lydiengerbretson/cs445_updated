#include <stdio.h>
#include "utility_func.h"
#include "symbol_table.h"
#include "nonterms.h"
#include "120gram_lydia.tab.h"

void print_tables(int type)
{
    int i = 0;
    char * temp_name = "";
   if (type == 1) // FUNCTIONS
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
    else if(type == 2) // Classes
    {
	   for (i = 0; i < TABLE_SIZE ; i++)
       {
         if (class_tables[i])
         {
             if (class_tables[i] == NULL)
             {
                 printf("Null on %d", i);
             }
             else
             {
               printf("Class Table %d: %s\n", i + 1, class_tables[i]->name);
             }
         }
         else
         {
            break;
         }
       }
    }

}

int get_declarator_type(struct tree *t)
{
	int rule = 0; // Default not class 0-4 is non-class, > 4 is class
	switch(t->prodrule)
	{
		case DIRECT_DECLARATOR_1:
		case DIRECT_DECLARATOR_2:
		case DIRECT_DECLARATOR_3:
		case DIRECT_DECLARATOR_4:
			break;
		case DIRECT_DECLARATOR_5:
		case DIRECT_DECLARATOR_6:
		case DIRECT_DECLARATOR_7:
		case DIRECT_DECLARATOR_8:
			rule = 5; // Greater than 4 just set to 5
			break;
		default:
			break;
	}
	
	return rule;
}

char * get_func_name(struct tree * t, int *is_class)
{
   char * error = "Could not find a name.";
   int seenfunc = 0;
   int seenclass = 0;
   *is_class = 0; //Not a class method
   while (1 && t != NULL) {
      switch (t->prodrule) {
      case FUNCTION_DEFINITION_1:
		if (t->kid[1] == NULL)
		{
			t= t->kid[0]; // We're a class constructor
		}
		else
		{
			t = t->kid[1];
		}
        break;
     case IDENTIFIER:
        if (seenfunc || seenclass) 
        {
            return t->leaf->text;            
        }
        return error ;
	  case CLASS_NAME:
		if (seenclass)
		{
			return t->leaf->text;
		}
		return error;
      case DIRECT_DECLARATOR_1: 
	  case DIRECT_DECLARATOR_4:// May have to have for class's or other direct_declarers
	 /* now we have seen function syntax, want the ident */
        seenfunc = 1;
        t = t->kid[0];
        break;
	  case DIRECT_DECLARATOR_5:
	  case DIRECT_DECLARATOR_6:
	  case DIRECT_DECLARATOR_8:
	  // We are a class's function
	    seenclass = 1;
		*is_class = 1; // This is a class method
        t = t->kid[1];
		break;
	  case DIRECT_DECLARATOR_7:
		seenclass = 1;
	    *is_class = 2; // 2 is class constructor
		t = t->kid[2];
        break;
      default:
        return error;
	 }
      }
}

char * get_class_name(struct tree *t)
{
     char * error = "Could not find a name.";
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
			case DIRECT_DECLARATOR_7:
			  classdef = 1;
              t = t->kid[2];
              break;
            default:
            {
                return error;
            }
         }
     }
}

// helper function to check class members for postfix statements
void check_class_members_post(struct tree *t)
{
	// need to check if this function has been declared in the correct scoping
	int i; 
	// there should only be three children in a postfix expr
	//printf("Found postfix expr: %s \n", t->kid[0]->kid[0]->leaf->text);
	//printf("Found postfix expr: %s \n", t->kid[0]->kid[1]->leaf->text);
	//printf("Found postfix expr: %s \n", t->kid[0]->kid[2]->leaf->text);
	for(i=0; i < TABLE_SIZE; i++)
	{
		if(class_tables[i])
		{
			if(!find_sym_in_list(t->kid[0]->kid[2]->leaf->text, class_tables[i]->name))
				semanticerror("Undeclared variable.", t->kid[0]->kid[2]);
		}

	}
	
}

// helper function to check class members in selection and iteration statements 
void check_class_members_state(struct tree *t)
{
	// need to check if this function has been declared in the correct scoping
	int i; 
	// there should only be three children in a postfix expr
	//printf("Found postfix expr: %s \n", t->kid[2]->kid[0]->leaf->text);
	//printf("Found postfix expr: %s \n", t->kid[2]->kid[1]->leaf->text);
	//printf("Found postfix expr: %s \n", t->kid[2]->kid[2]->leaf->text);
	for(i=0; i < TABLE_SIZE; i++)
	{
		if(class_tables[i])
		{
			if(!find_sym_in_list(t->kid[2]->kid[2]->leaf->text, class_tables[i]->name))
				semanticerror("Undeclared variable.", t->kid[2]->kid[2]);
		}

	}
	
}

// helper function to check class members in selection and iteration statements 
void check_class_members_jump(struct tree *t)
{
	// need to check if this function has been declared in the correct scoping
	int i; 
	// there should only be three children in a postfix expr
	//printf("Found postfix expr: %s \n", t->kid[2]->kid[0]->leaf->text);
	//printf("Found postfix expr: %s \n", t->kid[2]->kid[1]->leaf->text);
	//printf("Found postfix expr: %s \n", t->kid[2]->kid[2]->leaf->text);
	for(i=0; i < TABLE_SIZE; i++)
	{
		if(class_tables[i])
		{
			if(!find_sym_in_list(t->kid[1]->kid[0]->kid[2]->leaf->text, class_tables[i]->name))
				semanticerror("Undeclared variable.", t->kid[1]->kid[0]->kid[2]);
		}

	}
	
}
