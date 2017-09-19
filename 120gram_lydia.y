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
#include <stdlib.h>
#include <string.h>
#include <limits.h>

//#include "120lex.h"
#include "token.h"
#include "tree_lydia.h"
#include "nonterms.h"


#define YYDEBUG 1
extern int yylineno;
extern char *yytext;
//extern tableptr classtable;

/* extern in 120++.c 
 * Head pointer to start of program (translation unit).
 */
 
Treeptr YYPROGRAM; 

 
int yydebug=0;

static void yyerror(char *s);

%}

%union {
  struct tree *t;
}

%token <t> IDENTIFIER INTEGER FLOATING CHARACTER STRING
%token <t> TYPEDEF_NAME CLASS_NAME ENUM_NAME
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
%token <t> ';' '(' ')' ',' '{' '}' '[' ']' '&' '.' '<' '>' '_'
%token <t> '+' '=' '-' '%' '*' '/' '|' '?' '^' '!' '~' ':' 

%token <t> NAMESPACE_NAME TEMPLATE_NAME

%type <t> using_declaration
%type <t> using_directive namespace_definition namespace_alias_definition
%type <t> named_namespace_definition unnamed_namespace_definition
%type <t> original_namespace_definition namespace_body qualified_namespace_specifier
%type <t> typedef_name assignment_expression extension_namespace_definition
%type <t> enum_name identifier literal class_name 
%type <t> integer_literal character_literal floating_literal string_literal 
%type <t> boolean_literal translation_unit primary_expression id_expression 
%type <t> unqualified_id qualified_id nested_name_specifier postfix_expression 
%type <t> expression_list unary_expression unary_operator new_expression 
%type <t> new_placement new_type_id new_declarator direct_new_declarator 
%type <t> new_initializer delete_expression cast_expression pm_expression 
%type <t> multiplicative_expression additive_expression shift_expression
%type <t> relational_expression inclusive_or_expression 
%type <t> equality_expression and_expression exclusive_or_expression
%type <t> logical_and_expression logical_or_expression conditional_expression 
%type <t> assignment_operator expression constant_expression statement 
%type <t> labeled_statement expression_statement compound_statement statement_seq 
%type <t> selection_statement condition iteration_statement for_init_statement 
%type <t> jump_statement declaration_statement declaration_seq declaration 
%type <t> block_declaration simple_declaration decl_specifier decl_specifier_seq 
%type <t> storage_class_specifier function_specifier type_specifier simple_type_specifier 
%type <t> type_name elaborated_type_specifier enum_specifier enumerator_list 
%type <t> enumerator_definition enumerator 
%type <t> asm_definition linkage_specification init_declarator_list 
%type <t> init_declarator declarator direct_declarator ptr_operator 
%type <t> cv_qualifier_seq cv_qualifier declarator_id type_id 
%type <t> type_specifier_seq abstract_declarator direct_abstract_declarator
%type <t> parameter_declaration_list parameter_declaration function_definition
%type <t> parameter_declaration_clause function_body 
%type <t> initializer initializer_clause initializer_list class_specifier 
%type <t> class_head class_key member_specification member_declaration 
%type <t> member_declarator_list member_declarator pure_specifier constant_initializer 
%type <t> access_specifier 
%type <t> conversion_function_id conversion_type_id conversion_declarator ctor_initializer 
%type <t> mem_initializer_list mem_initializer mem_initializer_id operator_function_id 
%type <t> try_block function_try_block exception_specification
%type <t> handler_seq handler exception_declaration throw_expression 
%type <t> type_id_list declaration_seq_opt nested_name_specifier_opt 
%type <t> expression_list_opt COLONCOLON_opt new_placement_opt new_initializer_opt 
%type <t> new_declarator_opt expression_opt statement_seq_opt condition_opt 
%type <t> enumerator_list_opt initializer_opt constant_expression_opt
%type <t> abstract_declarator_opt  COMMA_opt 
%type <t> type_specifier_seq_opt direct_abstract_declarator_opt ctor_initializer_opt
%type <t> member_specification_opt SEMICOLON_opt conversion_declarator_opt 
%type <t> handler_seq_opt assignment_expression_opt type_id_list_opt operator

%type <t> namespace_name original_namespace_name


%start translation_unit

%debug
%%

/*----------------------------------------------------------------------
 * Context-dependent identifiers.
 *----------------------------------------------------------------------*/

typedef_name:
        TYPEDEF_NAME {$$ = $1;}
	;

namespace_name:
	original_namespace_name {$$ = $1;}
	;


original_namespace_name:
	NAMESPACE_NAME {$$ = $1;}
	;


class_name:
	CLASS_NAME {$$ = $1;}
	;

enum_name:
	ENUM_NAME { $$ = $1; }
	;

/*----------------------------------------------------------------------
 * Lexical elements.
 *----------------------------------------------------------------------*/

identifier:
        IDENTIFIER {$$ = $1;}
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
        declaration_seq_opt {$$ = create_tree("program", TRANSLATION_UNIT_1, 1, $1); YYPROGRAM = $$;}
	;

/*----------------------------------------------------------------------
 * Expressions.
 *----------------------------------------------------------------------*/

primary_expression:
	literal {$$ = $1;}
	| THIS {$$ = $1;}
	| '(' expression ')' {$$ = create_tree("primary_expression_1", PRIMARY_EXPRESSION_1, 3, $1, $2, $3);}
	| id_expression {$$ = $1;}
	;

id_expression:
        unqualified_id { $$ = $1; } 
	| qualified_id { $$ = $1; }
	;

unqualified_id:
        identifier { $$ = $1; }
	| operator_function_id {$$ = $1;}
	| conversion_function_id {$$ = $1;}
	| '~' class_name {$$ = create_tree("unqualified_id_1", UNQUALIFIED_ID_1, 2, $1, $2);}
	;

qualified_id:
	nested_name_specifier unqualified_id {$$ = create_tree("qualified_id_1", QUALIFIED_ID_1, 2, $1, $2);}
        | nested_name_specifier TEMPLATE unqualified_id {$$ = create_tree("qualified_id_2", QUALIFIED_ID_1, 3, $1, $2, $3);}
	;

