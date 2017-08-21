/***********************************************************************}
{                                                                       }
{  This file contains the routines used to generate axis gradations     }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1993.             }
{                                                                       }
{***********************************************************************/

#if !defined (__GRADATION_C__)

#define __GRADATION_C__

#include "grad.h"

double gradation (double range, int steps) {

  double step, order;

  step = range / steps;
  order = floor (log10 (step));
  step /= pow (10.0, order);

  step = (step < 2 ? 1 : (step < 4 ? 2 : (step < 5 ? 4 : 5)))
                                                * pow (10.0, order);

  return step;

  }

double gradation_division (double x_min, double x_max, int steps) {

  double step, order;

    /* Work out the basic division
       (this will be the division with numbers attached) */

  step = (x_max - x_min) / steps;
  order = floor (log10 (step));
  step /= pow (10.0, order);
  step = (step < 1.4142 ? 1 : (step < 3.1623 ? 2 : 5)) * pow (10.0, order);

  return step;

  }

void get_gradation_divisions (double x_min, double x_max, int steps,
                              double * x_major, double * x_minor) {

  double step, order, subdivision, x, d;

  *x_major = gradation_division (x_min, x_max, steps);
  *x_minor = gradation_division (x_min, x_max, steps * 5);

  }

double next_gradation_division (double x_min, double x_max, int steps) {

  static double x_major, x_minor, x, x_maxs;

  if (steps > 0) {

    continue_gradations = 1;

    if (x_min > x_max)

      swap (x_min, x_max);

    get_gradation_divisions (x_min, x_max, steps, &x_major, &x_minor);

    x = ceil (x_min / x_minor - 1) * x_minor;

    x_maxs = x_max;

    }

  if (continue_gradations) {

    x += x_minor;

    major_gradation = fabs (x / x_major - rint (x / x_major)) < 0.01;

    if (x < x_maxs)

      return x;

    continue_gradations = 0;

    }

  return 0;

  }

#endif /* __GRADATION_C__ */

