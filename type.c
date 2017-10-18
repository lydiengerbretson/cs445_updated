#include "type.h"
#include "120gram_lydia.tab.h"
#include "nonterms.h"

// function adapted from https://github.com/westrope/wookie-chomp/blob/hw3/Hw2/prodrule.c
int get_base_type( struct tree *t)
{

	if(strcmp(t->leaf->text, "string") == 0)
		return STRING_TYPE;
	if(strcmp(t->leaf->text, "ofstream") == 0)
		return OFSTREAM_TYPE;
	if(strcmp(t->leaf->text, "LYDIA") == 0)
		return 1234; // testing purposes

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
	default:
		return 0;
	}

  
}

