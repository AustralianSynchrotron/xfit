/***********************************************************************}
{                                                                       }
{  Multiple-scattering FEFF exafs calculation (FEFF 6.01) for xfit      }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__FEFF_601_C_C)

#define __FEFF_601_C_C

#include "console.h"

#include "feff_601.h"

#define PHASE_NAME "ph%02d.bin"


#include "event.h"

#define _SIZE_          10
#define _SIZE0_         7
#define _LEGS_          0
#define _RMAX_          1
#define _PW_FILTER_     2
#define _CW_FILTER_     3
#define _E_             4
#define _P_             5
#define _shells_        6
#define _E0_(i)         (_SIZE0_ + _SIZE_ * ((i)) + 0)
#define _E0i_(i)        (_SIZE0_ + _SIZE_ * ((i)) + 1)
#define _S02_(i)        (_SIZE0_ + _SIZE_ * ((i)) + 2)
#define _Z_(i)          (_SIZE0_ + _SIZE_ * ((i)) + 3)
#define _potential_(i)  (_SIZE0_ + _SIZE_ * ((i)) + 4)
#define _N_(i)          (_SIZE0_ + _SIZE_ * ((i)) + 5)
#define _x_(i)          (_SIZE0_ + _SIZE_ * ((i)) + 6)
#define _y_(i)          (_SIZE0_ + _SIZE_ * ((i)) + 7)
#define _z_(i)          (_SIZE0_ + _SIZE_ * ((i)) + 8)
#define _sigma2_(i)     (_SIZE0_ + _SIZE_ * ((i)) + 9)


  /* To match the conventional use of the "shells" parameter:

     "shells" = maximum shell index.

     The parameters common to each shell in the model are:

                  default

       LEGS    :  4
       RMAX    :  4
       PW      :  0
       CW      :  0
       E       :  1e9
       P       :  0

     Each shell has the following parameters:

                  default

       E0      :  0
       E0i     :  0
       S02     :  1 : shell 0 / 0 : shell > 0
       POT     :  shell
       Z       :  0
       N       :  1
       x       :  0
       y       :  0
       z       :  0
       sigma2  :  0

     The shell will not be used as an absorber if S02 is 0 and E0 is 0.

     E0 -> E00, S02 -> S020, CENTRAL -> SHELL0

     */

#define is_sigma2_(x) (x >= _sigma2_ (0) && \
                      (x -  _sigma2_ (0)) % _SIZE_ == 0)

#define M MODEL
#define I IDENTIFIER
#define V VECTOR_ID

#define SC NORMAL
#define AT ATOM_t
#define VE VECTOR
#define PO PVECTOR

parameter_type FEFF_601_parameter_name [] =

  { { "LEGS",          _LEGS_,          0,      0, 0, M, 0, I, SC, 0, 0 },
    { "LMAX",          _LEGS_,          0,      0, 0, M, 0, I, SC, 0, 0 },
    { "RMAX",          _RMAX_,          0,      0, 0, M, 0, I, SC, 0, 0 },
    { "PW",            _PW_FILTER_,     0,      0, 0, M, 0, I, SC, 0, 0 },
    { "PW_FILTER",     _PW_FILTER_,     0,      0, 0, M, 0, I, SC, 0, 0 },
    { "CW",            _CW_FILTER_,     0,      0, 0, M, 0, I, SC, 0, 0 },
    { "CW_FILTER",     _CW_FILTER_,     0,      0, 0, M, 0, I, SC, 0, 0 },
    { "E",             _E_,             0,      0, 0, M, 0, I, SC, 0, 1 },
    { "PH",            _P_,             0,      0, 0, M, 0, I, SC, 0, 0 },
    { "REPHASE",       _P_,             0,      0, 0, M, 0, I, SC, 0, 0 },
    { "SHELLS",        _shells_,        0,      0, 0, M, 0, I, SC, 0, 0 },

    { "E0(%d)",        _E0_ (0),        0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "E0%d",          _E0_ (0),        0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "E0",            _E0_ (0),        0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "E0i(%d)",       _E0i_ (0),       0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "E0i%d",         _E0i_ (0),       0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "E0i",           _E0i_ (0),       0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "S02(%d)",       _S02_ (0),       0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "S02%d",         _S02_ (0),       0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "S02",           _S02_ (0),       0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "AF",            _S02_ (0),       0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "AFAC",          _S02_ (0),       0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "SHELL(%d)",     _Z_ (0),         0, _SIZE_, 0, M, 0, I, AT, 0, 0 },
    { "SHELL%d",       _Z_ (0),         0, _SIZE_, 0, M, 0, I, AT, 0, 0 },
    { "CENTRAL",       _Z_ (0),         0, _SIZE_, 0, M, 0, I, AT, 0, 0 },
    { "POT(%d)",       _potential_ (0), 0, _SIZE_, 0, M, 0, I, SC, 0, 0 },
    { "POT%d",         _potential_ (0), 0, _SIZE_, 0, M, 0, I, SC, 0, 0 },
    { "POTENTIAL(%d)", _potential_ (0), 0, _SIZE_, 0, M, 0, I, SC, 0, 0 },
    { "POTENTIAL%d",   _potential_ (0), 0, _SIZE_, 0, M, 0, I, SC, 0, 0 },
    { "N(%d)",         _N_ (0),         0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "N%d",           _N_ (0),         0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "x(%d)",         _x_ (0),         0, _SIZE_, 1, M, 0, I, VE, 0, 1 },
    { "x%d",           _x_ (0),         0, _SIZE_, 1, M, 0, I, VE, 0, 1 },
    { "y(%d)",         _x_ (0),         0, _SIZE_, 2, M, 0, I, VE, 0, 1 },
    { "y%d",           _x_ (0),         0, _SIZE_, 2, M, 0, I, VE, 0, 1 },
    { "z(%d)",         _x_ (0),         0, _SIZE_, 3, M, 0, I, VE, 0, 1 },
    { "z%d",           _x_ (0),         0, _SIZE_, 3, M, 0, I, VE, 0, 1 },
    { "v(%d)",         _x_ (0),         0, _SIZE_, 0, M, 0, V, VE, 0, 1 },
    { "v%d",           _x_ (0),         0, _SIZE_, 0, M, 0, V, VE, 0, 1 },
    { "r(%d)",         _x_ (0),         0, _SIZE_, 1, M, 0, I, PO, 0, 1 },
    { "r%d",           _x_ (0),         0, _SIZE_, 1, M, 0, I, PO, 0, 1 },
    { "T(%d)",         _x_ (0),         0, _SIZE_, 2, M, 0, I, PO, 0, 1 },
    { "T%d",           _x_ (0),         0, _SIZE_, 2, M, 0, I, PO, 0, 1 },
    { "P(%d)",         _x_ (0),         0, _SIZE_, 3, M, 0, I, PO, 0, 1 },
    { "P%d",           _x_ (0),         0, _SIZE_, 3, M, 0, I, PO, 0, 1 },
    { "Q(%d)",         _x_ (0),         0, _SIZE_, 0, M, 0, V, PO, 0, 1 },
    { "Q%d",           _x_ (0),         0, _SIZE_, 0, M, 0, V, PO, 0, 1 },
    { "sigma2(%d)",    _sigma2_ (0),    0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "sigma2%d",      _sigma2_ (0),    0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "s(%d)",         _sigma2_ (0),    0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "s%d",           _sigma2_ (0),    0, _SIZE_, 0, M, 0, I, SC, 0, 1 },

    { 0 } };

