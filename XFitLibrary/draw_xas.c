/***********************************************************************}
{                                                                       }
{  This file contains the routines used to draw the EXAFS               }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__DRAW_EXAFS_C__)

#define __DRAW_EXAFS_C__

#include "draw_xas.h"
#include "draw_3d.h"
#include "grad.h"
#include "x_curves.h"
#include "buttons.h"
#include "global.h"

#include "exafs_ty.h"


  /*********************************************************************}
  {                                                                     }
  {  Colours                                                            }
  {                                                                     }
  {*********************************************************************/

#define C_Observed            0   , 1   , 0
#define C_i_Observed          0   , 0.5 , 0
#define C_w_Observed          0.5 , 0.5 , 0
#define C_f_Observed          1   , 1   , 0

#define C_Calculated          0   , 0   , 1
#define C_i_Calculated        0   , 0   , 1
#define C_w_Calculated        0   , 0.5 , 0.5
#define C_f_Calculated        0   , 1   , 1

#define C_Obs_Calc            1   , 0   , 0

#define C_k_Window            1   , 0   , 0

#define C_im_FT_Observed      0   , 0   , 0.5
#define C_re_FT_Observed      0   , 0.5 , 0
#define C_m_FT_Observed       0.5 , 0.5 , 0
#define C_w_FT_Observed       1   , 1   , 0

#define C_im_FT_Calculated    0   , 0   , 1
#define C_re_FT_Calculated    0   , 1   , 0
#define C_m_FT_Calculated     0   , 0.5 , 0.5
#define C_w_FT_Calculated     0   , 1   , 1

#define C_m_FT_Obs_Calc       1   , 0   , 0

#define C_r_Window            1   , 0   , 0


  /*********************************************************************}
  {                                                                     }
  {  Buttons                                                            }
  {                                                                     }
  {*********************************************************************/

#define EXAFS_y_max  1.520
#define EXAFS_y_min -3.675

#define FT_y_max  4.039
#define FT_y_min -1.156

#define Xm 0.92
#define XM 0.97

#define Fm 0.435
#define FM 0.485

button_type exafs_button [] = {

  { 0.10, 0.15, Xm, XM, C_Observed,         0, "O" },
  { 0.17, 0.22, Xm, XM, C_i_Observed,       0, "Oi" },
  { 0.24, 0.29, Xm, XM, C_w_Observed,       0, "Ow" },
  { 0.31, 0.36, Xm, XM, C_f_Observed,       0, "Of" },

  { 0.40, 0.45, Xm, XM, C_Calculated,       0, "C" },
  { 0.47, 0.52, Xm, XM, C_i_Calculated,     0, "Ci" },
  { 0.54, 0.59, Xm, XM, C_w_Calculated,     0, "Cw" },
  { 0.61, 0.66, Xm, XM, C_f_Calculated,     0, "Cf" },

  { 0.70, 0.75, Xm, XM, C_Obs_Calc,         0, "-" },

  { 0.79, 0.84, Xm, XM, C_k_Window,         0, "W" },

  { 0.10, 0.15, Fm, FM, C_im_FT_Observed,   0, "Oi" },
  { 0.17, 0.22, Fm, FM, C_re_FT_Observed,   0, "Or" },
  { 0.24, 0.29, Fm, FM, C_m_FT_Observed,    0, "Om" },
  { 0.31, 0.36, Fm, FM, C_w_FT_Observed,    0, "Ow" },

  { 0.40, 0.45, Fm, FM, C_im_FT_Calculated, 0, "Ci" },
  { 0.47, 0.52, Fm, FM, C_re_FT_Calculated, 0, "Cr" },
  { 0.54, 0.59, Fm, FM, C_m_FT_Calculated,  0, "Cm" },
  { 0.61, 0.66, Fm, FM, C_w_FT_Calculated,  0, "Cw" },

  { 0.70, 0.75, Fm, FM, C_m_FT_Obs_Calc,    0, "-" },

  { 0.79, 0.84, Fm, FM, C_r_Window,         0, "W" },

  { 0 }

  };

#undef Xm
#undef XM
#undef Fm
#undef FM


  /* Draw the axes for a graph

     The graph extends over 0 .. x_max and -y_max .. y_max

     The graph is mapped out in a space x_A .. x_B, y_A .. y_B */

