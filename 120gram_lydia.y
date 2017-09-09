/*
 * Grammar for 120++, a subset of C++ used in CS 120 at University of Idaho
 *
 * Adaptation by Clinton Jeffery, with help from Matthew Brown, Ranger
 * Adams, and Shea Newton.
 *
 * Based on Sandro Sigala's transcription of the ISO C++ 1996 draft standard.
 * 
 */

/*	$Id: parser.y,v 1.3 1997/11/19 15:13:16 sandro Exp $	*/

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
 * ISO C++ parser.
 *
 * Based on the ISO C++ draft standard of December '96.
 */

%{
#include <stdio.h>
#include <string.h>
#include "nonterm.h"
#include "tree.h"

extern int lineno;
extern char *yytext;
int yydebug=0;

static void yyerror(char *s);


/*
 * warning, this %union is not what you need, it is for demonstration purposes.
 */
%}
%union {
  char *text;
  nodeptr n;
  }

%type < n > identifier literal integer_literal character_literal floating_literal 
%type < n > string_literal boolean_literal translation_unit
%type < n > primary_expression id_expression unqualified_id qualified_id nested_name_specifier 
%type < n > postfix_expression expression_list unary_expression unary_operator  
%type < n > new_expression new_placement new_type_id new_declarator direct_new_declarator
%type < n > new_initializer delete_expression pm_expression multiplicative_expression
%type < n > additive_expression shift_expression relational_expression equality_expression and_expression
%type < n > exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression
%type < n > conditional_expression assignment_expression assignment_operator expression constant_expression
%type < n > statement labeled_statement expression_statement compound_statement statement_seq selection_statement
%type < n > condition iteration_statement for_init_statement jump_statement declaration_statement
%type < n > declaration_seq declaration block_declaration simple_declaration 
%type < n > decl_specifier decl_specifier_seq type_specifier simple_type_specifier elaborated_type_specifier 
%type < n > init_declarator_list init_declarator declarator direct_declarator ptr_operator 
%type < n > declarator_id type_id type_specifier_seq abstract_declarator direct_abstract_declarator
%type < n > parameter_declaration_clause parameter_declaration_list parameter_declaration function_definition
%type < n > function_body initializer initializer_clause initializer_list class_specifier class_head
%type < n > class_key member_specification member_declaration member_declarator_list member_declarator 
%type < n > constant_initializer access_specifier ctor_initializer mem_initializer_list mem_initializer 
%type < n > mem_initializer_id declaration_seq_opt expression_list_opt new_placement_opt
%type < n > new_initializer_opt new_declarator_opt expression_opt statement_seq_opt condition_opt
%type < n > initializer_opt constant_expression_opt abstract_declarator_opt type_specifier_seq_opt
%type < n > direct_abstract_declarator_opt ctor_initializer_opt COMMA_opt member_specification_opt SEMICOLON_opt

%token < n >  IDENTIFIER INTEGER FLOATING CHARACTER STRING
%token < n >  TYPEDEF_NAME CLASS_NAME

%token < n >  ELLIPSIS COLONCOLON DOTSTAR ADDEQ SUBEQ MULEQ DIVEQ MODEQ
%token < n >  XOREQ ANDEQ OREQ SL SR SREQ SLEQ EQ NOTEQ LTEQ GTEQ ANDAND OROR
%token < n >  PLUSPLUS MINUSMINUS ARROWSTAR ARROW

%token < n >  BOOL BREAK CASE CATCH CHAR CLASS CONTINUE
%token < n >  DEFAULT DELETE DO DOUBLE  ELSE 
%token < n >  FALSE FLOAT FOR GOTO IF INT LONG NEW
%token < n >  PRIVATE PROTECTED PUBLIC  RETURN
%token < n >  SHORT SIGNED SIZEOF STATIC STRUCT SWITCH 
%token < n >  TRUE UNSIGNED VOID WHILE 
%token < n > ';' '{' '}' ',' ':' '=' '(' ')' '[' ']' '.' '&' '!' '~' '-'
%token < n > '+' '*' '/' '%' '<' '>' '^' '|' '?'

%start translation_unit

%%


/*----------------------------------------------------------------------
 * Lexical elements.
 *----------------------------------------------------------------------*/

identifier:
	IDENTIFIER { $$ = strdup(yytext); }
	;

literal:
	integer_literal {$$ = $1;}
	| character_literal {$$ = $1;}
	| floating_literal {$$ = $1;}
	| string_literal {$$ = $1;}
	| boolean_literal {$$ = $1;}
	;

integer_literal:
	INTEGER {$$ = $1;}
	;

character_literal:
	CHARACTER {$$ = $1;}
	;

floating_literal:
	FLOATING {$$ = $1;}
	;

string_literal:
	STRING {$$ = $1;}
	;

