/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for GLR parsing with Bison,
   Copyright (C) 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

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
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* This is the parser code for GLR (Generalized LR) parser. */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

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
     ZZ_INCLUDE = 261,
     ZZ_PREDICATE = 262,
     ZZ_FUNCTION = 263,
     ZZ_CONSTANT = 264,
     ZZ_VARIABLE = 265,
     ZZ_TYPE = 266,
     ZZ_FORALL = 267,
     ZZ_EXIST = 268,
     ZZ_EQUIV = 269,
     ZZ_IMPLY = 270
   };
#endif
/* Tokens.  */
#define ZZ_NUM 258
#define ZZ_DOTDOTDOT 259
#define ZZ_STRING 260
#define ZZ_INCLUDE 261
#define ZZ_PREDICATE 262
#define ZZ_FUNCTION 263
#define ZZ_CONSTANT 264
#define ZZ_VARIABLE 265
#define ZZ_TYPE 266
#define ZZ_FORALL 267
#define ZZ_EXIST 268
#define ZZ_EQUIV 269
#define ZZ_IMPLY 270




/* Copy the first part of user declarations.  */
#line 66 "../src/parser/fol.y"

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

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
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
#line 142 "fol.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#ifndef YYFREE
# define YYFREE free
#endif
#ifndef YYMALLOC
# define YYMALLOC malloc
#endif
#ifndef YYREALLOC
# define YYREALLOC realloc
#endif

#define YYSIZEMAX ((size_t) -1)

#ifdef __cplusplus
   typedef bool yybool;
#else
   typedef unsigned char yybool;
#endif
#define yytrue 1
#define yyfalse 0

#ifndef YYSETJMP
# include <setjmp.h>
# define YYJMP_BUF jmp_buf
# define YYSETJMP(env) setjmp (env)
# define YYLONGJMP(env, val) longjmp (env, val)
#endif

/*-----------------.
| GCC extensions.  |
`-----------------*/

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (!defined (__GNUC__) || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5) || __STRICT_ANSI__)
#  define __attribute__(Spec) /* empty */
# endif
#endif


#ifdef __cplusplus
# define YYOPTIONAL_LOC(Name) /* empty */
#else
# define YYOPTIONAL_LOC(Name) Name __attribute__ ((__unused__))
#endif

#ifndef YYASSERT
# define YYASSERT(condition) ((void) ((condition) || (abort (), 0)))
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   187

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  86
/* YYNRULES -- Number of rules. */
#define YYNRULES  155
/* YYNRULES -- Number of states. */
#define YYNSTATES  216
/* YYMAXRHS -- Maximum number of symbols on right-hand side of rule. */
#define YYMAXRHS 10
/* YYMAXLEFT -- Maximum number of symbols to the left of a handle
   accessed by $0, $-1, etc., in any rule. */
#define YYMAXLEFT 0

/* YYTRANSLATE(X) -- Bison symbol number corresponding to X.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   270

#define YYTRANSLATE(YYX)						\
  ((YYX <= 0) ? YYEOF :							\
   (unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      24,     2,     2,    27,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    20,     2,     2,     2,    23,     2,     2,
      32,    33,    21,    13,    31,    12,    26,    22,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      36,    28,    35,    34,    25,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    19,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    18,     2,
       2,     2,     2,    29,     2,    30,     2,     2,     2,     2,
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
       5,     6,     7,     8,     9,    10,    11,    14,    15,    16,
      17
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
     191,   192,   196,   197,   201,   202,   204,   205,   207,   209,
     210,   211,   218,   219,   220,   221,   231,   233,   237,   239,
     241,   243,   245,   247,   249,   251,   253,   254,   256,   257,
     262,   263,   266,   267,   273,   274,   280,   281,   287,   288,
     294,   295,   296,   297,   304,   305,   309,   311,   313,   314,
     315,   321,   323,   325,   326,   327,   335,   336,   337,   343,
     345,   347,   350,   353,   355,   358,   359,   361,   362,   367,
     369,   371,   373,   375,   377,   380,   382,   383,   384,   391,
     392,   397,   399,   401,   403,   405
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const signed char yyrhs[] =
{
      38,     0,    -1,    -1,    38,     1,    24,    -1,    38,    44,
      -1,    38,    46,    -1,    38,    48,    44,    -1,    38,    55,
      44,    -1,    38,    63,    44,    -1,    38,    67,    44,    -1,
      38,    42,    80,    81,    -1,    38,    42,    84,    -1,    38,
      42,    44,    -1,    -1,    -1,    -1,    38,    39,    91,    40,
      92,    43,    41,    44,    -1,    25,    -1,    42,    25,    -1,
      -1,    26,    -1,    24,    -1,    27,    -1,    -1,    24,    -1,
      27,    -1,     6,     5,    47,    -1,    24,    -1,    27,    -1,
      -1,    50,    28,    29,    49,    51,    30,    -1,    10,    -1,
      11,    -1,    -1,    -1,    51,    54,    52,    53,    -1,    10,
      -1,     5,    -1,     9,    -1,    -1,    31,    -1,    24,    31,
      24,    -1,    24,    31,    -1,    31,    24,    -1,    -1,    -1,
      -1,    50,    28,    29,    56,     3,    57,    31,    58,    59,
      30,    -1,     4,    31,     3,    -1,    60,    -1,    -1,    62,
      31,    61,    60,    -1,    62,    -1,     3,    -1,    -1,    -1,
      -1,    10,    64,    32,    65,    72,    66,    33,    -1,    -1,
      -1,    -1,    71,    68,    10,    69,    32,    70,    72,    33,
      -1,    11,    -1,    10,    -1,    -1,    72,    31,    73,    75,
      -1,    -1,    72,    31,    74,    77,    -1,    75,    -1,    77,
      -1,    -1,    11,    76,    79,    -1,    -1,    10,    78,    79,
      -1,    -1,    20,    -1,    -1,    20,    -1,    34,    -1,    -1,
      -1,     7,    82,    32,    83,    88,    33,    -1,    -1,    -1,
      -1,    90,    28,    85,     8,    86,    32,    87,    88,    33,
      -1,    89,    -1,    88,    54,    89,    -1,     9,    -1,     5,
      -1,     3,    -1,    10,    -1,     9,    -1,     5,    -1,     3,
      -1,    10,    -1,    -1,     3,    -1,    -1,    32,    93,    92,
      33,    -1,    -1,    94,   108,    -1,    -1,    92,    17,    95,
      45,    92,    -1,    -1,    92,    16,    96,    45,    92,    -1,
      -1,    92,    18,    97,    45,    92,    -1,    -1,    92,    19,
      98,    45,    92,    -1,    -1,    -1,    -1,    99,   103,   100,
     104,   101,    92,    -1,    -1,    20,   102,    92,    -1,    14,
      -1,    15,    -1,    -1,    -1,   105,   107,    31,   106,   104,
      -1,   107,    -1,    10,    -1,    -1,    -1,   114,     7,   109,
      32,   110,   115,    33,    -1,    -1,    -1,   111,   117,   113,
     112,   117,    -1,    35,    -1,    36,    -1,    35,    28,    -1,
      36,    28,    -1,    28,    -1,    20,    28,    -1,    -1,    21,
      -1,    -1,   115,    31,   116,   117,    -1,   117,    -1,     9,
      -1,     5,    -1,     3,    -1,    10,    -1,    13,    10,    -1,
     118,    -1,    -1,    -1,     8,   119,    32,   120,   115,    33,
      -1,    -1,   117,   122,   121,   117,    -1,    13,    -1,    12,
      -1,    21,    -1,    22,    -1,    23,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   113,   113,   115,   126,   127,   128,   129,   130,   131,
     132,   134,   136,   140,   145,   150,   139,   197,   198,   201,
     204,   214,   216,   219,   222,   224,   230,   290,   292,   303,
     301,   321,   334,   349,   354,   353,   374,   374,   374,   379,
     382,   384,   391,   397,   408,   414,   441,   406,   452,   489,
     496,   494,   503,   506,   543,   555,   556,   542,   576,   599,
     624,   575,   653,   653,   659,   658,   663,   662,   665,   666,
     671,   670,   683,   682,   699,   701,   718,   721,   727,   739,
     752,   738,   796,   802,   847,   794,   892,   900,   910,   911,
     917,   926,   934,   943,   955,   964,   979,   982,   997,   996,
    1011,  1011,  1015,  1014,  1027,  1026,  1039,  1038,  1050,  1049,
    1061,  1063,  1069,  1061,  1080,  1079,  1095,  1103,  1116,  1118,
    1116,  1125,  1129,  1156,  1186,  1155,  1345,  1364,  1345,  1529,
    1543,  1557,  1574,  1591,  1604,  1627,  1629,  1646,  1644,  1655,
    1683,  1693,  1705,  1720,  1726,  1737,  1934,  1970,  1933,  2054,
    2053,  2138,  2147,  2156,  2165,  2174
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ZZ_NUM", "ZZ_DOTDOTDOT", "ZZ_STRING",
  "ZZ_INCLUDE", "ZZ_PREDICATE", "ZZ_FUNCTION", "ZZ_CONSTANT",
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
  "type_code", "@18", "variable_code", "@19", "exist_unique", "pd_not_qs",
  "predicate_definition", "@20", "@21", "function_definition", "@22",
  "@23", "@24", "constants_in_groundings",
  "is_constant_string_num_variable", "function_return_constant", "weight",
  "sentence", "@25", "@26", "@27", "@28", "@29", "@30", "@31", "@32",
  "@33", "@34", "quantifier", "variables", "@35", "@36", "quant_variable",
  "atomic_sentence", "@37", "@38", "@39", "@40", "internal_predicate_sign",
  "asterisk", "terms", "@41", "term", "function_term", "@42", "@43", "@44",
  "internal_function_sign", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    37,    38,    38,    38,    38,    38,    38,    38,    38,
      38,    38,    38,    39,    40,    41,    38,    42,    42,    43,
      43,    44,    44,    45,    45,    45,    46,    47,    47,    49,
      48,    50,    50,    51,    52,    51,    53,    53,    53,    54,
      54,    54,    54,    54,    56,    57,    58,    55,    59,    59,
      61,    60,    60,    62,    64,    65,    66,    63,    68,    69,
      70,    67,    71,    71,    73,    72,    74,    72,    72,    72,
      76,    75,    78,    77,    79,    79,    80,    80,    80,    82,
      83,    81,    85,    86,    87,    84,    88,    88,    89,    89,
      89,    89,    90,    90,    90,    90,    91,    91,    93,    92,
      94,    92,    95,    92,    96,    92,    97,    92,    98,    92,
      99,   100,   101,    92,   102,    92,   103,   103,   105,   106,
     104,   104,   107,   109,   110,   108,   111,   112,   108,   113,
     113,   113,   113,   113,   113,   114,   114,   116,   115,   115,
     117,   117,   117,   117,   117,   117,   119,   120,   118,   121,
     118,   122,   122,   122,   122,   122
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
       0,     3,     0,     3,     0,     1,     0,     1,     1,     0,
       0,     6,     0,     0,     0,     9,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     0,     4,
       0,     2,     0,     5,     0,     5,     0,     5,     0,     5,
       0,     0,     0,     6,     0,     3,     1,     1,     0,     0,
       5,     1,     1,     0,     0,     7,     0,     0,     5,     1,
       1,     2,     2,     1,     2,     0,     1,     0,     4,     1,
       1,     1,     1,     1,     2,     1,     0,     0,     6,     0,
       4,     1,     1,     1,     1,     1
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     1,     0,     1,     0,     1,     0,     1,
       0,     0,     0,     2,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0
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
       0,     0,     0,     0,     0,     0
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error.  */
static const unsigned char yydefact[] =
{
       2,     0,     1,     0,     0,    31,    32,    21,    17,    22,
      96,    76,     4,     5,     0,     0,     0,     0,     0,    58,
       3,     0,     0,    97,    14,    94,    93,    92,    95,    77,
      18,    78,    12,     0,    11,     0,     6,     0,     7,     8,
       9,     0,    27,    28,    26,    55,   100,    79,    10,    82,
      29,    59,     0,   114,    98,    19,   126,     0,     0,     0,
      33,     0,     0,    72,    70,    56,    68,    69,   100,   100,
     104,   102,   106,   108,    20,    15,   136,   101,     0,     0,
     116,   117,   111,    80,    83,    39,    45,    60,    74,    74,
      64,     0,   115,     0,    23,    23,    23,    23,     0,   142,
     141,   146,   140,   143,     0,     0,   145,   123,     0,     0,
       0,     0,    30,    40,    34,     0,     0,    75,    73,    71,
       0,     0,    57,    99,    24,    25,   100,   100,   100,   100,
      16,     0,   144,   152,   151,     0,   153,   154,   155,   133,
     129,   130,   127,   149,     0,   122,   112,     0,   121,    90,
      89,    88,    91,    39,    86,    84,    42,    43,     0,    46,
       0,    65,    67,   105,   103,   107,   109,   147,   134,   131,
     132,     0,     0,   124,   100,     0,    81,     0,     0,    41,
      37,    38,    36,    35,     0,    61,     0,   128,   150,     0,
     113,   119,    87,    39,    53,     0,     0,    49,    52,     0,
     139,     0,     0,    85,     0,    47,    50,   137,   148,   125,
     120,    48,     0,     0,    51,   138
};

/* YYPDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     1,    10,    46,    98,    11,    75,    12,   126,    13,
      44,    14,    60,    15,    85,   158,   183,   177,    16,    61,
     115,   184,   196,   197,   212,   198,    17,    22,    52,    91,
      18,    41,    62,   116,    19,    65,   120,   121,    66,    89,
      67,    88,   118,    33,    48,    58,   109,    34,    59,   110,
     178,   153,   154,    35,    24,    55,    69,    56,    95,    94,
      96,    97,    57,   108,   174,    68,    82,   146,   147,   202,
     148,    77,   144,   189,    78,   171,   142,    79,   199,   213,
     200,   106,   131,   186,   172,   143
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -79
static const short int yypact[] =
{
     -79,    11,   -79,   -19,     8,     5,    13,   -79,   -79,   -79,
      27,    24,   -79,   -79,   -20,    26,   -20,   -20,   -20,   -79,
     -79,    23,     9,   -79,   -79,   -79,   -79,   -79,   -79,   -79,
     -79,   -79,   -79,    73,   -79,    87,   -79,    59,   -79,   -79,
     -79,   123,   -79,   -79,   -79,   -79,    25,   -79,   -79,   -79,
     131,   -79,    81,   -79,   -79,    93,    21,    89,   107,   136,
     -79,   143,   116,   -79,   -79,   118,   -79,   -79,    25,    25,
     -79,   -79,   -79,   -79,   -79,   -79,   -79,   -79,    92,   144,
     -79,   -79,   -79,   -79,   -79,    22,   -79,   -79,   130,   130,
     142,   120,   -79,    51,   102,   102,   102,   102,   -20,   -79,
     -79,   -79,   -79,   -79,   145,    43,   -79,   -79,   146,   111,
     122,   126,   -79,   134,   -79,   128,    81,   -79,   -79,   -79,
     149,   151,   -79,   -79,   -79,   -79,    25,    25,    25,    25,
     -79,   132,   -79,   -79,   -79,   135,   -79,   -79,   -79,   -79,
     137,   138,   -79,   -79,   139,   -79,   -79,   152,   -79,   -79,
     -79,   -79,   -79,    65,   -79,   -79,   148,   -79,   108,   -79,
     109,   -79,   -79,   113,    56,   150,   -79,   -79,   -79,   -79,
     -79,    92,    92,   -79,    25,   147,   -79,   111,   111,   -79,
     -79,   -79,   -79,   -79,   124,   -79,    92,    60,    64,    92,
     106,   -79,   -79,    75,   -79,   153,   140,   -79,   154,   110,
      60,   114,   157,   -79,   165,   -79,   -79,   -79,   -79,   -79,
     -79,   -79,   170,    92,   -79,    60
};

/* YYPGOTO[NTERM-NUM].  */
static const signed char yypgoto[] =
{
     -79,   -79,   -79,   -79,   -79,   -79,   -79,    -8,    41,   -79,
     -79,   -79,   -79,   -79,   -79,   -79,   -79,    90,   -79,   -79,
     -79,   -79,   -79,   -38,   -79,   -79,   -79,   -79,   -79,   -79,
     -79,   -79,   -79,   -79,   -79,    61,   -79,   -79,    62,   -79,
      55,   -79,    91,   -79,   -79,   -79,   -79,   -79,   -79,   -79,
     -79,     1,     4,   -79,   -79,   -67,   -79,   -79,   -79,   -79,
     -79,   -79,   -79,   -79,   -79,   -79,   -79,   -16,   -79,   -79,
      36,   -79,   -79,   -79,   -79,   -79,   -79,   -79,    -2,   -79,
     -78,   -79,   -79,   -79,   -79,   -79
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, parse error.  */
#define YYTABLE_NINF -136
static const short int yytable[] =
{
     105,    92,    93,    32,     7,    20,    36,     9,    38,    39,
      40,     2,     3,    21,   -13,   -63,   -13,     4,   -13,   -13,
     -13,     5,     6,   -62,   -13,   -13,   -13,    25,  -135,    26,
      23,   -13,   -13,    27,    28,     7,     8,   -54,     9,  -110,
    -110,    45,    76,   -13,    29,    53,   111,    42,     7,    30,
      43,     9,   112,   113,    37,   133,   134,    54,    31,   163,
     164,   165,   166,   135,   136,   137,   138,    70,    71,    72,
      73,   139,   133,   134,    72,    73,   133,   134,   140,   141,
      47,   136,   137,   138,   123,   136,   137,   138,    50,   111,
     130,    63,    64,   187,   188,    99,   113,   100,   176,   111,
     101,   102,   103,    80,    81,   104,   113,   190,   203,    70,
      71,    72,    73,   180,   149,    49,   150,   181,   182,    74,
     151,   152,    70,    71,    72,    73,   124,   194,   195,   125,
      71,    72,    73,    51,   -44,   215,   127,   128,   129,    83,
      90,   207,   185,   208,    84,   207,    86,   209,    87,    90,
     117,   107,   -66,   122,   155,   132,   145,   156,   157,   159,
      64,    63,   145,   168,   167,   169,   170,   145,   211,    73,
     205,   173,   179,   194,   214,   114,   162,   160,   191,   193,
     119,   192,   161,   175,   204,   206,   210,   201
};

/* YYCONFLP[YYPACT[STATE-NUM]] -- Pointer into YYCONFL of start of
   list of conflicting reductions corresponding to action entry for
   state STATE-NUM in yytable.  0 means no conflicts.  The list in
   yyconfl is terminated by a rule number of 0.  */
static const unsigned char yyconflp[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     1,     0,     0,     0,     0,     0,     0,     0,     0,
       3,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     9,     0,     0,
       0,     0,     0,     0,     0,    11,    13,    15,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    17,    19,    21,    23,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0
};

/* YYCONFL[I] -- lists of conflicting rule numbers, each terminated by
   0, pointed into by YYCONFLP.  */
static const short int yyconfl[] =
{
       0,    13,     0,    96,     0,   118,     0,   150,     0,   150,
       0,   150,     0,   150,     0,   150,     0,   113,     0,   113,
       0,   113,     0,   113,     0,   118,     0
};

static const unsigned char yycheck[] =
{
      78,    68,    69,    11,    24,    24,    14,    27,    16,    17,
      18,     0,     1,     5,     3,    10,     5,     6,     7,     8,
       9,    10,    11,    10,    13,    14,    15,     3,     7,     5,
       3,    20,    21,     9,    10,    24,    25,    32,    27,    14,
      15,    32,    21,    32,    20,    20,    24,    24,    24,    25,
      27,    27,    30,    31,    28,    12,    13,    32,    34,   126,
     127,   128,   129,    20,    21,    22,    23,    16,    17,    18,
      19,    28,    12,    13,    18,    19,    12,    13,    35,    36,
       7,    21,    22,    23,    33,    21,    22,    23,    29,    24,
      98,    10,    11,   171,   172,     3,    31,     5,    33,    24,
       8,     9,    10,    14,    15,    13,    31,   174,    33,    16,
      17,    18,    19,     5,     3,    28,     5,     9,    10,    26,
       9,    10,    16,    17,    18,    19,    24,     3,     4,    27,
      17,    18,    19,    10,     3,   213,    95,    96,    97,    32,
      31,    31,    33,    33,     8,    31,     3,    33,    32,    31,
      20,     7,    10,    33,    32,    10,    10,    31,    24,    31,
      11,    10,    10,    28,    32,    28,    28,    10,     3,    19,
      30,    32,    24,     3,   212,    85,   121,   116,    31,   178,
      89,   177,   120,   147,    31,    31,   202,   189
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    38,     0,     1,     6,    10,    11,    24,    25,    27,
      39,    42,    44,    46,    48,    50,    55,    63,    67,    71,
      24,     5,    64,     3,    91,     3,     5,     9,    10,    20,
      25,    34,    44,    80,    84,    90,    44,    28,    44,    44,
      44,    68,    24,    27,    47,    32,    40,     7,    81,    28,
      29,    10,    65,    20,    32,    92,    94,    99,    82,    85,
      49,    56,    69,    10,    11,    72,    75,    77,   102,    93,
      16,    17,    18,    19,    26,    43,    21,   108,   111,   114,
      14,    15,   103,    32,     8,    51,     3,    32,    78,    76,
      31,    66,    92,    92,    96,    95,    97,    98,    41,     3,
       5,     8,     9,    10,    13,   117,   118,     7,   100,    83,
      86,    24,    30,    31,    54,    57,    70,    20,    79,    79,
      73,    74,    33,    33,    24,    27,    45,    45,    45,    45,
      44,   119,    10,    12,    13,    20,    21,    22,    23,    28,
      35,    36,   113,   122,   109,    10,   104,   105,   107,     3,
       5,     9,    10,    88,    89,    32,    31,    24,    52,    31,
      72,    75,    77,    92,    92,    92,    92,    32,    28,    28,
      28,   112,   121,    32,   101,   107,    33,    54,    87,    24,
       5,     9,    10,    53,    58,    33,   120,   117,   117,   110,
      92,    31,    89,    88,     3,     4,    59,    60,    62,   115,
     117,   115,   106,    33,    31,    30,    31,    31,    33,    33,
     104,     3,    61,   116,    60,   117
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
   to be completed by functions that call yyexpandGLRStack before the
   stack is expanded, thus insuring that all necessary pointers get
   properly redirected to new data. */
#define YYHEADROOM 2

#ifndef YYSTACKEXPANDABLE
# if (! defined (__cplusplus) \
      || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL))
