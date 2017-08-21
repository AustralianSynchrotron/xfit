/***********************************************************************}
{                                                                       }
{  Apply the filter to chi and the derivatives of chi                   }
{                                                                       }
{***********************************************************************/

#if !defined (__FILTER_C__)

#define __FILTER_C__

#include "filter.h"

#include "console.h"

#include "common.h"
#include "fourier_.h"
#include "interpol.h"


  /*********************************************************************}
  {                                                                     }
  {  Calculate d (filtered chi) / d (filtering parameter)               }
  {                                                                     }
  {*********************************************************************/

int dchif_dx (double dchi_f [], exafs_data_type * E,

                            double scratch [], node * x) {

  /*********************************************************************}
  {                                                                     }
  {  Derivatives of chi with respect to filtering parameters :          }
  {                                                                     }
  {          -1                        kW                               }
  {    X = FT   ([R window] * FT (W * k   * [k_window]                  }
  {                                                                     }
  {                         * (interpolate (X   , E   , E0) - X    )))  }
  {                                          obs   obs         calc     }
  {                                                                     }
  {  To calculate the chi values and derivatives we require :           }
  {                                                                     }
  {    * X                                                              }
  {       calc                                                          }
  {                                                                     }
  {    * X                                                              }
  {       obs                                                           }
  {                                                                     }
  {    * window parameters                                              }
  {                                                                     }
  {    * k and R windows                                                }
  {                                                                     }
  {    * dX    / dx                                                     }
  {        calc                                                         }
  {                                                                     }
  {                                                                     }
  {  The routine uses : E->...                                          }
  {                                                                     }
  {    Uniform k : N  k_window_v []  chi_exp_calc []  k []              }
  {                                                                     }
  {    Uniform R : r_max  r_window_v []  density [] **                  }
  {                                                                     }
  {    Experimental : N_exp  energy []  k_exp []  chi_exp []            }
  {                                                                     }
  {    Other : W  kW  E0                                                }
  {                                                                     }
  {    Windows : k_window  r_window                                     }
  {                                                                     }
  {    ** density [] is the only array modified                         }
  {                                                                     }
  {  "scratch" must be at least max (N, N_exp) long                     }                                               }
  {                                                                     }
  {*********************************************************************/

  int i, o, N, N_exp, N_R;

  double * dk, * k_window, * R_window, * dchi, delta_R, delta_k_;


    /* Number of points in the arrays */

  N     = E->N;
  N_exp = E->N_exp;

  N_R   = N / 2 + 1;


    /* Calculate the steps in k and R */

  delta_R = delta_r (E->r_max, N);
  delta_k_ = delta_k (E->r_max);


    /* Set the default windows and curve to use in the filter */

  k_window = E->k_window_v;
  R_window = E->r_window_v;

  dchi = E->chi_exp_calc;


    /**********************************************************************}
    {                                                                      }
    {  After this section the calculation is:                              }
    {                                                                      }
    {                -1                        kW                          }
    {    dX / dx = FT   ((R window) * FT (W * k   * (k window) * dchi))    }
    {                                                                      }
    {**********************************************************************/

  switch (o = x->parameter.offset) {

      /* E0 */

    case exafs_E0 :

        /******************************************************************}
        {                                                                  }
        {             -1                        kW                         }
        {  dX/dE0 = FT   ([R window] * FT (W * k   * [k window] *          }
        {                                                                  }
        {                       * (dinterpolate (X   , E   , E0) / dE0)))  }
        {                                         obs   obs                }
        {                                                                  }
        {******************************************************************/

      dk = scratch;

      dchi = dchi_f;

      for (i = 0; i < N_exp; i++)

        dk [i] = dk_dE_EXAFS (E->energy [i] - E->E0);

      dinterpolate_dx (0, N_exp, E->k_exp, dk, E->chi_exp, N, E->k, dchi);

      break;


        /* W */

    case exafs_W :

        /******************************************************************}
        {                                                                  }
        {            -1                    kW                              }
        {  dX/dW = FT   ([R window] * FT (k   * [k window] *               }
        {                                                                  }
        {                     * (interpolate (X   , E   , E0) - X    )))   }
        {                                      obs   obs         calc      }
        {                                                                  }
        {          = X / W                                                 }
        {                                                                  }
        {******************************************************************/

      dchi = dchi_f;

      for (i = 0; i < N; i++)

        dchi [i] = E->chi_exp_calc [i] / E->W;

      break;


        /* kW */

    case exafs_kW :

        /******************************************************************}
        {                                                                  }
        {             -1                             kW                    }
        {  dX/dkW = FT   ([R window] * FT (W * ln k k   * [k window]       }
        {                                                                  }
        {                     * (interpolate (X   , E   , E0) - X    )))   }
        {                                      obs   obs         calc      }
        {                                                                  }
        {******************************************************************/

      dchi = dchi_f;

      for (dchi [0] = 0, i = 1; i < N; i++)

        dchi_f [i] = E->chi_exp_calc [i] * log (E->k [i]);

      break;


        /* k window edge coordinate */

    case exafs_k0 :
    case exafs_k1 :

        /******************************************************************}
        {                                                                  }
        {             -1                        kW                         }
        {  dX/dk# = FT   ([R window] * FT (W * k   * d[k window] / dk#     }
        {                                                                  }
        {                     * (interpolate (X   , E   , E0) - X    )))   }
        {                                      obs   obs         calc      }
        {                                                                  }
        {******************************************************************/

      k_window = scratch;

      calculate_dwindow_dedge (N, E->k, k_window,
                                     E->k_window, o - exafs_k0, 'X');

      break;


        /* k window edge width */

    case exafs_kw0 :
    case exafs_kw1 :

        /******************************************************************}
        {                                                                  }
        {              -1                        kW                        }
        {  dX/dkw# = FT   ([R window] * FT (W * k   * d[k window] / dkw#   }
        {                                                                  }
        {                     * (interpolate (X   , E   , E0) - X    )))   }
        {                                      obs   obs         calc      }
        {                                                                  }
        {******************************************************************/

      k_window = scratch;

      calculate_dwindow_dedge (N, E->k, k_window,
                                     E->k_window, o - exafs_kw0, 'W');

      break;


        /* R window edge coordinate */

    case exafs_r0 :
    case exafs_r1 :

        /******************************************************************}
        {                                                                  }
        {             -1                               kW                  }
        {  dX/dr# = FT   (d[R window] / dr# * FT (W * k   * [k window]     }
        {                                                                  }
        {                     * (interpolate (X   , E   , E0) - X    )))   }
        {                                      obs   obs         calc      }
        {                                                                  }
        {******************************************************************/

      for (i = 0; i < N_R; i++)

        scratch [i] = i * delta_R;

      R_window = scratch;

      calculate_dwindow_dedge (N_R, scratch, R_window,
                                          E->r_window, o - exafs_r0, 'X');

      break;


        /* R window edge width */

    case exafs_rw0 :
    case exafs_rw1 :

        /******************************************************************}
        {                                                                  }
        {              -1                                kW                }
        {  dX/drw# = FT   (d[R window] / drw# * FT (W * k   * [k window]   }
        {                                                                  }
        {                     * (interpolate (X   , E   , E0) - X    )))   }
        {                                      obs   obs         calc      }
        {                                                                  }
        {******************************************************************/

      for (i = 0; i < N_R; i++)

        scratch [i] = i * delta_R;

      R_window = scratch;

      calculate_dwindow_dedge (N_R, scratch, R_window,
                                          E->r_window, o - exafs_rw0, 'W');

      break;


        /* Polarisation / O / ... */

    default :

        /******************************************************************}
        {                                                                  }
        {  dX/dx = 0                                                       }
        {                                                                  }
        {******************************************************************/

      dchi = dchi_f;

      for (i = 0; i < N; i++)

        dchi_f [i] = 0;

        /* Return the number of points */

      return N;

    }


    /* Apply the weighting factor W * k**kW */

  for (i = 0; i < N; i++)

    dchi_f [i] = dchi [i] * E->W * pow (E->k [i], E->kW);


    /* Apply the filter */

  Fourier_filter (N, delta_k_, dchi_f, k_window, 0,
                                    E->density, R_window, dchi_f);


    /* Return the number of points */

  return N;

  }


