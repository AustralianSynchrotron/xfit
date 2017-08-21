/***********************************************************************}
{                                                                       }
{  This file contains the routines used to control an include stack     }
{                                                                       }
{***********************************************************************/

#if !defined (__INCLUDE_C__)

#define __INCLUDE_C__

#include "include.h"

#include "console.h"

#include "misc_com.h"

extern int BackGround;


  /*********************************************************************}
  {                                                                     }
  {  Put a new file on top of the stack                                 }
  {                                                                     }
  {  Return 0 on failure                                                }
  {                                                                     }
  {*********************************************************************/

int include_file (char filename [], file_stack_type ** i_stack) {

  FILE * file;

  file_stack_type * include;

  char * found;
  

  file = NULL;

  found = filename;

    /* If the file is "stdin" use that stream */

  if (!strcmp (filename, "stdin"))

    fflush (file = stdin);

  else {

      /* Open the file */

    found = find_file (filename);

    if (found)
        
      file = fopen (found, "r");

    }


  if (file == NULL)

      /* Failure */

    return 0;


  BackGround = file != stdin;
  

    /* Create the new entry */

  include = calloc_check (1, sizeof (file_stack_type));


    /* Initialise the new entry */

  include->file = file;

  include->file_name = strdup (found);

  include->line   = 0;
  include->column = 0;

  include->c [0] = '\n';

  include->stack = 0;

  include->saved = 1;

  include->old   = 0;


    /* Point back to the previous entry */

  include->previous = * i_stack;


    /* Move the stack pointer to this entry */

  *i_stack = include;


    /* Return */

  return 1;

  }


  /*********************************************************************}
  {                                                                     }
  {  Terminate the file currently on top of the stack                   }
  {                                                                     }
  {  Return 0 on if the stack is not empty                              }
  {                                                                     }
  {*********************************************************************/

int end_include_file (file_stack_type ** i_stack) {

  file_stack_type * include;


    /* Get the file on top of the stack */

  include = *i_stack;


    /* Move the stack pointer to the previous file */

  *i_stack = include->previous;

  if (*i_stack)

    BackGround = (*i_stack)->file != stdin;

  else

    BackGround = 1;
  

    /* Close the file and free the entry */

  if (include->file != stdin)

    fclose (include->file);

  free_check (include);


    /* Return <> 0 if the stack is empty */

  return *i_stack == NULL;

  }


  /*********************************************************************}
  {                                                                     }
  {  Change the position depending on the character read                }
  {                                                                     }
  {*********************************************************************/

void move_position (long * line, long * column, char c) {

  switch (c) {

    case '\n' :

      (*line)++;

      *column = 0;

      break;

    case '\t' :

      *column = (*column / 8) * 8;

      break;

    default :

      (*column)++;

    }
  }


  /*********************************************************************}
  {                                                                     }
  {  Calculate the current position in the file                         }
  {                                                                     }
  {*********************************************************************/

void get_position (long * line, long * column, file_stack_type * i_file) {

  int i;


    /* Get the position of the last character saved */

  *line = i_file->line;

  *column = i_file->column;


    /* Change the position using the saved characters */

  for (i = 0; i < i_file->saved; i++)

    move_position (line, column, i_file->c [(i_file->old + i) % BUF_SIZE]);

  }


#endif /* __INCLUDE_C__ */
