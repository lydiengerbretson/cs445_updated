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

int lineno;
int saw_iostream, saw_string, saw_std;

//static int yywrap(void);
static void skip_until_eol(void);
static void skip_comment(void);
static int check_identifier(char *);
void insert_typename_tree(struct tree *t, int category); 

// extern variables
extern filenodeptr file_stack;
extern char* filetext; 
extern FILE *yyin; 
extern YYSTYPE yylval; 

// global variables
struct token *YYTOKEN; 
struct tokenlist *YYTOKENLIST; 
char* file_name; 
char* temp_name; 

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
"<iostream>"            { BEGIN(INITIAL); }
"<fstream>"             { BEGIN(INITIAL); }
"<cstdlib>"             { BEGIN(INITIAL); }
"<cstring>"             { BEGIN(INITIAL); }
"<string>"              { BEGIN(INITIAL); }
"<ctime>" 				{ BEGIN(INITIAL); }
"<cmath>"               { BEGIN(INITIAL); }
"<iomanip>"             { BEGIN(INITIAL); }
\"[^\"]+\"              { handle_include(yytext); BEGIN(INITIAL); }
}

"\n"					{ ++lineno; }
[\t\f\v\r ]+			{ /* Ignore whitespace. */ }

"/*"					{ skip_comment(); }
"//"					{ skip_until_eol(); }

"{"					    { return add_token('{'); }
"<%"				    { return add_token('{'); }
"}"					    { return add_token('}'); }
"%>"					{ return add_token('}'); }
"["					    { return add_token('['); }
"<:"					{ return add_token('['); }
"]"					    { return add_token(']'); }
":>"					{ return add_token(']'); }
"("					    { return add_token('('); }
")"					    { return add_token(')'); }
";"					    { return add_token(';'); }
":"					    { return add_token(':'); }
"..."					{ return add_token(ELLIPSIS); }
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
"xor"					{ return add_token('^'); }
"&"					    { return add_token('&'); }
"bitand"				{ return add_token('&'); }
"|"					    { return add_token('|'); }
"bitor"					{ return add_token('|'); }
"~"					    { return add_token('~'); }
"compl"					{ return add_token('~'); }
"!"					    { return add_token('!'); }
"not"					{ return add_token('!'); }
"="					    { return add_token('='); }
"<"					    { return add_token('<'); }
">"					    { return add_token('>'); }
"+="					{ return add_token(ADDEQ); }
"-="					{ return add_token(SUBEQ); }
"*="					{ return add_token(MULEQ); }
"/="					{ return add_token(DIVEQ); }
"%="					{ return add_token(MODEQ); }
"^="					{ return add_token(XOREQ); }
"xor_eq"				{ return add_token(XOREQ); }
"&="					{ return add_token(ANDEQ); }
"and_eq"				{ return add_token(ANDEQ); }
"|="					{ return add_token(OREQ); }
"or_eq"					{ return add_token(OREQ); }
"<<"					{ return add_token(SL); }
">>"					{ return add_token(SR); }
"<<="					{ return add_token(SLEQ); }
">>="					{ return add_token(SREQ); }
"=="					{ return add_token(EQ); }
"!="					{ return add_token(NOTEQ); }
"not_eq"				{ return add_token(NOTEQ); }
"<="					{ return add_token(LTEQ); }
">="					{ return add_token(GTEQ); }
"&&"					{ return add_token(ANDAND); }
"and"					{ return add_token(ANDAND); }
"||"					{ return add_token(OROR); }
"or"					{ return add_token(OROR); }
"++"					{ return add_token(PLUSPLUS); }
"--"					{ return add_token(MINUSMINUS); }
","					    { return add_token(','); }
"->*"					{ return add_token(ARROWSTAR); }
"->"					{ return add_token(ARROW); }

