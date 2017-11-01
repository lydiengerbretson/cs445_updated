#include "type.h"
#include "120gram_lydia.tab.h"
#include "utility_func.h"
#include "nonterms.h"

// function adapted from https://github.com/westrope/wookie-chomp/blob/hw3/Hw2/prodrule.c
int get_base_type( struct tree *t)
{

    if(!t)
	{
		return 0; 
	}
	else
	{
	//if(strcmp(t->leaf->text, "string") == 0)
		//return STRING_TYPE;
	//if(strcmp(t->leaf->text, "ofstream") == 0)
		//return OFSTREAM_TYPE;
	//if(strcmp(t->leaf->text, "LYDIA") == 0)
		//return 1234; // testing purposes
	}

	switch(t->prodrule)
	{
		
	case INT:
		return INT_TYPE;
	case DOUBLE:
	case FLOATING:
		return DOUBLE_TYPE;
	case CHAR:
		return CHAR_TYPE;
	case VOID:
		return VOID_TYPE;
	case BOOL:
		return BOOL_TYPE;
	case CLASS_NAME:
		if(strcmp(t->leaf->text, "string") == 0)
		{
			return STRING_TYPE;
		}
		else
		{
			return CLASS_TYPE;
		}
	default:
		return 0;
	}

  
}
void type_shift_check(struct tree *t, char *table_name)
{
	int type1; 
    int j;
	static int is_post = 0;

  if(!t)
  {
      // do nothing
  }
  else 
  { 
	   //printf("SIZEOF: %d\n", sizeof(0x65a00)); 
	   
      //if((long)t > 0x60000 && (long)t < 0x80000)
	  {
	  if (t->nkids == 0)
	  {
		  type1 = find_type_in_list(t->leaf->text, table_name);
		  //printf(" LEAF: \"%s\": %d %d\n",
				 // t->leaf->text, t->leaf->category, type1); 
		  if(is_post)
		  {
			  // need to allow arrays and functions as well 
			  if(type1 > 0)
			  {
				  // do nothing 
			  }
		  }
		  else
		  {
		  
		  if(t->leaf->category != STRING
		  && t->leaf->category != IDENTIFIER
		  && t->leaf->category != SL)
		  {
			  semanticerror("Incorrect operand or incompatible type.", t);
		  }
		  }
	  }
	  else
	  {
		//printf(" KID %s: %d\n", t->prodrule_name, t->nkids);
		if(t->prodrule == POSTFIX_EXPRESSION_1
		|| t->prodrule == POSTFIX_EXPRESSION_2)
		{
			//printf("it's a postfix!!\n");
			is_post = 1;
		}
 
		for(j=0; j<t->nkids; j++)
		{
			type_shift_check(t->kid[j], table_name);
		}
	  }
	  }

}
}

