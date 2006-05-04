/* A Bison parser, made by GNU Bison 2.0.  */

/* Skeleton parser for GLR parsing with Bison,
   Copyright (C) 2002, 2003, 2004 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* This is the parser code for GLR (Generalized LR) parser. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <setjmp.h>

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "glr.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ZZ_NUM = 258,
     ZZ_DOTDOTDOT = 259,
     ZZ_STRING = 260,
     ZZ_EQEQ = 261,
     ZZ_INCLUDE = 262,
     ZZ_PREDICATE = 263,
     ZZ_FUNCTION = 264,
     ZZ_CONSTANT = 265,
     ZZ_VARIABLE = 266,
     ZZ_TYPE = 267,
     ZZ_FORALL = 268,
     ZZ_EXIST = 269,
     ZZ_EQUIV = 270,
     ZZ_IMPLY = 271
   };
#endif
#define ZZ_NUM 258
#define ZZ_DOTDOTDOT 259
#define ZZ_STRING 260
#define ZZ_EQEQ 261
#define ZZ_INCLUDE 262
#define ZZ_PREDICATE 263
#define ZZ_FUNCTION 264
#define ZZ_CONSTANT 265
#define ZZ_VARIABLE 266
#define ZZ_TYPE 267
#define ZZ_FORALL 268
#define ZZ_EXIST 269
#define ZZ_EQUIV 270
#define ZZ_IMPLY 271




/* Copy the first part of user declarations.  */
#line 1 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"

#define YYSTYPE int
#define YYDEBUG 1

#include "fol.h"
#include "follex.cpp"

  // 0: no output; 1,2: increasing order of verbosity
int folDbg = 0;
//int folDbg = 1;
//int folDbg = 2;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined (YYLTYPE) && ! defined (YYLTYPE_IS_DECLARED)
typedef struct YYLTYPE
{

  char yydummy;

} YYLTYPE;
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

/* Default (constant) value used for initialization for null
   right-hand sides.  Unlike the standard yacc.c template,
   here we set the default value of $$ to a zeroed-out value.
   Since the default value is undefined, this behavior is
   technically correct. */
static YYSTYPE yyval_default;

/* Copy the second part of user declarations.  */


/* Line 217 of glr.c.  */
#line 141 "fol.cpp"

#ifndef YYFREE
# define YYFREE free
#endif
#ifndef YYMALLOC
# define YYMALLOC malloc
#endif
#ifndef YYREALLOC
# define YYREALLOC realloc
#endif

#ifdef __cplusplus
   typedef bool yybool;
#else
   typedef unsigned char yybool;
#endif
#define yytrue 1
#define yyfalse 0

/*-----------------.
| GCC extensions.  |
`-----------------*/

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if !defined (__GNUC__) || __GNUC__ < 2 || \
(__GNUC__ == 2 && __GNUC_MINOR__ < 5) || __STRICT_ANSI__
#  define __attribute__(Spec) /* empty */
# endif
#endif

#ifndef YYASSERT
# define YYASSERT(condition) ((void) ((condition) || (abort (), 0)))
#endif

#ifndef ATTRIBUTE_UNUSED
# define ATTRIBUTE_UNUSED __attribute__ ((__unused__))
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   206

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  85
/* YYNRULES -- Number of rules. */
#define YYNRULES  153
/* YYNRULES -- Number of states. */
#define YYNSTATES  214
/* YYMAXRHS -- Maximum number of symbols on right-hand side of rule. */
#define YYMAXRHS 10
/* YYMAXLEFT -- Maximum number of symbols to the left of a handle
   accessed by $0, $-1, etc., in any rule. */
#define YYMAXLEFT 0

/* YYTRANSLATE(X) -- Bison symbol number corresponding to X.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   271

#define YYTRANSLATE(YYX)						\
  ((YYX <= 0) ? YYEOF :							\
   (unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      25,     2,     2,    28,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    21,     2,     2,     2,    24,     2,     2,
      33,    34,    22,    14,    32,    13,    27,    23,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      37,    29,    36,    35,    26,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    20,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    19,     2,
       2,     2,     2,    30,     2,    31,     2,     2,     2,     2,
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
       5,     6,     7,     8,     9,    10,    11,    12,    15,    16,
      17,    18
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     4,     8,    11,    14,    18,    22,    26,
      30,    35,    39,    43,    44,    45,    46,    55,    57,    60,
      61,    63,    65,    67,    68,    70,    72,    76,    78,    80,
      81,    88,    90,    92,    93,    94,    99,   101,   103,   105,
     106,   108,   112,   115,   118,   119,   120,   121,   132,   136,
     138,   139,   144,   146,   148,   149,   150,   151,   159,   160,
     161,   162,   171,   173,   175,   176,   181,   182,   187,   189,
     191,   193,   195,   196,   198,   200,   201,   202,   209,   210,
     211,   212,   222,   224,   228,   230,   232,   234,   236,   238,
     240,   242,   244,   245,   247,   248,   253,   254,   257,   258,
     264,   265,   271,   272,   278,   279,   285,   286,   287,   288,
     295,   296,   300,   302,   304,   305,   306,   312,   314,   316,
     317,   318,   326,   327,   332,   334,   336,   339,   342,   344,
     345,   347,   348,   353,   355,   357,   359,   361,   363,   366,
     370,   371,   373,   374,   375,   382,   383,   384,   385,   394,
     396,   398,   400,   402
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const signed char yyrhs[] =
{
      39,     0,    -1,    -1,    39,     1,    25,    -1,    39,    45,
      -1,    39,    47,    -1,    39,    49,    45,    -1,    39,    56,
      45,    -1,    39,    64,    45,    -1,    39,    68,    45,    -1,
      39,    43,    78,    79,    -1,    39,    43,    82,    -1,    39,
      43,    45,    -1,    -1,    -1,    -1,    39,    40,    89,    41,
      90,    44,    42,    45,    -1,    26,    -1,    43,    26,    -1,
      -1,    27,    -1,    25,    -1,    28,    -1,    -1,    25,    -1,
      28,    -1,     7,     5,    48,    -1,    25,    -1,    28,    -1,
      -1,    51,    29,    30,    50,    52,    31,    -1,    11,    -1,
      12,    -1,    -1,    -1,    52,    55,    53,    54,    -1,    11,
      -1,     5,    -1,    10,    -1,    -1,    32,    -1,    25,    32,
      25,    -1,    25,    32,    -1,    32,    25,    -1,    -1,    -1,
      -1,    51,    29,    30,    57,     3,    58,    32,    59,    60,
      31,    -1,     4,    32,     3,    -1,    61,    -1,    -1,    63,
      32,    62,    61,    -1,    63,    -1,     3,    -1,    -1,    -1,
      -1,    11,    65,    33,    66,    73,    67,    34,    -1,    -1,
      -1,    -1,    72,    69,    11,    70,    33,    71,    73,    34,
      -1,    12,    -1,    11,    -1,    -1,    73,    32,    74,    76,
      -1,    -1,    73,    32,    75,    77,    -1,    76,    -1,    77,
      -1,    12,    -1,    11,    -1,    -1,    21,    -1,    35,    -1,
      -1,    -1,     8,    80,    33,    81,    86,    34,    -1,    -1,
      -1,    -1,    88,    29,    83,     9,    84,    33,    85,    86,
      34,    -1,    87,    -1,    86,    55,    87,    -1,    10,    -1,
       5,    -1,     3,    -1,    11,    -1,    10,    -1,     5,    -1,
       3,    -1,    11,    -1,    -1,     3,    -1,    -1,    33,    91,
      90,    34,    -1,    -1,    92,   106,    -1,    -1,    90,    18,
      93,    46,    90,    -1,    -1,    90,    17,    94,    46,    90,
      -1,    -1,    90,    19,    95,    46,    90,    -1,    -1,    90,
      20,    96,    46,    90,    -1,    -1,    -1,    -1,    97,   101,
      98,   102,    99,    90,    -1,    -1,    21,   100,    90,    -1,
      15,    -1,    16,    -1,    -1,    -1,   103,   105,    32,   104,
     102,    -1,   105,    -1,    11,    -1,    -1,    -1,   111,     8,
     107,    33,   108,   112,    34,    -1,    -1,   109,   114,   110,
     114,    -1,    36,    -1,    37,    -1,    36,    29,    -1,    37,
      29,    -1,    29,    -1,    -1,    22,    -1,    -1,   112,    32,
     113,   114,    -1,   114,    -1,   116,    -1,    10,    -1,     5,
      -1,     3,    -1,   115,    11,    -1,   115,    11,    21,    -1,
      -1,    14,    -1,    -1,    -1,     9,   117,    33,   118,   112,
      34,    -1,    -1,    -1,    -1,   119,    33,   120,   114,   122,
     121,   114,    34,    -1,    14,    -1,    13,    -1,    22,    -1,
      23,    -1,    24,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,    49,    49,    51,    62,    63,    64,    65,    66,    67,
      68,    70,    72,    76,    81,    86,    75,   144,   145,   148,
     151,   161,   163,   166,   169,   171,   177,   240,   242,   253,
     251,   271,   284,   299,   304,   303,   324,   324,   324,   329,
     332,   334,   341,   347,   358,   364,   391,   356,   402,   439,
     446,   444,   453,   456,   493,   505,   506,   492,   526,   549,
     574,   525,   603,   603,   609,   608,   613,   612,   615,   616,
     620,   633,   652,   655,   661,   673,   686,   672,   730,   736,
     776,   728,   821,   829,   839,   840,   846,   855,   863,   872,
     884,   893,   908,   911,   926,   925,   940,   940,   944,   943,
     956,   955,   968,   967,   979,   978,   990,   992,   998,   990,
    1009,  1008,  1024,  1032,  1045,  1047,  1045,  1054,  1058,  1084,
    1113,  1083,  1270,  1270,  1428,  1442,  1456,  1473,  1490,  1505,
    1507,  1525,  1523,  1535,  1540,  1714,  1724,  1736,  1751,  1757,
    1781,  1783,  1795,  1831,  1794,  1911,  1927,  1932,  1911,  2016,
    2025,  2034,  2043,  2052
};
#endif

#if (YYDEBUG) || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ZZ_NUM", "ZZ_DOTDOTDOT", "ZZ_STRING",
  "ZZ_EQEQ", "ZZ_INCLUDE", "ZZ_PREDICATE", "ZZ_FUNCTION", "ZZ_CONSTANT",
  "ZZ_VARIABLE", "ZZ_TYPE", "'-'", "'+'", "ZZ_FORALL", "ZZ_EXIST",
  "ZZ_EQUIV", "ZZ_IMPLY", "'v'", "'^'", "'!'", "'*'", "'/'", "'%'",
  "'\\n'", "'@'", "'.'", "'\\r'", "'='", "'{'", "'}'", "','", "'('", "')'",
  "'?'", "'>'", "'<'", "$accept", "input", "@1", "@2", "@3", "at",
  "fullstop", "newline", "optnewline", "include", "nnewline",
  "type_declaration", "@4", "is_variable_type", "constant_declarations",
  "@5", "variable_or_string_or_constant", "constant_sep",
  "numeric_type_declaration", "@6", "@7", "@8", "numeric_types",
  "single_numeric_types", "@9", "single_numeric_type",
  "predicate_declaration", "@10", "@11", "@12", "function_declaration",
  "@13", "@14", "@15", "function_return_type", "types", "@16", "@17",
  "type_code", "variable_code", "pd_not_qs", "predicate_definition", "@18",
  "@19", "function_definition", "@20", "@21", "@22",
  "constants_in_groundings", "is_constant_string_num_variable",
  "function_return_constant", "weight", "sentence", "@23", "@24", "@25",
  "@26", "@27", "@28", "@29", "@30", "@31", "@32", "quantifier",
  "variables", "@33", "@34", "quant_variable", "atomic_sentence", "@35",
  "@36", "@37", "internal_predicate_sign", "asterisk", "terms", "@38",
  "term", "plus", "function_term", "@39", "@40", "@41", "@42", "@43",
  "internal_function_sign", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    38,    39,    39,    39,    39,    39,    39,    39,    39,
      39,    39,    39,    40,    41,    42,    39,    43,    43,    44,
      44,    45,    45,    46,    46,    46,    47,    48,    48,    50,
      49,    51,    51,    52,    53,    52,    54,    54,    54,    55,
      55,    55,    55,    55,    57,    58,    59,    56,    60,    60,
      62,    61,    61,    63,    65,    66,    67,    64,    69,    70,
      71,    68,    72,    72,    74,    73,    75,    73,    73,    73,
      76,    77,    78,    78,    78,    80,    81,    79,    83,    84,
      85,    82,    86,    86,    87,    87,    87,    87,    88,    88,
      88,    88,    89,    89,    91,    90,    92,    90,    93,    90,
      94,    90,    95,    90,    96,    90,    97,    98,    99,    90,
     100,    90,   101,   101,   103,   104,   102,   102,   105,   107,
     108,   106,   109,   106,   110,   110,   110,   110,   110,   111,
     111,   113,   112,   112,   114,   114,   114,   114,   114,   114,
     115,   115,   117,   118,   116,   119,   120,   121,   116,   122,
     122,   122,   122,   122
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     0,     3,     2,     2,     3,     3,     3,     3,
       4,     3,     3,     0,     0,     0,     8,     1,     2,     0,
       1,     1,     1,     0,     1,     1,     3,     1,     1,     0,
       6,     1,     1,     0,     0,     4,     1,     1,     1,     0,
       1,     3,     2,     2,     0,     0,     0,    10,     3,     1,
       0,     4,     1,     1,     0,     0,     0,     7,     0,     0,
       0,     8,     1,     1,     0,     4,     0,     4,     1,     1,
       1,     1,     0,     1,     1,     0,     0,     6,     0,     0,
       0,     9,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     0,     4,     0,     2,     0,     5,
       0,     5,     0,     5,     0,     5,     0,     0,     0,     6,
       0,     3,     1,     1,     0,     0,     5,     1,     1,     0,
       0,     7,     0,     4,     1,     1,     2,     2,     1,     0,
       1,     0,     4,     1,     1,     1,     1,     1,     2,     3,
       0,     1,     0,     0,     6,     0,     0,     0,     8,     1,
       1,     1,     1,     1
};

/* YYDPREC[RULE-NUM] -- Dynamic precedence of rule #RULE-NUM (0 if none). */
static const unsigned char yydprec[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       0,     1,     0,     1,     0,     1,     0,     0,     0,     2,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0
};