#  define YYSTACKEXPANDABLE 1
# else
#  define YYSTACKEXPANDABLE 0
# endif
#endif

#if YYERROR_VERBOSE

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
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
static size_t
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
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
    return strlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* !YYERROR_VERBOSE */

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
  /** Type tag: always true. */
  yybool yyisState;
  /** Type tag for yysemantics. If true, yysval applies, otherwise
   *  yyfirstVal applies. */
  yybool yyresolved;
  /** Number of corresponding LALR(1) machine state. */
  yyStateNum yylrState;
  /** Preceding state in this stack */
  yyGLRState* yypred;
  /** Source position of the first token produced by my symbol */
  size_t yyposn;
  union {
    /** First in a chain of alternative reductions producing the
     *  non-terminal corresponding to this state, threaded through
     *  yynext. */
    yySemanticOption* yyfirstVal;
    /** Semantic value for this state. */
    YYSTYPE yysval;
  } yysemantics;
  /** Source location for this state. */
  YYLTYPE yyloc;
};

struct yyGLRStateSet {
  yyGLRState** yystates;
  size_t yysize, yycapacity;
};

struct yySemanticOption {
  /** Type tag: always false. */
  yybool yyisState;
  /** Rule number for this reduction */
  yyRuleNum yyrule;
  /** The last RHS state in the list of states to be reduced. */
  yyGLRState* yystate;
  /** Next sibling in chain of options. To facilitate merging,
   *  options are chained in decreasing order by address. */
  yySemanticOption* yynext;
};

/** Type of the items in the GLR stack. The yyisState field
 *  indicates which item of the union is valid. */
union yyGLRStackItem {
  yyGLRState yystate;
  yySemanticOption yyoption;
};

struct yyGLRStack {
  int yyerrState;


  yySymbol* yytokenp;
  YYJMP_BUF yyexception_buffer;
  yyGLRStackItem* yyitems;
  yyGLRStackItem* yynextFree;
  size_t yyspaceLeft;
  yyGLRState* yysplitPoint;
  yyGLRState* yylastDeleted;
  yyGLRStateSet yytops;
};

static void yyexpandGLRStack (yyGLRStack* yystack);

static void yyFail (yyGLRStack* yystack, const char* yymsg)
  __attribute__ ((__noreturn__));
static void
yyFail (yyGLRStack* yystack, const char* yymsg)
{
  if (yymsg != NULL)
    yyerror (yymsg);
  YYLONGJMP (yystack->yyexception_buffer, 1);
}

static void yyMemoryExhausted (yyGLRStack* yystack)
  __attribute__ ((__noreturn__));
static void
yyMemoryExhausted (yyGLRStack* yystack)
{
  YYLONGJMP (yystack->yyexception_buffer, 2);
}

#if YYDEBUG || YYERROR_VERBOSE
/** A printable representation of TOKEN.  */
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
static void yyfillin (yyGLRStackItem *, int, int) __attribute__ ((__unused__));
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
     __attribute__ ((__unused__));
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
	      YYLTYPE* YYOPTIONAL_LOC (yylocp),
	      yyGLRStack* yystack
              )
{
  yybool yynormal __attribute__ ((__unused__)) =
    (yystack->yysplitPoint == NULL);
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
  return yyerror (YY_("parse error: cannot back up")),     \
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
#line 113 "../src/parser/fol.y"
    { if (folDbg >= 2) printf("input: empty\n"); ;}
    break;

  case 3:
#line 116 "../src/parser/fol.y"
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
#line 140 "../src/parser/fol.y"
    { 
    if (folDbg >= 2) printf("input: weight\n"); 
    zzreset();
  ;}
    break;

  case 14:
#line 145 "../src/parser/fol.y"
    {  
    if (folDbg >= 2) printf("input: sentence\n");
      // the states should be reset because a parse error may have occurred
  ;}
    break;

  case 15:
#line 150 "../src/parser/fol.y"
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

      //if there are errors, we can keep this formula for further processing
    if (zznumErrors == 0)
    {
        //defer converting the formula to CNF until we have read all the
        //.db files and are sure that we know all the constants to ground
        //formula's clauses
      ZZFormulaInfo* epfi 
        = new ZZFormulaInfo(formula, zzformulaStr, zzfdnumPreds, zzwt, 
                            zzdefaultWt, zzdomain, zzmln, zzvarNameToIdMap, 
                            zzplusVarMap, zznumAsterisk,
                            zzhasFullStop, zzreadHardClauseWts, 
                            zzmustHaveWtOrFullStop);
      zzformulaInfos.append(epfi); 
    }

    if (zzwt) { delete zzwt; zzwt = NULL; }
  ;}
    break;

  case 17:
#line 197 "../src/parser/fol.y"
    { zzconsumeToken(zztokenList,"@"); ;}
    break;

  case 18:
#line 198 "../src/parser/fol.y"
    { zzconsumeToken(zztokenList,"@"); ;}
    break;

  case 20:
#line 205 "../src/parser/fol.y"
    { 
  if (folDbg >= 1) printf(".\n"); zzconsumeToken(zztokenList,"."); 
  zzassert(!zzhasFullStop, "expecting no full stop");
  zzhasFullStop = true;
  zzformulaStr.append(".");
;}
    break;

  case 21:
#line 214 "../src/parser/fol.y"
    { if (folDbg >= 1) printf("\\n\n"); zzconsumeToken(zztokenList,"\n"); ;}
    break;

  case 22:
#line 216 "../src/parser/fol.y"
    { if (folDbg >= 1) printf("\\r\n"); zzconsumeToken(zztokenList,"\r"); ;}
    break;

  case 24:
#line 222 "../src/parser/fol.y"
    { if (folDbg >= 1) printf("\\n\n"); zzconsumeToken(zztokenList,"\n"); ;}
    break;

  case 25:
#line 224 "../src/parser/fol.y"
    { if (folDbg >= 1) printf("\\r\n"); zzconsumeToken(zztokenList,"\r"); ;}
    break;

  case 26:
#line 231 "../src/parser/fol.y"
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
#line 290 "../src/parser/fol.y"
    { if (folDbg >= 1) printf("\\n\n"); ;}
    break;

  case 28:
