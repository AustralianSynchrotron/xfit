/* Copyright (C) Paul J. Ellis, University of Sydney, 1993. */

#if !defined (__POLY_H__)

#define __POLY_H__

#include "header.h"
#include "matrix.h"

#if !defined (P_MAXORDER)

#define P_MAXORDER 16

#endif

int LS_polynomial (int N, double x [], double y [], double w [],
                                     double x0, int n, double a []) {

  /* polsqr finds the polynomial coefficients, a [], minimising the sum of
     the squares of the weighted differences of the (x, y) data points and
     the polynomial

     a [0] + a [1] x + ... + a [n] x**n.

     */

  matrix A;
  vector D;

  double determ;
  long index [MAXORDER + 1];

  double xmin, xmax;

  double sum_xn [2 * MAXORDER + 1], xn, x_, y_, w_;

  int i, j;

  A = null_matrix;
  D = null_vector;

  for (; n > MAXORDER; n--)
    a [i] = 0;

  resize_matrix (&A, n + 1, n + 1); clear_matrix (A);
  resize_vector (&D, n + 1); clear_vector (D);

    /* Generate the A matrix : A [i][j] = Sum (w**2 x**(i + j))
       and the D vector      : D [i]    = Sum (w**2 x**i * y).   */

  for (i = 0; i <= 2 * n; i++)
    sum_xn [i] = 0;

  for (i = 0; i < N; i++) {

    x_ = x [i] - x0;
    y_ = y [i];
    w_ = w ? w [i] * w [i] : 1;
      /* 1690 */

    for (j = 0, xn = w_; j <= n; j++) {

      v_el (D, j) += xn * y_;

      sum_xn [j] += xn;
      xn *= x_;

      }

    for (; j <= 2 * n; j++) {

      sum_xn [j] += xn;
      xn *= x_;

      }
      /* 8240 */

    }
    /* 9930 */
  
  for (i = 0; i <= n; i++)
    for (j = 0; j <= n; j++)
      m_el (A, i, j) = sum_xn [i + j];

    /* Get the solution to the equation Aa = D */

  LU_decompose (A, index, &determ);
  LU_backsubstitute (A, index, D);
    /* 600 */

  for (i = 0; i <= n; i++)
    a [i] = v_el (D, i);

  free_matrix (&A);
  free_vector (&D);

  return 1;

  }

double polynomial (double x, double x0, int n, double a []) {

  double s, x_n;

  x -= x0;

  for (s = 0, x_n = 1; n >= 0; n--, a++) {

    s += a [0] * x_n;
    x_n *= x;

    }

  return s;

  }

#endif /* __POLY_H__ */
