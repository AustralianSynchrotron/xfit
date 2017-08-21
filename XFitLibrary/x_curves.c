/***********************************************************************}
{                                                                       }
{  Calculate the various curves used in the exafs window                }
{                                                                       }
{***********************************************************************/

#if !defined (__X_CURVES_C__)

#define __X_CURVES_C__

#include "x_curves.h"

#include "fourier_.h"
#include "interpol.h"


  /*********************************************************************}
  {                                                                     }
  {  Calculate the various curves used in the exafs window              }
  {                                                                     }
  {  All of the curves can be simply calculated from the following :    }
  {                                                                     }
  {    * experimental k                                                 }
  {                                                                     }
  {    * weighted experimental exafs                                    }
  {                                                                     }
  {    * interpolated experimental exafs                                }
  {                                                                     }
  {    * uniform k                                                      }
  {                                                                     }
  {    * k window                                                       }
  {                                                                     }
  {    * weighted calculated exafs                                      }
  {                                                                     }
  {    * fourier-transform of observed exafs                            }
  {                                                                     }
  {    * fourier-transform of calculated exafs                          }
  {                                                                     }
  {    * r window                                                       }
  {                                                                     }
  {    * filtered observed exafs                                        }
  {                                                                     }
  {    * filtered calculated exafs                                      }
  {                                                                     }
  {                                                                     }
  {  A call to this routine with exafs = 0 will free any space          }
  {                                                                     }
  {*********************************************************************/

