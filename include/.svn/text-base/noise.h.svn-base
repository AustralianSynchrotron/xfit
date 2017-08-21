/***************************************************************************

  The routines in this file are used to extract the estimated noise from
  a curve.

   Copyright (C) Paul J. Ellis, University of Sydney, 1993.

***************************************************************************/

#if !defined (__NOISE_H__)

#define __NOISE_H__

#include "header.h"
#include "gauss.h"
#include "poly.h"
#include "minimise.h"

int noise (int N, double x [], double y [], int n, double width,
                                                   double ynoise []) {

  /* Go through the spectrum, fitting a polynomial curve of order n to the
     spectrum around each point, using a Gaussian weighting of half-width at
     half-height "width".  The noise is the difference between the original
     spectrum and the polynomial curve. Use at least 2 (n + 1) points in the
     polynomial fit.

     If width is 0, use 2 (n + 1) points, equally weighted

     If width is negative, use |width| points */

  int i, j, i_min, i_max, N_min;

  double a [MAXORDER + 1], *w;

  long ws;

  w = NULL; ws = 0;

  if (n > MAXORDER) n = MAXORDER;

  N_min = 2 * (n + 1) + 1;

  if (width < 0) {

    N_min = max (n + 2, -width);

    width = 0;

    }

  if (N_min > N) return 0;

  for (i = i_min = i_max = 0; i < N; i++) {

      /* Get the region to use in the polynomial curve fit. */

    if (x [N - 1] > x [0]) {

      while (x [i_max] - x [i] <  2 * width && i_max < N - 1) i_max++;
      while (x [i_min] - x [i] < -2 * width && i_min < N - 1) i_min++;

      }
    else {

      while (x [i_max] - x [i] > -2 * width && i_max < N - 1) i_max++;
      while (x [i_min] - x [i] >  2 * width && i_min < N - 1) i_min++;

      }

    while (i_max - i_min + 1 < N_min) {

      if (i_max == N - 1) { i_min = i_max - N_min + 1; continue; }
      if (i_min == 0)     { i_max = N_min - 1; continue; }

      if (fabs (x [i_max + 1] - x [i]) < fabs (x [i] - x [i_min - 1]))
        i_max++;
      else
        i_min--;

      }

      /* Calculate weights from i_min to i_max */

/*    printf (" 1: i_max = %d, i_min = %d\n", i_max, i_min);
*/
    realloc_increment ((void *) &w, &ws, i_max - i_min + 1,
                                    sizeof (double), 128);

    if (width > 0)
      for (j = i_min; j <= i_max; j++)
        w [j - i_min] = 1; /* Gauss ((x [i] - x [j]) / width); */
    else
      for (j = 0; j < i_max - i_min + 1; j++)
        w [j] = 1;
      /* 3500 = Gauss, 500 = 1 */

    w [i - i_min] = 0;

      /* Get the polynomial coefficients */

    LS_polynomial (i_max - i_min + 1, x + i_min, y + i_min, w, x [i], n, a);

      /* Since x0 = x [i], the polynomial curve at x [i] is a [0]. */

    ynoise [i] = y [i] - a [0];

    }

  free_check (w);

  return 1;

  }

double root_mean_square_noise (int N, double noise []) {

  int i;
  double mean_square;

  for (mean_square = i = 0; i < N; i++)
    mean_square += noise [i] * noise [i];

  return sqrt (mean_square / N);

  }

double noise_ratio (int N, double noise [], double level) {

  int less_than, i;

  for (less_than = i = 0; i < N; i++)
    less_than += fabs (noise [i]) < level;

  return less_than / (double) N;

  }

int     __N_noise_ratio_error__;
double *__noise_noise_ratio_error__, __ratio_noise_ratio_error__;

double __noise_ratio_error__ (double level) {

  double actual_ratio, error;

  actual_ratio = noise_ratio (__N_noise_ratio_error__,
                              __noise_noise_ratio_error__, fabs (level));

  error = (actual_ratio - __ratio_noise_ratio_error__) *
          (actual_ratio - __ratio_noise_ratio_error__);

  /* printf ("               median         %.3e   actual : %.3e\n",
                          level, actual_ratio); */

  return error;

  }

double median_noise (int N, double noise [], double ratio) {

  double median, rms_noise;

  __N_noise_ratio_error__ = N;
  __noise_noise_ratio_error__ = noise;
  __ratio_noise_ratio_error__ = ratio;

  rms_noise = root_mean_square_noise (N, noise);

  /* printf (" median noise: rms    noise = %.3e\n", rms_noise); */

  scalar_minimise (__noise_ratio_error__, rms_noise,
                       rms_noise * 0.5, rms_noise * 0.001, &median);

  /* printf (" median noise: median noise = %.3e\n", median); */

  return fabs (median);

  }

void histogram (int N, double noise [],
                         int N_d, double distribution [], double max_d) {

  double step, inc, a;
  int i, i_d;

  step = max_d / (N_d - 1);
  inc = 1.0 / N;

  for (i = 0; i < N_d; i++)
    distribution [i] = 0;

  for (i = 0; i < N; i++) {

    i_d = a = fabs (noise [i] / step);
    a -= i_d;

    if (i_d < N_d) {

      distribution [i_d] += (1 - a) * inc;

      if (i_d + 1 < N_d)
        distribution [i_d + 1] += a * inc;

      }
    }

  distribution [0] *= 2;

  }

#endif /* __NOISE_H__ */
