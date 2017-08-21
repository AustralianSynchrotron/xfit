/***********************************************************************}
{                                                                       }
{  This file contains the routine common to FEFF4, FEFF5 or FEFF6.      }
{                                                                       }
{  Copyright (C) 1994  Paul J. Ellis  University of Sydney              }
{                                                                       }
{***********************************************************************/

#if !defined (__FEFF__H)

#define __FEFF__H

#include "header.h"

#include "exafs.h"

#include "feff_ms.h"

#define LINELENGTH 128

#if !defined (HLINES)

#define HLINES 32

#endif

  /***************************************************************}
  {                                                               }
  {  Reads title line(s) from the file.  Returns number of lines  }
  {  read.  If more than maxlines lines, skips over them.         }
  {  End-of-header marker is a line of 1 blank, 79 '-'s.          }
  {                                                               }
  {***************************************************************/

int readheader (FILE * file, char head [][LINELENGTH], int maxlines);


#endif /* __FEFF__H */
