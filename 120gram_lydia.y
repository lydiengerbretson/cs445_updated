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
#include "token.h"
#include "tree_lydia.h"
#include "nonterms.h"

extern int yylineno;
extern char *yytext;

// tree pointer to the start of the program
Treeptr YYPROGRAM; 
int yydebug=0;

static void yyerror(char *s);
void insert_typename_tree(struct tree *t, int category);



/*
 * warning, this %union is not what you need, it is for demonstration purposes.
 */
%}

%union {
  struct tree *t;
}


%token <t> IDENTIFIER INTEGER FLOATING CHARACTER STRING
%token <t> TYPEDEF_NAME CLASS_NAME ENUM_NAME NAMESPACE_NAME TEMPLATE_NAME

%token <t> ELLIPSIS COLONCOLON DOTSTAR ADDEQ SUBEQ MULEQ DIVEQ MODEQ
%token <t> XOREQ ANDEQ OREQ SL SR SREQ SLEQ EQ NOTEQ LTEQ GTEQ ANDAND OROR
%token <t> PLUSPLUS MINUSMINUS ARROWSTAR ARROW

%token <t> ASM AUTO BOOL BREAK CASE CATCH CHAR CLASS CONST CONST_CAST CONTINUE
%token <t> DEFAULT DELETE DO DOUBLE DYNAMIC_CAST ELSE ENUM EXPLICIT EXPORT EXTERN
%token <t> FALSE FLOAT FOR FRIEND GOTO IF INLINE INT LONG MUTABLE NAMESPACE NEW
%token <t> OPERATOR PRIVATE PROTECTED PUBLIC REGISTER REINTERPRET_CAST RETURN
%token <t> SHORT SIGNED SIZEOF STATIC STATIC_CAST STRUCT SWITCH TEMPLATE THIS
%token <t> THROW TRUE TRY TYPEDEF TYPEID TYPENAME UNION UNSIGNED USING VIRTUAL
%token <t> VOID VOLATILE WCHAR_T WHILE

%token <t> ';' '{' '}' ',' ':' '=' '(' ')' '[' ']' '.' '&' '!' '~' '-'
%token <t> '+' '*' '/' '%' '<' '>' '^' '|' '?' '_'

%type <t> identifier literal 
%type <t> boolean_literal
%type <t> translation_unit primary_expression id_expression unqualified_id
%type <t> qualified_id nested_name_specifier postfix_expression expression_list
%type <t> unary_expression unary_operator new_expression new_placement new_type_id
%type <t> new_declarator direct_new_declarator new_initializer delete_expression
%type <t> pm_expression multiplicative_expression additive_expression
%type <t> shift_expression relational_expression equality_expression and_expression
%type <t> exclusive_or_expression inclusive_or_expression logical_and_expression 
%type <t> logical_or_expression conditional_expression assignment_expression
%type <t> assignment_operator expression constant_expression statement
%type <t> labeled_statement expression_statement compound_statement statement_seq
%type <t> selection_statement condition iteration_statement for_init_statement 
%type <t> jump_statement declaration_statement declaration_seq declaration
%type <t> block_declaration simple_declaration decl_specifier decl_specifier_seq
%type <t> storage_class_specifier function_specifier type_specifier simple_type_specifier
%type <t> elaborated_type_specifier 
%type <t> init_declarator_list init_declarator declarator 
%type <t> direct_declarator ptr_operator cv_qualifier_seq cv_qualifier declarator_id
%type <t> type_id type_specifier_seq abstract_declarator direct_abstract_declarator 
%type <t> parameter_declaration_clause parameter_declaration_list parameter_declaration
%type <t> function_definition function_body initializer initializer_clause initializer_list
%type <t> class_specifier class_head class_key member_specification member_declaration
%type <t> member_declarator_list member_declarator pure_specifier constant_initializer
%type <t> ctor_initializer
%type <t> mem_initializer_list mem_initializer mem_initializer_id   
%type <t> declaration_seq_opt expression_list_opt
%type <t> new_placement_opt new_initializer_opt new_declarator_opt
%type <t> expression_opt statement_seq_opt condition_opt
%type <t> initializer_opt constant_expression_opt abstract_declarator_opt type_specifier_seq_opt
%type <t> ctor_initializer_opt COMMA_opt member_specification_opt
%type <t> SEMICOLON_opt access_specifier


%start translation_unit

%%

/*----------------------------------------------------------------------
 * Context-dependent identifiers.
 *----------------------------------------------------------------------*/


/*----------------------------------------------------------------------
 * Lexical elements.
 *----------------------------------------------------------------------*/

identifier:
	IDENTIFIER { $$ = $1; }
	;

literal:
	INTEGER {$$ = $1;}
	| CHARACTER {$$ = $1;}
	| FLOATING {$$ = $1;}
	| STRING {$$ = $1;}
	| boolean_literal {$$ = $1;}
	;