/* YYMERGER[RULE-NUM] -- Index of merging function for rule #RULE-NUM. */
static const unsigned char yymerger[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error.  */
static const unsigned char yydefact[] =
{
       2,     0,     1,     0,     0,    31,    32,    21,    17,    22,
      92,    72,     4,     5,     0,     0,     0,     0,     0,    58,
       3,     0,     0,    93,    14,    90,    89,    88,    91,    73,
      18,    74,    12,     0,    11,     0,     6,     0,     7,     8,
       9,     0,    27,    28,    26,    55,    96,    75,    10,    78,
      29,    59,     0,   110,    94,    19,   122,     0,     0,     0,
      33,     0,     0,    71,    70,    56,    68,    69,    96,    96,
     100,    98,   102,   104,    20,    15,   130,    97,   140,     0,
     112,   113,   107,    76,    79,    39,    45,    60,    64,     0,
     111,     0,    23,    23,    23,    23,     0,   137,   136,   142,
     135,   141,     0,     0,   134,     0,   119,     0,     0,     0,
       0,    30,    40,    34,     0,     0,     0,     0,    57,    95,
      24,    25,    96,    96,    96,    96,    16,     0,   128,   124,
     125,   140,   138,   146,     0,   118,   108,     0,   117,    86,
      85,    84,    87,    39,    82,    80,    42,    43,     0,    46,
       0,    65,    67,   101,    99,   103,   105,   143,   126,   127,
     123,   139,   140,   120,    96,     0,    77,     0,     0,    41,
      37,    38,    36,    35,     0,    61,   140,     0,   140,   109,
     115,    83,    39,    53,     0,     0,    49,    52,     0,   133,
     150,   149,   151,   152,   153,   147,     0,     0,    81,     0,
      47,    50,   131,   144,   140,   121,   116,    48,     0,   140,
       0,    51,   132,   148
};

/* YYPDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     1,    10,    46,    96,    11,    75,    12,   122,    13,
      44,    14,    60,    15,    85,   148,   173,   167,    16,    61,
     114,   174,   185,   186,   208,   187,    17,    22,    52,    89,
      18,    41,    62,   115,    19,    65,   116,   117,    66,    67,
      33,    48,    58,   108,    34,    59,   109,   168,   143,   144,
      35,    24,    55,    69,    56,    93,    92,    94,    95,    57,
     107,   164,    68,    82,   136,   137,   197,   138,    77,   134,
     178,    78,   131,    79,   188,   209,   189,   103,   104,   127,
     176,   105,   162,   204,   195
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -79
static const short int yypact[] =
{
     -79,    11,   -79,    -1,    25,    -4,    27,   -79,   -79,   -79,
      47,    38,   -79,   -79,    55,    31,    55,    55,    55,   -79,
     -79,    57,    32,   -79,   -79,   -79,   -79,   -79,   -79,   -79,
     -79,   -79,   -79,    70,   -79,    58,   -79,    88,   -79,   -79,
     -79,    98,   -79,   -79,   -79,   -79,    46,   -79,   -79,   -79,
     129,   -79,    -7,   -79,   -79,   121,     9,   146,    59,   133,
     -79,   143,   135,   -79,   -79,   127,   -79,   -79,    53,    56,
     -79,   -79,   -79,   -79,   -79,   -79,   -79,   -79,    37,   159,
     -79,   -79,   -79,   -79,   -79,     3,   -79,   -79,   158,   136,
     -79,   103,    89,    89,    89,    89,    55,   -79,   -79,   -79,
     -79,   -79,   107,   160,   -79,   139,   -79,   163,   100,   142,
     141,   -79,   151,   -79,   145,    -7,   166,   168,   -79,   -79,
     -79,   -79,    60,    75,    79,    83,   -79,   147,   -79,   152,
     153,    37,   164,   -79,   150,   -79,   -79,   173,   -79,   -79,
     -79,   -79,   -79,    20,   -79,   -79,   161,   -79,   140,   -79,
     -19,   -79,   -79,   134,   144,   167,   -79,   -79,   -79,   -79,
     -79,   -79,    37,   -79,    86,   156,   -79,   100,   100,   -79,
     -79,   -79,   -79,   -79,   162,   -79,    37,   111,    37,   110,
     -79,   -79,    81,   -79,   157,   165,   -79,   169,   115,   -79,
     -79,   -79,   -79,   -79,   -79,   -79,   126,   179,   -79,   188,
     -79,   -79,   -79,   -79,    37,   -79,   -79,   -79,   189,    37,
     170,   -79,   -79,   -79
};

/* YYPGOTO[NTERM-NUM].  */
static const signed char yypgoto[] =
{
     -79,   -79,   -79,   -79,   -79,   -79,   -79,    -8,    62,   -79,
     -79,   -79,   -79,   -79,   -79,   -79,   -79,   108,   -79,   -79,
     -79,   -79,   -79,   -14,   -79,   -79,   -79,   -79,   -79,   -79,
     -79,   -79,   -79,   -79,   -79,    80,   -79,   -79,    82,    85,
     -79,   -79,   -79,   -79,   -79,   -79,   -79,   -79,    29,    33,
     -79,   -79,   -67,   -79,   -79,   -79,   -79,   -79,   -79,   -79,
     -79,   -79,   -79,   -79,     2,   -79,   -79,    66,   -79,   -79,
     -79,   -79,   -79,   -79,    28,   -79,   -78,   -79,   -79,   -79,
     -79,   -79,   -79,   -79,   -79
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, parse error.  */
#define YYTABLE_NINF -146
static const short int yytable[] =
{
     102,    90,    91,    32,    63,    64,    36,   -63,    38,    39,
      40,     2,     3,    88,   -13,   175,   -13,  -129,     4,   -13,
     -13,   -13,     5,     6,    20,   -13,   -13,   -13,   110,   -54,
      21,    76,   -13,   -13,   111,   112,     7,     8,   -62,     9,
      97,    25,    98,    26,   -13,   110,    99,   100,    27,    28,
      23,   101,   112,   160,   166,   153,   154,   155,   156,    29,
      37,  -106,  -106,     7,    30,    45,     9,    53,  -106,  -106,
    -145,  -106,  -106,    31,    53,  -106,  -106,    53,    47,    54,
       7,    53,    42,     9,   177,    43,    54,    49,   126,    54,
    -106,  -106,    83,    54,  -106,  -106,    53,   179,  -106,  -106,
      53,  -106,  -106,   139,    53,   140,   110,    53,    54,    51,
     141,   142,    54,   112,   120,   198,    54,   121,    50,    54,
      70,    71,    72,    73,   190,   191,   210,    70,    71,    72,
      73,   212,   -44,   192,   193,   194,   128,   119,    70,    71,
      72,    73,    84,   129,   130,   170,    86,   202,    74,   203,
     171,   172,    71,    72,    73,   123,   124,   125,   202,    88,
     205,    80,    81,    72,    73,   183,   184,   106,    87,   -66,
     118,   132,   133,   146,   135,   145,   147,   149,    64,    63,
     157,   158,   159,   163,   135,   161,   169,    73,   180,   199,
     135,   207,   183,   113,   211,   150,   200,   182,   151,   206,
     181,   201,   152,   165,   213,     0,   196
};

/* YYCONFLP[YYPACT[STATE-NUM]] -- Pointer into YYCONFL of start of
   list of conflicting reductions corresponding to action entry for
   state STATE-NUM in yytable.  0 means no conflicts.  The list in
   yyconfl is terminated by a rule number of 0.  */
static const unsigned char yyconflp[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       3,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       0,     0,     0,     0,     0,     0,     7,     0,     0,     9,
       0,     0,     0,    13,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    15,     0,
       0,     0,    17,     0,     0,     0,    19,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    23,    25,    27,
      29,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    11,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      31,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0
};

/* YYCONFL[I] -- lists of conflicting rule numbers, each terminated by
   0, pointed into by YYCONFLP.  */
static const short int yyconfl[] =
{
       0,    13,     0,    92,     0,    96,     0,    96,     0,    96,
       0,   114,     0,    96,     0,    96,     0,    96,     0,    96,
       0,    96,     0,   109,     0,   109,     0,   109,     0,   109,
       0,   114,     0
};

static const short int yycheck[] =
{
      78,    68,    69,    11,    11,    12,    14,    11,    16,    17,
      18,     0,     1,    32,     3,    34,     5,     8,     7,     8,
       9,    10,    11,    12,    25,    14,    15,    16,    25,    33,
       5,    22,    21,    22,    31,    32,    25,    26,    11,    28,
       3,     3,     5,     5,    33,    25,     9,    10,    10,    11,
       3,    14,    32,   131,    34,   122,   123,   124,   125,    21,
      29,    15,    16,    25,    26,    33,    28,    21,    15,    16,
      33,    15,    16,    35,    21,    15,    16,    21,     8,    33,
      25,    21,    25,    28,   162,    28,    33,    29,    96,    33,
      15,    16,    33,    33,    15,    16,    21,   164,    15,    16,
      21,    15,    16,     3,    21,     5,    25,    21,    33,    11,
      10,    11,    33,    32,    25,    34,    33,    28,    30,    33,
      17,    18,    19,    20,    13,    14,   204,    17,    18,    19,
      20,   209,     3,    22,    23,    24,    29,    34,    17,    18,
      19,    20,     9,    36,    37,     5,     3,    32,    27,    34,
      10,    11,    18,    19,    20,    93,    94,    95,    32,    32,
      34,    15,    16,    19,    20,     3,     4,     8,    33,    11,
      34,    11,    33,    32,    11,    33,    25,    32,    12,    11,
      33,    29,    29,    33,    11,    21,    25,    20,    32,    32,
      11,     3,     3,    85,   208,   115,    31,   168,   116,   197,
     167,    32,   117,   137,    34,    -1,   178
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    39,     0,     1,     7,    11,    12,    25,    26,    28,
      40,    43,    45,    47,    49,    51,    56,    64,    68,    72,
      25,     5,    65,     3,    89,     3,     5,    10,    11,    21,
      26,    35,    45,    78,    82,    88,    45,    29,    45,    45,
      45,    69,    25,    28,    48,    33,    41,     8,    79,    29,
      30,    11,    66,    21,    33,    90,    92,    97,    80,    83,
      50,    57,    70,    11,    12,    73,    76,    77,   100,    91,
      17,    18,    19,    20,    27,    44,    22,   106,   109,   111,
      15,    16,   101,    33,     9,    52,     3,    33,    32,    67,
      90,    90,    94,    93,    95,    96,    42,     3,     5,     9,
      10,    14,   114,   115,   116,   119,     8,    98,    81,    84,
      25,    31,    32,    55,    58,    71,    74,    75,    34,    34,
      25,    28,    46,    46,    46,    46,    45,   117,    29,    36,
      37,   110,    11,    33,   107,    11,   102,   103,   105,     3,
       5,    10,    11,    86,    87,    33,    32,    25,    53,    32,
      73,    76,    77,    90,    90,    90,    90,    33,    29,    29,
     114,    21,   120,    33,    99,   105,    34,    55,    85,    25,
       5,    10,    11,    54,    59,    34,   118,   114,   108,    90,
      32,    87,    86,     3,     4,    60,    61,    63,   112,   114,
      13,    14,    22,    23,    24,   122,   112,   104,    34,    32,
      31,    32,    32,    34,   121,    34,   102,     3,    62,   113,
     114,    61,   114,    34
};


/* Prevent warning if -Wmissing-prototypes.  */
int yyparse (void);

/* Error token number */
#define YYTERROR 1

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */


#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N) ((void) 0)
#endif


#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#define YYLEX yylex ()

YYSTYPE yylval;

YYLTYPE yylloc;

int yynerrs;
int yychar;

static const int YYEOF = 0;
static const int YYEMPTY = -2;

typedef enum { yyok, yyaccept, yyabort, yyerr } YYRESULTTAG;

#define YYCHK(YYE)							     \
   do { YYRESULTTAG yyflag = YYE; if (yyflag != yyok) return yyflag; }	     \
   while (0)

#if YYDEBUG

#if ! defined (YYFPRINTF)
#  define YYFPRINTF fprintf
#endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;

#else /* !YYDEBUG */

# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)

#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYMAXDEPTH * sizeof (GLRStackItem)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

/* Minimum number of free items on the stack allowed after an
   allocation.  This is to allow allocation and initialization
   to be completed by functions that call expandGLRStack before the
   stack is expanded, thus insuring that all necessary pointers get
   properly redirected to new data. */
#define YYHEADROOM 2

#if (! defined (YYSTACKEXPANDABLE) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))
#define YYSTACKEXPANDABLE 1
#else
#define YYSTACKEXPANDABLE 0
#endif

/** State numbers, as in LALR(1) machine */
typedef int yyStateNum;

/** Rule numbers, as in LALR(1) machine */
typedef int yyRuleNum;

/** Grammar symbol */
typedef short int yySymbol;

/** Item references, as in LALR(1) machine */
typedef short int yyItemNum;

typedef struct yyGLRState yyGLRState;
typedef struct yySemanticOption yySemanticOption;
typedef union yyGLRStackItem yyGLRStackItem;
typedef struct yyGLRStack yyGLRStack;
typedef struct yyGLRStateSet yyGLRStateSet;

struct yyGLRState {
  yybool yyisState;
  yybool yyresolved;
  yyStateNum yylrState;
  yyGLRState* yypred;
  size_t yyposn;
  union {
    yySemanticOption* yyfirstVal;
    YYSTYPE yysval;
  } yysemantics;
  YYLTYPE yyloc;
};

struct yyGLRStateSet {
  yyGLRState** yystates;
  size_t yysize, yycapacity;
};

struct yySemanticOption {
  yybool yyisState;
  yyRuleNum yyrule;
  yyGLRState* yystate;
  yySemanticOption* yynext;
};

union yyGLRStackItem {
  yyGLRState yystate;
  yySemanticOption yyoption;
};

struct yyGLRStack {
  int yyerrflag;
  int yyerrState;


  yySymbol* yytokenp;
  jmp_buf yyexception_buffer;
  yyGLRStackItem* yyitems;
  yyGLRStackItem* yynextFree;
  int yyspaceLeft;
  yyGLRState* yysplitPoint;
  yyGLRState* yylastDeleted;
  yyGLRStateSet yytops;
};

static void yyinitGLRStack (yyGLRStack* yystack, size_t yysize);
static void yyexpandGLRStack (yyGLRStack* yystack);
static void yyfreeGLRStack (yyGLRStack* yystack);

static void
yyFail (yyGLRStack* yystack, const char* yyformat, ...)
{
  yystack->yyerrflag = 1;
  if (yyformat != NULL)
    {
      char yymsg[256];
      va_list yyap;
      va_start (yyap, yyformat);
      vsprintf (yymsg, yyformat, yyap);
      yyerror (yymsg);
    }
  longjmp (yystack->yyexception_buffer, 1);
}

#if YYDEBUG || YYERROR_VERBOSE
/** A printable representation of TOKEN.  Valid until next call to
 *  tokenName. */
static inline const char*
yytokenName (yySymbol yytoken)
{
  if (yytoken == YYEMPTY)
    return "";

  if (yytoken < 0) { char* buf = new char[1]; buf[0]='\0'; return buf; } return yytname[yytoken];
}
#endif

/** Fill in YYVSP[YYLOW1 .. YYLOW0-1] from the chain of states starting
 *  at YYVSP[YYLOW0].yystate.yypred.  Leaves YYVSP[YYLOW1].yystate.yypred
 *  containing the pointer to the next state in the chain. Assumes
 *  YYLOW1 < YYLOW0.  */
static void yyfillin (yyGLRStackItem *, int, int) ATTRIBUTE_UNUSED;
static void
yyfillin (yyGLRStackItem *yyvsp, int yylow0, int yylow1)
{
  yyGLRState* s;
  int i;
  s = yyvsp[yylow0].yystate.yypred;
  for (i = yylow0-1; i >= yylow1; i -= 1)
    {
      YYASSERT (s->yyresolved);
      yyvsp[i].yystate.yyresolved = yytrue;
      yyvsp[i].yystate.yysemantics.yysval = s->yysemantics.yysval;
      yyvsp[i].yystate.yyloc = s->yyloc;
      s = yyvsp[i].yystate.yypred = s->yypred;
    }
}

/* Do nothing if YYNORMAL or if *YYLOW <= YYLOW1.  Otherwise, fill in
   YYVSP[YYLOW1 .. *YYLOW-1] as in yyfillin and set *YYLOW = YYLOW1.
   For convenience, always return YYLOW1.  */
static inline int yyfill (yyGLRStackItem *, int *, int, yybool)
     ATTRIBUTE_UNUSED;
static inline int
yyfill (yyGLRStackItem *yyvsp, int *yylow, int yylow1, yybool yynormal)
{
  if (!yynormal && yylow1 < *yylow)
    {
      yyfillin (yyvsp, *yylow, yylow1);
      *yylow = yylow1;
    }
  return yylow1;
}

/** Perform user action for rule number YYN, with RHS length YYRHSLEN,
 *  and top stack item YYVSP.  YYLVALP points to place to put semantic
 *  value ($$), and yylocp points to place for location information
 *  (@$). Returns yyok for normal return, yyaccept for YYACCEPT,
 *  yyerr for YYERROR, yyabort for YYABORT. */
static YYRESULTTAG
yyuserAction (yyRuleNum yyn, int yyrhslen, yyGLRStackItem* yyvsp,
	      YYSTYPE* yyvalp,
	      YYLTYPE* yylocp ATTRIBUTE_UNUSED,
	      yyGLRStack* yystack
              )
{
  yybool yynormal ATTRIBUTE_UNUSED = (yystack->yysplitPoint == NULL);
  int yylow;

# undef yyerrok
# define yyerrok (yystack->yyerrState = 0)
# undef YYACCEPT
# define YYACCEPT return yyaccept
# undef YYABORT
# define YYABORT return yyabort
# undef YYERROR
# define YYERROR return yyerrok, yyerr
# undef YYRECOVERING
# define YYRECOVERING (yystack->yyerrState != 0)
# undef yyclearin
# define yyclearin (yychar = *(yystack->yytokenp) = YYEMPTY)
# undef YYFILL
# define YYFILL(N) yyfill (yyvsp, &yylow, N, yynormal)
# undef YYBACKUP
# define YYBACKUP(Token, Value)						     \
  return yyerror ("parse error: cannot back up"),	     \
	 yyerrok, yyerr

  yylow = 1;
  if (yyrhslen == 0)
    *yyvalp = yyval_default;
  else
    *yyvalp = yyvsp[YYFILL (1-yyrhslen)].yystate.yysemantics.yysval;
  YYLLOC_DEFAULT (*yylocp, yyvsp - yyrhslen, yyrhslen);

  switch (yyn)
    {
        case 2:
#line 49 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { if (folDbg >= 2) printf("input: empty\n"); ;}
    break;

  case 3:
#line 52 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    yyerrok; // tell Bison not to suppress any errors
    const char* tok; 
    while (true)
    {
      tok = zztokenList.removeLast();
      if (strcmp(tok,"\n")==0) { delete tok; break; }
      delete tok;
    }
  ;}
    break;

  case 13:
#line 76 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    if (folDbg >= 2) printf("input: weight\n"); 
    zzreset();
  ;}
    break;

  case 14:
#line 81 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {  
    if (folDbg >= 2) printf("input: sentence\n");
      // the states should be reset because a parse error may have occurred
  ;}
    break;

  case 15:
#line 86 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {

    ListObj* formula;
    zzassert(zzoldNewVarList.size()==0,"expected zzoldNewVarList.size()==0");
    zzassert(zzformulaListObjs.size()==1,"expected zzformulaListObjs.size()=1");
   	formula = zzformulaListObjs.top(); zzformulaListObjs.pop();

   	zzdetermineEqPredTypes(formula);//set the any unknown type of '=' predicates
    zzeqPredList.clear();
   	zzdetermineIntPredTypes(formula);//set the any unknown type of internal predicates
    zzintPredList.clear();
   	zzdetermineIntFuncTypes(formula);//set the any unknown type of internal functions
    zzintFuncList.clear();
    zzsetPlusVarTypeId();// set typeIds of variables with pluses before them
    zzcheckVarNameToIdMap();
    
    if (zzhasFullStop && zzwt != NULL)
    {
      zzerr("A weight must not be specified for a formula that is "
            "terminated with a period (i.e. the formula is hard).");
      delete zzwt; zzwt = NULL;
    }
	
    // at this point we are sure we are dealing with a formula

    //if the '!' operator is used, check that it's correctly used
    if (zzuniqueVarIndexes.size() > 0)
    { 
      if (zzfdnumPreds != 1 || zznumAsterisk >0)
        zzerr("'!' can only be used in a clause with one predicate, "
              "and cannot be used with '*'.");
      if (zzformulaStr.find("EXIST") != string::npos || 
          zzformulaStr.find("FORALL") != string::npos)
        zzerr("'!' cannot be used with quantifiers");
    }

      //if there are errors, we can keep this formula for further processing
    if (zznumErrors == 0)
    {
        //defer converting the formula to CNF until we have read all the
        //.db files and are sure that we know all the constants to ground
        //formula's clauses
	    ZZFormulaInfo* epfi 
        = new ZZFormulaInfo(formula, zzformulaStr, zzfdnumPreds, zzwt, 
                            zzdefaultWt, zzdomain, zzmln, zzvarNameToIdMap, 
                            zzplusVarMap, zznumAsterisk, zzuniqueVarIndexes,
                            zzhasFullStop, zzreadHardClauseWts, 
                            zzmustHaveWtOrFullStop);
      zzformulaInfos.append(epfi); 
    }

    if (zzwt) { delete zzwt; zzwt = NULL; }
  ;}
    break;

  case 17:
#line 144 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { zzconsumeToken(zztokenList,"@"); ;}
    break;

  case 18:
#line 145 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { zzconsumeToken(zztokenList,"@"); ;}
    break;

  case 20:
#line 152 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
  if (folDbg >= 1) printf(".\n"); zzconsumeToken(zztokenList,"."); 
  zzassert(!zzhasFullStop, "expecting no full stop");
  zzhasFullStop = true;
  zzformulaStr.append(".");
;}
    break;

  case 21:
#line 161 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { if (folDbg >= 1) printf("\\n\n"); zzconsumeToken(zztokenList,"\n"); ;}
    break;

  case 22:
#line 163 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { if (folDbg >= 1) printf("\\r\n"); zzconsumeToken(zztokenList,"\r"); ;}
    break;

  case 24:
#line 169 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { if (folDbg >= 1) printf("\\n\n"); zzconsumeToken(zztokenList,"\n"); ;}
    break;

  case 25:
#line 171 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { if (folDbg >= 1) printf("\\r\n"); zzconsumeToken(zztokenList,"\r"); ;}
    break;

  case 26:
#line 178 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  const char* inc = zztokenList.removeLast();
  const char* str = zztokenList.removeLast();
  const char* nl = zztokenList.removeLast();
  zzassert(strcmp(inc,"#include")==0,"expecting #include keyword");

  if (folDbg >= 1) printf("#include %s ", str);

  string s(str);
  if (s.length() == 0)
  {
    zzerr("empty string used with #include");
    delete [] inc;
    delete [] str;
    delete [] nl;
    break;
  }

  zzassert(s.at(0) == '"' && s.at(s.length()-1) == '"', "no enclosing \"");
  s = s.substr(1, s.length()-2);
  int len = s.length();

  // if it is a .cpp file, then we are dealing with linked-in functions and predicates
  if (s.at(len-4)=='.' && s.at(len-3)=='c' && s.at(len-2)=='p' &&
	  s.at(len-1)=='p') {
	
	zzcompileFunctions(str);
	zzusingLinkedPredicates = true;    
	zzusingLinkedFunctions = true;    
    break;
  }

  FILE* newin = fopen(s.c_str(), "r" );
  if (newin)
  {
    zzinStack.push(ZZFileState(yyin, string(zzinFileName), zznumCharRead, 
                               zzline, zzcolumn)); 
    ungetc('\n', newin); // pretend that file begins with a newline
    zzline = 1;
    zzcolumn = -1;
    zzline--;
    zzinFileName = str;
    yyrestart(newin); 
    zznumCharRead = 0;

    // if it is a .db file containing ground predicates
    if ((s.at(len-3)=='.' && s.at(len-2)=='d' && s.at(len-1)=='b'))
//    	||
//    	(s.at(len-5)=='.' && s.at(len-4)=='f' && s.at(len-3)=='u' &&
//    	s.at(len-2)=='n' && s.at(len-1)=='c'))
    	zzparseGroundPred = true;
  } 
  else
    zzerr("Failed to open file %s.", str);
  
  delete [] inc;
  delete [] str;
  delete [] nl;
;}
    break;

  case 27:
#line 240 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { if (folDbg >= 1) printf("\\n\n"); ;}
    break;

  case 28:
#line 242 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { if (folDbg >= 1) printf("\\r\n"); ;}
    break;

  case 29:
#line 253 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
  zzconsumeToken(zztokenList,"=");
  zzconsumeToken(zztokenList,"{");
  if (folDbg >= 1) printf("= { "); 
  if (folDbg >= 2) printf("type_declarations: constant_declarations\n");
;}
    break;

  case 30:
#line 261 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  if (folDbg >= 1) printf("} ");           
  zzconsumeToken(zztokenList, "}");
  delete [] zztypeName;
  zztypeName = NULL;
;}
    break;

  case 31:
#line 272 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  const char* idf = zztokenList.removeLast();
  if (folDbg >= 1) printf("type t_%s ", idf);
  zzassert(!zzdomain->isType(idf), "expecting type to be undefined");
  int id = zzaddTypeToDomain(zzdomain, idf);
  zzassert(id >= 0,"expecting id >= 0");
  zzassert(zztypeName==NULL,"expecting zztypeName==NULL");
  zztypeName = new char[strlen(idf)+1];
  strcpy(zztypeName, idf);
  delete [] idf;
;}
    break;

  case 32:
#line 285 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  const char* idf = zztokenList.removeLast();
  if (folDbg >= 1) printf("type t_%s ", idf);
  zzassert(zzdomain->isType(idf),"expecting type to be defined");
  //zzwarn("Type %s has been declared before.",idf);
  zzassert(zztypeName==NULL,"expecting zztypeName==NULL");
  zztypeName = new char[strlen(idf)+1];
  strcpy(zztypeName, idf);
  delete [] idf;
;}
    break;

  case 34:
#line 304 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { if (folDbg >= 2) printf("constant_declarations: ZZ_VARIABLE\n"); ;}
    break;

  case 35:
#line 306 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    const char* vsc = zztokenList.removeLast();
    if (folDbg >= 1) printf("cd_%s ", vsc);
    int constId = zzdomain->getConstantId(vsc);
    if (constId < 0)
      zzaddConstantToDomain(vsc, zztypeName);
    else
    {
      const char* prevType = zzdomain->getConstantTypeName(constId);
      if (strcmp(prevType,zztypeName)!=0)
        zzerr("constant %s previously declared to be of type %s is redeclared "
              "to be of type %s", vsc, prevType, zztypeName);
    }
    delete [] vsc;
  ;}
    break;

  case 40:
#line 332 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { zzconsumeToken(zztokenList, ","); ;}
    break;

  case 41:
#line 335 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,"\n");
    zzconsumeToken(zztokenList,",");
    zzconsumeToken(zztokenList,"\n");
  ;}
    break;

  case 42:
#line 342 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,"\n");
    zzconsumeToken(zztokenList,",");
  ;}
    break;

  case 43:
#line 348 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,",");
    zzconsumeToken(zztokenList,"\n");
  ;}
    break;

  case 44:
#line 358 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  if (folDbg >= 2) printf("numeric_type_declarations: \n");
  zzconsumeToken(zztokenList,"=");
  zzconsumeToken(zztokenList,"{");  
;}
    break;

  case 45:
#line 364 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  const char* numStr = zztokenList.removeLast();
  if (folDbg >= 1) printf(" %s ", numStr);
  
  double d = atof(numStr);
  zzinitialNumDeclaration = int(d);
  if (d != zzinitialNumDeclaration) zzerr("constant %s must be an integer", numStr);

  char constStr[100];
  zzcreateIntConstant(constStr, zztypeName, zzinitialNumDeclaration);
  int constId = zzdomain->getConstantId(constStr);
  if (constId < 0) 
    zzaddConstantToDomain(constStr, zztypeName);
  else
  {
    const char* prevType = zzdomain->getConstantTypeName(constId);
    if (strcmp(prevType,zztypeName)!=0)
    {
      char buf[30]; sprintf(buf, "%d", zzinitialNumDeclaration);
      zzerr("constant %s previously declared to be of type %s is redeclared "
            "to be of type %s", buf, prevType, zztypeName);        
    }
  }

  delete [] numStr;
;}
    break;

  case 46:
#line 391 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  zzconsumeToken(zztokenList, ",");
;}
    break;

  case 47:
#line 396 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  zzconsumeToken(zztokenList, "}");
;}
    break;

  case 48:
#line 403 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  //const char* numStr1 = zztokenList.removeLast();
  zzconsumeToken(zztokenList, "...");
  zzconsumeToken(zztokenList, ",");
  const char* numStr2 = zztokenList.removeLast();
  if (folDbg >= 1) printf("= ... , %s } ", numStr2);
  
  double d2 = atof(numStr2);
  int i2 = int(d2);
  if (d2 != i2) zzerr("constant %s must be an integer", numStr2);
  if (zzinitialNumDeclaration > i2)
    zzerr("first constant cannot be larger than last constant %s", numStr2);

  for (int i = zzinitialNumDeclaration + 1; i <= i2; i++)
  {
    char constStr[100];
    zzcreateIntConstant(constStr, zztypeName, i);
    int constId = zzdomain->getConstantId(constStr);
    if (constId < 0) 
      zzaddConstantToDomain(constStr, zztypeName);
    else
    {
      const char* prevType = zzdomain->getConstantTypeName(constId);
      if (strcmp(prevType,zztypeName)!=0)
      {
        char buf[30]; sprintf(buf, "%d", i);
        zzerr("constant %s previously declared to be of type %s is redeclared "
              "to be of type %s", buf, prevType, zztypeName);        
      }
    }
  }

  delete [] numStr2; delete [] zztypeName;
  zztypeName = NULL;
;}
    break;

  case 50:
#line 446 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {  
    zzconsumeToken(zztokenList, ",");
    if (folDbg >= 1) printf(", "); 
    if (folDbg >= 2) printf("single_numeric_types: single_numeric_type\n"); 
  ;}
    break;

  case 53:
#line 457 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  const char* numStr = zztokenList.removeLast();
  if (folDbg >= 1) printf(" %s ", numStr);
  
  double d = atof(numStr);
  int i = int(d);
  if (d != i) zzerr("constant %s must be an integer", numStr);

  char constStr[100];
  zzcreateIntConstant(constStr, zztypeName, i);
  int constId = zzdomain->getConstantId(constStr);
  if (constId < 0) 
    zzaddConstantToDomain(constStr, zztypeName);
  else
  {
    const char* prevType = zzdomain->getConstantTypeName(constId);
    if (strcmp(prevType,zztypeName)!=0)
    {
      char buf[30]; sprintf(buf, "%d", i);
      zzerr("constant %s previously declared to be of type %s is redeclared "
            "to be of type %s", buf, prevType, zztypeName);        
    }
  }

  delete [] numStr;
;}
    break;

  case 54:
#line 493 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  const char* predName= zztokenList.removeLast();

  if (folDbg >= 1) printf("ZZ_PREDICATE pc_%s ", predName);  
    //predicate has not been declared a function
  zzassert(zzdomain->getFunctionId(predName) < 0, 
           "not expecting pred name to be declared as a function name");
  zzassert(zzpredTemplate==NULL,"expecting zzpredTemplate==NULL");
  zzpredTemplate = new PredicateTemplate();
  zzpredTemplate->setName(predName);
  delete [] predName;
;}
    break;

  case 55:
#line 505 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { if (folDbg >= 1) printf("( "); zzconsumeToken(zztokenList,"("); ;}
    break;

  case 56:
#line 506 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { if (folDbg >= 2) printf("predicate_declaration: types\n"); ;}
    break;

  case 57:
#line 508 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {  
  if (folDbg >= 1) printf(") "); 

  zzconsumeToken(zztokenList,")");

  zzassert(zzpredTemplate, "not expecting zzpredTemplate==NULL");
  int id = zzdomain->addPredicateTemplate(zzpredTemplate);
  zzassert(id >= 0, "expecting pred template id >= 0");
  zzpredTemplate->setId(id);
  zzpredTemplate = NULL;
;}
    break;

  case 58:
#line 526 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
  const char* retTypeName = zztokenList.removeLast();
  if (folDbg >= 1) printf("ZZ_FUNCTION t_%s ", retTypeName); 
  if (folDbg >= 2) printf("function_declaration: ZZ_VARIABLE\n"); 

  if (!zzdomain->isType(retTypeName))
  {
    int id = zzaddTypeToDomain(zzdomain, retTypeName);
    zzassert(id >= 0, "expecting retTypeName's id >= 0");
  }

  zzassert(zzfuncTemplate==NULL, "expecting zzfuncTemplate==NULL");
  zzfuncTemplate = new FunctionTemplate();
  zzfuncTemplate->setRetTypeName(retTypeName,zzdomain);

  // We are creating a new predicate as well
  zzassert(zzpredTemplate==NULL,"expecting zzpredTemplate==NULL");
  zzpredTemplate = new PredicateTemplate();
  zzaddType(retTypeName, zzpredTemplate, NULL, false, zzdomain);

  delete [] retTypeName;
;}
    break;

  case 59:
#line 549 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  const char* funcName = zztokenList.removeLast();

  if (folDbg >= 1) printf("fc_%s ", funcName); 
  zzassert(zzdomain->getPredicateId(funcName) < 0, 
           "not expecting func name to be declared as pred name");
  zzfuncTemplate->setName(funcName);

  // Predicate name is PredicateTemplate::ZZ_RETURN_PREFIX + function name
  char* predName;
  predName = (char *)malloc((strlen(PredicateTemplate::ZZ_RETURN_PREFIX) +
  							strlen(funcName) + 1)*sizeof(char));
  strcpy(predName, PredicateTemplate::ZZ_RETURN_PREFIX);
  strcat(predName, funcName);
    	
  // Check that predicate has not been declared a function
  zzassert(zzdomain->getFunctionId(predName) < 0, 
           "not expecting pred name to be declared as a function name");
  zzassert(zzpredTemplate,"expecting zzpredTemplate!=NULL");
  zzpredTemplate->setName(predName);

  delete [] predName;
  delete [] funcName;
;}
    break;

  case 60:
#line 574 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  zzconsumeToken(zztokenList,"(");
  if (folDbg >= 1) printf("( "); 
  if (folDbg >= 2) printf("function_declaration: types\n"); 
;}
    break;

  case 61:
#line 580 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {  
  zzconsumeToken(zztokenList,")");
  if (folDbg >= 1) printf(") "); 
  zzassert(zzfuncTemplate, "expecting zzfuncTemplate != NULL");
  int id = zzdomain->addFunctionTemplate(zzfuncTemplate);
  zzassert(id >= 0, "expecting function template's id >= 0");
  zzfuncTemplate->setId(id);
  zzfuncTemplate = NULL;

  zzassert(zzpredTemplate, "not expecting zzpredTemplate==NULL");
  // Predicate could already be there
  if (!zzdomain->isPredicate(zzpredTemplate->getName()))
  {
    int predId = zzdomain->addPredicateTemplate(zzpredTemplate);
    zzassert(predId >= 0, "expecting pred template id >= 0");
    zzpredTemplate->setId(predId);
  }
  zzpredTemplate = NULL;

;}
    break;

  case 64:
#line 609 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { if (folDbg >= 1) printf(", "); zzconsumeToken(zztokenList,","); ;}
    break;

  case 66:
#line 613 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { if (folDbg >= 1) printf(", "); zzconsumeToken(zztokenList,","); ;}
    break;

  case 70:
#line 621 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {  
  const char* ttype = zztokenList.removeLast();
  if (folDbg >= 1) printf("t_%s ", ttype);
  zzaddType(ttype, zzpredTemplate, NULL, false, zzdomain);
  // If we are in a function definition, then add the type to the function too
  if (zzfuncTemplate)
	zzaddType(ttype, NULL, zzfuncTemplate, false, zzdomain);
  	
  delete [] ttype;
;}
    break;

  case 71:
#line 634 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  const char* varName = zztokenList.removeLast();;
  if (folDbg >= 1) printf("t_%s ", varName);
  zzassert(!zzdomain->isType(varName), "expecting varName to be not a type");
  int id = zzaddTypeToDomain(zzdomain, varName);
  zzassert(id >= 0, "expecting var id >= 0");
  zzaddType(varName, zzpredTemplate, NULL, false, zzdomain);
  // If we are in a function definition, then add the type to the function too
  if (zzfuncTemplate)
	zzaddType(varName, NULL, zzfuncTemplate, false, zzdomain);
  delete [] varName;
;}
    break;

  case 72:
#line 652 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    zztrueFalseUnknown = 't';
  ;}
    break;

  case 73:
#line 656 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,"!"); 
    if (folDbg >= 1) printf("! "); 
    zztrueFalseUnknown = 'f';
  ;}
    break;

  case 74:
#line 662 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,"?"); 
    if (folDbg >= 1) printf("? "); 
    zztrueFalseUnknown = 'u';
  ;}
    break;

  case 75:
#line 673 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  const char* predName = zztokenList.removeLast();
  if (folDbg >= 1) printf("pd_%s ", predName); 
  
  zzcreatePred(zzpred, predName);
  if (zztrueFalseUnknown == 't')      zzpred->setTruthValue(TRUE);
  else if (zztrueFalseUnknown == 'f') zzpred->setTruthValue(FALSE);
  else if (zztrueFalseUnknown == 'u') zzpred->setTruthValue(UNKNOWN);
  else { zzassert(false, "expecting t,f,u"); }

  delete [] predName;
;}
    break;

  case 76:
#line 686 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
  zzconsumeToken(zztokenList,"("); 
  if (folDbg >= 1) printf("( "); 
  if (folDbg >= 2) printf("predicate_definition: constants_in_groundings\n");
;}
    break;

  case 77:
#line 692 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
  zzconsumeToken(zztokenList,")"); 
  if (folDbg >= 1) printf(")\n"); 
  
  zzcheckPredNumTerm(zzpred);
  int predId = zzpred->getId();
  hash_map<int,PredicateHashArray*>::iterator it;
  if ((it=zzpredIdToGndPredMap.find(predId)) == zzpredIdToGndPredMap.end())
    zzpredIdToGndPredMap[predId] = new PredicateHashArray;
  
  PredicateHashArray* pha = zzpredIdToGndPredMap[predId];
  if (pha->append(zzpred) < 0)
  {
    int a = pha->find(zzpred);
    zzassert(a >= 0, "expecting ground predicate to be found");
    string origTvStr = (*pha)[a]->getTruthValueAsStr();
    (*pha)[a]->setTruthValue(zzpred->getTruthValue());
    string newTvStr = (*pha)[a]->getTruthValueAsStr();

    if (zzwarnDuplicates)
    {
      ostringstream oss;
      oss << "Duplicate ground predicate "; zzpred->print(oss, zzdomain); 
      oss << " found. ";
      if (origTvStr.compare(newTvStr) != 0)
        oss << "Changed its truthValue from " << origTvStr << " to " <<newTvStr 
            << endl;
      zzwarn(oss.str().c_str());
    }
    delete zzpred;
  }
  zzpred = NULL;
;}
    break;

  case 78:
#line 730 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
  zzconsumeToken(zztokenList,"="); 
  if (folDbg >= 1) printf("= "); 
  if (folDbg >= 2) printf("function_definition: constants_in_groundings\n");
;}
    break;

  case 79:
#line 736 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  // Predicate name is PredicateTemplate::ZZ_RETURN_PREFIX + function name
  const char* funcName = zztokenList.removeLast();
  char* predName;
  predName = (char *)malloc((strlen(PredicateTemplate::ZZ_RETURN_PREFIX) +
  							strlen(funcName) + 1)*sizeof(char));
  strcpy(predName, PredicateTemplate::ZZ_RETURN_PREFIX);
  strcat(predName, funcName);

  if (folDbg >= 1) printf("pd_%s ", predName); 
  
  zzcreatePred(zzpred, predName);
  zzpred->setTruthValue(TRUE);

  char constName[100];
  char* constString;
  if (zztmpReturnNum) {
  	zzcreateAndCheckIntConstant(zztmpReturnConstant, zzfunc, zzpred, zzdomain, constName);
    if (constName == NULL) {
      constString = (char *)malloc((strlen(zztmpReturnConstant) + 1)*sizeof(char));
      strcpy(constString, zztmpReturnConstant);
    } else {
	  constString = (char *)malloc((strlen(constName) + 1)*sizeof(char));
      strcpy(constString, constName);
    }
  } else {
    constString = (char *)malloc((strlen(zztmpReturnConstant) + 1)*sizeof(char));  
  	strcpy(constString, zztmpReturnConstant);
  }

  //Add return constant parsed earlier
  zzaddConstantToPredFunc(constString);

  zztmpReturnNum = false;
  delete [] zztmpReturnConstant;
  delete [] funcName;
  delete [] predName;
  delete [] constString;
;}
    break;

  case 80:
#line 776 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
  zzconsumeToken(zztokenList,"("); 
  if (folDbg >= 1) printf("( "); 
  if (folDbg >= 2) printf("function_definition: constants_in_groundings\n");
;}
    break;

  case 81:
#line 782 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
  zzconsumeToken(zztokenList,")"); 
  if (folDbg >= 1) printf(")\n"); 
  
  zzcheckPredNumTerm(zzpred);
  int predId = zzpred->getId();
  hash_map<int,PredicateHashArray*>::iterator it;
  if ((it=zzpredIdToGndPredMap.find(predId)) == zzpredIdToGndPredMap.end())
    zzpredIdToGndPredMap[predId] = new PredicateHashArray;
  
  PredicateHashArray* pha = zzpredIdToGndPredMap[predId];
  if (pha->append(zzpred) < 0)
  {
    int a = pha->find(zzpred);
    zzassert(a >= 0, "expecting ground predicate to be found");
    string origTvStr = (*pha)[a]->getTruthValueAsStr();
    (*pha)[a]->setTruthValue(zzpred->getTruthValue());
    string newTvStr = (*pha)[a]->getTruthValueAsStr();

    if (zzwarnDuplicates)
    {
      ostringstream oss;
      oss << "Duplicate ground predicate "; zzpred->print(oss, zzdomain); 
      oss << " found. ";
      if (origTvStr.compare(newTvStr) != 0)
        oss << "Changed its truthValue from " << origTvStr << " to " <<newTvStr 
            << endl;
      zzwarn(oss.str().c_str());
    }
    //delete zzpred;
  }

  // Insert FALSE for all other return values

  zzpred = NULL;
;}
    break;

  case 82:
#line 822 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {     
    const char* constName = zztokenList.removeLast();
    if (folDbg >= 1) printf("cg_%s ", constName);
    zzaddConstantToPredFunc(constName);
    delete [] constName;
  ;}
    break;

  case 83:
#line 830 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {     
    const char* constName = zztokenList.removeLast();
    if (folDbg >= 1) printf("cg_%s ", constName);
    zzaddConstantToPredFunc(constName);
    delete [] constName;
  ;}
    break;

  case 85:
#line 841 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    if (zzconstantMustBeDeclared)
      zzerr("Constant %s must be declared before it is used.",
            zztokenList.back());
  ;}
    break;

  case 86:
#line 847 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    const char* intStr = zztokenList.removeLast();
    char constName[100];
    zzcreateAndCheckIntConstant(intStr, zzfunc, zzpred, zzdomain, constName);
    if (constName == NULL) zztokenList.addLast(intStr);
    else                   zztokenList.addLast(constName);
    delete [] intStr;
  ;}
    break;

  case 87:
#line 856 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    if (zzconstantMustBeDeclared)
      zzerr("Constant %s must be declared before it is used",
            zztokenList.back());
  ;}
    break;

  case 88:
#line 864 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    const char* tmp = zztokenList.removeLast();
	zztmpReturnConstant = (char *)malloc((strlen(tmp) + 1)*sizeof(char));
  	strcpy(zztmpReturnConstant, tmp);
  	zztmpReturnNum = false;
  	delete []tmp;
  	if (folDbg >= 1) printf("ic_%s ", zztmpReturnConstant); 
  ;}
    break;

  case 89:
#line 873 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    if (zzconstantMustBeDeclared)
      zzerr("Constant %s must be declared before it is used.",
            zztokenList.back());
    const char* tmp = zztokenList.removeLast();
	zztmpReturnConstant = (char *)malloc((strlen(tmp) + 1)*sizeof(char));
  	strcpy(zztmpReturnConstant, tmp);
  	zztmpReturnNum = false;
  	delete []tmp;
  	if (folDbg >= 1) printf("ic_%s ", zztmpReturnConstant);
  ;}
    break;

  case 90:
#line 885 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  	const char* tmp = zztokenList.removeLast();
  	zztmpReturnConstant = (char *)malloc((strlen(tmp) + 1)*sizeof(char));
  	strcpy(zztmpReturnConstant, tmp);
  	zztmpReturnNum = true;
  	delete []tmp;
  	if (folDbg >= 1) printf("icnum_%s ", zztmpReturnConstant); 
  ;}
    break;

  case 91:
#line 894 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    if (zzconstantMustBeDeclared)
      zzerr("Constant %s must be declared before it is used",
            zztokenList.back());
    const char* tmp = zztokenList.removeLast();
	zztmpReturnConstant = (char *)malloc((strlen(tmp) + 1)*sizeof(char));
  	strcpy(zztmpReturnConstant, tmp);
  	zztmpReturnNum = false;
  	delete []tmp;
  	if (folDbg >= 1) printf("ic_%s ", zztmpReturnConstant);
  ;}
    break;

  case 93:
#line 912 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    const char* wt = zztokenList.removeLast();
    if (folDbg >= 1) printf("n_%f ", atof(wt));
    if (zzwt) delete zzwt;
    zzwt = new double(atof(wt));
    delete [] wt;
  ;}
    break;

  case 94:
#line 926 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,"("); 
    if (folDbg >= 1) printf("( "); 
    if (folDbg >= 2) printf("sentence: '(' sentence\n");
    zzformulaStr.append("(");
  ;}
    break;

  case 95:
#line 934 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,")"); 
    if (folDbg >= 1) printf(") "); 
    zzformulaStr.append(")");
  ;}
    break;

  case 96:
#line 940 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { if (folDbg >= 2) printf("sentence: atomic_sentence\n"); ;}
    break;

  case 98:
#line 944 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    const char* imp = zztokenList.removeLast(); 
    if (folDbg >= 1) printf("=> ");
    zzformulaStr.append(" => ");
    delete [] imp;
  ;}
    break;

  case 99:
#line 952 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { zzcreateListObjFromTopTwo(zzformulaListObjs, "=>"); ;}
    break;

  case 100:
#line 956 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    const char* eq = zztokenList.removeLast(); 
    if (folDbg >= 1) printf("<=> ");
    zzformulaStr.append(" <=> ");
    delete [] eq;  
  ;}
    break;

  case 101:
#line 964 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { zzcreateListObjFromTopTwo(zzformulaListObjs, "<=>"); ;}
    break;

  case 102:
#line 968 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,"v"); 
    if (folDbg >= 1) printf("v "); 
    zzformulaStr.append(" v ");
  ;}
    break;

  case 103:
#line 975 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { zzcreateListObjFromTopTwo(zzformulaListObjs, "v"); ;}
    break;

  case 104:
#line 979 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,"^"); 
    if (folDbg >= 1) printf("^ "); 
    zzformulaStr.append(" ^ ");
  ;}
    break;

  case 105:
#line 986 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { zzcreateListObjFromTopTwo(zzformulaListObjs, "^"); ;}
    break;

  case 106:
#line 990 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { if (folDbg >= 2) printf("sentence: quantifier\n"); ;}
    break;

  case 107:
#line 992 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    if (folDbg >= 2) printf("sentence: variables\n"); 
    zzformulaListObjs.push(new ListObj);
    pair<StringToStringMap*,int> pr(new StringToStringMap,zzscopeCounter++);
    zzoldNewVarList.push_front(pr);
  ;}
    break;

  case 108:
#line 998 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { if (folDbg >= 2) printf("sentence: sentence\n"); ;}
    break;

  case 109:
#line 1000 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    zzcreateListObjFromTopThree(zzformulaListObjs);
    pair<StringToStringMap*, int> pr = zzoldNewVarList.front();
    zzoldNewVarList.pop_front();
    delete pr.first;
  ;}
    break;

  case 110:
#line 1009 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    zzassert(!zzisNegated,"expecting !zzisNegated");
    zzisNegated = true;

    zzconsumeToken(zztokenList,"!");
    if (folDbg >= 1) printf("! "); 
    if (folDbg >= 2) printf("sentence: sentence\n");
    zzformulaStr.append("!");
  ;}
    break;

  case 111:
#line 1019 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { zzcreateListObjFromTop(zzformulaListObjs, "!"); ;}
    break;

  case 112:
#line 1025 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    const char* fa = zztokenList.removeLast();
    if (folDbg >= 1) printf("FORALL ");
    zzformulaListObjs.push(new ListObj("FORALL"));
    zzformulaStr.append("FORALL ");
    delete [] fa;
  ;}
    break;

  case 113:
#line 1033 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    const char* ex = zztokenList.removeLast();
    if (folDbg >= 1) printf("EXIST "); 
    zzformulaListObjs.push(new ListObj("EXIST"));
    zzformulaStr.append("EXIST ");
    delete [] ex;
  ;}
    break;

  case 114:
#line 1045 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { if (folDbg >= 2) printf("variables: variables\n"); ;}
    break;

  case 115:
#line 1047 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {  
    zzconsumeToken(zztokenList,",");
    if (folDbg >= 1) printf(", "); 
    zzformulaStr.append(",");
  ;}
    break;

  case 117:
#line 1054 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { zzformulaStr.append(" "); ;}
    break;

  case 118:
#line 1059 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  const char* varName = zztokenList.removeLast();  
  if (folDbg >= 1) printf("v_%s ", varName); 

  if (isupper(varName[0])) 
  {
    zzerr("Variable %s must be begin with a lowercase character.", varName);
    ((char*)varName)[0] = tolower(varName[0]);
  }

  int scopeNum = zzoldNewVarList.front().second;
  string newVarName = zzappend(varName,scopeNum);

  StringToStringMap* oldNewVarMap = zzoldNewVarList.front().first;
  (*oldNewVarMap)[varName]=newVarName;
  zzvarNameToIdMap[newVarName] = ZZVarIdType(--zzvarCounter, zzanyTypeId);
  zzformulaListObjs.top()->append(newVarName.c_str());
  zzformulaStr.append(varName); //use the old var name in the orig string
  delete [] varName;
;}
    break;

  case 119:
#line 1084 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    const char* predName = zztokenList.removeLast();
    if (folDbg >= 1) printf("p_%s ", predName); 

    zzfdnumPreds++;
    ListObj* predlo = new ListObj;

	if (PredicateTemplate::isInternalPredicateTemplateName(predName))
	{
	  zzinfixPredName = (char *)malloc((strlen(predName)
    								  	+ 1)*sizeof(char));
	  strcpy(zzinfixPredName, predName);
	  const PredicateTemplate* t = zzdomain->getEmptyPredicateTemplate();
      zzassert(zzpred == NULL,"expecting zzpred==NULL");
      zzpred = new Predicate(t);
      predlo->append(PredicateTemplate::EMPTY_NAME);
	}
	else
	{
      zzcreatePred(zzpred, predName);
      predlo->append(predName);
	}
	
    zzformulaStr.append(predName);
    if(zzisNegated)  { zzpred->setSense(false); zzisNegated = false; }
    zzpredFuncListObjs.push(predlo);
    delete [] predName;
  ;}
    break;

  case 120:
#line 1113 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {  
    zzconsumeToken(zztokenList, "(");
    if (folDbg >= 1) printf("( "); 
    if (folDbg >= 2) printf("atomic_sentence: terms\n"); 
    zzformulaStr.append("(");

  ;}
    break;

  case 121:
#line 1122 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {  
    zzconsumeToken(zztokenList, ")");
    if (folDbg >= 1) printf(") "); 

	  //If an internal pred., then need to determine type
	if (zzinfixPredName)
	{
	  ListObj* predlo = zzpredFuncListObjs.top();
      predlo->replace(PredicateTemplate::EMPTY_NAME, zzinfixPredName);
		// types are possibly unknown
		// If '=' predicate then types are possibly unknown
		//if (strcmp(zzinfixPredName, PredicateTemplate::EQUAL_NAME)==0) {
      int lTypeId = zzgetTypeId(zzpred->getTerm(0), (*predlo)[1]->getStr());
      int rTypeId = zzgetTypeId(zzpred->getTerm(1), (*predlo)[2]->getStr());

      if (lTypeId > 0 && rTypeId > 0) //if both types are known
      {
        if (lTypeId != rTypeId)
          zzerr("The types on the left and right of '=' must match.");
        if (strcmp(zzinfixPredName, PredicateTemplate::EQUAL_NAME)==0)
        {
 	      zzsetEqPredTypeName(lTypeId);
 	    }
 	    else
 	    {
 	      zzsetInternalPredTypeName(zzinfixPredName, lTypeId);
 	    }
      }
      else  // if only one type is known
      if ( (lTypeId<=0 && rTypeId>0) || (lTypeId>0 && rTypeId<=0) )
      {
        int knownTypeId = (lTypeId>0) ? lTypeId : rTypeId;
        if (strcmp(zzinfixPredName, PredicateTemplate::EQUAL_NAME)==0)
        {
          zzsetEqPredTypeName(knownTypeId);
        }
        else
 	    {
 	      zzsetInternalPredTypeName(zzinfixPredName, knownTypeId);
 	    }
        const char* lvarName = (*predlo)[1]->getStr();
        const char* rvarName = (*predlo)[2]->getStr();
        const char* unknownVarName = (lTypeId>0) ?  rvarName : lvarName;
        zzvarNameToIdMap[unknownVarName].typeId_ = knownTypeId;
      }
      else // if both types are unknown
      {
          //both sides must be variables
        const char* lvarName = (*predlo)[1]->getStr();
        const char* rvarName = (*predlo)[2]->getStr();
        lTypeId = zzgetVarTypeId(lvarName);
        rTypeId = zzgetVarTypeId(rvarName);
	
        if (lTypeId > 0 && rTypeId > 0) //if both types are known
        {
          if (lTypeId != rTypeId)
            zzerr("The types of %s and %s on the left and right of "
                   "'=' must match.", lvarName, rvarName);
          if (strcmp(zzinfixPredName, PredicateTemplate::EQUAL_NAME)==0)
          {
          	zzsetEqPredTypeName(lTypeId);
          }
          else
 	      {
 	      	zzsetInternalPredTypeName(zzinfixPredName, lTypeId);
 	      }
        }
        else  // if only one type is known
        if ( (lTypeId<=0 && rTypeId>0) || (lTypeId>0 && rTypeId<=0) )
        {
          int knownTypeId = (lTypeId>0) ? lTypeId : rTypeId;
          if (strcmp(zzinfixPredName, PredicateTemplate::EQUAL_NAME)==0)
          {
          	zzsetEqPredTypeName(knownTypeId);
          }
          else
 	      {
 	      	zzsetInternalPredTypeName(zzinfixPredName, knownTypeId);
 	      }
          const char* unknowVarName = (lTypeId>0) ?  rvarName : lvarName;
          zzvarNameToIdMap[unknowVarName].typeId_ = knownTypeId;
        }
        else
        {      
		  if (strcmp(zzinfixPredName, PredicateTemplate::EQUAL_NAME)==0)
          {
          	string unknownEqName = zzappend(PredicateTemplate::EQUAL_NAME, 
            	                            zzeqTypeCounter++);
          	zzeqPredList.push_back(ZZUnknownEqPredInfo(unknownEqName,lvarName,
                                                       rvarName));
          	predlo->replace(PredicateTemplate::EQUAL_NAME, unknownEqName.c_str());
          }
          else
          {
          	string unknownIntPredName =
          	  zzappendWithUnderscore(zzinfixPredName, zzintPredTypeCounter++);
          	zzintPredList.push_back(ZZUnknownIntPredInfo(unknownIntPredName, lvarName,
                                                       	 rvarName));
          	predlo->replace(zzinfixPredName, unknownIntPredName.c_str());
          }
        }
      }
      zzinfixPredName = NULL;
	  delete [] zzinfixPredName;
	}

    zzcheckPredNumTerm(zzpred);
    delete zzpred;
    zzpred = NULL;
    zzassert(zzpredFuncListObjs.size()==1,
             "expecting zzpredFuncListObjs.size()==1");
    ListObj* predlo = zzpredFuncListObjs.top();
    zzpredFuncListObjs.pop();

    if(zzisAsterisk)
    {
      zzisAsterisk = false;
      ListObj* lo = new ListObj;
      lo->append("*"); lo->append(predlo);
      zzformulaListObjs.push(lo);
    }
    else
      zzformulaListObjs.push(predlo);

    zzformulaStr.append(")");

	// If we have replaced a function inside the predicate
	// then we have to add the conjunction
	while (!zzfuncConjStack.empty())
	{
		// create the second part of the conjunction
		//zzformulaStr.append(" ^ ");
		ListObj* topPredlo = zzfuncConjStack.top();
	    zzfuncConjStack.pop();
		zzformulaListObjs.push(topPredlo);
		zzcreateListObjFromTopTwo(zzformulaListObjs, "^");

	} //while (!zzfuncConjStack.empty())

	if (!zzfuncConjStr.empty())
	{
		zzformulaStr.append(zzfuncConjStr);
		zzfuncConjStr.clear();
	}
	zzfunc = NULL;
  ;}
    break;

  case 122:
#line 1270 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    ++zzfdnumPreds;
    //zzfdisEqualPred = true;
    //const PredicateTemplate* t = zzdomain->getEqualPredicateTemplate();
    const PredicateTemplate* t = zzdomain->getEmptyPredicateTemplate();
        
    zzassert(zzpred == NULL,"expecting zzpred==NULL");
    zzpred = new Predicate(t);

    ListObj* predlo = new ListObj;
    //predlo->append(PredicateTemplate::EQUAL_NAME);
    predlo->append(PredicateTemplate::EMPTY_NAME);
    zzpredFuncListObjs.push(predlo);
    if(zzisNegated)  { zzpred->setSense(false); zzisNegated = false; }

    if (folDbg >= 2) printf("atomic_sentence (left): term\n"); 
  ;}
    break;

  case 123:
#line 1290 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {  
    ListObj* predlo = zzpredFuncListObjs.top();
    predlo->replace(PredicateTemplate::EMPTY_NAME, zzinfixPredName);

	// types are possibly unknown
	// If '=' predicate then types are possibly unknown
	//if (strcmp(zzinfixPredName, PredicateTemplate::EQUAL_NAME)==0) {
      int lTypeId = zzgetTypeId(zzpred->getTerm(0), (*predlo)[1]->getStr());
      int rTypeId = zzgetTypeId(zzpred->getTerm(1), (*predlo)[2]->getStr());

      if (lTypeId > 0 && rTypeId > 0) //if both types are known
      {
        if (lTypeId != rTypeId)
          zzerr("The types on the left and right of '=' must match.");
        if (strcmp(zzinfixPredName, PredicateTemplate::EQUAL_NAME)==0)
        {
 	      zzsetEqPredTypeName(lTypeId);
 	    }
 	    else
 	    {
 	      zzsetInternalPredTypeName(zzinfixPredName, lTypeId);
 	    }
      }
      else  // if only one type is known
      if ( (lTypeId<=0 && rTypeId>0) || (lTypeId>0 && rTypeId<=0) )
      {
        int knownTypeId = (lTypeId>0) ? lTypeId : rTypeId;
        if (strcmp(zzinfixPredName, PredicateTemplate::EQUAL_NAME)==0)
        {
          zzsetEqPredTypeName(knownTypeId);
        }
        else
 	    {
 	      zzsetInternalPredTypeName(zzinfixPredName, knownTypeId);
 	    }
        const char* lvarName = (*predlo)[1]->getStr();
        const char* rvarName = (*predlo)[2]->getStr();
        const char* unknownVarName = (lTypeId>0) ?  rvarName : lvarName;
        zzvarNameToIdMap[unknownVarName].typeId_ = knownTypeId;
      }
      else // if both types are unknown
      {
          //both sides must be variables
        const char* lvarName = (*predlo)[1]->getStr();
        const char* rvarName = (*predlo)[2]->getStr();
        lTypeId = zzgetVarTypeId(lvarName);
        rTypeId = zzgetVarTypeId(rvarName);
	
        if (lTypeId > 0 && rTypeId > 0) //if both types are known
        {
          if (lTypeId != rTypeId)
            zzerr("The types of %s and %s on the left and right of "
                   "'=' must match.", lvarName, rvarName);
          if (strcmp(zzinfixPredName, PredicateTemplate::EQUAL_NAME)==0)
          {
          	zzsetEqPredTypeName(lTypeId);
          }
          else
 	      {
 	      	zzsetInternalPredTypeName(zzinfixPredName, lTypeId);
 	      }
        }
        else  // if only one type is known
        if ( (lTypeId<=0 && rTypeId>0) || (lTypeId>0 && rTypeId<=0) )
        {
          int knownTypeId = (lTypeId>0) ? lTypeId : rTypeId;
          if (strcmp(zzinfixPredName, PredicateTemplate::EQUAL_NAME)==0)
          {
          	zzsetEqPredTypeName(knownTypeId);
          }
          else
 	      {
 	      	zzsetInternalPredTypeName(zzinfixPredName, knownTypeId);
 	      }
          const char* unknowVarName = (lTypeId>0) ?  rvarName : lvarName;
          zzvarNameToIdMap[unknowVarName].typeId_ = knownTypeId;
        }
        else
        {      
		  if (strcmp(zzinfixPredName, PredicateTemplate::EQUAL_NAME)==0)
          {
          	string unknownEqName = zzappend(PredicateTemplate::EQUAL_NAME, 
            	                            zzeqTypeCounter++);
          	zzeqPredList.push_back(ZZUnknownEqPredInfo(unknownEqName,lvarName,
                                                       rvarName));
          	predlo->replace(PredicateTemplate::EQUAL_NAME, unknownEqName.c_str());
          }
          else
          {
          	string unknownIntPredName =
          	  zzappendWithUnderscore(zzinfixPredName, zzintPredTypeCounter++);
          	zzintPredList.push_back(ZZUnknownIntPredInfo(unknownIntPredName, lvarName,
                                                       	 rvarName));
          	predlo->replace(zzinfixPredName, unknownIntPredName.c_str());
          }
        }
      }
	//}
	//else // Infix predicate other than '='
	//{
	  //Only left term could be unknown
	  //const char* leftTerm = (*predlo)[1]->getStr();
	//}
	    
    zzassert(zzpredFuncListObjs.size()==1,
             "expecting zzpredFuncListObjs.size()==1");
    ListObj* topPredlo = zzpredFuncListObjs.top();
    zzpredFuncListObjs.pop();
    zzformulaListObjs.push(topPredlo);
    	
	delete zzpred;
	zzpred = NULL;
	
	// If we have replaced a function inside the predicate
	// then we have to add the conjunction
	while (!zzfuncConjStack.empty())
	{
	  // create the second part of the conjunction
	  //zzformulaStr.append(" ^ ");
		
      ListObj* topPredlo = zzfuncConjStack.top();
	  zzfuncConjStack.pop();
	  zzformulaListObjs.push(topPredlo);
	  zzcreateListObjFromTopTwo(zzformulaListObjs, "^");
	} //while (!zzfuncConjStack.empty())

	if (!zzfuncConjStr.empty())
	{
	  zzformulaStr.append(zzfuncConjStr);
	  zzfuncConjStr.clear();
	}
	zzfunc = NULL;
	zzinfixPredName = NULL;
	delete [] zzinfixPredName;
  ;}
    break;

  case 124:
#line 1429 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  	zzconsumeToken(zztokenList, ">");
    if (folDbg >= 1) printf("> "); 
    if (folDbg >= 2) printf("atomic_sentence (right): term\n"); 
    zzformulaStr.append(" > ");
    zzinfixPredName = (char *)malloc((strlen(PredicateTemplate::GT_NAME)
    								  + 1)*sizeof(char));
  	strcpy(zzinfixPredName, PredicateTemplate::GT_NAME);
  	//zzcreateInternalPredTemplate(zzinfixPredName);
    //const PredicateTemplate* t = zzdomain->getPredicateTemplate(zzinfixPredName);
	//zzpred->setTemplate((PredicateTemplate*)t);
  ;}
    break;

  case 125:
#line 1443 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
   	zzconsumeToken(zztokenList, "<");
    if (folDbg >= 1) printf("< "); 
    if (folDbg >= 2) printf("atomic_sentence (right): term\n"); 
    zzformulaStr.append(" < "); 
    zzinfixPredName = (char *)malloc((strlen(PredicateTemplate::LT_NAME)
    								  + 1)*sizeof(char));
  	strcpy(zzinfixPredName, PredicateTemplate::LT_NAME);
  	//zzcreateInternalPredTemplate(zzinfixPredName);
    //const PredicateTemplate* t = zzdomain->getPredicateTemplate(zzinfixPredName);
	//zzpred->setTemplate((PredicateTemplate*)t);
  ;}
    break;

  case 126:
#line 1457 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  	zzconsumeToken(zztokenList, ">");
    if (folDbg >= 1) printf(">");
    zzformulaStr.append(" >");
    zzconsumeToken(zztokenList, "=");
    if (folDbg >= 1) printf("= "); 
    if (folDbg >= 2) printf("atomic_sentence (right): term\n"); 
    zzformulaStr.append("= ");
    zzinfixPredName = (char *)malloc((strlen(PredicateTemplate::GTEQ_NAME)
    								  + 1)*sizeof(char));
  	strcpy(zzinfixPredName, PredicateTemplate::GTEQ_NAME);
  	//zzcreateInternalPredTemplate(zzinfixPredName);
	//const PredicateTemplate* t = zzdomain->getPredicateTemplate(zzinfixPredName);
	//zzpred->setTemplate((PredicateTemplate*)t);
  ;}
    break;

  case 127:
#line 1474 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
	zzconsumeToken(zztokenList, "<");
    if (folDbg >= 1) printf("<");
    zzformulaStr.append(" <");
    zzconsumeToken(zztokenList, "=");
    if (folDbg >= 1) printf("= "); 
    if (folDbg >= 2) printf("atomic_sentence (right): term\n"); 
    zzformulaStr.append("= ");
    zzinfixPredName = (char *)malloc((strlen(PredicateTemplate::LTEQ_NAME)
    								  + 1)*sizeof(char));
  	strcpy(zzinfixPredName, PredicateTemplate::LTEQ_NAME);
  	//zzcreateInternalPredTemplate(zzinfixPredName);
    //const PredicateTemplate* t = zzdomain->getPredicateTemplate(zzinfixPredName);
	//zzpred->setTemplate((PredicateTemplate*)t);
  ;}
    break;

  case 128:
#line 1491 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  	zzconsumeToken(zztokenList, "=");
    if (folDbg >= 1) printf("= "); 
    if (folDbg >= 2) printf("atomic_sentence (right): term\n"); 
    zzformulaStr.append(" = ");
    zzinfixPredName = (char *)malloc((strlen(PredicateTemplate::EQUAL_NAME)
    								  + 1)*sizeof(char));
  	strcpy(zzinfixPredName, PredicateTemplate::EQUAL_NAME);
    const PredicateTemplate* t = zzdomain->getEqualPredicateTemplate();
	zzpred->setTemplate((PredicateTemplate*)t);
  ;}
    break;

  case 130:
#line 1508 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList, "*");
    if (folDbg >= 1) printf("* "); 
    if (zzisNegated) zzerr("'!' and '*' cannot be used at the same time");
    zznumAsterisk++;
    zzassert(!zzisAsterisk,"expecting !zzisAsterisk");
    zzisAsterisk = true;
    zzformulaStr.append("*");
  ;}
    break;

  case 131:
#line 1525 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {  
    zzconsumeToken(zztokenList, ",");
    if (folDbg >= 1) printf(", "); 
    if (folDbg >= 2) printf("terms: term\n"); 
    // While parsing function, we do not want to append anything to the formula
    if (zzfunc == NULL) zzformulaStr.append(",");
  ;}
    break;

  case 134:
#line 1541 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {  
    zzassert(zzfunc != NULL,"expecting zzfunc != NULL");
    zzcheckFuncNumTerm(zzfunc);
    zzassert(zzpred != NULL, "expecting zzpred != NULL");

	// replace function
	 
	// Append Term funcVar<zzfuncVarCounter> to predicate where
	// function appears and set flag to add conjunction

	// 1. Make new variable
    char* varName;
    char funcVarCounterString[10];
    int funcVarCounterLength =
      sprintf(funcVarCounterString, "%d", zzfuncVarCounter);
    varName = (char *)malloc((strlen(ZZ_FUNCVAR_PREFIX) +
  								funcVarCounterLength + 1)*sizeof(char));
  	strcpy(varName, ZZ_FUNCVAR_PREFIX);
 	strcat(varName, funcVarCounterString);
    ++zzfdnumVars;
    ++zzfuncVarCounter;
    string newVarName = zzgetNewVarName(varName);
      
   	bool rightNumTerms = true;
   	bool rightType = true;
   	int exp, unexp;

	// 2. Create new predicate
	// Predicate name is PredicateTemplate::ZZ_RETURN_PREFIX + function name
	char* predName;
	predName = (char *)malloc((strlen(PredicateTemplate::ZZ_RETURN_PREFIX) +
  	    					   strlen(zzfunc->getName()) + 1)*sizeof(char));
	strcpy(predName, PredicateTemplate::ZZ_RETURN_PREFIX);
 	strcat(predName, zzfunc->getName());
    	
	// Only insert predicate declaration, if not yet declared
	if (zzdomain->getPredicateId(predName) < 0)
	{
	  zzassert(zzpredTemplate==NULL,"expecting zzpredTemplate==NULL");
	  zzpredTemplate = new PredicateTemplate();
	  zzpredTemplate->setName(predName);
			
	  // Register the types
	  // First parameter is the return type
	  const char* ttype = zzfunc->getRetTypeName();
	  zzaddType(ttype, zzpredTemplate, NULL, false, zzdomain);
	
	  // Register the parameter types
	  for (int i = 0; i < zzfunc->getNumTerms(); i++)
	  {
		const char* ttype = zzfunc->getTermTypeAsStr(i);
		zzaddType(ttype, zzpredTemplate, NULL, false, zzdomain);
	  }
	
	  zzassert(zzpredTemplate, "not expecting zzpredTemplate==NULL");
	  int id = zzdomain->addPredicateTemplate(zzpredTemplate);
	  zzassert(id >= 0, "expecting pred template id >= 0");
	  zzpredTemplate->setId(id);
	  zzpredTemplate = NULL;
	}

	Predicate* prevPred = zzpred;
	zzpred = NULL;
    zzfdnumPreds++;
	zzcreatePred(zzpred, predName);
    	
    ListObj* predlo = new ListObj;
    predlo->append(predName);
		
	// Put predicate list object in stack to be used in conjunction later
    zzfuncConjStack.push(predlo);
    zzfuncConjStr.append(" ^ ");
    zzfuncConjStr.append(predName);
	zzfuncConjStr.append("(");
    zzputVariableInPred(varName, folDbg);
    zzfuncConjStack.top()->append(varName);
    zzfuncConjStr.append(varName);
      	
	// Information about the terms is in zzfunc
	for (int i = 0; i < zzfunc->getNumTerms(); i++)
	{
	  zzfuncConjStr.append(", ");
	  Term* term = new Term(*zzfunc->getTerm(i));
	  zzpred->appendTerm(term);

	  const char* name;
	  if (term->getType() == Term::VARIABLE)
	  {
		name = (*zzpredFuncListObjs.top())[i+1]->getStr();
	  }
	  else if (term->getType() == Term::CONSTANT)
	  {
		name = zzdomain->getConstantName(term->getId());
	  }
	  zzpredFuncListObjs.top()->append(name);
	  zzfuncConjStack.top()->append(name);
	  zzfuncConjStr.append(name);
	}
	zzfuncConjStr.append(")");
		
    // 4. Append new variable to function in stack or to predicate
    if (!zzfuncStack.empty())
    {
   	  Function* prevFunc = zzfuncStack.top(); 
	  zzfuncStack.pop();
    
      // check that we have not exceeded the number of terms
      if ((unexp=prevFunc->getNumTerms()) ==
       	  (exp=prevFunc->getTemplate()->getNumTerms()))
      {
       	rightNumTerms = false;
       	zzerr("Wrong number of terms for function %s. "
           	  "Expected %d but given %d", prevFunc->getName(), exp, unexp+1);
      }
      
      int varId = -1;      
      if (rightNumTerms)
      {
       	// check that the variable is of the right type
     	int typeId = prevFunc->getTermTypeAsInt(prevFunc->getNumTerms());
      	rightType = zzcheckRightTypeAndGetVarId(typeId, newVarName.c_str(),
	                                            varId);
      }

      if (rightNumTerms && rightType)
      {
     	prevFunc->appendTerm(new Term(varId, (void*)prevFunc, false));
     	zzpredFuncListObjs.pop();
    	zzpredFuncListObjs.top()->append(newVarName.c_str());
      }
    	      	  
      zzfunc = prevFunc;
    }
	else // function stack is empty, so append to predicate
    {
      // check that we have not exceeded the number of terms
      if ((unexp=prevPred->getNumTerms()) ==
      	  (exp=prevPred->getTemplate()->getNumTerms()))
      {
        rightNumTerms = false;
        zzerr("Wrong number of terms for predicate %s. "
              "Expected %d but given %d", prevPred->getName(), exp, unexp+1);
      }
        
      int varId = -1;
      if (rightNumTerms)
      {
        // check that the variable is of the right type
        int typeId = prevPred->getTermTypeAsInt(prevPred->getNumTerms());
        rightType = zzcheckRightTypeAndGetVarId(typeId, newVarName.c_str(),
        									    varId);
      }
      
	  if (rightNumTerms && rightType)
	  {
		prevPred->appendTerm(new Term(varId, (void*)prevPred, true));

   		// Pop the function from the stack
   		zzoldFuncLo = zzpredFuncListObjs.top();
   		zzpredFuncListObjs.pop();
   		zzpredFuncListObjs.top()->append(newVarName.c_str());
   		zzformulaStr.append(varName);
      }
	  zzfunc = NULL;
        	
    }
    delete [] varName;
	delete [] predName;
	//zzformulaStr.append(")");

	zzpred = prevPred;
  ;}
    break;

  case 135:
#line 1715 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    const char* constName = zztokenList.removeLast();
    if (folDbg >= 1) printf("c2_%s ", constName); 
    zztermIsConstant(constName);
    if (zzfunc) zzfdfuncConstants.append(string(constName));
    else        zzfdconstName = constName;
    delete [] constName;
  ;}
    break;

  case 136:
#line 1725 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    const char* constName = zztokenList.removeLast();
    if (folDbg >= 1) printf("c2_%s ", constName); 
      if (zzconstantMustBeDeclared)
        zzerr("Constant %s must be declared before it is used", constName);
    zztermIsConstant(constName);
    if (zzfunc) zzfdfuncConstants.append(string(constName));
    else        zzfdconstName = constName;
    delete [] constName;
  ;}
    break;

  case 137:
#line 1737 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    const char* intStr = zztokenList.removeLast();
    if (folDbg >= 1) printf("c3_%s ", intStr);

    char constName[100];
    zzcreateAndCheckIntConstant(intStr, zzfunc, zzpred, zzdomain, constName);
    if (constName == NULL) { break; delete [] intStr; }

    zztermIsConstant(constName);
    if (zzfunc) zzfdfuncConstants.append(string(constName));
    else        zzfdconstName = constName;
    delete [] intStr;
  ;}
    break;

  case 138:
#line 1752 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    zztermIsVariable(folDbg);
    if (zzisPlus) zzisPlus = false;
  ;}
    break;

  case 139:
#line 1758 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    zztermIsVariable(folDbg);

    zzconsumeToken(zztokenList, "!");
    if (folDbg >= 1) printf("! "); 
    if (zzisPlus) { zzisPlus = false; zzerr("'+' cannot be used with '!'"); }
    if (zzfunc) zzerr("'+' cannot be used in a function");
    if (zzpred)
    {
      if (!zzpred->getSense())
        zzerr("'!' cannot be used on a variable of a negated predicate.");
      int termIdx = zzpred->getNumTerms()-1;
      if (zzpred->getTerm(termIdx)->isConstant())
        zzerr("'!' cannot be used with a constant term.");
      zzuniqueVarIndexes.append(termIdx);
    }
    else
    { zzexit("zzpred is NULL in 'not' grammar rule"); }

    zzformulaStr.append("!");    
  ;}
    break;

  case 141:
#line 1784 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList, "+");
    if (folDbg >= 1) printf("+ "); 
    zzassert(!zzisPlus,"expecting !zzisPlus");
    zzisPlus = true;
    zzformulaStr.append("+");
  ;}
    break;

  case 142:
#line 1795 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    const char* funcName = zztokenList.removeLast();
    if (folDbg >= 1) printf("f_%s ", funcName);
    if (zzfunc != NULL) { zzfuncStack.push(zzfunc); zzfunc = NULL; }
    ++zzfdnumFuncs;
    zzfdfuncName = funcName;

    ListObj* funclo = new ListObj;

	  // Check if internal function
 	if (FunctionTemplate::isInternalFunctionTemplateName(funcName))
	{
	  zzinfixFuncName = (char *)malloc((strlen(funcName)
    								  	+ 1)*sizeof(char));
	  strcpy(zzinfixFuncName, funcName);
	  const FunctionTemplate* t;
	  if (FunctionTemplate::isInternalFunctionUnaryTemplateName(funcName))
	  	t = zzdomain->getEmptyFunctionUnaryTemplate();
	  else
	  	t = zzdomain->getEmptyFunctionBinaryTemplate();
      zzassert(zzfunc == NULL,"expecting zzfunc==NULL");
      zzfunc = new Function(t);
      funclo->append(FunctionTemplate::EMPTY_FTEMPLATE_NAME);
	}
	else
	{
	  zzcreateFunc(zzfunc, funcName); 
	  funclo->append(funcName);
	}
	
    zzpredFuncListObjs.push(funclo);
    //zzformulaStr.append(funcName);

    delete [] funcName;
  ;}
    break;

  case 143:
#line 1831 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    zzconsumeToken(zztokenList, "(");
    if (folDbg >= 1) printf("( "); 
    if (folDbg >= 2) printf("term: terms\n");
  ;}
    break;

  case 144:
#line 1837 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  	  //If an internal func., then need to determine type
	if (zzinfixFuncName)
	{
	  ListObj* funclo = zzpredFuncListObjs.top();
      funclo->replace(FunctionTemplate::EMPTY_FTEMPLATE_NAME, zzinfixFuncName);
	  const FunctionTemplate* t = zzgetGenericInternalFunctionTemplate(zzinfixFuncName);
		// If in a pred. (not LHS of infix pred.), then we know the return type
	  if (zzpred)
	  	((FunctionTemplate*)t)->setRetTypeId(
	  		zzpred->getTermTypeAsInt(zzpred->getNumTerms()), zzdomain);
	  zzfunc->setTemplate((FunctionTemplate*)t);

		// types are possibly unknown
	  bool unknownTypes = false;
		
		// First element is return type
	  Array<int> typeIds(zzfunc->getNumTerms() + 1);
	  typeIds.append(zzfunc->getRetTypeId());
	  if (typeIds[0] <= 0) unknownTypes = true;
		
		// Then term types
	  for (int i = 1; i <= zzfunc->getNumTerms(); i++)
	  {
	  	typeIds.append(zzgetTypeId(zzfunc->getTerm(i-1), (*funclo)[i]->getStr()));
	  	if (typeIds[i] <= 0) unknownTypes = true;
	  }

		// If all types are known
	  if (!unknownTypes)
	  {
 	  	zzsetInternalFuncTypeName(zzinfixFuncName, typeIds);
	  }
      else // Not all types are known, delay processing
      {
      	Array<string> varNames(typeIds.size());
      	
      	char* varName;
    	char funcVarCounterString[10];
    	int funcVarCounterLength =
      		sprintf(funcVarCounterString, "%d", zzfuncVarCounter);
    	varName = (char *)malloc((strlen(ZZ_FUNCVAR_PREFIX) +
  								  funcVarCounterLength + 1)*sizeof(char));
  		strcpy(varName, ZZ_FUNCVAR_PREFIX);
 		strcat(varName, funcVarCounterString);
	    string newVarName = zzgetNewVarName(varName);
      	
      	varNames.append(newVarName);
      	
      	for (int i = 1; i < typeIds.size(); i++)
      	  varNames[i] = (*funclo)[i]->getStr();

		// Predicate name is PredicateTemplate::ZZ_RETURN_PREFIX + function name
		char* predName;
		predName = (char *)malloc((strlen(PredicateTemplate::ZZ_RETURN_PREFIX) +
  	    					   strlen(zzfunc->getName()) + 1)*sizeof(char));
		strcpy(predName, PredicateTemplate::ZZ_RETURN_PREFIX);
 		strcat(predName, zzinfixFuncName);
 	
        string unknownIntFuncName =
          	  zzappendWithUnderscore(predName, zzintFuncTypeCounter++);
        zzintFuncList.push_back(ZZUnknownIntFuncInfo(unknownIntFuncName, varNames));
        //funclo->replace(zzinfixFuncName, unknownIntFuncName.c_str());
		delete [] predName;
		delete [] varName;
      }
      zzinfixFuncName = NULL;
	  delete [] zzinfixFuncName;
	}
  	
    zzconsumeToken(zztokenList, ")");
    if (folDbg >= 1) printf(") ");
  ;}
    break;

  case 145:
#line 1911 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    // Make an "empty" function
    const char* funcName = FunctionTemplate::EMPTY_FTEMPLATE_NAME;
    if (zzfunc != NULL) { zzfuncStack.push(zzfunc); zzfunc = NULL; }
    ++zzfdnumFuncs;
    zzfdfuncName = funcName;
    //zzcreateFunc(zzfunc, funcName);
	const FunctionTemplate* t = zzdomain->getEmptyFunctionBinaryTemplate();
	zzassert(zzfunc == NULL, "expect zzfunc == NULL");
  	zzfunc = new Function(t);

    ListObj* funclo = new ListObj;
    funclo->append(funcName);
    zzpredFuncListObjs.push(funclo);
  ;}
    break;

  case 146:
#line 1927 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    zzconsumeToken(zztokenList, "(");
    if (folDbg >= 1) printf("( ");
  ;}
    break;

  case 147:
#line 1932 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
    //Replace empty function with function sign just parsed
    //zzcreateInternalFuncTemplate(zzinfixFuncName);
    //const FunctionTemplate* t = zzdomain->getFunctionTemplate(zzinfixFuncName);
	//zzfunc->setTemplate((FunctionTemplate*)t);
    
    //ListObj* funclo = zzpredFuncListObjs.top();
    //funclo->replace(FunctionTemplate::EMPTY_FTEMPLATE_NAME, zzinfixFuncName);
    
    //delete [] zzinfixFuncName;
  ;}
    break;

  case 148:
#line 1944 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
	ListObj* funclo = zzpredFuncListObjs.top();
    funclo->replace(FunctionTemplate::EMPTY_FTEMPLATE_NAME, zzinfixFuncName);
    const FunctionTemplate* t = zzgetGenericInternalFunctionTemplate(zzinfixFuncName);
	  // If in a pred. (not LHS of infix pred.), then we know the return type
	if (zzpred)
	  ((FunctionTemplate*)t)->setRetTypeId(
	  	zzpred->getTermTypeAsInt(zzpred->getNumTerms()), zzdomain);
	zzfunc->setTemplate((FunctionTemplate*)t);

	  // types are possibly unknown
	bool unknownTypes = false;
		
	  // First element is return type
	Array<int> typeIds(zzfunc->getNumTerms() + 1);
	typeIds.append(zzfunc->getRetTypeId());
	if (typeIds[0] <= 0) unknownTypes = true;
		
	  // Then term types
	for (int i = 1; i <= zzfunc->getNumTerms(); i++)
	{
	  typeIds.append(zzgetTypeId(zzfunc->getTerm(i-1), (*funclo)[i]->getStr()));
	  if (typeIds[i] <= 0) unknownTypes = true;
	}

	  // If all types are known
	if (!unknownTypes)
	{
 	  zzsetInternalFuncTypeName(zzinfixFuncName, typeIds);
	}
    else // Not all types are known, delay processing
    {
      Array<string> varNames(typeIds.size());
      	
      char* varName;
      char funcVarCounterString[10];
      int funcVarCounterLength =
      		sprintf(funcVarCounterString, "%d", zzfuncVarCounter);
      varName = (char *)malloc((strlen(ZZ_FUNCVAR_PREFIX) +
  								funcVarCounterLength + 1)*sizeof(char));
  	  strcpy(varName, ZZ_FUNCVAR_PREFIX);
 	  strcat(varName, funcVarCounterString);
	  string newVarName = zzgetNewVarName(varName);
      	
      varNames.append(newVarName);
      	
      for (int i = 1; i < typeIds.size(); i++)
        varNames[i] = (*funclo)[i]->getStr();
		// Predicate name is PredicateTemplate::ZZ_RETURN_PREFIX + function name
	  char* predName;
	  predName = (char *)malloc((strlen(PredicateTemplate::ZZ_RETURN_PREFIX) +
  	       					     strlen(zzfunc->getName()) + 1)*sizeof(char));
	  strcpy(predName, PredicateTemplate::ZZ_RETURN_PREFIX);
 	  strcat(predName, zzinfixFuncName);
 	
      string unknownIntFuncName =
       	  zzappendWithUnderscore(predName, zzintFuncTypeCounter++);
      zzintFuncList.push_back(ZZUnknownIntFuncInfo(unknownIntFuncName, varNames));
        //funclo->replace(zzinfixFuncName, unknownIntFuncName.c_str());
	  delete [] predName;
	  delete [] varName;
    }
    zzinfixFuncName = NULL;
	delete [] zzinfixFuncName;

    zzconsumeToken(zztokenList, ")");
    if (folDbg >= 1) printf(") ");
  ;}
    break;

  case 149:
#line 2017 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  	zzconsumeToken(zztokenList, "+");
    if (folDbg >= 1) printf("+ "); 
    zzinfixFuncName = (char *)malloc((strlen(PredicateTemplate::PLUS_NAME)
    								  + 1)*sizeof(char));
  	strcpy(zzinfixFuncName, PredicateTemplate::PLUS_NAME);
  ;}
    break;

  case 150:
#line 2026 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
   	zzconsumeToken(zztokenList, "-");
    if (folDbg >= 1) printf("- "); 
    zzinfixFuncName = (char *)malloc((strlen(PredicateTemplate::MINUS_NAME)
    								  + 1)*sizeof(char));
  	strcpy(zzinfixFuncName, PredicateTemplate::MINUS_NAME);
  ;}
    break;

  case 151:
#line 2035 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  	zzconsumeToken(zztokenList, "*");
    if (folDbg >= 1) printf("* ");
    zzinfixFuncName = (char *)malloc((strlen(PredicateTemplate::TIMES_NAME)
    								  + 1)*sizeof(char));
  	strcpy(zzinfixFuncName, PredicateTemplate::TIMES_NAME);
  ;}
    break;

  case 152:
#line 2044 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
	zzconsumeToken(zztokenList, "/");
    if (folDbg >= 1) printf("/ ");
    zzinfixFuncName = (char *)malloc((strlen(PredicateTemplate::DIVIDEDBY_NAME)
    								  + 1)*sizeof(char));
  	strcpy(zzinfixFuncName, PredicateTemplate::DIVIDEDBY_NAME);
  ;}
    break;

  case 153:
#line 2053 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"
    {
  	zzconsumeToken(zztokenList, "%");
    if (folDbg >= 1) printf("%% ");
    zzinfixFuncName = (char *)malloc((strlen(PredicateTemplate::MOD_NAME)
    								  + 1)*sizeof(char));
  	strcpy(zzinfixFuncName, PredicateTemplate::MOD_NAME);
  ;}
    break;


    }

  return yyok;
# undef yyerrok
# undef YYABORT
# undef YYACCEPT
# undef YYERROR
# undef YYBACKUP
# undef yyclearin
# undef YYRECOVERING
/* Line 750 of glr.c.  */
#line 3100 "fol.cpp"
}


static void
yyuserMerge (int yyn, YYSTYPE* yy0, YYSTYPE* yy1)
{
  /* `Use' the arguments.  */
  (void) yy0;
  (void) yy1;

  switch (yyn)
    {
      
    }
}

			      /* Bison grammar-table manipulation.  */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}

/** Number of symbols composing the right hand side of rule #RULE. */
static inline int
yyrhsLength (yyRuleNum yyrule)
{
  return yyr2[yyrule];
}

/** Left-hand-side symbol for rule #RULE. */
static inline yySymbol
yylhsNonterm (yyRuleNum yyrule)
{
  return yyr1[yyrule];
}

#define yyis_pact_ninf(yystate) \
  ((yystate) == YYPACT_NINF)

/** True iff LR state STATE has only a default reduction (regardless
 *  of token). */
static inline yybool
yyisDefaultedState (yyStateNum yystate)
{
  return yyis_pact_ninf (yypact[yystate]);
}

/** The default reduction for STATE, assuming it has one. */
static inline yyRuleNum
yydefaultAction (yyStateNum yystate)
{
  return yydefact[yystate];
}

#define yyis_table_ninf(yytable_value) \
  0

/** Set *YYACTION to the action to take in YYSTATE on seeing YYTOKEN.
 *  Result R means
 *    R < 0:  Reduce on rule -R.
 *    R = 0:  Error.
 *    R > 0:  Shift to state R.
 *  Set *CONFLICTS to a pointer into yyconfl to 0-terminated list of
 *  conflicting reductions.
 */