void draw_graph_axes (double x_A, double y_A, double x_B, double y_B,
                      double x_max, double y_max,
                      int x_steps, int y_steps,
                      double dx, double dy) {

  double x, y, X, Y;


    /* Get the scaling factor for the axes */

  X = (x_B - x_A) / x_max;
  Y = (y_B - y_A) / y_max;


    /* Draw the x-axis */

  for (x = next_gradation_division (0, x_max, x_steps);

           continue_gradations; x = next_gradation_division (0, 0, 0))

    if (major_gradation) {

      if (x < x_max * 0.01 / x_steps)

        Colour (0.5, 0.5, 0.5);

      else {

        Colour (1, 1, 1);

        G_draw_number (x_A + x * X, y_A - 1.6 * dy, 0,
                                        dx, dy, "%.2g", x, "C0N0", 0);

        Colour (0.2, 0.2, 0.2);

        }

      G_line2 (x_A + x * X, 2 * y_A - y_B, x_A + x * X, y_B);

      Colour (0.5, 0.5, 0.5);

      G_line2 (x_A + x * X, y_A, x_A + x * X, y_A - 0.25 * dy);

      }
    else {

      Colour (0.1, 0.1, 0.1);

      G_line2 (x_A + x * X, 2 * y_A - y_B, x_A + x * X, y_B);

      }


    /* Draw the y axis */

  Colour (0.5, 0.5, 0.5);

  for (y = next_gradation_division (-y_max, y_max, 2 * y_steps);

           continue_gradations; y = next_gradation_division (0, 0, 0))

    if (major_gradation) {

      Colour (1, 1, 1);

      if (fabs (y) < y_max * 0.01 / y_steps) {

        G_draw_number (x_A - 0.6 * dx, y_A + y * Y - 0.5 * dy, 0,
                                      dx, dy, "%.2g", 0, "R0N0", 0);

        Colour (0.5, 0.5, 0.5);

        }
      else {

        G_draw_number (x_A - 0.6 * dx, y_A + y * Y - 0.5 * dy, 0,
                                      dx, dy, "%.2g", y, "R0N0", 0);

        Colour (0.2, 0.2, 0.2);

        }

      G_line2 (x_A, y_A + y * Y, x_B, y_A + y * Y);

      Colour (0.5, 0.5, 0.5);

      G_line2 (x_A, y_A + y * Y, x_A - 0.25 * dx, y_A + y * Y);

      }
    else {

      Colour (0.1, 0.1, 0.1);

      G_line2 (x_A, y_A + y * Y, x_B, y_A + y * Y);

      }
  }


  /*********************************************************************}
  {                                                                     }
  {  Redraw an exafs window                                             }
  {                                                                     }
  {*********************************************************************/

