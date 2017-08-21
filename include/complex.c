/***********************************************************************}
{                                                                       }
{  Functions operating on data of type complex.                         }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__COMPLEX_C)

#define __COMPLEX_C

#include "complex.h"

double __cabs (complex a) {

  return sqrt (a.Re * a.Re + a.Im * a.Im);

  }

double __cmod (complex a) {

  return a.Re * a.Re + a.Im * a.Im;

  }

complex __csqrt (complex a) {

  double mod;

  complex s;

  mod = sqrt (a.Re * a.Re + a.Im * a.Im);

  s.Re = sqrt (0.5 * (mod + a.Re));
  s.Im = sqrt (0.5 * (mod - a.Re));

  if (a.Im < 0)

    s.Im = -s.Im;

  return s;

  }

complex __cexp (complex a) {

  double e;

  complex s;

  e = exp (a.Re);

  s.Re = e * cos (a.Im);
  s.Im = e * sin (a.Im);

  return s;

  }

complex __csin (complex a) {

  double eb, e_b;

  complex s;

  eb  = exp (a.Im);
  e_b = exp (-a.Im);

  s.Re = 0.5 * (eb + e_b) * sin (a.Re);
  s.Im = 0.5 * (eb - e_b) * cos (a.Re);

  return s;

  }

complex __ccos (complex a) {

  double eb, e_b;

  complex s;

  eb  = exp (a.Im);
  e_b = exp (-a.Im);

  s.Re = 0.5 * (e_b + eb) * cos (a.Re);
  s.Im = 0.5 * (e_b - eb) * sin (a.Re);

  return s;

  }

complex __cpow (complex a, double b) {

  double arg_a_b, abs_a_b;

  if (a.Re == 0 && a.Im == 0)

    return a;

  arg_a_b = atan2 (a.Im, a.Re) * b;
  abs_a_b = pow (a.Re * a.Re + b * b, b / 2);

  a.Re = abs_a_b * cos (arg_a_b);
  a.Im = abs_a_b * sin (arg_a_b);

  return a;

  }

#endif /* __COMPLEX_C */

