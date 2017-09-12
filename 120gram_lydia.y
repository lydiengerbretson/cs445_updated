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
#include "120tree_lydia.h"

Treeptr YYPROGRAM; 
extern int yylineno;
extern char *yytext;

// insert struct tree * to start of program

int yydebug=0;

static void yyerror(char *s);


/*
 * warning, this %union is not what you need, it is for demonstration purposes.
 */
%}

%union {
  struct tree *t;
  }

%type <t> typedef_name class_name
%type <t> template_name identifier literal integer_literal
%type <t> character_literal floating_literal string_literal boolean_literal
%type <t> translation_unit primary_expression id_expression unqualified_id
%type <t> qualified_id nested_name_specifier postfix_expression expression_list
%type <t> unary_expression unary_operator new_expression new_placement new_type_id
%type <t> new_declarator direct_new_declarator new_initializer delete_expression
%type <t> cast_expression pm_expression multiplicative_expression additive_expression
%type <t> shift_expression relational_expression equality_expression and_expression
%type <t> exclusive_or_expression inclusive_or_expression logical_and_expression 
%type <t> logical_or_expression conditional_expression assignment_expression
%type <t> assignment_operator expression constant_expression statement
%type <t> labeled_statement expression_statement compound_statement statement_seq
%type <t> selection_statement condition iteration_statement for_init_statement 
%type <t> jump_statement declaration_statement declaration_seq declaration
%type <t> block_declaration simple_declaration decl_specifier decl_specifier_seq
%type <t> storage_class_specifier function_specifier type_specifier simple_type_specifier
%type <t> type_name elaborated_type_specifier enum_name enum_specifier enumerator_list
%type <t> enumerator_definition enumerator 
%type <t> namespace_definition named_namespace_definition original_namespace_definition
%type <t> unnamed_namespace_definition namespace_body namespace_alias namespace_alias_definition
%type <t> qualified_namespace_specifier using_declaration using_directive asm_definition
%type <t> linkage_specification init_declarator_list init_declarator declarator 
%type <t> direct_declarator ptr_operator cv_qualifier_seq cv_qualifier declarator_id
%type <t> type_id type_specifier_seq abstract_declarator direct_abstract_declarator 
%type <t> parameter_declaration_clause parameter_declaration_list parameter_declaration
%type <t> function_definition function_body initializer initializer_clause initializer_list
%type <t> class_specifier class_head class_key member_specification member_declaration
%type <t> member_declarator_list member_declarator pure_specifier constant_initializer
%type <t> base_clause base_specifier_list base_specifier access_specifier
%type <t> conversion_function_id conversion_type_id conversion_declarator ctor_initializer
%type <t> mem_initializer_list mem_initializer mem_initializer_id operator_function_id
%type <t> operator template_declaration template_parameter_list template_parameter
%type <t> type_parameter template_id template_argument_list template_argument
%type <t> explicit_instantiation explicit_specialization try_block function_try_block
%type <t> handler_seq handler exception_declaration throw_expression exception_specification
%type <t> type_id_list declaration_seq_opt expression_list_opt
%type <t> COLONCOLON_opt new_placement_opt new_initializer_opt new_declarator_opt
%type <t> expression_opt statement_seq_opt condition_opt enumerator_list_opt
%type <t> initializer_opt constant_expression_opt abstract_declarator_opt type_specifier_seq_opt
%type <t> direct_abstract_declarator_opt ctor_initializer_opt COMMA_opt member_specification_opt
%type <t> SEMICOLON_opt conversion_declarator_opt EXPORT_opt handler_seq_opt 
%type <t> assignment_expression_opt type_id_list_opt
%type <t> namespace_name original_namespace_name
 


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
%token <t> '+' '*' '/' '%' '<' '>' '^' '|' '?'

%start translation_unit

%%

/*----------------------------------------------------------------------
 * Context-dependent identifiers.
 *----------------------------------------------------------------------*/

typedef_name:
	/* identifier */
	TYPEDEF_NAME {$$ = $1;}
	;

namespace_name:
	original_namespace_name {$$ = $1;}
	;

original_namespace_name:
	/* identifier */
	NAMESPACE_NAME {$$ = $1;}
	;

