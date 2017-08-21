/***********************************************************************}
{                                                                       }
{  Routines for reading the feff##.dat files written by FEFF 4.06 and   }
{  for calculating EXAFS and the derivatives of EXAFS with respect to   }
{  the parameters, using the data read.                                 }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__FF4_CHI_C__)

#define __FF4_CHI_C__

#include "console.h"

#include "ff4_chi.h"

#define _k_     -1
#define _E0_     0
#define _S02_    1
#define _N_      2
#define _x_      3
#define _y_      4
#define _z_      5
#define _sigma2_ 6

#define _x_polar_ 7
#define _y_polar_ 8
#define _z_polar_ 9

#define _Q_     10


  /**********************************************}
  {                                              }
  {  Functions to manipulate the data structure  }
  {                                              }
  {**********************************************/

void setup_FEFF_406 (FEFF_406_data * FEFF) {

  FEFF->points = 0;

  FEFF->lines  = 0;

  FEFF->line   = NULL;

  }


  /***************************************}
  {                                       }
  {  Read the data from a FEFF 4.06 file  }
  {                                       }
  {***************************************/

int read_FEFF_406 (FILE * file, char filename [], FEFF_406_data * FEFF) {

  char line [256];

  int  i, length, final_line;


    /* Empty the data structure */

  if (FEFF->line != NULL) {

    free (FEFF->line [0]);

    free (FEFF->line);

    }

  setup_FEFF_406 (FEFF);


    /* Count the lines in the header and the storage required */

  FEFF->lines = length = 0;

  final_line = -1;

  while (!feof (file) && FEFF->lines != final_line) {

    fgets (line, 256, file);

    if (!strncmp (line + 1, "--------", 8))

      final_line = FEFF->lines + 4;

    strtok (line, "\r\n");

    length += strlen (line) + 1;

    FEFF->lines++;

    }


    /* Allocate space for the header */

  FEFF->line = calloc_check (FEFF->lines, sizeof (char *));

  if (!FEFF->line)

    return 0;

  FEFF->line [0] = calloc_check (length, sizeof (char));

  if (!FEFF->line [0])

    return 0;


    /* Read and save all lines up to the data */

  rewind (file);

  FEFF->lines = length = 0;

  while (!feof (file) && FEFF->lines != final_line) {

    fgets (line, 256, file);

    strtok (line, "\r\n");

    FEFF->line [FEFF->lines] = FEFF->line [0] + length;

    strcpy (FEFF->line [FEFF->lines], line);

    length += strlen (line) + 1;

    FEFF->lines++;

    }


    /* Check that this isn't the end of the file */

  if (feof (file)) {

    cwarn ("read_FEFF_406 : unexpected EOF in file %s", filename);

    return 0;

    }


    /* Get the absorber, scatterer, distance and size */

  sscanf (FEFF->line [FEFF->lines - 2], " %*ld %*ld %*lf %lf %*lf %*lf %ld",
                    &(FEFF->R), &(FEFF->points));

  sscanf (FEFF->line [1], " %*s Z=%ld", &(FEFF->ZA));
  sscanf (FEFF->line [2], " %*s 1 Z=%ld", &(FEFF->ZS));


    /* Check that the data will fit into the structure */

  if (FEFF->points <= 0 || FEFF->points > MAXPOINTS) {

    cwarn ("read_FEFF_406: %1ld points in the file %s (minimum = 1, maximum = %d)",
             FEFF->points, MAXPOINTS, filename);

    return 0;

    }


    /* Read the data points */

  for (i = 0; i < FEFF->points; i++) {

    if (feof (file)) {

      cwarn ("read_FEFF_406: unexpected EOF in file %s reading data line %1ld",
               filename, i + 1);

      return 0;

      }

    fgets (line, 256, file);

    sscanf (line, " %lf %lf %lf %lf %lf %lf %lf",

                    FEFF->k          + i,
                    FEFF->phi_c      + i,
                    FEFF->feff       + i,
                    FEFF->phase_feff + i,
                    FEFF->red_factor + i,
                    FEFF->lambda     + i,
                    FEFF->real_p     + i);

    }


    /* Return the number of points read */

  return i;

  }


  /*********************************************************************}
  {                                                                     }
  {  Calculate the exafs given the exafs data and model parameters      }
  {                                                                     }
  {*********************************************************************/