boolean_literal:
	TRUE {$$ = $1;}
	| FALSE {$$ = $1;}
	;

/*----------------------------------------------------------------------
 * Translation unit.
 *----------------------------------------------------------------------*/

translation_unit:
	declaration_seq_opt {$$ = create_tree("program", DECLARATION_SEQ_OPT_1, 1, $1); YYPROGRAM = $$; }
	;

/*----------------------------------------------------------------------
 * Expressions.
 *----------------------------------------------------------------------*/

primary_expression:
	literal {$$ = $1;}
	| '(' expression ')' { $$ = create_tree("primary_expression", PRIMARY_EXPRESSION_1, 3, $1, $2, $3); }
	| id_expression {$$ = $1;}
	;

id_expression:
	unqualified_id {$$ = $1;}
	| qualified_id {$$ = $1;}
	;

unqualified_id:
	identifier {$$ = $1;}
	| '~' CLASS_NAME {$$ = create_tree("unqualified_id", UNQUALIFIED_ID_1, 2, $1, $2);}
	;

qualified_id:
	nested_name_specifier unqualified_id { $$ = create_tree("qualified_id_1", QUALIFIED_ID_1, 2, $1, $2);}

nested_name_specifier:
	CLASS_NAME COLONCOLON nested_name_specifier {$$ = create_tree("nested_name_specifier_1", NESTED_NAME_SPECIFIER_1, 3, $1, $2, $3);}
	| CLASS_NAME COLONCOLON {$$ = create_tree("nested_name_specifier_3", NESTED_NAME_SPECIFIER_1, 2, $1, $2);}

	;

postfix_expression:
	primary_expression {$$ = $1;}
	| postfix_expression '[' expression ']' {$$ = create_tree("postfix_expression_1", POSTFIX_EXPRESSION_1, 4, $1, $2, $3, $4);}
	| postfix_expression '(' expression_list_opt ')' {$$ = create_tree("postfix_expression_2", POSTFIX_EXPRESSION_1, 4, $1, $2, $3, $4);}
	| postfix_expression '.' COLONCOLON id_expression {$$ = create_tree("postfix_expression_3", POSTFIX_EXPRESSION_1, 4, $1, $2, $3, $4);}
	| postfix_expression '.' id_expression {$$ = create_tree("postfix_expression_4", POSTFIX_EXPRESSION_1, 3, $1, $2, $3);}
	| postfix_expression ARROW COLONCOLON id_expression {$$ = create_tree("postfix_expression_5", POSTFIX_EXPRESSION_1,  4, $1, $2, $3, $4);}
	| postfix_expression ARROW id_expression {$$ = create_tree("postfix_expression_6", POSTFIX_EXPRESSION_1, 3, $1, $2, $3);}
	| postfix_expression PLUSPLUS {$$ = create_tree("postfix_expression_7", POSTFIX_EXPRESSION_1, 2, $1, $2);}
	| postfix_expression MINUSMINUS {$$ = create_tree("postfix_expression_8", POSTFIX_EXPRESSION_1, 2, $1, $2);}
	;

expression_list:
	assignment_expression { $$ = $1; }
	| expression_list ',' assignment_expression {$$ = create_tree("expression_list_1", EXPRESSION_LIST_1, 3, $1, $2, $3);}
	;

unary_expression:
	postfix_expression { $$ = $1; } 
	| PLUSPLUS unary_expression {$$ = create_tree("unary_expression_1", UNARY_EXPRESSION_1, 2, $1, $2);}
	| MINUSMINUS unary_expression {$$ = create_tree("unary_expression_2", UNARY_EXPRESSION_1, 2, $1, $2);}
	| '*' unary_expression {$$ = create_tree("unary_expression_3", UNARY_EXPRESSION_1, 2, $1, $2);}
	| '&' unary_expression {$$ = create_tree("unary_expression_4", UNARY_EXPRESSION_1, 2, $1, $2);}
	| unary_operator unary_expression {$$ = create_tree("unary_expression_5", UNARY_EXPRESSION_1, 2, $1, $2);}
	| SIZEOF unary_expression {$$ = create_tree("unary_expression_6", UNARY_EXPRESSION_1, 2, $1, $2);}
	| SIZEOF '(' type_id ')' {$$ = create_tree("unary_expression_7", UNARY_EXPRESSION_1, 4, $1, $2, $3, $4);}
	| new_expression { $$ = $1; } 
	| delete_expression { $$ = $1; } 
	;

unary_operator:
	  '+' { $$ = $1; }
	| '-' { $$ = $1; }
	| '!' { $$ = $1; }
	| '~' { $$ = $1; }
	;

