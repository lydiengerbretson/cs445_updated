/*	$Id: lexer.l,v 1.2 1997/11/19 15:13:15 sandro Exp $	*/

/*
 * Copyright (c) 1997 Sandro Sigala <ssigala@globalnet.it>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * ISO C++ lexical analyzer.
 *
 * Based on the ISO C++ draft standard of December '96.
 * 
 * Lydia Engerbretson
 * Fall 2017
 * adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/120pp/120lex.l
 *
 */

%{
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "120gram_lydia.tab.h"
#include "token.h"
#include "tree_lydia.h"
#include "symbol_table.h"

int lineno;
int saw_iostream, saw_string, saw_std;

#define MAX_INCLUDE_DEPTH 10
YY_BUFFER_STATE include_stack[MAX_INCLUDE_DEPTH]; 
int include_stack_ptr = 0; 

static void skip_until_eol(void);
static void skip_comment(void);
static int check_identifier(char *, SymbolTable);
void insert_typename_tree(struct tree *t, SymbolTable table); 
int typenametable_insert(char *s, SymbolTable table); 

// extern variables
extern filenodeptr file_stack;
extern char* filetext; 
extern FILE *yyin; 
extern YYSTYPE yylval; 
extern SymbolTable CLASSTABLE;
extern SymbolTable GLOBAL_TABLE; 
extern struct typenametable_entry *HEAD; 

// global variables
struct token *YYTOKEN; 
char* file_name; 

void lexerr(const char *s); 
int add_token(int category);
void handle_include(char *s); 
%}
%option yylineno
%option noyywrap
%x INCLUDE STR