#undef M
#undef I
#undef V

#undef SC
#undef AT
#undef VE
#undef PO

static char * working_path = NULL;


  /************************************}
  {                                    }
  {  Rationalise the parameter vector  }
  {                                    }
  {************************************/

int FEFF_601_rationalise (parameter_set * set) {

  static int shells_0 = -1;

  int shell, shells, shell_size, i, i0;

  vector * model, * model_0;


    /* Get the vectors */

  model   = parameter_vector_Oi (set, MODEL, 0);

  model_0 = parameter_vector_Oi (set, MODEL + COPY, 0);

  shells = shell_size = 0;

  i0 = model->rows;

  if (i0 > _shells_) {

    shells = v_el (*model, _shells_);

    shell_size = (i0 -_SIZE0_ - 1) / _SIZE_;

    }

  if (shells == shells_0)

    shells = max (shell_size, 0);

  shells++;


    /* Total number of shells (including shell 0) */

  resize_parameter_vector (model  , _SIZE0_ + shells * _SIZE_);

  resize_parameter_vector (model_0, _SIZE0_ + shells * _SIZE_);

  for (i = i0; i < model->rows; i++)

    v_el (*model, i) = v_el (*model_0, i) = 0;

  i0--;


    /* Overall parameters */

  if (_LEGS_ > i0)

    v_el (*model, _LEGS_) = v_el (*model_0, _LEGS_) = 4;

  if (_RMAX_ > i0)

    v_el (*model, _RMAX_) = v_el (*model_0, _RMAX_) = 4;

  if (_E_ > i0)

    v_el (*model, _E_) = v_el (*model_0, _E_) = 1e9;


    /* Rationalise the parameters */

  v_el (*model, _LEGS_) =

                   ceil (fabs (v_el (*model, _LEGS_)) - 0.01);

  v_el (*model, _RMAX_)      = max (0, v_el (*model, _RMAX_));

  v_el (*model, _PW_FILTER_) = max (0, v_el (*model, _PW_FILTER_));
  v_el (*model, _CW_FILTER_) = max (0, v_el (*model, _CW_FILTER_));

  for (shell = shells_0 + 1; shell < shells; shell++) {

    if (_N_ (shell) != i0)

      v_el (*model, _N_ (shell)) = v_el (*model_0, _N_ (shell)) = 1;

    if (_potential_ (shell) != i0)

       v_el (*model, _potential_ (shell)) =

       v_el (*model_0, _potential_ (shell)) = shell;

    if (_S02_ (shell) != i0)

       v_el (*model, _S02_ (shell)) =

       v_el (*model_0, _S02_ (shell)) = shell < 1;

    }

  shells--;

  v_el (*model, _shells_) = v_el (*model_0, _shells_) = shells_0 = shells;

  return shells;

  }

  /*********************************************************************}
  {                                                                     }
  {  Define the structure used to hold a previous calculation           }
  {                                                                     }
  {*********************************************************************/

typedef struct {

  double * parameter;

  path_l * path;

  double   polar [3],
           normal [3],
           ellipticity;

  double   Q;

  int      A, ZA, hole, shells;

  int      N;

  double * k,
         * chi;

  double   key;

  double   time;

  } FEFF601_P; /* 116 bytes */


  /*********************************************************************}
  {                                                                     }
  {  Free the storage and set the structure to the empty state          }
  {                                                                     }
  {*********************************************************************/

void destruct_FEFF601_P (FEFF601_P *P) {

  free_check (P->parameter);

  destruct_path_l (&(P->path));

  P->parameter = NULL;

  P->polar [0] =
  P->polar [1] =
  P->polar [2] = 0;

  P->normal [0] =
  P->normal [1] =
  P->normal [2] = 0;

  P->ellipticity = 0;

  P->A         =
  P->ZA        =
  P->hole      =
  P->shells    = 0;

  P->key       = 0;

  P->time      = 0;

  free_check (P->k);

  P->k         = NULL;

  free_check (P->chi);

  P->chi       = NULL;

  P->N         = 0;

  }


  /*********************************************************************}
  {                                                                     }
  {  Find the structure with the matching key                           }
  {                                                                     }
  {  If key is 0 and no entries are free, clear and return the oldest   }
  {                                                                     }
  {*********************************************************************/

