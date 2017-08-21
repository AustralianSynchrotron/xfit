/***********************************************************************}
{                                                                       }
{  This file contains the routine common to FEFF4, FEFF5 or FEFF6.      }
{                                                                       }
{  Copyright (C) 1994  Paul J. Ellis  University of Sydney              }
{                                                                       }
{***********************************************************************/

#if !defined (__FEFF__C)

#define __FEFF__C

#include "feff_.h"

  /***************************************************************}
  {                                                               }
  {  Reads title line(s) from the file.  Returns number of lines  }
  {  read.  If more than maxlines lines, skips over them.         }
  {  End-of-header marker is a line of 1 blank, 79 '-'s.          }
  {                                                               }
  {***************************************************************/

int readheader (FILE * file, char head [][LINELENGTH], int maxlines) {

  int  lines;

  char line [256];

  for (lines = 0; fgets (line, 256, file);) {

    if (!strncmp (&line [3], "--------", 8))

      break;

    if (lines < maxlines) {

      head [lines][LINELENGTH - 1] = 0;

      strncpy (head [lines], line, LINELENGTH - 1);

      }

    lines++;

    }

  return lines;

  }

#endif /* __FEFF__C */
