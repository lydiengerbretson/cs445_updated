#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "tree_lydia.h"
#include "120gram_lydia.tab.h"
#include "hash.h"
#include "symbol_table.h"
#include "utility_func.h"

// code help from Tovah Whitesell (SEL)
// ideas and program outline: https://github.com/andschwa/partial-cpp-compiler

// Forward declare this function for compiler
int yylex(); 
int yylex_destroy();
int yy_create_buffer( FILE *file, int size );
void yypush_buffer_state ( int buffer );
void codegen(struct tree * t);

// extern variables in 120lex_lydia.l
extern FILE *yyin; 
extern FILE *output;
extern char *yytext; 
extern struct token *YYTOKEN; 
extern int yyparse(); 
extern YYSTYPE yylval;
extern Treeptr YYPROGRAM; 
extern struct typenametable_entry *HEAD; 

SymbolTable CLASSTABLE; // SymbolTable is a struct table *
SymbolTable CLASS_FUNCTION_TABLE; // ??
SymbolTable FUNCTION_TABLE; // ??
SymbolTable GLOBAL_TABLE; // Globals 
SymbolTable PARAM_TABLE;

char *filetext; 
filenodeptr file_stack = NULL; 

 

int main(int argc, char **argv)
{
   int i;
   char *file_list[argc]; 
   int result = 0; 
   YYPROGRAM = NULL; 
  //struct typenametable_entry *tmp_head; 
  
  CLASSTABLE = new_table( "ct" );

  GLOBAL_TABLE = new_table("gt"); 
  
   int nLen; 
   ++argv, --argc;  /* skip over program name */
   
   for (i = 0; i < argc; i++) 
   {
		file_list[i] = argv[i];
		//printf("file list %s\n", file_list[i]); 
   }
   

   if (argc >= 1)
   {
	   for(i = 0; i < argc; i++)
	   { 

		// helper function adapted from: https://stackoverflow.com/questions/624990/code-to-change-file-extension-up-for-review
          char *fn = malloc(strlen(file_list[i]) + 4);
		  char *temp_file = strdup(file_list[i]); 
		  nLen = strlen(temp_file);
		  if(nLen > 0)
		  {
			  while(nLen)
			  {
				  if(temp_file[nLen] == '.')
				  {
					  temp_file[nLen] = '\0';
					  break;
				  }
				  nLen --; 
			  }
		  }
		  		  
		  sprintf(fn, "%s.ic", temp_file);
		  fprintf(stdout, "File: %s\n", file_list[i]); 
		  yyin = fopen(file_list[i], "r"); 
    
          output = fopen(fn,"a");   
		  
		  if(yyin == NULL)
		  {
			  fprintf(stderr, "File does not exist.\n"); 
			  return -1; 
		  }
		  // push file onto stack 
	      filetext = file_list[i]; 
		  push_file_node(&file_stack, filetext); // adapted from https://github.com/park2331
		  yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
	   
	   }
		result = yyparse(); 
	
		 if (yyin) 
         {
            fclose(yyin);
         }
         else
         {
            fprintf(stderr, "File Input/Output Error: %s. \nPlease check if this file exists or if there is an error within the file.\n", filetext); 
         }
		 if(result == 0)
		 {		
            //print the parse tree if needed (used for testing purposes now)	 
		     //print_tree(YYPROGRAM, 0); 
			//populate the symbol table, inserting variables into symbol tables
			// TODO: add user option to print out symbol table
			populate_symbol_table( YYPROGRAM , GLOBAL_TABLE );
			//print_tree(YYPROGRAM, 0); 
			codegen(YYPROGRAM); 
			
			 // print the names of the symbol tables for testing purposes
             //print_tables(1);
			// print_tables(2);
			 
			 // return with no errors
			 return 0;


		 }
		 else
		 {
			 return 2; 
		 }
		 
			yylex_destroy(); 
			
   }
   return 0;
 }

