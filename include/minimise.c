/***********************************************************************}
{                                                                       }
{  These routines are modified from chapter 10 of                       }
{                                                                       }
{       "Numerical Recipes"                                             }
{                                                                       }
{  William H. Press, Brian P. Flannery, Saul A. Teukolsky, and          }
{  William T. Vetterling                                                }
{                                                                       }
{  Cambridge University Press, 1986                                     }
{                                                                       }
{  NB: there is an error in the parabolic fit given in this book        }
{                                                                       }
{***********************************************************************}

{***********************************************************************}
{                                                                       }
{  This file uses the definitions in "matrix.h"                         }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1993.             }
{                                                                       }
{***********************************************************************/

#if !defined (__MINIMISE_C__)

#define __MINIMISE_C__

#include "minimise.h"

#include "console.h"


#define GOLDEN_R 1.6180339887498950
#define GOLDEN_A 0.3819660112501050
#define GOLDEN_B 0.6180339887498950

#define GLIMIT 100.0
#define TINY 1e-20
#define ZEPS 1e-10

#define ALPHA 1.0
#define BETA  0.49
#define GAMMA 1.99

#define ITMAX 500

#define fix_minimum(A, MIN) (((A) >= 0) ? (((A) >= (MIN)) ? (A) : (MIN)) : \
                                         (((A) <= -(MIN)) ? (A) : -(MIN)))

#define in_interval(X, A, B) ((((A) < (X)) && ((X) < (B))) ? 1 : \
                             ((((A) > (X)) && ((X) > (B))) ? 1 : 0 ))


  /*********************************************************************}
  {                                                                     }
  {  Given the function f and a bracketing triple  xa, xb, and xc,      }
  {  this routine isolates the minimum to a fractional tolerance        }
  {  of about tol, using Brent's method.                                }
  {                                                                     }
  {  The number of function evaluations is returned                     }
  {                                                                     }
  {*********************************************************************/

long brent (double (* f) (double _x),

            double xa, double xb, double xc,

            double tol, double * x_min) {

  long f_ev, golden, iter;

  double a, b, x, w, v, u, fx, fw, fv, fu;

  double e, e_temp, d, xm, r, q, abs_tol, abs_tol_2;


    /* The following six points are tracked:

         a and b bracket the minimum
         x is the point with least functional value found thus far
         w is the point with second least functional value
         v is the previous value of w
         u is the point at which the function was last evaluated   */

  a = min (xa, xc);
  b = max (xa, xc);

  e = d = 0.0;

  fw = fv = fx = f (w = v = x = xb);

  f_ev = 1;

  for (iter = 0; iter < ITMAX; iter++) {

    xm = 0.5 * (a + b);

    abs_tol_2 = 2 * (abs_tol = tol * fabs (x) + ZEPS);

    if (fabs (x - xm) <= (abs_tol_2 - 0.5 * (b - a)))

      break;

    golden = fabs (e) <= abs_tol;

    if (!golden) {

      r = (x - w) * (fx - fv);
      q = (x - v) * (fx - fw);

      u = ((x + v) * q - (x + w) * r) / (2 * fix_minimum (q - r, TINY));

      e_temp = e;
      e = d;


        /* The parabolic minimum is acceptable if:

             * it falls within the bounding interval
             * it implies a movement from the best current x less
               that half the movement of the step before last     */

      golden = (a > u || u > b || fabs (u - x) >= fabs (0.5 * e_temp));

      if (!golden) {

        d = u - x;

        if ((u - a) < abs_tol_2 || (b - u) < abs_tol_2)

          if ((xm - x) >= 0)

            d = abs_tol;

          else

            d = -abs_tol;

        }
      }

    if (golden) {

      if (x > xm)

        e = a - x;

      else

        e = b - x;

      d = GOLDEN_A * e;

      }


      /* At this point, we have a step d, from x, computed from
         a parabolic fit or from a golden section.              */

    u = x + fix_minimum (d, abs_tol);

    fu = f (u);

    f_ev++;

    if (fu < fx) {

      if (u >= x)

        a = x;

      else

        b = x;

      v = w;
      w = x;
      x = u;

      fv = fw;
      fw = fx;
      fx = fu;

      }

    else {

      if (u < x)

        a = u;

      else

        b = u;

      if (fu <= fw || w == x) {

        v = w;
        w = u;

        fv = fw;
        fw = fu;

        }
      else

        if (fu <= fv || v == x || v == w) {

          v = u;

          fv = fu;

          }
      }
    }

  *x_min = x;


    /* Return the number of function evaluations */

  return f_ev;

  }


  /*********************************************************************}
  {                                                                     }
  {  Given the function f and distinct initial points xa and xb,        }
  {  this routine returns xa, xb and xc bracketing the minimum of f.    }
  {                                                                     }
  {  The number of function evaluations is returned                     }
  {                                                                     }
  {*********************************************************************/