boolean_literal:
	TRUE {$$ = $1;}
	| FALSE {$$ = $1;}
	;

/*----------------------------------------------------------------------
 * Translation unit.
 *----------------------------------------------------------------------*/

translation_unit:
	declaration_seq_opt /* alctree */
	;

/*----------------------------------------------------------------------
 * Expressions.
 *----------------------------------------------------------------------*/

primary_expression:
	literal {$$ = $1;}
	| '(' expression ')' /* alctree */
	| id_expression {$$ = $1;}
	;

id_expression:
	unqualified_id {$$ = $1;}
	| qualified_id {$$ = $1;}
	;

unqualified_id:
	identifier {$$ = $1;}
	| '~' CLASS_NAME /* alctree */
	;

qualified_id:
	nested_name_specifier unqualified_id
	;

nested_name_specifier:
	CLASS_NAME COLONCOLON nested_name_specifier
	| CLASS_NAME COLONCOLON
	;

postfix_expression:
	primary_expression
	| postfix_expression '[' expression ']' /* alctree */
	| postfix_expression '(' expression_list_opt ')' /* alctree */
	| simple_type_specifier '(' expression_list_opt ')'
	| postfix_expression '.' COLONCOLON id_expression
	| postfix_expression '.' id_expression
	| postfix_expression ARROW COLONCOLON id_expression
	| postfix_expression ARROW id_expression
	| postfix_expression PLUSPLUS
	| postfix_expression MINUSMINUS
	;

expression_list:
	assignment_expression
	| expression_list ',' assignment_expression
	;

unary_expression:
	postfix_expression
	| PLUSPLUS unary_expression
	| MINUSMINUS unary_expression
	| '*' unary_expression
	| '&' unary_expression
	| unary_operator unary_expression
	| SIZEOF unary_expression
	| SIZEOF '(' type_id ')'
	| new_expression
	| delete_expression
	;

unary_operator:
	  '+'
	| '-'
	| '!'
	| '~'
	;

new_expression:
	  NEW new_placement_opt new_type_id new_initializer_opt
	| COLONCOLON NEW new_placement_opt new_type_id new_initializer_opt
	| NEW new_placement_opt '(' type_id ')' new_initializer_opt
	| COLONCOLON NEW new_placement_opt '(' type_id ')' new_initializer_opt
	;

new_placement:
	'(' expression_list ')'
	;

new_type_id:
	type_specifier_seq new_declarator_opt
	;

new_declarator:
	ptr_operator new_declarator_opt
	| direct_new_declarator
	;

direct_new_declarator:
	'[' expression ']'
	| direct_new_declarator '[' constant_expression ']'
	;

new_initializer:
	'(' expression_list_opt ')'
	;

delete_expression:
	  DELETE unary_expression
	| COLONCOLON DELETE unary_expression
	| DELETE '[' ']' unary_expression
	| COLONCOLON DELETE '[' ']' unary_expression
	;

pm_expression:
	unary_expression
	| pm_expression DOTSTAR unary_expression
	| pm_expression ARROWSTAR unary_expression
	;

multiplicative_expression:
	pm_expression
	| multiplicative_expression '*' pm_expression
	| multiplicative_expression '/' pm_expression
	| multiplicative_expression '%' pm_expression
	;

additive_expression:
	multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

shift_expression:
	additive_expression
	| shift_expression SL additive_expression
	| shift_expression SR additive_expression
	;

relational_expression:
	shift_expression
	| relational_expression '<' shift_expression
	| relational_expression '>' shift_expression
	| relational_expression LTEQ shift_expression
	| relational_expression GTEQ shift_expression
	;

equality_expression:
	relational_expression
	| equality_expression EQ relational_expression
	| equality_expression NOTEQ relational_expression
	;

and_expression:
	equality_expression
	| and_expression '&' equality_expression
	;

exclusive_or_expression:
	and_expression
	| exclusive_or_expression '^' and_expression
	;

inclusive_or_expression:
	exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression
	;

logical_and_expression:
	inclusive_or_expression
	| logical_and_expression ANDAND inclusive_or_expression
	;

logical_or_expression:
	logical_and_expression
	| logical_or_expression OROR logical_and_expression
	;

conditional_expression:
	logical_or_expression
	| logical_or_expression  '?' expression ':' assignment_expression
	;

assignment_expression:
	conditional_expression
	| logical_or_expression assignment_operator assignment_expression
	;

assignment_operator:
	'='
	| MULEQ
	| DIVEQ
	| MODEQ
	| ADDEQ
	| SUBEQ
	| SREQ
	| SLEQ
	| ANDEQ
	| XOREQ
	| OREQ
	;

expression:
	assignment_expression
	| expression ',' assignment_expression
	;

constant_expression:
	conditional_expression
	;

/*----------------------------------------------------------------------
 * Statements.
 *----------------------------------------------------------------------*/

