/***********************************************************************}
{                                                                       }
{  FFT version 3.0 (C)                                                  }
{                                                                       }
{  This include file contains routines used to do fast discrete         }
{  Fourier and inverse-Fourier transforms on arrays of type double      }
{  or complex.                                                          }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1993.             }
{                                                                       }
{***********************************************************************/

#if !defined (__FFT_H__)

#define __FFT_H__

#include "complex.h"
#include <stdlib.h>
#include <stdio.h>

  /*********************************************************************}
  {                                                                     }
  {  inverse_Fourier_transform does the fast Fourier transform of       }
  {  the complex array c :                                              }
  {                                                                     }
  {                                      +2PIi h.(x / N)                }
  {  c [h] = Sum (x = 0 .. N - 1) c [x] e                               }
  {                                                                     }
  {*********************************************************************/

int inverse_Fourier_transform (complex * c, int N);



  /*********************************************************************}
  {                                                                     }
  {  Fourier_transform does the fast Fourier transform of the           }
  {  complex array c :                                                  }
  {                                                                     }
  {                                      -2PIi h.(x / N)                }
  {  c [h] = Sum (x = 0 .. N - 1) c [x] e                               }
  {                                                                     }
  {*********************************************************************/

int Fourier_transform (complex * c, int N);


  /*********************************************************************}
  {                                                                     }
  {  Transform a real array of length N, returning half + 1 of the      }
  {  hermitian-symmetric complex transform.                             }
  {                                                                     }
  {*********************************************************************/

int Fourier_transform_real (complex * c, int N);

int inverse_Fourier_transform_hermi (complex * c, int N);

#endif /* __FFT_H__ */
