#include "type.h"
#include "120gram_lydia.tab.h"
#include "nonterms.h"

// function adapted from https://github.com/westrope/wookie-chomp/blob/hw3/Hw2/prodrule.c
int get_base_type( struct tree *t)
{
	
	switch(t->prodrule)
	{
		
	case INT:
		printf("INT TYPE!\n");
		return 1;
	case DOUBLE:
		printf("DOUBLE TYPE!\n");
		return 2;
	case CHAR:
		printf("CHAR TYPE!\n");
		return 3;
	case VOID:
		printf("VOID TYPE!\n");
		return 4;
	default:
		return 0;
	}
	
	
	return 0;

  
}

