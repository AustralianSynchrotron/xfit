/***********************************************************************}
{                                                                       }
{  This file contains the routines used to draw the EXAFS               }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined DRAW_XAFS_C

#define DRAW_XAFS_C

#include "draw_xas.h"
#include "grad.h"
#include "x_curves.h"
#include "global.h"

#include "exafs_ty.h"

// typedef int __stdcall fDataW (void *, void *, char *);
typedef int fDataW (void *, void *, char *);

extern fDataW * DataW;

extern void  * PCal;


typedef struct {

  long Gid;

  int No, N, Nc, NR;

  double * k,
         * R,

         * ko,
         * chio,
         * sigmao,
         * smootho,
         * chioi,
         * chiof,

         * kc,
         * chic,
         * chici,
         * chicf,

         * k_window,
         * R_window;

  complex * FTo, * FTc;

  double kmax, Rmax;

  } XafsData;


  // Redraw an xafs window

long draw_xafs (exafs_list * exafs_l, char * title) {

  long Gid;

  int C;

  exafs_data_type * xafs;

  XafsData Data;


  Gid = 0;
  
  xafs = NULL;

  if (exafs_l) {

    Gid = exafs_l->Gid;

    xafs = &(exafs_l->exafs);


      // Set all buttons on

    for (C = 0; C < 32; C++)

      xafs->button [C] = 1;
    

      // Update the exafs structure

    copy_set_exafs_data (xafs, global_data.parameters);


      // Copy the data into the XafsData structure

    calculate_exafs_curves (xafs, &Data.No, &Data.Nc, &Data.N, &Data.NR,

                            &Data.ko, &Data.chio, &Data.chioi,
                            &Data.kc, &Data.chic, &Data.chici,

                            &Data.k, &Data.k_window,

                            &Data.R, &Data.FTo, &Data.FTc,

                            &Data.R_window, &Data.chiof, &Data.chicf);

    Data.sigmao  = xafs->sigma;
    Data.smootho = xafs->schi_exp;

    Data.Gid = Gid;


      // Calculate the maximum k to show

    Data.kmax = min (Data.ko [Data.No - 1], xafs->k_window.x_right +
                      max (xafs->k_window.right_width * 3, 1));


      // Calculate the maximum R to show

    Data.Rmax = min (Data.R [Data.NR - 1], xafs->r_window.x_right +
                      max (xafs->r_window.right_width * 3, 1));

    }


    // Call out

  Gid = DataW (PCal, xafs ? &Data : NULL, title);

  return Gid;

  }

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  


  int i;








      // The vertical scale is changed to fit the maximum displayed curve

  omax = cmax = Xmax = 0;


    // Draw the scale

  Ortho2 (-0.12, 1.05, EXAFS_y_min, EXAFS_y_max);

  draw_graph_axes (0, 0, 1, 1, kmax, Xmax, 8, 4, 0.016, 0.072);

  Ortho2 (-0.12 * kmax, 1.05 * kmax, EXAFS_y_min * Xmax,
                                     EXAFS_y_max * Xmax);


    // Draw the weighted experimental exafs 

  if (exafs->button [draw_Observed]) {

    Colour (C_Observed);

    G_polyline2 (No_l, ko, Xo);

    }


    // Draw the interpolated weighted exafs 

  if (exafs->button [draw_i_Observed]) {

    Colour (C_i_Observed);

    G_polyline2 (N_l, k, Xoi);

    }


    // Draw the windowed interpolated weighted exafs

  if (exafs->button [draw_w_Observed]) {

    Colour (C_w_Observed);

    for (i = 0; i < N_l; i++)

      Xoi [i] *= k_window [i];

    G_polyline2 (N_l, k, Xoi);

    }


    // Draw the weighted calculated exafs data 

  if (exafs->button [draw_Calculated] && Xc) {

    Colour (C_Calculated);

    G_polyline2 (Nc_l, kc, Xc);

    }


    // Draw the interpolated weighted calculated exafs data 

  if (exafs->button [draw_i_Calculated] && Xci) {

    Colour (C_i_Calculated);

    G_polyline2 (N_l, k, Xci);

    }


    // Draw the windowed interpolated weighted calculated exafs 

  if (exafs->button [draw_w_Calculated] && Xci) {

    Colour (C_w_Calculated);

    for (i = 0; i < N_l; i++)

      Xci [i] *= k_window [i];

    G_polyline2 (N_l, k, Xci);

    }


    // Draw the filtered observed exafs

  if (exafs->button [draw_f_Observed]) {

    Colour (C_f_Observed);

    G_polyline2 (N_l, k, Xo_fil);

    }


    // Draw the filtered calculated exafs 

  if (exafs->button [draw_f_Calculated] && Xc_fil) {

    Colour (C_f_Calculated);

    G_polyline2 (N_l, k, Xc_fil);

    }


    // Draw the diference between the observed and calculated exafs 

  if (exafs->button [draw_Obs_Calc] && Xoi && Xci) {

    Colour (C_Obs_Calc);

    for (i = 0; i < N_l; i++)

      Xoi [i] -= Xci [i];

    G_polyline2 (N_l, k, Xoi);

    }


    // Draw the exafs window 

  if (exafs->button [draw_k_Window]) {

    Colour (C_k_Window);

    Ortho2 (-0.12 * kmax, 1.05 * kmax, EXAFS_y_min, EXAFS_y_max);

    G_polyline2 (N_l, k, k_window);

    Ortho2 (-0.12 * kmax, 1.05 * kmax, EXAFS_y_min * Xmax,
                                       EXAFS_y_max * Xmax);

    }


    // Calculate the maximum r to show 

  rmax = min (r [Nco - 1], exafs->r_window.x_right +

                      max (exafs->r_window.right_width * 3, 1));


    // Get the number of points to display 

  Nco_l = min (rmax / r [1] + 1, Nco);


    // The vertical scale is changed to fit the maximum displayed curve

  omax = cmax = 0;


    // Use Xoi, k and k_window as temporary arrays

  T = Xoi;

  m_FTo = k;

  m_FTc = k_window;


    // Calculate |FT| for the observed exafs

  if (button [draw_m_FT_Observed] || button [draw_w_FT_Observed])

    for (i = 0; i < Nco_l; i++)

      m_FTo [i] = __cabs (FTo [i]);


    // Calculate |FT| for the calculated exafs

  if ((button [draw_m_FT_Calculated] || button [draw_w_FT_Calculated]) && FTc)

    for (i = 0; i < Nco_l; i++)

      m_FTc [i] = __cabs (FTc [i]);


    // Calculate |FT| for the difference exafs 

  if (button [draw_m_FT_Obs_Calc] && FTc)

    for (i = 0; i < Nco_l; i++)

      T [i] = sqrt ((FTo [i].Re - FTc [i].Re) * (FTo [i].Re - FTc [i].Re) +
                    (FTo [i].Im - FTc [i].Im) * (FTo [i].Im - FTc [i].Im));


    // Get the maximum observed FT 

  omax = cmax = FTmax = 0;

  if (button [draw_m_FT_Observed])

    for (i = 0; i < Nco_l; i++)

      omax = max (omax, fabs (m_FTo [i]));

  else {

    if (button [draw_w_FT_Observed])

      for (i = 0; i < Nco_l; i++)

        omax = max (omax, fabs (m_FTo [i] * r_window [i]));

    if (button [draw_im_FT_Observed])

      for (i = 0; i < Nco_l; i++)

        omax = max (omax, fabs (FTo [i].Im));

    if (button [draw_re_FT_Observed])

      for (i = 0; i < Nco_l; i++)

        omax = max (omax, fabs (FTo [i].Re));

    }


    // Get the maximum calculated exafs

  if (button [draw_m_FT_Calculated] && FTc)

    for (i = 0; i < Nco_l; i++)

      cmax = max (cmax, fabs (m_FTc [i]));

  else {

    if (button [draw_w_FT_Calculated] && FTc)

      for (i = 0; i < Nco_l; i++)

        cmax = max (cmax, fabs (m_FTc [i] * r_window [i]));

    if (button [draw_im_FT_Calculated] && FTc)

      for (i = 0; i < Nco_l; i++)

        cmax = max (cmax, fabs (FTc [i].Im));

    if (button [draw_re_FT_Calculated] && FTc)

      for (i = 0; i < Nco_l; i++)

        cmax = max (cmax, fabs (FTc [i].Re));

    }


    // Get the maximum difference exafs

  if (button [draw_m_FT_Obs_Calc] && FTc)

    for (i = 0; i < Nco_l; i++)

      FTmax = max (FTmax, fabs (T [i]));


    // Get the vertical scale

  FTmax = max (max (max (omax, cmax), FTmax), 1);


    // Draw the axes 

  Ortho2 (-0.12, 1.05, FT_y_min, FT_y_max);

  draw_graph_axes (0, 0, 1, 1, rmax, FTmax, 8, 4, 0.016, 0.072);

  Ortho2 (-0.12 * rmax, 1.05 * rmax, FT_y_min * FTmax,
                                     FT_y_max * FTmax);


    // Draw the r window 

  if (button [draw_r_Window]) {

    Colour (C_r_Window);

    Ortho2 (-0.12 * rmax, 1.05 * rmax, FT_y_min, FT_y_max);

    G_polyline2 (Nco_l, r, r_window);

    Ortho2 (-0.12 * rmax, 1.05 * rmax, FT_y_min * FTmax,
                                       FT_y_max * FTmax);

    }


    // Draw the |FT| of the difference exafs

  if (button [draw_m_FT_Obs_Calc] && FTc) {

    Colour (C_m_FT_Obs_Calc);

    G_polyline2 (Nco_l, r, T);

    }


    // Draw the FT of the observed exafs 

  if (button [draw_im_FT_Observed]) {

    Colour (C_im_FT_Observed);

    for (i = 0; i < Nco_l; i++)

      T [i] = FTo [i].Im;

    G_polyline2 (Nco_l, r, T);

    }

  if (button [draw_re_FT_Observed]) {

    Colour (C_re_FT_Observed);

    for (i = 0; i < Nco_l; i++)

      T [i] = FTo [i].Re;

    G_polyline2 (Nco_l, r, T);

    }


    // Draw modulus of the FT of the observed exafs 

  if (button [draw_m_FT_Observed]) {

    Colour (C_m_FT_Observed);

    G_polyline2 (Nco_l, r, m_FTo);

    }


    // Draw the windowed modulud of the FT of the observed exafs 

  if (button [draw_w_FT_Observed]) {

    Colour (C_w_FT_Observed);

    for (i = 0; i < Nco_l; i++)

      m_FTo [i] *= r_window [i];

    G_polyline2 (Nco_l, r, m_FTo);

    }


    // Draw the FT of the calculated exafs

  if (button [draw_im_FT_Calculated] && FTc) {

    Colour (C_im_FT_Calculated);

    for (i = 0; i < Nco_l; i++)

      T [i] = FTc [i].Im;

    G_polyline2 (Nco_l, r, T);

    }

  if (button [draw_re_FT_Calculated] && FTc) {

    Colour (C_re_FT_Calculated);

    for (i = 0; i < Nco_l; i++)

      T [i] = FTc [i].Re;

    G_polyline2 (Nco_l, r, T);

    }


    // Draw the modulus of the FT of the calculated exafs 

  if (button [draw_m_FT_Calculated] && FTc) {

    Colour (C_m_FT_Calculated);

    G_polyline2 (Nco_l, r, m_FTc);

    }


    // Draw the windowed modulus of the FT of the observed exafs

  if (button [draw_w_FT_Calculated] && FTc) {

    Colour (C_w_FT_Calculated);

    for (i = 0; i < Nco_l; i++)

      m_FTc [i] *= r_window [i];

    G_polyline2 (Nco_l, r, m_FTc);

    }


    // Return

  return Gid;

  }

$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/




  /*********************************************************************}
  {                                                                     }
  {  External request to redraw the EXAFS window                        }
  {                                                                     }
  {*********************************************************************/

int redraw_exafs (exafs_list * exafs_l) {

  cwarn ("redraw_exafs (%p)", exafs_l);

  return draw_xafs (exafs_l, NULL);
  
  }


  /*********************************************************************}
  {                                                                     }
  {  Redraw the EXAFS window if the parameters have changed             }
  {                                                                     }
  {*********************************************************************/

int check_exafs (exafs_list * exafs_l) {

  int changed;


    // Update the exafs structure

  changed = exafs_l->changed;

  changed = changed || copy_set_exafs_data (& (exafs_l->exafs), global_data.parameters);

  if (changed) 

    draw_xafs (exafs_l, NULL);

  exafs_l->changed = 0;

  return 1;

  }


int check_all_exafs () {

  if (global_data.exafs) {

    exafs_list * xafs;

    for (xafs = *(global_data.exafs); xafs; xafs = xafs->next)

      check_exafs (xafs);

    }

  return 1;
  
  }


long setup_exafs_window (char title []) {

  return draw_xafs (NULL, title);

  }


int close_exafs_window (exafs_list * exafs_l) {

  return draw_xafs (exafs_l, "close window");

  }
  

#endif // DRAW_XAFS_C
