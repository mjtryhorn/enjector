/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         VM_PARSER_STYPE
/* Substitute the variable and function names.  */
#define yyparse         vm_parser_parse
#define yylex           vm_parser_lex
#define yyerror         vm_parser_error
#define yylval          vm_parser_lval
#define yychar          vm_parser_char
#define yydebug         vm_parser_debug
#define yynerrs         vm_parser_nerrs

/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 20 "vm_parser.y"

	#include <string.h>
	#include <stdio.h>
	
	#include <enjector/core/vm_parser.h>
	
	extern char yytext[];
		
	char* pszTempVar = 0;
	char* pszLabelName = 0;
	
	#define emit(opcode, p1, p2, p3, p4) vm_parser_function_emit(opcode, p1, p2, p3, p4)
	#define var_create					 vm_parser_generator_temp_var_create
	#define labels_pop					 vm_parser_generator_temp_labels_pop
	#define labels_push					 vm_parser_generator_temp_labels_push
	#define var_all_reset				 vm_parser_generator_temp_var_all_reset
	#define label_create				 vm_parser_generator_temp_label_create

/* Line 371 of yacc.c  */
#line 95 "vm_parser.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "vm_parser.tab.h".  */
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
/* Line 387 of yacc.c  */
#line 44 "vm_parser.y"

	int number;
	double decimal;
	char* string;


/* Line 387 of yacc.c  */
#line 167 "vm_parser.tab.c"
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

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 195 "vm_parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined VM_PARSER_STYPE_IS_TRIVIAL && VM_PARSER_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   175

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  30
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  67
/* YYNRULES -- Number of states.  */
#define YYNSTATES  119

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   269

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      21,    22,    16,    15,    23,    14,    26,    17,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    29,    25,
       2,    24,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    27,     2,    28,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    19,     2,    20,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    18
};

#if VM_PARSER_DEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    14,    20,    24,    26,
      27,    30,    31,    36,    43,    46,    50,    53,    56,    61,
      64,    69,    72,    77,    82,    84,    88,    90,    94,    96,
     100,   104,   108,   112,   115,   119,   121,   123,   125,   127,
     131,   133,   135,   137,   142,   149,   153,   155,   156,   158,
     161,   163,   164,   167,   173,   177,   181,   183,   185,   187,
     189,   191,   193,   197,   198,   202,   204,   208
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      31,     0,    -1,    32,    -1,    32,    33,    -1,    -1,    34,
      19,    36,    20,    -1,     9,     3,    21,    35,    22,    -1,
      35,    23,     3,    -1,     3,    -1,    -1,    36,    37,    -1,
      -1,     3,    24,    43,    25,    -1,     3,    26,     3,    24,
      43,    25,    -1,    44,    25,    -1,    10,    43,    25,    -1,
      10,    25,    -1,    38,    37,    -1,    38,    19,    36,    20,
      -1,    39,    37,    -1,    39,    19,    36,    20,    -1,    13,
      25,    -1,    11,    21,    41,    22,    -1,    40,    21,    41,
      22,    -1,    12,    -1,    41,     8,    42,    -1,    42,    -1,
      43,     7,    43,    -1,    43,    -1,    43,    15,    43,    -1,
      43,    14,    43,    -1,    43,    16,    43,    -1,    43,    17,
      43,    -1,    14,    43,    -1,    21,    43,    22,    -1,     4,
      -1,     5,    -1,     6,    -1,     3,    -1,     3,    26,     3,
      -1,    44,    -1,    46,    -1,    49,    -1,     3,    21,    45,
      22,    -1,     3,    26,     3,    21,    45,    22,    -1,    43,
      23,    45,    -1,    43,    -1,    -1,    47,    -1,    48,    47,
      -1,    48,    -1,    -1,    17,     3,    -1,    17,     3,    27,
      41,    28,    -1,    27,    53,    28,    -1,    19,    51,    20,
      -1,     4,    -1,     5,    -1,     6,    -1,     3,    -1,    49,
      -1,    52,    -1,    52,    23,    52,    -1,    -1,     3,    29,
      50,    -1,    50,    -1,    50,    23,    50,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    86,    86,    93,    94,   100,   106,   113,   114,   115,
     122,   123,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   145,   149,   153,   157,   158,   162,   163,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   194,   195,   202,   203,   204,   213,   217,
     218,   219,   223,   224,   230,   231,   235,   236,   237,   238,
     239,   243,   244,   245,   249,   253,   254,   255
};
#endif

