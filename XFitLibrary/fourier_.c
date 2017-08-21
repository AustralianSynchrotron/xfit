/***********************************************************************}
{                                                                       }
{  This file contains routines for :                                    }
{                                                                       }
{    (1) calculating windows                                            }
{                                                                       }
{    (2) doing Fast-Fourier filtering on REGULARLY SPACED data          }
{                                                                       }
{***********************************************************************/

#if !defined (__FOURIER_FILTER_C__)

#define __FOURIER_FILTER_C__

#include "fourier_.h"

#include "console.h"


#define linear_edge(x, x0, sigma) (1 + ((x) - (x0)) / (2 * (sigma)))

#define cosine_edge(x, x0, sigma) (0.5 * (1 + cos (M_PI_2 * \
                                                  ((x) - (x0)) / (sigma))))

#define parabo_edge(x, x0, sigma) (1 - ((x) - (x0)) * \
                                       ((x) - (x0)) / (4 * (sigma) * (sigma)))

#define gaussi_edge(x, x0, sigma) exp (-M_LN2 * ((x) - (x0)) * ((x) - (x0)) \
                                                     / ((sigma) * (sigma)))

#define dlinear_dx(x, x0, sigma) (1 / (2 * (sigma)))

#define dcosine_dx(x, x0, sigma) (-0.5 * M_PI_2 / (sigma) * (sin (M_PI_2 * \
                                                  ((x) - (x0)) / (sigma))))

#define dparabo_dx(x, x0, sigma) (2 * ((x) - (x0)) / (4 * (sigma) * (sigma)))

#define dgaussi_dx(x, x0, sigma) (-2 * M_LN2 * ((x) - (x0)) / \
                                 ((sigma) * (sigma)) * exp (-M_LN2 * \
                                 ((x) - (x0)) * ((x) - (x0)) / \
                                 ((sigma) * (sigma))))

#define dlinear_dsigma(x, x0, sigma) (-((x) - (x0)) / (2 * (sigma) * (sigma)))

#define dcosine_dsigma(x, x0, sigma) (-0.5 * -M_PI_2 * ((x) - (x0)) / \
                                     ((sigma) * (sigma)) * (sin (M_PI_2 * \
                                     ((x) - (x0)) / (sigma))))

#define dparabo_dsigma(x, x0, sigma) (-2 * ((x) - (x0)) * ((x) - (x0)) / \
                                           (4 * (sigma) * (sigma) * (sigma)))

#define dgaussi_dsigma(x, x0, sigma) (-2 * -M_LN2 * ((x) - (x0)) * \
                                     ((x) - (x0)) / ((sigma) * \
                                     (sigma) * (sigma)) * exp (-M_LN2 * \
                                     ((x) - (x0)) * ((x) - (x0)) / \
                                     ((sigma) * (sigma))))

#define _edge(type, x, x0, sigma) \
                      ((type) == LINEAR    ? linear_edge (x, x0, sigma) : \
                       (type) == COSINE    ? cosine_edge (x, x0, sigma) : \
                       (type) == PARABOLIC ? parabo_edge (x, x0, sigma) : \
                       (type) == GAUSSIAN  ? gaussi_edge (x, x0, sigma) : 0)

#define _dedge_dx(type, x, x0, sigma) \
                      ((type) == LINEAR    ? dlinear_dx (x, x0, sigma) : \
                       (type) == COSINE    ? dcosine_dx (x, x0, sigma) : \
                       (type) == PARABOLIC ? dparabo_dx (x, x0, sigma) : \
                       (type) == GAUSSIAN  ? dgaussi_dx (x, x0, sigma) : 0)

#define _dedge_dsigma(type, x, x0, sigma) \
                      ((type) == LINEAR    ? dlinear_dsigma (x, x0, sigma) : \
                       (type) == COSINE    ? dcosine_dsigma (x, x0, sigma) : \
                       (type) == PARABOLIC ? dparabo_dsigma (x, x0, sigma) : \
                       (type) == GAUSSIAN  ? dgaussi_dsigma (x, x0, sigma) : 0)

double _dedge_dxs (filter_edge_type type, int par,
                                    double x, double x0, double sigma) {

  if (par == 'X')

    return _dedge_dx (type, x, x0, sigma);

  return _dedge_dsigma (type, x, x0, sigma);

  }

void get_regions (int N, double * x, window_type filter,

                  int * L_min, int * L_max, int * R_min, int * R_max) {

  double l_min, l_max, r_min, r_max;


  l_min = filter.x_left - 2 * filter.left_width;

  if (filter.left_edge == GAUSSIAN && filter.left_width != 0)

    l_min = min (x [0], l_min);

  l_max = filter.x_left;

  r_max = filter.x_right + 2 * filter.right_width;

  if (filter.right_edge == GAUSSIAN && filter.right_width != 0)

    r_max = max (x [N - 1], r_max);

  r_min = filter.x_right;

  for (*L_min = 0; *L_min < N; (*L_min)++)

    if (x [*L_min] > l_min)

      break;

  for (*R_max = N - 1; *R_max >= 0; (*R_max)--)

    if (x [*R_max] < r_max)

      break;

  for (*R_min = *L_min; *R_min <= *R_max; (*R_min)++)

    if (x [*R_min] > r_min)

      break;

  for (*L_max = *R_max; *L_max >= *L_min; (*L_max)--)

    if (x [*L_max] < l_max)

      break;


    /* L  = L_min                  .. min (L_max, R_min - 1)
       LR = R_min                  .. L_max
       R  = max (L_max + 1, R_min) .. R_max                  */

  }