FEFF601_P * find_FEFF601_P (FEFF601_P P [], int entries, double key) {

  int i;

  double oldest, newest;


    /* Find the maximum and minimum time stamp

       The oldest entry has the lowest time stamp */

  oldest = newest = P [0].time;

  for (i = 1; i < entries; i++) {

    oldest = min (P [i].time, oldest);
    newest = max (P [i].time, newest);

    }


    /* Go through the entries looking for a match */

  for (i = 0; i < entries; i++)

    if (P [i].key == key & key != 0) {

        /* Update the time stamp */

      P [i].time = newest + 1;

      return P + i;

      }


    /* If the key is 0, empty and return the oldest entry */

  if (key == 0)

    for (i = 0; i < entries; i++)

      if (P [i].time == oldest) {

        destruct_FEFF601_P (P + i);

        P [i].time = newest + 1;

        return P + i;

        }


    /* Return NULL on failure */

  return NULL;

  }


  /*********************************************************************}
  {                                                                     }
  {  Check whether the saved calculation matches the parameters.        }
  {                                                                     }
  {*********************************************************************/

FEFF601_P * check_FEFF601_P (FEFF601_P * S,

                             double parameter [],

                             double polar [], double normal [],
                             double ellipticity,

                             int A, int ZA, int hole,

                             int N, double k []) {

  int i, M, shell, shells, used, S_used;

  double R_max, S_R_max;


    /* Check that the absorber and edge are correct */

  if (S->ZA   != ZA   ||
      S->hole != hole ||
      S->A    != A)

    return NULL;


    /* S matches this shell and edge.

       Check that no parameters affecting this calculation have changed:

         (ignore all parameters for shells more distant than RMAX for both
          the current and saved parameters) */

  shells = parameter [_shells_] + 1;

  if (S->shells != shells)

    return NULL;


  S_R_max = S->parameter [_RMAX_];

  R_max = parameter [_RMAX_];

  M = TRUE;


    /* Check the global parameters */

  for (i = 0; i < _SIZE0_ && M; i++)

    M = S->parameter [i] == parameter [i] ||

        (i == _P_ && parameter [i] == 0);

  if (!M)

    return NULL;


    /* Check each shell within RMAX */

  for (shell = 0; shell < shells; shell++) {

    used = distance3 (parameter + _x_ (shell),
                      parameter + _x_ (A)) <= R_max;

    S_used = distance3 (S->parameter + _x_ (shell),
                        S->parameter + _x_ (A)) <= S_R_max;

    if (used && S_used) {

      for (i = _SIZE0_ + _SIZE_ *  shell;
           i < _SIZE0_ + _SIZE_ * (shell + 1) && M; i++) {

        M = S->parameter [i] == parameter [i];

        if (i == _S02_ (shell))

          if (shell == A)

            M |= fabs (S->parameter [i]) > 0.001;

          else

            M |= 1;

        }

      if (!M || used != S_used)

        return NULL;

      }
    }


    /* Check the k values at which chi was calculated */

  if (N > S->N)

    return NULL;

  for (i = 0; i < S->N; i++)

    if (k [i] != S->k [i])

      return NULL;


    /* Check the polarisation vector */

  if (length3 (polar) == 0 && length3 (S->polar) == 0)

    return S;


  if (fabs (dot3 (polar, S->polar) - 1) > 1e-14)

    return NULL;


  if (ellipticity == 0 && S->ellipticity == 0)

    return S;


    /* Check the normal vector */

  if (fabs (dot3 (normal, S->normal) - 1) > 1e-14)

    return NULL;


    /* S matches the parameters */

  return S;

  }

#define MAXSAVED 16


  /*********************************************************************}
  {                                                                     }
  {  Calculate exafs from a single shell                                }
  {                                                                     }
  {*********************************************************************/

