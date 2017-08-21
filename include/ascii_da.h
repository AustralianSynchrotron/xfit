/***********************************************************************}
{                                                                       }
{  Purpose:                                                             }
{                                                                       }
{       Routines and structures for ASCII data files.                   }
{                                                                       }
{  Content:                                                             }
{                                                                       }
{       type:    ascii_data_type                                        }
{                                                                       }
{       data:    ASCII_next                                             }
{                                                                       }
{       routine: ASCII_next_word destruct_ascii_data ascii_file_column  }
{                ascii_file_column_l read_ascii_data_file               }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__ASCII_DATA_H__)

#define __ASCII_DATA_H__

#include "scan.h"

#include "comment.h"

#if !defined (A_MAXLINE)

#define A_MAXLINE 256

#endif

#if !defined (A_MAXCOLUMNS)

#define A_MAXCOLUMNS 32

#endif

typedef struct {

  char * filename;

  comment_type * comment;

  int columns, entries;

  char * heading [A_MAXCOLUMNS];

  double * column [A_MAXCOLUMNS];

  char * command;

  } ascii_data_type;

#if !defined (__ASCII_DATA_C__)

extern char * ASCII_next;

#else

char * ASCII_next = NULL;

#endif

char * ASCII_next_word (void);

void destruct_ascii_data (ascii_data_type * A);

int ascii_file_column (char heading [], ascii_data_type * A);

int ascii_file_column_l (char * heading_list [], ascii_data_type * A,
                         int  * matched);

int read_ascii_data_file (char filename [], ascii_data_type * A);

#endif /* __ASCII_DATA_H__ */
