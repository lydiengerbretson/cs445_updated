#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "ytab.h"

int yylex(); // Forward declare this function for compiler
// create a global that tokenlist *head here 
struct tokenlist *YYTOKENLIST; 

//filenodeptr file_stack = NULL; 

int main(int argc, char **argv)
{
	
	int i;
	int k; 
	int eof;
   struct tokenlist* head;
   char *file_list[argc]; 
   ++argv, --argc;  /* skip over program name */
   

   if (argc >= 1)
   {
	   for(i = 0; i < argc; i++)
	   { 
	      construct_list_head();
	      
		  printf("Opening %s\n", argv[i]); 
		  curr_filename = argv[i]; 
		  
		  //file_list[i] = argv[i]; 
		 //strcpy( file_name, file_list[i]); 

		  yyin = fopen(argv[i], "r");	
		  // push file onto stack 
		  //push_file_node(&file_stack, file_name); 
		  //yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
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
			//yypop_buffer_state(); 
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
	
     