nested_name_specifier:
	class_name COLONCOLON nested_name_specifier {$$ = create_tree("nested_name_specifier_2", NESTED_NAME_SPECIFIER_1, 3, $1, $2, $3);}
        | namespace_name COLONCOLON nested_name_specifier {$$ = create_tree("nested_name_specifier_2", NESTED_NAME_SPECIFIER_1, 3, $1, $2, $3);}
	| class_name COLONCOLON {$$ = create_tree("nested_name_specifier_3", NESTED_NAME_SPECIFIER_1, 2, $1, $2);}
	;

postfix_expression:
	primary_expression {$$ = $1;}
	| postfix_expression '[' expression ']' {$$ = create_tree("postfix_expression_1", POSTFIX_EXPRESSION_1, 4, $1, $2, $3, $4);}
	| postfix_expression '(' expression_list_opt ')' {$$ = create_tree("postfix_expression_2", POSTFIX_EXPRESSION_1, 4, $1, $2, $3, $4);}

	| postfix_expression '.' COLONCOLON id_expression {$$ = create_tree("postfix_expression_3", POSTFIX_EXPRESSION_1, 4, $1, $2, $3, $4);}
	| postfix_expression '.' id_expression {$$ = create_tree("postfix_expression_4", POSTFIX_EXPRESSION_1, 3, $1, $2, $3);}

	| postfix_expression ARROW COLONCOLON id_expression {$$ = create_tree("postfix_expression_5", POSTFIX_EXPRESSION_1, 4, $1, $2, $3, $4);}
	| postfix_expression ARROW id_expression {$$ = create_tree("postfix_expression_6", POSTFIX_EXPRESSION_1, 3, $1, $2, $3);}
	| postfix_expression PLUSPLUS {$$ = create_tree("postfix_expression_7", POSTFIX_EXPRESSION_1, 2, $1, $2);}
	| postfix_expression MINUSMINUS {$$ = create_tree("postfix_expression_8", POSTFIX_EXPRESSION_1, 2, $1, $2);}

	;

expression_list:
	assignment_expression { $$ = $1; }
    | expression_list ',' assignment_expression {$$ = create_tree("expression_list_1", EXPRESSION_LIST_1, 2, $1 , $3); }
	;

unary_expression:
	postfix_expression {$$ = $1;}
	| PLUSPLUS cast_expression {$$ = create_tree("unary_expression_1", UNARY_EXPRESSION_1, 2, $1, $2);}
	| MINUSMINUS cast_expression {$$ = create_tree("unary_expression_2", UNARY_EXPRESSION_1, 2, $1, $2);}
	| '*' cast_expression {$$ = create_tree("unary_expression_3", UNARY_EXPRESSION_1, 2, $1, $2);}
	| '&' cast_expression {$$ = create_tree("unary_expression_4", UNARY_EXPRESSION_1, 2, $1, $2);}
	| unary_operator cast_expression {$$ = create_tree("unary_expression_5", UNARY_EXPRESSION_1, 2, $1, $2);}
	| SIZEOF unary_expression {$$ = create_tree("unary_expression_6", UNARY_EXPRESSION_1, 2, $1, $2);}
	| SIZEOF '(' type_id ')' {$$ = create_tree("unary_expression_7", UNARY_EXPRESSION_1, 4, $1, $2, $3, $4);}
	| new_expression {$$ = $1;}
	| delete_expression {$$ = $1;}
	;

unary_operator:
	'+' {$$ = $1;}
	| '-' {$$ = $1;}
	| '!' {$$ = $1;}
	| '~' {$$ = $1;}
	;

new_expression:
	NEW new_placement_opt new_type_id new_initializer_opt {$$ = create_tree("new_expression_1", NEW_EXPRESSION_1, 4, $1, $2, $3, $4);}
	| COLONCOLON NEW new_placement_opt new_type_id new_initializer_opt {$$ = create_tree("new_expression_2", NEW_EXPRESSION_1, 5, $1, $2, $3, $4, $5);}
	| NEW new_placement_opt '(' type_id ')' new_initializer_opt {$$ = create_tree("new_expression_3", NEW_EXPRESSION_1, 6, $1, $2, $3, $4, $5, $6);}
	| COLONCOLON NEW new_placement_opt '(' type_id ')' new_initializer_opt {$$ = create_tree("new_expression_4", NEW_EXPRESSION_1, 7, $1, $2, $3, $4, $5, $6, $7);}
	;

new_placement:
	'(' expression_list ')' {$$ = create_tree("new_placement_1", NEW_PLACEMENT_1, 3, $1, $2, $3);}
	;

new_type_id:
	type_specifier_seq new_declarator_opt {$$ = create_tree("new_type_id_1", NEW_TYPE_ID_1, 2, $1, $2);}
	;

new_declarator:
	ptr_operator new_declarator_opt {$$ = create_tree("new_declarator_1", NEW_DECLARATOR_1, 2, $1, $2);}
	| direct_new_declarator {$$ = $1;}
	;

direct_new_declarator:
	'[' expression ']' {$$ = create_tree("direct_new_declarator_1", DIRECT_NEW_DECLARATOR_1, 3, $1, $2, $3);}
	| direct_new_declarator '[' constant_expression ']' {$$ = create_tree("direct_new_declarator_2", DIRECT_NEW_DECLARATOR_1, 4, $1, $2, $3, $4);}
	;

new_initializer:
	'(' expression_list_opt ')' {$$ = create_tree("new_initializer_1", NEW_INITIALIZER_1, 3, $1, $2, $3);}
	;

delete_expression:
	DELETE cast_expression {$$ = create_tree("delete_expression_1", DELETE_EXPRESSION_1, 2, $1, $2);}
	| COLONCOLON DELETE cast_expression {$$ = create_tree("delete_expression_2", DELETE_EXPRESSION_1, 3, $1, $2, $3);}
	| DELETE '[' ']' cast_expression {$$ = create_tree("delete_expression_3", DELETE_EXPRESSION_1, 4, $1, $2, $3, $4);}
	| COLONCOLON DELETE '[' ']' cast_expression {$$ = create_tree("delete_expression_4", DELETE_EXPRESSION_1, 5, $1, $2, $3, $4, $5);}
	;

cast_expression:
	unary_expression {$$ = $1;}
	| '(' type_id ')' cast_expression {$$ = create_tree("cast_expression_1", CAST_EXPRESSION_1, 4, $1, $2, $3, $4);}
	;