int calculate_exafs_curves (exafs_data_type * exafs,

                            int * experimental,

                            int * calculated,

                            int * uniform,

                            int * fft,

                            double ** experimental_k,

                            double ** experimental_exafs,

                            double ** i_experimental_exafs,

                            double ** calculated_k,

                            double ** calculated_exafs,

                            double ** i_calculated_exafs,

                            double ** uniform_k,

                            double ** k_windo,

                            double ** uniform_r,

                            complex ** ft_observed_exafs,

                            complex ** ft_calculated_exafs,

                            double ** r_windo,

                            double ** f_observed_exafs,

                            double ** f_calculated_exafs) {

  static int No_l [8] = { 0 },
             Nc_l [8] = { 0 },
             N_l  [8] = { 0 }, c_count = 0;

  int No, Nc, N, Nco, No_x, Nc_x, N_x, Nco_x, i;

  double k_step, k_step_c, r_step;

  static double * ko = 0, * Xo = 0, * k = 0, * Xoi = 0, * Xci_m = 0,

                * Xo_fil = 0, * Xc_fil = 0, * kc_m = 0, * Xc_m = 0,

                * r = 0, * k_window = 0, * r_window = 0;

  static complex * FTo, * FTc;

  double * Xci, * kc, * Xc;


    /* If exafs = 0 free the space ... */



    /* Get the number of experimental, calculated and uniform k and r points */

  No = exafs->N_exp;
  N  = exafs->N;
  Nco = exafs->N / 2 + 1;

  Nc = 0;

  if (exafs->chi_calc)

    Nc = exafs->Nc;


    /* Add the numbers to the list of the last 8 calls */

  No_l [c_count] = No;
  Nc_l [c_count] = Nc;
  N_l  [c_count] = N;

  c_count = (c_count + 1) % 8;


    /* Get the largest numbers from the last 8 calls */

  No_x = Nc_x = N_x = 0;

  for (i = 0; i < 8; i++) {

    No_x = max (No_x, No_l [i]);
    Nc_x = max (Nc_x, Nc_l [i]);
    N_x  = max (N_x,  N_l  [i]);

    }

  Nco_x = N_x / 2 + 1;


    /* Allocate the arrays based on the last 8 calls */

  ko     = realloc_check (ko,    No_x * sizeof (double));
  Xo     = realloc_check (Xo,    No_x * sizeof (double));

  k      = realloc_check (k,      N_x * sizeof (double));

  Xoi    = realloc_check (Xoi,    N_x * sizeof (double));
  Xci_m  = realloc_check (Xci_m,  N_x * sizeof (double));

  Xo_fil = realloc_check (Xo_fil, N_x * sizeof (double));
  Xc_fil = realloc_check (Xc_fil, N_x * sizeof (double));

  kc_m   = realloc_check (kc_m,  Nc_x * sizeof (double));
  Xc_m   = realloc_check (Xc_m,  Nc_x * sizeof (double));

  r      = realloc_check (r,    Nco_x * sizeof (double));

  FTo    = realloc_check (FTo,  Nco_x * sizeof (complex));
  FTc    = realloc_check (FTc,  Nco_x * sizeof (complex));

  k_window = realloc_check (k_window, N_x * sizeof (double));

  r_window = realloc_check (r_window, Nco_x * sizeof (double));


    /*

    ko [No] *

    Xo [No] *

    k [N] *

    Xoi [N] *              { ko k }

    k_window [N] *         { k }

    r [Nco] *

    r_window [Nco] *       { r }

    FTo [Nco] *            { Xoi k_window r_window }

    Xo_fil [N] *


    kc [Nc] : k / kc_m / 0 *

    Xc [Nc] :  Xc_m / 0 *

    Xci [N] : Xc / Xci_m / 0 * { kc k }

    FTc [Nco] *            { Xci k_window r_window }

    Xc_fil [N] *

    */

    /* Calculate the experimental k */

  for (i = 0; i < No; i++)

    ko [i] = k_EXAFS (exafs->energy [i] - exafs->E0);


    /* Calculate the weighted experimental exafs */

  for (i = 0; i < No; i++)

    Xo [i] = exafs->chi_exp [i] * pow (ko [i], exafs->kW);


    /* Calculate the k scale */

  k_step = delta_k (exafs->r_max);

  for (i = 0; i < N; i++)

    k [i] = i * k_step;


    /* Calculate the interpolated experimental exafs */

  interpolate (0, No, ko, exafs->chi_exp, N, k, Xoi);


    /* Calculate the weighted interpolated experimental exafs */

  for (i = 0; i < N; i++)

    Xoi [i] *= pow (k [i], exafs->kW);


    /* Calculate the k window */

  calculate_window (N, k, k_window, exafs->k_window);


    /* Calculate the r scale */

  r_step = delta_r (exafs->r_max, N);

  for (i = 0; i < Nco; i++)

    r [i] = i * r_step;


    /* Calculate the r window */

  calculate_window (Nco, r, r_window, exafs->r_window);


    /* calculate the filtered observed exafs and FT.

       Quick time check showed that c. 18.2 % is spent in filter */

  Fourier_filter (N, k_step, Xoi, k_window, 0, FTo, r_window, Xo_fil);


    /* Get the calculated exafs k scale */

  if (exafs->chi_calc) {

    Nc = exafs->Nc;

    Xc = Xc_m;

    Xci = Xci_m;

    if (Nc == N && exafs->r_max_c == exafs->r_max) {

        /* The calculated exafs is on the same scale as k */

      kc = k;

      Xci = Xc;

      }
    else {

        /* The calculated exafs is on its own scale */

      k_step_c = delta_k (exafs->r_max_c);

      kc = kc_m;

      for (i = 0; i < Nc; i++)

        kc [i] = i * k_step_c;

      }


      /* Get the weighted calculated exafs */

    for (i = 0; i < Nc; i++)

      Xc [i] = exafs->chi_calc [i] * pow (kc [i], exafs->kW);


      /* Calculate the interpolated calculated exafs */

    if (Xci != Xc) {

      interpolate (0, Nc, kc, exafs->chi_calc, N, k, Xci);


        /* Calculate the weighted interpolated calculated exafs */

      for (i = 0; i < N; i++)

        Xci [i] *= pow (k [i], exafs->kW);

      }


      /* Calculate the filtered observed exafs and FT */

    Fourier_filter (N, k_step, Xci, k_window, 0, FTc, r_window, Xc_fil);

    }


    /* Attach the arrays to the arguments */

  if (experimental) *experimental = No;

  if (uniform)      *uniform = N;

  if (fft)          *fft = Nco;


  if (experimental_k)       *experimental_k = ko;

  if (experimental_exafs)   *experimental_exafs = Xo;

  if (i_experimental_exafs) *i_experimental_exafs = Xoi;

  if (uniform_k)            *uniform_k = k;

  if (k_windo)              *k_windo = k_window;

  if (uniform_r)            *uniform_r = r;

  if (ft_observed_exafs)    *ft_observed_exafs = FTo;

  if (r_windo)              *r_windo = r_window;

  if (f_observed_exafs)     *f_observed_exafs = Xo_fil;

  if (exafs->chi_calc) {

    if (calculated)           *calculated = Nc;

    if (calculated_k)         *calculated_k = kc;

    if (calculated_exafs)     *calculated_exafs = Xc;

    if (i_calculated_exafs)   *i_calculated_exafs = Xci;

    if (ft_calculated_exafs)  *ft_calculated_exafs = FTc;

    if (f_calculated_exafs)   *f_calculated_exafs = Xc_fil;

    }
  else {

    if (calculated)           *calculated = 0;

    if (calculated_k)         *calculated_k = 0;

    if (calculated_exafs)     *calculated_exafs = 0;

    if (i_calculated_exafs)   *i_calculated_exafs = 0;

    if (ft_calculated_exafs)  *ft_calculated_exafs = 0;

    if (f_calculated_exafs)   *f_calculated_exafs = 0;

    }


    /* Return */

  return 1;

  }


#endif /* __X_CURVES_C__ */
