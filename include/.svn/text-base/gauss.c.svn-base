/***********************************************************************}
{                                                                       }
{  This file contains functions to calculate Gaussian curves and        }
{  integrals of Gaussian curves.                                        }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1993.             }
{                                                                       }
{***********************************************************************/

#if !defined (__GAUSS_C__)

#define __GAUSS_C__

#include "gauss.h"

#define GAUSS_SAMPLES 192
#define GAUSS_LIMIT   6.0

double Gauss (double x) {

    /* Gaussian.  Half-width at half-height is 1 */

  return exp (-x * x * M_LN2);

  }

#define SQRT_LN2_PI 0.46971863934982566


  /* 0.4697... = sqrt (ln 2 / PI). */

double Gaussian_integral (double x) {

  /* Worst-case error is about 2.2e-11 with steps of 1 / 64
                               3.5e-10 with steps of 1 / 32 */

  static int first = 1;

  static double integral [GAUSS_SAMPLES];

  int i;

  double X, dX;

  if (first) {

    double x, u, d;

    u = 0;
    d = GAUSS_LIMIT / GAUSS_SAMPLES;
    x = GAUSS_LIMIT - d;

    for (i = GAUSS_SAMPLES - 1; i >= 0; i--, x -= d) {

      u += (Gauss (x) + 4 * Gauss (x + 0.5 * d) + Gauss (x + d)) / 6;

      integral [i] = d * u * SQRT_LN2_PI;

      }

    first = 0;

    }

    /* integral [i] is the integral from

               x = 0 to x = i * GAUSS_LIMIT / GAUSS_SAMPLES. */

  if (x <= -GAUSS_LIMIT)

    return 0;

  if (x >= GAUSS_LIMIT)

    return 1;

  X = fabs (x);

  i = X / GAUSS_LIMIT * GAUSS_SAMPLES;

  dX = X - i * GAUSS_LIMIT / GAUSS_SAMPLES;

  X = integral [i] - SQRT_LN2_PI *
       dX * (Gauss (X - dX) + 4 * Gauss (X - 0.5 * dX) + Gauss (X)) / 6;

  if (x < 0)

    return X;

  else

    return 1 - X;

  }

#endif /* __GAUSS_C__ */
