/***********************************************************************}
{                                                                       }
{  Purpose:                                                             }
{                                                                       }
{       Routines and structures for comment text.                       }
{                                                                       }
{  Content:                                                             }
{                                                                       }
{       type:    comment_type                                           }
{                                                                       }
{       routine: add_comment destruct_comment fprint_comment            }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__COMMENT_C__)

#define __COMMENT_C__

#include "comment.h"

#include "console.h"


int destruct_comment (comment_type ** c) {

  comment_type * cc, * next;

  for (cc = *c; cc; cc = next) {

    next = (cc)->next;

    free_check ((cc)->text);
    free_check ((cc));

    }

  *c = NULL;

  return 1;

  }

int add_comment (comment_type ** c, int line,
                                    int column, char text []) {

  if (text ? text [0] == 0 : 1)

    return 0;

  while (*c)

    c = &((*c)->next);

  *c = calloc_check (1, sizeof (comment_type));

  (*c)->text = calloc_check (strlen (text) + 1, sizeof (char));

  strcpy ((*c)->text, text);

  (*c)->column = column;

  (*c)->line = line;

  return 1;

  }

void check_line (FILE * file, char * line) {

  if (strlen (line) > 128) {

    fcout (file, "%s", line);

    line [0] = 0;

    }
  }

int fprint_comment (FILE * file, char format [],
                                 comment_type * comment) {

  char line [256];
  
  int column, count;

  comment_type * comment0;

  char * text;

  line [0] = 0;

  for (comment0 = 0; comment; comment0 = comment, comment = comment->next) {

    check_line (file, line);

    if (comment0 ? comment->line != comment0->line : TRUE) {

      column = 0;

      sprintf (line + strlen (line), format, comment->line);

      check_line (file, line);

      }

    for (; column < comment->column; column++) {

      strcat (line, " ");

      check_line (file, line);

      }

    for (text = comment->text; text [0]; text++) {

      if (text [0] == '\t') {

        strcat (line, NSPACES (8 - column % 8));

        column += 8 - column % 8;

        }
      else {

        count = strlen (line);
        
        line [count] = text [0];

        line [count + 1] = 0;

        column++;

        }

      check_line (file, line);

      }
    }

  fcout (file, "%s\n", line);

  return 1;

  }

#endif /* __COMMENT_C__ */