double cFEFF_601_EXAFS_i (double parameter [],

                          int ZA, int hole,

                          double polar [], double normal [],
                          double ellipticity,

                          double Q,

                          long Nx, double k [], double Chi [],

                          int save, int status, int A) {

  /*********************************************************************}
  {                                                                     }
  {  Calculate the exafs from shell "A" using FEFF 6.01.                }
  {                                                                     }
  {*********************************************************************/

  static FEFF601_P   SP [MAXSAVED] = { 0 };

  static double      polar0 [3] = { 0 };

  static int         first = 1;

  FEFF601_P        * S, * match;

  double E0, E0i, S02, NA, * potential, * Z, * N, * sigma2,

         polar_t [3], normal_t [3], CW, O;

  _v3_   * v;

  int shells, shell, parameter_size, i, instance, index,

      C_position, C_number, C_path, P, scratch, update_paths, count;

  path_l * path_list, * O_path_list, * path, * O_path;

  union { unsigned int i [2]; double d; } hash [9];
  

    /* Check for any events on the queue */

  Poll_event (0);

  if (first && 0) {

    clean_FEFF6 ("cFEFF_601_EXAFS_i", working_path);

    free_check (working_path);

    working_path = NULL;

    first = 0;

    }


    /* Status < 0: Release all memory and purge all files. */

  if (status < 0) {

    clean_FEFF6 ("cFEFF_601_EXAFS_i", working_path);

    free_check (working_path);

    working_path = NULL;

    for (i = 0; i < MAXSAVED; i++)

      destruct_FEFF601_P (SP + i);

    manage_FEFF6_files ("cFEFF_601_EXAFS_i", working_path, 0, -1);

    return 1;

    }


    /* Check the parameters */

  if (hole < 1)

    hole = 1;

  if (Nx <= 0)

    return 0;

  shells = parameter [_shells_] + 1;

  if (A < 0 || A >= shells)

    return 0;

  parameter_size = _SIZE0_ + _SIZE_ * shells;


    /* Global parameters */

  E0  = parameter [_E0_  (A)];
  E0i = parameter [_E0i_ (A)];
  S02 = parameter [_S02_ (A)];
  NA  = parameter [_N_   (A)];


    /* Check that atom A is an absorber */

  if ((E0 == 0 && E0i == 0 && S02 == 0) || parameter [_Z_ (A)] != ZA)

    return 0;


    /* If the absorber is vacant, return 0 */

  if (Q == 0 || NA == 0)

    return 0;


    /* If the EXAFS is 0, return Q * NA */

  if (S02 == 0) {

    for (i = 0; i < Nx; i++)

      Chi [i] = 0;

    return Q * NA;

    }


    /* Normalise the polarisation vectors */

  if (polar) {

    polar_t [0] = polar [0];
    polar_t [1] = polar [1];
    polar_t [2] = polar [2];

    polar = polar_t;

    normalise3 (polar);

    }
  else

    polar = polar0;

  if (normal) {

    normal_t [0] = normal [0];
    normal_t [1] = normal [1];
    normal_t [2] = normal [2];

    normal = normal_t;

    normalise3 (normal);

    }
  else

    normal = polar0;

  if (ellipticity == 0 || length3 (polar) == 0)

    normal = polar0;

  if (length3 (normal) == 0)

    ellipticity = 0;



    /* The index is used as the key into the set of saved calculations */

  hash [0].i [0] = A;
  hash [0].i [1] = 0;
  hash [1].i [0] = hole;
  hash [1].i [1] = 0;
  
  hash [2].d = polar  [0];
  hash [3].d = polar  [1];
  hash [4].d = polar  [2];
  hash [5].d = normal [0];
  hash [6].d = normal [1];
  hash [7].d = normal [2];

  hash [8].d = ellipticity;

  index = 0;
  
  for (i = 0; i < 9; i++) {

    index = ((unsigned int) index) * 618031 + hash [i].i [0];
    index = ((unsigned int) index) * 391961 + hash [i].i [1];

    }

  index = ((unsigned int) index) % 1000000;


  /*  cout (" %.2d %.7d ZA: %d  hole: %d  polar: %.3f %.3f %.3f  Q: %g\n",
                    A, index, ZA, hole, polar [0], polar [1], polar [2], Q);
   */

    /* Check whether the parameters match a saved calculation */

  S = find_FEFF601_P (SP, MAXSAVED, index + 1);

  /*
  cout (" S = %p\n", S);
  */

  if (S) {

      /* Check that the parameters match */

    match = check_FEFF601_P (S, parameter, polar, normal, ellipticity,
                                                    A, ZA, hole, Nx, k);

    if (match && (status != 1)) {


        /* Multiply the saved chi by Q * N [A] * (new S02) / (old S02) */

      O = parameter [_S02_ (A)] / S->parameter [_S02_ (A)]

          * Q * NA;

      for (i = 0; i < Nx; i++)

        Chi [i] = S->chi [i] * O;


        /* Return the number of absorbing atoms */

      return Q * NA;

      }
    }


    /* Allocate space for the model parameters */

  potential = calloc_check (shells, sizeof (double));
  Z         = calloc_check (shells, sizeof (double));
  N         = calloc_check (shells, sizeof (double));
  v         = calloc_check (shells, sizeof (_v3_));
  sigma2    = calloc_check (shells, sizeof (double));


    /* Copy the model parameters */

  instance = -1;

  for (shell = 0; shell < shells; shell++) {

    potential [shell]    = parameter [_potential_ (shell)];
    Z         [shell]    = parameter [_Z_         (shell)];
    N         [shell]    = parameter [_N_         (shell)];
    v         [shell][0] = parameter [_x_         (shell)];
    v         [shell][1] = parameter [_y_         (shell)];
    v         [shell][2] = parameter [_z_         (shell)];
    sigma2    [shell]    = parameter [_sigma2_    (shell)];

    if (shell <= A && Z [shell] == ZA)

      instance++;

    }


    /* Check for any change in:

           positions
           polarisation
           atomic numbers
           absorber
           core-hole
           atomic potentials
           number of legs
           maximum path length
           path filter parameters

           Check the changes from the saved parameters:

           S: the set of parameters matching the paths */

  C_position = C_number = C_path = 1;

 /* if (S) cout (" %d %d %d %f %f %f %f %f %f\n", polar [0] != S->polar [0],
            polar [1] != S->polar [1], polar [2] != S->polar [2],
            polar [0], polar [1], polar [2],
            S->polar [0], S->polar [1], S->polar [2]);
*/
  if (S)

    if (shells == S->shells) {

      C_position = 0;

      C_number   = hole != S->hole ||
                   ZA   != S->ZA   ||
                   A    != S->A    ||

        ((length3 (polar) != 0 || length3 (S->polar) != 0) &&

          fabs (dot3 (polar, S->polar) - 1) > 1e-13) ||

/*                 polar [0] != S->polar [0] ||
                   polar [1] != S->polar [1] ||
                   polar [2] != S->polar [2] ||
*/
                   normal [0] != S->normal [0] ||
                   normal [1] != S->normal [1] ||
                   normal [2] != S->normal [2] ||

                   ellipticity != S->ellipticity;

      C_path = parameter [_LEGS_]      != S->parameter [_LEGS_]      ||
               parameter [_RMAX_]      != S->parameter [_RMAX_]      ||
               parameter [_PW_FILTER_] != S->parameter [_PW_FILTER_] ||
               parameter [_CW_FILTER_] != S->parameter [_CW_FILTER_];

      for (shell = 0; shell < shells; shell++) {

        C_number    |= Z [shell] != S->parameter [_Z_ (shell)] ||

                       potential [shell] !=
                                  S->parameter [_potential_ (shell)];

        C_position  |= v [shell][0] != S->parameter [_x_ (shell)] ||
                       v [shell][1] != S->parameter [_y_ (shell)] ||
                       v [shell][2] != S->parameter [_z_ (shell)];

        }
      }


    /* Get the parameter _P_ */

  P = parameter [_P_] != 0;

  parameter [_P_] = 0;


    /* Comparison with the saved parameters:

                       Changed

         C_position  : shells position

         C_number    : the number of shells / atom types
                                            / potentials
                                            / edge
                                            / absorber
                                            / polarisation

         C_path      : the path selection criteria


         P : redo the calculation from scratch */

  scratch = P || C_number;


    /* Rationalise the potential indices (= 0 for the absorber ...) */

  rationalise_FEFF6_potentials ("cFEFF_601_EXAFS_i", ZA, instance,
                                               shells, potential, Z);


    /* Recalculate the paths if ... */

  update_paths = (status == 1 && C_position) || scratch || C_path;


  /* cout (" C_position = %d, C_number = %d, C_path = %d\n"
          " update_paths = %d, scratch = %d, status = %d\n",
            C_position, C_number, C_path, update_paths, scratch, status); */


    /* feff.inp is used if:

                C_position
                update_paths */

  CW = 0;

  if (update_paths)

    CW = parameter [_CW_FILTER_];

  if (C_position || update_paths)

    make_FEFF6 ("cFEFF_601_EXAFS_i",

                 working_path,

                 ZA, instance, hole,
                 polar, normal, ellipticity,
                 parameter [_LEGS_], parameter [_RMAX_],
                 E0, E0i,
                 CW, parameter [_PW_FILTER_],
                 shells, potential, Z, N, v, sigma2,
                 S02);


    /* Get the path lists */

  path_list = O_path_list = NULL;

  if (S)

    O_path_list = S->path;


    /* At least some paths will be recalculated if:

                C_position
                update_paths */

  /* cout (" path_list: %p, O_path_list: %p\n", path_list, O_path_list); */

  if (C_position || update_paths) {

    path_l ** l;


      /* Rename / delete phase files : phase.bin = index */

    manage_FEFF6_files ("cFEFF_601_EXAFS_i", working_path, scratch, index);

    
      /* Check for any events on the queue */

    Poll_event (0);


      /* Recalculate the paths if required */

    if (update_paths) {


        /* Delete the old paths */

      if (O_path_list) {

        destruct_path_l (&(S->path));

        O_path_list = NULL;

        }


        /* Calculate the new set of paths */

      execute_FEFF6 ("cFEFF_601_EXAFS_i", working_path, scratch, 1, 0, 0, 0);


        /* Read the new paths:

                delete paths.dat */

      read_FEFF6_paths (working_path, &path_list, 0, shells, potential, Z, v);

      }


      /* Currently: O_path_list: saved paths / 0
                      path_list: 0 / new set of paths


         Rewrite the paths.dat file to increase the number of
         significant figures in the atomic positions passed to
         GENFMT in FEFF 6.01.

         If the paths have not been recalculated, just get the
         set of paths differing from the saved set. */

    for (path = O_path_list; path; path = path->next)

      path->use = 1;

    for (path = path_list; path; path = path->next)

      path->use = 1;

    if (update_paths)

      rewrite_FEFF6_paths ("cFEFF_601_EXAFS_i", working_path, path_list, 1, v, &count);

    else

      rewrite_FEFF6_paths ("cFEFF_601_EXAFS_i", working_path, O_path_list, 0, v, &count);



      /* Check for any events on the queue */

    Poll_event (0);


      /* Call GENFMT for the (sub)set of paths to recalculate:

         phase.bin + feff.inp + paths.dat -> files.dat + feffxxxx.dat */

      /* Note for FEFF7: the feffxxxx.dat files are now only generated during the
         FF2CHI call with the appropriate PRINT parameter set to 3, so we now
         call FF2CHI as well in the following call */

    if (count) {

      execute_FEFF6 ("cFEFF_601_EXAFS_i", working_path, 0, 0, 1, 1, 0);


        /* Read in the new paths and the exafs data :

                delete paths.dat files.dat feffxxxx.dat */

      read_FEFF6_paths (working_path, &path_list, 1, shells, potential, Z, v);

      }
    else

      destruct_path_l (&path_list);


      /* Delete any paths missing exafs data */

    for (l = &path_list; *l;)

      if ((*l)->points == 0) {

        path = *l;

        *l = path->next;

        free_check (path);

        }
      else

        l = &((*l)->next);

    }


    /* At this point we should have two sets of paths

               the current set                (O_path_list)
               the set of recalculated paths  (  path_list)

       We have to make a path list containing all of the paths.

       */

  for (path = O_path_list; path; path = path->next)

    path->use = 1;

  for (path = path_list; path; path = path->next)

    path->use = 1;


    /* Consolidate the lists into a single set (path_list)

       Where two paths match, the old one precedes
       the new one and is set to be unused         */


  /* cout (" O_path_list:\n");

  print_paths (O_path_list); 

  cout (" path_list:\n");

  print_paths (path_list); */


  if (O_path_list) {

    path_l * l;

    for (O_path = O_path_list, path = path_list;

                  O_path && path; O_path = O_path->next)

      if (O_path->number == path->number) {

        O_path->use = 0;

        l = O_path->next;

        O_path->next = path;

        O_path = path;

        path = path->next;

        O_path->next = l;

        }


      /* Now we have one set only (with duplicates) */

    path_list = O_path_list;

    }


    /* Check for any events on the queue */

  Poll_event (0);


    /* Calculate the exafs / absorber */

  ff6_chi (save && status != 2 ? 0.0 : -1,

           path_list,

           polar,
           E0, E0i,
           S02, parameter [_E_],

           shells, potential,
           Z, N, v, sigma2,

           Nx, k, Chi);


    /* Regenerate the saved path list:

       If the current calculation is to be saved delete the unused paths

       If not, keep all of the original paths */

  if (save) {

    path_l ** l;

    for (l = &path_list; *l; l = &((*l)->next)) {

      path = *l;

      if (path->use == 0) {

        *l = path->next;

        free_check (path);

        }
      }
    }


    /* Delete any multiple paths */

  if (!save && !update_paths) {

    path_l * l;

    for (path = path_list; path; path = path->next)

      if (path->next)

        if (path->number == path->next->number) {

          l = path->next;

          path->next = l->next;

          free_check (l);

          }
    }


    /* Save if:

         (i)  the paths have been recalculated
         (ii) the save has been requested

         */


  if (save || update_paths) {

      /* Save the parameters used to calculate the paths */

    if (S == NULL)

      S = find_FEFF601_P (SP, MAXSAVED, 0);

    S->parameter = realloc_check (S->parameter,
                                    parameter_size * sizeof (double));

    for (i = 0; i < parameter_size; i++)

      S->parameter [i] = parameter [i];

    S->A      = A;
    S->ZA     = ZA;
    S->hole   = hole;
    S->shells = shells;

    S->polar [0] = polar [0];
    S->polar [1] = polar [1];
    S->polar [2] = polar [2];

    S->normal [0] = normal [0];
    S->normal [1] = normal [1];
    S->normal [2] = normal [2];

    S->ellipticity = ellipticity;

    S->key = index + 1;


      /* Save the paths */

    S->path = path_list;


      /* Save the calculated exafs */

    S->k   = realloc_check (S->k, Nx * sizeof (double));
    S->chi = realloc_check (S->chi, Nx * sizeof (double));

    for (i = 0; i < Nx; i++) {

      S->k   [i] = k   [i];
      S->chi [i] = Chi [i];

      }

    S->N = Nx;

    }


    /* Multiply by Q * NA */

  O = Q * NA;

  for (i = 0; i < Nx; i++)

    Chi [i] *= O;


    /* Free the memory */

  free_check (potential);
  free_check (Z);
  free_check (N);
  free_check (v);
  free_check (sigma2);


    /* Return the number of absorbers */

  return O;

  }


  /*********************************************************************}
  {                                                                     }
  {  Calculate exafs                                                    }
  {                                                                     }
  {*********************************************************************/

