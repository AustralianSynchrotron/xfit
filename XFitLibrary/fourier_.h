/***********************************************************************}
{                                                                       }
{  This file contains routines for :                                    }
{                                                                       }
{    (1) calculating windows                                            }
{                                                                       }
{    (2) doing Fast-Fourier filtering on REGULARLY SPACED data          }
{                                                                       }
{***********************************************************************/

#if !defined (__FOURIER_FILTER_H__)

#define __FOURIER_FILTER_H__

#include "header.h"

#include "fft.h"


typedef enum {

  PARZEN  = 1, LINEAR = 1,

  HANNING = 2, COSINE = 2,

  WELCH   = 3, PARABOLIC = 3,

  GAUSSIAN

  } filter_edge_type;


typedef struct {

  filter_edge_type left_edge, right_edge;

  double x_left, x_right, left_width, right_width;

  } window_type;


#define NULL_window { COSINE, COSINE, 0.0, 1000.0, 1.0, 1.0 }

#if !defined (__FOURIER_FILTER_C__)

extern window_type null_window;

#else

window_type null_window = NULL_window;

#endif

typedef struct {

  filter_edge_type edge;

  char  * name;

  short unique;

  } filter_edge_name_type;


#if !defined (__FOURIER_FILTER_C__)

extern filter_edge_name_type filter_edge_name [];

#else

filter_edge_name_type filter_edge_name [] = {

  { PARZEN,   "linear",    1 },
  { PARZEN,   "parzen",    4 },
  { HANNING,  "cosine",    1 },
  { HANNING,  "hanning",   1 },
  { WELCH,    "parabolic", 1 },
  { WELCH,    "welch",     1 },
  { GAUSSIAN, "gaussian",  1 },

  { 0 } };

#endif


int calculate_window (int N, double x [], double w [], window_type filter);

int calculate_dwindow_dx (int N, double x [], double dw_dx [],

                                              window_type filter);

int calculate_dwindow_dedge (int N, double x [], double dw_dx [],

                              window_type filter, int edge, int parameter);

int Fourier_filter (int N, double dk, double * chi, double * chi_w,

                             double dr, complex * p, double * p_w,

                             double * chi_d);

  /* (chi * chi_w -> p) * p_w -> chi_d

     If dr is the step in r, dk the step in k : N * dr * dk = PI

     chi and chi_w are N int, p and p_w are (N / 2 + 1) (N must be even) */


#endif /* __FOURIER_FILTER_H__ */