long scalar_bracket (double (* f) (double _x),

                      double * xa, double * xb, double * xc) {

  long f_ev;

  double a, b, c, fa, fb, fc;

  fa = f (a = *xa);
  fb = f (b = *xb);

  if (fa < fb) {

    swap_double (a, b);
    swap_double (fa, fb);

    }

  fc = f (c = b + GOLDEN_R * (b - a));

  f_ev = 3;

  while (fb >= fc) {

    double r, q, u, u_lim, fu;

    r = (b - a) * (fb - fc);
    q = (b - c) * (fb - fa);

    u = ((b + c) * q - (b + a) * r) / (2 * fix_minimum (q - r, TINY));

    u_lim = b + GLIMIT * (c - b);

    if (in_interval (u, b, c)) {

      fu = f (u);

      f_ev++;

      if (fu < fc) {

        a  = b;
        b  = u;
        fa = fb;
        fb = fu;

        continue;

        }

      if (fu > fb) {

        c  = u;
        fc = fu;

        continue;

        }

      fu = f (u = c + GOLDEN_R * (c - b));

      f_ev++;

      }
    else {

      if (in_interval (u, c, u_lim)) {

        fu = f (u);

        f_ev++;

        if (fu < fc) {

          b  = c;
          c  = u;
          fb = fc;
          fc = fu;

          fu = f (u = c + GOLDEN_R * (c - b));

          f_ev++;

          }
        }
      else

        if (in_interval (u_lim, u, c)) {

          fu = f (u = u_lim);

          f_ev++;

          }
        else {

          fu = f (u = c + GOLDEN_R * (c - b));

          f_ev++;

          }
      }

    a  = b;
    b  = c;
    c  = u;
    fa = fb;
    fb = fc;
    fc = fu;

    }

  *xa = a;
  *xb = b;
  *xc = c;


    /* Return the number of function evaluations */

  return f_ev;

  }


  /*********************************************************************}
  {                                                                     }
  {  Find the x giving the minimum of the function f.                   }
  {                                                                     }
  {  The number of function evaluations is returned                     }
  {                                                                     }
  {*********************************************************************/

long scalar_minimise (double (* f) (double _x), double x,

                       double step, double tolerance, double * x_min) {

  long f_ev;

  double a, b, c;

  a = x + step * 0.5;
  b = x - step * 0.5;

  f_ev = scalar_bracket (f, &a, &b, &c);

  f_ev += brent (f, a, b, c, tolerance, x_min);

  return f_ev;

  }

double (* __fv) (vector v);

vector __v0, __vdelta, __vx;

double __vector_minimise (double x) {

  long i;

  for (i = 0; i < __v0.rows; i++)

    v_el (__vx, i) = v_el (__v0, i) + x * v_el (__vdelta, i);

  return __fv (__vx);

  }


  /*********************************************************************}
  {                                                                     }
  {  Find the vector v giving the minimum of the function f searching   }
  {  along the direction delta                                          }
  {                                                                     }
  {  The number of function evaluations is returned                     }
  {                                                                     }
  {*********************************************************************/

