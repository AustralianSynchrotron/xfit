
/*  A Bison parser, made from xfity.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	EDGE	258
#define	EXAFSW	259
#define	FTW	260
#define	DATA	261
#define	KWEIGHT	262
#define	RESTORE	263
#define	REFINE	264
#define	LIST	265
#define	DELETE	266
#define	GROUP	267
#define	EXIT	268
#define	C_MODEL	269
#define	SAVE	270
#define	CHECK_DERIVS	271
#define	PRINT	272
#define	MONTECARLO	273
#define	CLEAN	274
#define	SYSTEM	275
#define	INVALID	276
#define	INCLUDE	277
#define	LOG_FILE	278
#define	HELP	279
#define	IDENTIFIER	280
#define	VECTOR_ID	281
#define	SC_F0	282
#define	SC_F1	283
#define	SC_F2	284
#define	POW_OP	285
#define	CONSTANT	286
#define	STRING_LIT	287
#define	HELP_TOPIC	288
#define	WINDOW_EDGE	289
#define	EXAFS_EDGE	290
#define	LIST_PARAMETER	291
#define	EOL	292
#define	LE_OP	293
#define	GE_OP	294
#define	EQ_OP	295
#define	AEQ_OP	296
#define	CROSS	297
#define	MODEL_THEORY	298

#line 17 "xfity.y"


#define __Y_TAB_C__

extern int lex_state;

#define lex(state) lex_state = state;

#include <malloc.h>

#include "console.h"

#include <unistd.h>

#include "matrix.h"

#include "expressi.h"


#line 37 "xfity.y"
typedef union {

  char    cval;
  short   sval;
  int     ival;
  long    lval;
  char *  pval;
  double  dval;
  vector4 vval;
  node *  node;

  } YYSTYPE;
#line 50 "xfity.y"


#include "c_constr.h"

#include "save_exa.h"

#include "actions.h"
#include "minimise.h"
#include "commands.h"

typedef struct {

  parameter_type data;

  double factor;

  } linear_el_type;

void prompt (const char *);

#define PROMPT prompt (NULL);


#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		241
#define	YYFLAG		-32768
#define	YYNTBASE	62

#define YYTRANSLATE(x) ((unsigned)(x) <= 298 ? yytranslate[x] : 114)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    54,
    55,    46,    44,    56,    45,    48,    47,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    50,
    52,    51,    53,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    58,     2,    59,    49,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    60,    57,    61,     2,     2,     2,     2,     2,
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
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     4,     9,    14,    19,    24,    29,    30,    35,    36,
    41,    44,    47,    50,    53,    55,    57,    59,    64,    71,
    75,    79,    83,    85,    87,    89,    91,    93,    97,   101,
   103,   106,   109,   111,   115,   119,   121,   125,   129,   131,
   135,   139,   141,   143,   147,   153,   155,   158,   161,   164,
   166,   168,   170,   174,   178,   182,   186,   188,   192,   196,
   197,   201,   203,   204,   207,   209,   212,   215,   218,   221,
   224,   227,   230,   233,   236,   239,   242,   245,   248,   251,
   254,   257,   260,   263,   265,   267,   270,   271,   274,   276,
   279,   282,   284,   287,   291,   294,   298,   303,   307,   310,
   313,   317,   322,   326,   331,   337,   338,   342,   344,   348,
   351,   354,   357,   360,   361,   364,   367,   369,   372,   373,
   377,   378,   383,   384,   387,   389,   392,   394,   397,   400,
   401,   404,   406,   409,   411,   414,   416,   419,   423,   426,
   427,   430,   433,   437
};

static const short yyrhs[] = {    75,
    40,    75,     0,    75,    41,    75,    83,     0,    75,    50,
    75,    83,     0,    75,    51,    75,    83,     0,    75,    38,
    75,    83,     0,    75,    39,    75,    83,     0,     0,    67,
    65,    52,    75,     0,     0,    68,    66,    52,    82,     0,
    53,    75,     0,    53,    63,     0,    53,    82,     0,    53,
    32,     0,    25,     0,    26,     0,    27,     0,    28,    54,
    75,    55,     0,    29,    54,    75,    56,    75,    55,     0,
    54,    75,    55,     0,    57,    75,    57,     0,    57,    82,
    57,     0,    31,     0,    67,     0,    69,     0,    70,     0,
    71,     0,    80,    48,    80,     0,    72,    30,    72,     0,
    72,     0,    44,    72,     0,    45,    72,     0,    73,     0,
    74,    46,    73,     0,    74,    47,    73,     0,    74,     0,
    75,    44,    74,     0,    75,    45,    74,     0,    68,     0,
    54,    82,    55,     0,    58,    77,    59,     0,    82,     0,
    75,     0,    75,    56,    75,     0,    75,    56,    75,    56,
    75,     0,    76,     0,    76,    49,     0,    44,    78,     0,
    45,    78,     0,    79,     0,    78,     0,    80,     0,    81,
    42,    80,     0,    81,    46,    73,     0,    81,    47,    73,
     0,    74,    46,    80,     0,    81,     0,    82,    44,    81,
     0,    82,    45,    81,     0,     0,    60,    75,    61,     0,
    85,     0,     0,    85,    86,     0,    37,     0,    62,    37,
     0,    63,    37,     0,    64,    37,     0,    89,    37,     0,
     8,    37,     0,    19,    37,     0,    93,    37,     0,    94,
    37,     0,   108,    37,     0,   102,    37,     0,    98,    37,
     0,    97,    37,     0,    92,    37,     0,    91,    37,     0,
    90,    37,     0,    87,    37,     0,    88,    37,     0,     1,
    37,     0,    22,     0,    24,     0,    24,    33,     0,     0,
    23,    32,     0,     6,     0,     6,    32,     0,    20,    32,
     0,    17,     0,    17,    31,     0,    17,    31,    31,     0,
    17,    32,     0,    17,    32,    31,     0,    17,    32,    31,
    31,     0,    16,    52,    31,     0,    53,    16,     0,     9,
   113,     0,     9,    31,   113,     0,     9,    45,    31,   113,
     0,    18,   113,    95,     0,    18,    31,   113,    95,     0,
    18,    31,    31,   113,    95,     0,     0,    60,    96,    61,
     0,    75,     0,    96,    56,    75,     0,    14,    43,     0,
    14,    53,     0,    53,    14,     0,    11,    99,     0,     0,
    99,   100,     0,    36,   101,     0,   112,     0,   101,   112,
     0,     0,    15,   103,   105,     0,     0,    15,    32,   104,
   105,     0,     0,   105,   106,     0,    36,     0,    36,   107,
     0,   112,     0,   107,   112,     0,    10,   109,     0,     0,
   109,   110,     0,    36,     0,    36,   111,     0,   112,     0,
   111,   112,     0,    31,     0,    31,    45,     0,    31,    45,
    31,     0,    45,    31,     0,     0,   113,    67,     0,   113,
    68,     0,   113,    56,    67,     0,   113,    56,    68,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    94,    98,    99,   100,   101,   102,   106,   106,   109,   109,
   112,   116,   120,   127,   131,   134,   137,   140,   143,   147,
   148,   149,   150,   151,   152,   153,   154,   155,   156,   160,
   161,   162,   166,   167,   168,   171,   172,   173,   177,   178,
   179,   183,   184,   185,   186,   190,   191,   195,   196,   200,
   201,   205,   206,   207,   208,   209,   212,   213,   214,   217,
   218,   222,   226,   227,   231,   232,   233,   234,   235,   236,
   237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
   247,   248,   249,   250,   255,   256,   260,   261,   265,   266,
   270,   274,   275,   276,   277,   278,   279,   283,   291,   301,
   302,   303,   308,   310,   312,   316,   317,   321,   322,   326,
   329,   331,   336,   340,   341,   345,   349,   351,   356,   357,
   357,   358,   361,   362,   366,   367,   371,   373,   378,   382,
   383,   387,   388,   392,   394,   399,   400,   401,   402,   405,
   406,   407,   408,   409
};

static const char * const yytname[] = {   "$","error","$undefined.","EDGE","EXAFSW",
"FTW","DATA","KWEIGHT","RESTORE","REFINE","LIST","DELETE","GROUP","EXIT","C_MODEL",
"SAVE","CHECK_DERIVS","PRINT","MONTECARLO","CLEAN","SYSTEM","INVALID","INCLUDE",
"LOG_FILE","HELP","IDENTIFIER","VECTOR_ID","SC_F0","SC_F1","SC_F2","POW_OP",
"CONSTANT","STRING_LIT","HELP_TOPIC","WINDOW_EDGE","EXAFS_EDGE","LIST_PARAMETER",
"EOL","LE_OP","GE_OP","EQ_OP","AEQ_OP","CROSS","MODEL_THEORY","'+'","'-'","'*'",
"'/'","'.'","'^'","'<'","'>'","'='","'?'","'('","')'","','","'|'","'['","']'",
"'{'","'}'","constraint_expr","relational_expr","assignment_expr","@1","@2",
"id","v_id","s_f0","s_f1","s_f2","primary_s_expr","unary_s_expr","multiplicative_s_expr",
"s_expr","primary_v_expr","comma_expr","postfix_v_expr","unary_v_expr","d_v_expr",
"multiplicative_v_expr","v_expr","sigma","translation_unit","statement_list",
"statement","help_statement","log_statement","data_statement","system_statement",
"print_statement","check_d_statement","refine_statement","montecarlo_statement",
"ex_list","exc_list","model_statement","delete_statement","delete_list","delete_list_entry",
"delete_range_list","save_statement","@3","@4","save_list","save_list_entry",
"save_range_list","list_statement","list_list","list_list_entry","list_range_list",
"list_range","id_list",""
};
#endif

static const short yyr1[] = {     0,
    62,    63,    63,    63,    63,    63,    65,    64,    66,    64,
    64,    64,    64,    64,    67,    68,    69,    70,    71,    72,
    72,    72,    72,    72,    72,    72,    72,    72,    72,    73,
    73,    73,    74,    74,    74,    75,    75,    75,    76,    76,
    76,    77,    77,    77,    77,    78,    78,    79,    79,    80,
    80,    81,    81,    81,    81,    81,    82,    82,    82,    83,
    83,    84,    85,    85,    86,    86,    86,    86,    86,    86,
    86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
    86,    86,    86,    86,    87,    87,    88,    88,    89,    89,
    90,    91,    91,    91,    91,    91,    91,    92,    92,    93,
    93,    93,    94,    94,    94,    95,    95,    96,    96,    97,
    97,    97,    98,    99,    99,   100,   101,   101,   103,   102,
   104,   102,   105,   105,   106,   106,   107,   107,   108,   109,
   109,   110,   110,   111,   111,   112,   112,   112,   112,   113,
   113,   113,   113,   113
};

static const short yyr2[] = {     0,
     3,     4,     4,     4,     4,     4,     0,     4,     0,     4,
     2,     2,     2,     2,     1,     1,     1,     4,     6,     3,
     3,     3,     1,     1,     1,     1,     1,     3,     3,     1,
     2,     2,     1,     3,     3,     1,     3,     3,     1,     3,
     3,     1,     1,     3,     5,     1,     2,     2,     2,     1,
     1,     1,     3,     3,     3,     3,     1,     3,     3,     0,
     3,     1,     0,     2,     1,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     1,     1,     2,     0,     2,     1,     2,
     2,     1,     2,     3,     2,     3,     4,     3,     2,     2,
     3,     4,     3,     4,     5,     0,     3,     1,     3,     2,
     2,     2,     2,     0,     2,     2,     1,     2,     0,     3,
     0,     4,     0,     2,     1,     2,     1,     2,     2,     0,
     2,     1,     2,     1,     2,     1,     2,     3,     2,     0,
     2,     2,     3,     3
};

static const short yydefact[] = {    63,
     0,     0,    89,     0,   140,   130,   114,     0,   119,     0,
    92,   140,     0,     0,    84,     0,    85,    15,    16,    17,
     0,     0,    23,    65,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    24,    39,    25,    26,    27,    30,    33,
    36,     0,    46,    51,    50,     0,    64,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
    90,    70,   140,     0,   100,   129,   113,   110,   111,   121,
   123,     0,    93,    95,   140,   106,    71,    91,    88,    86,
     0,     0,     0,     0,    24,    39,    31,    48,    32,    49,
   112,    99,    14,    12,    36,    11,    52,    57,    13,     0,
     0,     0,     0,    43,     0,    42,    66,    67,    68,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    47,     0,    81,    82,    69,    80,    79,    78,
    72,    73,    77,    76,    75,    74,   101,   140,     0,   141,
   142,   132,   131,     0,   115,   123,   120,    98,    94,    96,
   140,   106,     0,   103,     0,     0,     0,    48,    49,     0,
     0,     0,     0,     0,     0,    20,    40,    21,    22,     0,
    41,     0,     0,    29,    34,    35,    60,    60,     1,    60,
    37,    38,    60,    60,    28,   102,   143,   144,   136,     0,
   133,   134,   116,   117,   122,   125,   124,    97,   106,   104,
   108,     0,    18,     0,     0,    56,    53,    54,    55,    58,
    59,    44,     8,    10,     0,     5,     6,     2,     3,     4,
   137,   139,   135,   118,   126,   127,   105,     0,   107,     0,
     0,     0,   138,   128,   109,    19,    45,    61,     0,     0,
     0
};

static const short yydefgoto[] = {    31,
    32,    33,   110,   111,    85,    86,    36,    37,    38,    39,
    40,    41,    42,    43,   105,    44,    45,    46,    98,   101,
   216,   239,     1,    47,    48,    49,    50,    51,    52,    53,
    54,    55,   154,   202,    56,    57,    67,   145,   193,    58,
    71,   146,   147,   197,   225,    59,    66,   143,   191,   192,
    65
};

static const short yypact[] = {-32768,
   199,   -30,    10,   -19,    -6,-32768,-32768,    95,    15,    13,
    19,    42,    56,    64,-32768,    72,    94,-32768,-32768,-32768,
    82,    93,-32768,-32768,    74,    74,   233,   241,   241,   241,
   102,   114,   117,   112,   116,-32768,-32768,-32768,   141,-32768,
    66,    70,   127,-32768,-32768,   146,-32768,   160,   161,   166,
   183,   195,   196,   200,   201,   202,   203,   204,   205,-32768,
-32768,-32768,-32768,   214,    11,   210,   212,-32768,-32768,-32768,
-32768,   219,   220,   223,   224,     1,-32768,-32768,-32768,-32768,
   241,   241,   -10,   -10,-32768,-32768,   141,-32768,   141,-32768,
-32768,-32768,-32768,-32768,    87,   122,   146,   149,    33,    62,
   130,    -4,    78,    31,   215,    33,-32768,-32768,-32768,   211,
   221,    74,   241,   241,   241,   241,   241,   241,   241,   241,
   241,   241,-32768,    26,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,    11,-32768,   158,-32768,
-32768,    18,-32768,    18,-32768,-32768,   235,-32768,-32768,   244,
-32768,     1,   241,-32768,   135,    41,   241,-32768,-32768,   241,
    26,   241,   241,   241,   241,-32768,-32768,-32768,-32768,   241,
-32768,   241,   241,-32768,-32768,-32768,   -14,   -14,   142,   -14,
    66,    66,   -14,   -14,-32768,    11,-32768,-32768,   231,   248,
    18,-32768,    18,-32768,   235,    18,-32768,-32768,     1,-32768,
   142,   -18,-32768,   241,    87,   146,-32768,-32768,-32768,   149,
   149,    85,   142,    33,   241,-32768,-32768,-32768,-32768,-32768,
   249,-32768,-32768,-32768,    18,-32768,-32768,   241,-32768,   137,
   241,   -16,-32768,-32768,   142,-32768,   142,-32768,   281,   282,
-32768
};

static const short yypgoto[] = {-32768,
   256,-32768,-32768,-32768,     3,     7,-32768,-32768,-32768,   -20,
  -104,     5,   -27,-32768,-32768,    -2,-32768,    -8,    47,   -15,
    51,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,  -139,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,   138,-32768,-32768,-32768,-32768,-32768,-32768,  -127,
    -1
};


#define	YYLAST		299


static const short yytable[] = {    96,
   100,   102,   104,    34,    87,    89,    60,    35,   175,   176,
    76,    99,   200,   103,   106,    19,   194,    62,    97,    97,
    97,    97,    88,    90,    63,    18,    19,   119,   120,   119,
   120,    95,    95,    95,    95,    18,    19,   228,    64,   119,
   120,    61,   229,   157,   238,   215,    70,    30,   189,    73,
    74,    19,   168,   155,   156,   175,   139,   208,   209,   227,
   153,   137,   190,   223,    72,   224,   139,   140,   226,    83,
    84,   141,    75,   152,   119,   120,   164,   165,   140,   157,
   158,   159,   141,    30,   119,   120,   170,   177,   178,   179,
   180,   174,    77,   183,   184,    78,   204,   234,    18,    19,
    20,    21,    22,    79,    23,   119,   120,   115,   116,   117,
   118,   113,   114,   119,   120,   185,   166,    83,    84,   121,
   122,   164,   165,   181,   182,   201,    80,    28,   119,   120,
    29,    30,   160,   114,   169,    81,   186,    68,   107,   140,
   231,   187,   212,   141,   213,   188,    82,    69,    97,   199,
   108,   206,   207,   109,   140,    97,    97,   214,   141,   115,
   116,   205,   118,    -7,    97,   119,   120,    -9,   205,   205,
   112,   121,   122,   164,   165,   123,   230,   205,   119,   120,
   119,   120,    18,    19,   167,   119,   120,   232,   140,   203,
   161,   236,   141,   124,   162,   163,   125,   126,   -62,     2,
   235,   140,   127,   237,     3,   141,     4,     5,     6,     7,
   210,   211,     8,     9,    10,    11,    12,    13,    14,   128,
    15,    16,    17,    18,    19,    20,    21,    22,   217,    23,
   218,   129,   130,   219,   220,    24,   131,   132,   133,   134,
   135,   136,    25,    26,   138,   142,    91,   144,    92,   148,
   149,    27,    28,   150,   151,    29,    30,    18,    19,    20,
    21,    22,   172,    23,    93,    18,    19,    20,    21,    22,
   196,    23,   173,   171,   198,   221,    25,    26,   222,   233,
   240,   241,    94,   195,    25,    26,    28,     0,     0,    29,
    30,     0,     0,     0,    28,     0,     0,    29,    30
};

static const short yycheck[] = {    27,
    28,    29,    30,     1,    25,    26,    37,     1,   113,   114,
    12,    27,   152,    29,    30,    26,   144,    37,    27,    28,
    29,    30,    25,    26,    31,    25,    26,    44,    45,    44,
    45,    27,    28,    29,    30,    25,    26,    56,    45,    44,
    45,    32,    61,    54,    61,    60,    32,    58,    31,    31,
    32,    26,    57,    81,    82,   160,    56,   162,   163,   199,
    60,    63,    45,   191,    52,   193,    56,    65,   196,    44,
    45,    65,    31,    75,    44,    45,    44,    45,    76,    54,
    83,    84,    76,    58,    44,    45,    56,   115,   116,   117,
   118,   112,    37,   121,   122,    32,    56,   225,    25,    26,
    27,    28,    29,    32,    31,    44,    45,    38,    39,    40,
    41,    46,    47,    44,    45,   124,    55,    44,    45,    50,
    51,    44,    45,   119,   120,   153,    33,    54,    44,    45,
    57,    58,    46,    47,    57,    54,   138,    43,    37,   137,
    56,   139,   170,   137,   172,   139,    54,    53,   157,   151,
    37,   160,   161,    37,   152,   164,   165,   173,   152,    38,
    39,   157,    41,    52,   173,    44,    45,    52,   164,   165,
    30,    50,    51,    44,    45,    49,   204,   173,    44,    45,
    44,    45,    25,    26,    55,    44,    45,   215,   186,    55,
    42,    55,   186,    48,    46,    47,    37,    37,     0,     1,
   228,   199,    37,   231,     6,   199,     8,     9,    10,    11,
   164,   165,    14,    15,    16,    17,    18,    19,    20,    37,
    22,    23,    24,    25,    26,    27,    28,    29,   178,    31,
   180,    37,    37,   183,   184,    37,    37,    37,    37,    37,
    37,    37,    44,    45,    31,    36,    14,    36,    16,    31,
    31,    53,    54,    31,    31,    57,    58,    25,    26,    27,
    28,    29,    52,    31,    32,    25,    26,    27,    28,    29,
    36,    31,    52,    59,    31,    45,    44,    45,    31,    31,
     0,     0,    27,   146,    44,    45,    54,    -1,    -1,    57,
    58,    -1,    -1,    -1,    54,    -1,    -1,    57,    58
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

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
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 192 "bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#endif

int
yyparse(YYPARSE_PARAM)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 94 "xfity.y"
{ yyval.node = onod2 (EQUTO, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 2:
#line 98 "xfity.y"
{ yyval.node = onod3 (AEQTO, yyvsp[-3].node, yyvsp[-1].node, yyvsp[0].node); ;
    break;}
case 3:
#line 99 "xfity.y"
{ yyval.node = onod3 (LTHAN, yyvsp[-3].node, yyvsp[-1].node, yyvsp[0].node); ;
    break;}
case 4:
#line 100 "xfity.y"
{ yyval.node = onod3 (GTHAN, yyvsp[-3].node, yyvsp[-1].node, yyvsp[0].node); ;
    break;}
case 5:
#line 101 "xfity.y"
{ yyval.node = onod3 (LTHAN, yyvsp[-3].node, yyvsp[-1].node, yyvsp[0].node); ;
    break;}
case 6:
#line 102 "xfity.y"
{ yyval.node = onod3 (GTHAN, yyvsp[-3].node, yyvsp[-1].node, yyvsp[0].node); ;
    break;}
case 7:
#line 106 "xfity.y"
{ lex (yyvsp[0].node->parameter.lex_state); ;
    break;}
case 8:
#line 106 "xfity.y"
{
                                          assign (yyvsp[-3].node, v_value (yyvsp[0].node));
                                          free_tree (yyvsp[-3].node); lex (0); ;
    break;}
case 9:
#line 109 "xfity.y"
{ lex (yyvsp[0].node->parameter.lex_state); ;
    break;}
case 10:
#line 109 "xfity.y"
{
                                          assign (yyvsp[-3].node, v_value (yyvsp[0].node));
                                          free_tree (yyvsp[-3].node); lex (0); ;
    break;}
case 11:
#line 112 "xfity.y"
{ cout ("\n   ");
                                          fprintf_expression (stdout, yyvsp[0].node, 0);
                                          cout (" = %.8g\n\n",
                                                               s_value (yyvsp[0].node)); ;
    break;}
case 12:
#line 116 "xfity.y"
{ cout ("\n   ");
                                          fprintf_expression (stdout, yyvsp[0].node, 0);
                                          cout (" = %.8g\n\n",
                                                               s_value (yyvsp[0].node)); ;
    break;}
case 13:
#line 120 "xfity.y"
{ vector4 v;
                                          cout ("\n   ");
                                          fprintf_expression (stdout, yyvsp[0].node, 0);
                                          v = v_value (yyvsp[0].node);
                                          cout (" = (%.8g, %.8g, %.8g)\n\n",
                                              v_el (v, 0), v_el (v, 1),
                                              v_el (v, 2)); ;
    break;}
case 14:
#line 127 "xfity.y"
{ cout (yyvsp[0].pval);
                                          cout ("\n"); ;
    break;}
case 17:
#line 137 "xfity.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 18:
#line 140 "xfity.y"
{ yyval.node = node_op1 (yyvsp[-3].node, yyvsp[-1].node); ;
    break;}
case 19:
#line 143 "xfity.y"
{ yyval.node = node_op2 (yyvsp[-5].node, yyvsp[-3].node, yyvsp[-1].node); ;
    break;}
case 20:
#line 147 "xfity.y"
{ yyval.node = yyvsp[-1].node; ;
    break;}
case 21:
#line 148 "xfity.y"
{ yyval.node = onod1 (ABS, yyvsp[-1].node); ;
    break;}
case 22:
#line 149 "xfity.y"
{ yyval.node = onod1 (VABS, yyvsp[-1].node); ;
    break;}
case 23:
#line 150 "xfity.y"
{ yyval.node = cnode (CONST, yyvsp[0].dval); ;
    break;}
case 28:
#line 155 "xfity.y"
{ yyval.node = onod2 (VDOT, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 29:
#line 156 "xfity.y"
{ yyval.node = node_op2 (yyvsp[-1].node, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 31:
#line 161 "xfity.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 32:
#line 162 "xfity.y"
{ yyval.node = onod1 (NEG, yyvsp[0].node); ;
    break;}
case 34:
#line 167 "xfity.y"
{ yyval.node = onod2 (MUL, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 35:
#line 168 "xfity.y"
{ yyval.node = onod2 (DIV, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 37:
#line 172 "xfity.y"
{ yyval.node = onod2 (ADD, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 38:
#line 173 "xfity.y"
{ yyval.node = onod2 (SUB, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 40:
#line 178 "xfity.y"
{ yyval.node = yyvsp[-1].node; ;
    break;}
case 41:
#line 179 "xfity.y"
{ yyval.node = yyvsp[-1].node; ;
    break;}
case 43:
#line 184 "xfity.y"
{ yyval.node = onod1 (VCAST, yyvsp[0].node); ;
    break;}
case 44:
#line 185 "xfity.y"
{ yyval.node = onod2 (VCAST, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 45:
#line 186 "xfity.y"
{ yyval.node = onod3 (VCAST, yyvsp[-4].node, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 47:
#line 191 "xfity.y"
{ yyval.node = onod1 (CARET, yyvsp[-1].node); ;
    break;}
case 48:
#line 195 "xfity.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 49:
#line 196 "xfity.y"
{ yyval.node = onod1 (VNEG, yyvsp[0].node); ;
    break;}
case 53:
#line 206 "xfity.y"
{ yyval.node = onod2 (VCRO, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 54:
#line 207 "xfity.y"
{ yyval.node = onod2 (VMUL, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 55:
#line 208 "xfity.y"
{ yyval.node = onod2 (VDIV, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 56:
#line 209 "xfity.y"
{ yyval.node = onod2 (VMUL, yyvsp[0].node, yyvsp[-2].node); ;
    break;}
case 58:
#line 213 "xfity.y"
{ yyval.node = onod2 (VADD, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 59:
#line 214 "xfity.y"
{ yyval.node = onod2 (VSUB, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 60:
#line 217 "xfity.y"
{ yyval.node = cnode (CONST, 1.0); ;
    break;}
case 61:
#line 218 "xfity.y"
{ yyval.node = yyvsp[-1].node;                 ;
    break;}
case 62:
#line 222 "xfity.y"
{ clean_act; ;
    break;}
case 63:
#line 226 "xfity.y"
{ PROMPT ;
    break;}
case 64:
#line 227 "xfity.y"
{ lex (0); ;
    break;}
case 65:
#line 231 "xfity.y"
{ PROMPT ;
    break;}
case 66:
#line 232 "xfity.y"
{ PROMPT add_cons (yyvsp[-1].node); ;
    break;}
case 67:
#line 233 "xfity.y"
{ PROMPT add_rest (yyvsp[-1].node); ;
    break;}
case 68:
#line 234 "xfity.y"
{ PROMPT ;
    break;}
case 69:
#line 235 "xfity.y"
{ PROMPT ;
    break;}
case 70:
#line 236 "xfity.y"
{ PROMPT restore_act; ;
    break;}
case 71:
#line 237 "xfity.y"
{ PROMPT clean_act; ;
    break;}
case 72:
#line 238 "xfity.y"
{ PROMPT ;
    break;}
case 73:
#line 239 "xfity.y"
{ PROMPT ;
    break;}
case 74:
#line 240 "xfity.y"
{ PROMPT ;
    break;}
case 75:
#line 241 "xfity.y"
{ PROMPT ;
    break;}
case 76:
#line 242 "xfity.y"
{ PROMPT ;
    break;}
case 77:
#line 243 "xfity.y"
{ PROMPT ;
    break;}
case 78:
#line 244 "xfity.y"
{ PROMPT ;
    break;}
case 79:
#line 245 "xfity.y"
{ PROMPT ;
    break;}
case 80:
#line 246 "xfity.y"
{ PROMPT ;
    break;}
case 81:
#line 247 "xfity.y"
{ PROMPT ;
    break;}
case 82:
#line 248 "xfity.y"
{ PROMPT ;
    break;}
case 83:
#line 249 "xfity.y"
{ yyerrok; ;
    break;}
case 85:
#line 255 "xfity.y"
{ help_command (""); ;
    break;}
case 86:
#line 256 "xfity.y"
{ help_command (yyvsp[0].pval); free (yyvsp[0].pval); ;
    break;}
case 88:
#line 261 "xfity.y"
{ log_command (yyvsp[0].pval); ;
    break;}
case 89:
#line 265 "xfity.y"
{ exafs_act (""); ;
    break;}
case 90:
#line 266 "xfity.y"
{ exafs_act (yyvsp[0].pval); free (yyvsp[0].pval); ;
    break;}
case 91:
#line 270 "xfity.y"
{ system_command (yyvsp[0].pval); free (yyvsp[0].pval); ;
    break;}
case 92:
#line 274 "xfity.y"
{ print_act ("", 0, 1); ;
    break;}
case 93:
#line 275 "xfity.y"
{ print_act ("", yyvsp[0].dval, 1); ;
    break;}
case 94:
#line 276 "xfity.y"
{ print_act ("", yyvsp[-1].dval, yyvsp[0].dval); ;
    break;}
case 95:
#line 277 "xfity.y"
{ print_act (yyvsp[0].pval, 0, 1); ;
    break;}
case 96:
#line 278 "xfity.y"
{ print_act (yyvsp[-1].pval, yyvsp[0].dval, 1); ;
    break;}
case 97:
#line 279 "xfity.y"
{ print_act (yyvsp[-2].pval, yyvsp[-1].dval, yyvsp[0].dval); ;
    break;}
case 98:
#line 283 "xfity.y"
{ check_derivatives = max (0, yyvsp[0].dval);
                                          if (check_derivatives)
                                            cout ("\n derivative check is ON"
                                                    " at level %d\n\n",
                                                    check_derivatives);
                                          else
                                            cout ("\n derivative check is"
                                                    " OFF\n\n"); ;
    break;}
case 99:
#line 291 "xfity.y"
{ if (check_derivatives)
                                            cout ("\n derivative check is ON"
                                                    " at level %d\n\n",
                                                    check_derivatives);
                                          else
                                            cout ("\n derivative check is"
                                                    " OFF\n\n"); ;
    break;}
case 100:
#line 301 "xfity.y"
{ refine_act (20, yyvsp[0].node); free_tree (yyvsp[0].node); ;
    break;}
case 101:
#line 302 "xfity.y"
{ refine_act (yyvsp[-1].dval, yyvsp[0].node); free_tree (yyvsp[0].node); ;
    break;}
case 102:
#line 303 "xfity.y"
{ refine_act (-(yyvsp[-1].dval), yyvsp[0].node);
                                                               free_tree (yyvsp[0].node); ;
    break;}
case 103:
#line 308 "xfity.y"
{ montecarlo_act (16, 20, yyvsp[-1].node, (node **) yyvsp[0].pval);
                                                  free_tree (yyvsp[-1].node); free_check (yyvsp[0].pval); ;
    break;}
case 104:
#line 310 "xfity.y"
{ montecarlo_act (yyvsp[-2].dval, 20, yyvsp[-1].node, (node **) yyvsp[0].pval);
                                                  free_tree (yyvsp[-1].node); free_check (yyvsp[0].pval); ;
    break;}
case 105:
#line 312 "xfity.y"
{ montecarlo_act (yyvsp[-3].dval, yyvsp[-2].dval, yyvsp[-1].node,(node **) yyvsp[0].pval);
                                                          free_tree (yyvsp[-1].node); free_check (yyvsp[0].pval); ;
    break;}
case 106:
#line 316 "xfity.y"
{ yyval.pval = NULL; ;
    break;}
case 107:
#line 317 "xfity.y"
{ yyval.pval = yyvsp[-1].pval; ;
    break;}
case 108:
#line 321 "xfity.y"
{ yyval.pval = (char *) node_list (NULL, (node *) yyvsp[0].node); ;
    break;}
case 109:
#line 322 "xfity.y"
{ yyval.pval = (char *) node_list ((node **) yyvsp[-2].pval, (node *) yyvsp[0].node); ;
    break;}
case 110:
#line 326 "xfity.y"
{ change_model (&global_data, yyvsp[0].pval);
                                          cout ("\n new theory : %s\n\n",
                                          global_data.model->description); ;
    break;}
case 111:
#line 329 "xfity.y"
{ cout ("\n current theory : %s\n\n",
                                          global_data.model->description); ;
    break;}
case 112:
#line 331 "xfity.y"
{ cout ("\n current theory : %s\n\n",
                                          global_data.model->description); ;
    break;}
case 117:
#line 349 "xfity.y"
{ delete_act (yyvsp[-1].lval, v_el (yyvsp[0].vval, 0),
                                                               v_el (yyvsp[0].vval, 1)); ;
    break;}
case 118:
#line 351 "xfity.y"
{ delete_act (yyvsp[-2].lval, v_el (yyvsp[0].vval, 0),
                                                               v_el (yyvsp[0].vval, 1)); ;
    break;}
case 119:
#line 356 "xfity.y"
{ save_act ("", 0, 0, 0); ;
    break;}
case 121:
#line 357 "xfity.y"
{ save_act (yyvsp[0].pval, 0, 0, 0); ;
    break;}
case 125:
#line 366 "xfity.y"
{ save_act (NULL, yyvsp[0].lval, 0, 999); ;
    break;}
case 127:
#line 371 "xfity.y"
{ save_act (NULL, yyvsp[-1].lval,
                                                 v_el (yyvsp[0].vval, 0), v_el (yyvsp[0].vval, 1)); ;
    break;}
case 128:
#line 373 "xfity.y"
{ save_act (NULL, yyvsp[-2].lval,
                                                 v_el (yyvsp[0].vval, 0), v_el (yyvsp[0].vval, 1)); ;
    break;}
case 132:
#line 387 "xfity.y"
{ list_act (yyvsp[0].lval, 0, 999); ;
    break;}
case 134:
#line 392 "xfity.y"
{ list_act (yyvsp[-1].lval, v_el (yyvsp[0].vval, 0),
                                                              v_el (yyvsp[0].vval, 1)); ;
    break;}
case 135:
#line 394 "xfity.y"
{ list_act (yyvsp[-2].lval, v_el (yyvsp[0].vval, 0),
                                                              v_el (yyvsp[0].vval, 1)); ;
    break;}
case 136:
#line 399 "xfity.y"
{ yyval.vval = make_vector2 (yyvsp[0].dval,  yyvsp[0].dval); ;
    break;}
case 137:
#line 400 "xfity.y"
{ yyval.vval = make_vector2 (yyvsp[-1].dval, 999); ;
    break;}
case 138:
#line 401 "xfity.y"
{ yyval.vval = make_vector2 (yyvsp[-2].dval,  yyvsp[0].dval); ;
    break;}
case 139:
#line 402 "xfity.y"
{ yyval.vval = make_vector2 ( 0,  yyvsp[0].dval); ;
    break;}
case 140:
#line 405 "xfity.y"
{ yyval.node = NULL; ;
    break;}
case 141:
#line 406 "xfity.y"
{ yyval.node = add_node1 (&(yyvsp[-1].node), yyvsp[0].node); ;
    break;}
case 142:
#line 407 "xfity.y"
{ yyval.node = add_node1 (&(yyvsp[-1].node), yyvsp[0].node); ;
    break;}
case 143:
#line 408 "xfity.y"
{ yyval.node = add_node1 (&(yyvsp[-2].node), yyvsp[0].node); ;
    break;}
case 144:
#line 409 "xfity.y"
{ yyval.node = add_node1 (&(yyvsp[-2].node), yyvsp[0].node); ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 487 "bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 412 "xfity.y"


#include <stdio.h>

