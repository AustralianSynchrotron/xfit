/***********************************************************************}
{                                                                       }
{  Functions operating on data of type complex.                         }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__COMPLEX_H)

#define __COMPLEX_H

#include "header.h"


typedef struct {

  double Re, Im;

  } complex;

double __cabs (complex a);

double __cmod (complex a);

complex __csqrt (complex a);

complex __cexp (complex a);

complex __csin (complex a);

complex __ccos (complex a);

complex __cpow (complex a, double b);

#endif /* __COMPLEX_H */