int cFEFF_601_EXAFS (double parameter [],

                     int ZA, int hole,

                     double polar [], double normal [], 
                     double ellipticity [],

                     double Q [],

                     int N, double k [], double Chi [],

                     int save, int status) {

  /*********************************************************************}
  {                                                                     }
  {  Calculate exafs using FEFF 6.01.                                   }
  {                                                                     }
  {*********************************************************************/

  double * Chi_, P, O, O_count;

  int shell, shells, i;


    /* status < 0: free memory and remove temporary files */

  if (status < 0) {

    cFEFF_601_EXAFS_i (NULL, 0, 0, NULL, NULL, 0, 0, 0, NULL, NULL, 0, status, 0);

    return 1;

    }

  if (N <= 0)

    return 0;


    /* Clear Chi */

  for (i = 0; i < N; i++)

    Chi [i] = 0;


    /* Allocate temporary storage */

  Chi_ = calloc_check (N, sizeof (double));


    /* Calculate the average chi, weighted by absorber occupancy */

  shells = parameter [_shells_] + 1;

  O_count = 0;


    /* Save the P parameter for each absorber */

  P = parameter [_P_];

  for (shell = 0; shell < shells; shell++) {

    parameter [_P_] = P;

    O = cFEFF_601_EXAFS_i (parameter,
                           ZA, hole,
                           polar + 3 * shell, normal + 3 * shell,
                           ellipticity [shell],
                           Q [shell],
                           N, k, Chi_,
                           save, status, shell);

    if (O) {

      O = fabs (O);

      for (i = 0; i < N; i++)

        Chi [i] += Chi_ [i];

      O_count += O;

      }
    }

  parameter [_P_] = 0;

  if (O_count)

    for (i = 0; i < N; i++)

      Chi [i] /= O_count;


    /* Free memory */

  free_check (Chi_);


    /* Return the number of points */

  return N;

  }


  /*********************************************************************}
  {                                                                     }
  {  Calculate the exafs                                                }
  {                                                                     }
  {*********************************************************************/