"asm"                   { return add_token(ASM); }
"auto"                  { return add_token(AUTO); }
"bool"					{ return add_token(BOOL); }
"break"					{ return add_token(BREAK); }
"case"					{ return add_token(CASE); }
"char"					{ return add_token(CHAR); }
"class"					{ return add_token(CLASS); }
"continue"				{ return add_token(CONTINUE); }
"const"                 { return add_token(CONST); }
"const_cast"            { return add_token(CONST_CAST); }
"default"				{ return add_token(DEFAULT); }
"delete"				{ return add_token(DELETE); }
"do"					{ return add_token(DO); }
"double"				{ return add_token(DOUBLE); }
"dynamic_cast"          { return add_token(DYNAMIC_CAST); }
"else"					{ return add_token(ELSE); }
"enum"                  { return add_token(ENUM); }
"explicit"              { return add_token(EXPLICIT); }
"export"                { return add_token(EXPORT); }
"extern"                { return add_token(EXTERN); }
"false"					{ return add_token(FALSE); }
"float"					{ return add_token(FLOAT); }
"for"					{ return add_token(FOR); }
"friend"                { return add_token(FRIEND); }
"if"					{ return add_token(IF); }
"inline"                { return add_token(INLINE); }
"int"					{ return add_token(INT); }
"long"					{ return add_token(LONG); }
"mutable"               { return add_token(MUTABLE); }
"namespace"             { return add_token(NAMESPACE); }
"new"					{ return add_token(NEW); }
"operator" 	            { return add_token(OPERATOR); }
"private"				{ return add_token(PRIVATE); }
"protected"				{ return add_token(PROTECTED); }
"public"				{ return add_token(PUBLIC); }
"register"              { return add_token(REGISTER); }
"reinterpret_cast"      { return add_token(REINTERPRET_CAST); }
"return"				{ return add_token(RETURN); }
"short"					{ return add_token(SHORT); }
"signed"				{ return add_token(SIGNED); }
"sizeof"				{ return add_token(SIZEOF); }
"struct"				{ return add_token(STRUCT); }
"static"                { return add_token(STATIC); }
"static_cast"           { return add_token(STATIC_CAST); }
"switch"				{ return add_token(SWITCH); }
"template"              { return add_token(TEMPLATE); }
"this"                  { return add_token(THIS); }
"throw"                 { return add_token(THROW); }
"true"					{ return add_token(TRUE); }
"try"                   { return add_token(TRY); }
"typedef"               { return add_token(TYPEDEF); }
"typeid"                { return add_token(TYPEID); }
"typename"              { return add_token(TYPENAME); }
"union"					{ return add_token(UNION); }
"unsigned"				{ return add_token(UNSIGNED); }
"using"                 { return add_token(USING); }
"virtual"               { return add_token(VIRTUAL); }
"void"					{ return add_token(VOID); }
"volatile"              { return add_token(VOLATILE); }
"wchar_t"               { return add_token(WCHAR_T); }
"while"					{ return add_token(WHILE); }

[a-zA-Z_][a-zA-Z_0-9]*			    { return add_token(check_identifier(yytext)); }

"0"[xX][0-9a-fA-F]+{intsuffix}?		{ return add_token(INTEGER); }
"0"[0-7]+{intsuffix}?			    { return add_token(INTEGER); }
[0-9]+{intsuffix}?			        { return add_token(INTEGER); }

{fracconst}{exppart}?{floatsuffix}?	{ return add_token(FLOATING); }
[0-9]+{exppart}{floatsuffix}?		{ return add_token(FLOATING); }

"'"{chartext}*"'"			    { return add_token(CHARACTER); }
"L'"{chartext}*"'"			    { return add_token(CHARACTER); }

"\""{stringtext}*"\""			{ return add_token(STRING); }
"L\""{stringtext}*"\""			{ return add_token(STRING); }

"#include <iostream>"			{ saw_iostream = 1; }
"#include <string>"			    { saw_string = 1; }
"using namespace std;"			{ saw_std = 1;
		if (saw_string) typenametable_insert("string", CLASS_NAME);
		if (saw_iostream) {
			typenametable_insert("ifstream", CLASS_NAME);
			typenametable_insert("ofstream", CLASS_NAME);
			}
		}


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
	
	yylval.t = create_tree(yytext, 0); 
	yylval.t->leaf = YYTOKEN; 
	
	return category; 
   
  	
}

//static int yywrap(void)
//{
	//return 1;
//}

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

/*
 * Type name table.
 * Replace this with a hash table, when you get a chance.
 */
struct typenametable_entry {
   char *name;
   int category;
   struct typenametable_entry *next;
   } *head;

int typenametable_lookup(char *s)
{
   struct typenametable_entry *tmp = head;
   while (tmp) {
      if (!strcmp(s, tmp->name)) return tmp->category;
      tmp = tmp->next;
   }
   return IDENTIFIER;
}

int typenametable_insert(char *s, int cat)
{
   struct typenametable_entry *tmp = head;
   while (tmp) {
      if (!strcmp(s, tmp->name)) {
         fprintf(stderr,
	    "warning, %s is already in the typename table as a %d\n",
	    s, tmp->category );
         return tmp->category;
         }
      tmp = tmp->next;
   }
   tmp = calloc(1, sizeof (struct typenametable_entry));
   if(!tmp) { fprintf(stderr, "tnti out of memory\n"); return -1; }
   tmp->name = strdup(s);
   tmp->category = cat;
   tmp->next = head;
   head = tmp;
}

void insert_typename_tree(struct tree *t, int category)
{
	typenametable_insert(((struct token *)t->leaf)->text, category); 
	printf("added class name %s to tree!!!\n", ((struct token *)t->leaf)->text); 
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
  push_file_node(&file_stack, fname);
  yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
   
}

// lexical error function
void lexerr(const char *s)
{
    filetext = file_stack->filename;
	fprintf (stderr, "File %s line %d, token %s: %s\n", filetext, yylineno, yytext, s);
    exit (1); 
}

static int check_identifier(char *s)
{
	return typenametable_lookup(s);
}