pm_expression:
	cast_expression {$$ = $1;}
	| pm_expression DOTSTAR cast_expression {$$ = create_tree("pm_expression_335", 333, 3, $1, $2, $3);}
	| pm_expression ARROWSTAR cast_expression {$$ = create_tree("pm_expression_336", 333, 3, $1, $2, $3);}
	;

multiplicative_expression:
	pm_expression {$$ = $1;}
	| multiplicative_expression '*' pm_expression {$$ = create_tree("multiplicative_expression_341", 339, 3, $1, $2, $3);}
	| multiplicative_expression '/' pm_expression {$$ = create_tree("multiplicative_expression_342", 339, 3, $1, $2, $3);}
	| multiplicative_expression '%' pm_expression {$$ = create_tree("multiplicative_expression_343", 339, 3, $1, $2, $3);}
	;

additive_expression:
	multiplicative_expression {$$ = $1;}
	| additive_expression '+' multiplicative_expression {$$ = create_tree("additive_expression_348", 346, 3, $1, $2, $3);}
	| additive_expression '-' multiplicative_expression {$$ = create_tree("additive_expression_349", 346, 3, $1, $2, $3);}
	;

shift_expression:
	additive_expression {$$ = $1;}
	| shift_expression SL additive_expression {$$ = create_tree("shift_expression_354", 352, 3, $1, $2, $3);}
	| shift_expression SR additive_expression {$$ = create_tree("shift_expression_355", 352, 3, $1, $2, $3);}
	;

relational_expression:
	shift_expression {$$ = $1;}
	| relational_expression '<' shift_expression {$$ = create_tree("relational_expression_360", 358, 3, $1, $2, $3);}
	| relational_expression '>' shift_expression {$$ = create_tree("relational_expression_361", 358, 3, $1, $2, $3);}
	| relational_expression LTEQ shift_expression {$$ = create_tree("relational_expression_362", 358, 3, $1, $2, $3);}
	| relational_expression GTEQ shift_expression {$$ = create_tree("relational_expression_363", 358, 3, $1, $2, $3);}
	;

equality_expression:
	relational_expression {$$ = $1;}
	| equality_expression EQ relational_expression {$$ = create_tree("equality_expression_368", 366, 3, $1, $2, $3);}
	| equality_expression NOTEQ relational_expression {$$ = create_tree("equality_expression_369", 366, 3, $1, $2, $3);}
	;

and_expression:
	equality_expression {$$ = $1;}
	| and_expression '&' equality_expression {$$ = create_tree("and_expression_374", 372, 3, $1, $2, $3);}
	;

exclusive_or_expression:
	and_expression {$$ = $1;}
	| exclusive_or_expression '^' and_expression {$$ = create_tree("exclusive_or_expression_379", 377, 3, $1, $2, $3);}
	;

inclusive_or_expression:
	exclusive_or_expression {$$ = $1;}
	| inclusive_or_expression '|' exclusive_or_expression {$$ = create_tree("inclusive_or_expression_384", 382, 3, $1, $2, $3);}
	;

logical_and_expression:
	inclusive_or_expression {$$ = $1;}
	| logical_and_expression ANDAND inclusive_or_expression {$$ = create_tree("logical_and_expression_389", 387, 3, $1, $2, $3);}
	;

logical_or_expression:
	logical_and_expression {$$ = $1;}
	| logical_or_expression OROR logical_and_expression {$$ = create_tree("logical_or_expression_394", 392, 3, $1, $2, $3);}
	;

conditional_expression:
	logical_or_expression {$$ = $1;}
	| logical_or_expression  '?' expression ':' assignment_expression {$$ = create_tree("conditional_expression_399", 397, 5, $1, $2, $3, $4, $5);}
	;

assignment_expression:
	conditional_expression {$$ = $1;}
	| logical_or_expression assignment_operator assignment_expression {$$ = create_tree("assignment_expression_404", 402, 3, $1, $2, $3);}
	| throw_expression {$$ = $1;}
	;

assignment_operator:
	'=' {$$ = $1;}
	| MULEQ {$$ = $1;}
	| DIVEQ {$$ = $1;}
	| MODEQ {$$ = $1;}
	| ADDEQ {$$ = $1;}
	| SUBEQ {$$ = $1;}
	| SREQ {$$ = $1;}
	| SLEQ {$$ = $1;}
	| ANDEQ {$$ = $1;}
	| XOREQ {$$ = $1;}
	| OREQ {$$ = $1;}
	;

expression:
	assignment_expression {$$ = $1;}
	| expression ',' assignment_expression {$$ = create_tree("expression_424", 422, 2, $1 , $3);}
	;

constant_expression:
	conditional_expression {$$ = $1;}
	;

/*----------------------------------------------------------------------
 * Statements.
 *----------------------------------------------------------------------*/

statement:
	labeled_statement {$$ = $1;}
	| expression_statement {$$ = $1;}
	| compound_statement {$$ = $1;}
	| selection_statement {$$ = $1;}
	| iteration_statement {$$ = $1;}
	| jump_statement {$$ = $1;}
	| declaration_statement {$$ = $1;}
	| try_block {$$ = $1;}
	;

labeled_statement:
	identifier ':' statement {$$ = create_tree("labeled_statement_447", 446, 3, $1, $2, $3);}
	| CASE constant_expression ':' statement {$$ = create_tree("labeled_statement_448", 446, 4, $1, $2, $3, $4);}
	| DEFAULT ':' statement {$$ = create_tree("labeled_statement_449", 446, 3, $1, $2, $3);}
	;

expression_statement:
//	expression_opt ';' {$$ = create_tree("expression_statement_453", 452, 2, $1, $2);}
        expression_opt ';' { $$ = $1; }
	;

compound_statement:
	'{' statement_seq_opt '}' {$$ = create_tree("compound_statement_457", 456, 3, $1, $2, $3);}
	;

statement_seq:
	statement {$$ = $1;}
	| statement_seq statement {$$ = create_tree("statement_seq_462", 460, 2, $1, $2);}
	;

selection_statement:
	IF '(' condition ')' statement {$$ = create_tree("selection_statement_466", 465, 5, $1, $2, $3, $4, $5);}
	| IF '(' condition ')' statement ELSE statement {$$ = create_tree("selection_statement_467", 465, 7, $1, $2, $3, $4, $5, $6, $7);}
	| SWITCH '(' condition ')' statement {$$ = create_tree("selection_statement_468", 465, 5, $1, $2, $3, $4, $5);}
	;

