/***********************************************************************}
{                                                                       }
{  This file contains the definitions of exafs types and general        }
{  routines                                                             }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__EXAFS_TYPES_H__)

#define __EXAFS_TYPES_H__

#include "ascii_da.h"
#include "fourier_.h"
#include "paramete.h"

#include "exafs.h"
#include "common.h"


  /*********************************************************************}
  {                                                                     }
  {  Define the structure used to hold an exafs data set                }
  {                                                                     }
  {*********************************************************************/

typedef struct {

  double key;                   /* The key to the parameter vector */

  ascii_data_type ascii_file;

  long absorber, edge, N_exp;

  long N, Nc, mode;

  double r_max, r_max_c, E0c;

  double * energy,
         * chi_exp,
         * sigma,
         * schi_exp,

         * k_exp,
         * k,

         * k_window_v,
         * r_window_v,

         * chi_exp_calc,
         * chi_exp_f,
         * chi_calc,
         * chi_f,

         * save;

  complex * density;

  double E0, W, kW;

  int      shells, shells_0, PQ_offset;

  double * polarisation,
         * normal,
         * ellipticity;

  double * O;

  window_type k_window, r_window;

  char button [32];

  } exafs_data_type;

typedef struct exafs_list_struct {

  exafs_data_type exafs;

  struct exafs_list_struct * next;

  long Gid;

  int changed;

  } exafs_list;


  /*********************************************************************}
  {                                                                     }
  {  Functions manipulating exafs data sets                             }
  {                                                                     }
  {*********************************************************************/

void clear_exafs (exafs_data_type * ED);


  /* Names for the parameters of the exafs files */

#define FSIZE 512

#define D DATA_SET
#define I IDENTIFIER
#define V VECTOR_ID

#define SC NORMAL
#define AT ATOM_t
#define VE VECTOR
#define PO PVECTOR

#if defined (__EXAFS_TYPES_C__) || defined (__FILTER_C__)

#define exafs_E0      0
#define exafs_abs     1
#define exafs_edge    2
#define exafs_W       3
#define exafs_kW      4
#define exafs_N       5
#define exafs_mode    6
#define exafs_rmax    7

#define exafs_k0      8
#define exafs_k1      9
#define exafs_kw0    10
#define exafs_kw1    11
#define exafs_ke0    12
#define exafs_ke1    13

#define exafs_r0     14
#define exafs_r1     15
#define exafs_rw0    16
#define exafs_rw1    17
#define exafs_re0    18
#define exafs_re1    19

#define exafs_shells 20

#define exafs_x(n)  (21 + (n) * 8)
#define exafs_n(n)  (24 + (n) * 8)
#define exafs_E(n)  (27 + (n) * 8)
#define exafs_O(n)  (28 + (n) * 8)

#endif

#if !defined (__EXAFS_TYPES_C__)

extern parameter_type EXAFS_parameter_name [];

#else