int draw_exafs (exafs_list * exafs_l) {

  int G_rotate_o, i;

  int No, Nc, N, Nco;

  double * ko, * Xo, * Xoi, * kc, * Xc, * Xci, * k, * k_window,

         * r, * r_window, * Xo_fil, * Xc_fil, * m_FTo, * m_FTc, * T;

  complex * FTo, * FTc;

  exafs_data_type * exafs;

  int No_l, Nc_l, N_l, Nco_l;

  double kmax, omax, cmax, Xmax, rmax, FTmax;

  char * button;

  int changed;


    /* Turn off the rotation */

  G_rotate_o = G_rotate;

  G_rotate = 0;


  exafs = &(exafs_l->exafs);

  button = exafs->button;


    /* Update the exafs structure */

  changed = copy_set_exafs_data (exafs, global_data.parameters);


  if (changed) {

      /* Make sure that the entire window is invalid */

    invalidatewindow (exafs_l->Gid);

    return 0;

    }


    /* Select the exafs window */

  /* select_window (exafs_l->Gid);

  reshape_window (); */


    /* Clear it */

  Clear_RGB (0, 0, 0);


    /* Draw the buttons */

  Ortho (0.0, 1.0, 0.0, 1.0, -10.0, 10.0);

  draw_buttons (exafs_button);


    /* Get all of the curves */

  calculate_exafs_curves (exafs, &No, &Nc, &N, &Nco,

                            &ko, &Xo, &Xoi, &kc, &Xc, &Xci, &k, &k_window,

                            &r, &FTo, &FTc, &r_window, &Xo_fil, &Xc_fil);


    /* Calculate the maximum k to show */

  kmax = min (ko [No - 1], exafs->k_window.x_right +

                      max (exafs->k_window.right_width * 3, 1));


    /* The vertical scale is changed to fit the maximum displayed curve */

  omax = cmax = Xmax = 0;


    /* Get the number of points to display */

  N_l = min (kmax / k [1] + 1, N);

  No_l = 0;

  for (i = 0; i < No; i++)

    if (ko [i] <= kmax)

      No_l = i + 1;

    else

      break;

  Nc_l = 0;

  for (i = 0; i < Nc; i++)

    if (kc [i] <= kmax)

      Nc_l = i + 1;

    else

      break;


    /* Get the maximum observed exafs */

  if (button [draw_Observed] || button [draw_i_Observed])

    for (i = 0; i < No_l; i++)

      omax = max (omax, fabs (Xo [i]));

  else

    if (button [draw_w_Observed])

      for (i = 0; i < N_l; i++)

        omax = max (omax, fabs (Xoi [i] * k_window [i]));

  if (button [draw_f_Observed])

    for (i = 0; i < N_l; i++)

      omax = max (omax, fabs (Xo_fil [i]));


    /* Get the maximum calculated exafs */

  if ((button [draw_Calculated] || button [draw_i_Calculated]) && Xc)

    for (i = 0; i < Nc_l; i++)

      cmax = max (cmax, fabs (Xc [i]));

  else

    if (button [draw_w_Calculated] && Xci)

      for (i = 0; i < Nc_l; i++)

        cmax = max (cmax, fabs (Xci [i] * k_window [i]));

  if (button [draw_f_Calculated] && Xc_fil)

    for (i = 0; i < N_l; i++)

      cmax = max (cmax, fabs (Xc_fil [i]));


    /* Get the maximum difference exafs */

  if (button [draw_Obs_Calc] && Xc)

    for (i = 0; i < N_l; i++)

      Xmax = max (Xmax, fabs (Xoi [i] - Xci [i]));


    /* Get the vertical scale */

  Xmax = max (max (max (omax, cmax), Xmax), 1);


    /* Draw the scale */

  Ortho2 (-0.12, 1.05, EXAFS_y_min, EXAFS_y_max);

  draw_graph_axes (0, 0, 1, 1, kmax, Xmax, 8, 4, 0.016, 0.072);

  Ortho2 (-0.12 * kmax, 1.05 * kmax, EXAFS_y_min * Xmax,
                                     EXAFS_y_max * Xmax);


    /* Draw the weighted experimental exafs */

  if (exafs->button [draw_Observed]) {

    Colour (C_Observed);

    G_polyline2 (No_l, ko, Xo);

    }


    /* Draw the interpolated weighted exafs */

  if (exafs->button [draw_i_Observed]) {

    Colour (C_i_Observed);

    G_polyline2 (N_l, k, Xoi);

    }


    /* Draw the windowed interpolated weighted exafs */

  if (exafs->button [draw_w_Observed]) {

    Colour (C_w_Observed);

    for (i = 0; i < N_l; i++)

      Xoi [i] *= k_window [i];

    G_polyline2 (N_l, k, Xoi);

    }


    /* Draw the weighted calculated exafs data */

  if (exafs->button [draw_Calculated] && Xc) {

    Colour (C_Calculated);

    G_polyline2 (Nc_l, kc, Xc);

    }


    /* Draw the interpolated weighted calculated exafs data */

  if (exafs->button [draw_i_Calculated] && Xci) {

    Colour (C_i_Calculated);

    G_polyline2 (N_l, k, Xci);

    }


    /* Draw the windowed interpolated weighted calculated exafs */

  if (exafs->button [draw_w_Calculated] && Xci) {

    Colour (C_w_Calculated);

    for (i = 0; i < N_l; i++)

      Xci [i] *= k_window [i];

    G_polyline2 (N_l, k, Xci);

    }


    /* Draw the filtered observed exafs */

  if (exafs->button [draw_f_Observed]) {

    Colour (C_f_Observed);

    G_polyline2 (N_l, k, Xo_fil);

    }


    /* Draw the filtered calculated exafs */

  if (exafs->button [draw_f_Calculated] && Xc_fil) {

    Colour (C_f_Calculated);

    G_polyline2 (N_l, k, Xc_fil);

    }


    /* Draw the diference between the observed and calculated exafs */

  if (exafs->button [draw_Obs_Calc] && Xoi && Xci) {

    Colour (C_Obs_Calc);

    for (i = 0; i < N_l; i++)

      Xoi [i] -= Xci [i];

    G_polyline2 (N_l, k, Xoi);

    }


    /* Draw the exafs window */

  if (exafs->button [draw_k_Window]) {

    Colour (C_k_Window);

    Ortho2 (-0.12 * kmax, 1.05 * kmax, EXAFS_y_min, EXAFS_y_max);

    G_polyline2 (N_l, k, k_window);

    Ortho2 (-0.12 * kmax, 1.05 * kmax, EXAFS_y_min * Xmax,
                                       EXAFS_y_max * Xmax);

    }


    /* Calculate the maximum r to show */

  rmax = min (r [Nco - 1], exafs->r_window.x_right +

                      max (exafs->r_window.right_width * 3, 1));


    /* Get the number of points to display */

  Nco_l = min (rmax / r [1] + 1, Nco);


    /* The vertical scale is changed to fit the maximum displayed curve */

  omax = cmax = 0;


    /* Use Xoi, k and k_window as temporary arrays */

  T = Xoi;

  m_FTo = k;

  m_FTc = k_window;


    /* Calculate |FT| for the observed exafs */

  if (button [draw_m_FT_Observed] || button [draw_w_FT_Observed])

    for (i = 0; i < Nco_l; i++)

      m_FTo [i] = __cabs (FTo [i]);


    /* Calculate |FT| for the calculated exafs */

  if ((button [draw_m_FT_Calculated] || button [draw_w_FT_Calculated]) && FTc)

    for (i = 0; i < Nco_l; i++)

      m_FTc [i] = __cabs (FTc [i]);


    /* Calculate |FT| for the difference exafs */

  if (button [draw_m_FT_Obs_Calc] && FTc)

    for (i = 0; i < Nco_l; i++)

      T [i] = sqrt ((FTo [i].Re - FTc [i].Re) * (FTo [i].Re - FTc [i].Re) +
                    (FTo [i].Im - FTc [i].Im) * (FTo [i].Im - FTc [i].Im));


    /* Get the maximum observed FT */

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


    /* Get the maximum calculated exafs */

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


    /* Get the maximum difference exafs */

  if (button [draw_m_FT_Obs_Calc] && FTc)

    for (i = 0; i < Nco_l; i++)

      FTmax = max (FTmax, fabs (T [i]));


    /* Get the vertical scale */

  FTmax = max (max (max (omax, cmax), FTmax), 1);


    /* Draw the axes */

  Ortho2 (-0.12, 1.05, FT_y_min, FT_y_max);

  draw_graph_axes (0, 0, 1, 1, rmax, FTmax, 8, 4, 0.016, 0.072);

  Ortho2 (-0.12 * rmax, 1.05 * rmax, FT_y_min * FTmax,
                                     FT_y_max * FTmax);


    /* Draw the r window */

  if (button [draw_r_Window]) {

    Colour (C_r_Window);

    Ortho2 (-0.12 * rmax, 1.05 * rmax, FT_y_min, FT_y_max);

    G_polyline2 (Nco_l, r, r_window);

    Ortho2 (-0.12 * rmax, 1.05 * rmax, FT_y_min * FTmax,
                                       FT_y_max * FTmax);

    }


    /* Draw the |FT| of the difference exafs */

  if (button [draw_m_FT_Obs_Calc] && FTc) {

    Colour (C_m_FT_Obs_Calc);

    G_polyline2 (Nco_l, r, T);

    }


    /* Draw the FT of the observed exafs */

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


    /* Draw modulus of the FT of the observed exafs */

  if (button [draw_m_FT_Observed]) {

    Colour (C_m_FT_Observed);

    G_polyline2 (Nco_l, r, m_FTo);

    }


    /* Draw the windowed modulud of the FT of the observed exafs */

  if (button [draw_w_FT_Observed]) {

    Colour (C_w_FT_Observed);

    for (i = 0; i < Nco_l; i++)

      m_FTo [i] *= r_window [i];

    G_polyline2 (Nco_l, r, m_FTo);

    }


    /* Draw the FT of the calculated exafs */

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


    /* Draw the modulus of the FT of the calculated exafs */

  if (button [draw_m_FT_Calculated] && FTc) {

    Colour (C_m_FT_Calculated);

    G_polyline2 (Nco_l, r, m_FTc);

    }


    /* Draw the windowed modulus of the FT of the observed exafs */

  if (button [draw_w_FT_Calculated] && FTc) {

    Colour (C_w_FT_Calculated);

    for (i = 0; i < Nco_l; i++)

      m_FTc [i] *= r_window [i];

    G_polyline2 (Nco_l, r, m_FTc);

    }


    /* Swap the buffers */

  swapbuffers ();


    /* Restore the rotation */

  G_rotate = G_rotate_o;


    /* Return */

  return 1;

  }

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


  /*********************************************************************}
  {                                                                     }
  {  Check the buttons in an exafs window                               }
  {                                                                     }
  {  Return 1 if the window should be redrawn                           }
  {                                                                     }
  {*********************************************************************/

