#include "type.h"
#include "120gram_lydia.tab.h"
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
	int type2;
    int k, j;
	static int is_post = 0;

  if(!t)
  {
      // do nothing
  }
  else 
  { 
	  if (t->nkids == 0)
	  {

		  //printf(" LEAF: \"%s\": %d %d\n",
				 // t->leaf->text, t->leaf->category, type1); 
		  type1 = find_type_in_list(t->leaf->text, table_name);
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
			  semanticerror("Incompatible types.", t);
		  }
		  }
	  }
	  else
	  {
		//printf(" KID %s: %d\n", t->prodrule_name, t->nkids);
		if(t->prodrule == POSTFIX_EXPRESSION_1)
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

void type_relation_check(struct tree *t, char *table_name)
{
	int type1; 
	int type2;
    int k, j;
	static int rel_exp = 0;
	// TODO: Pass in type!
	

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
			  //printf("yay!!\n");
		       if(type1 != BOOL_TYPE 
			   && type1 != INT_TYPE 
			   && type1 != DOUBLE_TYPE
			   && t->leaf->category != DOUBLE
			   && t->leaf->category != FLOATING
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
			       semanticerror("Incompatible types.", t);
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
	int type2;
    int k, j;
	int kids1;
	int kids2;
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
		  
		  // if not an operator 
		  if(type1 > 0 && type1 != parent_type)
		  {
			  semanticerror("Incompatible types.", t);
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
	int type2;
    int k, j;


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
		 // printf(" LEAF: \"%s\": %d %d\n",
				 //t->leaf->text, t->leaf->category, type1); 
		  
		  // if not an operator 
		  if(type1 > 0 
		  && type1 != INT_TYPE
		  && type1 != CHAR_TYPE)
		  {
			  semanticerror("Incompatible types.", t);
		  }

	  }
	  else
	  {
		//printf(" KID: %s: %d\n", t->prodrule_name, t->nkids);
 
		for(j=0; j<t->nkids; j++)
		{
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
	// this means that this is an array!
	if(t->kid[0]->prodrule == POSTFIX_EXPRESSION_1)
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
	&& t->kid[2]->leaf->category != DOUBLE)
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