void type_relation_check(struct tree *t, char *table_name)
{
	int type1; 
    int j;
	static int rel_exp = 0;
	

  if(!t)
  {
      // do nothing
  }
  else 
  { 
	  if (t->nkids == 0)
	  {
		 // printf("2: %s\n", t->prodrule_name);

          type1 = find_type_in_list(t->leaf->text, table_name);
		  //printf(" LEAF: \"%s\": %d %d \n",
				   //t->leaf->text, t->leaf->category, type1); 
	      if(rel_exp)
		  { 
               if(type1 == STRING_TYPE
			   || type1 == CHAR_TYPE
			   || type1 == VOID_TYPE
			   || type1 == PTR_TYPE)
			   {
				   semanticerror("Need a bool or int or double type.", t);
			   }
		       if(type1 != BOOL_TYPE 
			   && type1 != INT_TYPE 
			   /*&& type1 != DOUBLE_TYPE*/
			   /* && t->leaf->category != DOUBLE*/
			   /*&& t->leaf->category != FLOATING*/
		       && t->leaf->category != BOOL
		       && t->leaf->category != INTEGER
		       && t->leaf->category != IDENTIFIER
		       && t->leaf->category != ANDAND
		       && t->leaf->category != OROR
		       && t->leaf->category != EQ
		       && t->leaf->category != 62
		       && t->leaf->category != 60
			   && t->leaf->category != NOTEQ
			   && t->leaf->category != LTEQ
			   && t->leaf->category != GTEQ)
		       {

			        semanticerror("Need a bool or int or double type.", t);
		  
		       }
		  }
		  // if not a relational expression and just a logical expression
		  // you cannot have double types in a logical expression 
		  else
		  {
               //printf("exit 2\n"); 
		       if(type1 > 0 && type1 != BOOL_TYPE && type1 != INT_TYPE)
		       {

			       semanticerror("Need a bool or int type.", t);
		  
		       }

		       // checking operands 
		       if(t->leaf->category != BOOL
		       && t->leaf->category != INTEGER
		       && t->leaf->category != IDENTIFIER
		       && t->leaf->category != ANDAND
		       && t->leaf->category != OROR
		       && t->leaf->category != EQ
			   && t->leaf->category != 40 // parens
		       && t->leaf->category != 41 // parens
			   && t->leaf->category != 42 // *
		       && t->leaf->category != 60 // <
		       && t->leaf->category != 62 // >
			   && t->leaf->category != 33 // !
			   && t->leaf->category != NOTEQ
			   && t->leaf->category != LTEQ
			   && t->leaf->category != GTEQ)
		       {
			       semanticerror("2: Incompatible types.", t);
		       }
		  }
	  }
	  else
	  {
		//printf(" KID %s: %d\n", t->prodrule_name, t->nkids);
        //printf("3: %s\n", t->prodrule_name);
      	  if(t->prodrule == RELATIONAL_EXPRESSION_1
		  || t->prodrule == EQUALITY_EXPRESSION_1)
		  {
			  rel_exp = 1;
		  }
		  else
		  {
			  rel_exp = 0;
		  }
		for(j=0; j<t->nkids; j++)
		{
			type_relation_check(t->kid[j], table_name);
		}
	  }
}


}
// trying to traverse tree for expressions like x = a * b * c
void type_add_check(struct tree *t, char *table_name, int base_type)
{
	int type1; 
    int j;

	static int parent_type; 
	// find type of left side of expression 
	//printf("calling temp func!!\n");
	
	//t = t->kid[2];
  parent_type = base_type; 
  //printf("parent type: %d\n", parent_type);
  if(!t)
  {
      // do nothing
  }
  else 
  { 
	  if (t->nkids == 0)
	  {
		  // TODO: make sure type1 > 0: checkundeclared here
		 
		  type1 = find_type_in_list(t->leaf->text, table_name);
		  //printf(" LEAF: \"%s\": %d %d\n",
				// t->leaf->text, t->leaf->category, type1); 
		  if(t->leaf->category == IDENTIFIER)
		  {
				if(!find_sym_in_list(t->leaf->text, table_name))
				{
					semanticerror("Undeclared variable", t); 
				}
		  }
		  
		  // if not an operator 
		  if(type1 > 0 && type1 != parent_type)
		  {
			  semanticerror("3: Incompatible types.", t);
		  }
	  }
	  else
	  {
		//printf(" KID: %s: %d\n", t->prodrule_name, t->nkids);
 
		for(j=0; j<t->nkids; j++)
		{
			type_add_check(t->kid[j], table_name, base_type);
		}
	  }
}


}

// checking out switch statement types
void type_switch_check(struct tree *t, char *table_name)
{
	int type1; 

    int j;


  if(!t)
  {
      // do nothing
  }
  else 
  { 
	  if (t->nkids == 0)
	  {
		  // TODO: make sure type1 > 0: checkundeclared here
		  type1 = find_type_in_list(t->leaf->text, table_name);
		 //printf(" LEAF: \"%s\": %d %d\n",
				 //t->leaf->text, t->leaf->category, type1); 
		  
		  // if not an operator 
		  
		  if(type1 > 0 
		  && type1 != INT_TYPE
		  && type1 != CHAR_TYPE) 
		  {
			  semanticerror("4: Incompatible types.", t);
		  }
		  

	  }
	  else
	  {
		//printf(" KID: %s: %d\n", t->prodrule_name, t->nkids);
 
		for(j=0; j<t->nkids; j++)
		{
			if(t->prodrule == SELECTION_STATEMENT_3)
			type_switch_check(t->kid[j], table_name);
		}
	  }
}


}