condition:
	expression {$$ = $1;}
	| type_specifier_seq declarator '=' assignment_expression {$$ = create_tree("condition_473", 471, 4, $1, $2, $3, $4);}
	;

iteration_statement:
	WHILE '(' condition ')' statement {$$ = create_tree("iteration_statement_477", 476, 5, $1, $2, $3, $4, $5);}
	| DO statement WHILE '(' expression ')' ';' {$$ = create_tree("iteration_statement_478", 476, 7, $1, $2, $3, $4, $5, $6, $7);}
	| FOR '(' for_init_statement condition_opt ';' expression_opt ')' statement {$$ = create_tree("iteration_statement_479", 476, 8, $1, $2, $3, $4, $5, $6, $7, $8);}
	;

for_init_statement:
	expression_statement {$$ = $1;}
	| simple_declaration {$$ = $1;}
	;

jump_statement:
        BREAK ';' { $$ = $1; }
        | CONTINUE ';' { $$ = $1; }
	| RETURN expression_opt ';' {$$ = create_tree("jump_statement_490", 487, 2, $1, $2 );}
	| GOTO identifier ';' {$$ = create_tree("jump_statement_491", 487, 2, $1, $2 );}
	;

declaration_statement:
	block_declaration {$$ = $1;}
	;

/*----------------------------------------------------------------------
 * Declarations.
 *----------------------------------------------------------------------*/

declaration_seq:
        declaration { $$ = $1; }
	| declaration_seq declaration {$$ = create_tree("declaration_seq_504", 502, 2, $1, $2);}
	;

declaration:
	block_declaration {$$ = $1;}
	| function_definition {$$ = $1;}
//        | template_declaration {$$ = $1;}
//	| explicit_instantiation {$$ = $1;} 
//	| explicit_specialization {$$ = $1;} 
	| linkage_specification {$$ = $1;}
        | namespace_definition {$$ = $1;}
	;

block_declaration:
        simple_declaration { $$ = $1; }
	| asm_definition { $$ = $1; }
        | namespace_alias_definition { $$ = $1; }
	| using_declaration { $$ = $1; }
	| using_directive { $$ = $1; } 
	;

simple_declaration:
	decl_specifier_seq init_declarator_list ';' {$$ = create_tree("simple_declaration_526", 525, 2, $1, $2 );}
        |  decl_specifier_seq ';' { $$ = $1; }
	;

decl_specifier:
	storage_class_specifier { $$ = $1; }
        | type_specifier { $$ = $1; }
	| function_specifier {$$ = $1;}
	| FRIEND {$$ = $1;}
	| TYPEDEF {$$ = $1;}
	;

decl_specifier_seq:
	decl_specifier {$$ = $1;}
	| decl_specifier_seq decl_specifier {$$ = create_tree("decl_specifier_seq_540", 538, 2, $1, $2);}
	;

storage_class_specifier:
	AUTO {$$ = $1;}
	| REGISTER {$$ = $1;}
	| STATIC {$$ = $1;}
	| EXTERN {$$ = $1;}
	| MUTABLE {$$ = $1;}
	;

function_specifier:
	INLINE {$$ = $1;}
	| VIRTUAL {$$ = $1;}
	| EXPLICIT {$$ = $1;}
	;

type_specifier:
        simple_type_specifier { $$ = $1;}
        | class_specifier {$$ = $1;}
	| enum_specifier {$$ = $1;}
	| elaborated_type_specifier {$$ = $1;}
	| cv_qualifier {$$ = $1;}
	;

simple_type_specifier:
	type_name { $$ = $1;}
	| nested_name_specifier type_name {$$ = create_tree("simple_type_specifier_567", 565, 2, $1, $2);}
	| COLONCOLON nested_name_specifier_opt type_name {$$ = create_tree("simple_type_specifier_568", 565, 3, $1, $2, $3);}
	| CHAR {$$ = $1;}
	| WCHAR_T {$$ = $1;}
	| BOOL {$$ = $1;}
	| SHORT {$$ = $1;}
        | INT {$$ = $1;}
	| LONG {$$ = $1;}
	| SIGNED {$$ = $1;}
	| UNSIGNED {$$ = $1;}
	| FLOAT {$$ = $1;}
	| DOUBLE {$$ = $1;}
	| VOID {$$ = $1;}
	;

type_name:
     class_name {$$ = $1;}
	| enum_name {$$ = $1;}
	| typedef_name {$$ = $1;}
	;

elaborated_type_specifier:
	class_key COLONCOLON nested_name_specifier identifier {$$ = create_tree("elaborated_type_specifier_589", 588, 4, $1, $2, $3, $4);}
	| class_key COLONCOLON identifier {$$ = create_tree("elaborated_type_specifier_590", 588, 3, $1, $2, $3);}
	| ENUM COLONCOLON nested_name_specifier identifier {$$ = create_tree("elaborated_type_specifier_591", 588, 4, $1, $2, $3, $4);}
	| ENUM COLONCOLON identifier {$$ = create_tree("elaborated_type_specifier_592", 588, 3, $1, $2, $3);}
	| ENUM nested_name_specifier identifier {$$ = create_tree("elaborated_type_specifier_593", 588, 3, $1, $2, $3);}
	| TYPENAME COLONCOLON_opt nested_name_specifier identifier {$$ = create_tree("elaborated_type_specifier_594", 588, 4, $1, $2, $3, $4);}
	;

/*
enum_name:
	identifier {$$ = $1;}
	;
*/

enum_specifier:
	ENUM identifier '{' enumerator_list_opt '}' {$$ = create_tree("enum_specifier_605", 604, 5, $1, $2, $3, $4, $5);}
	;

enumerator_list:
	enumerator_definition {$$ = $1;}
	| enumerator_list ',' enumerator_definition {$$ = create_tree("enumerator_list_610", 608, 3, $1, $2, $3);}
	;

enumerator_definition:
	enumerator {$$ = $1;}
	| enumerator '=' constant_expression {$$ = create_tree("enumerator_definition_615", 613, 3, $1, $2, $3);}
	;

enumerator:
	identifier {$$ = $1;}
	;

/*
namespace_name:
	original_namespace_name {$$ = $1;}
	| namespace_alias {$$ = $1;}
	;

original_namespace_name:
	identifier {$$ = $1;}
	;
*/

