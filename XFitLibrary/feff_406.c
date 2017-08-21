/***********************************************************************}
{                                                                       }
{  Single-scattering FEFF exafs calculation (FEFF 4.06) for xfit        }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__FEFF_406_C_C__)

#define __FEFF_406_C_C__

#include "feff_406.h"

#include "console.h"

#include "misc_com.h"

#include "event.h"



#define _SIZE_      8
#define _SIZE0_     1
#define _shells_    0
#define _E0_(i)     (_SIZE0_ + _SIZE_ * ((i)) + 0)
#define _S02_(i)    (_SIZE0_ + _SIZE_ * ((i)) + 1)
#define _Z_(i)      (_SIZE0_ + _SIZE_ * ((i)) + 2)
#define _N_(i)      (_SIZE0_ + _SIZE_ * ((i)) + 3)
#define _x_(i)      (_SIZE0_ + _SIZE_ * ((i)) + 4)
#define _y_(i)      (_SIZE0_ + _SIZE_ * ((i)) + 5)
#define _z_(i)      (_SIZE0_ + _SIZE_ * ((i)) + 6)
#define _sigma2_(i) (_SIZE0_ + _SIZE_ * ((i)) + 7)

#define _x_polar_(S,i)  (_SIZE0_ + _SIZE_ * ((S)) + 8 * ((i)) + 0)
#define _y_polar_(S,i)  (_SIZE0_ + _SIZE_ * ((S)) + 8 * ((i)) + 1)
#define _z_polar_(S,i)  (_SIZE0_ + _SIZE_ * ((S)) + 8 * ((i)) + 2)
#define _Q_(S,i)        (_SIZE0_ + _SIZE_ * ((S)) + 8 * ((i)) + 7)


  /* To match the conventional use of the "shells" parameters:

     "shells" = maximum shell index.

     Each shell has the following parameters:

                   default

        E0      :  0
        S02     :  1 : shell 0 / 0 : shell > 0
        Z       :  0
        x       :  0
        y       :  0
        z       :  0
        sigma2  :  0

     The shell will not be used as an absorber if S02 is 0 and E0 is 0.

     E0 -> E00, S02 ->S020, CENTRAL -> SHELL0

     */

#define M MODEL
#define I IDENTIFIER
#define V VECTOR_ID

#define SC NORMAL
#define AT ATOM_t
#define VE VECTOR
#define PO PVECTOR

parameter_type FEFF_406_parameter_name [] =

  { { "SHELLS",     _shells_,     0,      0, 0, M, 0, I, SC, 0, 0 },

    { "E0(%d)",     _E0_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 0 },
    { "E0%d",       _E0_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 0 },
    { "E0",         _E0_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "S02(%d)",    _S02_ (0),    0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "S02%d",      _S02_ (0),    0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "S02",        _S02_ (0),    0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "AF",         _S02_ (0),    0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "AFAC",       _S02_ (0),    0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "SHELL(%d)",  _Z_ (0),      0, _SIZE_, 0, M, 0, I, AT, 0, 0 },
    { "SHELL%d",    _Z_ (0),      0, _SIZE_, 0, M, 0, I, AT, 0, 0 },
    { "CENTRAL",    _Z_ (0),      0, _SIZE_, 0, M, 0, I, AT, 0, 0 },
    { "(Z)%d",      _Z_ (0),      0, _SIZE_, 0, M, 0, I, AT, 0, 0 },
    { "N(%d)",      _N_ (0),      0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "N%d",        _N_ (0),      0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "x(%d)",      _x_ (0),      0, _SIZE_, 1, M, 0, I, VE, 0, 1 },
    { "x%d",        _x_ (0),      0, _SIZE_, 1, M, 0, I, VE, 0, 1 },
    { "y(%d)",      _x_ (0),      0, _SIZE_, 2, M, 0, I, VE, 0, 1 },
    { "y%d",        _x_ (0),      0, _SIZE_, 2, M, 0, I, VE, 0, 1 },
    { "z(%d)",      _x_ (0),      0, _SIZE_, 3, M, 0, I, VE, 0, 1 },
    { "z%d",        _x_ (0),      0, _SIZE_, 3, M, 0, I, VE, 0, 1 },
    { "v(%d)",      _x_ (0),      0, _SIZE_, 0, M, 0, V, VE, 0, 1 },
    { "v%d",        _x_ (0),      0, _SIZE_, 0, M, 0, V, VE, 0, 1 },
    { "r(%d)",      _x_ (0),      0, _SIZE_, 1, M, 0, I, PO, 0, 1 },
    { "r%d",        _x_ (0),      0, _SIZE_, 1, M, 0, I, PO, 0, 1 },
    { "T(%d)",      _x_ (0),      0, _SIZE_, 2, M, 0, I, PO, 0, 1 },
    { "T%d",        _x_ (0),      0, _SIZE_, 2, M, 0, I, PO, 0, 1 },
    { "P(%d)",      _x_ (0),      0, _SIZE_, 3, M, 0, I, PO, 0, 1 },
    { "Q(%d)",      _x_ (0),      0, _SIZE_, 0, M, 0, V, PO, 0, 1 },
    { "Q%d",        _x_ (0),      0, _SIZE_, 0, M, 0, V, PO, 0, 1 },
    { "sigma2(%d)", _sigma2_ (0), 0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "sigma2%d",   _sigma2_ (0), 0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "s(%d)",      _sigma2_ (0), 0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "s%d",        _sigma2_ (0), 0, _SIZE_, 0, M, 0, I, SC, 0, 1 },

    { 0 } };