new_expression:
	  NEW new_placement_opt new_type_id new_initializer_opt {$$ = create_tree("new_expression_1", NEW_EXPRESSION_1, 4, $1, $2, $3, $4);}
	| COLONCOLON NEW new_placement_opt new_type_id new_initializer_opt {$$ = create_tree("new_expression_2", NEW_EXPRESSION_1, 5, $1, $2, $3, $4, $5);}
	;

new_placement:
	'(' expression_list ')' {$$ = create_tree("new_placement_1", NEW_PLACEMENT_1, 3, $1, $2, $3);}
	;

new_type_id:
	type_specifier_seq new_declarator_opt {$$ = create_tree("new_type_id", NEW_TYPE_ID_1, 2, $1, $2);}
	;

new_declarator:
	ptr_operator new_declarator_opt {$$ = create_tree("new_declarator_1", NEW_DECLARATOR_1, 2, $1, $2);}
	| direct_new_declarator { $$ = $1 ;}
	;

direct_new_declarator:
	'[' expression ']' {$$ = create_tree("direct_new_declarator_1", DIRECT_NEW_DECLARATOR_1, 3, $1, $2, $3);}
	| direct_new_declarator '[' constant_expression ']'{$$ = create_tree("direct_new_declarator_2", DIRECT_NEW_DECLARATOR_1, 4, $1, $2, $3, $4);}
	;

new_initializer:
	'(' expression_list_opt ')' {$$ = create_tree("new_initializer_1", NEW_INITIALIZER_1, 3, $1, $2, $3);}
	;

delete_expression:
	  DELETE unary_expression {$$ = create_tree("delete_expression_1", DELETE_EXPRESSION_1, 2, $1, $2);}
	| COLONCOLON DELETE unary_expression {$$ = create_tree("delete_expression_2", DELETE_EXPRESSION_1, 3, $1, $2, $3);}
	| DELETE '[' ']' unary_expression {$$ = create_tree("delete_expression_3", DELETE_EXPRESSION_1, 4, $1, $2, $3, $4);}
	| COLONCOLON DELETE '[' ']' unary_expression {$$ = create_tree("delete_expression_4", DELETE_EXPRESSION_1, 5, $1, $2, $3, $4, $5);}
	;

pm_expression:
	unary_expression { $$ = $1 ;}
	| pm_expression DOTSTAR unary_expression {$$ = create_tree("pm_expression_1", PM_EXPRESSION_1, 3, $1, $2, $3);}
	| pm_expression ARROWSTAR unary_expression {$$ = create_tree("pm_expression_2", PM_EXPRESSION_1, 3, $1, $2, $3);}
	;

multiplicative_expression:
	pm_expression { $$ = $1 ;}
	| multiplicative_expression '*' pm_expression {$$ = create_tree("mult_expression_1", MULTIPLICATIVE_EXPRESSION_1, 3, $1, $2, $3);}
	| multiplicative_expression '/' pm_expression {$$ = create_tree("mult_expression_2", MULTIPLICATIVE_EXPRESSION_1, 3, $1, $2, $3);}
	| multiplicative_expression '%' pm_expression {$$ = create_tree("mult_expression_3", MULTIPLICATIVE_EXPRESSION_1, 3, $1, $2, $3);}
	;

additive_expression:
	multiplicative_expression { $$ = $1 ;}
	| additive_expression '+' multiplicative_expression {$$ = create_tree("add_expression_1", ADDITIVE_EXPRESSION_1, 3, $1, $2, $3);}
	| additive_expression '-' multiplicative_expression {$$ = create_tree("add_expression_2", ADDITIVE_EXPRESSION_1, 3, $1, $2, $3);}
	;

shift_expression:
	additive_expression { $$ = $1 ;}
	| shift_expression SL additive_expression {$$ = create_tree("shift_expression_1", SHIFT_EXPRESSION_1, 3, $1, $2, $3);}
	| shift_expression SR additive_expression {$$ = create_tree("shift_expression_2", SHIFT_EXPRESSION_1, 3, $1, $2, $3);}
	;

relational_expression:
	shift_expression { $$ = $1 ;}
	| relational_expression '<' shift_expression {$$ = create_tree("relational_expression_1", RELATIONAL_EXPRESSION_1, 3, $1, $2, $3);}
	| relational_expression '>' shift_expression {$$ = create_tree("relational_expression_2", RELATIONAL_EXPRESSION_1, 3, $1, $2, $3);}
	| relational_expression LTEQ shift_expression {$$ = create_tree("relational_expression_3", RELATIONAL_EXPRESSION_1, 3, $1, $2, $3);}
	| relational_expression GTEQ shift_expression {$$ = create_tree("relational_expression_4", RELATIONAL_EXPRESSION_1, 3, $1, $2, $3);}
	;

equality_expression:
	relational_expression { $$ = $1 ;}
	| equality_expression EQ relational_expression {$$ = create_tree("equality_expression_1", EQUALITY_EXPRESSION_1, 3, $1, $2, $3);}
	| equality_expression NOTEQ relational_expression {$$ = create_tree("equality_expression_2", EQUALITY_EXPRESSION_1, 3, $1, $2, $3);}
	;