statement:
	labeled_statement
	| expression_statement
	| compound_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	| declaration_statement
	;

labeled_statement:
	identifier ':' statement
	| CASE constant_expression ':' statement
	| DEFAULT ':' statement
	;

expression_statement:
	expression_opt ';'
	;

compound_statement:
	'{' statement_seq_opt '}'
	;

statement_seq:
	statement
	| statement_seq statement
	;

selection_statement:
	IF '(' condition ')' statement
	| IF '(' condition ')' statement ELSE statement
	| SWITCH '(' condition ')' statement
	;

condition:
	expression
	| type_specifier_seq declarator '=' assignment_expression
	;

iteration_statement:
	WHILE '(' condition ')' statement
	| DO statement WHILE '(' expression ')' ';'
	| FOR '(' for_init_statement condition_opt ';' expression_opt ')' statement
	;

for_init_statement:
	expression_statement
	| simple_declaration
	;

jump_statement:
	BREAK ';'
	| CONTINUE ';'
	| RETURN expression_opt ';'
	| GOTO identifier ';'
	;

declaration_statement:
	block_declaration
	;

/*----------------------------------------------------------------------
 * Declarations.
 *----------------------------------------------------------------------*/

declaration_seq:
	declaration
	| declaration_seq declaration
	;

declaration:
	block_declaration
	| function_definition
	;

block_declaration:
	simple_declaration
	;

simple_declaration:
	  decl_specifier_seq init_declarator_list ';'
	|  decl_specifier_seq ';'
	;

decl_specifier:
	| type_specifier

	;

decl_specifier_seq:
	  decl_specifier
	| decl_specifier_seq decl_specifier
	;


type_specifier:
	simple_type_specifier
	| class_specifier { $$ = NULL; }
	| elaborated_type_specifier { $$ = NULL; }

	;

simple_type_specifier:
	CLASS_NAME
	| nested_name_specifier CLASS_NAME { $$ = NULL; }
	| CHAR { $$ = NULL; }
	| BOOL { $$ = NULL; }
	| SHORT { $$ = NULL; }
	| INT { $$ = NULL; }
	| LONG { $$ = NULL; }
	| SIGNED { $$ = NULL; }
	| UNSIGNED { $$ = NULL; }
	| FLOAT { $$ = NULL; }
	| DOUBLE { $$ = NULL; }
	| VOID { $$ = NULL; }
	;


elaborated_type_specifier:
	  class_key COLONCOLON nested_name_specifier identifier
	| class_key COLONCOLON identifier
	
	;


/*----------------------------------------------------------------------
 * Declarators.
 *----------------------------------------------------------------------*/

init_declarator_list:
	init_declarator
	| init_declarator_list ',' init_declarator
	;

init_declarator:
	declarator initializer_opt
	;

declarator:
	direct_declarator
	| ptr_operator declarator
	;

direct_declarator:
	  declarator_id { $$ = NULL; }
	| direct_declarator '(' parameter_declaration_clause ')'   
	| direct_declarator '(' parameter_declaration_clause ')' 
	| direct_declarator '(' parameter_declaration_clause ')'  { $$ = NULL; }
	| direct_declarator '(' parameter_declaration_clause ')' { $$ = alctree(DIRECTDECLARATOR_5, 2, $1, $3); }
	| CLASS_NAME '(' parameter_declaration_clause ')' { $$ = NULL; }
	| CLASS_NAME COLONCOLON declarator_id '(' parameter_declaration_clause ')' { $$ = NULL; }
	| CLASS_NAME COLONCOLON CLASS_NAME '(' parameter_declaration_clause ')' { $$ = NULL; }
	| direct_declarator '[' constant_expression_opt ']' { $$ = NULL; }
	| '(' declarator ')' { $$ = NULL; }
	;

ptr_operator:
	'*' { $$ = NULL; }
	| '*'  { $$ = NULL; }
	| '&' { $$ = NULL; }
	| nested_name_specifier '*' { $$ = NULL; }
	| nested_name_specifier '*'  { $$ = NULL; }
	| COLONCOLON nested_name_specifier '*' { $$ = NULL; }
	| COLONCOLON nested_name_specifier '*'  { $$ = NULL; }
	;


declarator_id:
	  id_expression
	| COLONCOLON id_expression
	| COLONCOLON nested_name_specifier CLASS_NAME
	| COLONCOLON CLASS_NAME
	;

type_id:
	type_specifier_seq abstract_declarator_opt
	;

type_specifier_seq:
	type_specifier type_specifier_seq_opt
	;

abstract_declarator:
	ptr_operator abstract_declarator_opt
	| direct_abstract_declarator
	;