namespace_definition:
	named_namespace_definition {$$ = $1;}
	| unnamed_namespace_definition {$$ = $1;}
	;

named_namespace_definition:
	original_namespace_definition {$$ = $1;}
	| extension_namespace_definition {$$ = $1;}
	;

original_namespace_definition:
	NAMESPACE identifier '{' namespace_body '}' {$$ = create_tree("original_namespace_definition_644", 643, 5, $1, $2, $3, $4, $5);}
	;

extension_namespace_definition:
	NAMESPACE original_namespace_name '{' namespace_body '}' {$$ = create_tree("extension_namespace_definition_648", 647, 5, $1, $2, $3, $4, $5);}
	;

unnamed_namespace_definition:
	NAMESPACE '{' namespace_body '}' {$$ = create_tree("unnamed_namespace_definition_652", 651, 4, $1, $2, $3, $4);}
	;

namespace_body:
	declaration_seq_opt {$$ = $1;}
	;

/*
namespace_alias:
	identifier {$$ = $1;}
	;
*/

namespace_alias_definition:
	NAMESPACE identifier '=' qualified_namespace_specifier ';' {$$ = create_tree("namespace_alias_definition_666", 665, 5, $1, $2, $3, $4, $5);}
	;


qualified_namespace_specifier:
	COLONCOLON nested_name_specifier namespace_name {$$ = create_tree("qualified_namespace_specifier_671", 670, 3, $1, $2, $3);}
	| COLONCOLON namespace_name {$$ = create_tree("qualified_namespace_specifier_672", 670, 2, $1, $2);}
	| nested_name_specifier namespace_name {$$ = create_tree("qualified_namespace_specifier_673", 670, 2, $1, $2);}
	| namespace_name {$$ = $1;}
	;

using_declaration:
	USING TYPENAME COLONCOLON nested_name_specifier unqualified_id ';' {$$ = create_tree("using_declaration_678", 677, 6, $1, $2, $3, $4, $5, $6);}
	| USING TYPENAME nested_name_specifier unqualified_id ';' {$$ = create_tree("using_declaration_679", 677, 5, $1, $2, $3, $4, $5);}
	| USING COLONCOLON nested_name_specifier unqualified_id ';' {$$ = create_tree("using_declaration_680", 677, 5, $1, $2, $3, $4, $5);}
	| USING nested_name_specifier unqualified_id ';' {$$ = create_tree("using_declaration_681", 677, 4, $1, $2, $3, $4);}
	| USING COLONCOLON unqualified_id ';' {$$ = create_tree("using_declaration_682", 677, 4, $1, $2, $3, $4);}
	;

using_directive:
	USING NAMESPACE COLONCOLON nested_name_specifier namespace_name ';' {$$ = create_tree("using_directive_686", 685, 6, $1, $2, $3, $4, $5, $6);}
	| USING NAMESPACE COLONCOLON namespace_name ';' {$$ = create_tree("using_directive_687", 685, 5, $1, $2, $3, $4, $5);}
	| USING NAMESPACE nested_name_specifier namespace_name ';' {$$ = create_tree("using_directive_688", 685, 5, $1, $2, $3, $4, $5);}
	| USING NAMESPACE namespace_name ';' {$$ = create_tree("using_directive_689", 685, 4, $1, $2, $3, $4);}
	;


asm_definition:
	ASM '(' string_literal ')' ';' {$$ = create_tree("asm_definition_694", 693, 5, $1, $2, $3, $4, $5);}
	;

linkage_specification:
	EXTERN string_literal '{' declaration_seq_opt '}' {$$ = create_tree("linkage_specification_698", 697, 5, $1, $2, $3, $4, $5);}
	| EXTERN string_literal declaration {$$ = create_tree("linkage_specification_699", 697, 3, $1, $2, $3);}
	;

/*----------------------------------------------------------------------
 * Declarators.
 *----------------------------------------------------------------------*/

init_declarator_list:
	init_declarator {$$ = $1;}
	| init_declarator_list ',' init_declarator {$$ = create_tree("init_declarator_list_708", 706, 2, $1 , $3);}
	;

init_declarator:
	declarator initializer_opt {$$ = create_tree("init_declarator_712", 711, 2, $1, $2);}
	;

declarator:
        direct_declarator {$$ = $1;}
	| ptr_operator declarator {$$ = create_tree("declarator_717", 715, 2, $1, $2);}
	;

direct_declarator:
        declarator_id { $$ = $1; }
	| direct_declarator '('parameter_declaration_clause ')' cv_qualifier_seq exception_specification {$$ = create_tree("direct_declarator_722", 720, 5, $1, $2, $3, $4, $5);}
	| direct_declarator '('parameter_declaration_clause ')' cv_qualifier_seq {$$ = create_tree("direct_declarator_723", 720, 4, $1, $2, $3, $4);}
	| direct_declarator '('parameter_declaration_clause ')' exception_specification {$$ = create_tree("direct_declarator_724", 720, 4, $1, $2, $3, $4);}
        | direct_declarator '('parameter_declaration_clause ')' {$$ = create_tree("direct_declarator_725", 720, 2, $1 , $3 );}
	| CLASS_NAME '('parameter_declaration_clause ')' {$$ = create_tree("direct_declarator_726", 720, 3, $1, $2, $3);}
	| CLASS_NAME COLONCOLON declarator_id '('parameter_declaration_clause ')' {$$ = create_tree("direct_declarator_727", 720, 3, $1, $3, $5);}
	| CLASS_NAME COLONCOLON CLASS_NAME '('parameter_declaration_clause ')' {$$ = create_tree("direct_declarator_728", 720, 5, $1, $2, $3, $4, $5);}
	| direct_declarator '[' constant_expression_opt ']' {$$ = create_tree("direct_declarator_729", 720, 4, $1, $2, $3, $4);}
	| '(' declarator ')' {$$ = create_tree("direct_declarator_730", 720, 3, $1, $2, $3);}
	;

ptr_operator:
	'*' { $$ = $1; }
//	| '*' cv_qualifier_seq {$$ = create_tree("ptr_operator_735", 733, 2, $1, $2);}
	| '&' { $$ = $1; }
	| nested_name_specifier '*' {$$ = create_tree("ptr_operator_737", 733, 2, $1, $2);}
