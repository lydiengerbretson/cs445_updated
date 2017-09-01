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
extern filenodeptr file_stack;
extern char* file_name;  
extern int include; 

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
"<cstring>"             { BEGIN(INITIAL); }
"<string>"              { BEGIN(INITIAL); }
"<ctime>" 				{ BEGIN(INITIAL); }
"<cmath>"               { BEGIN(INITIAL); }
"<iomanip>"             { BEGIN(INITIAL); }
\"[^\"]+\"              { handle_include(yytext); BEGIN(INITIAL); }
}

"auto"                  { add_token(AUTO); return AUTO; }
"bool"                  { add_token(BOOL); return BOOL; }
"break"                 { add_token(BREAK); return BREAK; }
"case"                  { add_token(CASE); return CASE; }
"char"                  { add_token(CHAR); return CHAR; }
"class"                 { add_token(CLASS); return CLASS; } //  added
"const"                 { add_token(CONST); return CONST; }
"continue"              { add_token(CONTINUE); return CONTINUE; }
"default"               { add_token(DEFAULT); return DEFAULT; }
"delete"                { add_token(DELETE); return DELETE; } //  added
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
"new"                   { add_token(NEW); return NEW; } //  added 
"private"               { add_token(PRIVATE); return PRIVATE; } //  added
"protected"             { add_token(PROTECTED); return PROTECTED; } //  added
"public"                { add_token(PUBLIC); return PUBLIC; } //  added
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
"this"                  { add_token(THIS); return THIS; } 
"true"                  { add_token(TRUE); return TRUE; } //  added 
"unsigned"              { add_token(UNSIGNED); return UNSIGNED; }
"void"                  { add_token(VOID); return VOID; }
"volatile"              { add_token(VOLATILE); return VOLATILE; } 
"while"                 { add_token(WHILE); return WHILE; }

{L}({L}|{D})*           { add_token(IDENTIFIER); return IDENTIFIER; }


0[xX]{H}+{IS}?          { lexerr("Hex not supported\n"); }
0{O}+{IS}?              { lexerr("Octal not supported\n"); }

{D}+{IS}?               { add_token(ICON); return ICON; }
'(\\.|[^\\'])+'         { add_token(CCON); return CCON;  }
{D}+{E}{FS}?            { add_token(FCON); return FCON;  }
{D}*"."{D}+({E})?{FS}?  { add_token(FCON); return FCON;  }
{D}+"."{D}*({E})?{FS}?  { add_token(FCON); return FCON;  }
{LIT}                   { add_token(STRING); return STRING; }


">>="                   { add_token(SRASN); return SRASN;  }
"<<="                   { add_token(SLASN); return SLASN; }
"+="                    { add_token(PLASN); return PLASN;  }
"-="                    { add_token(MIASN); return MIASN; }
"*="                    { add_token(MUASN); return MUASN;  }
"/="                    { add_token(DIASN); return DIASN;  }
"%="                    { add_token(MOASN); return MOASN; }
"&="                    { add_token(ANASN); return ANASN; }
"^="                    { add_token(ERASN); return ERASN; }
"|="                    { add_token(ORASN); return ORASN;  }
">>"                    { add_token(SHR); return SHR;  }
"<<"                    { add_token(SHL); return SHL; }
"++"                    { add_token(INCOP); return INCOP; }
"--"                    { add_token(DECOP); return DECOP; }
"->"                    { add_token(FOLLOW); return FOLLOW; }
"->*"                   { add_token(FOLLOWPTR); return FOLLOWPTR;} // just added
"&&"                    { add_token(ANDAND); return ANDAND;  }
"||"                    { add_token(OROR); return OROR; }
"<="                    { add_token(LE); return LE; }
">="                    { add_token(GE); return GE; }
"=="                    { add_token(EQ); return EQ; }
"!="                    { add_token(NE); return NE; }
";"                     { add_token(SM); return SM; }
"{"                     { add_token(LC); return LC; }
"}"                     { add_token(RC); return RC; }
","                     { add_token(CM); return CM; }
":"                     { add_token(COLON); return COLON; }
"="                     { add_token(ASN); return ASN; }
"("                     { add_token(LP); return LP; }
")"                     { add_token(RP); return RP; }
"["                     { add_token(LB); return LB; }
"]"                     { add_token(RB); return RB; }
"."                     { add_token(DOT); return DOT; }
"&"                     { add_token(AND); return AND; }
"!"                     { add_token(BANG); return BANG; }
"~"                     { add_token(NOT); return NOT; }
"-"                     { add_token(MINUS); return MINUS; }
"+"                     { add_token(PLUS); return PLUS; }
"*"                     { add_token(MUL); return MUL; }
"/"                     { add_token(DIV); return DIV; }
"%"                     { add_token(MOD); return MOD; }
"<"                     { add_token(LT); return LT; }
">"                     { add_token(GT); return GT; }
"^"                     { add_token(ER); return ER; }
"|"                     { add_token(OR); return OR; }
"?"                     { add_token(QUEST); return QUEST; }
.                       { lexerr(yytext); return; }


%%

void lexerr(char *s)
{
	errors++;

	fprintf(stderr, "%s: lexical error", s);

	/* to do: add mechanism for reporting file name and line number */
   
	fprintf(stderr, ", token = \"%s\"\n", yytext);
	fprintf(stderr, ", filename = \"%s\"\n", curr_filename);
	fprintf(stderr, ", line number = \"%d\"\n", yylineno);
}

void add_token(int category)
{
   // global YYTOKEN
   	YYTOKEN = create_token(category, yytext, yylineno, curr_filename);  // used to be curr_filename
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
   char *fname = malloc(strlen(s)+1-strlen("#include \"\""));
   fname = strchr(s, '\"')+1;
   fname[strlen(fname)-1] = '\0';
   fprintf(stdout, "included filename '%s'\n", fname); fflush(stdout);
   
   strcpy(curr_filename, fname);  
   yyin = fopen(fname,"r");
   // push file onto stack 
   //push_file_node(&file_stack, fname); 
   //yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE)); 
   if (yyin == NULL) 
   {
     lexerr("cannot open include file");
     exit(1);
   }
   
}
