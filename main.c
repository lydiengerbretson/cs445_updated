#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "tree_lydia.h"
#include "120gram_lydia.tab.h"
#include "hash.h"
#include "symbol_table.h"

// code help from Tovah Whitesell (SEL)
// ideas and program outline: https://github.com/andschwa/partial-cpp-compiler

// Forward declare this function for compiler
int yylex(); 

// extern variables in 120lex_lydia.l
extern FILE *yyin; 
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
   int k; 
   int eof;
   struct tokenlist* head;
   char *file_list[argc]; 
   int result = 0; 
   YYPROGRAM = NULL; 
  //struct typenametable_entry *tmp_head; 
  CLASSTABLE = new_table( "classtable" );
  CLASS_FUNCTION_TABLE = new_table("class_function_table"); 
  FUNCTION_TABLE = new_table("local_table"); 
  GLOBAL_TABLE = new_table("global_table"); 
  PARAM_TABLE = new_table("parameter_table"); 
   
   ++argv, --argc;  /* skip over program name */
   
   for (i = 0; i < argc; i++) 
   {
		file_list[i] = argv[i];
		printf("file list %s\n", file_list[i]); 
   }
   

   if (argc >= 1)
   {
	   for(i = 0; i < argc; i++)
	   { 

		  printf("Opening %s\n", file_list[i]); 
		  yyin = fopen(file_list[i], "r"); 
		  
		  if(yyin == NULL)
		  {
			  printf("File does not exist.\n"); 
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
            printf("File Input/Output Error: %s. \nPlease check if this file exists or if there is an error within the file.\n", filetext); 
         }
		 if(result == 0)
		 {			 
			//print_tree(YYPROGRAM, 0); 
			//populate symbol table
			 populate_symbol_table( YYPROGRAM , GLOBAL_TABLE );
			//populate_symbol_table(YYPROGRAM, CLASSTABLE); 
			//populate_symbol_table(YYPROGRAM, FUNCTION_TABLE); 

		 }
		 else
		 {
			 return 2; 
		 }
		 
			yylex_destroy(); 
			
   }
 }

