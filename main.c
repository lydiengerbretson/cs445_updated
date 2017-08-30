#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "ytab.h"

int yylex(); // Forward declare this function for compiler
// create a global that tokenlist *head here 
struct tokenlist *YYTOKENLIST; 

int main(int argc, char **argv)
{
	
	int i;
	int k; 
	int eof;
   struct tokenlist* head;
   ++argv, --argc;  /* skip over program name */

	// c
	// add tokens to token list in clex.l
	// print token list in main.c
   if (argc >= 1)
   {
	   for(i = 0; i < argc; i++)
	   { 
	      construct_list_head();
	      
		  printf("Opening %s\n", argv[i]); 
		  //printf("Category          Text         Line no           Filename      Ival/Sval\n"); 
	     // printf("---------------------------------------------------------------------------\n"); 
		  curr_filename = argv[i]; 
		  yyin = fopen(argv[i], "r");	
		   while (1 && yyin) 
		   {
			   eof = yylex();
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
            printf("File %s not recognized.\n",argv[i]);
            break;
         }
	     
	      printf("Category          Text         Line no           Filename      Ival/Sval\n"); 
	      printf("---------------------------------------------------------------------------\n"); 
	      print_token_list(); 
	      head = YYTOKENLIST;
	      clear_tokens(head); 
         
        }
     }


	}
	
     
