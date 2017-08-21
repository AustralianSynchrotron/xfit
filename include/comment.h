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

#if !defined (__COMMENT_H__)

#define __COMMENT_H__

#include "header.h"

typedef struct comment_t_s {

  int line, column;

  struct comment_t_s * next;

  char * text;

  } comment_type;

int destruct_comment (comment_type ** c);

int add_comment (comment_type ** c, int line,
                                    int column, char text []);

int fprint_comment (FILE * file, char format [],
                                 comment_type * comment);

#endif /* __COMMENT_H__ */

