/***********************************************************************}
{                                                                       }
{  This file contains the routines used to control an include stack     }
{                                                                       }
{***********************************************************************/

#if !defined (__INCLUDE_H__)

#define __INCLUDE_H__

#include <sys/types.h>

#include "common.h"

#if (YYLMAX > 1024)

#define BUF_SIZE YYLMAX

#else

#define BUF_SIZE 1024

#endif


  /*********************************************************************}
  {                                                                     }
  {  Define the structure use to hold the state of an input file        }
  {                                                                     }
  {*********************************************************************/

typedef struct file_stack_type_ {

  FILE * file;          /* -> file */

  char * file_name;     /* file name */

  char c [BUF_SIZE];    /* the last BUF_SIZE characters read */

  long   line, column,  /* current position (line / column) */

         stack,         /* size of the character stack */

         old,           /* index of the oldest saved character */

         saved;         /* number of saved characters */

  struct file_stack_type_ * previous;   /* -> next file on the stack */

  } file_stack_type;


  /*********************************************************************}
  {                                                                     }
  {  Put a new file on top of the stack                                 }
  {                                                                     }
  {  Return 0 on failure                                                }
  {                                                                     }
  {*********************************************************************/

int include_file (char filename [], file_stack_type ** i_stack);


  /*********************************************************************}
  {                                                                     }
  {  Terminate the file currently on top of the stack                   }
  {                                                                     }
  {  Return 0 on if the stack is not empty                              }
  {                                                                     }
  {*********************************************************************/

int end_include_file (file_stack_type ** i_stack);


  /*********************************************************************}
  {                                                                     }
  {  Change the position depending on the character read                }
  {                                                                     }
  {*********************************************************************/

void move_position (long * line, long * column, char c);


  /*********************************************************************}
  {                                                                     }
  {  Calculate the current position in the file                         }
  {                                                                     }
  {*********************************************************************/

void get_position (long * line, long * column, file_stack_type * i_file);



#endif /* __INCLUDE_H__ */