int calculate_FEFF_406_EXAFS (FEFF_406_data * FEFF [],

                              double polar [], double Q,

                              double E0, double S02,

                              double N, double v [], double sigma2,

                              int N_, double k [], double Chi []) {

  /*********************************************************************}
  {                                                                     }
  {  FEFF [0], FEFF [1] and FEFF [2] point to the FEFF data             }
  {                                                                     }
  {  All FEFF [] up to the first NULL are used                          }
  {                                                                     }
  {  If possible, these should surround the interatomic distance R      }
  {                                                                     }
  {  If R lie within the range covered, the data is interpolated        }
  {                                                                     }
  {    If the data are at R0 and R1, the weights used are:              }
  {                                                                     }
  {      W0 = (1 / R - 1 / R1) / (1 / R0 - 1 / R1)                      }
  {      W1 = (1 / R0 - 1 / R) / (1 / R0 - 1 / R1)                      }
  {                                                                     }
  {  If R lies outside the range, the nearest data set is used          }
  {                                                                     }
  {  If k <= 0 or k > data limit chi = 0                                }
  {                                                                     }
  {  k must be monotonic increasing                                     }
  {                                                                     }
  {*********************************************************************/

  long i, i0, N_data;

  double polar_t [3], polar_length,

         achi  [MAXPOINTS], achix [MAXPOINTS],

         R_D [3], e, W0, W1, WS, R, p, phase_shift, k_;

  FEFF_406_data FEFF_R;


    /* Clear the array */

  for (i = 0; i < N_; i++)

    Chi [i] = 0;


    /* Check the polarisation */

  polar_length = 0;

  if (polar) {

    polar_t [0] = polar [0];
    polar_t [1] = polar [1];
    polar_t [2] = polar [2];

    polar = polar_t;

    polar_length = normalise3 (polar);

    }

  if (polar_length == 0)

    polar = NULL;

  R = length3 (v);


    /* Calculate the polarisation factor */

  if (polar) {

    p = (v [0] * polar [0] + v [1] * polar [1] + v [2] * polar [2]) / R;

    p = 3 * p * p;

    }
  else

    p = 1;


    /* Calculate achi [..] and achix [..].

       Chi (k) =
                                                             2
             p * Q * S02 * N * red_factor (k) * feff (k) / (k R )

                          2  2
                  -2 sigma  k  - 2 R / lambda (k)
               * e

               * sin (2 k R + phi_c (k) + phase_feff (k))

                                                              2
       achi (k) = p * S02 * N * red_factor (k) * feff (k) / (k R )

                     -2 R / lambda (k)
                  * e

       achix (k) = pchi_c (k) + phase_feff (k)

                                   2  2
                           -2 sigma  k
       Chi (k) = achi (k) e            * sin (2 k R + achix (k))  */


    /* Get the distances from the data sets */

  R_D [0] = (FEFF [0] == NULL ? 0 : FEFF [0]->R);
  R_D [1] = (FEFF [1] == NULL ? 0 : FEFF [1]->R);
  R_D [2] = (FEFF [2] == NULL ? 0 : FEFF [2]->R);


    /* Count the data sets */

  N_data = (FEFF [2] != NULL) + (FEFF [1] != NULL);



  if (R <= R_D [0] || R >= R_D [N_data]) {

      /* Using one data set only, with no interpolation */

    if (R >= R_D [N_data])

      FEFF += N_data;

    FEFF_R = *(FEFF [0]);

    }
  else {

      /* Interpolating between two data sets.

         Set FEFF so that we are using FEFF [0] and FEFF [1] */

    if (R > R_D [1]) {

      FEFF += 1;

      R_D [0] = R_D [1];
      R_D [1] = R_D [2];

      }

    W0 = (1 / R - 1 / R_D [1]) / (1 / R_D [0] - 1 / R_D [1]);
    W1 = (1 / R_D [0] - 1 / R) / (1 / R_D [0] - 1 / R_D [1]);

    if (W0 >= W1)

      WS = -W1;

    else

      WS = W0;


      /* Calculate the interpolated data set */

    FEFF_R.points = FEFF [0]->points;

    for (i = 0; i < FEFF_R.points; i++) {

      FEFF_R.k          [i] =      FEFF [0]->k          [i];

      FEFF_R.phi_c      [i] = W0 * FEFF [0]->phi_c      [i] +
                              W1 * FEFF [1]->phi_c      [i];
      FEFF_R.feff       [i] = W0 * FEFF [0]->feff       [i] +
                              W1 * FEFF [1]->feff       [i];
      FEFF_R.red_factor [i] = W0 * FEFF [0]->red_factor [i] +
                              W1 * FEFF [1]->red_factor [i];
      FEFF_R.lambda     [i] = W0 * FEFF [0]->lambda     [i] +
                              W1 * FEFF [1]->lambda     [i];

      phase_shift = M_2PI * floor ((FEFF [1]->phase_feff [i] -
                                    FEFF [0]->phase_feff [i]) / M_2PI + 0.5);

      FEFF_R.phase_feff [i] = W0 * FEFF [0]->phase_feff [i] +
                              W1 * FEFF [1]->phase_feff [i] +
                              WS * phase_shift;

      }
    }


    /* Calculate achi [..] and achix [..]. */

  for (i = 1; i < FEFF_R.points; i++) {

    achi [i] = p * Q * S02 * N * FEFF_R.red_factor [i] * FEFF_R.feff [i] *
               exp (-2 * R / FEFF_R.lambda [i]) / (FEFF_R.k [i] * R * R);

    achix [i] = FEFF_R.phi_c [i] + FEFF_R.phase_feff [i];

    }


     /* Since |feff| = 0 at k = 0, handle the first point separately.

       The phase data is ok.  Linearly extrapolate for the amplitude */

  achi [0] = 2 * achi [1] - achi [2];

  achix [0] = FEFF_R.phi_c [0] + FEFF_R.phase_feff [0];


    /* Set start to the first point covered by the data */

  for (i = 0; i < N_; i++)

    if (E_EXAFS (k [i]) >= E0)

      break;


  for (i0 = 1; i0 < FEFF_R.points && i < N_; i0++) {

      /* If the point k [i] lies in FEFF_R.k [i0 - 1 .. i0] ... */

    k_ = k_EXAFS (E_EXAFS (k [i]) - E0);

    if (FEFF_R.k [i0] >= k_ && FEFF_R.k [i0 - 1] <= k_) {

      e = (k_ - FEFF_R.k [i0 - 1]) / (FEFF_R.k [i0] - FEFF_R.k [i0 - 1]);

      Chi [i] = ((1 - e) * achi [i0 - 1] +
                 (    e) * achi [i0    ]) *

                 exp (-2 * sigma2 * k_ * k_) *

                 sin (2 * k_ * R + (1 - e) * achix [i0 - 1] +
                                   (    e) * achix [i0    ]);

      i++;

      i0--;

      }
    }


    /* Return the number of points */

  return N_;

  }


  /*********************************************************************}
  {                                                                     }
  {  Calculate the derivative of the exafs with respect to a parameter  }
  {                                                                     }
  {*********************************************************************/