class_name:
	/* identifier */
	CLASS_NAME {$$ = $1;}
	| template_id /* to do: unsupport error */
	;

enum_name:
	/* identifier */
	ENUM_NAME { $$ = $1; }
	;

template_name:
	/* identifier */
	TEMPLATE_NAME /* to do: unsupport error */
	;

/*----------------------------------------------------------------------
 * Lexical elements.
 *----------------------------------------------------------------------*/

identifier:
	IDENTIFIER { $$ = $1; }
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
	declaration_seq_opt {$$ = create_tree("program", 1, $1); YYPROGRAM = $$;}
	;

/*----------------------------------------------------------------------
 * Expressions.
 *----------------------------------------------------------------------*/

primary_expression:
	literal {$$ = $1;}
	| THIS {$$ = $1;}
	| '(' expression ')' { $$ = create_tree("primary_expression", 3, $1, $2, $3); }
	| id_expression {$$ = $1;}
	;

id_expression:
	unqualified_id {$$ = $1;}
	| qualified_id {$$ = $1;}
	;

unqualified_id:
	identifier {$$ = $1;}
	| operator_function_id {$$ = $1;}
	| conversion_function_id {$$ = $1;}
	| '~' class_name {$$ = create_tree("unqualified_id", 2, $1, $2);}
	;

qualified_id:
	nested_name_specifier unqualified_id { $$ = create_tree("qualified_id", 2, $1, $2);}
	| nested_name_specifier TEMPLATE unqualified_id /* to do: unsupport error */
	;

nested_name_specifier:
	class_name COLONCOLON nested_name_specifier {$$ = create_tree("nested_name_specifier_1", 3, $1, $2, $3);}
	| namespace_name COLONCOLON nested_name_specifier {$$ = create_tree("nested_name_specifier_2", 3, $1, $2, $3);}
	| class_name COLONCOLON {$$ = create_tree("nested_name_specifier_3", 2, $1, $2);}

	;

postfix_expression:
	primary_expression
	| postfix_expression '[' expression ']'
	| postfix_expression '(' expression_list_opt ')'
	| DOUBLE '(' expression_list_opt ')'
	| INT '(' expression_list_opt ')'
	| CHAR '(' expression_list_opt ')'
	| BOOL '(' expression_list_opt ')'
	| postfix_expression '.' TEMPLATE COLONCOLON id_expression /* to do: unsupport error */
	| postfix_expression '.' TEMPLATE id_expression
	| postfix_expression '.' COLONCOLON id_expression
	| postfix_expression '.' id_expression
	| postfix_expression ARROW TEMPLATE COLONCOLON id_expression
	| postfix_expression ARROW TEMPLATE id_expression
	| postfix_expression ARROW COLONCOLON id_expression
	| postfix_expression ARROW id_expression
	| postfix_expression PLUSPLUS
	| postfix_expression MINUSMINUS
	| DYNAMIC_CAST '<' type_id '>' '(' expression ')'
	| STATIC_CAST '<' type_id '>' '(' expression ')'
	| REINTERPRET_CAST '<' type_id '>' '(' expression ')'
	| CONST_CAST '<' type_id '>' '(' expression ')'
	| TYPEID '(' expression ')'
	| TYPEID '(' type_id ')'
	;

expression_list:
	assignment_expression
	| expression_list ',' assignment_expression
	;

unary_expression:
	postfix_expression
	| PLUSPLUS cast_expression
	| MINUSMINUS cast_expression
	| '*' cast_expression
	| '&' cast_expression
	| unary_operator cast_expression
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
	  DELETE cast_expression
	| COLONCOLON DELETE cast_expression
	| DELETE '[' ']' cast_expression
	| COLONCOLON DELETE '[' ']' cast_expression
	;

cast_expression:
	unary_expression
	| '(' type_id ')' cast_expression
	;

pm_expression:
	cast_expression
	| pm_expression DOTSTAR cast_expression
	| pm_expression ARROWSTAR cast_expression
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
	| throw_expression
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
	| try_block
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
	| template_declaration
	| explicit_instantiation
	| explicit_specialization
	| linkage_specification
	| namespace_definition
	;