#line 292 "../src/parser/fol.y"
    { if (folDbg >= 1) printf("\\r\n"); ;}
    break;

  case 29:
#line 303 "../src/parser/fol.y"
    { 
  zzconsumeToken(zztokenList,"=");
  zzconsumeToken(zztokenList,"{");
  if (folDbg >= 1) printf("= { "); 
  if (folDbg >= 2) printf("type_declarations: constant_declarations\n");
;}
    break;

  case 30:
#line 311 "../src/parser/fol.y"
    {
  if (folDbg >= 1) printf("} ");           
  zzconsumeToken(zztokenList, "}");
  delete [] zztypeName;
  zztypeName = NULL;
;}
    break;

  case 31:
#line 322 "../src/parser/fol.y"
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
#line 335 "../src/parser/fol.y"
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
#line 354 "../src/parser/fol.y"
    { if (folDbg >= 2) printf("constant_declarations: ZZ_VARIABLE\n"); ;}
    break;

  case 35:
#line 356 "../src/parser/fol.y"
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
#line 382 "../src/parser/fol.y"
    { zzconsumeToken(zztokenList, ","); ;}
    break;

  case 41:
#line 385 "../src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,"\n");
    zzconsumeToken(zztokenList,",");
    zzconsumeToken(zztokenList,"\n");
  ;}
    break;

  case 42:
#line 392 "../src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,"\n");
    zzconsumeToken(zztokenList,",");
  ;}
    break;

  case 43:
#line 398 "../src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,",");
    zzconsumeToken(zztokenList,"\n");
  ;}
    break;

  case 44:
#line 408 "../src/parser/fol.y"
    {
  if (folDbg >= 2) printf("numeric_type_declarations: \n");
  zzconsumeToken(zztokenList,"=");
  zzconsumeToken(zztokenList,"{");  
;}
    break;

  case 45:
#line 414 "../src/parser/fol.y"
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
#line 441 "../src/parser/fol.y"
    {
  zzconsumeToken(zztokenList, ",");
;}
    break;

  case 47:
#line 446 "../src/parser/fol.y"
    {
  zzconsumeToken(zztokenList, "}");
;}
    break;

  case 48:
#line 453 "../src/parser/fol.y"
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
#line 496 "../src/parser/fol.y"
    {  
    zzconsumeToken(zztokenList, ",");
    if (folDbg >= 1) printf(", "); 
    if (folDbg >= 2) printf("single_numeric_types: single_numeric_type\n"); 
  ;}
    break;

  case 53:
#line 507 "../src/parser/fol.y"
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
#line 543 "../src/parser/fol.y"
    {
  const char* predName = zztokenList.removeLast();

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
#line 555 "../src/parser/fol.y"
    { if (folDbg >= 1) printf("( "); zzconsumeToken(zztokenList,"("); ;}
    break;

  case 56:
#line 556 "../src/parser/fol.y"
    { if (folDbg >= 2) printf("predicate_declaration: types\n"); ;}
    break;

  case 57:
#line 558 "../src/parser/fol.y"
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
#line 576 "../src/parser/fol.y"
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
#line 599 "../src/parser/fol.y"
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

  free(predName);
  delete [] funcName;
;}
    break;

  case 60:
#line 624 "../src/parser/fol.y"
    {
  zzconsumeToken(zztokenList,"(");
  if (folDbg >= 1) printf("( "); 
  if (folDbg >= 2) printf("function_declaration: types\n"); 
;}
    break;

  case 61:
#line 630 "../src/parser/fol.y"
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
#line 659 "../src/parser/fol.y"
    { if (folDbg >= 1) printf(", "); zzconsumeToken(zztokenList,","); ;}
    break;

  case 66:
#line 663 "../src/parser/fol.y"
    { if (folDbg >= 1) printf(", "); zzconsumeToken(zztokenList,","); ;}
    break;

  case 70:
#line 671 "../src/parser/fol.y"
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

  case 72:
#line 683 "../src/parser/fol.y"
    {
  const char* varName = zztokenList.removeLast();
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

  case 74:
#line 699 "../src/parser/fol.y"
    {
  ;}
    break;

  case 75:
#line 702 "../src/parser/fol.y"
    {
    zzconsumeToken(zztokenList, "!");
    if (folDbg >= 1) printf("! "); 
    if (zzfuncTemplate)
      zzerr("'!' cannot be used in a function declaration.");
    zzassert(zzpredTemplate, "'!' used outside a predicate declaration.");

    int termIdx = zzpredTemplate->getNumTerms()-1;
    zzpredTemplate->addUniqueVarIndex(termIdx);
  ;}
    break;

  case 76:
#line 718 "../src/parser/fol.y"
    { 
    zztrueFalseUnknown = 't';
  ;}
    break;

  case 77:
#line 722 "../src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,"!"); 
    if (folDbg >= 1) printf("! "); 
    zztrueFalseUnknown = 'f';
  ;}
    break;

  case 78:
#line 728 "../src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,"?"); 
    if (folDbg >= 1) printf("? "); 
    zztrueFalseUnknown = 'u';
  ;}
    break;

  case 79:
#line 739 "../src/parser/fol.y"
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

  case 80:
#line 752 "../src/parser/fol.y"
    { 
  zzconsumeToken(zztokenList,"("); 
  if (folDbg >= 1) printf("( "); 
  if (folDbg >= 2) printf("predicate_definition: constants_in_groundings\n");
;}
    break;

  case 81:
#line 758 "../src/parser/fol.y"
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

  case 82:
#line 796 "../src/parser/fol.y"
    { 
  zzconsumeToken(zztokenList,"="); 
  if (folDbg >= 1) printf("= "); 
  if (folDbg >= 2) printf("function_definition: constants_in_groundings\n");
;}
    break;

  case 83:
#line 802 "../src/parser/fol.y"
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
  if (zztmpReturnNum)
  {
  	zzcreateAndCheckIntConstant(zztmpReturnConstant, zzfunc, zzpred, zzdomain, constName);
    if (constName == NULL)
    {
      constString = (char *)malloc((strlen(zztmpReturnConstant) + 1)*sizeof(char));
      strcpy(constString, zztmpReturnConstant);
    } else
    {
	  constString = (char *)malloc((strlen(constName) + 1)*sizeof(char));
      strcpy(constString, constName);
    }
  }
  else
  {
    constString = (char *)malloc((strlen(zztmpReturnConstant) + 1)*sizeof(char));  
  	strcpy(constString, zztmpReturnConstant);
  }

  //Add return constant parsed earlier
  zzaddConstantToPredFunc(constString);

  zztmpReturnNum = false;
  free(zztmpReturnConstant);
  delete [] funcName;
  free(predName);
  free(constString);
;}
    break;

  case 84:
#line 847 "../src/parser/fol.y"
    { 
  zzconsumeToken(zztokenList,"("); 
  if (folDbg >= 1) printf("( "); 
  if (folDbg >= 2) printf("function_definition: constants_in_groundings\n");
;}
    break;

  case 85:
#line 853 "../src/parser/fol.y"
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

  case 86:
#line 893 "../src/parser/fol.y"
    {     
    const char* constName = zztokenList.removeLast();
    if (folDbg >= 1) printf("cg_%s ", constName);
    zzaddConstantToPredFunc(constName);
    delete [] constName;
  ;}
    break;

  case 87:
#line 901 "../src/parser/fol.y"
    {     
    const char* constName = zztokenList.removeLast();
    if (folDbg >= 1) printf("cg_%s ", constName);
    zzaddConstantToPredFunc(constName);
    delete [] constName;
  ;}
    break;

  case 89:
#line 912 "../src/parser/fol.y"
    {
    if (zzconstantMustBeDeclared)
      zzerr("Constant %s must be declared before it is used.",
            zztokenList.back());
  ;}
    break;

  case 90:
#line 918 "../src/parser/fol.y"
    {
    const char* intStr = zztokenList.removeLast();
    char constName[100];
    zzcreateAndCheckIntConstant(intStr, zzfunc, zzpred, zzdomain, constName);
    if (constName == NULL) zztokenList.addLast(intStr);
    else                   zztokenList.addLast(constName);
    delete [] intStr;
  ;}
    break;

  case 91:
#line 927 "../src/parser/fol.y"
    {
    if (zzconstantMustBeDeclared)
      zzerr("Constant %s must be declared before it is used",
            zztokenList.back());
  ;}
    break;

  case 92:
#line 935 "../src/parser/fol.y"
    {
    const char* tmp = zztokenList.removeLast();
	zztmpReturnConstant = (char *)malloc((strlen(tmp) + 1)*sizeof(char));
  	strcpy(zztmpReturnConstant, tmp);
  	zztmpReturnNum = false;
  	delete []tmp;
  	if (folDbg >= 1) printf("ic_%s ", zztmpReturnConstant); 
  ;}
    break;

  case 93:
#line 944 "../src/parser/fol.y"
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

  case 94:
#line 956 "../src/parser/fol.y"
    {
  	const char* tmp = zztokenList.removeLast();
  	zztmpReturnConstant = (char *)malloc((strlen(tmp) + 1)*sizeof(char));
  	strcpy(zztmpReturnConstant, tmp);
  	zztmpReturnNum = true;
  	delete []tmp;
  	if (folDbg >= 1) printf("icnum_%s ", zztmpReturnConstant); 
  ;}
    break;

  case 95:
#line 965 "../src/parser/fol.y"
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

  case 97:
#line 983 "../src/parser/fol.y"
    {
    const char* wt = zztokenList.removeLast();
    if (folDbg >= 1) printf("n_%f ", atof(wt));
    if (zzwt) delete zzwt;
    zzwt = new double(atof(wt));
    delete [] wt;
  ;}
    break;

  case 98:
