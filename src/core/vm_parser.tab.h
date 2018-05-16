/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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

#ifndef YY_VM_PARSER_VM_PARSER_TAB_H_INCLUDED
# define YY_VM_PARSER_VM_PARSER_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef VM_PARSER_DEBUG
# if defined YYDEBUG
#  if YYDEBUG
#   define VM_PARSER_DEBUG 1
#  else
#   define VM_PARSER_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define VM_PARSER_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined VM_PARSER_DEBUG */
#if VM_PARSER_DEBUG
extern int vm_parser_debug;
#endif

/* Tokens.  */
#ifndef VM_PARSER_TOKENTYPE
# define VM_PARSER_TOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum vm_parser_tokentype {
     NAME = 258,
     NUMBER = 259,
     DECIMAL = 260,
     STRING = 261,
     CONDITION = 262,
     CONDITIONAL_OPERAND = 263,
     FUNCTION = 264,
     RETURN = 265,
     IF = 266,
     WHILE = 267,
     DOEVENTS = 268,
     UMINUS = 269
   };
#endif


#if ! defined VM_PARSER_STYPE && ! defined VM_PARSER_STYPE_IS_DECLARED
typedef union VM_PARSER_STYPE
{
/* Line 2058 of yacc.c  */
#line 44 "vm_parser.y"

	int number;
	double decimal;
	char* string;


/* Line 2058 of yacc.c  */
#line 86 "vm_parser.tab.h"
} VM_PARSER_STYPE;
# define VM_PARSER_STYPE_IS_TRIVIAL 1
# define vm_parser_stype VM_PARSER_STYPE /* obsolescent; will be withdrawn */
# define VM_PARSER_STYPE_IS_DECLARED 1
#endif

extern VM_PARSER_STYPE vm_parser_lval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int vm_parser_parse (void *YYPARSE_PARAM);
#else
int vm_parser_parse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int vm_parser_parse (void);
#else
int vm_parser_parse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_VM_PARSER_VM_PARSER_TAB_H_INCLUDED  */