static inline void
yygetLRActions (yyStateNum yystate, int yytoken,
	        int* yyaction, const short int** yyconflicts)
{
  int yyindex = yypact[yystate] + yytoken;
  if (yyindex < 0 || YYLAST < yyindex || yycheck[yyindex] != yytoken)
    {
      *yyaction = -yydefact[yystate];
      *yyconflicts = yyconfl;
    }
  else if (! yyis_table_ninf (yytable[yyindex]))
    {
      *yyaction = yytable[yyindex];
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
  else
    {
      *yyaction = 0;
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
}

static inline yyStateNum
yyLRgotoState (yyStateNum yystate, yySymbol yylhs)
{
  int yyr;
  yyr = yypgoto[yylhs - YYNTOKENS] + yystate;
  if (0 <= yyr && yyr <= YYLAST && yycheck[yyr] == yystate)
    return yytable[yyr];
  else
    return yydefgoto[yylhs - YYNTOKENS];
}

static inline yybool
yyisShiftAction (int yyaction)
{
  return 0 < yyaction;
}

static inline yybool
yyisErrorAction (int yyaction)
{
  return yyaction == 0;
}

				/* GLRStates */

static void
yyaddDeferredAction (yyGLRStack* yystack, yyGLRState* yystate,
		     yyGLRState* rhs, yyRuleNum yyrule)
{
  yySemanticOption* yynewItem;
  yynewItem = &yystack->yynextFree->yyoption;
  yystack->yyspaceLeft -= 1;
  yystack->yynextFree += 1;
  yynewItem->yyisState = yyfalse;
  yynewItem->yystate = rhs;
  yynewItem->yyrule = yyrule;
  yynewItem->yynext = yystate->yysemantics.yyfirstVal;
  yystate->yysemantics.yyfirstVal = yynewItem;
  if (yystack->yyspaceLeft < YYHEADROOM)
    yyexpandGLRStack (yystack);
}

				/* GLRStacks */

/** Initialize SET to a singleton set containing an empty stack. */
static void
yyinitStateSet (yyGLRStateSet* yyset)
{
  yyset->yysize = 1;
  yyset->yycapacity = 16;
  yyset->yystates = (yyGLRState**) YYMALLOC (16 * sizeof yyset->yystates[0]);
  yyset->yystates[0] = NULL;
}

static void yyfreeStateSet (yyGLRStateSet* yyset)
{
  YYFREE (yyset->yystates);
}

/** Initialize STACK to a single empty stack, with total maximum
 *  capacity for all stacks of SIZE. */
static void
yyinitGLRStack (yyGLRStack* yystack, size_t yysize)
{
  yystack->yyerrflag = 0;
  yystack->yyerrState = 0;
  yynerrs = 0;
  yystack->yyspaceLeft = yysize;
  yystack->yynextFree = yystack->yyitems =
    (yyGLRStackItem*) YYMALLOC (yysize * sizeof yystack->yynextFree[0]);
  yystack->yysplitPoint = NULL;
  yystack->yylastDeleted = NULL;
  yyinitStateSet (&yystack->yytops);
}

#define YYRELOC(YYFROMITEMS,YYTOITEMS,YYX,YYTYPE) \
  &((YYTOITEMS) - ((YYFROMITEMS) - (yyGLRStackItem*) (YYX)))->YYTYPE

/** If STACK is expandable, extend it.  WARNING: Pointers into the
    stack from outside should be considered invalid after this call.
    We always expand when there are 1 or fewer items left AFTER an
    allocation, so that we can avoid having external pointers exist
    across an allocation. */
static void
yyexpandGLRStack (yyGLRStack* yystack)
{
#if YYSTACKEXPANDABLE
  yyGLRStack yynewStack;
  yyGLRStackItem* yyp0, *yyp1;
  size_t yysize, yynewSize;
  size_t yyn;
  yysize = yystack->yynextFree - yystack->yyitems;
  if (YYMAXDEPTH <= yysize)
    yyFail (yystack, "Parser stack overflow. Try disambiguating with parentheses");
  yynewSize = 2*yysize;
  if (YYMAXDEPTH < yynewSize)
    yynewSize = YYMAXDEPTH;
  yyinitGLRStack (&yynewStack, yynewSize);
  for (yyp0 = yystack->yyitems, yyp1 = yynewStack.yyitems, yyn = yysize;
       0 < yyn;
       yyn -= 1, yyp0 += 1, yyp1 += 1)
    {
      *yyp1 = *yyp0;
      if (*(yybool *) yyp0)
	{
	  yyGLRState* yys0 = &yyp0->yystate;
	  yyGLRState* yys1 = &yyp1->yystate;
	  if (yys0->yypred != NULL)
	    yys1->yypred =
	      YYRELOC (yyp0, yyp1, yys0->yypred, yystate);
	  if (! yys0->yyresolved && yys0->yysemantics.yyfirstVal != NULL)
	    yys1->yysemantics.yyfirstVal =
	      YYRELOC(yyp0, yyp1, yys0->yysemantics.yyfirstVal, yyoption);
	}
      else
	{
	  yySemanticOption* yyv0 = &yyp0->yyoption;
	  yySemanticOption* yyv1 = &yyp1->yyoption;
	  if (yyv0->yystate != NULL)
	    yyv1->yystate = YYRELOC (yyp0, yyp1, yyv0->yystate, yystate);
	  if (yyv0->yynext != NULL)
	    yyv1->yynext = YYRELOC (yyp0, yyp1, yyv0->yynext, yyoption);
	}
    }
  if (yystack->yysplitPoint != NULL)
    yystack->yysplitPoint = YYRELOC (yystack->yyitems, yynewStack.yyitems,
				 yystack->yysplitPoint, yystate);

  for (yyn = 0; yyn < yystack->yytops.yysize; yyn += 1)
    if (yystack->yytops.yystates[yyn] != NULL)
      yystack->yytops.yystates[yyn] =
	YYRELOC (yystack->yyitems, yynewStack.yyitems,
		 yystack->yytops.yystates[yyn], yystate);
  YYFREE (yystack->yyitems);
  yystack->yyitems = yynewStack.yyitems;
  yystack->yynextFree = yynewStack.yynextFree + yysize;
  yystack->yyspaceLeft = yynewStack.yyspaceLeft - yysize;

#else

  yyFail (yystack, "Parser stack overflow. Try disambiguating with parentheses");
#endif
}

static void
yyfreeGLRStack (yyGLRStack* yystack)
{
  YYFREE (yystack->yyitems);
  yyfreeStateSet (&yystack->yytops);
}

/** Assuming that S is a GLRState somewhere on STACK, update the
 *  splitpoint of STACK, if needed, so that it is at least as deep as
 *  S. */
static inline void
yyupdateSplit (yyGLRStack* yystack, yyGLRState* yys)
{
  if (yystack->yysplitPoint != NULL && yystack->yysplitPoint > yys)
    yystack->yysplitPoint = yys;
}

/** Invalidate stack #K in STACK. */
static inline void
yymarkStackDeleted (yyGLRStack* yystack, int yyk)
{
  if (yystack->yytops.yystates[yyk] != NULL)
    yystack->yylastDeleted = yystack->yytops.yystates[yyk];
  yystack->yytops.yystates[yyk] = NULL;
}

/** Undelete the last stack that was marked as deleted.  Can only be
    done once after a deletion, and only when all other stacks have
    been deleted. */
static void
yyundeleteLastStack (yyGLRStack* yystack)
{
  if (yystack->yylastDeleted == NULL || yystack->yytops.yysize != 0)
    return;
  yystack->yytops.yystates[0] = yystack->yylastDeleted;
  yystack->yytops.yysize = 1;
  YYDPRINTF ((stderr, "Restoring last deleted stack as stack #0.\n"));
  yystack->yylastDeleted = NULL;
}

static inline void
yyremoveDeletes (yyGLRStack* yystack)
{
  size_t yyi, yyj;
  yyi = yyj = 0;
  while (yyj < yystack->yytops.yysize)
    {
      if (yystack->yytops.yystates[yyi] == NULL)
	{
	  if (yyi == yyj)
	    {
	      YYDPRINTF ((stderr, "Removing dead stacks.\n"));
	    }
	  yystack->yytops.yysize -= 1;
	}
      else
	{
	  yystack->yytops.yystates[yyj] = yystack->yytops.yystates[yyi];
	  if (yyj != yyi)
	    {
	      YYDPRINTF ((stderr, "Rename stack %lu -> %lu.\n",
			  (unsigned long int) yyi, (unsigned long int) yyj));
	    }
	  yyj += 1;
	}
      yyi += 1;
    }
}

/** Shift to a new state on stack #K of STACK, corresponding to LR state
 * LRSTATE, at input position POSN, with (resolved) semantic value SVAL. */
static inline void
yyglrShift (yyGLRStack* yystack, int yyk, yyStateNum yylrState, size_t yyposn,
	    YYSTYPE yysval, YYLTYPE* yylocp)
{
  yyGLRStackItem* yynewItem;

  yynewItem = yystack->yynextFree;
  yystack->yynextFree += 1;
  yystack->yyspaceLeft -= 1;
  yynewItem->yystate.yyisState = yytrue;
  yynewItem->yystate.yylrState = yylrState;
  yynewItem->yystate.yyposn = yyposn;
  yynewItem->yystate.yyresolved = yytrue;
  yynewItem->yystate.yypred = yystack->yytops.yystates[yyk];
  yystack->yytops.yystates[yyk] = &yynewItem->yystate;
  yynewItem->yystate.yysemantics.yysval = yysval;
  yylocp->yydummy = 'a'; yynewItem->yystate.yyloc = *yylocp;
  if (yystack->yyspaceLeft < YYHEADROOM)
    yyexpandGLRStack (yystack);
}

/** Shift stack #K of YYSTACK, to a new state corresponding to LR
 *  state YYLRSTATE, at input position YYPOSN, with the (unresolved)
 *  semantic value of YYRHS under the action for YYRULE. */
static inline void
yyglrShiftDefer (yyGLRStack* yystack, int yyk, yyStateNum yylrState,
		 size_t yyposn, yyGLRState* rhs, yyRuleNum yyrule)
{
  yyGLRStackItem* yynewItem;

  yynewItem = yystack->yynextFree;
  yynewItem->yystate.yyisState = yytrue;
  yynewItem->yystate.yylrState = yylrState;
  yynewItem->yystate.yyposn = yyposn;
  yynewItem->yystate.yyresolved = yyfalse;
  yynewItem->yystate.yypred = yystack->yytops.yystates[yyk];
  yynewItem->yystate.yysemantics.yyfirstVal = NULL;
  yystack->yytops.yystates[yyk] = &yynewItem->yystate;
  yystack->yynextFree += 1;
  yystack->yyspaceLeft -= 1;
  yyaddDeferredAction (yystack, &yynewItem->yystate, rhs, yyrule);
}

/** Pop the symbols consumed by reduction #RULE from the top of stack
 *  #K of STACK, and perform the appropriate semantic action on their
 *  semantic values.  Assumes that all ambiguities in semantic values
 *  have been previously resolved. Set *VALP to the resulting value,
 *  and *LOCP to the computed location (if any).  Return value is as
 *  for userAction. */
static inline YYRESULTTAG
yydoAction (yyGLRStack* yystack, int yyk, yyRuleNum yyrule,
	    YYSTYPE* yyvalp, YYLTYPE* yylocp)
{
  int yynrhs = yyrhsLength (yyrule);

  if (yystack->yysplitPoint == NULL)
    {
      /* Standard special case: single stack. */
      yyGLRStackItem* rhs = (yyGLRStackItem*) yystack->yytops.yystates[yyk];
      YYASSERT (yyk == 0);
      yystack->yynextFree -= yynrhs;
      yystack->yyspaceLeft += yynrhs;
      yystack->yytops.yystates[0] = & yystack->yynextFree[-1].yystate;
      return yyuserAction (yyrule, yynrhs, rhs,
			   yyvalp, yylocp, yystack);
    }
  else
    {
      int yyi;
      yyGLRState* yys;
      yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
      yys = yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred
	= yystack->yytops.yystates[yyk];
      for (yyi = 0; yyi < yynrhs; yyi += 1)
	{
	  yys = yys->yypred;
	  YYASSERT (yys);
	}
      yyupdateSplit (yystack, yys);
      yystack->yytops.yystates[yyk] = yys;
      return yyuserAction (yyrule, yynrhs, yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
			   yyvalp, yylocp, yystack);
    }
}

#if !YYDEBUG
# define YY_REDUCE_PRINT(K, Rule)
#else
# define YY_REDUCE_PRINT(K, Rule)	\
do {					\
  if (yydebug)				\
    yy_reduce_print (K, Rule);		\
} while (0)

/*----------------------------------------------------------.
| Report that the RULE is going to be reduced on stack #K.  |
`----------------------------------------------------------*/

static inline void
yy_reduce_print (size_t yyk, yyRuleNum yyrule)
{
  int yyi;
  YYFPRINTF (stderr, "Reducing stack %lu by rule %d (line %lu), ",
	     (unsigned long int) yyk, yyrule - 1,
	     (unsigned long int) yyrline[yyrule]);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytokenName (yyrhs[yyi]));
  YYFPRINTF (stderr, "-> %s\n", yytokenName (yyr1[yyrule]));
}
#endif

/** Pop items off stack #K of STACK according to grammar rule RULE,
 *  and push back on the resulting nonterminal symbol.  Perform the
 *  semantic action associated with RULE and store its value with the
 *  newly pushed state, if FORCEEVAL or if STACK is currently
 *  unambiguous.  Otherwise, store the deferred semantic action with
 *  the new state.  If the new state would have an identical input
 *  position, LR state, and predecessor to an existing state on the stack,
 *  it is identified with that existing state, eliminating stack #K from
 *  the STACK. In this case, the (necessarily deferred) semantic value is
 *  added to the options for the existing state's semantic value.
 */
static inline YYRESULTTAG
yyglrReduce (yyGLRStack* yystack, size_t yyk, yyRuleNum yyrule,
             yybool yyforceEval)
{
  size_t yyposn = yystack->yytops.yystates[yyk]->yyposn;

  if (yyforceEval || yystack->yysplitPoint == NULL)
    {
      YYSTYPE yysval;
      YYLTYPE yyloc;

      YY_REDUCE_PRINT (yyk, yyrule);
      YYCHK (yydoAction (yystack, yyk, yyrule, &yysval, &yyloc));
      yyglrShift (yystack, yyk,
		  yyLRgotoState (yystack->yytops.yystates[yyk]->yylrState,
				 yylhsNonterm (yyrule)),
		  yyposn, yysval, &yyloc);
    }
  else
    {
      size_t yyi;
      int yyn;
      yyGLRState* yys, *yys0 = yystack->yytops.yystates[yyk];
      yyStateNum yynewLRState;

      for (yys = yystack->yytops.yystates[yyk], yyn = yyrhsLength (yyrule);
	   0 < yyn; yyn -= 1)
	{
	  yys = yys->yypred;
	  YYASSERT (yys);
	}
      yyupdateSplit (yystack, yys);
      yynewLRState = yyLRgotoState (yys->yylrState, yylhsNonterm (yyrule));
      YYDPRINTF ((stderr,
		  "Reduced stack %lu by rule #%d; action deferred. Now in state %d.\n",
		  (unsigned long int) yyk, yyrule - 1, yynewLRState));
      for (yyi = 0; yyi < yystack->yytops.yysize; yyi += 1)
	if (yyi != yyk && yystack->yytops.yystates[yyi] != NULL)
	  {
	    yyGLRState* yyp, *yysplit = yystack->yysplitPoint;
	    yyp = yystack->yytops.yystates[yyi];
	    while (yyp != yys && yyp != yysplit && yyp->yyposn >= yyposn)
	      {
		if (yyp->yylrState == yynewLRState && yyp->yypred == yys)
		  {
		    yyaddDeferredAction (yystack, yyp, yys0, yyrule);
		    yymarkStackDeleted (yystack, yyk);
		    YYDPRINTF ((stderr, "Merging stack %lu into stack %lu.\n",
				(unsigned long int) yyk,
				(unsigned long int) yyi));
		    return yyok;
		  }
		yyp = yyp->yypred;
	      }
	  }
      yystack->yytops.yystates[yyk] = yys;
      yyglrShiftDefer (yystack, yyk, yynewLRState, yyposn, yys0, yyrule);
    }
  return yyok;
}

static int
yysplitStack (yyGLRStack* yystack, int yyk)
{
  if (yystack->yysplitPoint == NULL)
    {
      YYASSERT (yyk == 0);
      yystack->yysplitPoint = yystack->yytops.yystates[yyk];
    }
  if (yystack->yytops.yysize >= yystack->yytops.yycapacity)
    {
      yystack->yytops.yycapacity *= 2;
      yystack->yytops.yystates =
	(yyGLRState**) YYREALLOC (yystack->yytops.yystates,
				  (yystack->yytops.yycapacity
				   * sizeof yystack->yytops.yystates[0]));
    }
  yystack->yytops.yystates[yystack->yytops.yysize]
    = yystack->yytops.yystates[yyk];
  yystack->yytops.yysize += 1;
  return yystack->yytops.yysize-1;
}

/** True iff Y0 and Y1 represent identical options at the top level.
 *  That is, they represent the same rule applied to RHS symbols
 *  that produce the same terminal symbols. */
static yybool
yyidenticalOptions (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  if (yyy0->yyrule == yyy1->yyrule)
    {
      yyGLRState *yys0, *yys1;
      int yyn;
      for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
	   yyn = yyrhsLength (yyy0->yyrule);
	   yyn > 0;
	   yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
	if (yys0->yyposn != yys1->yyposn)
	  return yyfalse;
      return yytrue;
    }
  else
    return yyfalse;
}

/** Assuming identicalOptions (Y0,Y1), (destructively) merge the
 *  alternative semantic values for the RHS-symbols of Y1 into the
 *  corresponding semantic value sets of the symbols of Y0. */
static void
yymergeOptionSets (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  yyGLRState *yys0, *yys1;
  int yyn;
  for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
       yyn = yyrhsLength (yyy0->yyrule);
       yyn > 0;
       yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
    if (yys0 == yys1)
      break;
    else if (! yys0->yyresolved && ! yys1->yyresolved)
      {
	yySemanticOption* yyz;
	for (yyz = yys0->yysemantics.yyfirstVal; yyz->yynext != NULL;
	     yyz = yyz->yynext)
	  continue;
	yyz->yynext = yys1->yysemantics.yyfirstVal;
      }
}

/** Y0 and Y1 represent two possible actions to take in a given
 *  parsing state; return 0 if no combination is possible,
 *  1 if user-mergeable, 2 if Y0 is preferred, 3 if Y1 is preferred. */
static int
yypreference (yySemanticOption* y0, yySemanticOption* y1)
{
  yyRuleNum r0 = y0->yyrule, r1 = y1->yyrule;
  int p0 = yydprec[r0], p1 = yydprec[r1];

  if (p0 == p1)
    {
      if (yymerger[r0] == 0 || yymerger[r0] != yymerger[r1])
	return 0;
      else
	return 1;
    }
  if (p0 == 0 || p1 == 0)
    return 0;
  if (p0 < p1)
    return 3;
  if (p1 < p0)
    return 2;
  return 0;
}

static YYRESULTTAG yyresolveValue (yySemanticOption* yyoptionList,
				   yyGLRStack* yystack, YYSTYPE* yyvalp,
				   YYLTYPE* yylocp);

static YYRESULTTAG
yyresolveStates (yyGLRState* yys, int yyn, yyGLRStack* yystack)
{
  YYRESULTTAG yyflag;
  if (0 < yyn)
    {
      YYASSERT (yys->yypred);
      yyflag = yyresolveStates (yys->yypred, yyn-1, yystack);
      if (yyflag != yyok)
	return yyflag;
      if (! yys->yyresolved)
	{
	  yyflag = yyresolveValue (yys->yysemantics.yyfirstVal, yystack,
				   &yys->yysemantics.yysval, &yys->yyloc
				  );
	  if (yyflag != yyok)
	    return yyflag;
	  yys->yyresolved = yytrue;
	}
    }
  return yyok;
}

static YYRESULTTAG
yyresolveAction (yySemanticOption* yyopt, yyGLRStack* yystack,
	         YYSTYPE* yyvalp, YYLTYPE* yylocp)
{
  yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
  int yynrhs;

  yynrhs = yyrhsLength (yyopt->yyrule);
  YYCHK (yyresolveStates (yyopt->yystate, yynrhs, yystack));
  yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred = yyopt->yystate;
  return yyuserAction (yyopt->yyrule, yynrhs,
		       yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
		       yyvalp, yylocp, yystack);
}

#if YYDEBUG
static void
yyreportTree (yySemanticOption* yyx, int yyindent)
{
  int yynrhs = yyrhsLength (yyx->yyrule);
  int yyi;
  yyGLRState* yys;
  yyGLRState* yystates[YYMAXRHS];
  yyGLRState yyleftmost_state;

  for (yyi = yynrhs, yys = yyx->yystate; 0 < yyi; yyi -= 1, yys = yys->yypred)
    yystates[yyi] = yys;
  if (yys == NULL)
    {
      yyleftmost_state.yyposn = 0;
      yystates[0] = &yyleftmost_state;
    }
  else
    yystates[0] = yys;

  if (yyx->yystate->yyposn < yys->yyposn + 1)
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, empty>\n",
	       yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
	       yyx->yyrule);
  else
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, tokens %lu .. %lu>\n",
	       yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
	       yyx->yyrule, (unsigned long int) (yys->yyposn + 1),
	       (unsigned long int) yyx->yystate->yyposn);
  for (yyi = 1; yyi <= yynrhs; yyi += 1)
    {
      if (yystates[yyi]->yyresolved)
	{
	  if (yystates[yyi-1]->yyposn+1 > yystates[yyi]->yyposn)
	    YYFPRINTF (stderr, "%*s%s <empty>\n", yyindent+2, "",
		       yytokenName (yyrhs[yyprhs[yyx->yyrule]+yyi-1]));
	  else
	    YYFPRINTF (stderr, "%*s%s <tokens %lu .. %lu>\n", yyindent+2, "",
		       yytokenName (yyrhs[yyprhs[yyx->yyrule]+yyi-1]),
		       (unsigned long int) (yystates[yyi - 1]->yyposn + 1),
		       (unsigned long int) yystates[yyi]->yyposn);
	}
      else
	yyreportTree (yystates[yyi]->yysemantics.yyfirstVal, yyindent+2);
    }
}
#endif

static void
yyreportAmbiguity (yySemanticOption* yyx0, yySemanticOption* yyx1,
		   yyGLRStack* yystack)
{
  /* `Unused' warnings.  */
  (void) yyx0;
  (void) yyx1;

#if YYDEBUG
  YYFPRINTF (stderr, "Ambiguity detected.\n");
  YYFPRINTF (stderr, "Option 1,\n");
  yyreportTree (yyx0, 2);
  YYFPRINTF (stderr, "\nOption 2,\n");
  yyreportTree (yyx1, 2);
  YYFPRINTF (stderr, "\n");
#endif
  yyFail (yystack, "ambiguity detected");
}


/** Resolve the ambiguity represented by OPTIONLIST, perform the indicated
 *  actions, and return the result. */
static YYRESULTTAG
yyresolveValue (yySemanticOption* yyoptionList, yyGLRStack* yystack,
		YYSTYPE* yyvalp, YYLTYPE* yylocp)
{
  yySemanticOption* yybest;
  yySemanticOption* yyp;
  int yymerge;

  yybest = yyoptionList;
  yymerge = 0;
  for (yyp = yyoptionList->yynext; yyp != NULL; yyp = yyp->yynext)
    {
      if (yyidenticalOptions (yybest, yyp))
	yymergeOptionSets (yybest, yyp);
      else
	switch (yypreference (yybest, yyp))
	  {
	  case 0:
	    yyreportAmbiguity (yybest, yyp, yystack);
	    break;
	  case 1:
	    yymerge = 1;
	    break;
	  case 2:
	    break;
	  case 3:
	    yybest = yyp;
	    yymerge = 0;
	    break;
	  }
    }

  if (yymerge)
    {
      int yyprec = yydprec[yybest->yyrule];
      YYCHK (yyresolveAction (yybest, yystack, yyvalp, yylocp));
      for (yyp = yybest->yynext; yyp != NULL; yyp = yyp->yynext)
	{
	  if (yyprec == yydprec[yyp->yyrule])
	    {
	      YYSTYPE yyval1;
	      YYLTYPE yydummy;
	      YYCHK (yyresolveAction (yyp, yystack, &yyval1, &yydummy));
	      yyuserMerge (yymerger[yyp->yyrule], yyvalp, &yyval1);
	    }
	}
      return yyok;
    }
  else
    return yyresolveAction (yybest, yystack, yyvalp, yylocp);
}

static YYRESULTTAG
yyresolveStack (yyGLRStack* yystack)
{
  if (yystack->yysplitPoint != NULL)
    {
      yyGLRState* yys;
      int yyn;

      for (yyn = 0, yys = yystack->yytops.yystates[0];
	   yys != yystack->yysplitPoint;
	   yys = yys->yypred, yyn += 1)
	continue;
      YYCHK (yyresolveStates (yystack->yytops.yystates[0], yyn, yystack
			     ));
    }
  return yyok;
}

static void
yycompressStack (yyGLRStack* yystack)
{
  yyGLRState* yyp, *yyq, *yyr;

  if (yystack->yytops.yysize != 1 || yystack->yysplitPoint == NULL)
    return;

  for (yyp = yystack->yytops.yystates[0], yyq = yyp->yypred, yyr = NULL;
       yyp != yystack->yysplitPoint;
       yyr = yyp, yyp = yyq, yyq = yyp->yypred)
    yyp->yypred = yyr;

  yystack->yyspaceLeft += yystack->yynextFree - yystack->yyitems;
  yystack->yynextFree = ((yyGLRStackItem*) yystack->yysplitPoint) + 1;
  yystack->yyspaceLeft -= yystack->yynextFree - yystack->yyitems;
  yystack->yysplitPoint = NULL;
  yystack->yylastDeleted = NULL;

  while (yyr != NULL)
    {
      yystack->yynextFree->yystate = *yyr;
      yyr = yyr->yypred;
      yystack->yynextFree->yystate.yypred = & yystack->yynextFree[-1].yystate;
      yystack->yytops.yystates[0] = &yystack->yynextFree->yystate;
      yystack->yynextFree += 1;
      yystack->yyspaceLeft -= 1;
    }
}

