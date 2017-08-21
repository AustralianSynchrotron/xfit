/***********************************************************************}
{                                                                       }
{  This file contains the definitions of exafs types and general        }
{  routines                                                             }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__EXAFS_TYPES_C__)

#define __EXAFS_TYPES_C__

#include "exafs_ty.h"


  /*********************************************************************}
  {                                                                     }
  {  Functions manipulating exafs data sets                             }
  {                                                                     }
  {*********************************************************************/

void clear_exafs (exafs_data_type * exafs) {

  int i;

  for (i = 0; i < sizeof (exafs_data_type); i++)

    ((char *) exafs) [i] = 0;

  exafs->key      = 0;

  exafs->N        = 1024;

  exafs->mode     = 2;

  exafs->r_max    = 28.35926161;

  exafs->W        = 1.0;

  exafs->kW       = 3.0;

  exafs->PQ_offset = exafs_x (0);

  exafs->k_window =

  exafs->r_window = null_window;

  exafs->button [0] = exafs->button [3] =

  exafs->button [4] = exafs->button [7] =

  exafs->button [8] = exafs->button [9] =

  exafs->button [12] = exafs->button [13] =

  exafs->button [16] = exafs->button [17] =

  exafs->button [18] = exafs->button [19] = 1;

  }


void copy_exafs_data (parameter_set * set, exafs_data_type E) {

  vector * P;

  double * x;

  int shell, i;

  P = parameter_vector (set, E.key);

  resize_parameter_vector (P, 21 + E.shells * 8);

  x = &v_el (*P, 0);

  x [0]  = E.E0;
  x [1]  = E.absorber;
  x [2]  = E.edge;

  x [3]  = E.W;
  x [4]  = E.kW;

  x [5]  = E.N;
  x [6]  = E.mode;
  x [7]  = E.r_max;

  x [8]  = E.k_window.x_left;
  x [9]  = E.k_window.x_right;
  x [10] = E.k_window.left_width;
  x [11] = E.k_window.right_width;
  x [12] = E.k_window.left_edge;
  x [13] = E.k_window.right_edge;

  x [14] = E.r_window.x_left;
  x [15] = E.r_window.x_right;
  x [16] = E.r_window.left_width;
  x [17] = E.r_window.right_width;
  x [18] = E.r_window.left_edge;
  x [19] = E.r_window.right_edge;

  x [20] = E.shells;

  for (shell = 0, i = 21; shell < E.shells; shell++) {

    x [i++] = E.polarisation [shell * 3 + 0];
    x [i++] = E.polarisation [shell * 3 + 1];
    x [i++] = E.polarisation [shell * 3 + 2];

    x [i++] = E.normal [shell * 3 + 0];
    x [i++] = E.normal [shell * 3 + 1];
    x [i++] = E.normal [shell * 3 + 2];

    x [i++] = E.ellipticity [shell];

    x [i++] = E.O [shell];

    }
  }


  /*********************************************************************}
  {                                                                     }
  {  Copy the parameters to the exafs data    Return 1 if changed       }
  {                                                                     }
  {*********************************************************************/