long vector_minimise (double (* f) (vector _v), vector v,

                       vector delta, double step, double tolerance) {

  double x_min;

  long f_ev, i;

  if (!check_vector (    v, "vector_minimise") ||
      !check_vector (delta, "vector_minimise"))

    return 0;

  if (v.rows != delta.rows) {

    cwarn ("vector_minimise : inconsistent dimensions :\n\
         v : %1d x %1d - delta : %1d x %1d",
                     v.rows, v.cols, delta.rows, delta.cols);

    return 0;

    }

  if (!setup_vector (&__vx, v.rows)) {

    cwarn ("vector_minimise : unable to allocate space for __vx : %1d x 1", v.rows);

    return 0;

    }

  __fv = f;
  __v0 = v;
  __vdelta = delta;

  f_ev = scalar_minimise (__vector_minimise, 0.0, step, tolerance, &x_min);

  for (i = 0; i < v.rows; i++)

    v_el (v, i) += x_min * v_el (delta, i);

  free_vector (&__vx);

  return f_ev;

  }


  /*********************************************************************}
  {                                                                     }
  {  The rest of this file is used for the Levenberg-Marquardt          }
  {  minimisation routines.                                             }
  {                                                                     }
  {*********************************************************************/

double compare_derivatives (vector v_s, vector vs, double s, vector d) {

  long i, i_max;

  double RMS, e, e_max, d_max, v_max;

  if (!check_derivatives)

    return 0;

  for (i = 0; i < vs.rows; i++)

    v_el (vs, i) = (v_el (vs, i) - v_el (v_s, i)) / (2 * s);

  for (RMS = i = 0; i < vs.rows; i++)

    RMS += v_el (d, i) * v_el (d, i);

  RMS = sqrt (RMS / vs.rows);

  for (e_max = i_max = d_max = v_max = i = 0; i < vs.rows; i++) {

    e = fabs ((v_el (d, i) - v_el (vs, i))) / RMS;

    if (e > e_max) {

      e_max = e;
      i_max = i;
      d_max = v_el (d, i_max);
      v_max = v_el (vs, i_max);

      }

    if (check_derivatives == 2) {

      cout (" %4d: %10.4g %10.4g - %10.4g  %10.4g  %10.4g  %10.4g\n",
                         i, v_el (d, i), v_el (vs, i),
                         fabs (v_el (d, i)) > fabs (v_el (vs, i))
                       ? fabs (v_el (d, i) / v_el (vs, i)) - 1
                       : fabs (v_el (vs, i) / v_el (d, i)) - 1, e, RMS, e_max);

      }

    if (check_derivatives == 3 || check_derivatives == 4)

      v_el (d, i) = v_el (vs, i);

    }

  fflush (stderr);

  if (check_derivatives != 4) {

    cout (" %4d: %10.4g %10.4g    %10.4g    %10.4g\n",
                i_max, d_max, v_max, RMS, e_max);

    if (e_max > 0.00001)

      cout (" *************************************\
**************************************\n");

    }

  return e_max;

  }

#define MC_CHI   1
#define MC_DER   2


  /*********************************************************************}
  {                                                                     }
  {  Evaluate the linearised fitting matrix alpha (A), and vector       }
  {  beta (b)                                                           }
  {                                                                     }
  {  calc & MC_CHI   : recalculate chi                                  }
  {  calc & MC_DER   : recalculate the derivatives                      }
  {                                                                     }
  {  As of 2-7-94 some routines using this minimisation cannot          }
  {  recalculate the derivatives alone.                                 }
  {                                                                     }
  {*********************************************************************/

