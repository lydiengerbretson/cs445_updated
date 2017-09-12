#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "120tree_lydia.h"
#include "120gram_lydia.tab.h"


// code help from Tovah Whitesell (SEL)

// Forward declare this function for compiler
int yylex(); 

// extern variables in clex.l
extern FILE *yyin; 
extern struct tokenlist *YYTOKENLIST; 
extern struct token *YYTOKEN; 
extern int yyparse(); 
extern YYSTYPE yylval;
extern struct tree *YYPROGRAM; 

char *filetext; 
filenodeptr file_stack = NULL; 
 

int main(int argc, char **argv)
{
   int i;
   int k; 
   int eof;
   struct tokenlist* head;
   char *file_list[argc]; 
   int parse = 0; 
   
   ++argv, --argc;  /* skip over program name */
   
   for (i = 0; i < argc; i++) 
   {
		file_list[i] = argv[i];
   }
   

   if (argc >= 1)
   {
	   for(i = 0; i < argc; i++)
	   { 
	      construct_list_head();

		  yyin = fopen(file_list[i], "r"); 
		  // push file onto stack 
	      filetext = file_list[i]; 
		  push_file_node(&file_stack, filetext); // adapted from https://github.com/park2331
		  yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
	   }
		   while (1 && yyin) 
		   { 
			   //eof = yylex();
			   parse = yyparse(); 
			   if (eof == 0)
			   {
				   break;
			   }
		
		   }		
         
         if (yyin) 
         {
            fclose(yyin);
         }
         else
         {
            printf("File Input/Output Error: %s. \nPlease check if this file exists or if there is an error within the file.\n", filetext); 
         }
	     
	     //printf("Category          Text         Line no           Filename      Ival        Sval\n"); 
	     //printf("-----------------------------------------------------------------------------------\n"); 
	     //print_token_list(); 
	    // head = YYTOKENLIST;
	     //clear_tokens(head); 
		 
		 //print_tree(YYPROGRAM, 0);  
         
        }
     }


	
     