#undef M
#undef I
#undef V

#undef SC
#undef AT
#undef VE
#undef PO


  /*******************************************}
  {                                           }
  {  Rationalise the model parameter vectors  }
  {                                           }
  {*******************************************/

int FEFF_406_rationalise (parameter_set * set) {

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

    shell_size = (i0 - _SIZE0_ - 1) / _SIZE_;

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

  for (shell = shells_0 + 1; shell < shells; shell++) {

    if (_N_ (shell) != i0)

      v_el (*model, _N_ (shell)) = v_el (*model_0, _N_ (shell)) = 1;

    if (_S02_ (shell) != i0)

      v_el (*model, _S02_ (shell)) =

      v_el (*model_0, _S02_ (shell)) = shell == 0;

    }

  for (shell = 0; shell < shells; shell++)

    v_el (*model, _Z_ (shell)) = floor (v_el (*model, _Z_ (shell)) + 0.5);

  shells--;

  v_el (*model, _shells_) =

  v_el (*model_0, _shells_) = shells_0 = shells;

  return shells;

  }

#define FEFF_STACK_SIZE 32

#define MAP_SIZE 100


  /*********************************************************************}
  {                                                                     }
  {  Find the set of three FEFF 4.06 data files with absorber Za,       }
  {  core hole type hole and scatterer Zs most closely bounding the     }
  {  distance R.                                                        }
  {                                                                     }
  {  The data files have name format:                                   }
  {                                                                     }
  {           <absorber>_<hole>_<scatterer>_<distance>.406              }
  {                                                                     }
  {  <absorber> & <scatterer> are atomic symbols                        }                               }
  {                                                                     }
  {  <hole> is omitted if the edge is K                                 }
  {                                                                     }
  {  <distance> = distance * 10.  At least 2 digits are used            }
  {                                                                     }
  {    eg:  Cu_N_19.406                                                 }
  {                                                                     }
  {         Ru_L1_O_22.406                                              }
  {                                                                     }
  {*********************************************************************/