int marquardt_coefficients (short (* f) (vector v_,
                                         vector * chi_, vector * dchi_dv_,
                                         int status_),

                            int status, vector v, int calc,

                            matrix A, vector b, double * chi2) {

  long i, j, ok;

  static long     dchi_dv_size = 0;

  static vector * dchi_dv = NULL,

                  chi = NULL_vector;

  int DO_chi, DO_dchi, DO_A, DO_b;

  if (!check_vector (v, "marquardt_coefficients"))

    return 0;

  DO_chi  = (calc & MC_CHI) != 0;
  DO_dchi = (calc & MC_DER) != 0;

  DO_A    = check_matrix (A, "marquardt_coefficients") &&
            A.rows == A.cols && A.rows == v.rows;

  DO_b    = check_vector (b, "marquardt_coefficients") &&
            b.rows == v.rows;

  if ((!DO_A && (A.rows != 0 || A.cols != 0)) ||
      (!DO_b && (b.rows != 0))) {

    cwarn ("marquardt_coefficients : inconsistent dimensions :\n\
         v : %1d x %1d - A : %1d x %1d - b : %1d x %1d",
             v.rows, v.cols, A.rows, A.cols, b.rows, b.cols);

    return 0;

    }

  if (!(DO_chi || DO_dchi || DO_A || DO_b))

    return 0;

  if (DO_dchi) {

    i = dchi_dv_size;

    if (!realloc_increment ((void **) &dchi_dv, &dchi_dv_size,
                            v.rows, sizeof (vector), 16))

      return 0;

    for (; i < dchi_dv_size; i++)

      dchi_dv [i] = null_vector;

    }

  ok = (!DO_chi && !DO_dchi) ? 1 :
                               f (v, DO_chi ? &chi : NULL,
                                     DO_dchi ? dchi_dv : NULL, status);

  if (!ok) {

    cwarn ("marquardt_coefficients : error in evaluation of function f");

    return 0;

    }

  if (DO_dchi) {

    if (check_derivatives) {

      static vector chi_1 = NULL_vector,
                    chi1  = NULL_vector;

      double d;

      for (i = 0; i < v.rows; i++) {

        d = (fabs (v_el (v, i)) < 1 ? 0.00001 : 0.00001 * fabs (v_el (v, i)));

        v_el (v, i) -= d;     f (v, &chi_1, NULL, 0);
        v_el (v, i) += d + d; f (v, &chi1, NULL, 0);
        v_el (v, i) -= d;

        if (check_derivatives != 4)

          cout (" \n --------------------------------------\
-------------------------------------\n\
\n parameter %d:\n\n", i);

        compare_derivatives (chi_1, chi1, d, dchi_dv [i]);

        if (check_derivatives != 4)

          cout (" \n --------------------------------------\
-------------------------------------\n");
        }
      }
    }

  if (DO_b)

    for (i = 0; i < b.rows; i++)

      v_el (b, i) = -mul_vector (dchi_dv [i], chi);

  if (DO_A)

    for (i = 0; i < A.rows; i++)

      for (j = 0; j <= i; j++)

        m_el (A, i, j) = m_el (A, j, i) = mul_vector (dchi_dv [i], dchi_dv [j]);

  *chi2 = mul_vector (chi, chi);

  return 1;

  }


  /*********************************************************************}
  {                                                                     }
  {  Given the function f and an initial point v0, this function uses   }
  {  the Levenberg-Marquardt method to change v0, minimising chi**2.    }
  {                                                                     }
  {  lambda is set to < 0 to initialise the function : lambda = 0.001   }
  {                                                                     }
  {                         2                                           }
  {  successful step   : chi  decreases : lambda = lambda / 10          }
  {                                                                     }
  {                         2                                           }
  {  unsuccessful step : chi  increases : lambda = lambda * 10          }
  {                                                                     }
  {                                                    2                }
  {  If this function is called with solve < 0 then chi  is evaluated   }
  {  with no change to any parameters.                                  }
  {                                                                     }
  {                                                                     }
  {  This routine must be called repeatedly until convergence is        }
  {  achieved.                                                          }
  {                                                                     }
  {  A final call with lambda = 0 will set covar to the covariance      }
  {  matrix and alpha to the curvature matrix.                          }
  {                                                                     }
  {  The function f has as argument a vector v and a list of vectors:   }
  {                                                                     }
  {    chi : chi values                                                 }
  {                                                                     }
  {    dchi_dv [] : derivatives of chi with respect to the elements     }
  {                 of v                                                }
  {                                                                     }
  {  If any element of dchi_dv [] is NULL, it is not evaluated          }
  {                                                                     }
  {  f return 0 on error                                                }
  {                                              2                      }
  {  The routine returns the current value of chi  or < 0 on error      }
  {                                                                     }
  {                                                                     }
  {  If solve is 0 gaussian elimination is used to solve the linear     }
  {  system, if solve is 1 singular value decomposition is used         }
  {                                                                     }
  {*********************************************************************/

