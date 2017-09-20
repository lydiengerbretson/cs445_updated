/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_120GRAM_LYDIA_TAB_H_INCLUDED
# define YY_YY_120GRAM_LYDIA_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENTIFIER = 258,
    INTEGER = 259,
    FLOATING = 260,
    CHARACTER = 261,
    STRING = 262,
    TYPEDEF_NAME = 263,
    CLASS_NAME = 264,
    ENUM_NAME = 265,
    ELLIPSIS = 266,
    COLONCOLON = 267,
    DOTSTAR = 268,
    ADDEQ = 269,
    SUBEQ = 270,
    MULEQ = 271,
    DIVEQ = 272,
    MODEQ = 273,
    XOREQ = 274,
    ANDEQ = 275,
    OREQ = 276,
    SL = 277,
    SR = 278,
    SREQ = 279,
    SLEQ = 280,
    EQ = 281,
    NOTEQ = 282,
    LTEQ = 283,
    GTEQ = 284,
    ANDAND = 285,
    OROR = 286,
    PLUSPLUS = 287,
    MINUSMINUS = 288,
    ARROWSTAR = 289,
    ARROW = 290,
    ASM = 291,
    AUTO = 292,
    BOOL = 293,
    BREAK = 294,
    CASE = 295,
    CATCH = 296,
    CHAR = 297,
    CLASS = 298,
    CONST = 299,
    CONST_CAST = 300,
    CONTINUE = 301,
    DEFAULT = 302,
    DELETE = 303,
    DO = 304,
    DOUBLE = 305,
    DYNAMIC_CAST = 306,
    ELSE = 307,
    ENUM = 308,
    EXPLICIT = 309,
    EXPORT = 310,
    EXTERN = 311,
    FALSE = 312,
    FLOAT = 313,
    FOR = 314,
    FRIEND = 315,
    GOTO = 316,
    IF = 317,
    INLINE = 318,
    INT = 319,
    LONG = 320,
    MUTABLE = 321,
    NAMESPACE = 322,
    NEW = 323,
    OPERATOR = 324,
    PRIVATE = 325,
    PROTECTED = 326,
    PUBLIC = 327,
    REGISTER = 328,
    REINTERPRET_CAST = 329,
    RETURN = 330,
    SHORT = 331,
    SIGNED = 332,
    SIZEOF = 333,
    STATIC = 334,
    STATIC_CAST = 335,
    STRUCT = 336,
    SWITCH = 337,
    TEMPLATE = 338,
    THIS = 339,
    THROW = 340,
    TRUE = 341,
    TRY = 342,
    TYPEDEF = 343,
    TYPEID = 344,
    TYPENAME = 345,
    UNION = 346,
    UNSIGNED = 347,
    USING = 348,
    VIRTUAL = 349,
    VOID = 350,
    VOLATILE = 351,
    WCHAR_T = 352,
    WHILE = 353,
    NAMESPACE_NAME = 354,
    TEMPLATE_NAME = 355
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 80 "120gram_lydia.y" /* yacc.c:1909  */

  struct tree *t;

#line 159 "120gram_lydia.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_120GRAM_LYDIA_TAB_H_INCLUDED  */