// this function checks for cout and cin >> "hey there";
void type_shift_check_1(struct tree *t)
{
	//printf("Entering type shift check 1\n");
    if(strcmp(t->kid[0]->leaf->text, "cin")==0)
    {
		if(t->kid[1]->leaf->category != SR)
		{
			semanticerror("Incorrect operand.", t);
		}
		if(t->kid[2]->leaf->category != IDENTIFIER)
		{
			semanticerror("Need an identifier.", t);
		}
	}
	if(strcmp(t->kid[0]->leaf->text, "cout")==0)
	{
		if(t->kid[1]->leaf->category != SL)
		{
			semanticerror("Incorrect operand.", t);
		}
		if(t->kid[2]->leaf->category != STRING
	    && t->kid[2]->leaf->category != IDENTIFIER)
		{
			semanticerror("Need a string or identifier:", t);
		}
	}
	//printf("%s\n", t->kid[1]->leaf->text);
	//printf("%s\n", t->kid[2]->leaf->text);
}



void type_assign_check(struct tree *t, char *s)
{
	int t1, t2;
	// this means that this is an array or function!
	if(t->kid[0]->prodrule == POSTFIX_EXPRESSION_1
	|| t->kid[0]->prodrule == POSTFIX_EXPRESSION_2)
	{
		    // check types for both left and right hand sides
	t1 = find_type_in_list(t->kid[0]->kid[0]->leaf->text, s);
	//printf("%s, %d\n", t->kid[0]->kid[0]->leaf->text, t1);
	t2 = find_type_in_list(t->kid[0]->kid[2]->leaf->text, s);
	//printf("%s, %d\n", t->kid[0]->kid[2]->leaf->text, t2);
	if(t1 == INT_TYPE 
	&& t2 != INT_TYPE 
	&& t->kid[0]->kid[2]->leaf->category != INTEGER)
	{
		semanticerror("Illegal type in array.", t);
	}
    
	// check right hand side of the array 
	t2 = find_type_in_list(t->kid[2]->leaf->text, s);
	//printf("right hand side type: %d\n", t2);
	if(t1 == INT_TYPE
	&& t2 != INT_TYPE
	&& t->kid[2]->leaf->category != INTEGER)
	{
		semanticerror("Incorrect types in assignment expression.", t);
	}
	// check right hand side of the array 
	if(t1 == CHAR_TYPE
	&& t2 != CHAR_TYPE
	&& t->kid[2]->leaf->category != CHARACTER)
	{
		semanticerror("Incorrect types in assignment expression.", t);
	}
		
	}
	else
	{
    // check types for both left and right hand sides
	t1 = find_type_in_list(t->kid[0]->leaf->text, s);
	t2 = find_type_in_list(t->kid[2]->leaf->text, s);
	
	
    // TODO: Add more cases
	if(t1 == INT_TYPE 
	&& t2 != INT_TYPE 
	&& t->kid[2]->leaf->category != INTEGER)
	{
		semanticerror("Incorrect types in assignment expression.", t);
	}
	if(t1 == DOUBLE_TYPE
	&& t2 != DOUBLE_TYPE
	&& t->kid[2]->leaf->category != DOUBLE
	&& t->kid[2]->leaf->category != FLOATING)
	{
		semanticerror("Incorrect types in assignment expression.", t);
	}
	if(t1 == STRING_TYPE
	&& t2 != STRING_TYPE
	&& t->kid[2]->leaf->category != STRING)
	{
		semanticerror("Incorrect types in assignment expression.", t);
	}
	// focusing mainly on char*
	if(t1 == PTR_TYPE
	&& t2 != PTR_TYPE
	&& t2 != STRING_TYPE
	&& t->kid[2]->leaf->category != STRING)
	{
		semanticerror("Incorrect types in assignment expression.", t);
	}
	if(t1 == BOOL_TYPE
	&& t2 != BOOL_TYPE
	&& t->kid[2]->leaf->category != BOOL)
	{
		semanticerror("Incorrect types in assignment expression.", t);
	}
	}
	
}

