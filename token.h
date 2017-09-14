#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef TOKEN_H
#define TOKEN_H

/* Size of default input buffer. */
/* Retrieved from lex.yy.c file */
#ifndef YY_BUF_SIZE
#ifdef __ia64__
/* On IA-64, the buffer size is 16k, not 8k.
 * Moreover, YY_BUF_SIZE is 2*YY_READ_BUF_SIZE in the general case.
 * Ditto for the __ia64__ case accordingly.
 */
#define YY_BUF_SIZE 32768
#else
#define YY_BUF_SIZE 16384
#endif /* __ia64__ */
#endif

extern struct tokenlist *YYTOKENLIST; 

// adapted from Dr. Clinton Jeffery
struct token {
   int category;   /* the integer code returned by yylex */
   char *text;     /* the actual string (lexeme) matched */
   int lineno;     /* the line number on which the token occurs */
   char *filename; /* the source file in which the token occurs */
   int ival;       /* if you had an integer constant, store its value here */
   char *sval;      /* if you had a string constant, malloc space and store */
   /* add char cval */ 
   /* add double dval */
   };               /*    the string (less quotes and after escapes) here */ 
   
struct tokenlist {
   struct token *t;
   struct tokenlist *next;
   };
 
  // adapted from https://github.com/park2331
typedef struct file_node
{
	struct token *t; 
	char *filename; 
	struct file_node *next; 

} *filenodeptr; 

  
 // create a new token 
struct token* create_token(int category, char *text, int lineno, char *filename); 

// add new token to linked list 
void add_token_to_list(struct token *new_token); 

void print_token_list(); 

// clear tokens in linked list 
void clear_tokens(); 

void free_token(struct token *t); 

void construct_list_head();

// pop file node from file stack 
void pop_file_node(filenodeptr*); 

// push file node onto file stack 
void push_file_node(filenodeptr*, char *filename); 

#endif /*TOKEN_H*/