int FEFF_601_EXAFS (double parameter [],

                    int ZA, int hole,

                    double polar [], double normal [], 
                    double ellipticity [],
                    double Q [],

                    int N, double k [], double Chi [],

                    int status) {

    /* Calculate exafs using FEFF 6.01. */

  return cFEFF_601_EXAFS (parameter, ZA, hole, polar, normal,
                               ellipticity, Q, N, k, Chi, TRUE, status);

  }


  /*********************************************************************}
  {                                                                     }
  {  Calculate derivatives of exafs                                     }
  {                                                                     }
  {*********************************************************************/

int FEFF_601_dE_dx (double parameter [],

                    int ZA, int hole,

                    double polar [], double normal [],
                    double ellipticity [],
                    double Q [],

                    int N, double k0 [], double * dChi [], matrix dv) {

  /*********************************************************************}
  {                                                                     }
  {  calculate derivatives of exafs with respect to the columns of dv   }
  {                                                                     }
  {     dChi     = NULL : no derivatives                                }
  {                                                                     }
  {     dChi [i] = NULL : no derivative w.r.t. dv [..][i]               }
  {                                                                     }
  {     dv [i]   = 0    : the matrix 1 is used                          }
  {                                                                     }
  {  dChi [-1] is dchi / dk                                             }
  {                                                                     }
  {  The vector consists of the model parameter vector followed by      }
  {  the vector ((polarisation, normal, ellipticity, Q)i).              }
  {                                                                     }
  {*********************************************************************/

  int shells, parameter_size, vector_size, dv_unit, i, i_,
              derivative, derivatives, sigma_only;

  double * Chi0, * Chi_1, * vector_, * vector0, d, step, l;


    /* Array sizes */

  shells = parameter [_shells_] + 1;

  parameter_size = _SIZE0_ + _SIZE_ * shells;

  vector_size = parameter_size + shells * 8;

  dv_unit = dv.rows <= 0 || dv.cols <= 0;

  derivatives = dv.cols;

  if (dv_unit)

    derivatives = vector_size;


    /* Allocate space for temporary arrays */

  Chi0       = calloc_check (N, sizeof (double));
  Chi_1      = calloc_check (N, sizeof (double));
  vector0    = calloc_check (vector_size, sizeof (double));
  vector_    = calloc_check (vector_size, sizeof (double));


    /* Save the initial parameter values */

  for (i = 0; i < parameter_size; i++)

    vector0 [i] = parameter [i];

  i_ = parameter_size;

  for (i = 0; i < shells; i++) {

    vector0 [i_++] = polar [0 + i * 3];
    vector0 [i_++] = polar [1 + i * 3];
    vector0 [i_++] = polar [2 + i * 3];

    vector0 [i_++] = normal [0 + i * 3];
    vector0 [i_++] = normal [1 + i * 3];
    vector0 [i_++] = normal [2 + i * 3];

    vector0 [i_++] = ellipticity [i];

    vector0 [i_++] = Q [i];

    }


    /* Calculate chi with the current parameter values */

  N = cFEFF_601_EXAFS (parameter,
                       ZA, hole,
                       polar, normal, ellipticity, Q,
                       N, k0, Chi0,
                       FALSE, 0);


    /* Calculate the derivative with respect to k if required */

  if (dChi [-1]) {

    double * k;


      /* Calculate k shifted by 0.00001 */

    d = 0.00001;

    k = calloc_check (N, sizeof (double));

    for (i = 0; i < N; i++)

      k [i] = k0 [i] + d;


      /* Calculate chi at the new k */

    N = cFEFF_601_EXAFS (parameter,
                         ZA, hole,
                         polar, normal, ellipticity, Q,
                         N, k, dChi [-1],
                         FALSE, 0);

    free_check (k);


      /* Calculate the derivative */

    for (i = 0; i < N; i++)

      dChi [-1][i] = (dChi [-1][i] - Chi0 [i]) / d;

    }


    /* Calculate the other derivatives */

  for (derivative = 0; derivative < derivatives; derivative++)

    if (dChi [derivative] != NULL) {

        /* Get the vector along which to calculate the derivative. */

      for (i = 0; i < vector_size; i++)

        vector_ [i] = 0;

      if (dv_unit)

        vector_ [derivative] = 1;

      else

        for (i = 0; i < vector_size && i < dv.rows; i++)

          vector_ [i] = m_el (dv, i, derivative);


        /* Normalise the vector */

      for (l = i = 0; i < vector_size; i++)

        l += vector_ [i] * vector_ [i];

      l = sqrt (l);

      if (l == 0)

        for (i = 0; i < N; i++)

          dChi [derivative][i] = 0;

      else {

        for (i = 0; i < parameter_size; i++)

          vector_ [i] /= l;


          /* Use the minimum of the step sizes for the nonzero elements */

        sigma_only = 1;

        for (d = MAXDOUBLE, i = 0; i < vector_size; i++)

          if (vector_ [i] != 0) {

            if (!is_sigma2_ (i))

              sigma_only = 0;

            step = 0.00001 * (is_sigma2_ (i) && i < parameter_size

                              ? max (fabs (vector0 [i]), 0.01)

                              : 1)

                                          / fabs (vector_ [i]);

            d = min (d, step);

            }

        sigma_only = 0;

        if (sigma_only)

          d /= 2;


          /* Calculate the parameter vector + delta * vector */

        for (i = 0; i < vector_size; i++)

          vector_ [i] = vector0 [i] + d * vector_ [i];


          /* Copy the polarisation and Q parameters */

        i_ = parameter_size;

        for (i = 0; i < shells; i++) {

          polar [0 + i * 3] = vector_ [i_++];
          polar [1 + i * 3] = vector_ [i_++];
          polar [2 + i * 3] = vector_ [i_++];

          normal [0 + i * 3] = vector_ [i_++];
          normal [1 + i * 3] = vector_ [i_++];
          normal [2 + i * 3] = vector_ [i_++];

          ellipticity [i] = vector_ [i_++];

          Q [i] = vector_ [i_++];

          }


          /* Calculate chi with these parameters */

        N = cFEFF_601_EXAFS (vector_,
                             ZA, hole,
                             polar, normal, ellipticity, Q,
                             N, k0, dChi [derivative],
                             FALSE, 0);

        if (sigma_only) {

            /* Calculate the parameter vector + delta * vector */

          for (i = 0; i < vector_size; i++)

            vector_ [i] = vector0 [i] - d * vector_ [i];

          d *= 2;


            /* Calculate chi with these parameters */

          N = cFEFF_601_EXAFS (vector_,
                             ZA, hole,
                             polar, normal, ellipticity, Q,
                             N, k0, Chi_1,
                             FALSE, 0);

          }


          /* Calculate the derivative */

        if (!sigma_only)

          for (i = 0; i < N; i++)

            dChi [derivative][i] = (dChi [derivative][i] - Chi0 [i]) / d * l;

        else

          for (i = 0; i < N; i++)

            dChi [derivative][i] = (dChi [derivative][i] - Chi_1 [i]) / d * l;

        }
      }


    /* Restore the parameters */

  for (i = 0; i < parameter_size; i++)

    parameter [i] = vector0 [i];

  i_ = parameter_size;

  for (i = 0; i < shells; i++) {

    polar [0 + i * 3] = vector0 [i_++];
    polar [1 + i * 3] = vector0 [i_++];
    polar [2 + i * 3] = vector0 [i_++];

    normal [0 + i * 3] = vector0 [i_++];
    normal [1 + i * 3] = vector0 [i_++];
    normal [2 + i * 3] = vector0 [i_++];

    ellipticity [i] = vector0 [i_++];

    Q [i] = vector0 [i_++];

    }


    /* Free memory */

  free_check (Chi0);
  free_check (Chi_1);
  free_check (vector0);
  free_check (vector_);


    /* Return the number of points */

  return N;

  }


  /*********************************************************************}
  {                                                                     }
  {  Each exafs calculation uses two sets of parameters:                }
  {                                                                     }
  {    (1) model:                                                       }
  {                                                                     }
  {               LEGS, RMAX, PW, CW, E, P, SHELLS                      }
  {                                                                     }
  {             [ E0, E0i, S02, POT, Z, N, x, y, z, sigma2 ]            }
  {                                                                     }
  {    (2) The data set parameters                                      }
  {                                                                     }
  {               ZA, hole                                              }
  {                                                                     }
  {             [ polar, normal, ellipticity, Q ]                       }
  {                                                                     }
  {  Together these make up the parameter vector of size                }
  {                                                                     }
  {               9 + SHELLS * 18                                       }
  {                                                                     }
  {*********************************************************************/