int apply_filter_ (exafs_data_type * E, parameter_set * P,

                    double chi [], double * dchi_df [],

                    matrix formal_actual,

                    node * x_0) {


  static double * dchi_f   = 0,
                * scratch  = 0,
                * k        = 0,
                * k_window = 0,
                * R_window = 0,
                * k_exp    = 0;

  static complex * density = 0;

  static long d_f_s = 0,
              sc_s  = 0,
              k_s   = 0,
              k_w_s = 0,
              R_w_w = 0,
              k_e_s = 0,
              d_s   = 0;

  int N, N_R, N_exp, i, f, a, used;

  double delta_R, delta_k_, * R, * chi_obs;

  double Q;

  node * x;


    /* Get the array sizes */

  N     = E->N;
  N_exp = E->N_exp;

  N_R   = N / 2 + 1;


    /* Calculate the steps in k and R */

  delta_R  = delta_r (E->r_max, N);
  delta_k_ = delta_k (E->r_max);


    /* Reallocate space for the various arrays */

  realloc_increment ((void **) &k,        &k_s,   N, sizeof (double), 1024);
  realloc_increment ((void **) &k_window, &k_w_s, N, sizeof (double), 1024);
  realloc_increment ((void **) &R_window, &R_w_w, N, sizeof (double), 1024);
  realloc_increment ((void **) &dchi_f,   &d_f_s, N, sizeof (double), 1024);
  realloc_increment ((void **) &density,  &d_s, N_R, sizeof (complex), 512);
  realloc_increment ((void **) &k_exp,    &k_e_s, N_exp,
                                                     sizeof (double), 1024);

  realloc_increment ((void **) &scratch,  &sc_s,  max (N, N_exp),
                                                     sizeof (double), 1024);


    /* Update the exafs structure from the parameter set */

  copy_set_exafs_data (E, P);


    /* Save the calculated exafs

       Allocate space to save the calculated exafs */

  E->chi_calc = realloc (E->chi_calc, n_step (N, 1024) * sizeof (double));


    /* Copy the calculated exafs into the exafs structure */

  for (i = 0; i < N; i++)

    E->chi_calc [i] = chi [i];


    /* Save the number of points and the R limit */

  E->Nc = N;

  E->r_max_c = E->r_max;


    /* Calculate the windows :

       Calculate R and the R window */

  R = k;

  for (i = 0; i < N_R; i++)

    R [i] = delta_R * i;

  calculate_window (N_R, R, R_window, E->r_window);


    /* Calculate k and the k window */

  for (i = 0; i < N; i++)

    k [i] = delta_k_ * i;

  calculate_window (N, k, k_window, E->k_window);


    /* Calculate the experimental k */

  for (i = 0; i < N_exp; i++)

    k_exp [i] = k_EXAFS (E->energy [i] - E->E0);


    /* Attach the temporary arrays to the exafs structure */

  E->k_exp      = k_exp;
  E->k          = k;
  E->k_window_v = k_window;
  E->r_window_v = R_window;

  E->density    = density;


    /* Interpolate the experimental exafs onto the regular k */

  chi_obs = scratch;

  interpolate (0, N_exp, k_exp, E->chi_exp, N, k, chi_obs);


    /* subtract the calculated exafs from the experimental exafs */

  for (i = 0; i < N; i++)

    chi [i] -= chi_obs [i];


    /* Attach chi to the exafs structure */

  E->chi_exp_calc = chi;


    /* Convert the derivatives into derivatives of filtered chi */

  for (f = 0; f < formal_actual.cols; f++) {

      /* Check for uniformly 0 derivatives */

    for (i = 0; i < N; i++)

      if (dchi_df [f][i] != 0)

        break;

    if (i < N) {

      for (; i < N; i++)

          /* Apply the weighting factor W * k**kW */

        dchi_df [f][i] *= E->W * pow (k [i], E->kW);


        /* Apply the filter */

      Fourier_filter (N, delta_k_, dchi_df [f], k_window, 0, density,
                                                  R_window, dchi_df [f]);

      }
    }


    /* For each parameter of this data set, calculate the derivative
       of the filtered exafs with respect to the parameter and add it
       to the derivatives of the filtered exafs calculated thus far */

  for (a = 0, x = x_0; a < formal_actual.rows; a++, x = x->n [0]) {

      /* Check that parameter x is actually used */

    used = 0;

    for (f = 0; f < formal_actual.cols && !used; f++)

      used = m_el (formal_actual, a, f) != 0;

    if (used && x->parameter.key == E->key) {

        /* dchi / dactual [a] is a component of at least one dchi / dformal
           and actual [a] is a parameter of this data set

           Calculate dchi / dactual [a] */

      dchif_dx (dchi_f, E, scratch, x);


        /* Add the contribution to the derivatives */

      for (f = 0; f < formal_actual.cols; f++)

        if (m_el (formal_actual, a, f) != 0)

          for (i = 0; i < N; i++)

            dchi_df [f][i] += m_el (formal_actual, a, f) * dchi_f [i];

      }
    }


    /* Convert chi to filtered chi :

       Apply the weighting factor W * k**kW */

  for (i = 0; i < N; i++)

    chi [i] *= E->W * pow (k [i], E->kW);


    /* Apply the filter */

  Fourier_filter (N, delta_k_, chi, k_window, 0, density, R_window, chi);


    /* Multiply all chi and dchi / dx so Chi**2 is the integral of chi**2 */

  Q = sqrt (delta_k_);

  for (i = 0; i < N; i++)

    chi [i] *= Q;

  for (f = 0; f < formal_actual.cols; f++)

    for (i = 0; i < N; i++)

      dchi_df [f][i] *= Q;


    /* Delete the temporary arrays from the exafs structure */

  E->k_exp      =
  E->k          =
  E->k_window_v =
  E->r_window_v = 0;

  E->density    = 0;


    /* Return the number of points */

  return N;

  }


  /*********************************************************************}
  {                                                                     }
  {  Given the calculated exafs and derivatives prior to the filter,    }
  {  calculate chi and the derivatives after the filter                 }
  {                                                                     }
  {*********************************************************************/

int apply_filter (exafs_data_type * E, parameter_set * P,

                   double chi [], double * dchi_df [],

                   matrix formal_actual,

                   node * x_0) {


    /* The derivatives are not calculated if any of :

         * dchi_df = 0

         * formal_actual.cols = 0

         * x_0 = 0                               */

  if (dchi_df == NULL || formal_actual.cols == 0 || x_0 == NULL) {

    dchi_df = NULL;

    formal_actual = null_matrix;

    x_0 = NULL;

    }

  switch (E->mode) {

    case 0 :

    case 1 :

      /*****************************************************}
      {                                                     }
      {  modes 0 and 1 have been removed from this version  }
      {                                                     }
      {*****************************************************/

      cwarn (" *** modes 0 and 1 are no longer supported ***");

      return 0;

    case 2 :

      return (apply_filter_ (E, P, chi, dchi_df, formal_actual, x_0));

    }

  return 0;

  }


#endif /* __FILTER_C__ */

