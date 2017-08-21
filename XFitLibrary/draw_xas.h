/***********************************************************************}
{                                                                       }
{  This file contains the routines used to draw the EXAFS               }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__DRAW_EXAFS_H__)

#define __DRAW_EXAFS_H__

#include "exafs_ty.h"

#if defined (__DRAW_EXAFS_C__) || defined (__SAVE_EXAFS_C__)

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

#endif


  /*********************************************************************}
  {                                                                     }
  {  Set up an exafs window                                             }
  {                                                                     }
  {*********************************************************************/

long setup_exafs_window (char title []);


  /*********************************************************************}
  {                                                                     }
  {  External request to redraw the EXAFS window                        }
  {                                                                     }
  {*********************************************************************/

int redraw_exafs (exafs_list * exafs_l);


  /*********************************************************************}
  {                                                                     }
  {  Redraw the EXAFS window if the parameters have changed             }
  {                                                                     }
  {*********************************************************************/

int check_exafs (exafs_list * exafs_l);


  /*********************************************************************}
  {                                                                     }
  {  Close the EXAFS window                                             }
  {                                                                     }
  {*********************************************************************/

int close_exafs_window (exafs_list * exafs_l);


  /*********************************************************************}
  {                                                                     }
  {  Check all the EXAFS windows                                        }
  {                                                                     }
  {*********************************************************************/

int check_all_exafs ();

#endif /* __DRAW_EXAFS_H__ */