// checking out return/jump statement types
void type_return_check(struct tree *t, char *table_name, int func_type)
{
	int type1; 
	int j;
	int MULT = 42; 
	int DIV = 47; 
	int ADD = 43;
	int SUB = 45; 
	int MOD = 37; 
	int LP = 40; 
	int RP = 41; 
	int NOT = 33;


  if(!t)
  {
      // do nothing
  }
  else 
  { 
	  if (t->nkids == 0)
	  {
		  type1 = find_type_in_list(t->leaf->text, table_name);
		  //printf(" LEAF: \"%s\": %d %d\n",
				 //t->leaf->text, t->leaf->category, type1); 
		 if(type1 > 0 
		 && type1 != func_type)
		 {
			 semanticerror("Incorrect function return type:", t); 
		 }
		 else if(
		 // operations of a double type
		 func_type == DOUBLE_TYPE 
		 && t->leaf->category != INTEGER // because of calc.cpp:62
		 && t->leaf->category != FLOATING
		 && t->leaf->category != IDENTIFIER
		 && t->leaf->category != ADD
		 && t->leaf->category != SUB
		 && t->leaf->category != MULT
		 && t->leaf->category != DIV
		 && t->leaf->category != MOD
		 && t->leaf->category != PLUSPLUS
		 && t->leaf->category != MINUSMINUS
		 && t->leaf->category != LP
		 && t->leaf->category != RP)
		 {
			 semanticerror("Incorrect function return type:", t); 
		 }
		 else if(
		 // operations of a int type
		 func_type == INT_TYPE 
		 && t->leaf->category != INTEGER
		 && t->leaf->category != IDENTIFIER
		 && t->leaf->category != ADD
		 && t->leaf->category != SUB
		 && t->leaf->category != MULT
		 && t->leaf->category != DIV
		 && t->leaf->category != MOD
		 && t->leaf->category != PLUSPLUS
		 && t->leaf->category != MINUSMINUS
		 && t->leaf->category != LP
		 && t->leaf->category != RP)
		 {
			 semanticerror("Incorrect function return type:", t); 
		 }
		 else if(
		 // operations of a char type
		 func_type == CHAR_TYPE 
		 && t->leaf->category != CHARACTER
		 && t->leaf->category != IDENTIFIER)
		 {
			 semanticerror("Incorrect function return type:", t); 
		 }
		 else if(
		 // operations of a bool type
		 func_type == BOOL_TYPE
		 && t->leaf->category != BOOL
		 && t->leaf->category != TRUE
		 && t->leaf->category != FALSE
		 && t->leaf->category != IDENTIFIER
		 && t->leaf->category != ANDAND
		 && t->leaf->category != OROR
		 && t->leaf->category != EQ
	     && t->leaf->category != LP // parens
		 && t->leaf->category != RP // parens
	     && t->leaf->category != NOT // !
	     && t->leaf->category != NOTEQ
	     && t->leaf->category != LTEQ
		 && t->leaf->category != GTEQ)
		 {
			 semanticerror("Incorrect function return type:", t); 
		 }
		 else{
			 // do nothing for now
		 }

		  
		  

	  }
	  else
	  {
		//printf(" KID: %s: %d\n", t->prodrule_name, t->nkids);
 
		for(j=0; j<t->nkids; j++)
		{
			type_return_check(t->kid[j], table_name, func_type);
		}
	  }
}


}

