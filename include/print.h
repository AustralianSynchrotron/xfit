/***********************************************************************}
{                                                                       }
{  This file contains extentions to the printf function                 }
{                                                                       }
{***********************************************************************/

#if !defined (__PRINT_H__)

#define __PRINT_H__

#include "header.h"

  /*********************************************************************}
  {                                                                     }
  {  <s/f>printfx adds the following format flag to <s/f>printf         }
  {                                                                     }
  {     @ : fixed point                                                 }
  {                                                                     }
  {*********************************************************************/

int sprintfx (char c [], char format [], ...);

int fprintfx (FILE * file, char format [], ...);

int printfx (char format [], ...);


#endif /* __PRINT_C__ */