//	| nested_name_specifier '*' cv_qualifier_seq {$$ = create_tree("ptr_operator_738", 733, 3, $1, $2, $3);}
	| COLONCOLON nested_name_specifier '*' {$$ = create_tree("ptr_operator_739", 733, 3, $1, $2, $3);}
//	| COLONCOLON nested_name_specifier '*' cv_qualifier_seq {$$ = create_tree("ptr_operator_740", 733, 4, $1, $2, $3, $4);}
	;

cv_qualifier_seq:
	cv_qualifier {$$ = $1;}
	| cv_qualifier cv_qualifier_seq {$$ = create_tree("cv_qualifier_seq_745", 743, 2, $1, $2);}
	;

cv_qualifier:
	CONST {$$ = $1;}
	| VOLATILE {$$ = $1;}
	;

declarator_id:
	id_expression {$$ = $1;}
	| COLONCOLON id_expression {$$ = create_tree("declarator_id_755", 753, 2, $1, $2);}
	| COLONCOLON nested_name_specifier type_name {$$ = create_tree("declarator_id_756", 753, 3, $1, $2, $3);}
	| COLONCOLON type_name {$$ = create_tree("declarator_id_757", 753, 2, $1, $2);}
	;

type_id:
	type_specifier_seq abstract_declarator_opt {$$ = create_tree("type_id_761", 760, 2, $1, $2);}
	;

type_specifier_seq:
	type_specifier type_specifier_seq_opt {$$ = create_tree("type_specifier_seq_765", 764, 2, $1, $2);}
	;

abstract_declarator:
	ptr_operator abstract_declarator_opt {$$ = create_tree("abstract_declarator_769", 768, 2, $1, $2);}
	| direct_abstract_declarator {$$ = $1;}
	;

direct_abstract_declarator:
	direct_abstract_declarator_opt '(' parameter_declaration_clause ')' cv_qualifier_seq exception_specification {$$ = create_tree("direct_abstract_declarator_774", 773, 6, $1, $2, $3, $4, $5, $6);}
	| direct_abstract_declarator_opt '(' parameter_declaration_clause ')' cv_qualifier_seq {$$ = create_tree("direct_abstract_declarator_775", 773, 5, $1, $2, $3, $4, $5);}
	| direct_abstract_declarator_opt '(' parameter_declaration_clause ')' exception_specification {$$ = create_tree("direct_abstract_declarator_776", 773, 5, $1, $2, $3, $4, $5);}
	| direct_abstract_declarator_opt '(' parameter_declaration_clause ')' {$$ = create_tree("direct_abstract_declarator_777", 773, 4, $1, $2, $3, $4);}
	| direct_abstract_declarator_opt '[' constant_expression_opt ']' {$$ = create_tree("direct_abstract_declarator_778", 773, 4, $1, $2, $3, $4);}
	| '(' abstract_declarator ')' {$$ = create_tree("direct_abstract_declarator_779", 773, 3, $1, $2, $3);}
	;

parameter_declaration_clause:
        parameter_declaration_list { $$ = $1; }
        | parameter_declaration_list ELLIPSIS { $$ = NULL; }
	| ELLIPSIS {$$ = NULL;}
        | parameter_declaration_list ',' ELLIPSIS { $$ = NULL; }
	| { $$ = NULL; } 
	;

parameter_declaration_list:
	parameter_declaration {$$ = $1;}
	| parameter_declaration_list ',' parameter_declaration {$$ = create_tree("parameter_declaration_list_792", 790, 2, $1 , $3);}
	;

parameter_declaration:
	decl_specifier_seq declarator {$$ = create_tree("parameter_declaration_796", 795, 2, $1, $2);}
	| decl_specifier_seq declarator '=' assignment_expression {$$ = create_tree("parameter_declaration_797", 795, 4, $1, $2, $3, $4);}
	| decl_specifier_seq abstract_declarator_opt {$$ = create_tree("parameter_declaration_798", 795, 2, $1, $2);}
	| decl_specifier_seq abstract_declarator_opt '=' assignment_expression {$$ = create_tree("parameter_declaration_799", 795, 4, $1, $2, $3, $4);}
	;

function_definition:
	declarator ctor_initializer_opt function_body {$$ = create_tree("function_definition_803", 802, 3, $1, $2, $3);}
        | decl_specifier_seq declarator ctor_initializer_opt function_body {$$ = create_tree("function_definition_804", 802, 4, $1, $2, $3, $4);}
	| declarator function_try_block {$$ = create_tree("function_definition_805", 802, 2, $1, $2);}
	| decl_specifier_seq declarator function_try_block {$$ = create_tree("function_definition_806", 802, 3, $1, $2, $3);}
	;

function_body:
	compound_statement { $$ = $1; }
	;

initializer:
	'=' initializer_clause {$$ = create_tree("initializer_814", 813, 2, $1, $2);}
	| '(' expression_list ')' {$$ = create_tree("initializer_815", 813, 3, $1, $2, $3);}
	;

initializer_clause:
	assignment_expression {$$ = $1;}
	| '{' initializer_list COMMA_opt '}' {$$ = create_tree("initializer_clause_820", 818, 3, $1, $2, $3);}
	| '{' '}' {$$ = create_tree("initializer_clause_821", 818, 2, $1, $2);}
	;

initializer_list:
	initializer_clause {$$ = $1;}
	| initializer_list ',' initializer_clause {$$ = create_tree("initializer_list_826", 824, 3, $1, $2, $3);}
	;

/*----------------------------------------------------------------------
 * Classes.
 *----------------------------------------------------------------------*/

class_specifier:
        class_head '{' member_specification_opt '}' { $$ = create_tree("class_specifier_832", 831, 4, $1, $2, $3 , $4); }
	;

class_head:
        /* Previously was a hashtable implementation; will reimplement */
        class_key identifier { $$ = create_tree("class_head_836", 837, 2, $1, $2);  insert_typename_tree($2, CLASS_NAME);  } 
	| class_key nested_name_specifier identifier { $$ = create_tree("class_head_840", 837, 3, $1, $2, $3); insert_typename_tree($3, CLASS_NAME); }
  
	;

class_key:
        CLASS {$$ = $1;} 
	| STRUCT {$$ = $1;}
	| UNION {$$ = $1;}
	;

member_specification:
	member_declaration member_specification_opt {$$ = create_tree("member_specification_851", 850, 2, $1, $2);}
	| access_specifier ':' member_specification_opt {$$ = create_tree("member_specification_852", 850, 3, $1, $2, $3);}
	;

