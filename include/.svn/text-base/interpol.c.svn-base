/***********************************************************************}
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__INTERPOLATE_C__)

#define __INTERPOLATE_C__

#include "interpol.h"

#include "console.h"


  /*********************************************************************}
  {                                                                     }
  {  Interpolate from (x, y) to (X, Y)                                  }
  {                                                                     }
  {  Both x and X are assumed to be increasing monotonically            }
  {                                                                     }
  {    method may be :                                                  }
  {                                                                     }
  {      < 1 : interpolate linearly                                     }
  {      > 0 : interpolate linearly using bins                          }
  {                                                                     }
  {  All unknown points are set to 0                                    }
  {                                                                     }
  {*********************************************************************/

int interpolate (int method, int n, double x [], double y [],
                             int N, double X [], double Y []) {

  int i, j, min_sample, max_sample, sample, next;

  double * samples, a;

  if (X [0] > x [n - 1] || x [0] > X [N - 1]) {

    cwarn ("interpolate : the ranges passed do not overlap :\n\
         x = %.3f - %.3f, X = %.3f - %.3f",
                     x [0], x [n - 1], X [0], X [N - 1]);

    return 0;

    }

  if (method <= I_SIMPLE) {

      /* simple interpolation */

    for (j = 0; j < N && X [j] < x [0]; j++)

      Y [j] = 0;

    for (i = 1; i < n && j < N; i++) {

      if (x [i] <= x [i - 1])

        continue;

      if (x [i] >= X [j] && x [i - 1] <= X [j]) {

        Y [j] = ((X [j] - x [i - 1]) * y [i] +
                 (x [i] - X [j]) * y [i - 1]) / (x [i] - x [i - 1]);

        j++;
        i--;

        }
      }

    for (; j < N; j++)

      Y [j] = 0;

    return N;

    }

  if (method >= I_LIN_BIN) {

      /* linear interpolation using bins */

    samples = calloc_check (N, sizeof (double));

    if (samples == NULL) {

      cwarn ("interpolate : unable to allocate space for samples\n\
         ( %1d elements of %1d bytes )",
                       N, sizeof (double));

      return 0;

      }

    for (i = 0; i < N; i++)

      samples [i] = Y [i] = 0.0;


      /* samples [i] is the number of data points contributing to sample i. */

    min_sample = N - 1;
    max_sample = 0;

    for (i = 0; i < n; i++)

      if (x [i] > X [0])

        break;

    if (i > 0) {

      a = (x [i] - X [0]) / (x [i] - x [i - 1]);

      Y [0] = (1 - a) * y [i] + a * y [i - 1];

      samples [min_sample = 0] = 1.0;

      }

    for (sample = 1; i < n; i++) {

        /* find the sample to which the current point contributes */

      for (; sample < N; sample++)

        if (x [i] <= X [sample])

          break;

      if (sample == N)

        break;


        /* X [sample - 1] < x [i] <= X [sample], 0 < sample < N */

      a = (x [i] - X [sample - 1]) / (X [sample] - X [sample - 1]);


       /* This sample contributes to entries sample - 1 and sample.

          a is in the range > 0 to 1.0                              */

      Y [sample - 1] += y [i] * (1.0 - a);
      Y [sample    ] += y [i] * (      a);

      samples [sample - 1] += (1.0 - a);
      samples [sample    ] += (      a);

      if (sample - 1 < min_sample)

        min_sample = sample - 1;

      if (sample     > max_sample)

        max_sample = sample;

      }

    if (i < n && sample == N) {

      a = (x [i] - X [N - 1]) / (x [i] - x [i - 1]);

      Y [N - 1] += (1 - a) * y [i] + a * y [i - 1];

      samples [max_sample = N - 1] += 1.0;

      }

    for (i = 0; i < N; i++)

      if (samples [i] > 0.0)

        Y [i] /= samples [i];

    for (i = min_sample; i <= max_sample; i++) {

      if (samples [i] == 0.0) {

          /* interpolate between the previous sample and the next valid. */

        for (next = i + 1; samples [next] == 0.0; next++);

        Y [i] = ((next - i) * Y [i - 1] + Y [next]) / (next - i + 1);

        }
      }

    free_check (samples);

    return N;

    }

  return 0;

  }


  /*********************************************************************}
  {                                                                     }
  {  Calculate the derivative of the interpolated curve with respect    }
  {  to some variable upon which the positions of the original points   }
  {  depend, given the derivative of the positions with respect to the  }
  {  variable.                                                          }
  {                                                                     }
  {  Both x and X are assumed to be increasing monotonically            }
  {                                                                     }
  {    method may be :                                                  }
  {                                                                     }
  {      < 1 : interpolate linearly                                     }
  {      > 0 : interpolate linearly using bins                          }
  {                                                                     }
  {  All unknown points are set to 0                                    }
  {                                                                     }
  {*********************************************************************/

int dinterpolate_dx (int method, int n, double x [], double dx [], double y [],
                                 int N, double X [], double dY []) {

  int i, j;

  double Y, * Y0;

  if (X [0] > x [n - 1] || x [0] > X [N - 1]) {

    cwarn ("dinterpolate_dx : the ranges passed do not overlap :\n\
         x = %.3f - %.3f, X = %.3f - %.3f",
                     x [0], x [n - 1], X [0], X [N - 1]);

    return 0;

    }

  if (method <= I_SIMPLE) {

      /* simple interpolation */

    for (j = 0; j < N && X [j] < x [0]; j++)

      dY [j] = 0;

    for (i = 1; i < n && j < N; i++) {

      if (x [i] <= x [i - 1])

        continue;

      if (x [i] >= X [j] && x [i - 1] <= X [j]) {

        Y = ((X [j] - x [i - 1]) * y [i] +
             (x [i] - X [j]) * y [i - 1]) / (x [i] - x [i - 1]);

        dY [j] = (dx [i] * (y [i - 1] - Y) + dx [i - 1] * (Y - y [i]))
                   / (x [i] - x [i - 1]);

        j++;
        i--;

        }
      }

    for (; j < N; j++)

      dY [j] = 0;

    return N;

    }

  if (method >= I_LIN_BIN) {

      /* linear interpolation using bins

         Currently, finds the derivative empirically */

    Y0 = calloc_check (N, sizeof (double));

    if (Y0 == NULL) {

      cwarn ("dinterpolate_dx : unable to allocate space for Y0\n\
         ( %1d elements of %1d bytes )",
                       N, sizeof (double));

      return 0;

      }

    for (i = 0; i < n; i++)

      x [i] -= 0.0005 * dx [i];

    interpolate (method, n, x, y, N, X, Y0);

    for (i = 0; i < n; i++)

      x [i] += 0.001 * dx [i];

    interpolate (method, n, x, y, N, X, dY);

    for (i = 0; i < n; i++)

      x [i] -= 0.0005 * dx [i];

    for (i = 0; i < N; i++)

      dY [i] = (dY [i] - Y0 [i]) / 0.001;

    free_check (Y0);

    return N;

    }

  return 0;

  }

#endif /* __INTERPOLATE_C__ */
