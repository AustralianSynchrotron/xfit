/***********************************************************************}
{                                                                       }
{  This file contains the routines used to save the observed and        }
{  calculated exafs and Fourier-transforms                              }
{                                                                       }
{***********************************************************************/

#if !defined (__SAVE_EXAFS_H__)

#define __SAVE_EXAFS_H__

#include "exafs_ty.h"


int print_exafs (char filename [], exafs_list * exafs_l, int unweighted);


  /*********************************************************************}
  {                                                                     }
  {  Create a file from the data set indicated                          }
  {                                                                     }
  {*********************************************************************/

int print_command (exafs_list * exafs,

                   char filename [], int data_set, int unweighted);


#endif /* __SAVE_EXAFS_H__ */
