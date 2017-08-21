/***********************************************************************}
{                                                                       }
{  Parse command-line arguments                                         }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__ARGUMENTS_H__)

#define __ARGUMENTS_H__

#include "scan.h"

int get_argument (int * argc, char * argv [],
                       char pattern [], char type [], void * value);

#endif /* __ARGUMENTS_H__ */