#line 997 "../src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,"("); 
    if (folDbg >= 1) printf("( "); 
    if (folDbg >= 2) printf("sentence: '(' sentence\n");
    zzformulaStr.append("(");
  ;}
    break;

  case 99:
#line 1005 "../src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,")"); 
    if (folDbg >= 1) printf(") "); 
    zzformulaStr.append(")");
  ;}
    break;

  case 100:
#line 1011 "../src/parser/fol.y"
    { if (folDbg >= 2) printf("sentence: atomic_sentence\n"); ;}
    break;

  case 102:
#line 1015 "../src/parser/fol.y"
    {
    const char* imp = zztokenList.removeLast(); 
    if (folDbg >= 1) printf("=> ");
    zzformulaStr.append(" => ");
    delete [] imp;
  ;}
    break;

  case 103:
#line 1023 "../src/parser/fol.y"
    { zzcreateListObjFromTopTwo(zzformulaListObjs, "=>"); ;}
    break;

  case 104:
#line 1027 "../src/parser/fol.y"
    { 
    const char* eq = zztokenList.removeLast(); 
    if (folDbg >= 1) printf("<=> ");
    zzformulaStr.append(" <=> ");
    delete [] eq;  
  ;}
    break;

  case 105:
#line 1035 "../src/parser/fol.y"
    { zzcreateListObjFromTopTwo(zzformulaListObjs, "<=>"); ;}
    break;

  case 106:
#line 1039 "../src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,"v"); 
    if (folDbg >= 1) printf("v "); 
    zzformulaStr.append(" v ");
  ;}
    break;

  case 107:
#line 1046 "../src/parser/fol.y"
    { zzcreateListObjFromTopTwo(zzformulaListObjs, "v"); ;}
    break;

  case 108:
#line 1050 "../src/parser/fol.y"
    { 
    zzconsumeToken(zztokenList,"^"); 
    if (folDbg >= 1) printf("^ "); 
    zzformulaStr.append(" ^ ");
  ;}
    break;

  case 109:
#line 1057 "../src/parser/fol.y"
    { zzcreateListObjFromTopTwo(zzformulaListObjs, "^"); ;}
    break;

  case 110:
#line 1061 "../src/parser/fol.y"
    { if (folDbg >= 2) printf("sentence: quantifier\n"); ;}
    break;

  case 111:
#line 1063 "../src/parser/fol.y"
    { 
    if (folDbg >= 2) printf("sentence: variables\n"); 
    zzformulaListObjs.push(new ListObj);
    pair<StringToStringMap*,int> pr(new StringToStringMap,zzscopeCounter++);
    zzoldNewVarList.push_front(pr);
  ;}
    break;

  case 112:
#line 1069 "../src/parser/fol.y"
    { if (folDbg >= 2) printf("sentence: sentence\n"); ;}
    break;

  case 113:
#line 1071 "../src/parser/fol.y"
    { 
    zzcreateListObjFromTopThree(zzformulaListObjs);
    pair<StringToStringMap*, int> pr = zzoldNewVarList.front();
    zzoldNewVarList.pop_front();
    delete pr.first;
  ;}
    break;

  case 114:
#line 1080 "../src/parser/fol.y"
    { 
    zzassert(!zzisNegated,"expecting !zzisNegated");
    zzisNegated = true;

    zzconsumeToken(zztokenList,"!");
    if (folDbg >= 1) printf("! "); 
    if (folDbg >= 2) printf("sentence: sentence\n");
    zzformulaStr.append("!");
  ;}
    break;

  case 115:
#line 1090 "../src/parser/fol.y"
    { zzcreateListObjFromTop(zzformulaListObjs, "!"); ;}
    break;

  case 116:
#line 1096 "../src/parser/fol.y"
    { 
    const char* fa = zztokenList.removeLast();
    if (folDbg >= 1) printf("FORALL ");
    zzformulaListObjs.push(new ListObj("FORALL"));
    zzformulaStr.append("FORALL ");
    delete [] fa;
  ;}
    break;

  case 117:
#line 1104 "../src/parser/fol.y"
    { 
    const char* ex = zztokenList.removeLast();
    if (folDbg >= 1) printf("EXIST "); 
    zzformulaListObjs.push(new ListObj("EXIST"));
    zzformulaStr.append("EXIST ");
    delete [] ex;
  ;}
    break;

  case 118:
#line 1116 "../src/parser/fol.y"
    { if (folDbg >= 2) printf("variables: variables\n"); ;}
    break;

  case 119:
#line 1118 "../src/parser/fol.y"
    {  
    zzconsumeToken(zztokenList,",");
    if (folDbg >= 1) printf(", "); 
    zzformulaStr.append(",");
  ;}
    break;

  case 121:
#line 1125 "../src/parser/fol.y"
    { zzformulaStr.append(" "); ;}
    break;

  case 122:
#line 1130 "../src/parser/fol.y"
    {
  const char* varName = zztokenList.removeLast();  
  if (folDbg >= 1) printf("v_%s ", varName); 

  //if (isupper(varName[0])) 
  if (zzisConstant(varName)) 
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

  case 123:
#line 1156 "../src/parser/fol.y"
    {
    const char* predName = zztokenList.removeLast();
    if (folDbg >= 1) printf("p_%s ", predName); 

    zzfdnumPreds++;
    ListObj* predlo = new ListObj;

	if (PredicateTemplate::isInternalPredicateTemplateName(predName))
	{
	  //zzinfixPredName is misused here to store internal pred. name
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

  case 124:
#line 1186 "../src/parser/fol.y"
    {  
    zzconsumeToken(zztokenList, "(");
    if (folDbg >= 1) printf("( "); 
    if (folDbg >= 2) printf("atomic_sentence: terms\n"); 
    zzformulaStr.append("(");

  ;}
    break;

  case 125:
#line 1195 "../src/parser/fol.y"
    {  
    zzconsumeToken(zztokenList, ")");
    if (folDbg >= 1) printf(") "); 

	  //If an internal pred., then need to determine type
	  //zzinfixPredName is misused here to store internal pred. name
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
	  free(zzinfixPredName);
      zzinfixPredName = NULL;
	}

    zzcheckPredNumTerm(zzpred);
    delete zzpred;
    zzpred = NULL;
    zzassert(zzpredFuncListObjs.size()==1,
             "expecting zzpredFuncListObjs.size()==1");
    ListObj* predlo = zzpredFuncListObjs.top();
    zzpredFuncListObjs.pop();

    if (zzisAsterisk)
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
      //zzformulaListObjs.push(topPredlo);
      //zzcreateListObjFromTopTwo(zzformulaListObjs, "^");
      zzformulaListObjs.push(topPredlo);
      zzcreateListObjFromTopTwo(zzformulaListObjs, "v");
	} //while (!zzfuncConjStack.empty())

	if (!zzfuncConjStr.empty())
	{
      zzformulaStr.append(zzfuncConjStr);
      zzfuncConjStr.clear();
	}
	zzfunc = NULL;
  ;}
    break;

  case 126:
#line 1345 "../src/parser/fol.y"
    {
    ++zzfdnumPreds;
    //zzfdisEqualPred = true;
    //const PredicateTemplate* t = zzdomain->getEqualPredicateTemplate();
    const PredicateTemplate* t = zzdomain->getEmptyPredicateTemplate();

    zzassert(zzpred == NULL, "expecting zzpred==NULL");
    zzpred = new Predicate(t);

    ListObj* predlo = new ListObj;
    //predlo->append(PredicateTemplate::EQUAL_NAME);
    predlo->append(PredicateTemplate::EMPTY_NAME);
    zzpredFuncListObjs.push(predlo);
    if (zzisNegated)  { zzpred->setSense(false); zzisNegated = false; }

    if (folDbg >= 2) printf("atomic_sentence (left): term\n"); 
  ;}
    break;

  case 127:
#line 1364 "../src/parser/fol.y"
    {
  	ListObj* predlo = zzpredFuncListObjs.top();
    predlo->replace(PredicateTemplate::EMPTY_NAME, zzinfixPredName);
    
  	  // If type known from LHS, then set the pred types accordingly
    int lTypeId = zzgetTypeId(zzpred->getTerm(0), (*predlo)[1]->getStr());
    if (lTypeId > 0)
    {
      if (strcmp(zzinfixPredName, PredicateTemplate::EQUAL_NAME)==0)
      {
        zzsetEqPredTypeName(lTypeId);
      }
      else
 	  {
 	    zzsetInternalPredTypeName(zzinfixPredName, lTypeId);
 	  }
    }
  ;}
    break;

  case 128:
#line 1383 "../src/parser/fol.y"
    {  
    ListObj* predlo = zzpredFuncListObjs.top();
    //predlo->replace(PredicateTemplate::EMPTY_NAME, zzinfixPredName);

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
        if ( (lTypeId <= 0 && rTypeId > 0) || (lTypeId > 0 && rTypeId <= 0) )
        {
          int knownTypeId = (lTypeId > 0) ? lTypeId : rTypeId;
          if (strcmp(zzinfixPredName, PredicateTemplate::EQUAL_NAME) == 0)
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
      // This allows for the '!=' operator
    if (zzisNegated)
    {
      zzcreateListObjFromTop(zzformulaListObjs, "!");
      zzisNegated = false;
    }
    
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
      //zzformulaListObjs.push(topPredlo);
	  //zzcreateListObjFromTopTwo(zzformulaListObjs, "^");
	  zzformulaListObjs.push(topPredlo);
	  zzcreateListObjFromTopTwo(zzformulaListObjs, "v");
	} //while (!zzfuncConjStack.empty())

	if (!zzfuncConjStr.empty())
	{
	  zzformulaStr.append(zzfuncConjStr);
	  zzfuncConjStr.clear();
	}
	zzfunc = NULL;
	free(zzinfixPredName);
    zzinfixPredName = NULL;
  ;}
    break;

  case 129:
#line 1530 "../src/parser/fol.y"
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

  case 130:
#line 1544 "../src/parser/fol.y"
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

  case 131:
#line 1558 "../src/parser/fol.y"
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

  case 132:
#line 1575 "../src/parser/fol.y"
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

  case 133:
#line 1592 "../src/parser/fol.y"
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

  case 134:
#line 1605 "../src/parser/fol.y"
    {
  	zzconsumeToken(zztokenList, "!");
  	zzconsumeToken(zztokenList, "=");
    if (folDbg >= 1) printf("!= "); 
    if (folDbg >= 2) printf("atomic_sentence (right): term\n"); 
    zzformulaStr.append(" != ");
    zzinfixPredName = (char *)malloc((strlen(PredicateTemplate::EQUAL_NAME)
    								  + 1)*sizeof(char));
  	strcpy(zzinfixPredName, PredicateTemplate::EQUAL_NAME);
    const PredicateTemplate* t = zzdomain->getEqualPredicateTemplate();
	zzpred->setTemplate((PredicateTemplate*)t);
	  // Theoretically, we could have "!(a1 != a2)" which would be "a1 = a2"
    if (zzpred->getSense())
    {
      zzpred->setSense(false);
      zzisNegated = true;
    }
    else { zzpred->setSense(true); }
  ;}
    break;

  case 136:
#line 1630 "../src/parser/fol.y"
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

  case 137:
#line 1646 "../src/parser/fol.y"
    {  
    zzconsumeToken(zztokenList, ",");
    if (folDbg >= 1) printf(", "); 
    if (folDbg >= 2) printf("terms: term\n"); 
    // While parsing function, we do not want to append anything to the formula
    if (zzfunc == NULL) zzformulaStr.append(",");
  ;}
    break;

  case 139:
#line 1656 "../src/parser/fol.y"
    {
  	// After the first term in an internal pred., check if we can determine type
  	if (zzpred && zzpred->isEmptyPred() &&
  		zzpred->getNumTerms() == 1 && zzinfixPredName)
  	{
      ListObj* predlo = zzpredFuncListObjs.top();
      predlo->replace(PredicateTemplate::EMPTY_NAME, zzinfixPredName);
    
  	  	// If type known from term, then set the pred types accordingly
	  int lTypeId = zzgetTypeId(zzpred->getTerm(0), (*predlo)[1]->getStr());
      if (lTypeId>0)
      {
      	if (strcmp(zzinfixPredName, PredicateTemplate::EQUAL_NAME)==0)
      	{
          zzsetEqPredTypeName(lTypeId);
      	}
      	else
 	  	{
 	      zzsetInternalPredTypeName(zzinfixPredName, lTypeId);
 	  	}
      }
  	}
  ;}
    break;

  case 140:
#line 1684 "../src/parser/fol.y"
    {
    const char* constName = zztokenList.removeLast();
    if (folDbg >= 1) printf("c2_%s ", constName); 
    zztermIsConstant(constName, constName);
    if (zzfunc) zzfdfuncConstants.append(string(constName));
    else        zzfdconstName = constName;
    delete [] constName;
  ;}
    break;

  case 141:
#line 1694 "../src/parser/fol.y"
    {
    const char* constName = zztokenList.removeLast();
    if (folDbg >= 1) printf("c2_%s ", constName); 
      if (zzconstantMustBeDeclared)
        zzerr("Constant %s must be declared before it is used", constName);
    zztermIsConstant(constName, constName);
    if (zzfunc) zzfdfuncConstants.append(string(constName));
    else        zzfdconstName = constName;
    delete [] constName;
  ;}
    break;

  case 142:
#line 1706 "../src/parser/fol.y"
    {
    const char* intStr = zztokenList.removeLast();
    if (folDbg >= 1) printf("c3_%s ", intStr);

    char constName[100];
    zzcreateAndCheckIntConstant(intStr, zzfunc, zzpred, zzdomain, constName);
    if (constName == NULL) { break; delete [] intStr; }

    zztermIsConstant(constName, intStr);
    if (zzfunc) zzfdfuncConstants.append(string(constName));
    else        zzfdconstName = constName;
    delete [] intStr;
  ;}
    break;

  case 143:
#line 1721 "../src/parser/fol.y"
    {
    zztermIsVariable(folDbg);
    if (zzisPlus) zzisPlus = false;
  ;}
    break;

  case 144:
#line 1727 "../src/parser/fol.y"
    {
    zzconsumeToken(zztokenList, "+");
    if (folDbg >= 1) printf("+ "); 
    zzassert(!zzisPlus,"expecting !zzisPlus");
    zzisPlus = true;
    zzformulaStr.append("+");
    zztermIsVariable(folDbg);
    if (zzisPlus) zzisPlus = false;
  ;}
    break;

  case 145:
#line 1738 "../src/parser/fol.y"
    {
    zzassert(zzfunc != NULL,"expecting zzfunc != NULL");
    zzcheckFuncNumTerm(zzfunc);
    zzassert(zzpred != NULL, "expecting zzpred != NULL");

      // replace function
	 
	  // Append Term funcVar<zzfuncVarCounter> to predicate where
      // function appears and set flag to add conjunction

      // 1. Make new variable
    char* varName;
    string newVarName;
    
      // Check if function mapping already occurs in formula
    ZZFuncToFuncVarMap::iterator mit;
    if ((mit = zzfuncToFuncVarMap.find(zzfunc)) != zzfuncToFuncVarMap.end())
    {
      newVarName = (*mit).second;
      varName = (char *)malloc((strlen(newVarName.c_str()) + 1) * sizeof(char));
      strcpy(varName, newVarName.c_str());
    }
    else
    {
      char funcVarCounterString[10];
      int funcVarCounterLength =
        sprintf(funcVarCounterString, "%d", zzfuncVarCounter);
      varName = (char *)malloc((strlen(ZZ_FUNCVAR_PREFIX) +
  								funcVarCounterLength + 1)*sizeof(char));
      strcpy(varName, ZZ_FUNCVAR_PREFIX);
      strcat(varName, funcVarCounterString);
      ++zzfdnumVars;
      ++zzfuncVarCounter;
      newVarName = zzgetNewVarName(varName);
      
      Function* func = new Function(*zzfunc);
      zzfuncToFuncVarMap[func] = newVarName;
    }

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
    //zzfuncConjStr.append(" ^ ");
    zzfuncConjStr.append(" v !");
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
    zzcreateListObjFromTop(zzfuncConjStack, "!");
    
    // 4. Append new variable to function in stack or to predicate
    if (!zzfuncStack.empty())
    {
   	  Function* prevFunc = zzfuncStack.top(); 
	  zzfuncStack.pop();
    
      // check that we have not exceeded the number of terms
      if ((unexp = prevFunc->getNumTerms()) ==
       	  (exp = prevFunc->getTemplate()->getNumTerms()))
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
      if ((unexp = prevPred->getNumTerms()) ==
      	  (exp = prevPred->getTemplate()->getNumTerms()))
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
    free(varName);
	free(predName);
	//zzformulaStr.append(")");

    delete zzpred;
	zzpred = prevPred;
  ;}
    break;

  case 146:
#line 1934 "../src/parser/fol.y"
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

  case 147:
#line 1970 "../src/parser/fol.y"
    {
    zzconsumeToken(zztokenList, "(");
    if (folDbg >= 1) printf("( "); 
    if (folDbg >= 2) printf("term: terms\n");
  ;}
    break;

  case 148:
#line 1976 "../src/parser/fol.y"
    {
  	  //If an internal func., then need to determine type
	if (zzinfixFuncName)
	{
	  ListObj* funclo = zzpredFuncListObjs.top();
      funclo->replace(FunctionTemplate::EMPTY_FTEMPLATE_NAME, zzinfixFuncName);
	  const FunctionTemplate* t =
	    zzgetGenericInternalFunctionTemplate(zzinfixFuncName);
		// If in a pred. (not LHS of infix pred.), then we know the return type
	  if (zzpred)
	  {
	  	((FunctionTemplate*)t)->setRetTypeId(
	  		zzpred->getTermTypeAsInt(zzpred->getNumTerms()), zzdomain);
      }
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
		free(predName);
		free(varName);
      }
	  free(zzinfixFuncName);
      zzinfixFuncName = NULL;
	}
  	
    zzconsumeToken(zztokenList, ")");
    if (folDbg >= 1) printf(") ");
  ;}
    break;

  case 149:
#line 2054 "../src/parser/fol.y"
    {
      // Create a function corresponding to the infix sign
    if (zzfunc != NULL) { zzfuncStack.push(zzfunc); zzfunc = NULL; }
    ++zzfdnumFuncs;
    zzfdfuncName = zzinfixFuncName;
	const FunctionTemplate* t =
      zzgetGenericInternalFunctionTemplate(zzinfixFuncName);
  	zzfunc = new Function(t);
	  // If in a pred. (not LHS of infix pred.), then we know the return type
      // It's the type of the previous variable (getNumTerms() - 1)
	if (zzpred)
      ((FunctionTemplate*)t)->setRetTypeId(
        zzpred->getTermTypeAsInt(zzpred->getNumTerms() - 1), zzdomain);

    ListObj* funclo = new ListObj;
    funclo->append(zzfdfuncName.c_str());
    zzpredFuncListObjs.push(funclo);

      // The first term was added to the previous pred / func
      // So this needs to be switched to the infix function
    moveTermToInfixFunction();
  ;}
    break;

  case 150:
#line 2077 "../src/parser/fol.y"
    {
    ListObj* funclo = zzpredFuncListObjs.top();

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
      free(predName);
      free(varName);
    }
    free(zzinfixFuncName);
    zzinfixFuncName = NULL;
  ;}
    break;

  case 151:
#line 2139 "../src/parser/fol.y"
    {
    zzconsumeToken(zztokenList, "+");
    if (folDbg >= 1) printf("+ "); 
    zzinfixFuncName = (char *)malloc((strlen(PredicateTemplate::PLUS_NAME)
    								  + 1)*sizeof(char));
  	strcpy(zzinfixFuncName, PredicateTemplate::PLUS_NAME);
  ;}
    break;

  case 152:
#line 2148 "../src/parser/fol.y"
    {
   	zzconsumeToken(zztokenList, "-");
    if (folDbg >= 1) printf("- "); 
    zzinfixFuncName = (char *)malloc((strlen(PredicateTemplate::MINUS_NAME)
                                      + 1)*sizeof(char));
    strcpy(zzinfixFuncName, PredicateTemplate::MINUS_NAME);
  ;}
    break;

  case 153:
#line 2157 "../src/parser/fol.y"
    {
  	zzconsumeToken(zztokenList, "*");
    if (folDbg >= 1) printf("* ");
    zzinfixFuncName = (char *)malloc((strlen(PredicateTemplate::TIMES_NAME)
    								  + 1)*sizeof(char));
  	strcpy(zzinfixFuncName, PredicateTemplate::TIMES_NAME);
  ;}
    break;

  case 154:
#line 2166 "../src/parser/fol.y"
    {
	zzconsumeToken(zztokenList, "/");
    if (folDbg >= 1) printf("/ ");
    zzinfixFuncName = (char *)malloc((strlen(PredicateTemplate::DIVIDEDBY_NAME)
    								  + 1)*sizeof(char));
  	strcpy(zzinfixFuncName, PredicateTemplate::DIVIDEDBY_NAME);
  ;}
    break;

  case 155:
#line 2175 "../src/parser/fol.y"
    {
  	zzconsumeToken(zztokenList, "%");
    if (folDbg >= 1) printf("%% ");
    zzinfixFuncName = (char *)malloc((strlen(PredicateTemplate::MOD_NAME)
    								  + 1)*sizeof(char));
  	strcpy(zzinfixFuncName, PredicateTemplate::MOD_NAME);
  ;}
    break;


      default: break;
    }

  return yyok;
# undef yyerrok
# undef YYABORT
# undef YYACCEPT
# undef YYERROR
# undef YYBACKUP
# undef yyclearin
# undef YYRECOVERING
/* Line 872 of glr.c.  */
#line 3282 "fol.cpp"
}


