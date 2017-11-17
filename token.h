#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tac.h"


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
   double fval;
   struct addr address; 
   };               /*    the string (less quotes and after escapes) here */ 
   
 
  // adapted from https://github.com/park2331
typedef struct file_node
{
	struct token *t; 
	char *filename; 
	struct file_node *next; 

} *filenodeptr; 

  
 // create a new token 
struct token* create_token(int category, char *text, int lineno, char *filename); 

// pop file node from file stack 
void pop_file_node(filenodeptr*); 

// push file node onto file stack 
void push_file_node(filenodeptr*, char *filename); 

#endif /*TOKEN_H*/