int calculate_FEFF_406_dEXAFS_dx (FEFF_406_data * FEFF [],

                                double polar [], double Q,

                                double E0, double S02,

                                double N, double v [], double sigma2,

                                int N_, double k [], double dChi [],

                                int X) {

  /*********************************************************************}
  {                                                                     }
  {  FEFF [0], FEFF [1] and FEFF [2] point to the FEFF data             }
  {                                                                     }
  {  All FEFF [] up to the first NULL are used                          }
  {                                                                     }
  {  If possible, these should surround the interatomic distance R      }
  {                                                                     }
  {  If R lie within the range covered, the data is interpolated        }
  {                                                                     }
  {    If the data are at R0 and R1, the weights used are:              }
  {                                                                     }
  {      W0 = (1 / R - 1 / R1) / (1 / R0 - 1 / R1)                      }
  {      W1 = (1 / R0 - 1 / R) / (1 / R0 - 1 / R1)                      }
  {                                                                     }
  {  If R lies outside the range, the nearest data set is used          }
  {                                                                     }
  {  If k <= 0 or k > data limit chi = 0                                }
  {                                                                     }
  {  k must be monotonic increasing                                     }
  {                                                                     }
  {  The parameter is:                                                  }
  {                                                                     }
  {    X : -1 : k           (_k_)                                       }
  {                                                                     }
  {         0 : E0          (_E0_)                                      }
  {         1 : S02         (_S02_)                                     }
  {         2 : N           (_N_)                                       }
  {         3 : v [0] = x   (_x_)                                       }
  {         4 : v [1] = y   (_y_)                                       }
  {         5 : v [2] = z   (_z_)                                       }
  {         6 : sigma2      (_sigma2_)                                  }
  {                                                                     }
  {         7 : x (polar)   (_x_polar_)                                 }
  {         8 : y (polar)   (_y_polar_)                                 }
  {         9 : z (polar)   (_z_polar_)                                 }
  {                                                                     }
  {        10 : Q                                                       }
  {                                                                     }
  {*********************************************************************/

  int    i, i0, i_, N_data;

  double p, polar_length, polar_t [3], vp_R, R, R_D [3],

         W0, W1, dW0_dR, WS, dWS_dR,

         phase_shift,

         achi  [MAXPOINTS], dachi_dR  [MAXPOINTS],
         achix [MAXPOINTS], dachix_dR [MAXPOINTS],

         dphi_c_dR, dfeff_dR, dphase_feff_dR, dred_factor_dR, dlambda_dR,

         feff, red_factor, lambda,

         k_, e, de_dk,

         achi_, achix_, dachi_dR_, dachix_dR_, s,

         dchi_dR, dR_dx, dchi_dp, dp_dx;

  FEFF_406_data FEFF_R;


    /* Clear the array */

  for (i = 0; i < N_; i++)

    dChi [i] = 0;


    /* Check the polarisation */

  polar_length = 0;

  if (polar) {

    polar_t [0] = polar [0];
    polar_t [1] = polar [1];
    polar_t [2] = polar [2];

    polar = polar_t;

    polar_length = normalise3 (polar);

    }

  if (polar_length == 0)

    polar = NULL;

  R = length3 (v);


    /* Eliminate all cases returning 0 */

  if (X < _k_ || X > _Q_)

    return 0;

  if ((X >= _x_polar_ && X <= _z_polar_) && polar_length == 0)

    return N_;


    /* Calculate the polarisation factor */

  if (polar) {

      /* vp_R = v.polar / |v||polar| */

    vp_R = (v [0] * polar [0] + v [1] * polar [1] + v [2] * polar [2]) / R;

    p = 3 * vp_R * vp_R;

    }
  else

    p = 1;


    /* Get the distances from the data sets */

  R_D [0] = (FEFF [0] == NULL ? 0 : FEFF [0]->R);
  R_D [1] = (FEFF [1] == NULL ? 0 : FEFF [1]->R);
  R_D [2] = (FEFF [2] == NULL ? 0 : FEFF [2]->R);


    /* Count the data sets */

  N_data = (FEFF [2] != NULL) + (FEFF [1] != NULL);


  if (R < R_D [0] || R > R_D [N_data]) {

      /* Using one data set only, with no interpolation */

    if (R >= R_D [N_data])

      FEFF += N_data;

    FEFF_R = *(FEFF [0]);

    W0     = 1;
    W1     = 0;
    dW0_dR = 0;

    }
  else {

      /* Interpolating between two data sets.

         Set FEFF so that we are using FEFF [0] and FEFF [1] */

    if (R > R_D [1]) {

      FEFF += 1;

      R_D [0] = R_D [1];
      R_D [1] = R_D [2];

      }

    W0 = (1 / R - 1 / R_D [1]) / (1 / R_D [0] - 1 / R_D [1]);
    W1 = (1 / R_D [0] - 1 / R) / (1 / R_D [0] - 1 / R_D [1]);

    dW0_dR = (-1 / (R * R)) / (1 / R_D [0] - 1 / R_D [1]);

    if (W0 >= W1)

      WS = -W1;

    else

      WS = W0;

    dWS_dR = dW0_dR;


      /* Calculate the interpolated data set */

    FEFF_R.points = FEFF [0]->points;

    for (i = 0; i < FEFF_R.points; i++) {

      FEFF_R.k          [i] =      FEFF [0]->k          [i];

      FEFF_R.phi_c      [i] = W0 * FEFF [0]->phi_c      [i] +
                              W1 * FEFF [1]->phi_c      [i];
      FEFF_R.feff       [i] = W0 * FEFF [0]->feff       [i] +
                              W1 * FEFF [1]->feff       [i];
      FEFF_R.red_factor [i] = W0 * FEFF [0]->red_factor [i] +
                              W1 * FEFF [1]->red_factor [i];
      FEFF_R.lambda     [i] = W0 * FEFF [0]->lambda     [i] +
                              W1 * FEFF [1]->lambda     [i];

      phase_shift = M_2PI * floor ((FEFF [1]->phase_feff [i] -
                                    FEFF [0]->phase_feff [i]) / M_2PI + 0.5);

      FEFF_R.phase_feff [i] = W0 * FEFF [0]->phase_feff [i] +
                              W1 * FEFF [1]->phase_feff [i] +
                              WS * phase_shift;
      }
    }


    /* Calculate achi [..] and achix [..].

       (leave out the factor of p * Q * S02 * N) */

  for (i = 1; i < FEFF_R.points; i++) {

    achi [i] = FEFF_R.red_factor [i] * FEFF_R.feff [i] *
               exp (-2 * R / FEFF_R.lambda [i]) / (FEFF_R.k [i] * R * R);

    achix [i] = FEFF_R.phi_c [i] + FEFF_R.phase_feff [i];

    }


    /* Since |feff| = 0 at k = 0, handle the first point separately.

       The phase data is ok.  Linearly extrapolate for the amplitude */

  achi [0] = 2 * achi [1] - achi [2];

  achix [0] = FEFF_R.phi_c [0] + FEFF_R.phase_feff [0];


    /* Calculate dachi_dR [..] and dachix_dR [..].

       (include the factor of Q * S02 * N) */

  if (dW0_dR != 0 && X >= _x_ && X <= _z_) {

    for (i = 0; i < FEFF_R.points; i++) {

      dphi_c_dR      = dW0_dR * FEFF [0]->phi_c      [i] -
                       dW0_dR * FEFF [1]->phi_c      [i];
      dfeff_dR       = dW0_dR * FEFF [0]->feff       [i] -
                       dW0_dR * FEFF [1]->feff       [i];
      dred_factor_dR = dW0_dR * FEFF [0]->red_factor [i] -
                       dW0_dR * FEFF [1]->red_factor [i];
      dlambda_dR     = dW0_dR * FEFF [0]->lambda     [i] -
                       dW0_dR * FEFF [1]->lambda     [i];

      phase_shift = M_2PI * floor ((FEFF [1]->phase_feff [i] -
                                    FEFF [0]->phase_feff [i]) / M_2PI + 0.5);

      dphase_feff_dR = dW0_dR * FEFF [0]->phase_feff [i] -
                       dW0_dR * FEFF [1]->phase_feff [i] +
                       dWS_dR * phase_shift;

      feff           = FEFF_R.feff                   [i];
      red_factor     = FEFF_R.red_factor             [i];
      lambda         = FEFF_R.lambda                 [i];

      if (i >= 1)

        dachi_dR [i] = Q * S02 * N * achi [i] *
                       (dred_factor_dR / red_factor + dfeff_dR / feff +
                        -2 * (1 - R / lambda * dlambda_dR) / lambda - 2 / R);

      dachix_dR [i] = dphi_c_dR + dphase_feff_dR;

      }

    dachi_dR [0] = 2 * dachi_dR [1] - dachi_dR [2];

    }


    /* Set start to the first point covered by the data */

  for (i = 0; i < N_; i++)

    if (E_EXAFS (k [i]) >= E0)

      break;


  if (X == _N_ || X == _S02_ || X == _sigma2_ || X == _Q_) {

    for (i0 = 1; i0 < FEFF_R.points && i < N_; i0++) {

        /* If the point k [i] lies in FEFF_R.k [i0 - 1 .. i0] ... */

      k_ = k_EXAFS (E_EXAFS (k [i]) - E0);

      if (FEFF_R.k [i0] >= k_ && FEFF_R.k [i0 - 1] <= k_) {

        e = (k_ - FEFF_R.k [i0 - 1]) / (FEFF_R.k [i0] - FEFF_R.k [i0 - 1]);

        dChi [i] = ((1 - e) * achi [i0 - 1] +
                    (    e) * achi [i0    ]) *

                    exp (-2 * sigma2 * k_ * k_) *

                    sin (2 * k_ * R + (1 - e) * achix [i0 - 1]
                                    + (    e) * achix [i0    ]);

        if (X == _sigma2_)

            /* Add the factor -2 k**2 */

          dChi [i] *= -2 * k_ * k_;

        i++;

        i0--;

        }
      }


      /* Add the missing factor */

    switch (X) {

      case _Q_ :

        Q = p * S02 * N;

        break;

      case _N_ :

        Q = p * Q * S02;

        break;

      case _S02_ :

        Q = p * Q * N;

        break;

      case _sigma2_ :

        Q = p * Q * S02 * N;

      }

    for (i = 0; i < N_; i++)

      dChi [i] *= Q;

    }
  else

      /* Add the factor Q * S02 * N

         The polarisation factor p is still absent */

    for (i_ = 0; i_ < FEFF_R.points; i_++)

      achi [i_] *= Q * S02 * N;



  if (X == _k_ || X == _E0_) {

    for (i_ = i, i0 = 1; i0 < FEFF_R.points && i < N_; i0++) {

        /* If the point k [i] lies in FEFF_R.k [i0 - 1 .. i0] ... */

      k_ = k_EXAFS (E_EXAFS (k [i]) - E0);

      if (FEFF_R.k [i0] >= k_ && FEFF_R.k [i0 - 1] <= k_) {

        de_dk = 1.0 / (FEFF_R.k [i0] - FEFF_R.k [i0 - 1]);

        e = (k_ - FEFF_R.k [i0 - 1]) * de_dk;

        achi_  = (1 - e) * achi  [i0 - 1] + e * achi  [i0];
        achix_ = (1 - e) * achix [i0 - 1] + e * achix [i0];

        e = exp (-2 * sigma2 * k_ * k_);

        dChi [i] =

           sin (2 * k_ * R + achix_) * e *

               (achi_ * (-4 * sigma2 * k_) +

                de_dk * (achi [i0] - achi [i0 - 1])) +

           achi_ * cos (2 * k_ * R + achix_) * e *

             (2 * R + de_dk * (achix [i0] - achix [i0 - 1]));

        if (k_ > 0)

          if (X == _E0_)

              /* dchi / dE0 = dchi / dk * dk / dE0 */

            dChi [i] *= -0.5 * _KEV_ * _KEV_ / k_;

          else

              /* dchi / dk = dchi / dk' * dk' / dk

                           = dchi / dk' * k / k' */

            dChi [i] *= k [i] / k_;


          /* Add the p factor */

        dChi [i] *= p;

        i++;

        i0--;

        }
      }
    }


    /* Derivative with respect to components of the interatomic vector */

  if (X == _x_ || X == _y_ || X == _z_) {

      /* If polar is the normalised polarisation vector:

           dchi / dx [0] = dp / dx [0] * chi / p + dchi / dR * dR / dx [0]

           dR / dx [0] = x [0] / R

           dp / dx [0] = 3 * 2 * (x.polar / |x|**2) *
                                 (polar [0] - x [0] * x.polar / |x|**2)

                       = 3 * 2 *

                    ^   ^        ^         ^       ^   ^
                    x.polar * (polar [0] - x [0] * x.polar) / |x|

           dchi / dR = ...  */

    i_ = X - _x_;

    for (i0 = 1; i0 < FEFF_R.points && i < N_; i0++) {

        /* If the point k [i] lies in FEFF_R.k [i0 - 1 .. i0] ... */

      k_ = k_EXAFS (E_EXAFS (k [i]) - E0);

      if (FEFF_R.k [i0] >= k_ && FEFF_R.k [i0 - 1] <= k_) {

        s = exp (-2 * sigma2 * k_ * k_);

        e = (k_ - FEFF_R.k [i0 - 1]) / (FEFF_R.k [i0] - FEFF_R.k [i0 - 1]);

        achi_      = (1 - e) * achi  [i0 - 1] + e * achi  [i0];
        achix_     = (1 - e) * achix [i0 - 1] + e * achix [i0];

        if (dW0_dR) {

          dachi_dR_  = (1 - e) * dachi_dR  [i0 - 1] + e * dachi_dR  [i0];
          dachix_dR_ = (1 - e) * dachix_dR [i0 - 1] + e * dachix_dR [i0];

          dchi_dR = dachi_dR_ * s * sin (2 * k_ * R + achix_) +
                    achi_     * s * cos (2 * k_ * R + achix_) *
                                        (2 * k_ + dachix_dR_);

          }
        else

          dchi_dR = achi_     * s * cos (2 * k_ * R + achix_) *
                                        (2 * k_);


          /* Add the polarisation factor */

        dchi_dR *= p;

        dR_dx = v [i_] / R;

        dChi [i] = dchi_dR * dR_dx;

        if (polar) {

          dchi_dp = achi_ * s * sin (2 * k_ * R + achix_);

          dp_dx = 3 * 2 * vp_R * (polar [i_] - v [i_] / R * vp_R) / R;

          dChi [i] += dchi_dp * dp_dx;

          }

        i++;

        i0--;

        }
      }
    }


    /* Derivative with respect to components of the polarisation vector */

  if (X == _x_polar_ || X == _y_polar_ || X == _z_polar_) {

      /* If the polarisation vector is not normalised:

           dchi / dpolar [0] = dp / dpolar [0] * chi / p

           dp / dpolar [0] = 3 * 2 *

                   (x.polar / (|x||polar|)**2) *

                               (x [0] - polar [0] * x.polar / |polar|**2)

                           = 3 * 2 *

                    ^   ^      ^         ^         ^   ^
                    x.polar * (x [0] - polar [0] * x.polar) / |polar|

         */

    i_ = X - _x_;

    for (i0 = 1; i0 < FEFF_R.points && i < N_; i0++) {

        /* If the point k [i] lies in FEFF_R.k [i0 - 1 .. i0] ... */

      k_ = k_EXAFS (E_EXAFS (k [i]) - E0);

      if (FEFF_R.k [i0] >= k_ && FEFF_R.k [i0 - 1] <= k_) {

        s = exp (-2 * sigma2 * k_ * k_);

        e = (k_ - FEFF_R.k [i0 - 1]) / (FEFF_R.k [i0] - FEFF_R.k [i0 - 1]);

        achi_      = (1 - e) * achi  [i0 - 1] + e * achi  [i0];
        achix_     = (1 - e) * achix [i0 - 1] + e * achix [i0];

        if (polar) {

          dchi_dp = achi_ * s * sin (2 * k_ * R + achix_);

          dp_dx = 3 * 2 * vp_R * (v [i_] / R - polar [i_] * vp_R)

                                                     / polar_length;

          dChi [i] = dchi_dp * dp_dx;

          }

        i++;

        i0--;

        }
      }
    }


    /* Return the number of points */

  return N_;

  }

#undef _k_
#undef _E0_
#undef _S02_
#undef _N_
#undef _x_
#undef _y_
#undef _z_
#undef _sigma2_

#undef _x_polar_
#undef _y_polar_
#undef _z_polar_

#undef _Q_

#undef MAXPOINTS

#endif /* __FF4_CHI_C__ */