int find_FEFF_406_data_set (FEFF_406_data * stack [],

                            int ZA, int hole, int ZS, double R) {


  static FSR_node * FSR_file_map = NULL;

  static char * opath = NULL;



  char * path;

  FEFF_406_data * FEFF0;

  FILE * file;

  char filename [256], * SA, * SS, * found_file;

  int found, Ri0, Ri, c, x, i, i0, S, new_path;

  double R_S;


    /* Check the parameters */

  if (hole < 1)

    hole = 1;

  Ri0 = floor (10 * fabs (R) + 0.5);

  SA = atom_symbol (ZA);
  SS = atom_symbol (ZS);


    /* Get the search path */

  path = get_symbol ("data directory");


    /* Has it changed? */
    
  new_path = 0;

  if (path) {

    if (!opath)

      new_path = 1;

    else

      new_path = strcmp (path, opath);

    }


    /* Changed */

  if (new_path) {
      
    if (opath)

      free (opath);

    opath = strdup (path);


      /* Clear the file map */

    destruct_FSR_map (&FSR_file_map);


      /* Clear the data stack */

    for (c = 0; c < FEFF_STACK_SIZE; c++)

      if (stack [c]) {

        if (stack [c]->line != NULL) {

          free (stack [c]->line [0]);

          free (stack [c]->line);

          }

        free (stack [c]);

        stack [c] = NULL;

        }
    }


    /* Search for the closest 3 data sets bounding R */

  for (found = S = x = 0; found < 3 && x < 3; S++) {

    Ri = Ri0 + ((S % 2) ? (S + 1) / 2 : -S / 2);

    if (Ri < 0)

      x |= 1;

    if (Ri >= MAP_SIZE)

      x |= 2;

    if (Ri < 0 || Ri >= MAP_SIZE)

      continue;


      /* Search for data corresponding to distance R_S */

    R_S = 0.1 * Ri;


      /* Look for the data on the stack */

    for (c = FALSE, i0 = 0; i0 < FEFF_STACK_SIZE && !c; i0++)

      if (stack [i0])

        if (stack [i0]->ZA   == ZA   &&
            stack [i0]->ZS   == ZS   &&
            stack [i0]->hole == hole &&

            fabs (stack [i0]->R - R_S) < 0.05) {

            /* Move the entry onto the bottom of the stack */

          for (FEFF0 = stack [i0], i = i0; i >= 1; i--)

            stack [i] = stack [i - 1];

          stack [0] = FEFF0;

          found++;

          c = TRUE;

          }

    if (c)

      continue;


      /* If the data isn't on the stack, look for the file.

         Get the file name */

    if (hole == 1)

      sprintf (filename, "%s_%s_%02d.406", SA, SS, Ri);

    else

#if defined (SHORT_NAMES)

      sprintf (filename, "%sL%1d%s%02d.406", SA, hole - 1, SS, Ri);

#else

      sprintf (filename, "%s_L%1d_%s_%02d.406", SA, hole - 1, SS, Ri);

#endif


    file = NULL;


      /* Look for the file */

    found_file = find_file (filename);

    if (found_file)

      file = FSR_open_file (found_file,
                            ZA * 1000. + hole * 100. + ZS,
                            Ri,
                            &FSR_file_map, MAP_SIZE, new_path);


    if (file) {

        /* Replace the oldest stack entry */

      if ((FEFF0 = stack [FEFF_STACK_SIZE - 1]) == NULL)

        FEFF0 = calloc (1, sizeof (FEFF_406_data));

      read_FEFF_406 (file, found_file, FEFF0);

      fclose (file);

      FEFF0->hole = hole;


        /* Shift the stack up and put the new entry on the bottom */

      for (i = FEFF_STACK_SIZE - 1; i >= 1; i--)

        stack [i] = stack [i - 1];

      stack [0] = FEFF0;

      found++;

      }
    }


    /* Sort the data sets in order of increasing R */

  for (i0 = 0; i0 <= found - 2; i0++)

    for (i = i0 + 1; i <= found - 1; i++)

      if (stack [i0]->R > stack [i]->R) {

        FEFF0      = stack [i];
        stack [i]  = stack [i0];
        stack [i0] = FEFF0;

        }


    /* If no data sets were found ... */

  if (!found) {

    if (hole == 1)

      cwarn ("cannot find the FEFF 4.06 data files : %s_%s_##.406", SA, SS);

    else

      cwarn ("cannot find the FEFF 4.06 data files : %s_L%1d_%s_##.406", 
                                                     SA, hole - 1, SS);

    }


    /* Return the number of data sets found */

  return found;

  }


  /*********************************************************************}
  {                                                                     }
  {  Calculate the exafs                                                }
  {                                                                     }
  {*********************************************************************/