block_declaration:
	simple_declaration
	| asm_definition
	| namespace_alias_definition
	| using_declaration
	| using_directive
	;

simple_declaration:
	  decl_specifier_seq init_declarator_list ';'
	|  decl_specifier_seq ';'
	;

decl_specifier:
	storage_class_specifier
	| type_specifier
	| function_specifier { $$ = NULL; }
	| FRIEND { $$ = NULL; }
	| TYPEDEF { $$ = NULL; }
	;

decl_specifier_seq:
	  decl_specifier
	| decl_specifier_seq decl_specifier
	;

storage_class_specifier:
	AUTO { $$ = NULL; }
	| REGISTER { $$ = NULL; }
	| STATIC { $$ = NULL; }
	| EXTERN { $$ = NULL; }
	| MUTABLE { $$ = NULL; }
	;

function_specifier:
	INLINE
	| VIRTUAL
	| EXPLICIT
	;

type_specifier:
	simple_type_specifier
	| class_specifier { $$ = NULL; }
	| enum_specifier { $$ = NULL; }
	| elaborated_type_specifier { $$ = NULL; }
	| cv_qualifier { $$ = NULL; }
	;

simple_type_specifier:
	  type_name { $$ = NULL; }
	| nested_name_specifier type_name { $$ = NULL; }
	| COLONCOLON nested_name_specifier_opt type_name { $$ = NULL; }
	| CHAR { $$ = NULL; }
	| WCHAR_T { $$ = NULL; }
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

type_name:
	class_name
	| enum_name
	| typedef_name
	;

elaborated_type_specifier:
	  class_key COLONCOLON nested_name_specifier identifier
	| class_key COLONCOLON identifier
	| ENUM COLONCOLON nested_name_specifier identifier
	| ENUM COLONCOLON identifier
	| ENUM nested_name_specifier identifier
	| TYPENAME COLONCOLON_opt nested_name_specifier identifier
	| TYPENAME COLONCOLON_opt nested_name_specifier identifier '<' template_argument_list '>'
	;

/*
enum_name:
	identifier
	;
*/

enum_specifier:
	ENUM identifier '{' enumerator_list_opt '}'
	;

enumerator_list:
	enumerator_definition
	| enumerator_list ',' enumerator_definition
	;

enumerator_definition:
	enumerator
	| enumerator '=' constant_expression
	;

enumerator:
	identifier
	;

/*
namespace_name:
	original_namespace_name
	| namespace_alias
	;

original_namespace_name:
	identifier
	;
*/

namespace_definition:
	named_namespace_definition
	| unnamed_namespace_definition
	;

named_namespace_definition:
	original_namespace_definition
	| extension_namespace_definition
	;

original_namespace_definition:
	NAMESPACE identifier '{' namespace_body '}'
	;

extension_namespace_definition:
	NAMESPACE original_namespace_name '{' namespace_body '}'
	;

unnamed_namespace_definition:
	NAMESPACE '{' namespace_body '}'
	;

namespace_body:
	declaration_seq_opt
	;

namespace_alias:
	identifier
	;

namespace_alias_definition:
	NAMESPACE identifier '=' qualified_namespace_specifier ';'
	;

qualified_namespace_specifier:
	  COLONCOLON nested_name_specifier namespace_name
	| COLONCOLON namespace_name
	| nested_name_specifier namespace_name
	| namespace_name
	;

using_declaration:
	  USING TYPENAME COLONCOLON nested_name_specifier unqualified_id ';'
	| USING TYPENAME nested_name_specifier unqualified_id ';'
	| USING COLONCOLON nested_name_specifier unqualified_id ';'
	| USING nested_name_specifier unqualified_id ';'
	| USING COLONCOLON unqualified_id ';'
	;

using_directive:
	USING NAMESPACE COLONCOLON nested_name_specifier namespace_name ';'
	| USING NAMESPACE COLONCOLON namespace_name ';'
	| USING NAMESPACE nested_name_specifier namespace_name ';'
	| USING NAMESPACE namespace_name ';'
	;

asm_definition:
	ASM '(' string_literal ')' ';'
	;

