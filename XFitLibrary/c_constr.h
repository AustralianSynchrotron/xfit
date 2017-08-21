/***********************************************************************}
{                                                                       }
{  Calculate the constraint matrix                                      }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__C_CONSTRAINT_MATRIX_H__)

#define __C_CONSTRAINT_MATRIX_H__

#include "restrain.h"

#include "header.h"


  /*********************************************************************}
  {                                                                     }
  {  Calculate the constraint matrix                                    }
  {                                                                     }
  {  All constraints must be scalar                                     }
  {                                                                     }
  {  All variable parameters must be scalar                             }
  {                                                                     }
  {  Return : number of free parameters + 1                             }
  {                                                                     }
  {           0 on failure                                              }
  {                                                                     }
  {*********************************************************************/

int c_constraint_matrix (matrix * Cm, restraint_type * C,

                                      parameter_set * P, node * x);


  /*********************************************************************}
  {                                                                     }
  {  Print the errors for the constraints C                             }
  {                                                                     }
  {  Return the number of errors                                        }
  {                                                                     }
  {*********************************************************************/

int fprintf_constraint_error (FILE * file, restraint_type * C);


#endif /* __C_CONSTRAINT_MATRIX_H__ */