and_expression:
	equality_expression { $$ = $1 ;}
	| and_expression '&' equality_expression {$$ = create_tree("and_expression_1", AND_EXPRESSION_1, 3, $1, $2, $3);}
	;

exclusive_or_expression:
	and_expression { $$ = $1; }
	| exclusive_or_expression '^' and_expression {$$ = create_tree("excl_or_expression_1", EXCLUSIVE_OR_EXPRESSION_1, 3, $1, $2, $3);}
	;

inclusive_or_expression:
	exclusive_or_expression { $$ = $1; }
	| inclusive_or_expression '|' exclusive_or_expression {$$ = create_tree("incl_or_expression_1", INCLUSIVE_OR_EXPRESSION_1, 3, $1, $2, $3);}
	;

logical_and_expression:
	inclusive_or_expression { $$ = $1; }
	| logical_and_expression ANDAND inclusive_or_expression {$$ = create_tree("logical_and_expression_1", LOGICAL_AND_EXPRESSION_1, 3, $1, $2, $3);}
	;

logical_or_expression:
	logical_and_expression { $$ = $1; }
	| logical_or_expression OROR logical_and_expression {$$ = create_tree("logical_or_expression_1", LOGICAL_OR_EXPRESSION_1, 3, $1, $2, $3);}
	;

conditional_expression:
	logical_or_expression { $$ = $1; }
	| logical_or_expression  '?' expression ':' assignment_expression {$$ = create_tree("conditional_expression_1", CONDITIONAL_EXPRESSION_1, 5, $1, $2, $3, $4, $5);}
	;

assignment_expression:
	conditional_expression { $$ = $1; }
	| logical_or_expression assignment_operator assignment_expression {$$ = create_tree("assignment_expression_1", ASSIGNMENT_EXPRESSION_1, 3, $1, $2, $3);} 
	;

assignment_operator:
	'=' { $$ = $1; }
	| MULEQ { $$ = $1; }
	| DIVEQ { $$ = $1; }
	| MODEQ { $$ = $1; }
	| ADDEQ { $$ = $1; }
	| SUBEQ { $$ = $1; }
	| SREQ { $$ = $1; }
	| SLEQ { $$ = $1; }
	| ANDEQ { $$ = $1; }
	| XOREQ { $$ = $1; }
	| OREQ { $$ = $1; }
	;

expression:
	assignment_expression { $$ = $1; }
	| expression ',' assignment_expression {$$ = create_tree("expression_1", EXPRESSION_1, 3, $1, $2, $3);} 
	;

constant_expression:
	conditional_expression { $$ = $1; }
	;

/*----------------------------------------------------------------------
 * Statements.
 *----------------------------------------------------------------------*/

statement:
	labeled_statement { $$ = $1; }
	| expression_statement { $$ = $1; }
	| compound_statement { $$ = $1; }
	| selection_statement { $$ = $1; }
	| iteration_statement { $$ = $1; }
	| jump_statement { $$ = $1; }
	| declaration_statement { $$ = $1; }
	;

labeled_statement:
	identifier ':' statement {$$ = create_tree("labeled_statement_1", LABELED_STATEMENT_1, 3, $1, $2, $3);} 
	| CASE constant_expression ':' statement {$$ = create_tree("labeled_statement_2", LABELED_STATEMENT_1, 4, $1, $2, $3, $4);} 
	| DEFAULT ':' statement {$$ = create_tree("labeled_statement_3", LABELED_STATEMENT_1, 3, $1, $2, $3);} 
	;

expression_statement:
	expression_opt ';' {$$ = create_tree("expression_statement_1", EXPRESSION_STATEMENT_1, 2, $1, $2);} 
	;

compound_statement:
	'{' statement_seq_opt '}' {$$ = create_tree("compound_statement_1", COMPOUND_STATEMENT_1, 3, $1, $2, $3);} 
	;

statement_seq:
	statement { $$ = $1; }
	| statement_seq statement {$$ = create_tree("statement_seq_1", STATEMENT_SEQ_1, 2, $1, $2);} 
	;

selection_statement:
	IF '(' condition ')' statement {$$ = create_tree("selection_statement_1", SELECTION_STATEMENT_1, 5, $1, $2, $3, $4, $5);} 
	| IF '(' condition ')' statement ELSE statement { $$ = create_tree("selection_statement_2", SELECTION_STATEMENT_1, 7, $1, $2, $3, $4, $5, $6, $7);} 
	| SWITCH '(' condition ')' statement {$$ = create_tree("selection_statement_3", SELECTION_STATEMENT_1, 5, $1, $2, $3, $4, $5);} 
	;