member_declaration:
	decl_specifier_seq member_declarator_list ';' {$$ = create_tree("member_declaration_856", 855, 2, $1, $2);}
        | decl_specifier_seq ';' { $$ = $1; }
        | member_declarator_list ';' { $$ = $1; }
	| ';' { $$ = NULL; }
        | function_definition SEMICOLON_opt { $$ = $1; }
        | qualified_id ';' { $$ = $1; };
        | using_declaration { $$ = $1; }
	;

member_declarator_list:
        member_declarator { $$ = $1; }
	| member_declarator_list ',' member_declarator {$$ = create_tree("member_declarator_list_867", 865, 2, $1 , $3);}
	;

member_declarator:
	declarator { $$ = $1; }
	| declarator pure_specifier {$$ = create_tree("member_declarator_872", 870, 2, $1, $2);}
	| declarator constant_initializer {$$ = create_tree("member_declarator_873", 870, 2, $1, $2);}
	| identifier ':' constant_expression {$$ = create_tree("member_declarator_874", 870, 2, $1, $3);}
	;

/*
 * This rule need a hack for working around the ``= 0'' pure specifier.
 * 0 is returned as an ``INTEGER'' by the lexical analyzer but in this
 * context is different.
 */

pure_specifier:
/*        '=' '0' {$$ = create_tree("pure_specifier_883", 882, 2, $1, $2);}*/
        '=' '0' {$$ = NULL; }
	;

constant_initializer:
	'=' constant_expression {$$ = create_tree("constant_initializer_887", 886, 2, $1, $2);}
	;

/*----------------------------------------------------------------------
 * Derived classes.
 *----------------------------------------------------------------------*/


access_specifier:
	PRIVATE {$$ = $1;}
	| PROTECTED {$$ = $1;}
	| PUBLIC {$$ = $1;}
	;

/*----------------------------------------------------------------------
 * Special member functions.
 *----------------------------------------------------------------------*/

conversion_function_id:
	OPERATOR conversion_type_id {$$ = create_tree("conversion_function_id_929", 928, 2, $1, $2);}
	;

conversion_type_id:
	type_specifier_seq conversion_declarator_opt {$$ = create_tree("conversion_type_id_933", 932, 2, $1, $2);}
	;

conversion_declarator:
	ptr_operator conversion_declarator_opt {$$ = create_tree("conversion_declarator_937", 936, 2, $1, $2);}
	;

ctor_initializer:
	':' mem_initializer_list {$$ = create_tree("ctor_initializer_941", 940, 2, $1, $2);}
	;

mem_initializer_list:
	mem_initializer {$$ = $1;}
	| mem_initializer ',' mem_initializer_list {$$ = create_tree("mem_initializer_list_946", 944, 3, $1, $2, $3);}
	;

mem_initializer:
	mem_initializer_id '(' expression_list_opt ')' {$$ = create_tree("mem_initializer_950", 949, 4, $1, $2, $3, $4);}
	;

mem_initializer_id:
	COLONCOLON nested_name_specifier class_name {$$ = create_tree("mem_initializer_id_954", 953, 3, $1, $2, $3);}
	| COLONCOLON class_name {$$ = create_tree("mem_initializer_id_955", 953, 2, $1, $2);}
	| nested_name_specifier class_name {$$ = create_tree("mem_initializer_id_956", 953, 2, $1, $2);}
	| class_name {$$ = $1;}
	| identifier {$$ = $1;}
	;

/*----------------------------------------------------------------------
 * Overloading.
 *----------------------------------------------------------------------*/

operator_function_id:
	OPERATOR operator {$$ = create_tree("operator_function_id_966", 965, 2, $1, $2);}
	;

operator:
	NEW {$$ = $1;}
	| DELETE {$$ = $1;}
	| NEW '[' ']' {$$ = create_tree("operator_972", 969, 3, $1, $2, $3);}
	| DELETE '[' ']' {$$ = create_tree("operator_973", 969, 3, $1, $2, $3);}
	| '+' {$$ = $1;}
	| '_' {$$ = $1;}
	| '*' {$$ = $1;}
	| '/' {$$ = $1;}
	| '%' {$$ = $1;}
	| '^' {$$ = $1;}
	| '&' {$$ = $1;}
	| '|' {$$ = $1;}
	| '~' {$$ = $1;}
	| '!' {$$ = $1;}
	| '=' {$$ = $1;}
	| '<' {$$ = $1;}
	| '>' {$$ = $1;}
	| ADDEQ {$$ = $1;}
	| SUBEQ {$$ = $1;}
	| MULEQ {$$ = $1;}
	| DIVEQ {$$ = $1;}
	| MODEQ {$$ = $1;}
	| XOREQ {$$ = $1;}
	| ANDEQ {$$ = $1;}
	| OREQ {$$ = $1;}
	| SL {$$ = $1;}
	| SR {$$ = $1;}
	| SREQ {$$ = $1;}
	| SLEQ {$$ = $1;}
	| EQ {$$ = $1;}
	| NOTEQ {$$ = $1;}
	| LTEQ {$$ = $1;}
	| GTEQ {$$ = $1;}
	| ANDAND {$$ = $1;}
	| OROR {$$ = $1;}
	| PLUSPLUS {$$ = $1;}
	| MINUSMINUS {$$ = $1;}
	| ',' {$$ = $1;}
	| ARROWSTAR {$$ = $1;}
	| ARROW {$$ = $1;}
	| '(' ')' {$$ = create_tree("operator_1010", 969, 2, $1, $2);}
	| '[' ']' {$$ = create_tree("operator_1011", 969, 2, $1, $2);}
	;

/*----------------------------------------------------------------------
 * Templates.
 *----------------------------------------------------------------------*/
