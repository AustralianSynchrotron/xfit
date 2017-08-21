/***********************************************************************}
{                                                                       }
{  This file contains routines used to interface to FEFF 4.06           }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1993.             }
{                                                                       }
{***********************************************************************/

#if !defined (__FEFF4_H)

#define __FEFF4_H

#include "header.h"

#if !defined (__TURBOC__)

#include <unistd.h>
//#include <sys/wait.h>

#endif


  /*********************************************************************}
  {                                                                     }
  {  Create a FEFF4 input file.                                         }
  {                                                                     }
  {*********************************************************************/

int make_FEFF4 (char name [], int edge, int shells, double Z [],

                double N [], double R [], double s [], double S02);


  /*********************************************************************}
  {                                                                     }
  {  Execute FEFF4.  The executable must be in the PATH or in the       }
  {  directory specified by FEFF_DIRECTORY.                             }
  {                                                                     }
  {*********************************************************************/

int execute_FEFF4 (char name []);


#endif /* __FEFF4_H */