linkage_specification:
	EXTERN string_literal '{' declaration_seq_opt '}'
	| EXTERN string_literal declaration
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
	| direct_declarator '(' parameter_declaration_clause ')' cv_qualifier_seq exception_specification { $$ = NULL; }
	| direct_declarator '(' parameter_declaration_clause ')' cv_qualifier_seq { $$ = NULL; }
	| direct_declarator '(' parameter_declaration_clause ')' exception_specification { $$ = NULL; }
	| direct_declarator '(' parameter_declaration_clause ')' { $$ = create_tree("direct_declarator", 2, $1, $3); }
	| CLASS_NAME '(' parameter_declaration_clause ')' { $$ = NULL; }
	| CLASS_NAME COLONCOLON declarator_id '(' parameter_declaration_clause ')' { $$ = NULL; }
	| CLASS_NAME COLONCOLON CLASS_NAME '(' parameter_declaration_clause ')' { $$ = NULL; }
	| direct_declarator '[' constant_expression_opt ']' { $$ = NULL; }
	| '(' declarator ')' { $$ = NULL; }
	;

ptr_operator:
	'*' { $$ = NULL; }
	| '*' cv_qualifier_seq { $$ = NULL; }
	| '&' { $$ = NULL; }
	| nested_name_specifier '*' { $$ = NULL; }
	| nested_name_specifier '*' cv_qualifier_seq { $$ = NULL; }
	| COLONCOLON nested_name_specifier '*' { $$ = NULL; }
	| COLONCOLON nested_name_specifier '*' cv_qualifier_seq { $$ = NULL; }
	;

cv_qualifier_seq:
	cv_qualifier
	| cv_qualifier cv_qualifier_seq
	;

cv_qualifier:
	CONST
	| VOLATILE
	;

declarator_id:
	  id_expression
	| COLONCOLON id_expression
	| COLONCOLON nested_name_specifier type_name
	| COLONCOLON type_name
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
	  direct_abstract_declarator_opt '(' parameter_declaration_clause ')' cv_qualifier_seq exception_specification
	| direct_abstract_declarator_opt '(' parameter_declaration_clause ')' cv_qualifier_seq
	| direct_abstract_declarator_opt '(' parameter_declaration_clause ')' exception_specification
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
	| declarator function_try_block
	| decl_specifier_seq declarator function_try_block
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
	| class_key identifier base_clause
	| class_key nested_name_specifier identifier
	| class_key nested_name_specifier identifier base_clause
	;

class_key:
	CLASS
	| STRUCT
	| UNION
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
	| using_declaration
	| template_declaration
	;

member_declarator_list:
	member_declarator
	| member_declarator_list ',' member_declarator
	;

member_declarator:
	  declarator
	| declarator pure_specifier
	| declarator constant_initializer
	| identifier ':' constant_expression
	;

/*
 * This rule need a hack for working around the ``= 0'' pure specifier.
 * 0 is returned as an ``INTEGER'' by the lexical analyzer but in this
 * context is different.
 */
pure_specifier:
	'=' '0'
	;

constant_initializer:
	'=' constant_expression
	;

/*----------------------------------------------------------------------
 * Derived classes.
 *----------------------------------------------------------------------*/

base_clause:
	':' base_specifier_list
	;

base_specifier_list:
	base_specifier
	| base_specifier_list ',' base_specifier
	;

base_specifier:
	  COLONCOLON nested_name_specifier class_name
	| COLONCOLON class_name
	| nested_name_specifier class_name
	| class_name
	| VIRTUAL access_specifier COLONCOLON nested_name_specifier_opt class_name
	| VIRTUAL access_specifier nested_name_specifier_opt class_name
	| VIRTUAL COLONCOLON nested_name_specifier_opt class_name
	| VIRTUAL nested_name_specifier_opt class_name
	| access_specifier VIRTUAL COLONCOLON nested_name_specifier_opt class_name
	| access_specifier VIRTUAL nested_name_specifier_opt class_name
	| access_specifier COLONCOLON nested_name_specifier_opt class_name
	| access_specifier nested_name_specifier_opt class_name
	;