int copy_set_exafs_data (exafs_data_type * E, parameter_set * set) {

  vector * P;

  double * x;

  int shell, i, changed;

  P = parameter_vector (set, E->key);

  x = &v_el (*P, 0);

  i = P->rows;

  changed = E->shells != x [20];

  if (changed) { 

    E->shells = x [20];

    E->polarisation = realloc (E->polarisation, E->shells * sizeof (_v3_));
    E->normal       = realloc (E->normal,       E->shells * sizeof (_v3_));
    E->ellipticity  = realloc (E->ellipticity,  E->shells * sizeof (double));

    E->O = realloc (E->O, E->shells * sizeof (double));

    }

  changed = changed || 

            E->E0       != x [0] ||
            E->absorber != x [1] ||
            E->edge     != x [2] ||

            E->W        != x [3] ||
            E->kW       != x [4] ||

            E->N        != x [5] ||
            E->mode     != x [6] ||
            E->r_max    != x [7] ||

            E->k_window.x_left      != x [8]  ||
            E->k_window.x_right     != x [9]  ||
            E->k_window.left_width  != x [10] ||
            E->k_window.right_width != x [11] ||
            E->k_window.left_edge   != x [12] ||
            E->k_window.right_edge  != x [13] ||

            E->r_window.x_left      != x [14] ||
            E->r_window.x_right     != x [15] ||
            E->r_window.left_width  != x [16] ||
            E->r_window.right_width != x [17] ||
            E->r_window.left_edge   != x [18] ||
            E->r_window.right_edge  != x [19];


  if (changed) {

    E->E0       = x [0];
    E->absorber = x [1];
    E->edge     = x [2];

    E->W        = x [3];
    E->kW       = x [4];

    E->N        = x [5];
    E->mode     = x [6];
    E->r_max    = x [7];

    E->k_window.x_left      = x [8];
    E->k_window.x_right     = x [9];
    E->k_window.left_width  = x [10];
    E->k_window.right_width = x [11];
    E->k_window.left_edge   = x [12];
    E->k_window.right_edge  = x [13];

    E->r_window.x_left      = x [14];
    E->r_window.x_right     = x [15];
    E->r_window.left_width  = x [16];
    E->r_window.right_width = x [17];
    E->r_window.left_edge   = x [18];
    E->r_window.right_edge  = x [19];

    }


  if (E->polarisation && E->O)

    for (shell = 0; shell < E->shells; shell++) {

      i = 21 + shell * 8;

      changed = changed ||

                E->polarisation [shell * 3 + 0] != x [i++] ||
                E->polarisation [shell * 3 + 1] != x [i++] ||
                E->polarisation [shell * 3 + 2] != x [i++] ||

                E->normal [shell * 3 + 0] != x [i++] ||
                E->normal [shell * 3 + 1] != x [i++] ||
                E->normal [shell * 3 + 2] != x [i++] ||

                E->ellipticity [shell] != x [i++] ||

                E->O [shell] != x [i++];

      i = 21 + shell * 8;

      if (changed) {

        E->polarisation [shell * 3 + 0] = x [i++];
        E->polarisation [shell * 3 + 1] = x [i++];
        E->polarisation [shell * 3 + 2] = x [i++];

        E->normal [shell * 3 + 0] = x [i++];
        E->normal [shell * 3 + 1] = x [i++];
        E->normal [shell * 3 + 2] = x [i++];

        E->ellipticity [shell] = x [i++];

        E->O [shell] = x [i++];

        }
      }


  return changed;

  }


int rationalise_exafs (parameter_set * P, int Dn, int * Pn0) {

  vector * exafs, * exafs_0;

  int n, Cn, Pn, i, i0;


    /* Get the vectors */

  exafs   = parameter_vector_Oi (P, DATA_SET, Dn);

  exafs_0 = parameter_vector_Oi (P, DATA_SET + COPY, Dn);


    /* Get the shells parameter and the current number of shells */

  Pn = Cn = 0;

  i0 = exafs->rows;

  if (i0 > exafs_shells) {

    Pn = v_el (*exafs, exafs_shells);

    Cn = (i0 - exafs_x (0) + 7) / 8;

    }

  if (Pn == *Pn0)

    Pn = max (Cn, 1);


    /* Pn is the number of shells

       Set the correct vector size */

  resize_parameter_vector (exafs  , exafs_x (0) + Pn * 8);

  resize_parameter_vector (exafs_0, exafs_x (0) + Pn * 8);

  for (i = i0; i < exafs->rows; i++)

    v_el (*exafs, i) = v_el (*exafs_0, i) = 0;

  i0--;


    /* Set the default parameters */

  for (n = *Pn0; n < Pn; n++)

    if (exafs_O (n) != i0)

      v_el (*exafs, exafs_O (n)) = v_el (*exafs_0, exafs_O (n)) = 1;


    /* Set the correct number of shells */

  *Pn0 =

  v_el (*exafs, exafs_shells) =

  v_el (*exafs_0, exafs_shells) = Pn;


  return Pn;

  }

#endif /* __EXAFS_TYPES_H__ */
