/*
 * Lydia Engerbretson
 * Fall 2017
 *
 */

%option yylineno
%x INCLUDE

O        [0-7]
D        [0-9]
L        [a-zA-Z_]
H        [a-fA-F0-9]
E        [Ee][+-]?{D}+
FS       (f|F|l|L)
IS       (u|U|l|L)
W        [ \t\f]*
LIT      \"(\\.|[^\\"])*\"

%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "token.h"
#include "ytab.h"

extern int yychar;
FILE *saved_yyin;
extern struct tokenlist *YYTOKENLIST; 
extern struct token *YYTOKEN; 
extern char* curr_filename; 
extern char* prev_filename; 

void add_token(int category); // adapted from: https://github.com/andschwa/partial-cpp-compiler
void lexerr(char *s);
void handle_include(char *s);

/* #define DEBUG */

int errors;
int included_iostream = 0;

%}

%%

"\n"					{ yylineno++; }

[ \r\t\v\f\n]*          { /* eat whitespace */ }

"//".*$                 { /* eat comment */}

"/*"([^*]|"*"+[^/*])*"*"+"/"  { /* eat comment */} // adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/lecture.html#5

"#include"              {BEGIN(INCLUDE);}
<INCLUDE>{
"<iostream>"            { BEGIN(INITIAL); }
"<fstream>"             { BEGIN(INITIAL); }
"<cstdlib>"             { BEGIN(INITIAL); }
"<string>"              { BEGIN(INITIAL); }
"<ctime>" 				{ BEGIN(INITIAL); }
"<cmath>"               { BEGIN(INITIAL); }
\"[^\"]+\"              { handle_include(yytext); BEGIN(INITIAL); }
}

"bool"
"break"                 { add_token(BREAK); return BREAK; }
"case"                  { add_token(CASE); return CASE; }
"char"                  { add_token(CHAR); return CHAR; }
"class"                 { /*return CLASS;*/ } // just added
"const"                 { add_token(CONST); return CONST; }
"continue"              { add_token(CONTINUE); return CONTINUE; }
"default"               { add_token(DEFAULT); return DEFAULT; }
"delete"                { /*return DELETE;*/ } // just added
"do"                    { add_token(DO); return DO; }
"double"                { add_token(DOUBLE); return DOUBLE; }
"else"                  { add_token(ELSE); return ELSE; }
"enum"                  { add_token(ENUM); return ENUM; } 
"extern"                { add_token(EXTERN); return EXTERN; } 
"float"                 { add_token(FLOAT); return FLOAT; }
"for"                   { add_token(FOR); return FOR; }
"goto"                  { add_token(GOTO); return GOTO; } 
"if"                    { add_token(IF); return IF; }
"int"                   { add_token(INT); return INT; } 
"long"                  { add_token(LONG); return LONG; }
"new"                   { /*return NEW;*/ } // just added 
"private"               { /*return PRIVATE;*/ } // just added
"protected"             { /*return PROTECTED;*/ } // just added
"public"                 { /*return PUBLIC;*/ } // just added
"register"              { add_token(REGISTER); return REGISTER; } 
"return"                { add_token(RETURN); return RETURN; }
"short"                 { add_token(SHORT); return SHORT; }
"signed"                { add_token(SIGNED); return SIGNED; }
"sizeof"                { add_token(SIZEOF); return SIZEOF; }
"static"                { add_token(STATIC); return STATIC; } 
"struct"                { add_token(STRUCT); return STRUCT; }
"switch"                { add_token(SWITCH); return SWITCH; }
"typedef"               { add_token(TYPEDEF); return TYPEDEF; } 
"union"                 { add_token(UNION); return UNION; } 
"true"                  { /*return TRUE;*/ } // just added 
"unsigned"              { add_token(UNSIGNED); return UNSIGNED; }
"void"                  { add_token(VOID); return VOID; }
"volatile"              { add_token(VOLATILE); return VOLATILE; } 
"while"                 { add_token(WHILE); return WHILE; }

{L}({L}|{D})*           { add_token(IDENTIFIER); return IDENTIFIER; }


0[xX]{H}+{IS}?          { lexerr("Hex not supported\n"); }

0{O}+{IS}?              { lexerr("Octal not supported\n"); }

{D}+{IS}?               { add_token(ICON); return ICON; }

'(\\.|[^\\'])+'         { add_token(CCON); return CCON; }

{D}+{E}{FS}?            { add_token(FCON); return FCON; }
{D}*"."{D}+({E})?{FS}?  { add_token(FCON); return FCON; }
{D}+"."{D}*({E})?{FS}?  { add_token(FCON); return FCON; }

{LIT}                   { add_token(STRING); return STRING; }


">>="                   { return SRASN; }
"<<="                   { return SLASN; }
"+="                    { return PLASN; }
"-="                    { return MIASN; }
"*="                    { return MUASN; }
"/="                    { return DIASN; }
"%="                    { return MOASN; }
"&="                    { return ANASN; }
"^="                    { return ERASN; }
"|="                    { return ORASN; }
">>"                    { return SHR; }
"<<"                    { return SHL; }
"++"                    { return INCOP; }
"--"                    { return DECOP; }
"->"                    { return FOLLOW; }
"->*"                   { /* return */} // just added
"&&"                    { add_token(ANDAND); return ANDAND; }
"||"                    { return OROR; }
"<="                    { return LE; }
">="                    { return GE; }
"=="                    { add_token(EQ); return EQ; }
"!="                    { add_token(NE); return NE; }
";"                     { return SM; }
"{"                     { return LC; }
"}"                     { return RC; }
","                     { return CM; }
":"                     { return COLON; }
"="                     { return ASN; }
"("                     { return LP; }
")"                     { return RP; }
"["                     { return LB; }
"]"                     { return RB; }
"."                     { return DOT; }
"&"                     { return AND; }
"!"                     { return BANG; }
"~"                     { return NOT; }
"-"                     { return MINUS; }
"+"                     { return PLUS; }
"*"                     { return MUL; }
"/"                     { return DIV; }
"%"                     { return MOD; }
"<"                     { return LT; }
">"                     { return GT; }
"^"                     { return ER; }
"|"                     { return OR; }
"?"                     { return QUEST; }

%%

void lexerr(char *s)
{
	errors++;

	fprintf(stderr, "%s: lexical error", s);

	/* to do: add mechanism for reporting file name and line number */

	fprintf(stderr, ", token = \"%s\"\n", yytext);
}

void add_token(int category)
{
   //struct token * newtoken = NULL;
   
	YYTOKEN = create_token(category, yytext, yylineno, curr_filename);  
	add_token_to_list(YYTOKEN);  	
}
/*
 * Return 1 if done, 0 if yyin points at more input
 */
int yywrap()
{
   return 1;
}

void handle_include(char *s)
{

   char *fname = strchr(s, '\"')+1;
   fname[strlen(fname)-1] = '\0';
   fprintf(stdout, "included filename '%s'\n", fname); fflush(stdout);
   strcpy(curr_filename, fname); 
   yyin = fopen(fname,"r");
   if (yyin == NULL) 
   {
     lexerr("cannot open include file");
     exit(1);
   }

   // curr_filename = strdup(s); // this does not do anything
	
   //yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
   //yypop_buffer_state(); 
   
}