condition:
	expression { $$ = $1; }
	| type_specifier_seq declarator '=' assignment_expression {$$ = create_tree("condition_1", CONDITION_1, 4, $1, $2, $3, $4);} 
	;

iteration_statement:
	WHILE '(' condition ')' statement {$$ = create_tree("iteration_statement_1", ITERATION_STATEMENT_1, 5, $1, $2, $3, $4, $5);} 
	| DO statement WHILE '(' expression ')' ';' {$$ = create_tree("iteration_statement_2", ITERATION_STATEMENT_1, 7, $1, $2, $3, $4, $5, $6, $7);} 
	| FOR '(' for_init_statement condition_opt ';' expression_opt ')' statement {$$ = create_tree("iteration_statement_3", ITERATION_STATEMENT_1, 8, $1, $2, $3, $4, $5, $6, $7, $8);} 
	;

for_init_statement:
	expression_statement { $$ = $1; }
	| simple_declaration { $$ = $1; }
	;

jump_statement:
	BREAK ';' {$$ = create_tree("jump_statement_1", JUMP_STATEMENT_1, 2, $1, $2);} 
	| CONTINUE ';' {$$ = create_tree("jump_statement_2", JUMP_STATEMENT_1, 2, $1, $2);} 
	| RETURN expression_opt ';' {$$ = create_tree("jump_statement_3", JUMP_STATEMENT_1, 3, $1, $2, $3);} 
	| GOTO identifier ';' {$$ = create_tree("jump_statement_4", JUMP_STATEMENT_1, 3, $1, $2, $3);} 
	;

declaration_statement:
	block_declaration { $$ = $1; }
	;

/*----------------------------------------------------------------------
 * Declarations.
 *----------------------------------------------------------------------*/

declaration_seq:
	declaration { $$ = $1; }
	| declaration_seq declaration {$$ = create_tree("declaration_seq_1", DECLARATION_SEQ_1, 2, $1, $2);} 
	;

declaration:
	block_declaration { $$ = $1; }
	| function_definition { $$ = $1; }
	;

block_declaration:
	simple_declaration { $$ = $1; }
	;

simple_declaration:
	  decl_specifier_seq init_declarator_list ';' {$$ = create_tree("simple_declaration_1", SIMPLE_DECLARATION_1, 3, $1, $2, $3);}
	|  decl_specifier_seq ';' {$$ = create_tree("simple_declaration_2", SIMPLE_DECLARATION_1, 2, $1, $2);}
	;

decl_specifier:
	storage_class_specifier { $$ = $1; }
	| type_specifier { $$ = $1; }
	| function_specifier { $$ = $1; }
	;

decl_specifier_seq:
	  decl_specifier { $$ = $1; } 
	| decl_specifier_seq decl_specifier {$$ = create_tree("decl_specifier_seq_1", DECL_SPECIFIER_SEQ_1, 2, $1, $2);}
	;

storage_class_specifier:
	AUTO { $$ = $1; }
	| REGISTER { $$ = $1; }
	| STATIC { $$ = $1; }
	| EXTERN { $$ = $1; }
	| MUTABLE { $$ = $1; }
	;

function_specifier:
	INLINE { $$ = $1; }
	| VIRTUAL { $$ = $1; }
	| EXPLICIT { $$ = $1; }
	;

type_specifier:
	simple_type_specifier { $$ = $1; }
	| class_specifier { $$ = $1; }
	| elaborated_type_specifier { $$ = $1; }
	| cv_qualifier { $$ = $1; }
	;

simple_type_specifier:
	  CLASS_NAME { $$ = $1; }
	| nested_name_specifier CLASS_NAME {$$ = create_tree("simple_type_specifier_1", SIMPLE_TYPE_SPECIFIER_1, 2, $1, $2);}
	| CHAR { $$ = $1; }
	| WCHAR_T { $$ = $1; }
	| BOOL { $$ = $1; }
	| SHORT { $$ = $1; }
	| INT { $$ = $1; }
	| LONG { $$ = $1; }
	| SIGNED { $$ = $1; }
	| UNSIGNED { $$ = $1; }
	| FLOAT { $$ = $1; }
	| DOUBLE { $$ = $1; }
	| VOID { $$ = $1; }
	;

elaborated_type_specifier:
	  class_key COLONCOLON nested_name_specifier identifier {$$ = create_tree("elaborated_type_specifier_1", ELABORATED_TYPE_SPECIFIER_1, 4, $1, $2, $3, $4);}
	| class_key COLONCOLON identifier {$$ = create_tree("elaborated_type_specifier_2", ELABORATED_TYPE_SPECIFIER_1, 3, $1, $2, $3);}
	
	;



/*----------------------------------------------------------------------
 * Declarators.
 *----------------------------------------------------------------------*/

