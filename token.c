#include "token.h"
#include "120gram_lydia.tab.h"

 // seg fault and pointer help from Tovah Whitesell
 
 // adapted from https://github.com/park2331
 char *get_sval(char *sval) {

  int len = 0;
  char* inc = sval;
  char* updated_sval = malloc(strlen(sval)); 

  // increment pointer past first quote
  while (*inc++ != '\"') 
  {
    inc++;
  }

  // increment pointer to the final quote
  while (*inc != '\"') 
  {
    if (inc[0] == '\\') 
	{
      char esc = inc[1];
      if (esc == 'n' )
	  {
		updated_sval[len] = '\n';
		// len++; // ?
		inc++;
		inc++;
      } 
	  else if (esc == 't')
	  {
		updated_sval[len] = '\t';
		// len++; // ?
		inc++;
		inc++;
	  }
	  else if (esc == '0')
	  {
		updated_sval[len] = '\0';
		// len++; // ?
		inc++;
		inc++;
	  }
	  else
	  {
		inc++;
		inc++;
	  }
    } else 
	{
      updated_sval[len] = *inc;
      len++;
      inc++;
    }
  }
  updated_sval[len] = '\0';
  updated_sval = realloc(updated_sval, len);
  return updated_sval;
 }

 char *get_cval(char *cval) {

  int len = 0;
  //int i = 0; 
  char* inc = cval;
  char* updated_sval = malloc(strlen(cval)); 

  // increment pointer past first tick
  while (*inc++ != '\'') 
  {
    inc++;
  }

  // increment pointer to the final tick
  while (*inc != '\'') 
  {
      updated_sval[len] = *inc;
      len++;
      inc++;
  }
  updated_sval[len] = '\0';
  updated_sval = realloc(updated_sval, len);
  return updated_sval;
}
 // create a new token 
 // transferring ownership of memory to struct token members 
  // adapted from https://github.com/andschwa/partial-cpp-compiler
struct token* create_token(int category, char *text, int lineno, char *filename)
{
	struct token *new_token =  (struct token*)malloc(sizeof(struct token)); // without *new_token I get a seg fault 
	char *c;
	
	if(new_token != NULL)
	{
		
		new_token->category = category; 
		new_token->lineno = lineno; 
		
		new_token->text = calloc(strlen(text)+1, sizeof(char));
		strcpy(new_token->text, text);    
		new_token->filename = calloc(strlen(filename)+1, sizeof(char));
		strcpy(new_token->filename, filename); 
		
		// default values
		new_token->ival = 0; 
		new_token->sval = calloc(8, sizeof(char));
		strcpy(new_token->sval, "N/A");

	
		switch (category)
		{
			case INTEGER: 
				new_token->ival = atoi(text); 
				break;  
			case STRING:
			    new_token->sval = malloc(strlen(text)+1); 
				new_token->sval = get_sval(text); 
				break; 
			case CHARACTER:
				new_token->sval = malloc(strlen(text)+1); 
				c = get_cval(text); 
				strcpy(new_token->sval, c); 
				break; 
			default:  
				break; 
		}
		
		return new_token; 
		
	}	
}


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
   }
   

 // first node will be empty 
}

void print_token_list()
{
	if(YYTOKENLIST != NULL)
	{
		struct tokenlist *curr = YYTOKENLIST;  

		while((curr != NULL) && (curr->t != NULL)) // Nobody likes a seg fault
		{
		   if (curr->t->text != NULL && curr->t->filename != NULL)
		   {
			   printf("%d %20s %10d %20s %10d %10s\n", curr->t->category, curr->t->text, 
					   curr->t->lineno, curr->t->filename, 
					   curr->t->ival, curr->t->sval);
			}							
			curr = curr->next; 
		}
	}
}

void clear_tokens(struct tokenlist *tptr)
{        
	if(tptr != NULL)
	{		
      // Not at the tail, keeping going!
      if(tptr->next != NULL) 
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

// adapted from https://github.com/park2331
void pop_file_node(filenodeptr *top)
{
  filenodeptr temp;
  temp = *top;
  *top = (*top)->next;
  free(temp);
}

void push_file_node(filenodeptr *top, char *filename)
{
  filenodeptr temp = (filenodeptr)malloc(sizeof(struct file_node));
  temp->filename = malloc(sizeof(filename)+1);
  temp->next = NULL;
  strcpy(temp->filename, filename);

  if(top == NULL) {
    *top = temp;
  } else {
    temp->next = *top;
    *top = temp;
  }
}