int FEFF_406_EXAFS (FEFF_406_data * stack [],

                    double parameter [],

                    int ZA, int hole,

                    double polar [], double Q [],

                    int N, double k [], double Chi []) {

    /* Calculates the exafs

       stack is the FEFF data stack */

  double * Chi_,

           E0, S02, NS, NA, vS [3], vA [3], sigma2S, sigma2A,

           absorbers, NP [2], A_NA;

  int      i, shell, shellA, shells, shellsA, N_FEFF, ZS;

  FEFF_406_data *FEFF [3];


    /* Check for any events on the queue */

  Poll_event (0);


    /* Clear the array */

  for (i = 0; i < N; i++)

    Chi [i] = 0;


    /* Check the parameters */

  if (ZA <= 0)

    return 0;

  shells = parameter [_shells_];

  shells++;

  if (shells < 2)

    return N;


    /* Allocate space for the array to hold chi from each shell */

  Chi_ = calloc_check_ (N, sizeof (double), "FEFF_406_EXAFS");

  if (!Chi_)

    return 0;

  shellsA = shells;


    /* For each absorbing atom, add the exafs contribution */

  for (shellA = absorbers = 0; shellA < shellsA; shellA++) {

    E0  = parameter [_E0_  (shellA)];
    S02 = parameter [_S02_ (shellA)];

    NA  = parameter [_N_ (shellA)];

    A_NA = fabs (NA);

    vA [0] = parameter [_x_ (shellA)];
    vA [1] = parameter [_y_ (shellA)];
    vA [2] = parameter [_z_ (shellA)];

    sigma2A = parameter [_sigma2_ (shellA)];

    if (floor (parameter [_Z_ (shellA)] + 0.5) == ZA && (S02 != 0 || E0 != 0)) {

        /* This is an absorbing atom.

           For each scattering atom, add the exafs contribution */

      for (shell = 0; shell < shells; shell++) {

          /* Get the scatterer parameters and the FEFF 4.06 data sets */

        ZS = floor (parameter [_Z_ (shell)] + 0.5);

        NS = parameter [_N_ (shell)];

        vS [0] = parameter [_x_ (shell)];
        vS [1] = parameter [_y_ (shell)];
        vS [2] = parameter [_z_ (shell)];

        sigma2S = parameter [_sigma2_ (shell)];

        if (shell != shellA) {

          vS [0] -= vA [0];
          vS [1] -= vA [1];
          vS [2] -= vA [2];

          sigma2S += sigma2A;

          N_FEFF = find_FEFF_406_data_set (stack, ZA, hole, ZS,
                                                      length3 (vS));

          FEFF [0] = FEFF [1] = FEFF [2] = NULL;

          for (i = 0; i < N_FEFF; i++)

            FEFF [i] = stack [i];


            /* Get the occupancy factor (per absorber) */

          NP [0] = NA;
          NP [1] = NS;

          NS = N_factor (2, NP);

          if (N_FEFF)

            i = calculate_FEFF_406_EXAFS (FEFF,
                                          polar + shellA * 3, Q [shellA],
                                          E0, S02,
                                          NS, vS, sigma2S,
                                          N, k, Chi_);

          if (N_FEFF == 0 || i == 0) {

            free (Chi_);

            return 0;

            }


            /* Add the contribution from this pair */

          for (i--; i >= 0; i--)

            Chi [i] += A_NA * Chi_ [i];

          }
        }

      absorbers += fabs (Q [shellA] * NA);

      }
    }


    /* Normalise the exafs to the number of absorbers */

  if (absorbers != 0)

    for (i = 0; i < N; i++)

      Chi [i] /= absorbers;


    /* Free memory */

  free_check (Chi_);


    /* Return the number of points */

  return N;

  }


  /*********************************************************************}
  {                                                                     }
  {  Calculate derivatives of exafs                                     }
  {                                                                     }
  {*********************************************************************/