#if VM_PARSER_DEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NAME", "NUMBER", "DECIMAL", "STRING",
  "CONDITION", "CONDITIONAL_OPERAND", "FUNCTION", "RETURN", "IF", "WHILE",
  "DOEVENTS", "'-'", "'+'", "'*'", "'/'", "UMINUS", "'{'", "'}'", "'('",
  "')'", "','", "'='", "';'", "'.'", "'['", "']'", "':'", "$accept",
  "code", "functions", "function", "function_header", "parameters",
  "statements", "statement", "if_condition", "while_condition", "while",
  "conditional_expressions", "conditional_expression", "expression",
  "function_call", "expression_params", "xpath_expression",
  "xpath_fragments", "xpath_fragment", "json_expression", "json_value",
  "json_tuples", "json_tuple", "json_array_items", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,    45,    43,    42,    47,   269,   123,
     125,    40,    41,    44,    61,    59,    46,    91,    93,    58
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    30,    31,    32,    32,    33,    34,    35,    35,    35,
      36,    36,    37,    37,    37,    37,    37,    37,    37,    37,
      37,    37,    38,    39,    40,    41,    41,    42,    42,    43,
      43,    43,    43,    43,    43,    43,    43,    43,    43,    43,
      43,    43,    43,    44,    44,    45,    45,    45,    46,    47,
      47,    47,    48,    48,    49,    49,    50,    50,    50,    50,
      50,    51,    51,    51,    52,    53,    53,    53
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     4,     5,     3,     1,     0,
       2,     0,     4,     6,     2,     3,     2,     2,     4,     2,
       4,     2,     4,     4,     1,     3,     1,     3,     1,     3,
       3,     3,     3,     2,     3,     1,     1,     1,     1,     3,
       1,     1,     1,     4,     6,     3,     1,     0,     1,     2,
       1,     0,     2,     5,     3,     3,     1,     1,     1,     1,
       1,     1,     3,     0,     3,     1,     3,     0
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     2,     1,     0,     3,     0,     0,    11,     9,
       0,     8,     0,     0,    51,     0,    24,     0,     5,    10,
       0,     0,     0,     0,     6,     0,    51,    51,     0,    38,
      35,    36,    37,    51,     0,    63,    51,    16,    67,     0,
      40,    41,    48,    50,    42,    51,    21,    11,    17,    11,
      19,    51,    14,     7,    46,     0,     0,     0,     0,    33,
      52,     0,     0,    61,     0,    59,    56,    57,    58,    60,
      65,     0,    51,    51,    51,    51,    15,    49,     0,    26,
      28,     0,     0,     0,    51,    43,    12,    51,    51,    39,
      51,     0,    55,     0,    34,     0,    54,    30,    29,    31,
      32,    51,    22,    51,    18,    20,    23,    45,     0,     0,
       0,    64,    62,    66,    25,    27,    44,    13,    53
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,     5,     6,    12,    10,    19,    20,    21,
      22,    78,    79,    80,    40,    55,    41,    42,    43,    44,
      70,    62,    63,    71
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -51
static const yytype_int16 yypact[] =
{
     -51,    10,    -7,   -51,    15,   -51,     1,     7,   -51,    32,
      88,   -51,    61,    12,    20,    36,   -51,    38,   -51,   -51,
      75,   114,    71,    79,   -51,    93,    50,    76,   119,     6,
     -51,   -51,   -51,    76,   125,   147,    76,   -51,     2,   115,
     -51,   -51,   -51,   101,   -51,    76,   -51,   -51,   -51,   -51,
     -51,    76,   -51,   -51,   132,   129,   120,    22,   150,   -51,
     138,   137,   134,   144,   142,   -51,   -51,   -51,   -51,   -51,
     145,   141,    76,    76,    76,    76,   -51,   -51,    -5,   -51,
      35,    99,   103,     8,    50,   -51,   -51,    50,    76,   149,
      76,     2,   -51,   147,   -51,     2,   -51,   104,   104,   -51,
     -51,    76,   -51,    76,   -51,   -51,   -51,   -51,   151,   127,
       3,   -51,   -51,   -51,   -51,   146,   -51,   -51,   -51
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -51,   -51,   -51,   -51,   -51,   -51,    58,   118,   -51,   -51,
     -51,   -50,    70,   -14,    -6,   -19,   -51,   131,   -51,   -29,
     -47,   -51,    82,   -51
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -48
static const yytype_int8 yytable[] =
{
      39,    83,     4,   101,    23,    65,    66,    67,    68,    69,
       3,   101,    54,    56,    23,    23,   101,   102,     7,    59,
       8,    35,    64,    29,    30,    31,    32,    26,     9,    38,
     106,   118,    58,    26,    33,    11,    27,    34,    28,    35,
     110,    36,   103,    87,   111,    37,    88,    38,   113,    72,
      73,    74,    75,    29,    30,    31,    32,    45,    97,    98,
      99,   100,    69,    46,    33,   107,    69,    34,   108,    35,
      54,    36,   -47,    54,   109,    23,    23,    38,    13,    29,
      30,    31,    32,    24,    25,    14,    15,    16,    17,   115,
      33,    13,    51,    34,    47,    35,    53,    36,    14,    15,
      16,    17,    13,    38,    52,    81,    13,    82,    18,    14,
      15,    16,    17,    14,    15,    16,    17,    13,    34,   104,
      74,    75,    57,   105,    14,    15,    16,    17,    60,    72,
      73,    74,    75,    49,    72,    73,    74,    75,    48,    50,
      76,    72,    73,    74,    75,    86,    72,    73,    74,    75,
      61,    85,   117,    89,    92,    84,    72,    73,    74,    75,
      72,    73,    74,    75,    94,    90,    91,    93,    95,    96,
      87,   114,     0,   116,    77,   112
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-51)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
      14,    51,     9,     8,    10,     3,     4,     5,     6,    38,
       0,     8,    26,    27,    20,    21,     8,    22,     3,    33,
      19,    19,    36,     3,     4,     5,     6,    21,    21,    27,
      22,    28,    26,    21,    14,     3,    24,    17,    26,    19,
      90,    21,     7,    21,    91,    25,    24,    27,    95,    14,
      15,    16,    17,     3,     4,     5,     6,    21,    72,    73,
      74,    75,    91,    25,    14,    84,    95,    17,    87,    19,
      84,    21,    22,    87,    88,    81,    82,    27,     3,     3,
       4,     5,     6,    22,    23,    10,    11,    12,    13,   103,
      14,     3,    21,    17,    19,    19,     3,    21,    10,    11,
      12,    13,     3,    27,    25,    47,     3,    49,    20,    10,
      11,    12,    13,    10,    11,    12,    13,     3,    17,    20,
      16,    17,     3,    20,    10,    11,    12,    13,     3,    14,
      15,    16,    17,    19,    14,    15,    16,    17,    20,    21,
      25,    14,    15,    16,    17,    25,    14,    15,    16,    17,
       3,    22,    25,     3,    20,    23,    14,    15,    16,    17,
      14,    15,    16,    17,    22,    27,    29,    23,    23,    28,
      21,   101,    -1,    22,    43,    93
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    31,    32,     0,     9,    33,    34,     3,    19,    21,
      36,     3,    35,     3,    10,    11,    12,    13,    20,    37,
      38,    39,    40,    44,    22,    23,    21,    24,    26,     3,
       4,     5,     6,    14,    17,    19,    21,    25,    27,    43,
      44,    46,    47,    48,    49,    21,    25,    19,    37,    19,
      37,    21,    25,     3,    43,    45,    43,     3,    26,    43,
       3,     3,    51,    52,    43,     3,     4,     5,     6,    49,
      50,    53,    14,    15,    16,    17,    25,    47,    41,    42,
      43,    36,    36,    41,    23,    22,    25,    21,    24,     3,
      27,    29,    20,    23,    22,    23,    28,    43,    43,    43,
      43,     8,    22,     7,    20,    20,    22,    45,    45,    43,
      41,    50,    52,    50,    42,    43,    22,    25,    28
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if VM_PARSER_DEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !VM_PARSER_DEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !VM_PARSER_DEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1792 of yacc.c  */
#line 86 "vm_parser.y"
    {}
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 93 "vm_parser.y"
    {}
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 94 "vm_parser.y"
    {}
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 100 "vm_parser.y"
    { vm_parser_function_end(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 106 "vm_parser.y"
    { vm_parser_function_create((yyvsp[(2) - (5)].string)); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 113 "vm_parser.y"
    { emit(opPOP, 0, 0, 0, (yyvsp[(3) - (3)].string));  /*$$ = $3;*/ }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 114 "vm_parser.y"
    { emit(opPOP, 0, 0, 0, (yyvsp[(1) - (1)].string));  /*$$ = $1;*/ }
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 115 "vm_parser.y"
    {}
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 122 "vm_parser.y"
    {}
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 123 "vm_parser.y"
    {}
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 132 "vm_parser.y"
    { emit(opSETVAR,    (yyvsp[(3) - (4)].string),  0, 0, (yyvsp[(1) - (4)].string)); var_all_reset(); }
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 133 "vm_parser.y"
    { emit(opSETOBJVAR, (yyvsp[(1) - (6)].string), (yyvsp[(3) - (6)].string), 0, (yyvsp[(5) - (6)].string)); var_all_reset(); }
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 134 "vm_parser.y"
    { var_all_reset(); }
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 135 "vm_parser.y"
    { emit(opRET, (yyvsp[(2) - (3)].string), 0, 0, 0); var_all_reset(); }
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 136 "vm_parser.y"
    { emit(opRET, "0", 0, 0, 0); var_all_reset(); }
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 137 "vm_parser.y"
    { pszLabelName = (yyvsp[(1) - (2)].string); emit(opLABEL, pszLabelName, 0, 0, 0); }
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 138 "vm_parser.y"
    { pszLabelName = (yyvsp[(1) - (4)].string); emit(opLABEL, pszLabelName, 0, 0, 0); }
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 139 "vm_parser.y"
    { pszLabelName = labels_pop(); emit(opJUMP, labels_pop(), 0, 0, 0); emit(opLABEL, pszLabelName, 0, 0, 0); }
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 140 "vm_parser.y"
    { pszLabelName = labels_pop(); emit(opJUMP, labels_pop(), 0, 0, 0); emit(opLABEL, pszLabelName, 0, 0, 0); }
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 141 "vm_parser.y"
    { emit(opDOEVENTS, 0, 0, 0, 0); }
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 145 "vm_parser.y"
    { pszLabelName = label_create(); emit(opIFCONDITION, (yyvsp[(3) - (4)].string), pszLabelName, 0, 0); (yyval.string) = pszLabelName; }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 149 "vm_parser.y"
    {	pszLabelName = label_create(); emit(opIFCONDITION, (yyvsp[(3) - (4)].string), pszLabelName, 0, 0); labels_push(pszLabelName);	/* Do conditional check here */ }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 153 "vm_parser.y"
    { pszLabelName = label_create(); emit(opLABEL, pszLabelName, 0, 0, 0); labels_push(pszLabelName);	/* Loop back to this point */ }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 157 "vm_parser.y"
    { pszTempVar = var_create(); emit(opCONDITION, (yyvsp[(1) - (3)].string), (yyvsp[(2) - (3)].string), (yyvsp[(3) - (3)].string), pszTempVar); (yyval.string) = pszTempVar; }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 158 "vm_parser.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 162 "vm_parser.y"
    { pszTempVar = var_create(); emit(opCONDITION, (yyvsp[(1) - (3)].string), (yyvsp[(2) - (3)].string), (yyvsp[(3) - (3)].string), pszTempVar); (yyval.string) = pszTempVar; }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 163 "vm_parser.y"
    { pszTempVar = var_create(); emit(opCONDITION, (yyvsp[(1) - (1)].string), "!=", "0", pszTempVar); (yyval.string) = pszTempVar; }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 170 "vm_parser.y"
    { pszTempVar = var_create(); emit(opADD, (yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), 0, pszTempVar); (yyval.string) = pszTempVar; }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 171 "vm_parser.y"
    { pszTempVar = var_create(); emit(opSUB, (yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), 0, pszTempVar); (yyval.string) = pszTempVar; }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 172 "vm_parser.y"
    { pszTempVar = var_create(); emit(opMUL, (yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), 0, pszTempVar); (yyval.string) = pszTempVar; }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 173 "vm_parser.y"
    { pszTempVar = var_create(); emit(opDIV, (yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), 0, pszTempVar); (yyval.string) = pszTempVar; }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 174 "vm_parser.y"
    { pszTempVar = var_create(); emit(opNEG, (yyvsp[(2) - (2)].string),  0, 0, pszTempVar); (yyval.string) = pszTempVar; }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 175 "vm_parser.y"
    { (yyval.string) = (yyvsp[(2) - (3)].string); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 176 "vm_parser.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 177 "vm_parser.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 178 "vm_parser.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 179 "vm_parser.y"
    { pszTempVar = var_create(); emit(opGETVAR,    (yyvsp[(1) - (1)].string),  0, 0, pszTempVar); (yyval.string) = pszTempVar; }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 180 "vm_parser.y"
    { pszTempVar = var_create(); emit(opGETOBJVAR, (yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string), 0, pszTempVar); (yyval.string) = pszTempVar; }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 181 "vm_parser.y"
    { pszTempVar = var_create(); emit(opPOP,	    0,  0, 0, pszTempVar); (yyval.string) = pszTempVar; }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 182 "vm_parser.y"
    { pszTempVar = var_create(); /*TODO*/ (yyval.string) = pszTempVar; }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 183 "vm_parser.y"
    { pszTempVar = var_create(); /*TODO*/ (yyval.string) = pszTempVar; }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 194 "vm_parser.y"
    { emit(opCALL, (yyvsp[(1) - (4)].string), 0, 0, 0); }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 195 "vm_parser.y"
    { emit(opCALLOBJMETHOD, (yyvsp[(1) - (6)].string), (yyvsp[(3) - (6)].string), 0, 0); }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 202 "vm_parser.y"
    { emit(opPUSH, (yyvsp[(1) - (3)].string), 0, 0, 0); }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 203 "vm_parser.y"
    { emit(opPUSH, (yyvsp[(1) - (1)].string), 0, 0, 0); }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 204 "vm_parser.y"
    {}
    break;


/* Line 1792 of yacc.c  */
#line 1772 "vm_parser.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 258 "vm_parser.y"