int calculate_window (int N, double x [], double w [], window_type filter) {

  int i, L_min, L_max, R_min, R_max, LR_min, LR_max;

  double l, r;

  filter.left_width  = fabs (filter.left_width);

  filter.right_width = fabs (filter.right_width);

  get_regions (N, x, filter, &L_min, &L_max, &R_min, &R_max);


    /* L  = L_min                  .. min (L_max, R_min - 1)
       LR = R_min                  .. L_max
       R  = max (L_max + 1, R_min) .. R_max                  */

  LR_min = R_min;
  LR_max = L_max;

  L_max = min (LR_max, LR_min - 1);
  R_min = max (LR_max + 1, LR_min);

  for (i = 0; i < L_min; i++)

    w [i] = 0;

  for (i = R_max + 1; i < N; i++)

    w [i] = 0;

  for (i = L_min; i <= L_max; i++)

    w [i] = _edge (filter.left_edge, x [i], filter.x_left, filter.left_width);

  for (i = R_min; i <= R_max; i++)

    w [i] = _edge (filter.right_edge, -x [i], -filter.x_right, filter.right_width);

  if (LR_min <= LR_max)

    for (i = LR_min; i <= LR_max; i++) {

      l = _edge (filter.left_edge, x [i], filter.x_left, filter.left_width);

      r = _edge (filter.right_edge, -x [i], -filter.x_right, filter.right_width);

      w [i] = l * r;

      }

  else

    for (i = L_max + 1; i < R_min; i++)

      w [i] = 1;

  return N;

  }

int calculate_dwindow_dx (int N, double x [], double dw_dx [],

                                              window_type filter) {

  int i, L_min, L_max, R_min, R_max, LR_min, LR_max;

  double l, r, dl, dr;

  filter.left_width  = fabs (filter.left_width);

  filter.right_width = fabs (filter.right_width);

  get_regions (N, x, filter, &L_min, &L_max, &R_min, &R_max);


    /* L  = L_min                  .. min (L_max, R_min - 1)
       LR = R_min                  .. L_max
       R  = max (L_max + 1, R_min) .. R_max                  */

  LR_min = R_min;
  LR_max = L_max;

  L_max = min (LR_max, LR_min - 1);
  R_min = max (LR_max + 1, LR_min);

  for (i = 0; i < L_min; i++)

    dw_dx [i] = 0;

  for (i = R_max + 1; i < N; i++)

    dw_dx [i] = 0;

  for (i = L_min; i <= L_max; i++)

    dw_dx [i] = _dedge_dx (filter.left_edge, x [i], filter.x_left,
                                                    filter.left_width);

  for (i = R_min; i <= R_max; i++)

    dw_dx [i] = -_dedge_dx (filter.right_edge, -x [i], -filter.x_right,
                                                        filter.right_width);

  if (LR_min <= LR_max)

    for (i = LR_min; i <= LR_max; i++) {

      l  = _edge     (filter.left_edge, x [i], filter.x_left,
                                               filter.left_width);

      dl = _dedge_dx (filter.left_edge, x [i], filter.x_left,
                                               filter.left_width);

      r  = _edge      (filter.right_edge, -x [i], -filter.x_right,
                                                   filter.right_width);

      dr = -_dedge_dx (filter.right_edge, -x [i], -filter.x_right,
                                                   filter.right_width);

      dw_dx [i] = l * dr + r * dl;

      }

  else

    for (i = L_max + 1; i < R_min; i++)

      dw_dx [i] = 0;

  return N;

  }

