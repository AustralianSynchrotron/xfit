/***********************************************************************}
{                                                                       }
{  This file contains the routines used to save the observed and        }
{  calculated exafs and Fourier-transforms                              }
{                                                                       }
{***********************************************************************/

#if !defined (__SAVE_EXAFS_C__)

#define __SAVE_EXAFS_C__

#include "save_exa.h"
#include "exafs_ty.h"

#include "console.h"

#include "x_curves.h"
#include "global.h"
#include "print.h"

#include "ddata.h"

#define draw_Observed            0
#define draw_i_Observed          1
#define draw_w_Observed          2
#define draw_f_Observed          3
#define draw_Calculated          4
#define draw_i_Calculated        5
#define draw_w_Calculated        6
#define draw_f_Calculated        7
#define draw_Obs_Calc            8
#define draw_k_Window            9

#define draw_im_FT_Observed     10
#define draw_re_FT_Observed     11
#define draw_m_FT_Observed      12
#define draw_w_FT_Observed      13
#define draw_im_FT_Calculated   14
#define draw_re_FT_Calculated   15
#define draw_m_FT_Calculated    16
#define draw_w_FT_Calculated    17
#define draw_m_FT_Obs_Calc      18
#define draw_r_Window           19


#include <stdio.h>


  /* De-weight data */

void deweight (int N, double * D, double * k, double weight) {

  int C;

  if (k && D)
  
    for (C = 0; C < N; C++)

      if (k [C])

        D [C] *= pow (k [C], -weight);

      else
 
        D [C] = 0;

  }


