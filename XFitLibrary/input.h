/***********************************************************************}
{                                                                       }
{  This file contains the routines used by the lexer to control input   }
{                                                                       }
{  The routines input and unput are used by lex.yy.c                    }
{                                                                       }
{  The routine yyerror is used by y.tab.c                               }
{                                                                       }
{***********************************************************************/

#if !defined (__INPUT_H__)

#define __INPUT_H__

#include "include.h"

#include "event.h"

#if defined (__INPUT_C__)

file_stack_type * command_in = NULL;

int BackGround = 0;
int NoAntialias = 0;
int NoDoubleBuffer = 0;

#else

extern file_stack_type * command_in;

extern int BackGround;
extern int NoAntialias;
extern int NoDoubleBuffer;

extern FILE * yyin;

#endif


  /*********************************************************************}
  {                                                                     }
  {  Include command called by the lexer                                }
  {                                                                     }
  {*********************************************************************/

int include_command (char filename [], file_stack_type ** i_stack);


  /*********************************************************************}
  {                                                                     }
  {  input is called by the lexer (lex.yy.c) to get a character         }
  {                                                                     }
  {*********************************************************************/

#if defined (FLEX_SCANNER)
#define YY_INPUT(buf,result,max_size) \
                { int c; \
                  c = xf_input (); \
                  if (c) { buf [0] = c; result = 1; } else { result = YY_NULL; } }
#endif

int xf_input (void);


  /*********************************************************************}
  {                                                                     }
  {  unput is called by the lexer (lex.yy.c) to return a character      }
  {                                                                     }
  {*********************************************************************/

// void unput (int c);


  /*********************************************************************}
  {                                                                     }
  {  yyerror is called by the parser (y.tab.c) to report any error      }
  {                                                                     }
  {*********************************************************************/

void yyerror (const char * message);



#endif /* __INPUT_H__ */