int FEFF_406_dE_dx (FEFF_406_data * stack [],

                    double parameter [],

                    int ZA, int hole,

                    double polar [], double Q [],

                    int N, double k [], double dChi [], int X) {

  /*********************************************************************}
  {                                                                     }
  {  This function returns the derivatives of exafs with respect to     }
  {  the parameter X                                                    }
  {                                                                     }
  {  X = -1: k                                                          }
  {                                                                     }
  {  The parameters in the vector                                       }
  {                                                                     }
  {    ((polarisation, normal, ellipticity, Q)i)                        }
  {                                                                     }
  {  follow the model parameters                                        }
  {                                                                     }
  {*********************************************************************/

  double * dChi_, E0, S02,

           NS, NA, A_NA, dA_dN, S_QN_A,

           dNS,

           vS [3], vA [3], sigma2S, sigma2A, absorbers,

           NP [2], dNP [2];

  int      i, i0, shell, shellA, shells, shellsA, N_FEFF, ZS, x, S,

           X_QN_A;

  FEFF_406_data * FEFF [3];


    /* Check for any events on the queue */

  Poll_event (0);


    /* Clear the array */

  for (i = 0; i < N; i++)

    dChi [i] = 0.0;


    /* Check the parameters */

  if (ZA <= 0)

    return 0;

  shells = parameter [_shells_];

  shells++;

  if (shells < 2)

    return N;


    /* Allocate space for the temporary array */

  dChi_ = calloc_check_ (N, sizeof (double), "FEFF_406_dE_dx");

  if (!dChi_)

    return 0;

  shellsA = shells;

  X_QN_A = 0;


    /* For each absorbing atom, add the contribution */

  for (shellA = absorbers = 0; shellA < shellsA; shellA++) {

    E0  = parameter [_E0_  (shellA)];
    S02 = parameter [_S02_ (shellA)];

    NA  = parameter [_N_ (shellA)];

    A_NA  = fabs (NA);

    vA [0] = parameter [_x_ (shellA)];
    vA [1] = parameter [_y_ (shellA)];
    vA [2] = parameter [_z_ (shellA)];

    sigma2A = parameter [_sigma2_ (shellA)];

    if (floor (parameter [_Z_ (shellA)] + 0.5) == ZA && (S02 != 0 || E0 != 0)) {

        /* This is an absorbing atom */

      if (X == _N_ (shellA) || X == _Q_ (shells, shellA)) {

        X_QN_A = 1;


          /* S_QN_A = d |N * Q| / dX

                    = d  NQ / dx : N > 0  Q > 0  /  N < 0  Q < 0
                    = d -NQ / dx : N < 0  Q > 0  /  N > 0  Q < 0 */

        S_QN_A = NA;

        if (X == _N_ (shellA))

          S_QN_A = Q [shellA];

        if (NA * Q [shellA] < 0)

          S_QN_A = -S_QN_A;

        }


        /* For each scattering atom, add the exafs contribution */

      for (shell = 0; shell < shells; shell++) {

          /* Get the scatterer parameters and the FEFF 4.06 data sets */

        ZS = floor (parameter [_Z_ (shell)] + 0.5);

        NS = parameter [_N_ (shell)];

        vS [0] = parameter [_x_ (shell)];
        vS [1] = parameter [_y_ (shell)];
        vS [2] = parameter [_z_ (shell)];

        sigma2S = parameter [_sigma2_ (shell)];

          /* The parameters affecting this pair are:

                 X                 x

                k               : -1

                N      (shellA) : <internal>
                N      (shell)  : <internal>

                   -> N (path)  :  2

                E0     (shellA) :  0
                S02    (shellA) :  1
                x      (shellA) :  3  *
                y      (shellA) :  4  *
                z      (shellA) :  5  *
                sigma2 (shellA) :  6

                x      (shell)  :  3
                y      (shell)  :  4
                z      (shell)  :  5
                sigma2 (shell)  :  6

                * : -x (shell) ...

                x (polar) (shellA) : 7
                y (polar) (shellA) : 8
                z (polar) (shellA) : 9

                Q         (shellA) : 10

                */

        if ((X == -1 || X == _E0_      (shellA) || X == _N_      (shell)
                     || X == _S02_     (shellA) || X == _x_      (shell)
                     || X == _sigma2_  (shellA) || X == _y_      (shell)
                     || X == _N_       (shellA) || X == _z_      (shell)
                     || X == _x_       (shellA) || X == _sigma2_ (shell)
                     || X == _y_       (shellA)
                     || X == _z_       (shellA)
                     || X == _x_polar_ (shells, shellA)
                     || X == _y_polar_ (shells, shellA)
                     || X == _z_polar_ (shells, shellA)
                     || X == _Q_       (shells, shellA))

             && shell != shellA) {

          if (X == -1)

            x = -1;

          if (X == _N_ (shell) || X == _N_ (shellA)) {

            x = 2;

            dNP [0] = X == _N_ (shellA);
            dNP [1] = X == _N_ (shell);

            }

          if (X == _E0_ (shellA) || X == _S02_ (shellA))

            x = X - _E0_ (shellA);

          if (X >= _x_ (shellA) && X <= _sigma2_ (shellA))

            x = X - _x_ (shellA) + 3;

          if (X >= _x_ (shell) && X <= _sigma2_ (shell))

            x = X - _x_ (shell) + 3;

          if (X >= _x_polar_ (shells, shellA) &&
              X <= _z_polar_ (shells, shellA))

            x = X - _x_polar_ (shells, shellA) + 7;

          if (X == _Q_ (shells, shellA))

            x = 10;

          S = 1;

          if (X >= _x_ (shellA) && X <= _z_ (shellA))

            S = -1;

          vS [0] -= vA [0];
          vS [1] -= vA [1];
          vS [2] -= vA [2];

          sigma2S += sigma2A;

          N_FEFF = find_FEFF_406_data_set (stack, ZA, hole, ZS,
                                                      length3 (vS));

          FEFF [0] = FEFF [1] = FEFF [2] = NULL;

          for (i = 0; i < N_FEFF; i++)

            FEFF [i] = stack [i];

            /* Get the occupancy factor (per absorber) */

          NP [0] = NA;
          NP [1] = NS;

          NS = N_factor (2, NP);

            /* Get dNS / dN */

          dNS = 0;

          if (x == 2) {

            NP [0] -= 0.00001 * dNP [0];
            NP [1] -= 0.00001 * dNP [1];

            dNS = N_factor (2, NP);

            NP [0] += 0.00002 * dNP [0];
            NP [1] += 0.00002 * dNP [1];

            dNS = (N_factor (2, NP) - dNS) / 0.00002;

            }

          if (N_FEFF)

            i = calculate_FEFF_406_dEXAFS_dx (FEFF,
                                              polar + shellA * 3, Q [shellA],
                                              E0, S02,
                                              NS, vS, sigma2S,
                                              N, k, dChi_,
                                              x);

          if (N_FEFF == 0 || i == 0) {

            free (dChi_);

            return 0;

            }


            /* Chi / absorber = NS * Chi0

               dChi / dNS / absorber = Chi0

                 d(A * Chi) / dN = dA / dN * Chi +

                                   A * dChi / dNS * dNS / dN

                                 = dA / dN * NS * Chi0 +

                                   A * Chi0 * dNS / dN */

          if (x == 2) {

            i0 = i;

            if (dNP [0]) {

                /* dA / dN <> 0 */

              dA_dN = 1;

              if (NA < 0)

                dA_dN = -1;

              for (i = 0; i < i0; i++)

                dChi [i] += dA_dN * NS * dChi_ [i];

              }

            for (i = 0; i < i0; i++)

              dChi [i] += A_NA * dChi_ [i] * dNS;

            i = 0;

            }


            /* Add the derivative weighted by the number of absorbers */

          for (i--; i >= 0; i--)

            dChi [i] += A_NA * S * dChi_ [i];

          }
        }

      absorbers += fabs (NA * Q [shellA]);

      }
    }



    /* Normalise the exafs to the number of absorbers */

  if (absorbers != 0) {

      /* If X is N or Q for an absorbing atom

         dE / dX = d (sum (E/A * A) / sum (A)) / dX

                 = d (sum (E/A * A)) / dX / sum (A) -

                   sum (E/A * A) / sum (A) *

                                 d sum (A) / dX / sum (A)*/

    if (X_QN_A) {

      i0 = FEFF_406_EXAFS (stack,
                           parameter,
                           ZA, hole,
                           polar, Q,
                           N, k, dChi_);

      for (i = 0; i < i0; i++)

        dChi [i] -= dChi_ [i] * S_QN_A;

      }

    for (i = 0; i < N; i++)

      dChi [i] /= absorbers;

    }


    /* Free memory */

  free_check (dChi_);


    /* Return the number of points */

  return N;

  }


  /*********************************************************************}
  {                                                                     }
  {  Each exafs calculation uses two sets of parameters:                }
  {                                                                     }
  {    (1) model:                                                       }
  {                                                                     }
  {               SHELLS                                                }
  {                                                                     }
  {             [ E0, S02, Z, N, x, y, z, sigma2 ]                      }
  {                                                                     }
  {    (2) The data set parameters                                      }
  {                                                                     }
  {               ZA, hole                                              }
  {                                                                     }
  {             [ polar, normal, ellipticity, Q ]                       }
  {                                                                     }
  {  Together these make up the parameter vector of size                }
  {                                                                     }
  {               1 + SHELLS * 16                                       }
  {                                                                     }
  {*********************************************************************/

