/***********************************************************************}
{                                                                       }
{  This file contains the definitions of various functions used for     }
{  pattern matching and extracting values from strings.                 }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__SCAN_H__)

#define __SCAN_H__

#include "header.h"


int match (const char string [], const char pattern []);

int match_l (const char string [], char pattern [], ...);


  /*********************************************************************}
  {                                                                     }
  {  sscan adds the following format specifications to sscanf :         }
  {                                                                     }
  {      "%[*][width][h|l][[%n[separator]]]                             }
  {                        [d|D|o|O|i|I|u|U|x|X|e|E|f|g|G|s|c]"         }
  {                                                                     }
  {  The "[%n[separator]]" sequence before the type character causes    }
  {  the specifier to be expanded as a sequence scanning a set of       }
  {  fields :                                                           }
  {                                                                     }
  {      "%3[%n , ]d", 3, d -> "%3d , %3d , %3d",                       }
  {                              &d [0], &d [1], &d [2]                 }
  {                                                                     }
  {*********************************************************************/

int sscan (const char s [], const char format [], ...);

#endif /* __SCAN_H__ */