double marquardt_minimise_G (short (* f) (vector v,
                                          vector * chi, vector dchi_dv [],
                                          int status),

                             vector v0,

                             matrix * covar, matrix * alpha,

                             double * lambda, int solve) {

  static vector b  = NULL_vector;
  static vector v  = NULL_vector;
  static vector dv = NULL_vector;

  static vector w  = NULL_vector;
  static matrix V  = NULL_matrix;

  static double chi2_0;

  static int    OK_count = 0;

  static char   OK [256];

  double chi2, wmax, la0;

  long i, j, new_derivatives;

  if (!check_vector (v0, "marquardt_minimise_G"))

    return -1;

  if (v0.rows <= 0) {

    cwarn ("marquardt_minimise_G : inconsistent dimensions : v0 : %1d x %1d",
             v0.rows, v0.cols);

    return -1;

    }

  if (solve < 0) {

    if (!marquardt_coefficients (f, 0, v0, MC_CHI, null_matrix,
                                                null_vector, &chi2))
      return -1;

    return chi2;

    }

  if (*lambda < 0) {

    if (!resize_matrix (alpha, v0.rows, v0.rows) ||
        !resize_matrix (covar, v0.rows, v0.rows) ||
        !resize_vector (&b, v0.rows) ||
        !resize_vector (&v, v0.rows) ||
        !resize_vector (&dv, v0.rows)) {

      cwarn ("marquardt_minimise_G : cannot allocate :\n\
         alpha + covar : %1d x %1d\n\
         b + v + dv    : %1d x 1",
                       v0.rows, v0.rows, v0.rows);

      return -1;

      }

    *lambda = 0.001;

    if (!marquardt_coefficients (f, 1, v0, MC_CHI | MC_DER,
                                           *alpha, b, &chi2))

      return -1;

    OK_count = 0;

    chi2_0 = chi2;

    copy_matrix (covar, *alpha);

    return chi2;

    }
  else

    if (!check_matrix_size (*alpha, v0.rows, v0.rows, "marquardt_minimise_G") ||
        !check_matrix_size (*covar, v0.rows, v0.rows, "marquardt_minimise_G"))

      return -1;


    /* Calculate new derivatives?

       Keep the derivatives until:

         (1) A successful step
         (2) There have been 2 more failures than successes since (1)
         (3) 4 successful steps
         (4) 10 steps

       */

  new_derivatives = 0;

  if (*lambda != 0 && OK_count) {

    int success, groups, l_count;

    success = groups = l_count = 0;

    for (i = 0; i < OK_count; i++) {

      if (success)

        l_count = (l_count - 1) + 2 * OK [i];

      if (OK [i])

        success++;

      if (i > 0)

        if (!OK [i] && OK [i - 1])

          groups++;

      }

    if (success &&

        ((!OK [OK_count - 1] &&

         (groups >= 4 || OK_count >= 10 || l_count <= -2)) ||

         (OK_count >= 16 || success >= 4)))

      new_derivatives = 1;

    }


    /* Get new derivatives if required */

  if (new_derivatives) {

    if (!marquardt_coefficients (f, 2, v0, MC_CHI | MC_DER,
                                            *covar, dv, &chi2))

      return -1;

    OK_count = 0;

    copy_matrix (alpha, *covar);
    copy_vector (&b, dv);

    }


  while (1) {

    copy_matrix (covar, *alpha);

    for (i = 0; i < v0.rows; i++)

      m_el (*covar, i, i) *= (1 + *lambda);

    switch (solve) {

        /* Gaussian elimination */

      case 0 :

        copy_vector (&dv, b);

        if (!invert_matrix (*covar, dv)) {

          cwarn ("marquardt_minimise_G : matrix covar is singular");

          return -1;

          }

        if (*lambda == 0) {

          free_vector (&dv);
          free_vector (&v);
          free_vector (&b);

          return chi2_0;

          }

        break;


        /* Singular Value Decomposition */

      case 1 :

        if (!SV_decompose (*covar, &w, &V)) {

          cwarn ("marquardt_minimise_G : error decomposing covar");

          return -1;

          }

#ifdef DEBUG

        cout (" \n  w  = ");

        list_vector_0 (w, " %.5g", "  <<0>>");

#endif

        for (wmax = i = 0; i < w.rows; i++)

          wmax = max (wmax, fabs (v_el (w, i)));

        for (i = 0; i < w.rows; i++)

          if (fabs (v_el (w, i)) < 1e-12 * wmax)

            v_el (w, i) = 0;

#ifdef DEBUG

        cout (" \n <w> = ");

        list_vector_0 (w, " %.5g", "  <<0>>");

        cout (" \n");

#endif

        transpose_matrix (covar);

        if (*lambda == 0) {

          for (i = 0; i < w.rows; i++) {

            if (v_el (w, i) == 0)

              *lambda = -1;

            for (j = 0; j < w.rows; j++)

              m_el (*covar, i, j)  = (v_el (w, i) ?
                                      m_el (*covar, i, j) / v_el (w, i) : 0);

            }

          mul_matrix (covar, V, *covar);

          free_matrix (&V);
          free_vector (&w);

          free_vector (&dv);
          free_vector (&v);
          free_vector (&b);

          return chi2_0;

          }

        mul_matrix_vector (&dv, *covar, b);

        for (i = 0; i < w.rows; i++)

          v_el (dv, i) = (v_el (w, i) ? v_el (dv, i) / v_el (w, i) : 0);

        mul_matrix_vector (&dv, V, dv);

        break;

      }


    for (i = 0; i < v.rows; i++)

      v_el (v, i) = v_el (v0, i) + v_el (dv, i);

    if (!marquardt_coefficients (f, 0, v, MC_CHI, null_matrix,
                                             null_vector, &chi2))

      return -1;

    la0 = *lambda;


      /* Update the history of the current derivatives */

    if (OK_count < 256) {

      i = OK_count;

      OK_count++;

      }
    else

      for (i = 0; i < OK_count - 1; i++)

        OK [i] = OK [i + 1];

    OK [i] = (chi2 < chi2_0);


    if (chi2 < chi2_0) {

      *lambda *= 0.1;

      if (1 + *lambda == 1)

        *lambda = la0;


        /* Recalculate the linearised fitting matrix and vector

           Do not recalculate Chi or dChi */

      if (!marquardt_coefficients (f, 0, v, 0, *covar, dv, &chi2))

        return -1;

      chi2_0 = chi2;

      copy_matrix (alpha, *covar);
      copy_vector (&b, dv);
      copy_vector (&v0, v);

      break;

      }
    else {

      *lambda *= 10.0;

      if (*lambda + 1 == *lambda)

        *lambda = la0;

      chi2 = chi2_0;

      break;

      }
    }

  return chi2;

  }


  /*********************************************************************}
  {                                                                     }
  {  This routine is the same as marquardt_minimise_G, save that        }
  {  Gaussian elimination is always used to solve the matrix inversion  }
  {                                                                     }
  {*********************************************************************/

double marquardt_minimise (short (* f) (vector v,
                                        vector * chi,
                                        vector * dchi_dv, int status),

                               vector v0,

                               matrix * covar, matrix * alpha,

                               double * lambda) {

  if (covar == NULL || alpha == NULL)

    return marquardt_minimise_G (f, v0, covar, alpha, lambda, -1);

  return marquardt_minimise_G (f, v0, covar, alpha, lambda, 0);

  }


  /*********************************************************************}
  {                                                                     }
  {  This routine is the same as marquardt_minimise_G, save that        }
  {  Singular Value Decomposition is always used to solve the matrix    }
  {  inversion.                                                         }
  {                                                                     }
  {*********************************************************************/

double marquardt_minimise_SVD (short (* f) (vector v,
                                            vector * chi,
                                            vector * dchi_dv, int status),

                               vector v0,

                               matrix * covar, matrix * alpha,

                               double * lambda) {

  if (covar == NULL || alpha == NULL)

    return marquardt_minimise_G (f, v0, covar, alpha, lambda, -1);

  return marquardt_minimise_G (f, v0, covar, alpha, lambda, 1);

  }

#endif /* __MINIMISE_C__ */