int FEFF_406_calculate (vector model,

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
  {*********************************************************************/

  int error, parameter, parameters,
             derivative, derivatives, first, dv_unit, i;

  double m, * dchi;

  static FEFF_406_data * FEFF_stack [FEFF_STACK_SIZE] = { 0 };

  double * Q_;

  int shell, shells;


    /* Check the status */

  if (status < 0)

    return 1;


    /* Check the parameters */

  if (N <= 0)

    return 0;


    /* Check that polar [] and Q [] cover all of the shells */

  Q_ = NULL;

  shells = v_el (model, _shells_) + 1;

  if (N_Q < shells) {

      /* Allocate space to hold the expanded arrays */

    Q_ = calloc_check (4 * shells, sizeof (double));


      /* Put in the defaults */

    Q_ [0] = Q_ [1] = Q_ [2] = 0;

    Q_ [3 * shells] = 1;


      /* Copy the arrays */

    for (shell = 0; shell < N_Q; shell++) {

      Q_ [3 * shell + 0] = polar [3 * shell + 0];
      Q_ [3 * shell + 1] = polar [3 * shell + 1];
      Q_ [3 * shell + 2] = polar [3 * shell + 2];

      Q_ [3 * shells + shell] = Q [shell];

      }

    polar = Q_;

    Q = Q_ + 3 * shells;


      /* Extend the arrays */

    for (shell = N_Q; shell < shells; shell++) {

      polar [3 * shell + 0] = polar [3 * (shell - 1) + 0];
      polar [3 * shell + 1] = polar [3 * (shell - 1) + 1];
      polar [3 * shell + 2] = polar [3 * (shell - 1) + 2];

      Q [shell] = Q [shell - 1];

      }
    }


    /* calculate the exafs */

  if (chi)

    if (!FEFF_406_EXAFS (FEFF_stack,
                         &v_el (model, 0),
                         ZA, hole,
                         polar, Q,
                         N, k, chi)) {

      cwarn ("FEFF_406_calculate : cannot calculate chi");

      N = -1;

      }


    /* Calculate the derivatives */

  if (dchidv && N > 0) {

      /* Allocate space for the temporary array */

    dchi = calloc_check (N, sizeof (double));

    error = 0;


      /* Calculate dchi / dk */

    if (dchidv [-1]) {

      if (!FEFF_406_dE_dx (FEFF_stack,
                         &v_el (model, 0),
                         ZA, hole,
                         polar, Q,
                         N, k, dchidv [-1], -1))

        error = -1;

      }

    dv_unit = dv.rows <= 0 || dv.cols <= 0;

    if (dv_unit)

      derivatives = parameters  = model.rows;

    else {

      derivatives = dv.cols;

      parameters = min (dv.rows, model.rows + shells * 8);

      }


      /* Go through the parameters, adding the derivatives w.r.t
         the model parameters to the derivatives w.r.t. the vectors */

    for (parameter = 0; parameter < parameters && !error; parameter++) {

      first = 1;

      for (derivative = 0; derivative < derivatives; derivative++) {

          /* Get the matrix element dv [parameter][derivative] */

        if (dv_unit)

          m = (parameter == derivative);

        else

          m = m_el (dv, parameter, derivative);


          /* If m <> 0, dchi / dparameter contributes to dchi / dv */

        if (m != 0 && dchidv [derivative]) {

            /* If necessary, calculate dchi / dparameter */

          if (first)

            if (!FEFF_406_dE_dx (FEFF_stack,
                               &v_el (model, 0),
                               ZA, hole,
                               polar, Q,
                               N, k, dchi, parameter)) {

              error = parameter + 1;

              break;

              }

          first = 0;


            /* Add the contribution to dchi / dv from dchi / dparameter */

          for (i = 0; i < N; i++)

            dchidv [derivative][i] += m * dchi [i];

          }
        }
      }


      /* Report any errors */

    if (error) {

      cwarn ("FEFF_406_calculate : cannot calculate dchi / dparameter [%1d]",
                         error > 0 ? error - 1 : error);

      N = -1;

      }


      /* Free the memory */

    free_check (dchi);

    }


    /* Return the number of points */

  return N;

  }


#endif /* __FEFF_406_C_C__ */