int print_exafs (char filename [], exafs_list * exafs_l, int unweighted) {

  int i, column, count, Use_ko, Use_k, Use_kc, Use_r;

  int No, Nc, N, Nco;

  double * ko, * Xo, * Xoi, * kc, * Xc, * Xci, * k, * k_window,

         * r, * r_window, * Xo_fil, * Xc_fil;

  complex * FTo, * FTc;

  exafs_data_type * exafs;

  char button [36], line [256];

  double * curve [32], x, * C;

  static int type [32] = { 32, 33, 34, 35,

      draw_Observed, draw_i_Observed, draw_w_Observed, draw_f_Observed,

      draw_Calculated, draw_i_Calculated, draw_w_Calculated, draw_f_Calculated,

      draw_Obs_Calc, draw_k_Window,

      draw_im_FT_Observed, draw_re_FT_Observed,

      draw_m_FT_Observed, draw_w_FT_Observed,

      draw_im_FT_Calculated, draw_re_FT_Calculated,

      draw_m_FT_Calculated, draw_w_FT_Calculated,

      draw_m_FT_Obs_Calc, draw_r_Window, 0 };


  static char * heading [] = {

      "K_O", "K", "K_C", "R",

      "CHI_O", "CHI_O_INT", "CHI_O_WIN", "CHI_O_FIL",

      "CHI_C", "CHI_C_INT", "CHI_C_WIN", "CHI_C_FIL",

      "CHI_O_C", "K_WINDOW",

      "FT_O_IM", "FT_O_RE", "FT_O_MOD", "FT_O_WIN",

      "FT_C_IM", "FT_C_RE", "FT_C_MOD", "FT_C_WIN",

      "FT_O_C_MOD", "R_WINDOW",

      0 };


  static char * alias [] = {

      "Photoelectron momentum (observed)",
      "Photoelectron momentum",
      "Photoelectron momentum (calculated)",
      "FT scale in angstrom",

      "Observed XAFS",
      "Interpolated observed XAFS",
      "Windowed observed XAFS",
      "Filtered observed XAFS",

      "Calculated XAFS",
      "Interpolated calculated XAFS",
      "Windowed calculated XAFS",
      "Filtered calculated XAFS",

      "Residual XAFS",

      "XAFS window",

      "Imaginary part of the observed FT",
      "Real part of the observed FT",
      "Modulus of the observed FT",
      "Modulus of the windowed observed FT",

      "Imaginary part of the calculated FT",
      "Real part of the calculated FT",
      "Modulus of the calculated FT",
      "Modulus of the windowed calculated FT",

      "Modulus of the residual FT",

      "FT window",

      0 };


  static char * todeweight [][2] = { { "CHI_O",     "K_O" },  
                                     { "CHI_O_INT", "K"   },
                                     { "CHI_O_WIN", "K"   },
                                     { "CHI_O_FIL", "K"   },

                                     { "CHI_C",     "K_C" },
                                     { "CHI_C_INT", "K"   },
                                     { "CHI_C_WIN", "K"   },
                                     { "CHI_C_FIL", "K"   },

                                     { "CHI_O_C",   "K_O" },

                                     { NULL } };

  int number [32];

  FILE * file;


  void * DData;


    /* Create the data file */

  DData = NewDData ();

  if (DData == NULL) {

    cwarn ("unable to create %s", filename);

    return 0;

    }


  exafs = &(exafs_l->exafs);


    /* Turn on all buttons */

  for (i = 0; i < 20; i++)

    button [i] = 1;


    /* Update the exafs structure */

  copy_set_exafs_data (exafs, global_data.parameters);


    /* Get all of the curves */

  calculate_exafs_curves (exafs, &No, &Nc, &N, &Nco,

                            &ko, &Xo, &Xoi, &kc, &Xc, &Xci, &k, &k_window,

                            &r, &FTo, &FTc, &r_window, &Xo_fil, &Xc_fil);


    /* Turn off the buttons with no corresponding curve */

  if (FTc == 0) {

    button [draw_f_Calculated] =
    button [draw_i_Calculated] =
    button [draw_w_Calculated] =
    button [draw_Calculated] =

    button [draw_im_FT_Calculated] =
    button [draw_re_FT_Calculated] =
    button [draw_m_FT_Calculated] =
    button [draw_w_FT_Calculated] =

    button [draw_Obs_Calc] =
    button [draw_m_FT_Obs_Calc] = 0;

    }

  if (button [draw_Calculated] && k == kc)

    button [draw_i_Calculated] = 0;


    /* Find out which axes need to be printed */

  Use_ko = button [draw_Observed] || button [draw_Obs_Calc];

  Use_k = button [draw_i_Observed] || button [draw_i_Observed] ||
          button [draw_w_Observed] || button [draw_f_Observed] ||

          button [draw_f_Calculated] ||
          button [draw_i_Calculated] ||
          button [draw_w_Calculated] ||

          button [draw_k_Window] ||

         (button [draw_Calculated] && kc == k);

  Use_kc = button [draw_Calculated] && kc != k;

  Use_r = button [draw_r_Window] ||

          button [draw_im_FT_Observed] ||
          button [draw_re_FT_Observed] ||
          button [draw_m_FT_Observed]  ||
          button [draw_w_FT_Observed]  ||

          button [draw_im_FT_Calculated] ||
          button [draw_re_FT_Calculated] ||
          button [draw_m_FT_Calculated]  ||
          button [draw_w_FT_Calculated]  ||

          button [draw_m_FT_Obs_Calc];


    /* Print a column for each button selected */

  button [32] = Use_ko;
  button [33] = Use_k;
  button [34] = Use_kc;
  button [35] = Use_r;


    /* Point to the curves that don't need to be changed */

  curve [0] = ko;
  curve [1] = k;
  curve [2] = kc;
  curve [3] = r;

  curve [4] = Xo;
  curve [5] = Xoi;
  curve [6] = 0;
  curve [7] = Xo_fil;

  curve [8] = Xc;
  curve [9] = Xci;
  curve [10] = 0;
  curve [11] = Xc_fil;

  curve [12] = 0;
  
  curve [13] = k_window;

  curve [14] = 0;
  curve [15] = 0;
  curve [16] = 0;
  curve [17] = 0;

  curve [18] = 0;
  curve [19] = 0;
  curve [20] = 0;
  curve [21] = 0;

  curve [22] = 0;

  curve [23] = r_window;


    /* Get the limits on the column lengths (k < 20, R < 10) */

  for (i = 0; i < N; i++)

    if (k [i] > 20) {

      N = i;

      break;

      }

  if (kc)

    for (i = 0; i < Nc; i++)

      if (kc [i] > 20) {

        Nc = i;

        break;

        }

  for (i = 0; i < Nco; i++)

    if (r [i] > 10 && i >= Nc && i >= N) {

      Nco = i;

      break;

      }


    /* Add the number of entries in each column */

  number [0]  = No;
  number [1]  = N;
  number [2]  = Nc;
  number [3]  = Nco;

  number [4]  = No;
  number [5]  = N;
  number [6]  = N;
  number [7]  = N;

  number [8]  = Nc;
  number [9]  = N;
  number [10] = N;
  number [11] = N;

  number [12] = No;

  number [13] = N;

  number [14] = Nco;
  number [15] = Nco;
  number [16] = Nco;
  number [17] = Nco;

  number [18] = Nco;
  number [19] = Nco;
  number [20] = Nco;
  number [21] = Nco;

  number [22] = Nco;

  number [23] = Nco;


    /* Copy the data */

  for (column = 0; heading [column]; column++) {

    if (button [type [column]]) {

      if (AddDDataColumn (DData, heading [column], number [column], curve [column]))

        if (alias [column])
        
          AddDDataAlias (DData, heading [column], alias [column]);

      }
    }
    


    /* Modify the columns if necessary */

  N = DDataColumn (DData, "CHI_O_C", &C);

  if (N) {

    int i0 = 1;

    for (i = 0; i < N; i++) {

      if (i0 >= Nc)

        i0 = 1;

      if (kc [i0 - 1] > ko [i])

        i0 = 1;

      for (; i0 < Nc; i0++)

        if (kc [i0] >= ko [i])

          break;

      if (i0 < Nc) {

        double f;

        f = (ko [i] - kc [i0 - 1]) / (kc [i0] - kc [i0 - 1]);

        x = Xo [i] - ((1 - f) * Xc [i0 - 1] + f * Xc [i0]);

        }
      else

        x = Xo [i];

      C [i] = x;

      }
    }
  

  N = DDataColumn (DData, "CHI_O_WIN", &C);

  if (N)

    for (i = 0; i < N; i++)

      C [i] = Xoi [i] * k_window [i];


  N = DDataColumn (DData, "CHI_C_WIN", &C);

  if (N)

    for (i = 0; i < N; i++)

      C [i] = Xci [i] * k_window [i];


  N = DDataColumn (DData, "FT_O_RE", &C);

  if (N)

    for (i = 0; i < N; i++)

      C [i] = FTo [i].Re;


  N = DDataColumn (DData, "FT_O_IM", &C);

  if (N)

    for (i = 0; i < N; i++)

      C [i] = FTo [i].Im;


  N = DDataColumn (DData, "FT_O_MOD", &C);

  if (N)

    for (i = 0; i < N; i++)

      C [i] = __cabs (FTo [i]);


  N = DDataColumn (DData, "FT_O_WIN", &C);

  if (N)

    for (i = 0; i < N; i++)

      C [i] = __cabs (FTo [i]) * r_window [i];


  N = DDataColumn (DData, "FT_C_RE", &C);

  if (N)

    for (i = 0; i < N; i++)

      C [i] = FTc [i].Re;


  N = DDataColumn (DData, "FT_C_IM", &C);

  if (N)

    for (i = 0; i < N; i++)

      C [i] = FTc [i].Im;


  N = DDataColumn (DData, "FT_C_MOD", &C);

  if (N)

    for (i = 0; i < N; i++)

      C [i] = __cabs (FTc [i]);


  N = DDataColumn (DData, "FT_C_WIN", &C);

  if (N)

    for (i = 0; i < N; i++)

      C [i] = __cabs (FTc [i]) * r_window [i];


  N = DDataColumn (DData, "FT_O_C_MOD", &C);

  if (N)

    for (i = 0; i < N; i++)

      C [i] = sqrt ((FTo [i].Re - FTc [i].Re) *
                    (FTo [i].Re - FTc [i].Re) +

                    (FTo [i].Im - FTc [i].Im) *
                    (FTo [i].Im - FTc [i].Im));


    /* Save the data */

  if (DDataColumn (DData, "CHI_C_INT", &C))

    AddDDataAlias (DData, "CHI_C_INT", "CHI");

  else
    
    if (DDataColumn (DData, "CHI_C", &C)) {
  
      AddDDataAlias (DData, "CHI_C", "CHI");
      AddDDataAlias (DData, "CHI_C", "CHI_C_INT");

      }

  AddDDataEntry (DData, "COMMENT", "This file was created by the XFIT PRINT command");
  AddDDataEntry (DData, "COMMENT", "Note the different K scales:");
  AddDDataEntry (DData, "COMMENT", "  K_O is the x-scale for CHI_O and CHI_O_C");

  if (DDataColumn (DData, "K_C", &C))

    AddDDataEntry (DData, "COMMENT", "  K_C is the x-scale for CHI_C");

  else

    AddDDataAlias (DData, "K", "K_C");
  
  AddDDataEntry (DData, "COMMENT", "  K is the x-scale for K_WINDOW and all other XAFS columns");

  if (unweighted) {

    AddDDataEntry (DData, "COMMENT", " ");
    AddDDataEntry (DData, "COMMENT", "The XAFS curves are unweighted");

    }


  if (atom_symbol (exafs->absorber)) {

    sprintf (line, "ABSORBER   %s", atom_symbol (exafs->absorber));

    AddDDataEntry (DData, "XFIT", line);

    }

  if (X_ray_edge_symbol (exafs->edge)) {
  
    sprintf (line, "EDGE       %s", X_ray_edge_symbol (exafs->edge));

    AddDDataEntry (DData, "XFIT", line);

    }


    /* Deweight */

  for (column = 0; todeweight [column][0] && unweighted; column++) {

    static double * lC;

    if (column == 0)

      lC = NULL;
    
    N = DDataColumn (DData, todeweight [column][0], &C);

    if (N && N == DDataColumn (DData, todeweight [column][1], &k)) {

      if (C != lC)
      
        deweight (N, C, k, exafs->kW);

      lC = C;

      }
    } 


    /* Write the file */

  WriteDData (DData, filename);


    /* Free the data */
    
  FreeDData (DData);
  

  return 1;

  }


  /*********************************************************************}
  {                                                                     }
  {  Create a file from the data set indicated                          }
  {                                                                     }
  {*********************************************************************/

int print_command (exafs_list * exafs,

                   char filename [], int data_set, int unweighted) {

  exafs_list * data;

  int set, sets;


    /* get the filename if necessary */

  if (filename)

    if (filename [0] == 0)

      filename = NULL;


  if (!filename)

    filename = get_symbol ("OUTPUT FILE");


  if (filename)

    if (filename [0] == 0)

      filename = NULL;


  if (!filename) {

    cout ("\n no file specified\n\n");

    return 0;

    }


  for (sets = 0, data = exafs; data; sets++, data = data->next);

  for (set = 1, data = exafs; data; set++, data = data->next)

    if (set % sets == ((data_set % sets) + sets) % sets)

       print_exafs (filename, data, unweighted);

  return 1;

  }


#endif /* __SAVE_EXAFS_C__ */