// simple function to check class member types for postfix statements
// in assignment expressions
void type_class_member_check(struct tree *t,char *s)
{
    int i; 
    int type1; 
	int type2; 
	char *curr_class_name; 
	
	for(i=0; i < TABLE_SIZE; i++)
	{
		if(class_tables[i])
		{
			if(find_sym_in_list(t->kid[0]->kid[2]->leaf->text, class_tables[i]->name))
				{
					type1 = find_type_in_list(t->kid[0]->kid[2]->leaf->text, class_tables[i]->name);
					curr_class_name = strdup(class_tables[i]->name); 
					//printf("Found type! %s, %d \n", t->kid[0]->kid[2]->leaf->text, type1); 
				}
		}

	}
	// this is a function - on the right hand side 
	if(t->kid[2]->prodrule == POSTFIX_EXPRESSION_2)
	{
		if(t->kid[2]->kid[0]->prodrule == POSTFIX_EXPRESSION_4)
		{	
			type2 = find_type_in_list(t->kid[2]->kid[0]->kid[2]->leaf->text, curr_class_name); 
			//printf("%s in %s with type: %d\n", t->kid[2]->kid[0]->kid[2]->leaf->text, curr_class_name, type2);
			if(type1 != type2)
			{
				semanticerror("Incorrect type for class member:", t->kid[2]);
			}
		}
		else
		{
        // find type in right hand expression of assignment statement 
		type2 = find_type_in_list(t->kid[2]->kid[0]->leaf->text, "gt"); 
		
	    //printf("%s, type: %d\n", t->kid[0]->kid[2]->leaf->text, type1); 
		//printf("%s, type: %d\n", t->kid[2]->kid[0]->leaf->text, type2); 
		if(type1 != type2)
		{
			semanticerror("Incorrect type:", t->kid[2]); 
		}
		}
		
	}
	// this is an array 
	else if(t->kid[2]->prodrule == POSTFIX_EXPRESSION_1)
	{
        // find type in right hand expression of assignment statement 
		type2 = find_type_in_list(t->kid[2]->kid[0]->leaf->text, s); 
		
	    //printf("%s, type: %d\n", t->kid[0]->kid[2]->leaf->text, type1); 
		//printf("%s, type: %d\n", t->kid[2]->kid[0]->leaf->text, type2); 
		if(type1 != type2)
		{
			semanticerror("Incorrect type:", t->kid[2]); 
		}
		
	}
	else
	{
		// find type in right hand expresion of assignment statement
		type2 = find_type_in_list(t->kid[2]->leaf->text, s); 
		//printf("type2: %s, %d\n", t->kid[2]->leaf->text, type2); 
		if(type1 != type2)
		{
			semanticerror("Incorrect type:", t->kid[2]); 
		}
	}
	
}

// in assignment expressions
void type_class_member_func_check(struct tree *t, int assn_type)
{
    int i; 
    int type1; 
	//int type2; 
	char *curr_class_name; 
	
	for(i=0; i < TABLE_SIZE; i++)
	{
		if(class_tables[i])
		{
			if(find_sym_in_list(t->kid[2]->kid[0]->kid[2]->leaf->text, class_tables[i]->name))
				{
					type1 = find_type_in_list(t->kid[2]->kid[0]->kid[2]->leaf->text, class_tables[i]->name);
					curr_class_name = strdup(class_tables[i]->name); 
					printf("Found type! %s, %d, %s\n", t->kid[2]->kid[0]->kid[2]->leaf->text, type1, curr_class_name); 
					if(type1 != assn_type)
					{
						semanticerror("Class function does not match assignment type:", t->kid[2]->kid[0]->kid[2]); 
					}
				}
		}

	}
	
}

void type_class_member_check_relation(struct tree *t)
{
	int type1; 
	int j, i; 
	int DOT = 46;  // . 
	int NOT = 33;  // ! 
	//char *curr_class_name; 
	
	if(!t)
	{
      // do nothing
	}
	else 
	{ 
	  if (t->nkids == 0)
	  {

		  //printf(" LEAF: \"%s\": %d \n",
				  //t->leaf->text, t->leaf->category); 
		  
		  if(t->leaf->category != IDENTIFIER
		  && t->leaf->category != DOT
		  && t->leaf->category != INTEGER
		  && t->leaf->category != TRUE
		  && t->leaf->category != FALSE
		  && t->leaf->category != ANDAND
		  && t->leaf->category != OROR
		  && t->leaf->category != NOTEQ
		  && t->leaf->category != EQ
		  && t->leaf->category != LTEQ
		  && t->leaf->category != GTEQ
		  && t->leaf->category != NOT)
			{
				semanticerror("Need a boolean type.", t); 
			}
			
		  	for(i=0; i < TABLE_SIZE; i++)
			{
				if(class_tables[i])
				{
					if(find_sym_in_list(t->leaf->text, class_tables[i]->name))
						{
							type1 = find_type_in_list(t->leaf->text, class_tables[i]->name);
							//curr_class_name = strdup(class_tables[i]->name); 
							//printf("Found type! %s, %d \n", t->leaf->text, type1); 
							if(type1 > 0 
							&& type1 != BOOL_TYPE
							&& type1 != INT_TYPE)
							{
								semanticerror("Incorrect class member type, need bool or int type:", t); 
							}
						}
				}

			}

	  }
	  else
	  {

		for(j=0; j<t->nkids; j++)
		{
			type_class_member_check_relation(t->kid[j]);
		}
		
	  }
}
}

