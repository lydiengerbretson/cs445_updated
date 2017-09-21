#include "120gram_lydia.tab.h"
#include "token.h"

 // seg fault and pointer help from Tovah Whitesell
 
 // get_sval and get_cval remove the " and ' from each 
 // string end respectively
char * get_sval(char * in_str)
{
	char * temp = (char *)malloc(strlen(in_str) +1 );
	memset(temp, 0, strlen(in_str));
	int i = 1;
	int j = i - 1;
	for (i; in_str[i] != '"'; i++, j++)
	{
		temp[j] = in_str[i];
	}
	temp[strlen(temp) - 1] = '\0';
	strcpy(in_str, temp);
	return in_str;
}
char * get_cval(char *cval)
{
	char * temp = (char *)malloc(strlen(cval) +1 );
	memset(temp, 0, strlen(cval));
	int i = 1;
	int j = i - 1;
	for (i; cval[i] != '\''; i++, j++)
	{
		temp[j] = cval[i];
	}
	temp[strlen(temp) - 1] = '\0';
	strcpy(cval, temp);
	return cval;
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
		new_token->fval = 0.0; 
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
			case FLOATING: 
				new_token->fval = atof(text); 
				break;  
			default:  
				break; 
		}
		
		return new_token; 
		
	}	
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