int calculate_dwindow_dedge (int N, double x [], double dw_dx [],

                              window_type filter, int edge, int parameter) {

  int i, L_min, L_max, R_min, R_max, LR_min, LR_max;

  double l, r, dl, dr, l_w, r_w;

    /* Calculates the derivative of the window function with respect to
       the position or the width of an edge.

       edge : 0 / 'l' / 'L' = left
              1 / 'r' / 'R' = right

       parameter : 0 / 'x' / 'X'             = position
                   1 / 'w' / 'W' / 's' / 'S' = width     */

  if (edge == 0 || edge == 'l')

    edge = 'L';

  if (edge == 1 || edge == 'r')

    edge = 'R';

  if (parameter == 0   || parameter == 'x')

    parameter = 'X';

  if (parameter == 1   || parameter == 's' ||
      parameter == 'w' || parameter == 'W')

    parameter = 'S';

  filter.left_width  = fabs (l_w = filter.left_width);

  filter.right_width = fabs (r_w = filter.right_width);

  get_regions (N, x, filter, &L_min, &L_max, &R_min, &R_max);


    /* L  = L_min                  .. min (L_max, R_min - 1)
       LR = R_min                  .. L_max
       R  = max (L_max + 1, R_min) .. R_max                  */

  LR_min = R_min;
  LR_max = L_max;

  L_max = min (LR_max, LR_min - 1);
  R_min = max (LR_max + 1, LR_min);

  if (edge == 'L')

    for (i = L_min; i <= L_max; i++)

      dw_dx [i] = _dedge_dxs (filter.left_edge, parameter, x [i],
                               filter.x_left, filter.left_width);

  if (edge == 'R')

    for (i = R_min; i <= R_max; i++)

      dw_dx [i] = _dedge_dxs (filter.right_edge, parameter, -x [i],
                              -filter.x_right, filter.right_width);

  for (i = LR_min; i <= LR_max; i++) {

    if (edge == 'L') {

      dl = _dedge_dxs (filter.left_edge, parameter, x [i],
                        filter.x_left, filter.left_width);

      r  = _edge      (filter.right_edge, -x [i], -filter.x_right,
                                                   filter.right_width);

      dw_dx [i] = r * dl;

      }

    if (edge == 'R') {

      dr = _dedge_dxs (filter.right_edge, parameter, -x [i],
                       -filter.x_right, filter.right_width);

      l  = _edge      (filter.left_edge, x [i], filter.x_left,
                                                filter.left_width);

      dw_dx [i] = l * dr;

      }
    }

  for (i = 0; i <  L_min; i++)

    dw_dx [i] = 0;

  for (i = R_max; i < N; i++)

    dw_dx [i] = 0;

  for (i = LR_max + 1; i < LR_min; i++)

    dw_dx [i] = 0;

  if (edge != 'L')

    for (i = L_min; i <= L_max; i++)

      dw_dx [i] = 0;

  if (edge != 'R')

    for (i = R_min; i <= R_max; i++)

      dw_dx [i] = 0;

  if (edge == 'L' && parameter == 'X' ||
      edge == 'L' && parameter == 'S' && l_w < 0 ||
      edge == 'R' && parameter == 'S' && r_w < 0)

    for (i = 0; i < N; i++)

      dw_dx [i] = -dw_dx [i];

  return N;

  }

int Fourier_filter (int N, double dk, double * chi, double * chi_w,
                             double dr, complex * p, double * p_w,
                             double * chi_d) {

  /* (chi * chi_w -> p) * p_w -> chi_d

     If dr is the step in r, dk the step in k : N * dr * dk = PI

     chi and chi_w are N int, p and p_w are (N / 2 + 1) (N must be even) */

  int i, kill_p;

  if (chi_d == NULL) {

    cwarn ("Fourier_filter : destination chi is NULL");

    return 0;

    }

  if (chi == NULL && p == NULL) {

    cwarn ("Fourier_filter : chi and p are both NULL");

    return 0;

    }

  if (N % 2 != 0) {

    cwarn ("Fourier_filter : N must be even");

    return 0;

    }

  dk = fabs (dk);
  dr = fabs (dr);

  if (dr > dk)

    dk = M_PI / (N * dr);

  else

    if (dr < dk)

      dr = M_PI / (N * dk);

    else

      dr = dk = sqrt (M_PI / N);

  dk /= sqrt (M_PI);
  dr /= sqrt (M_PI);

  kill_p = (p == NULL);

  if (kill_p)

    if ((p = calloc_check_ (N / 2 + 1, sizeof (complex),
                             "Fourier_filter")) == NULL)

      return 0;

  if (chi != NULL) {

    for (i = 0; i < N / 2; i++) {

      p [i].Re = chi [2 * i    ];
      p [i].Im = chi [2 * i + 1];

      }

    if (chi_w != NULL) {

      for (i = 0; i < N / 2; i++) {

        p [i].Re *= chi_w [2 * i    ];
        p [i].Im *= chi_w [2 * i + 1];

        }
      }

    if (!Fourier_transform_real (p, N)) {

      cwarn ("Fourier_filter : cannot do Fourier transform");

      if (kill_p)

        free_check (p);

      return 0;

      }
    }

  for (i = 0; i < N / 2; i++) {

    chi_d [2 * i    ] = (p [i].Re = p [i].Re * dk);
    chi_d [2 * i + 1] = (p [i].Im = p [i].Im * dk);

    }

  chi_d [1] = (p [N / 2].Re = p [N / 2].Re * dk);

  if (p_w != NULL) {

    for (i = 0; i < N / 2; i++) {

      chi_d [2 * i    ] *= p_w [i];
      chi_d [2 * i + 1] *= p_w [i];

      }

    chi_d [1] = p [N / 2].Re * p_w [N / 2];

    }

  if (kill_p)

    free_check (p);

  if (!inverse_Fourier_transform_hermi ((complex *) chi_d, N)) {

    cwarn ("Fourier_filter : cannot do inverse Fourier transform");

    return 0;

    }

  for (i = 0; i < N; i++)

    chi_d [i] *= dr;

  return N;

  }

#endif /* __FOURIER_FILTER_C__ */