static void
yyuserMerge (int yyn, YYSTYPE* yy0, YYSTYPE* yy1)
{
  /* `Use' the arguments.  */
  (void) yy0;
  (void) yy1;

  switch (yyn)
    {
      
      default: break;
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

/** Number of symbols composing the right hand side of rule #RULE.  */
static inline int
yyrhsLength (yyRuleNum yyrule)
{
  return yyr2[yyrule];
}

static void
yydestroyGLRState (char const *yymsg, yyGLRState *yys)
{
  if (yys->yyresolved)
    yydestruct (yymsg, yystos[yys->yylrState],
		&yys->yysemantics.yysval);
  else
    {
#if YYDEBUG
      if (yydebug)
	{
	  YYFPRINTF (stderr, "%s unresolved ", yymsg);
	  yysymprint (stderr, yystos[yys->yylrState],
		      &yys->yysemantics.yysval);
	  YYFPRINTF (stderr, "\n");
	}
#endif

      if (yys->yysemantics.yyfirstVal)
        {
          yySemanticOption *yyoption = yys->yysemantics.yyfirstVal;
          yyGLRState *yyrh;
          int yyn;
          for (yyrh = yyoption->yystate, yyn = yyrhsLength (yyoption->yyrule);
               yyn > 0;
               yyrh = yyrh->yypred, yyn -= 1)
            yydestroyGLRState (yymsg, yyrh);
        }
    }
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
static yybool
yyinitStateSet (yyGLRStateSet* yyset)
{
  yyset->yysize = 1;
  yyset->yycapacity = 16;
  yyset->yystates = (yyGLRState**) YYMALLOC (16 * sizeof yyset->yystates[0]);
  if (! yyset->yystates)
    return yyfalse;
  yyset->yystates[0] = NULL;
  return yytrue;
}

static void yyfreeStateSet (yyGLRStateSet* yyset)
{
  YYFREE (yyset->yystates);
}

/** Initialize STACK to a single empty stack, with total maximum
 *  capacity for all stacks of SIZE. */
static yybool
yyinitGLRStack (yyGLRStack* yystack, size_t yysize)
{
  yystack->yyerrState = 0;
  yynerrs = 0;
  yystack->yyspaceLeft = yysize;
  yystack->yyitems =
    (yyGLRStackItem*) YYMALLOC (yysize * sizeof yystack->yynextFree[0]);
  if (!yystack->yyitems)
    return yyfalse;
  yystack->yynextFree = yystack->yyitems;
  yystack->yysplitPoint = NULL;
  yystack->yylastDeleted = NULL;
  return yyinitStateSet (&yystack->yytops);
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
  yyGLRStackItem* yynewItems;
  yyGLRStackItem* yyp0, *yyp1;
  size_t yysize, yynewSize;
  size_t yyn;
  yysize = yystack->yynextFree - yystack->yyitems;
  if (YYMAXDEPTH <= yysize)
    yyMemoryExhausted (yystack);
  yynewSize = 2*yysize;
  if (YYMAXDEPTH < yynewSize)
    yynewSize = YYMAXDEPTH;
  yynewItems = (yyGLRStackItem*) YYMALLOC (yynewSize * sizeof yynewItems[0]);
  if (! yynewItems)
    yyMemoryExhausted (yystack);
  for (yyp0 = yystack->yyitems, yyp1 = yynewItems, yyn = yysize;
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
    yystack->yysplitPoint = YYRELOC (yystack->yyitems, yynewItems,
				 yystack->yysplitPoint, yystate);

  for (yyn = 0; yyn < yystack->yytops.yysize; yyn += 1)
    if (yystack->yytops.yystates[yyn] != NULL)
      yystack->yytops.yystates[yyn] =
	YYRELOC (yystack->yyitems, yynewItems,
		 yystack->yytops.yystates[yyn], yystate);
  YYFREE (yystack->yyitems);
  yystack->yyitems = yynewItems;
  yystack->yynextFree = yynewItems + yysize;
  yystack->yyspaceLeft = yynewSize - yysize;

#else
  yyMemoryExhausted (yystack);
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
yymarkStackDeleted (yyGLRStack* yystack, size_t yyk)
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
yyglrShift (yyGLRStack* yystack, size_t yyk, yyStateNum yylrState,
	    size_t yyposn,
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
yyglrShiftDefer (yyGLRStack* yystack, size_t yyk, yyStateNum yylrState,
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
yydoAction (yyGLRStack* yystack, size_t yyk, yyRuleNum yyrule,
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

static size_t
yysplitStack (yyGLRStack* yystack, size_t yyk)
{
  if (yystack->yysplitPoint == NULL)
    {
      YYASSERT (yyk == 0);
      yystack->yysplitPoint = yystack->yytops.yystates[yyk];
    }
  if (yystack->yytops.yysize >= yystack->yytops.yycapacity)
    {
      yyGLRState** yynewStates;
      if (! ((yystack->yytops.yycapacity
	      <= (YYSIZEMAX / (2 * sizeof yynewStates[0])))
	     && (yynewStates =
		 (yyGLRState**) YYREALLOC (yystack->yytops.yystates,
					   ((yystack->yytops.yycapacity *= 2)
					    * sizeof yynewStates[0])))))
	yyMemoryExhausted (yystack);
      yystack->yytops.yystates = yynewStates;
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

/** Assuming identicalOptions (Y0,Y1), destructively merge the
 *  alternative semantic values for the RHS-symbols of Y1 and Y0. */
static void
yymergeOptionSets (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  yyGLRState *yys0, *yys1;
  int yyn;
  for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
       yyn = yyrhsLength (yyy0->yyrule);
       yyn > 0;
       yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
    {
      if (yys0 == yys1)
	break;
      else if (yys0->yyresolved)
	{
	  yys1->yyresolved = yytrue;
	  yys1->yysemantics.yysval = yys0->yysemantics.yysval;
	}
      else if (yys1->yyresolved)
	{
	  yys0->yyresolved = yytrue;
	  yys0->yysemantics.yysval = yys1->yysemantics.yysval;
	}
      else
	{
	  yySemanticOption** yyz0p;
	  yySemanticOption* yyz1;
	  yyz0p = &yys0->yysemantics.yyfirstVal;
	  yyz1 = yys1->yysemantics.yyfirstVal;
	  while (yytrue)
	    {
	      if (yyz1 == *yyz0p || yyz1 == NULL)
		break;
	      else if (*yyz0p == NULL)
		{
		  *yyz0p = yyz1;
		  break;
		}
	      else if (*yyz0p < yyz1)
		{
		  yySemanticOption* yyz = *yyz0p;
		  *yyz0p = yyz1;
		  yyz1 = yyz1->yynext;
		  (*yyz0p)->yynext = yyz;
		}
	      yyz0p = &(*yyz0p)->yynext;
	    }
	  yys1->yysemantics.yyfirstVal = yys0->yysemantics.yyfirstVal;
	}
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

static void yyreportAmbiguity (yySemanticOption* yyx0, yySemanticOption* yyx1,
			       yyGLRStack* yystack)
  __attribute__ ((__noreturn__));
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
  yyFail (yystack, YY_("syntax is ambiguous"));
}


/** Resolve the ambiguity represented by OPTIONLIST, perform the indicated
 *  actions, and return the result. */
static YYRESULTTAG
yyresolveValue (yySemanticOption* yyoptionList, yyGLRStack* yystack,
		YYSTYPE* yyvalp, YYLTYPE* yylocp)
{
  yySemanticOption* yybest;
  yySemanticOption** yypp;
  yybool yymerge;

  yybest = yyoptionList;
  yymerge = yyfalse;
  for (yypp = &yyoptionList->yynext; *yypp != NULL; )
    {
      yySemanticOption* yyp = *yypp;

      if (yyidenticalOptions (yybest, yyp))
	{
	  yymergeOptionSets (yybest, yyp);
	  *yypp = yyp->yynext;
	}
      else
	{
	  switch (yypreference (yybest, yyp))
	    {
	    case 0:
	      yyreportAmbiguity (yybest, yyp, yystack);
	      break;
	    case 1:
	      yymerge = yytrue;
	      break;
	    case 2:
	      break;
	    case 3:
	      yybest = yyp;
	      yymerge = yyfalse;
	      break;
	    default:
	      /* This cannot happen so it is not worth a YYASSERT (yyfalse),
	         but some compilers complain if the default case is
		 omitted.  */
	      break;
	    }
	  yypp = &yyp->yynext;
	}
    }

  if (yymerge)
    {
      yySemanticOption* yyp;
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
yyprocessOneStack (yyGLRStack* yystack, size_t yyk,
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
      YYDPRINTF ((stderr, "Stack %lu Entering state %d\n",
		  (unsigned long int) yyk, yystate));

      YYASSERT (yystate != YYFINAL);

      if (yyisDefaultedState (yystate))
	{
	  yyrule = yydefaultAction (yystate);
	  if (yyrule == 0)
	    {
	      YYDPRINTF ((stderr, "Stack %lu dies.\n",
			  (unsigned long int) yyk));
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
	      size_t yynewStack = yysplitStack (yystack, yyk);
	      YYDPRINTF ((stderr, "Splitting off stack %lu from %lu.\n",
			  (unsigned long int) yynewStack,
			  (unsigned long int) yyk));
	      YYCHK (yyglrReduce (yystack, yynewStack,
				  *yyconflicts, yyfalse));
	      YYCHK (yyprocessOneStack (yystack, yynewStack, yyposn,
					yylvalp, yyllocp));
	      yyconflicts += 1;
	    }

	  if (yyisShiftAction (yyaction))
	    {
	      YYDPRINTF ((stderr, "On stack %lu, ", (unsigned long int) yyk));
	      YY_SYMBOL_PRINT ("shifting", *yytokenp, yylvalp, yyllocp);
	      yyglrShift (yystack, yyk, yyaction, yyposn+1,
			  *yylvalp, yyllocp);
	      YYDPRINTF ((stderr, "Stack %lu now in state #%d\n",
			  (unsigned long int) yyk,
			  yystack->yytops.yystates[yyk]->yylrState));
	      break;
	    }
	  else if (yyisErrorAction (yyaction))
	    {
	      YYDPRINTF ((stderr, "Stack %lu dies.\n",
			  (unsigned long int) yyk));
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
	  size_t yysize0 = yytnamerr (NULL, yytokenName (*yytokenp));
	  size_t yysize = yysize0;
	  size_t yysize1;
	  yybool yysize_overflow = yyfalse;
	  char* yymsg = NULL;
	  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;
	  char *yyfmt;
	  char const *yyf;
	  static char const yyunexpected[] = "parse error, unexpected %s";
	  static char const yyexpecting[] = ", expecting %s";
	  static char const yyor[] = " or %s";
	  char yyformat[sizeof yyunexpected
			+ sizeof yyexpecting - 1
			+ ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
			   * (sizeof yyor - 1))];
	  char const *yyprefix = yyexpecting;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 1;

	  yyarg[0] = yytokenName (*yytokenp);
	  yyfmt = yystpcpy (yyformat, yyunexpected);

	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
		  {
		    yycount = 1;
		    yysize = yysize0;
		    yyformat[sizeof yyunexpected - 1] = '\0';
		    break;
		  }
		yyarg[yycount++] = yytokenName (yyx);
		yysize1 = yysize + yytnamerr (NULL, yytokenName (yyx));
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + strlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow)
	    yymsg = (char *) YYMALLOC (yysize);

	  if (yymsg)
	    {
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
		{
		  if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		    {
		      yyp += yytnamerr (yyp, yyarg[yyi++]);
		      yyf += 2;
		    }
		  else
		    {
		      yyp++;
		      yyf++;
		    }
		}
	      yyerror (yymsg);
	      YYFREE (yymsg);
	    }
	  else
	    {
	      yyerror (YY_("parse error"));
	      yyMemoryExhausted (yystack);
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (YY_("parse error"));
      yynerrs += 1;
    }
}

/* Recover from a parse error on YYSTACK, assuming that YYTOKENP,
   YYLVALP, and YYLLOCP point to the syntactic category, semantic
   value, and location of the look-ahead.  */
static void
yyrecoverSyntaxError (yyGLRStack* yystack,
		      YYSTYPE* yylvalp,
		      YYLTYPE* YYOPTIONAL_LOC (yyllocp)
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
	  yyFail (yystack, NULL);
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
	      YYLTYPE yyerrloc;yyerrloc.yydummy = 'a';
	      YY_SYMBOL_PRINT ("Shifting", yystos[yytable[yyj]],
			       yylvalp, &yyerrloc);
	      yyglrShift (yystack, 0, yytable[yyj],
			  yys->yyposn, *yylvalp, &yyerrloc);
	      yys = yystack->yytops.yystates[0];
	      break;
	    }
	}

      yydestroyGLRState ("Error: popping", yys);
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
    case yyok:								     \
      break;								     \
    case yyabort:							     \
      goto yyabortlab;							     \
    case yyaccept:							     \
      goto yyacceptlab;							     \
    case yyerr:								     \
      goto yyuser_error;						     \
    default:								     \
      goto yybuglab;							     \
    }									     \
  } while (0)


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
  int yyresult;
  yySymbol yytoken;
  yyGLRStack yystack;
  size_t yyposn;


  YYSTYPE* const yylvalp = &yylval;
  YYLTYPE* const yyllocp = &yylloc;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yytoken = YYEMPTY;
  yylval = yyval_default;


  if (! yyinitGLRStack (&yystack, YYINITDEPTH))
    goto yyexhaustedlab;
  switch (YYSETJMP (yystack.yyexception_buffer))
    {
    case 0: break;
    case 1: goto yyabortlab;
    case 2: goto yyexhaustedlab;
    default: goto yybuglab;
    }
  yystack.yytokenp = &yytoken;
  yyglrShift (&yystack, 0, 0, 0, yylval, &yylloc);
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
	    goto yyacceptlab;
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
		  yyglrShift (&yystack, 0, yyaction, yyposn, yylval, yyllocp);
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
	  size_t yys;
	  size_t yyn = yystack.yytops.yysize;
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
		yyFail (&yystack, YY_("parse error"));
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

 yyacceptlab:
  yyresult = 0;
  goto yyreturn;

 yybuglab:
  YYASSERT (yyfalse);
  /* Fall through.  */

 yyabortlab:
  yyresult = 1;
  goto yyreturn;

 yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */

 yyreturn:
  if (yytoken != YYEOF && yytoken != YYEMPTY)
    yydestruct ("Cleanup: discarding lookahead",
                yytoken, yylvalp);

  /* If the stack is well-formed, pop the stack until it is empty,
     destroying its entries as we go.  But free the stack regardless
     of whether it is well-formed.  */
  if (yystack.yyitems)
    {
      yyGLRState** yystates = yystack.yytops.yystates;
      if (yystates)
	while (yystates[0])
	  {
	    yyGLRState *yys = yystates[0];
	    yydestroyGLRState ("Cleanup: popping", yys);
	    yystates[0] = yys->yypred;
	    yystack.yynextFree -= 1;
	    yystack.yyspaceLeft += 1;
	  }
      yyfreeGLRStack (&yystack);
    }

  return yyresult;
}

/* DEBUGGING ONLY */
#ifdef YYDEBUG
static void yypstack (yyGLRStack* yystack, size_t yyk)
  __attribute__ ((__unused__));
static void yypdumpstack (yyGLRStack* yystack) __attribute__ ((__unused__));

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
yypstack (yyGLRStack* yystack, size_t yyk)
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


#line 2185 "../src/parser/fol.y"


/******************* function definitions ****************************/

  //warnDuplicates set to true by default
bool runYYParser(MLN* const & mln, Domain* const & dom,
                 const char* const & fileName, 
                 const bool& allPredsExceptQueriesAreClosedWorld,
                 const StringHashArray* const & openWorldPredNames,
                 const StringHashArray* const & closedWorldPredNames,
                 const StringHashArray* const & queryPredNames,
                 const bool& addUnitClauses, const bool& warnDuplicates,
                 const double& defaultWt, const bool& mustHaveWtOrFullStop,
                 const Domain* const & domain0, const bool& lazyInference,
                 const bool& flipWtsOfFlippedClause)
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
  zzflipWtsOfFlippedClause = flipWtsOfFlippedClause;

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
  zzcreateBlocks(zzdomain);
  
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
                           openWorldPredNames, closedWorldPredNames,
                           queryPredNames);

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