static YYRESULTTAG
yyprocessOneStack (yyGLRStack* yystack, int yyk,
	           size_t yyposn, YYSTYPE* yylvalp, YYLTYPE* yyllocp
		  )
{
  int yyaction;
  const short int* yyconflicts;
  yyRuleNum yyrule;
  yySymbol* const yytokenp = yystack->yytokenp;

  while (yystack->yytops.yystates[yyk] != NULL)
    {
      yyStateNum yystate = yystack->yytops.yystates[yyk]->yylrState;
      YYDPRINTF ((stderr, "Stack %d Entering state %d\n", yyk, yystate));

      YYASSERT (yystate != YYFINAL);

      if (yyisDefaultedState (yystate))
	{
	  yyrule = yydefaultAction (yystate);
	  if (yyrule == 0)
	    {
	      YYDPRINTF ((stderr, "Stack %d dies.\n", yyk));
	      yymarkStackDeleted (yystack, yyk);
	      return yyok;
	    }
	  YYCHK (yyglrReduce (yystack, yyk, yyrule, yyfalse));
	}
      else
	{
	  if (*yytokenp == YYEMPTY)
	    {
	      YYDPRINTF ((stderr, "Reading a token: "));
	      yychar = YYLEX;
	      *yytokenp = YYTRANSLATE (yychar);
	      YY_SYMBOL_PRINT ("Next token is", *yytokenp, yylvalp, yyllocp);
	    }
	  yygetLRActions (yystate, *yytokenp, &yyaction, &yyconflicts);

	  while (*yyconflicts != 0)
	    {
	      int yynewStack = yysplitStack (yystack, yyk);
	      YYDPRINTF ((stderr, "Splitting off stack %d from %d.\n",
			  yynewStack, yyk));
	      YYCHK (yyglrReduce (yystack, yynewStack,
				  *yyconflicts, yyfalse));
	      YYCHK (yyprocessOneStack (yystack, yynewStack, yyposn,
					yylvalp, yyllocp));
	      yyconflicts += 1;
	    }

	  if (yyisShiftAction (yyaction))
	    {
	      YYDPRINTF ((stderr, "On stack %d, ", yyk));
	      YY_SYMBOL_PRINT ("shifting", *yytokenp, yylvalp, yyllocp);
	      yyglrShift (yystack, yyk, yyaction, yyposn+1,
			  *yylvalp, yyllocp);
	      YYDPRINTF ((stderr, ", now in state #%d\n",
			  yystack->yytops.yystates[yyk]->yylrState));
	      break;
	    }
	  else if (yyisErrorAction (yyaction))
	    {
	      YYDPRINTF ((stderr, "Stack %d dies.\n", yyk));
	      yymarkStackDeleted (yystack, yyk);
	      break;
	    }
	  else
	    YYCHK (yyglrReduce (yystack, yyk, -yyaction, yyfalse));
	}
    }
  return yyok;
}

static void
yyreportSyntaxError (yyGLRStack* yystack,
		     YYSTYPE* yylvalp, YYLTYPE* yyllocp)
{
  /* `Unused' warnings. */
  (void) yylvalp;
  (void) yyllocp;

  if (yystack->yyerrState == 0)
    {
#if YYERROR_VERBOSE
      yySymbol* const yytokenp = yystack->yytokenp;
      int yyn;
      yyn = yypact[yystack->yytops.yystates[0]->yylrState];
      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  size_t yysize = 0;
	  const char* yyprefix;
	  char* yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += strlen (yyprefix) + strlen (yytokenName (yyx));
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
		yyprefix = " or ";
	      }
	  yysize += (sizeof ("parse error, unexpected ")
		     + strlen (yytokenName (*yytokenp)));
	  yymsg = (char*) YYMALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char* yyp = yymsg;
	      sprintf (yyp, "parse error%s%s",
		       (*yytokenp == YYEMPTY ? "" : ", unexpected "),
		       yytokenName (*yytokenp));
	      yyp += strlen (yyp);
	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			sprintf (yyp, "%s%s", yyprefix, yytokenName (yyx));
			yyp += strlen (yyp);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYFREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("parse error");
      yynerrs += 1;
    }
}

/* Recover from a parse error on YYSTACK, assuming that YYTOKENP,
   YYLVALP, and YYLLOCP point to the syntactic category, semantic
   value, and location of the look-ahead.  */
static void
yyrecoverSyntaxError (yyGLRStack* yystack,
		      YYSTYPE* yylvalp,
		      YYLTYPE* yyllocp ATTRIBUTE_UNUSED
		      )
{
  yySymbol* const yytokenp = yystack->yytokenp;
  size_t yyk;
  int yyj;

  if (yystack->yyerrState == 3)
    /* We just shifted the error token and (perhaps) took some
       reductions.  Skip tokens until we can proceed.  */
    while (yytrue)
      {
	if (*yytokenp == YYEOF)
	  {
	    /* Now pop stack until empty and fail. */
	    while (yystack->yytops.yystates[0] != NULL)
	      {
		yyGLRState *yys = yystack->yytops.yystates[0];

		yydestruct ("Error: popping",
                            yystos[yys->yylrState],
			    &yys->yysemantics.yysval);
		yystack->yytops.yystates[0] = yys->yypred;
		yystack->yynextFree -= 1;
		yystack->yyspaceLeft += 1;
	      }
	    yyFail (yystack, NULL);
	  }
	if (*yytokenp != YYEMPTY)
	  {
	    yydestruct ("Error: discarding",
			*yytokenp, yylvalp);
	  }
	YYDPRINTF ((stderr, "Reading a token: "));
	yychar = YYLEX;
	*yytokenp = YYTRANSLATE (yychar);
	YY_SYMBOL_PRINT ("Next token is", *yytokenp, yylvalp, yyllocp);
	yyj = yypact[yystack->yytops.yystates[0]->yylrState];
	if (yyis_pact_ninf (yyj))
	  return;
	yyj += *yytokenp;
	if (yyj < 0 || YYLAST < yyj || yycheck[yyj] != *yytokenp)
	  {
	    if (yydefact[yystack->yytops.yystates[0]->yylrState] != 0)
	      return;
	  }
	else if (yytable[yyj] != 0 && ! yyis_table_ninf (yytable[yyj]))
	  return;
      }

  /* Reduce to one stack.  */
  for (yyk = 0; yyk < yystack->yytops.yysize; yyk += 1)
    if (yystack->yytops.yystates[yyk] != NULL)
      break;
  if (yyk >= yystack->yytops.yysize)
    yyFail (yystack, NULL);
  for (yyk += 1; yyk < yystack->yytops.yysize; yyk += 1)
    yymarkStackDeleted (yystack, yyk);
  yyremoveDeletes (yystack);
  yycompressStack (yystack);

  /* Now pop stack until we find a state that shifts the error token. */
  yystack->yyerrState = 3;
  while (yystack->yytops.yystates[0] != NULL)
    {
      yyGLRState *yys = yystack->yytops.yystates[0];
      yyj = yypact[yys->yylrState];
      if (! yyis_pact_ninf (yyj))
	{
	  yyj += YYTERROR;
	  if (0 <= yyj && yyj <= YYLAST && yycheck[yyj] == YYTERROR
	      && yyisShiftAction (yytable[yyj]))
	    {
	      /* Shift the error token having adjusted its location.  */
	      YYLTYPE yyerrloc;
	      YY_SYMBOL_PRINT ("Shifting", yystos[yytable[yyj]],
			       yylvalp, &yyerrloc);
	      yyglrShift (yystack, 0, yytable[yyj],
			  yys->yyposn, *yylvalp, &yyerrloc);
	      yys = yystack->yytops.yystates[0];
	      break;
	    }
	}

      yydestruct ("Error: popping",
		  yystos[yys->yylrState],
	          &yys->yysemantics.yysval);
      yystack->yytops.yystates[0] = yys->yypred;
      yystack->yynextFree -= 1;
      yystack->yyspaceLeft += 1;
    }
  if (yystack->yytops.yystates[0] == NULL)
    yyFail (yystack, NULL);
}

#define YYCHK1(YYE)							     \
  do {									     \
    switch (YYE) {							     \
    default:								     \
      break;								     \
    case yyabort:							     \
      yystack.yyerrflag = 1;						     \
      goto yyDone;							     \
    case yyaccept:							     \
      yystack.yyerrflag = 0;						     \
      goto yyDone;							     \
    case yyerr:								     \
      goto yyuser_error;						     \
    }									     \
  } while (0)


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
  yySymbol yytoken;
  yyGLRStack yystack;
  size_t yyposn;


  YYSTYPE* const yylvalp = &yylval;
  YYLTYPE* const yyllocp = &yylloc;

  yyinitGLRStack (&yystack, YYINITDEPTH);
  yystack.yytokenp = &yytoken;

  YYDPRINTF ((stderr, "Starting parse\n"));

  if (setjmp (yystack.yyexception_buffer) != 0)
    goto yyDone;

  yylval = yyval_default;


  yyglrShift (&yystack, 0, 0, 0, yylval, &yylloc);
  yytoken = YYEMPTY;
  yyposn = 0;

  while (yytrue)
    {
      /* For efficiency, we have two loops, the first of which is
	 specialized to deterministic operation (single stack, no
	 potential ambiguity).  */
      /* Standard mode */
      while (yytrue)
	{
	  yyRuleNum yyrule;
	  int yyaction;
	  const short int* yyconflicts;

	  yyStateNum yystate = yystack.yytops.yystates[0]->yylrState;
          YYDPRINTF ((stderr, "Entering state %d\n", yystate));
	  if (yystate == YYFINAL)
	    goto yyDone;
	  if (yyisDefaultedState (yystate))
	    {
	      yyrule = yydefaultAction (yystate);
	      if (yyrule == 0)
		{

		  yyreportSyntaxError (&yystack, yylvalp, yyllocp);
		  goto yyuser_error;
		}
	      YYCHK1 (yyglrReduce (&yystack, 0, yyrule, yytrue));
	    }
	  else
	    {
	      if (yytoken == YYEMPTY)
		{
		  YYDPRINTF ((stderr, "Reading a token: "));
		  yychar = YYLEX;
		  yytoken = YYTRANSLATE (yychar);
                  YY_SYMBOL_PRINT ("Next token is", yytoken, yylvalp, yyllocp);
		}
	      yygetLRActions (yystate, yytoken, &yyaction, &yyconflicts);
	      if (*yyconflicts != 0)
		break;
	      if (yyisShiftAction (yyaction))
		{
		  YY_SYMBOL_PRINT ("Shifting", yytoken, yylvalp, yyllocp);
		  if (yytoken != YYEOF)
		    yytoken = YYEMPTY;
		  yyposn += 1;
		  yyglrShift (&yystack, 0, yyaction, yyposn,
		              yylval, yyllocp);
		  if (0 < yystack.yyerrState)
		    yystack.yyerrState -= 1;
		}
	      else if (yyisErrorAction (yyaction))
		{

		  yyreportSyntaxError (&yystack, yylvalp, yyllocp);
		  goto yyuser_error;
		}
	      else
		YYCHK1 (yyglrReduce (&yystack, 0, -yyaction, yytrue));
	    }
	}

      while (yytrue)
	{
	  int yys;
	  int yyn = yystack.yytops.yysize;
	  for (yys = 0; yys < yyn; yys += 1)
	    YYCHK1 (yyprocessOneStack (&yystack, yys, yyposn,
				       yylvalp, yyllocp));
	  yytoken = YYEMPTY;
	  yyposn += 1;
	  yyremoveDeletes (&yystack);
	  if (yystack.yytops.yysize == 0)
	    {
	      yyundeleteLastStack (&yystack);
	      if (yystack.yytops.yysize == 0)
		yyFail (&yystack, "parse error");
	      YYCHK1 (yyresolveStack (&yystack));
	      YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));

	      yyreportSyntaxError (&yystack, yylvalp, yyllocp);
	      goto yyuser_error;
	    }
	  else if (yystack.yytops.yysize == 1)
	    {
	      YYCHK1 (yyresolveStack (&yystack));
	      YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));
	      yycompressStack (&yystack);
	      break;
	    }
	}
      continue;
    yyuser_error:
      yyrecoverSyntaxError (&yystack, yylvalp, yyllocp);
      yyposn = yystack.yytops.yystates[0]->yyposn;
    }
 yyDone:
  /* On YYABORT, free the lookahead. */
  if (yystack.yyerrflag == 1 && yytoken != YYEMPTY)
    yydestruct ("Error: discarding lookahead",
                yytoken, yylvalp);

  yyfreeGLRStack (&yystack);
  return yystack.yyerrflag;
}

/* DEBUGGING ONLY */
#ifdef YYDEBUG
static void yypstack (yyGLRStack* yystack, int yyk) ATTRIBUTE_UNUSED;
static void yypdumpstack (yyGLRStack* yystack) ATTRIBUTE_UNUSED;

static void
yy_yypstack (yyGLRState* yys)
{
  if (yys->yypred)
    {
      yy_yypstack (yys->yypred);
      fprintf (stderr, " -> ");
    }
  fprintf (stderr, "%d@%lu", yys->yylrState, (unsigned long int) yys->yyposn);
}

static void
yypstates (yyGLRState* yyst)
{
  if (yyst == NULL)
    fprintf (stderr, "<null>");
  else
    yy_yypstack (yyst);
  fprintf (stderr, "\n");
}

static void
yypstack (yyGLRStack* yystack, int yyk)
{
  yypstates (yystack->yytops.yystates[yyk]);
}

#define YYINDEX(YYX)							     \
    ((YYX) == NULL ? -1 : (yyGLRStackItem*) (YYX) - yystack->yyitems)


static void
yypdumpstack (yyGLRStack* yystack)
{
  yyGLRStackItem* yyp;
  size_t yyi;
  for (yyp = yystack->yyitems; yyp < yystack->yynextFree; yyp += 1)
    {
      fprintf (stderr, "%3lu. ", (unsigned long int) (yyp - yystack->yyitems));
      if (*(yybool *) yyp)
	{
	  fprintf (stderr, "Res: %d, LR State: %d, posn: %lu, pred: %ld",
		   yyp->yystate.yyresolved, yyp->yystate.yylrState,
		   (unsigned long int) yyp->yystate.yyposn,
		   (long int) YYINDEX (yyp->yystate.yypred));
	  if (! yyp->yystate.yyresolved)
	    fprintf (stderr, ", firstVal: %ld",
		     (long int) YYINDEX (yyp->yystate.yysemantics.yyfirstVal));
	}
      else
	{
	  fprintf (stderr, "Option. rule: %d, state: %ld, next: %ld",
		   yyp->yyoption.yyrule,
		   (long int) YYINDEX (yyp->yyoption.yystate),
		   (long int) YYINDEX (yyp->yyoption.yynext));
	}
      fprintf (stderr, "\n");
    }
  fprintf (stderr, "Tops:");
  for (yyi = 0; yyi < yystack->yytops.yysize; yyi += 1)
    fprintf (stderr, "%lu: %ld; ", (unsigned long int) yyi,
	     (long int) YYINDEX (yystack->yytops.yystates[yyi]));
  fprintf (stderr, "\n");
}
#endif


#line 2063 "/homes/gws/marcs/workspace/alchemy-release/src/parser/fol.y"


/******************* function definitions ****************************/

  //warnDuplicates set to true by default
bool runYYParser(MLN* const & mln, Domain* const & dom,
                 const char* const & fileName, 
                 const bool& allPredsExceptQueriesAreClosedWorld,
                 const StringHashArray* const & openWorldPredNames,
                 const StringHashArray* const & queryPredNames,
                 const bool& addUnitClauses, const bool& warnDuplicates,
                 const double& defaultWt, const bool& mustHaveWtOrFullStop,
                 const Domain* const & domain0, const bool& lazyInference)
{
  zzinit();
  if (fileName) { yyin = fopen(fileName, "r" ); zzinFileName = fileName; }
  else            yyin = stdin;
  if (yyin == NULL) zzexit("Failed to open file %s.", fileName);

  signal(SIGFPE,zzsigHandler);
  signal(SIGABRT,zzsigHandler);
  signal(SIGILL,zzsigHandler);
  signal(SIGSEGV,zzsigHandler);

  zzwarnDuplicates = warnDuplicates;
  zzdefaultWt = defaultWt;
  zzmustHaveWtOrFullStop = mustHaveWtOrFullStop;

  ungetc('\n', yyin); // pretend that file begin with a newline
  zzmln = mln;
  zzdomain = dom;

  zzanyTypeId = zzdomain->getTypeId(PredicateTemplate::ANY_TYPE_NAME);
  zzassert(zzanyTypeId >= 0, "expecting zzanyTypeId >= 0");

  // Declare internally implemented predicates and functions
  //declareInternalPredicatesAndFunctions();

  zzisParsing = true;
  yyparse();
  zzisParsing = false;

  zzcheckAllTypesHaveConstants(zzdomain);

  // Insert groundings generated from internally implemented functions and predicates
  zzgenerateGroundingsFromInternalPredicatesAndFunctions();

  // Insert groundings generated from linked-in predicates
  if (zzusingLinkedPredicates) zzgenerateGroundingsFromLinkedPredicates(zzdomain);

  // Insert groundings generated from linked-in functions
  if (zzusingLinkedFunctions) zzgenerateGroundingsFromLinkedFunctions(zzdomain);

  if (zzok)
  {
      //append the formulas to MLN and set the weights of the hard clauses
    cout << "Adding clauses to MLN..." << endl;
    zzappendFormulasToMLN(zzformulaInfos, mln, domain0);

    if (addUnitClauses) zzappendUnitClausesToMLN(zzdomain, zzmln, zzdefaultWt);

    zzmln->setClauseInfoPriorMeansToClauseWts();

      // Change the constant ids so that constants of same type are ordered 
      // consecutively. Also ensure that the constants in the mln and map is 
      // consistent with the new constant ids
    zzdomain->reorderConstants(zzmln, zzpredIdToGndPredMap);

    zzmln->compress();

    Array<bool> isClosedWorldArr; 
    zzfillClosedWorldArray(isClosedWorldArr,allPredsExceptQueriesAreClosedWorld,
                           openWorldPredNames, queryPredNames);

    Database* db = new Database(zzdomain, isClosedWorldArr, zzstoreGroundPreds);
    if (lazyInference) db->setLazyFlag();
    zzaddGndPredsToDb(db);
    zzdomain->setDB(db);
    zzdomain->compress();
  }
  
  if (zznumErrors > 0) cout << "Num of errors detected = " << zznumErrors<<endl;

  zzcleanUp();

  signal(SIGFPE,SIG_DFL);
  signal(SIGABRT,SIG_DFL);
  signal(SIGILL,SIG_DFL);
  signal(SIGSEGV,SIG_DFL);

  return zzok;
}