init_declarator_list:
	init_declarator { $$ = $1; }
	| init_declarator_list ',' init_declarator {$$ = create_tree("init_declarator_list_1", INIT_DECLARATOR_LIST_1, 3, $1, $2, $3);}
	;

init_declarator:
	declarator initializer_opt { $$ = $1; }
	;

declarator:
	direct_declarator { $$ = $1; }
	| ptr_operator declarator {$$ = create_tree("declarator_1", DECLARATOR_1, 2, $1, $2);}
	;

direct_declarator:
	  declarator_id { $$ = $1; }
	| direct_declarator '(' parameter_declaration_clause ')' { $$ = create_tree("direct_declarator_1", DIRECT_DECLARATOR_1, 4, $1, $2, $3, $4); }
	| CLASS_NAME '(' parameter_declaration_clause ')' {$$ = create_tree("direct_declarator_2", DIRECT_DECLARATOR_1, 4, $1, $2, $3, $4);}
	| CLASS_NAME COLONCOLON declarator_id '(' parameter_declaration_clause ')' {$$ = create_tree("direct_declarator_3", DIRECT_DECLARATOR_1, 6, $1, $2, $3, $4, $5, $6);}
	| CLASS_NAME COLONCOLON CLASS_NAME '(' parameter_declaration_clause ')' {$$ = create_tree("direct_declarator_4", DIRECT_DECLARATOR_1, 6, $1, $2, $3, $4, $5, $6);}
	| direct_declarator '[' constant_expression_opt ']' {$$ = create_tree("direct_declarator_5", DIRECT_DECLARATOR_1, 4, $1, $2, $3, $4);}
	| '(' declarator ')' {$$ = create_tree("direct_declarator_6", DIRECT_DECLARATOR_1, 3, $1, $2, $3);}
	;

ptr_operator:
	'*' { $$ = $1; }
	| '*' cv_qualifier_seq {$$ = create_tree("ptr_operator_1", PTR_OPERATOR_1, 2, $1, $2);}
	| '&' { $$ = $1; }
	| nested_name_specifier '*' {$$ = create_tree("ptr_operator_2", PTR_OPERATOR_1, 2, $1, $2);}
	| nested_name_specifier '*' cv_qualifier_seq {$$ = create_tree("ptr_operator_3", PTR_OPERATOR_1, 3, $1, $2, $3);}
	| COLONCOLON nested_name_specifier '*' {$$ = create_tree("ptr_operator_4",PTR_OPERATOR_1, 3, $1, $2, $3);}
	| COLONCOLON nested_name_specifier '*' cv_qualifier_seq {$$ = create_tree("ptr_operator_5", PTR_OPERATOR_1, 4, $1, $2, $3, $4);}
	;

cv_qualifier_seq:
	cv_qualifier { $$ = $1; }
	| cv_qualifier cv_qualifier_seq {$$ = create_tree("cv_qualifier_1", CV_QUALIFIER_SEQ_1, 2, $1, $2);}
	;

cv_qualifier:
	CONST { $$ = $1; }
	| VOLATILE { $$ = $1; }
	;

declarator_id:
	id_expression {$$ = $1;}
	| COLONCOLON id_expression {$$ = create_tree("declarator_id_1", DECLARATOR_ID_1, 2, $1, $2);}
	| COLONCOLON nested_name_specifier CLASS_NAME {$$ = create_tree("declarator_id_2", DECLARATOR_ID_1, 3, $1, $2, $3);}
	| COLONCOLON CLASS_NAME {$$ = create_tree("declarator_id_3", DECLARATOR_ID_1, 2, $1, $2);}
	;

type_id:
	type_specifier_seq abstract_declarator_opt {$$ = create_tree("type_id_1", TYPE_ID_1, 2, $1, $2);}
	;

type_specifier_seq:
	type_specifier type_specifier_seq_opt {$$ = create_tree("type_specifier_seq_1", TYPE_SPECIFIER_SEQ_1, 2, $1, $2);}
	;

abstract_declarator:
	ptr_operator abstract_declarator_opt {$$ = create_tree("abstract_declarator_1", ABSTRACT_DECLARATOR_1, 2, $1, $2);}
	| direct_abstract_declarator {$$ = $1;}
	;

direct_abstract_declarator:
	direct_abstract_declarator '(' parameter_declaration_clause ')' { $$ = create_tree("direct_abstract_declarator_1", DIRECT_ABSTRACT_DECLARATOR_1, 4, $1, $2, $3, $4); }
	| '(' parameter_declaration_clause ')'			        { $$ = create_tree("direct_abstract_declarator_2", DIRECT_ABSTRACT_DECLARATOR_1, 3, $1, $2, $3); }
	| direct_abstract_declarator '[' constant_expression_opt ']'    { $$ = create_tree("direct_abstract_declarator_3", DIRECT_ABSTRACT_DECLARATOR_1, 4, $1, $2, $3, $4); }
	| '[' constant_expression_opt ']'			        { $$ = create_tree("direct_abstract_declarator_4", DIRECT_ABSTRACT_DECLARATOR_1, 3, $1, $2, $3); }
	| '(' abstract_declarator ')'				        { $$ = create_tree("direct_abstract_declarator_5", DIRECT_ABSTRACT_DECLARATOR_1, 3, $1, $2, $3); }
	;