intsuffix				([uU][lL]?)|([lL][uU]?)
fracconst				([0-9]*\.[0-9]+)|([0-9]+\.)
exppart					[eE][-+]?[0-9]+
floatsuffix				[fFlL]
chartext				([^'])|(\\.)
stringtext				([^"])|(\\.)

%%
"#include"              {BEGIN(INCLUDE);}
<INCLUDE>{
"<iostream>"            { saw_iostream = 1; BEGIN(INITIAL); }
"<fstream>"             { BEGIN(INITIAL); }
"<cstdlib>"             { BEGIN(INITIAL); }
"<cstring>"             { BEGIN(INITIAL); }
"<string>"              { saw_string = 1; BEGIN(INITIAL);  }
"<ctime>" 				{ BEGIN(INITIAL); }
"<cmath>"               { BEGIN(INITIAL); }
"<iomanip>"             { BEGIN(INITIAL); }

\"[^\"]+\"              { handle_include(yytext); BEGIN(INITIAL); }
}

"using namespace std;"			{ saw_std = 1;
		if (saw_string) typenametable_insert("string", CLASSTABLE);
		if (saw_iostream) {
			typenametable_insert("ifstream", CLASSTABLE);
			typenametable_insert("ofstream", CLASSTABLE);
			}
		}

"\n"					{ ++lineno; }
[\t\f\v\r ]+			{ /* Ignore whitespace. */ }

"/*"					{ skip_comment(); }
"//"					{ skip_until_eol(); }

"bool"					{ return add_token(BOOL); }
"break"					{ return add_token(BREAK); }
"case"					{ return add_token(CASE); }
"char"					{ return add_token(CHAR); }
"class"					{ return add_token(CLASS); }
"continue"				{ return add_token(CONTINUE); }
"default"				{ return add_token(DEFAULT); }
"delete"				{ return add_token(DELETE); }
"do"					{ return add_token(DO); }
"double"				{ return add_token(DOUBLE); }
"else"					{ return add_token(ELSE); }
"false"					{ return add_token(FALSE); }
"float"					{ return add_token(FLOAT); }
"for"					{ return add_token(FOR); }
"if"					{ return add_token(IF); }
"int"					{ return add_token(INT); }
"long"					{ return add_token(LONG); }
"new"					{ return add_token(NEW); }
"private"				{ return add_token(PRIVATE); }
"protected"				{ return add_token(PROTECTED); }
"public"				{ return add_token(PUBLIC); }
"return"				{ return add_token(RETURN); }
"short"					{ return add_token(SHORT); }
"signed"				{ return add_token(SIGNED); }
"struct"				{ return add_token(STRUCT); }
"static"                { return add_token(STATIC); }
"switch"				{ return add_token(SWITCH); }
"true"					{ return add_token(TRUE); }
"unsigned"				{ return add_token(UNSIGNED); }
"void"					{ return add_token(VOID); }
"while"					{ return add_token(WHILE); }

"inline"                { lexerr("Unsupported token.");}
"mutable"               { lexerr("Unsupported token.");}
"namespace"             { lexerr("Unsupported token."); }
"operator" 	            { lexerr("Unsupported token."); }
"explicit"              { lexerr("Unsupported token."); }
"export"                { lexerr("Unsupported token."); }
"dynamic_cast"          { lexerr("Unsupported token."); }
"enum"                  { lexerr("Unsupported token.");}
"extern"                { lexerr("Unsupported token."); }
"const"                 { lexerr("Unsupported token."); }
"const_cast"            { lexerr("Unsupported token."); }
"asm"                   { lexerr("Unsupported token."); }
"auto"                  { lexerr("Unsupported token."); }
"friend"                { lexerr("Unsupported token.");  }
"register"              { lexerr("Unsupported token."); }
"reinterpret_cast"      { lexerr("Unsupported token."); }
"wchar_t"               { lexerr("Unsupported token.");}
"volatile"              { lexerr("Unsupported token."); }
"using"                 { lexerr("Unsupported token.");}
"virtual"               { lexerr("Unsupported token."); }
"union"					{ lexerr("Unsupported token."); }
"try"                   { lexerr("Unsupported token.");}
"typedef"               { lexerr("Unsupported token.");  }
"throw"                 { lexerr("Unsupported token."); }
"template"              { lexerr("Unsupported token."); }
"this"                  { lexerr("Unsupported token."); }
"typeid"                { lexerr("Unsupported token.");}
"typename"              { lexerr("Unsupported token."); }
"static_cast"           { lexerr("Unsupported token."); }
"sizeof"				{ lexerr("Unsupported token."); }
"..."					{ lexerr("Unsupported token."); }


"{"					    { return add_token('{'); }
"}"					    { return add_token('}'); }
"["					    { return add_token('['); }
"]"					    { return add_token(']'); }
"("					    { return add_token('('); }
")"					    { return add_token(')'); }
";"					    { return add_token(';'); }
":"					    { return add_token(':'); }
"?"					    { return add_token('?'); }
"::"					{ return add_token(COLONCOLON); }
"."					    { return add_token('.'); }
".*"					{ return add_token(DOTSTAR); }
"+"					    { return add_token('+'); }
"-"					    { return add_token('-'); }
"*"					    { return add_token('*'); }
"/"					    { return add_token('/'); }
"%"					    { return add_token('%'); }
"^"					    { return add_token('^'); }
"&"					    { return add_token('&'); }
"|"					    { return add_token('|'); }
"~"					    { return add_token('~'); }
"!"					    { return add_token('!'); }
"="					    { return add_token('='); }
"<"					    { return add_token('<'); }
">"					    { return add_token('>'); }
"+="					{ return add_token(ADDEQ); }
"-="					{ return add_token(SUBEQ); }
"*="					{ return add_token(MULEQ); }
"/="					{ return add_token(DIVEQ); }
"%="					{ return add_token(MODEQ); }
"^="					{ return add_token(XOREQ); }
"&="					{ return add_token(ANDEQ); }
"|="					{ return add_token(OREQ); }
"<<"					{ return add_token(SL); }
">>"					{ return add_token(SR); }
"<<="					{ return add_token(SLEQ); }
">>="					{ return add_token(SREQ); }
"=="					{ return add_token(EQ); }
"!="					{ return add_token(NOTEQ); }
"<="					{ return add_token(LTEQ); }
">="					{ return add_token(GTEQ); }
"&&"					{ return add_token(ANDAND); }
"||"					{ return add_token(OROR); }
"++"					{ return add_token(PLUSPLUS); }
"--"					{ return add_token(MINUSMINUS); }
","					    { return add_token(','); }
"->*"					{ return add_token(ARROWSTAR); }
"->"					{ return add_token(ARROW); }

[a-zA-Z_][a-zA-Z_0-9]*			    { return add_token(check_identifier(yytext, CLASSTABLE));  }

"0"[xX][0-9a-fA-F]+{intsuffix}?		{ return add_token(INTEGER); }
"0"[0-7]+{intsuffix}?			    { return add_token(INTEGER); }
[0-9]+{intsuffix}?			        { return add_token(INTEGER); }

{fracconst}{exppart}?{floatsuffix}?	{ return add_token(FLOATING); }
[0-9]+{exppart}{floatsuffix}?		{ return add_token(FLOATING); }

"'"{chartext}*"'"			    { return add_token(CHARACTER); }
"L'"{chartext}*"'"			    { return add_token(CHARACTER); }

"\""{stringtext}*"\""			{ return add_token(STRING); }
"L\""{stringtext}*"\""			{ return add_token(STRING); }



.					            { lexerr("Unrecognized token.\n"); }

<<EOF>>                 {
 
  yypop_buffer_state();
  pop_file_node(&file_stack); // adapted from https://github.com/park2331
  if (file_stack == NULL) {
    yyterminate();    
  }

  yylineno = 1;
  
  if (!YY_CURRENT_BUFFER) {
    yyterminate();
  } 
}

%%

// create token and add to tree 
int add_token(int category)
{
   // global YYTOKEN
    filetext = file_stack->filename;
   	YYTOKEN = create_token(category, yytext, yylineno, filetext);  
	
    // adapted from https://github.com/park2331
	yylval.t = create_tree(yytext, category, 0); 	
	yylval.t->leaf = YYTOKEN; 
	
	return category; 
   
  	
}


/*
 * We use this routine instead a lex pattern because we don't need
 * to save the matched comment in the `yytext' buffer.
 */
static void skip_comment(void)
{
	int c1, c2;

	c1 = input();
	c2 = input();

	while (c2 != EOF && !(c1 == '*' && c2 == '/')) {
		if (c1 == '\n')
			++lineno;
		c1 = c2;
		c2 = input();
	}
}

/*
 * See the previous comment for an explanation.
 */
static void skip_until_eol(void)
{
	int c;

	while ((c = input()) != EOF && c != '\n')
		;
	++lineno;
}
 

void insert_typename_tree(struct tree *t, SymbolTable table)
{
    // adapted from https://github.com/andschwa/partial-cpp-compiler
	// adapted from https://github.com/park2331/compiler
    // checking 
   if ( lookup( t->leaf->text , table ) == true) {
        fprintf(stderr,
	    "warning, %s is already in the typename table as a %d\n",
            t->leaf->text, t->leaf->category );
   } else {

     //printf("INSERTED %s: %d\n ",t->leaf->text, t->leaf->category);
     insert_sym( t->leaf->text , table );

   }
    
}

// typenametable functions adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/120pp/120lex.l
int typenametable_insert(char *s, SymbolTable t)
{
   insert_sym(s, t); 
}

// handle user include files
void handle_include(char *s)
{
  // help from Clinton Jeffery
  FILE *include_buffer;
  YY_BUFFER_STATE incl_state;

  char *fname = malloc(strlen(yytext)+1-strlen("#include \"\""));
  fname = strchr(yytext, '\"')+1;
  fname[strlen(fname)-1] = '\0';
  printf("%s\n",fname);
  yyin = fopen(fname,"r");
  if(yyin == NULL)
  {
     printf("File %s is not recognized. \n", fname); 
	 return; 
  }
  yylineno = 1; 
  push_file_node(&file_stack, fname);   // adapted from https://github.com/park2331
  yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
  
   
}

// lexical error function
void lexerr(const char *s)
{
    filetext = file_stack->filename;
	fprintf (stderr, "File %s line %d, token %s: %s\n", filetext, yylineno, yytext, s);
    exit (1); 
}

static int check_identifier(char *s, SymbolTable t)
{
    // adapted from https://github.com/park2331/compiler
	if(lookup(s, t) == true)
	{
		return CLASS_NAME; 
	}
	else
	{
		return IDENTIFIER; 
	}

}