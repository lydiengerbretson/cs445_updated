//#include "token.h"
 // ideas and some code snippets adapted from https://github.com/andschwa/partial-cpp-compiler
 
 // create a new token 
 // transferring ownership of memory to struct token members 
struct token* create_token(int category, char *text, int lineno, char *filename)
{
	struct token *new_token =  (struct token*)malloc(sizeof(*new_token)); // without *new_token I get a seg fault 
	
	if(new_token != NULL)
	{
		
		new_token->category = category; 
		new_token->lineno = lineno; 
		
		new_token->text = calloc(strlen(text)+1, sizeof(char));
		strcpy(new_token->text, text);    
		new_token->filename = calloc(strlen(filename)+1, sizeof(char));
		strcpy(new_token->filename, filename); 
		
	
		switch (category)
		{
			case ICON: 
				new_token->ival = atoi(text); 
				break; 
			case SCON:
				new_token->sval = malloc(strlen(text)+1); 
				break; 
			case FCON: 
				new_token->fval = atof(text);
				break; 
			default:  
				break; 
		}
		
		//printf("%d %10s %10d %10s %10d", new_token->category, new_token->text, // testing purposes
				//new_token->lineno, new_token->filename, 
				//new_token->ival);
 
		// add print statement here 
		return new_token; 
		
	}


	
}

 // adapted from https://stackoverflow.com/questions/23279119/creating-and-understanding-linked-lists-of-structs-in-c
// add new token to linked list 
void add_token_to_list(struct token *new_token)
{
   if(YYTOKENLIST != NULL  && new_token != NULL)
   {
	   struct tokenlist *curr = YYTOKENLIST;
	   if(curr->t != NULL)
	   {
			struct tokenlist *new_t = (struct tokenlist*)malloc(sizeof(struct tokenlist)); 
	  
			if(new_t != NULL)
			{
				new_t->t = new_token; 
				new_t->next = NULL; 
			}
	      while(curr->next != NULL)
			{
				curr = curr->next; 
			}
			curr->next = new_t; // add new token  
	   }
	   else
	   {
		   curr->t = new_token; 
		   curr->next = NULL; 
	   }
	  // printf("*****Adding to list: text: %s *****\n", curr->t->text); 
	  // printf("*****Adding to list: category: %d *****\n", curr->t->category); 
   }
   

 // first node will be empty 
}

void print_token_list()
{
	if(YYTOKENLIST != NULL)
	{
		struct tokenlist *curr = YYTOKENLIST;  
		while(curr != NULL)
		{
			// print token	
		   // printf("printing token list...\n"); 
			printf("%d %10s %10d %10s %10d \n", curr->t->category, curr->t->text, 
					curr->t->lineno, curr->t->filename, 
					curr->t->ival);
										
			curr = curr->next; 
		}
	}
}

void clear_tokens(struct tokenlist *tptr)
{        
	if(tptr != NULL)
	{		
		        // Not at the head or tail, keeping going!
         if(tptr->next != NULL) // if not tail
          {
		      clear_tokens(tptr->next);
          }
              
          free_token(tptr->t);
          tptr->next = NULL;
          tptr = NULL;
              
	}


}

void free_token(struct token *t)
{
   if (t != NULL)
   {
      free(t->text);
      t->text = NULL;
      free(t->filename);
      t->filename = NULL;
      free(t->sval);
      t->sval = NULL;
      t = NULL;
   }
}

void construct_list_head() // reallocate new head... constructor of the head
{
 	YYTOKENLIST = (struct tokenlist *)malloc(sizeof(struct tokenlist)); 
 	YYTOKENLIST->t = NULL;
 	YYTOKENLIST->next = NULL;  
}