parameter_declaration_clause:
    parameter_declaration_list { $$ = $1; }
    | parameter_declaration_list ELLIPSIS { $$ = NULL; }
	| ELLIPSIS {$$ = NULL;}
    | parameter_declaration_list ',' ELLIPSIS { $$ = NULL; }
	| %empty { $$ = NULL; } 
	;

parameter_declaration_list:
	parameter_declaration {$$ = $1;}
	| parameter_declaration_list ',' parameter_declaration {$$ = create_tree("parameter_declaration_list_1", PARAMETER_DECLARATION_LIST_1, 3, $1, $2, $3);}
	;

parameter_declaration:
	decl_specifier_seq declarator {$$ = create_tree("parameter_declaration_1", PARAMETER_DECLARATION_1, 2, $1, $2);}
	| decl_specifier_seq declarator '=' assignment_expression {$$ = create_tree("parameter_declaration_2", PARAMETER_DECLARATION_1, 4, $1, $2, $3, $4);}
	| decl_specifier_seq abstract_declarator_opt {$$ = create_tree("parameter_declaration_3", PARAMETER_DECLARATION_1, 2, $1, $2);}
	| decl_specifier_seq abstract_declarator_opt '=' assignment_expression {$$ = create_tree("parameter_declaration_4", PARAMETER_DECLARATION_1, 4, $1, $2, $3, $4);}
	;

function_definition:
	declarator ctor_initializer_opt function_body {$$ = create_tree("function_definition_1", FUNCTION_DEFINITION_1, 3, $1, $2, $3);}
    | decl_specifier_seq declarator ctor_initializer_opt function_body {$$ = create_tree("function_definition_2", FUNCTION_DEFINITION_1, 4, $1, $2, $3, $4);}
	;

function_body:
	compound_statement {$$ = $1;}
	;

initializer:
	'=' initializer_clause {$$ = create_tree("initializer_1", INITIALIZER_1, 2, $1, $2);}
	| '(' expression_list ')' {$$ = create_tree("initializer_2", INITIALIZER_1, 3, $1, $2, $3);}
	;

initializer_clause:
	assignment_expression {$$ = $1;}
	| '{' initializer_list COMMA_opt '}' {$$ = create_tree("initializer_clause_1", INITIALIZER_CLAUSE_1, 3, $1, $2, $3);}
	| '{' '}' {$$ = create_tree("initializer_clause_2", INITIALIZER_CLAUSE_1, 2, $1, $2);}
	;

initializer_list:
	initializer_clause {$$ = $1;}
	| initializer_list ',' initializer_clause {$$ = create_tree("initializer_list_1", INITIALIZER_LIST_1, 3, $1, $2, $3);}
	;

/*----------------------------------------------------------------------
 * Classes.
 *----------------------------------------------------------------------*/

class_specifier:
    class_head '{' member_specification_opt '}' { $$ = create_tree("class_specifier_1", CLASS_SPECIFIER_1, 4, $1, $2, $3, $4); }
	;

class_head:       
    class_key identifier { $$ = create_tree("class_head_1", CLASS_HEAD_1, 2, $1, $2); insert_typename_tree($2, CLASS_NAME); } 
	| class_key nested_name_specifier identifier {$$ = create_tree("class_head_2", CLASS_HEAD_1, 3, $1, $2, $3); insert_typename_tree($3, CLASS_NAME);}
	;

class_key:
    CLASS {$$ = $1;} 
	| STRUCT {$$ = $1;}
	| UNION {$$ = $1;}
	;

member_specification:
	member_declaration member_specification_opt {$$ = create_tree("member_specification_1", MEMBER_SPECIFICATION_1, 2, $1, $2);}
	| access_specifier ':' member_specification_opt {$$ = create_tree("member_specification_2", MEMBER_SPECIFICATION_1, 3, $1, $2, $3);}
	;

member_declaration:
	decl_specifier_seq member_declarator_list ';' {$$ = create_tree("member_declaration_1", MEMBER_DECLARATION_1, 3, $1, $2, $3);}
	| decl_specifier_seq ';' {$$ = create_tree("member_declaration_2", MEMBER_DECLARATION_1, 2, $1, $2);}
	| member_declarator_list ';' {$$ = create_tree("member_declaration_3", MEMBER_DECLARATION_1, 2, $1, $2);}
	| ';' {$$ = $1;}
	| function_definition SEMICOLON_opt {$$ = create_tree("member_declaration_4", MEMBER_DECLARATION_1, 2, $1, $2);}
	| qualified_id ';' {$$ = create_tree("member_declaration_5", MEMBER_DECLARATION_1, 2, $1, $2);}
	;