/*
template_declaration:
	EXPORT_opt TEMPLATE '<' template_parameter_list '>' declaration {$$ = create_tree("template_declaration_1019", 1018, 6, $1, $2, $3, $4, $5, $6);}
	;

template_parameter_list:
	template_parameter {$$ = $1;}
	| template_parameter_list ',' template_parameter {$$ = create_tree("template_parameter_list_1024", 1022, 3, $1, $2, $3);}
	;

template_parameter:
	type_parameter {$$ = $1;}
	| parameter_declaration {$$ = $1;}
	;
type_parameter:
	CLASS identifier {$$ = create_tree("type_parameter_1032", 1031, 2, $1, $2);}
	| CLASS identifier '=' type_id {$$ = create_tree("type_parameter_1033", 1031, 4, $1, $2, $3, $4);}
	| TYPENAME identifier {$$ = create_tree("type_parameter_1034", 1031, 2, $1, $2);}
	| TYPENAME identifier '=' type_id {$$ = create_tree("type_parameter_1035", 1031, 4, $1, $2, $3, $4);}
	| TEMPLATE '<' template_parameter_list '>' CLASS identifier {$$ = create_tree("type_parameter_1036", 1031, 6, $1, $2, $3, $4, $5, $6);}
	| TEMPLATE '<' template_parameter_list '>' CLASS identifier '=' template_name {$$ = create_tree("type_parameter_1037", 1031, 8, $1, $2, $3, $4, $5, $6, $7, $8);}
	;

template_id:
	template_name '<' template_argument_list '>' {$$ = create_tree("template_id_1041", 1040, 4, $1, $2, $3, $4);}
	;

template_argument_list:
	template_argument {$$ = $1;}
	| template_argument_list ',' template_argument {$$ = create_tree("template_argument_list_1046", 1044, 3, $1, $2, $3);}
	;

template_argument:
	assignment_expression {$$ = $1;}
	| type_id {$$ = $1;}
	| template_name {$$ = $1;}
	;

explicit_instantiation:
	TEMPLATE declaration {$$ = create_tree("explicit_instantiation_1056", 1055, 2, $1, $2);}
	;

explicit_specialization:
	TEMPLATE '<' '>' declaration {$$ = create_tree("explicit_specialization_1060", 1059, 4, $1, $2, $3, $4);}
	;
*/
/*----------------------------------------------------------------------
 * Exception handling.
 *----------------------------------------------------------------------*/

try_block:
	TRY compound_statement handler_seq {$$ = create_tree("try_block_1068", 1067, 3, $1, $2, $3);}
	;

function_try_block:
	TRY ctor_initializer_opt function_body handler_seq {$$ = create_tree("function_try_block_1072", 1071, 4, $1, $2, $3, $4);}
	;

handler_seq:
	handler handler_seq_opt {$$ = create_tree("handler_seq_1076", 1075, 2, $1, $2);}
	;

handler:
	CATCH '(' exception_declaration ')' compound_statement {$$ = create_tree("handler_1080", 1079, 5, $1, $2, $3, $4, $5);}
	;

exception_declaration:
	type_specifier_seq declarator {$$ = create_tree("exception_declaration_1084", 1083, 2, $1, $2);}
	| type_specifier_seq abstract_declarator {$$ = create_tree("exception_declaration_1085", 1083, 2, $1, $2);}
	| type_specifier_seq {$$ = $1;}
	| ELLIPSIS {$$ = $1;}
	;

throw_expression:
	THROW assignment_expression_opt {$$ = create_tree("throw_expression_1091", 1090, 2, $1, $2);}
	;

exception_specification:
	THROW '(' type_id_list_opt ')' {$$ = create_tree("exception_specification_1095", 1094, 4, $1, $2, $3, $4);}
	;

type_id_list:
	type_id {$$ = $1;}
	| type_id_list ',' type_id {$$ = create_tree("type_id_list_1100", 1098, 3, $1, $2, $3);}
	;

/*----------------------------------------------------------------------
 * Epsilon (optional) definitions.
 *----------------------------------------------------------------------*/

declaration_seq_opt:
	{$$ = NULL;}
	| declaration_seq {$$ = $1;}
	;

nested_name_specifier_opt:
	{$$ = NULL;}
	| nested_name_specifier {$$ = $1;}
	;

expression_list_opt:
	{$$ = NULL;}
	| expression_list {$$ = $1;}
	;

COLONCOLON_opt:
	{$$ = NULL;}
	| COLONCOLON {$$ = $1;}
	;

new_placement_opt:
	{$$ = NULL;}
	| new_placement {$$ = $1;}
	;

new_initializer_opt:
	{$$ = NULL;}
	| new_initializer {$$ = $1;}
	;

new_declarator_opt:
	{$$ = NULL;}
	| new_declarator {$$ = $1;}
	;

expression_opt:
	{$$ = NULL;}
	| expression {$$ = $1;}
	;

statement_seq_opt:
	{$$ = NULL;}
	| statement_seq {$$ = $1;}
	;

condition_opt:
	{$$ = NULL;}
	| condition {$$ = $1;}
	;

enumerator_list_opt:
	{$$ = NULL;}
	| enumerator_list {$$ = $1;}
	;

initializer_opt:
	{$$ = NULL;}
	| initializer {$$ = $1;}
	;

constant_expression_opt:
	{$$ = NULL;}
	| constant_expression {$$ = $1;}
	;

abstract_declarator_opt:
	{$$ = NULL;}
	| abstract_declarator {$$ = $1;}
	;

type_specifier_seq_opt:
	{$$ = NULL;}
	| type_specifier_seq {$$ = $1;}
	;

direct_abstract_declarator_opt:
	{$$ = NULL;}
	| direct_abstract_declarator {$$ = $1;}
	;

ctor_initializer_opt:
	{$$ = NULL;}
	| ctor_initializer {$$ = $1;}
	;

COMMA_opt:
	{$$ = NULL;}
	| ',' {$$ = NULL;}
	;

member_specification_opt:
	{$$ = NULL;}
	| member_specification {$$ = $1;}
	;

SEMICOLON_opt:
	{$$ = NULL;}
	| ';' {$$ = NULL;}
	;

conversion_declarator_opt:
	{$$ = NULL;}
	| conversion_declarator {$$ = $1;}
	;
/*
EXPORT_opt:
        EXPORT {$$ = $1;}
	| {$$ = NULL;}
	;
*/

handler_seq_opt:
	{$$ = NULL;}
	| handler_seq {$$ = $1;}
	;

assignment_expression_opt:
	{$$ = NULL;}
	| assignment_expression {$$ = $1;}
	;

type_id_list_opt:
	{$$ = NULL;}
	| type_id_list {$$ = $1;}
	;

%%

static void yyerror(char *s) {
	fprintf(stderr, "line %d: %s\n", yylineno, s);
	fprintf(stderr, "%s\n", yytext);
}