int check_exafs_buttons (double x, double y, exafs_list * exafs_l) {

  int changed, i;


    /* Copy the button status into the buttons */

  for (i = 0; i < 20; i++)

    exafs_button [i].status = exafs_l->exafs.button [i];


    /* Check the buttons */

  changed = check_buttons (exafs_button, x, y);


    /* Save the button status */

  for (i = 0; i < 20; i++)

    exafs_l->exafs.button [i] = exafs_button [i].status;


    /* Return */

  return changed;

  }



  /*********************************************************************}
  {                                                                     }
  {  External request to redraw the EXAFS window                        }
  {                                                                     }
  {*********************************************************************/

int redraw_exafs (exafs_list * exafs_l) {

  invalidatewindow (exafs_l->Gid);

  }


  /*********************************************************************}
  {                                                                     }
  {  Redraw the EXAFS window if the parameters have changed             }
  {                                                                     }
  {*********************************************************************/

int check_exafs (exafs_list * exafs_l) {

  int changed;


    /* Update the exafs structure */

  changed = copy_set_exafs_data (& (exafs_l->exafs), global_data.parameters);

  if (changed) 

    invalidatewindow (exafs_l->Gid);

  }


  /*********************************************************************}
  {                                                                     }
  {  EXAFS window callback routine                                      }
  {                                                                     }
  {*********************************************************************/