access_specifier:
	PRIVATE
	| PROTECTED
	| PUBLIC
	;

/*----------------------------------------------------------------------
 * Special member functions.
 *----------------------------------------------------------------------*/

conversion_function_id:
	OPERATOR conversion_type_id
	;

conversion_type_id:
	type_specifier_seq conversion_declarator_opt
	;

conversion_declarator:
	ptr_operator conversion_declarator_opt
	;

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
 * Overloading.
 *----------------------------------------------------------------------*/

operator_function_id:
	OPERATOR operator
	;

operator:
	NEW
	| DELETE
	| NEW '[' ']'
	| DELETE '[' ']'
	| '+'
	| '_'
	| '*'
	| '/'
	| '%'
	| '^'
	| '&'
	| '|'
	| '~'
	| '!'
	| '='
	| '<'
	| '>'
	| ADDEQ
	| SUBEQ
	| MULEQ
	| DIVEQ
	| MODEQ
	| XOREQ
	| ANDEQ
	| OREQ
	| SL
	| SR
	| SREQ
	| SLEQ
	| EQ
	| NOTEQ
	| LTEQ
	| GTEQ
	| ANDAND
	| OROR
	| PLUSPLUS
	| MINUSMINUS
	| ','
	| ARROWSTAR
	| ARROW
	| '(' ')'
	| '[' ']'
	;

/*----------------------------------------------------------------------
 * Templates.
 *----------------------------------------------------------------------*/

template_declaration:
	EXPORT_opt TEMPLATE '<' template_parameter_list '>' declaration
	;

template_parameter_list:
	template_parameter
	| template_parameter_list ',' template_parameter
	;

template_parameter:
	type_parameter
	| parameter_declaration
	;

type_parameter:
	  CLASS identifier
	| CLASS identifier '=' type_id
	| TYPENAME identifier
	| TYPENAME identifier '=' type_id
	| TEMPLATE '<' template_parameter_list '>' CLASS identifier
	| TEMPLATE '<' template_parameter_list '>' CLASS identifier '=' template_name
	;

template_id:
	template_name '<' template_argument_list '>'
	;

template_argument_list:
	template_argument
	| template_argument_list ',' template_argument
	;

template_argument:
	assignment_expression
	| type_id
	| template_name
	;

explicit_instantiation:
	TEMPLATE declaration
	;

explicit_specialization:
	TEMPLATE '<' '>' declaration
	;

/*----------------------------------------------------------------------
 * Exception handling.
 *----------------------------------------------------------------------*/

try_block:
	TRY compound_statement handler_seq
	;

function_try_block:
	TRY ctor_initializer_opt function_body handler_seq
	;

handler_seq:
	handler handler_seq_opt
	;

handler:
	CATCH '(' exception_declaration ')' compound_statement
	;

exception_declaration:
	type_specifier_seq declarator
	| type_specifier_seq abstract_declarator
	| type_specifier_seq
	| ELLIPSIS
	;

throw_expression:
	THROW assignment_expression_opt
	;

exception_specification:
	THROW '(' type_id_list_opt ')'
	;

type_id_list:
	type_id
	| type_id_list ',' type_id
	;

/*----------------------------------------------------------------------
 * Epsilon (optional) definitions.
 *----------------------------------------------------------------------*/

declaration_seq_opt:
	/* epsilon */
	| declaration_seq
	;

nested_name_specifier_opt:
	/* epsilon */
	| nested_name_specifier
	;

expression_list_opt:
	/* epsilon */
	| expression_list
	;

COLONCOLON_opt:
	/* epsilon */
	| COLONCOLON
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

enumerator_list_opt:
	/* epsilon */
	| enumerator_list
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

conversion_declarator_opt:
	/* epsilon */
	| conversion_declarator
	;

EXPORT_opt:
	/* epsilon */
	| EXPORT
	;

handler_seq_opt:
	/* epsilon */
	| handler_seq
	;

assignment_expression_opt:
	/* epsilon */
	| assignment_expression
	;

type_id_list_opt:
	/* epsilon */
	| type_id_list
	;

%%

static void yyerror(char *s)
{
	fprintf(stderr, "%d: %s\n", yylineno, s);
}