direct_abstract_declarator:
	  direct_abstract_declarator_opt '(' parameter_declaration_clause ')'  
	| direct_abstract_declarator_opt '(' parameter_declaration_clause ')' 
	| direct_abstract_declarator_opt '(' parameter_declaration_clause ')' 
	| direct_abstract_declarator_opt '(' parameter_declaration_clause ')'
	| direct_abstract_declarator_opt '[' constant_expression_opt ']'
	| '(' abstract_declarator ')'
	;

parameter_declaration_clause:
	  parameter_declaration_list ELLIPSIS { $$ = NULL; }
	| parameter_declaration_list
	| ELLIPSIS { $$ = NULL; }
	| { $$ = NULL; }
	| parameter_declaration_list ',' ELLIPSIS { $$ = NULL; }
	;

parameter_declaration_list:
	parameter_declaration
	| parameter_declaration_list ',' parameter_declaration
	;

parameter_declaration:
	decl_specifier_seq declarator
	| decl_specifier_seq declarator '=' assignment_expression
	| decl_specifier_seq abstract_declarator_opt
	| decl_specifier_seq abstract_declarator_opt '=' assignment_expression
	;

function_definition:
	  declarator ctor_initializer_opt function_body
	| decl_specifier_seq declarator ctor_initializer_opt function_body

	;

function_body:
	compound_statement
	;

initializer:
	'=' initializer_clause
	| '(' expression_list ')'
	;

initializer_clause:
	assignment_expression
	| '{' initializer_list COMMA_opt '}'
	| '{' '}'
	;

initializer_list:
	initializer_clause
	| initializer_list ',' initializer_clause
	;

/*----------------------------------------------------------------------
 * Classes.
 *----------------------------------------------------------------------*/

class_specifier:
	class_head '{' member_specification_opt '}'
	;

class_head:
	  class_key identifier { typenametable_insert($2, CLASS_NAME); }
	| class_key nested_name_specifier identifier
	;

class_key:
	CLASS
	| STRUCT
	;

member_specification:
	member_declaration member_specification_opt
	| access_specifier ':' member_specification_opt
	;

member_declaration:
	  decl_specifier_seq member_declarator_list ';'
	| decl_specifier_seq ';'
	| member_declarator_list ';'
	| ';'
	| function_definition SEMICOLON_opt
	| qualified_id ';'
	;

member_declarator_list:
	member_declarator
	| member_declarator_list ',' member_declarator
	;

member_declarator:
	  declarator
	| declarator constant_initializer
	| identifier ':' constant_expression
	;

/*
 * This rule need a hack for working around the ``= 0'' pure specifier.
 * 0 is returned as an ``INTEGER'' by the lexical analyzer but in this
 * context is different.
 */


constant_initializer:
	'=' constant_expression
	;

/*----------------------------------------------------------------------
 * Derived classes.
 *----------------------------------------------------------------------*/

access_specifier:
	PRIVATE
	| PROTECTED
	| PUBLIC
	;

/*----------------------------------------------------------------------
 * Special member functions.
 *----------------------------------------------------------------------*/


ctor_initializer:
	':' mem_initializer_list
	;

mem_initializer_list:
	mem_initializer
	| mem_initializer ',' mem_initializer_list
	;

mem_initializer:
	mem_initializer_id '(' expression_list_opt ')'
	;

mem_initializer_id:
	  COLONCOLON nested_name_specifier class_name
	| COLONCOLON class_name
	| nested_name_specifier class_name
	| class_name
	| identifier
	;


/*----------------------------------------------------------------------
 * Epsilon (optional) definitions.
 *----------------------------------------------------------------------*/

declaration_seq_opt:
	/* epsilon */
	| declaration_seq
	;

expression_list_opt:
	/* epsilon */
	| expression_list
	;


new_placement_opt:
	/* epsilon */
	| new_placement
	;

new_initializer_opt:
	/* epsilon */
	| new_initializer
	;

new_declarator_opt:
	/* epsilon */
	| new_declarator
	;

expression_opt:
	/* epsilon */
	| expression
	;

statement_seq_opt:
	/* epsilon */
	| statement_seq
	;

condition_opt:
	/* epsilon */
	| condition
	;


initializer_opt:
	/* epsilon */
	| initializer
	;

constant_expression_opt:
	/* epsilon */
	| constant_expression
	;

abstract_declarator_opt:
	/* epsilon */
	| abstract_declarator
	;

type_specifier_seq_opt:
	/* epsilon */
	| type_specifier_seq
	;

direct_abstract_declarator_opt:
	/* epsilon */
	| direct_abstract_declarator
	;

ctor_initializer_opt:
	/* epsilon */
	| ctor_initializer
	;

COMMA_opt:
	/* epsilon */
	| ','
	;

member_specification_opt:
	/* epsilon */
	| member_specification
	;

SEMICOLON_opt:
	/* epsilon */
	| ';'
	;

%%

static void
yyerror(char *s)
{
	fprintf(stderr, "%d: %s\n", lineno, s);
}