long exafswin_CB (long Gid, long msg, long mp) {

  static int mouse_button [2] = { 0, 0 };

  static double o_mouse [2] = { 0, 0 };

  int i;

  exafs_list * exafs_l;


  for (exafs_l = * global_data.exafs; exafs_l; exafs_l = exafs_l->next)

    if (exafs_l->Gid == Gid)

      break;

  if (!Gid || !exafs_l)

    return 0;


  switch (msg) {

    case CB_OPEN:

      return 0;


    case CB_PAINT:

      draw_exafs (exafs_l);

      return 0;


    case CB_MOUSE_DN: {

      double x, y;

      mouse (&x, &y);

      if (mp == 1) {

        if (check_exafs_buttons (x, y, exafs_l))

          invalidatewindow (Gid);

        }
      else {

        o_mouse [0] = x;
        o_mouse [1] = y;

        }
 
      mouse_button [mp - 1] = 1;

      return 0;

      }


    case CB_MOUSE_UP:

      mouse_button [mp - 1] = 0;

      return 0;


    case CB_MOUSE_MO: 

      return 0;


    case CB_CLOSE:

      exafs_l->Gid = 0;

      return 0;

    }
  }


  /*********************************************************************}
  {                                                                     }
  {  Set up an exafs window                                             }
  {                                                                     }
  {*********************************************************************/

int setup_exafs_window (char title []) {

  return openwindow (title, OW_SQUARE, exafswin_CB);

  }


#endif /* __DRAW_EXAFS_C__ */