int FEFF_601_calculate (vector model,

                        int ZA, int hole,

                        int N_Q, double polar [], double normal [],
                                 double ellipticity [], double Q [],

                        int N, double k [], double chi [],

                        double * dchidv [], matrix dv,

                        int status) {

  /*********************************************************************}
  {                                                                     }
  {  This function returns :                                            }
  {                                                                     }
  {     (1) exafs                                                       }
  {                                                                     }
  {     (2) derivatives of exafs with respect to the columns of dv      }
  {                                                                     }
  {     chi        = NULL : no exafs                                    }
  {                                                                     }
  {     dchidv     = NULL : no derivatives                              }
  {                                                                     }
  {     dchidv [i] = NULL : no derivative w.r.t. dv [..][i]             }
  {                                                                     }
  {     dv [i]     = 0    : the matrix 1 is used.                       }
  {                                                                     }
  {  dchidv [-1] is dchi / dk                                           }
  {                                                                     }
  {  status: -1: clean up any temporary files.                          }
  {                                                                     }
  {           0: normal call.                                           }
  {                                                                     }
  {           1: recalculate the paths.                                 }
  {                                                                     }
  {           2: don't print to the console.                            }
  {                                                                     }
  {*********************************************************************/

  double * Q_;

  int shell, shells;

  if (status < 0) {

    FEFF_601_EXAFS (NULL, 0, 0, NULL, NULL, NULL, NULL, 0, NULL, NULL, status);

    return 1;

    }

  if (!working_path) {

    working_path = FEFF6_directory ();

    if (!working_path)

      cerror ("Cannot create a directory to execute FEFF6");

    }

  if (N <= 0)

    return 0;


    /* Check that polar [], normal [], ellipticity [] and Q [] cover all of
       the shells */

  Q_ = NULL;

  shells = v_el (model, _shells_) + 1;

  if (N_Q < shells) {

      /* Allocate space to hold the expanded arrays */

    Q_ = calloc_check (8 * shells, sizeof (double));


      /* Put in the defaults */

    Q_ [0] = Q_ [1] = Q_ [2] = 0;

    Q_ [3 * shells + 0] = Q_ [3 * shells + 1] = Q_ [3 * shells + 2] = 0;

    Q_ [6 * shells] = 0;

    Q_ [7 * shells] = 1;


      /* Copy the arrays */

    for (shell = 0; shell < N_Q; shell++) {

      Q_ [3 * shell + 0] = polar [3 * shell + 0];
      Q_ [3 * shell + 1] = polar [3 * shell + 1];
      Q_ [3 * shell + 2] = polar [3 * shell + 2];

      Q_ [3 * shells + 3 * shell + 0] = normal [3 * shell + 0];
      Q_ [3 * shells + 3 * shell + 1] = normal [3 * shell + 1];
      Q_ [3 * shells + 3 * shell + 2] = normal [3 * shell + 2];

      Q_ [6 * shells + shell] = ellipticity [shell];

      Q_ [7 * shells + shell] = Q [shell];

      }

    polar = Q_;

    normal = Q_ + 3 * shells;

    ellipticity = Q_ + 6 * shells;

    Q = Q_ + 7 * shells;


      /* Extend the arrays */

    for (shell = N_Q; shell < shells; shell++) {

      polar [3 * shell + 0] = polar [3 * (shell - 1) + 0];
      polar [3 * shell + 1] = polar [3 * (shell - 1) + 1];
      polar [3 * shell + 2] = polar [3 * (shell - 1) + 2];

      normal [3 * shell + 0] = normal [3 * (shell - 1) + 0];
      normal [3 * shell + 1] = normal [3 * (shell - 1) + 1];
      normal [3 * shell + 2] = normal [3 * (shell - 1) + 2];

      ellipticity [shell] = ellipticity [shell - 1];

      Q [shell] = Q [shell - 1];

      }
    }


    /* Calculate the exafs */

  if (chi)

    if (!FEFF_601_EXAFS (&v_el (model, 0),
                         ZA, hole,
                         polar, normal, ellipticity, Q,
                         N, k, chi, status)) {

      cwarn ("FEFF_601_calculate : cannot calculate chi");

      N = -1;

      }


    /* Calculate the derivatives */

  if (dchidv && N > 0)

    if (!FEFF_601_dE_dx (&v_el (model, 0),
                       ZA, hole,
                       polar, normal, ellipticity, Q,
                       N, k, dchidv, dv)) {

      cwarn ("FEFF_601_calculate : cannot calculate dchi / dvector");

      N = -1;

      }

  free_check (Q_);


    /* Return the number of points */

  return N;

  }


#endif /* __FEFF_601_C_C */