parameter_type EXAFS_parameter_name [] =

  { { "E0[%d]",       exafs_E0,     1, FSIZE, 0, D, 0, I, SC, 0, 1 },
    { "absorber[%d]", exafs_abs,    1, FSIZE, 0, D, 0, I, AT, 0, 0 },
    { "edge[%d]",     exafs_edge,   1, FSIZE, 0, D, 0, I, SC, 3, 0 },

    { "W[%d]",        exafs_W,      1, FSIZE, 0, D, 0, I, SC, 0, 1 },
    { "kW[%d]",       exafs_kW,     1, FSIZE, 0, D, 0, I, SC, 0, 1 },
    { "N[%d]",        exafs_N,      1, FSIZE, 0, D, 0, I, SC, 2, 0 },
    { "mode[%d]",     exafs_mode,   1, FSIZE, 0, D, 0, I, SC, 2, 0 },
    { "rmax[%d]",     exafs_rmax,   1, FSIZE, 0, D, 0, I, SC, 2, 0 },

    { "k0[%d]",       exafs_k0,     1, FSIZE, 0, D, 0, I, SC, 0, 1 },
    { "k1[%d]",       exafs_k1,     1, FSIZE, 0, D, 0, I, SC, 0, 1 },
    { "kw0[%d]",      exafs_kw0,    1, FSIZE, 0, D, 0, I, SC, 0, 1 },
    { "kw1[%d]",      exafs_kw1,    1, FSIZE, 0, D, 0, I, SC, 0, 1 },
    { "ke0[%d]",      exafs_ke0,    1, FSIZE, 0, D, 0, I, SC, 2, 0 },
    { "ke1[%d]",      exafs_ke1,    1, FSIZE, 0, D, 0, I, SC, 2, 0 },

    { "r0[%d]",       exafs_r0,     1, FSIZE, 0, D, 0, I, SC, 0, 1 },
    { "r1[%d]",       exafs_r1,     1, FSIZE, 0, D, 0, I, SC, 0, 1 },
    { "rw0[%d]",      exafs_rw0,    1, FSIZE, 0, D, 0, I, SC, 0, 1 },
    { "rw1[%d]",      exafs_rw1,    1, FSIZE, 0, D, 0, I, SC, 0, 1 },
    { "re0[%d]",      exafs_re0,    1, FSIZE, 0, D, 0, I, SC, 2, 0 },
    { "re1[%d]",      exafs_re1,    1, FSIZE, 0, D, 0, I, SC, 2, 0 },

    { "shells[%d]",   exafs_shells, 1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "x[%d]",             exafs_x (0),  1, FSIZE, 1, D, 0, I, VE, 0, 1 },
    { "y[%d]",             exafs_x (0),  1, FSIZE, 2, D, 0, I, VE, 0, 1 },
    { "z[%d]",             exafs_x (0),  1, FSIZE, 3, D, 0, I, VE, 0, 1 },
    { "v[%d]",             exafs_x (0),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },

    { "r[%d]",             exafs_x (0),  1, FSIZE, 1, D, 0, I, PO, 0, 1 },
    { "T[%d]",             exafs_x (0),  1, FSIZE, 2, D, 0, I, PO, 0, 1 },
    { "P[%d]",             exafs_x (0),  1, FSIZE, 3, D, 0, I, PO, 0, 1 },
    { "Q[%d]",             exafs_x (0),  1, FSIZE, 0, D, 0, V, PO, 0, 1 },

    { "O[%d]",             exafs_O (0),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v0[%d]",            exafs_x (0),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation0[%d]", exafs_x (0),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal0[%d]",       exafs_n (0),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity0[%d]",  exafs_E (0),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O0[%d]",            exafs_O (0),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy0[%d]",    exafs_O (0),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v1[%d]",            exafs_x (1),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation1[%d]", exafs_x (1),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal1[%d]",       exafs_n (1),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity1[%d]",  exafs_E (1),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O1[%d]",            exafs_O (1),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy1[%d]",    exafs_O (1),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v2[%d]",            exafs_x (2),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation2[%d]", exafs_x (2),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal2[%d]",       exafs_n (2),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity2[%d]",  exafs_E (2),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O2[%d]",            exafs_O (2),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy2[%d]",    exafs_O (2),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v3[%d]",            exafs_x (3),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation3[%d]", exafs_x (3),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal3[%d]",       exafs_n (3),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity3[%d]",  exafs_E (3),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O3[%d]",            exafs_O (3),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy3[%d]",    exafs_O (3),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v4[%d]",            exafs_x (4),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation4[%d]", exafs_x (4),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal4[%d]",       exafs_n (4),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity4[%d]",  exafs_E (4),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O4[%d]",            exafs_O (4),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy4[%d]",    exafs_O (4),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v5[%d]",            exafs_x (5),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation5[%d]", exafs_x (5),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal5[%d]",       exafs_n (5),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity5[%d]",  exafs_E (5),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O5[%d]",            exafs_O (5),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy5[%d]",    exafs_O (5),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v6[%d]",            exafs_x (6),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation6[%d]", exafs_x (6),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal6[%d]",       exafs_n (6),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity6[%d]",  exafs_E (6),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O6[%d]",            exafs_O (6),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy6[%d]",    exafs_O (6),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v7[%d]",            exafs_x (7),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation7[%d]", exafs_x (7),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal7[%d]",       exafs_n (7),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity7[%d]",  exafs_E (7),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O7[%d]",            exafs_O (7),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy7[%d]",    exafs_O (7),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v8[%d]",            exafs_x (8),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation8[%d]", exafs_x (8),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal8[%d]",       exafs_n (8),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity8[%d]",  exafs_E (8),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O8[%d]",            exafs_O (8),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy8[%d]",    exafs_O (8),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v9[%d]",            exafs_x (9),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation9[%d]", exafs_x (9),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal9[%d]",       exafs_n (9),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity9[%d]",  exafs_E (9),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O9[%d]",            exafs_O (9),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy9[%d]",    exafs_O (9),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v10[%d]",            exafs_x (10),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation10[%d]", exafs_x (10),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal10[%d]",       exafs_n (10),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity10[%d]",  exafs_E (10),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O10[%d]",            exafs_O (10),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy10[%d]",    exafs_O (10),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v11[%d]",            exafs_x (11),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation11[%d]", exafs_x (11),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal11[%d]",       exafs_n (11),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity11[%d]",  exafs_E (11),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O11[%d]",            exafs_O (11),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy11[%d]",    exafs_O (11),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v12[%d]",            exafs_x (12),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation12[%d]", exafs_x (12),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal12[%d]",       exafs_n (12),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity12[%d]",  exafs_E (12),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O12[%d]",            exafs_O (12),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy12[%d]",    exafs_O (12),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v13[%d]",            exafs_x (13),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation13[%d]", exafs_x (13),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal13[%d]",       exafs_n (13),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity13[%d]",  exafs_E (13),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O13[%d]",            exafs_O (13),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy13[%d]",    exafs_O (13),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v14[%d]",            exafs_x (14),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation14[%d]", exafs_x (14),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal14[%d]",       exafs_n (14),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity14[%d]",  exafs_E (14),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O14[%d]",            exafs_O (14),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy14[%d]",    exafs_O (14),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v15[%d]",            exafs_x (15),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation15[%d]", exafs_x (15),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal15[%d]",       exafs_n (15),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity15[%d]",  exafs_E (15),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O15[%d]",            exafs_O (15),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy15[%d]",    exafs_O (15),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v16[%d]",            exafs_x (16),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation16[%d]", exafs_x (16),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal16[%d]",       exafs_n (16),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity16[%d]",  exafs_E (16),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O16[%d]",            exafs_O (16),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy16[%d]",    exafs_O (16),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v17[%d]",            exafs_x (17),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation17[%d]", exafs_x (17),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal17[%d]",       exafs_n (17),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity17[%d]",  exafs_E (17),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O17[%d]",            exafs_O (17),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy17[%d]",    exafs_O (17),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v18[%d]",            exafs_x (18),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation18[%d]", exafs_x (18),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal18[%d]",       exafs_n (18),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity18[%d]",  exafs_E (18),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O18[%d]",            exafs_O (18),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy18[%d]",    exafs_O (18),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v19[%d]",            exafs_x (19),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation19[%d]", exafs_x (19),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal19[%d]",       exafs_n (19),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity19[%d]",  exafs_E (19),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O19[%d]",            exafs_O (19),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy19[%d]",    exafs_O (19),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v20[%d]",            exafs_x (20),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation20[%d]", exafs_x (20),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal20[%d]",       exafs_n (20),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity20[%d]",  exafs_E (20),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O20[%d]",            exafs_O (20),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy20[%d]",    exafs_O (20),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v21[%d]",            exafs_x (21),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation21[%d]", exafs_x (21),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal21[%d]",       exafs_n (21),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity21[%d]",  exafs_E (21),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O21[%d]",            exafs_O (21),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy21[%d]",    exafs_O (21),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v22[%d]",            exafs_x (22),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation22[%d]", exafs_x (22),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal22[%d]",       exafs_n (22),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity22[%d]",  exafs_E (22),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O22[%d]",            exafs_O (22),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy22[%d]",    exafs_O (22),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v23[%d]",            exafs_x (23),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation23[%d]", exafs_x (23),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal23[%d]",       exafs_n (23),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity23[%d]",  exafs_E (23),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O23[%d]",            exafs_O (23),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy23[%d]",    exafs_O (23),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v24[%d]",            exafs_x (24),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation24[%d]", exafs_x (24),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal24[%d]",       exafs_n (24),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity24[%d]",  exafs_E (24),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O24[%d]",            exafs_O (24),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy24[%d]",    exafs_O (24),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v25[%d]",            exafs_x (25),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation25[%d]", exafs_x (25),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal25[%d]",       exafs_n (25),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity25[%d]",  exafs_E (25),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O25[%d]",            exafs_O (25),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy25[%d]",    exafs_O (25),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v26[%d]",            exafs_x (26),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation26[%d]", exafs_x (26),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal26[%d]",       exafs_n (26),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity26[%d]",  exafs_E (26),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O26[%d]",            exafs_O (26),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy26[%d]",    exafs_O (26),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v27[%d]",            exafs_x (27),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation27[%d]", exafs_x (27),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal27[%d]",       exafs_n (27),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity27[%d]",  exafs_E (27),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O27[%d]",            exafs_O (27),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy27[%d]",    exafs_O (27),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v28[%d]",            exafs_x (28),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation28[%d]", exafs_x (28),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal28[%d]",       exafs_n (28),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity28[%d]",  exafs_E (28),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O28[%d]",            exafs_O (28),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy28[%d]",    exafs_O (28),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v29[%d]",            exafs_x (29),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation29[%d]", exafs_x (29),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal29[%d]",       exafs_n (29),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity29[%d]",  exafs_E (29),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O29[%d]",            exafs_O (29),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy29[%d]",    exafs_O (29),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v30[%d]",            exafs_x (30),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation30[%d]", exafs_x (30),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal30[%d]",       exafs_n (30),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity30[%d]",  exafs_E (30),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O30[%d]",            exafs_O (30),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy30[%d]",    exafs_O (30),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v31[%d]",            exafs_x (31),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation31[%d]", exafs_x (31),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal31[%d]",       exafs_n (31),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity31[%d]",  exafs_E (31),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O31[%d]",            exafs_O (31),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy31[%d]",    exafs_O (31),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { "v32[%d]",            exafs_x (32),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "polarisation32[%d]", exafs_x (32),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "normal32[%d]",       exafs_n (32),  1, FSIZE, 0, D, 0, V, VE, 0, 1 },
    { "ellipticity32[%d]",  exafs_E (32),  1, FSIZE, 0, D, 0, I, SC, 0, 1 },

    { "O32[%d]",            exafs_O (32),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },
    { "occupancy32[%d]",    exafs_O (32),  1, FSIZE, 0, D, 0, I, SC, 0, 0 },

    { 0 } };

#endif

#undef D
#undef I
#undef V

#undef SC
#undef AT
#undef VE
#undef PO

void copy_exafs_data (parameter_set * set, exafs_data_type E);

int copy_set_exafs_data (exafs_data_type * E, parameter_set * set);

int rationalise_exafs (parameter_set * P, int n, int * Pn0);

#endif /* __EXAFS_TYPES_H__ */