member_declarator_list:
	member_declarator {$$ = $1;}
	| member_declarator_list ',' member_declarator {$$ = create_tree("member_declarator_list_1", MEMBER_DECLARATOR_LIST_1, 3, $1, $2, $3);}
	;

member_declarator:
        declarator {$$ = $1;}
	| declarator pure_specifier {$$ = create_tree("member_declarator_1", MEMBER_DECLARATOR_1, 2, $1, $2);}
	| declarator constant_initializer {$$ = create_tree("member_declarator_2", MEMBER_DECLARATOR_1, 2, $1, $2);}
	| identifier ':' constant_expression {$$ = create_tree("member_declarator_3", MEMBER_DECLARATOR_1, 3, $1, $2, $3);}
	;

pure_specifier:
      '=' '0' {$$ = NULL; }
	;

constant_initializer:
	'=' constant_expression {$$ = create_tree("constant_initializer_1", CONSTANT_INITIALIZER_1, 2, $1, $2);}
	;
	
access_specifier:
	PRIVATE	    { $$ = $1; }
	| PROTECTED { $$ = $1; }
	| PUBLIC    { $$ = $1; }
	;


/*----------------------------------------------------------------------
 * Special member functions.
 *----------------------------------------------------------------------*/


ctor_initializer:
	':' mem_initializer_list {$$ = create_tree("ctor_initializer_1", CTOR_INITIALIZER_1, 2, $1, $2);}
	;

mem_initializer_list:
	mem_initializer {$$ = $1;}
	| mem_initializer ',' mem_initializer_list {$$ = create_tree("mem_initializer_list_1", MEM_INITIALIZER_LIST_1, 3, $1, $2, $3);}
	;

mem_initializer:
	mem_initializer_id '(' expression_list_opt ')' {$$ = create_tree("mem_initializer_1", MEM_INITIALIZER_1, 4, $1, $2, $3, $4);}
	;

mem_initializer_id:
	COLONCOLON nested_name_specifier CLASS_NAME {$$ = create_tree("mem_initializer_id_1", MEM_INITIALIZER_ID_1, 3, $1, $2, $3);}
	| COLONCOLON CLASS_NAME {$$ = create_tree("mem_initializer_id_2",  MEM_INITIALIZER_ID_1,2, $1, $2);}
	| nested_name_specifier CLASS_NAME {$$ = create_tree("mem_initializer_id_3", MEM_INITIALIZER_ID_1, 2, $1, $2);}
	| CLASS_NAME {$$ = $1;}
	| identifier {$$ = $1;}
	;


/*----------------------------------------------------------------------
 * Epsilon (optional) definitions.
 *----------------------------------------------------------------------*/

declaration_seq_opt:
	%empty {$$ = NULL;}
	| declaration_seq {$$ = $1;}
	;

expression_list_opt:
	%empty {$$ = NULL;}
	| expression_list {$$ = $1;}
	;

new_placement_opt:
	%empty {$$ = NULL;}
	| new_placement {$$ = $1;}
	;

new_initializer_opt:
	%empty {$$ = NULL;}
	| new_initializer {$$ = $1;}
	;

new_declarator_opt:
	%empty {$$ = NULL;}
	| new_declarator {$$ = $1;}
	;

expression_opt:
	%empty {$$ = NULL;}
	| expression {$$ = $1;}
	;

statement_seq_opt:
	%empty {$$ = NULL;}
	| statement_seq {$$ = $1;}
	;

condition_opt:
	%empty {$$ = NULL;}
	| condition {$$ = $1;}
	;


initializer_opt:
	%empty {$$ = NULL;}
	| initializer {$$ = $1;}
	;

constant_expression_opt:
	%empty {$$ = NULL;}
	| constant_expression {$$ = $1;}
	;

abstract_declarator_opt:
	%empty {$$ = NULL;}
	| abstract_declarator {$$ = $1;}
	;

type_specifier_seq_opt:
	%empty {$$ = NULL;}
	| type_specifier_seq {$$ = $1;}
	;

ctor_initializer_opt:
	%empty {$$ = NULL;}
	| ctor_initializer {$$ = $1;}
	;

COMMA_opt:
	%empty {$$ = NULL;}
	| ',' {$$ = $1;}
	;

member_specification_opt:
	%empty {$$ = NULL;}
	| member_specification {$$ = $1;}
	;

SEMICOLON_opt:
	%empty {$$ = NULL;}
	| ';' {$$ = $1;}
	;



%%

static void yyerror(char *s) {
	fprintf(stderr, "Syntax error: line %d:, token: %s   %s\n", yylineno, yytext, s);
	
	exit(2);
}
