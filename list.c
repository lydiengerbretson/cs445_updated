#include "list.h"

void insert_sym_list(char *s, char *t, int typ)
{
    struct entry *new_node = calloc(1, sizeof(Entry));
	
	struct entry  *temp;

	new_node->sym_table_name = strdup(t); 	
	new_node->name = strdup(s); 
	new_node->typ = typ;	
	new_node->next = NULL; 
	
	if(start == NULL)
	{
		// insert first node
		//printf("**Inserting first node: %s\n", new_node->name); 
		start = new_node; 
		//curr = new_node;
	}
	else
	{
		temp = start; 
		while(temp->next != NULL)
		{
			temp = temp->next; 
		}
		//printf("**Inserting the rest: %s\n", new_node->name); 
		temp->next = new_node; 
	}
	
}

bool find_sym_in_list(char *s, char *t)
{
	struct entry *temp;
	temp = start; 
	
	while(temp != NULL)
	{
		if(strcmp(s, temp->name) == 0 && strcmp(temp->sym_table_name, t) ==0)
		{
		//printf("**Printing the wanted symbol: %s type: %d\n", temp->name, temp->typ); 
		return true; 
		}
		temp = temp->next;
		
	}
	return false;
		
}

int find_type_in_list(char *s, char *t)
{
	
	struct entry *temp;
	temp = start; 
	
	while(temp != NULL)
	{
		//printf("**Printing the wanted symbol: %s type: %d, scope: %s \n",  temp->name, temp->typ, temp->sym_table_name); 

		if(strcmp(s, temp->name) == 0 && strcmp(t, temp->sym_table_name) == 0 )
		{
		//printf("**Printing the wanted symbol: %s type: %d, scope: %s \n",  temp->name, temp->typ, temp->sym_table_name); 
		return temp->typ;

		}
		temp = temp->next;
		
	}
	return 0;
}



void print_syms_in_list( char *t)
{
	struct entry *temp;
	temp = start; 
	
	while(temp != NULL)
	{
		if(strcmp(temp->sym_table_name, t) == 0)
		{
		printf("    %s %d\n", temp->name, temp->typ); 
		//return true; 
		}
		temp = temp->next;
		
	}
	//return false;
		
}

void parm_entry_in_list(char *t, char* parent_text, int parent_type)
{
	struct entry *temp;
	temp = start; 
	
	while(temp != NULL)
	{
		if(strcmp(temp->sym_table_name, t) == 0)
		{
            //printf(" parent text: %s, type: %d\n", parent_text, parent_type); 
			if(parent_type > 0 && parent_type != temp->typ)
			{
				printf("Problem here.\n"); 
			}
			printf("    %s %d\n", temp->name, temp->typ); 

		}
		temp = temp->next;
		
	}

		
}

int check_syms_in_list( char *t, int type)
{
	struct entry *temp;
	temp = start; 
	
	while(temp != NULL)
	{
		if(strcmp(temp->sym_table_name, t) == 0)
		{
		//printf("    %s %d\n", temp->name, temp->typ); 
		if(temp->typ > 0 && type != temp->typ)
		{
			//printf("Error %d is not equal to %d\n", type, temp->typ); 
			return 0;
		}
		if(temp->typ > 0 && type == temp->typ)
		{
			// only runs through it once.
			//printf("Correct %d is equal to %d\n", type, temp->typ); 
			return 1;
		}
		
		}
		temp = temp->next;
		
	}
	return 1;
		
}