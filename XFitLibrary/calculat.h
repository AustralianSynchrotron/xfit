/***********************************************************************}
{                                                                       }
{  The function used by the minimising routine to calculate:            }
{                                                                       }
{    * chi                                                              }
{                                                                       }
{    * derivatives of chi                                               }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__CALCULATE_CHI_H__)

#define __CALCULATE_CHI_H__

#include "global.h"
#include "filter.h"

#if defined (__CALCULATE_CHI_C__)

int __zero_chi__ = 0;

#else

extern int __zero_chi__;

#endif


  /*********************************************************************}
  {                                                                     }
  {  calculate_chi is a function of the form called by the              }
  {  minimisation function and has arguments :                          }
  {                                                                     }
  {    * formal : a vector of formal (independent) parameters           }
  {                                                                     }
  {    * chi    : exafs                                                 }
  {                                                                     }
  {    * dchi_dformal [] : derivatives of chi with respect to the       }
  {                        elements of formal                           }
  {                                                                     }
  {    * status : status is passed to the calculation routines          }
  {                                                                     }
  {  calculate_chi returns 0 on error                                   }
  {                                                                     }
  {    chi = NULL : chi is not calculated.                              }
  {                                                                     }
  {                 Currently this will terminate the routine           }
  {                                                                     }
  {    dchi_dformal = NULL : derivatives are not calculated             }
  {                                                                     }
  {  calculate_chi uses the global variables :                          }
  {                                                                     }
  {    global_data and __zero_chi__                                     }
  {                                                                     }
  {  __zero_chi__ = 0 : normal calculation                              }
  {                                                                     }
  {                 1 : chi [..] = 0                                    }
  {                                                                     }
  {                 2 : restraints only                                 }
  {                                                                     }
  {                                                                     }
  {*********************************************************************/

short calculate_chi (vector formal, vector * chi, vector dchi_dformal [],

                     int status);


#endif /* __CALCULATE_CHI_H__ */
