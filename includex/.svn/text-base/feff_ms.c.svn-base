/***********************************************************************}
{                                                                       }
{  This file contains the routine common to FEFF5 and FEFF6.            }
{                                                                       }
{  Copyright (C) 1994  Paul J. Ellis  University of Sydney              }
{                                                                       }
{***********************************************************************/

#if !defined (__FEFF_MS_C)

#define __FEFF_MS_C

#include "feff_ms.h"

#include "console.h"

typedef double _v3_ [3];

  /*********************************************************************}
  {                                                                     }
  {  The N (occupancy) factor used for the scattering path              }
  {  starting on atom 0 is:                                             }
  {                                                                     }
  {        max (1, |N [...]|) * min (1, |N [...]|) * sign (N [...])     }
  {                                                                     }
  {    where N [...] is the list of the N factors of the atoms.         }
  {                                                                     }
  {    All N are divided to make N [0] = 1.                             }
  {                                                                     }
  {    The maximum value returned is 1000.                              }
  {                                                                     }
  {*********************************************************************/

double N_factor (int legs, double N []) {

  int l;

  double Nmin, Nmax, Nsign;

  Nmin = MAXDOUBLE;
  Nmax = 0;

  Nsign = 1;

  for (l = 0; l < legs; l++) {

    Nmin = min (Nmin, fabs (N [l]));
    Nmax = max (Nmax, fabs (N [l]));

    Nsign *= N [l] < 0 ? -1 : 1;

    }


    /* Nmin = Nmin (true) * |N [0]|
       Nmax = Nmax (true) * |N [0]| ... */

  if (Nmin == 0)

    return 0;

  if (Nmin * Nmax >= 1000 * N [0] * N [0])

    return 1000 * Nsign;

  return Nmin * Nmax * Nsign / (N [0] * N [0]);

  }

char * file_index (char name [], int index) {

  static char _name [16];

  sprintf (_name, name, index);

  return _name;

  }


  /*********************************************************************}
  {                                                                     }
  {  The following functions use angstrom**-1 and eV as input and       }
  {  output.  The internal workings are in atomic units.                }
  {                                                                     }
  {*********************************************************************/

double xk2xkp (double xk, double Vrcorr) {

  double E;

  E = fabs (xk) * xk * BOHR * BOHR + Vrcorr / RYDBERG;

  return (E < 0 ? -sqrt (-E) : sqrt (E)) / BOHR;

  }

double xkp2xk (double xkp, double Vrcorr) {

  double E;

  E = fabs (xkp) * xkp * BOHR * BOHR - Vrcorr / RYDBERG;

  return (E < 0 ? -sqrt (-E) : sqrt (E)) / BOHR;

  }


  /*********************************************************************}
  {                                                                     }
  {  sigms version 1.1 (24 March 94)                                    }
  {                                                                     }
  {    calculates the Debye-waller factor for a multiple-scattering     }
  {    path given the single-scattering factors for each atom.          }
  {                                                                     }
  {  The calculation make the following assumptions:                    }
  {                                                                     }
  {    (1) The vibration of all of the atoms are isotropic              }
  {    (2) The relative vibrations are isotropic                        }
  {               *                                                     }
  {  (1) => < u  u  > = c   I          c   scalar                       }
  {           ~i ~i      ii  3x3        ii                              }
  {                                                                     }
  {               *                                                     }
  {  (2) => < u  u  > = c   I          c   scalar                       }
  {           ~i ~j      ij  3x3        ij                              }
  {                                                                     }
  {  if i' = i + 1 the Debye-Waller factor for a scattering path L is   }
  {                                                                     }
  {     2                             ^    2                            }
  {  sig        = < [sum  (u - u  ) . R   ]  >                          }
  {     L               i  ~i  ~i'    ~ii'                              }
  {                                                                     }
  {                      ^*        *   ^       ^*        *   ^          }
  {             = sum  [ R   < u  u  > R    -  R   < u  u  > R          }
  {                  ij  ~jj'  ~j ~i   ~ii'    ~jj'  ~j'~i   ~ii'       }
  {                                                                     }
  {                      ^*        *   ^       ^*        *   ^          }
  {                    - R   < u  u  > R    +  R   < u  u  > R    ]     }
  {                      ~jj'  ~j ~i'  ~ii'    ~jj'  ~j'~i'  ~ii'       }
  {                                                                     }
  {                      ^*          *   ^         ^*        *   ^      }
  {             = sum  [ R     < u  u  > R      -  R   < u  u  > R      }
  {                  ij  ~j'j''  ~j'~i'  ~i'i''    ~jj'  ~j'~i'  ~i'i'' }
  {                                                                     }
  {                      ^*          *   ^       ^*        *   ^        }
  {                    - R     < u  u  > R    +  R   < u  u  > R    ]   }
  {                      ~j'j''  ~j'~i'  ~ii'    ~jj'  ~j'~i'  ~ii'     }
  {                                                                     }
  {                       ^*      ^*          *    ^       ^            }
  {             = sum  [ (R     - R   ) < u  u  > (R     - R   ) ]      }
  {                  ij   ~j'j''  ~jj'    ~j'~i'   ~i'i''  ~ii'         }
  {                                                                     }
  {  (1), (2)  =>                                                       }
  {                                                                     }
  {     2                 ^       ^        ^       ^                    }
  {  sig        = sum  [ (R     - R   ) . (R     - R   ) c    ]         }
  {     L            ij   ~j'j''  ~jj'     ~i'i''  ~ii'   i'j'          }
  {                                                                     }
  {                            2                                        }                                                       }
  {  If we define the shell sig  parameters to be                       }
  {                                                                     }
  {     2                                                               }
  {  sig        = 4 c    =>                                             }
  {     i            ii                                                 }
  {                                                                     }
  {                  2                                                  }
  {  c          = sig  / 4                                              }
  {   ii             i                                                  }
  {                                                                     }
  {  The single-scattering temperature factor for atom i is             }
  {                                                                     }
  {     2                                                               }
  {  sig        = 4 (c   - 2 c   + c  )                                 }
  {     0i            00      0i    ii                                  }
  {                                                                     }
  {  If two atoms i and j form part of a rigid group                    }
  {                                                                     }
  {     2                                                               }
  {  sig        = 4 (c   - 2 c   + c  ) = 0   =>                        }
  {     ij            ii      ij    jj                                  }
  {                                                                     }
  {  c          = (c   + c  ) / 2                                       }
  {   ij            ii    jj                                            }
  {                                                                     }
  {  If the two atoms are isolated from each other                      }
  {                                                                     }
  {  c          = 0                                                     }
  {   ij                                                                }
  {                                                                     }
  {  c   is modelled using the expression                               }
  {   ij                                                                }
  {                                                                     }
  {  c          = min ( (c   + c  ) / 2 * tanh (E |R  |) / (E |R  |),   }
  {   ij                  ii    jj                  ij          ij      }
  {                                                                     }
  {                      c  , c   )                                     }
  {                       ii   jj                                       }
  {                                                                     }
  {  where E is an empirical parameter                                  }
  {                                                                     }
  {*********************************************************************/

  /*********************************************************************}
  {                                                                     }
  {  legs   : number of legs in the path                                }
  {  R      : position of each atom in path                             }
  {  sigma2 : Debye-Waller factor of each atom in path                  }
  {  group  : indicates the group to which the atom belongs             }
  {                                                                     }
  {*********************************************************************/

double sigms (int legs, double E, double R [][3], double sigma2 [],

              int group []) {

  double s, r, c [8], d, D [8][3], cij;

  int l, l1, i, j, i1, j1;

  E = fabs (E);

  for (l = 0; l < legs; l++) {

      /* calculate the leg directions and cii values */

    l1 = (l + 1) % legs;

    D [l][0] = R [l1][0] - R [l][0];
    D [l][1] = R [l1][1] - R [l][1];
    D [l][2] = R [l1][2] - R [l][2];

    normalise3 (D [l]);

    c [l] = sigma2 [l] / 4;

    }

  s = 0;

  for (i = 0; i < legs; i++)

    for (j = 0; j < legs; j++) {

      i1 = (i + 1) % legs;
      j1 = (j + 1) % legs;

      d = (D [i][0] - D [i1][0]) * (D [j][0] - D [j1][0]) +
          (D [i][1] - D [i1][1]) * (D [j][1] - D [j1][1]) +
          (D [i][2] - D [i1][2]) * (D [j][2] - D [j1][2]);

      r = distance3 (R [i1], R [j1]);

      cij = c [i1];

      if (r > 1e-5)

        cij = (c [i1] + c [j1]) / 2;

      if (E * r > 1e-6) {

        cij /= (E * r);

        if (E * r < 20)

          cij *= tanh (E * r);

        }

      if (group)

        if (group [i1] != group [j1])

          cij = 0;

      cij = min (cij, min (c [i1], c [j1]));

      s += d * cij;

      }

  return s;

  }


  /*********************************************************************}
  {                                                                     }
  {  This routine calculates the polarisation factor as in              }
  {                                                                     }
  {    Gurman, S. J.: J. Phys. C. 21, 1988, 3699-3717                   }
  {                                                                     }
  {    "The small-atom approximation in EXAFS and surface EXAFS"        }
  {                                                                     }
  {    for K and L(1) edge EXAFS:                                       }
  {                                                                     }
  {        ^   ^   ^   ^             ^   ^                              }
  {      -(R . e) (R . e) / P (acos (R . R ))                           }
  {         i       f        1        i   f                             }
  {                                                                     }
  {    where:                                                           }
  {                                                                     }
  {      e  is the polarisation vector                                  }
  {                                                                     }
  {      R  is the initial leg of the path                              }
  {       i                                                             }
  {      R  is the final leg                                            }
  {       f                                                             }
  {      P  is the Legendre polynomial.                                 }
  {       1                                                             }
  {                                                                     }
  {  This is also the factor calculated in                              }
  {                                                                     }
  {    John J. Boland, Stephen E. Crane, and John D. Baldeschwieler:    }
  {                                                                     }
  {    J. Chem. Phys. 77(1), 1 July 1982, 142-153.                      }
  {                                                                     }
  {         ^   ^   ^   ^      ^   ^                                    }
  {      <-(R . e) (R . e)> = -R . R  / 3                               }
  {          i       f          i   f                                   }
  {                                                                     }
  {   So we normalise the polarised EXAFS by dividing by this factor.   }
  {                                                                     }
  {    Note that this expression cannot match exactly the expression    }
  {    used implicitely in FEFF 5.05 as the magnitude of the EXAFS is   }
  {    not zero when the initial and final legs of the path are         }
  {    normal.  To avoid grossly overestimating the importance of       }
  {    paths approximating this case, the factor actually used is:      }
  {                                                                     }
  {              ^   ^                                                  }
  {       { x = -R . R  }                                               }
  {               i   f                                                 }
  {                                                                     }
  {                       -1    -1  -1                                  }
  {      f = (|x| + (l|x|)   - l   )   sign (x) / 3                     }
  {                                                                     }
  {                                                                     }
  {       l = 239.73 (3 f maximum = 8):                                 }
  {                                                                     }
  {       0  : |x| = 1      : 3 f = 1      :    0.00%                   }
  {       10 :       0.9848 :       1.0154 :   -0.01%                   }
  {       20 :       0.9397 :       1.0639 :   -0.03%                   }
  {       30 :       0.8660 :       1.1538 :   -0.07%                   }
  {       40 :       0.7660 :       1.3032 :   -0.17%                   }
  {       50 :       0.6428 :       1.5501 :   -0.36%                   }
  {       60 :       0.5000 :       1.9835 :   -0.83%                   }
  {       70 :       0.3420 :       2.8568 :   -2.29%                   }
  {       80 :       0.1736 :       5.1680 :  -10.26%                   }
  {       90 :       0.0000 :       0.0000 : -100.00%                   }
  {                                                                     }
  {*********************************************************************/

double polarisation_factor (int legs, double R [][3], double polar []) {

  double e [3], i [3], f [3], x, fx;

  if (polar == NULL)

    return 1;

  e [0] = polar [0];
  e [1] = polar [1];
  e [2] = polar [2];

  if (normalise3 (e) == 0)

    return 1;

  i [0] = R [1][0] - R [0][0];
  i [1] = R [1][1] - R [0][1];
  i [2] = R [1][2] - R [0][2];

  f [0] = R [legs - 1][0] - R [0][0];
  f [1] = R [legs - 1][1] - R [0][1];
  f [2] = R [legs - 1][2] - R [0][2];

  if (normalise3 (i) == 0 || normalise3 (f) == 0)

    return 1;

  x = dot3 (i, f);

  if (fabs (x) < 1e-16)

    return 0;

  fx = (x < 0 ? -1 : 1) * (fabs (x) + 0.00417136 / fabs (x)
                                    - 0.00417136);

  return 3 * dot3 (i, e) * dot3 (f, e) / fx;

  }
#endif /* __FEFF_MS_C */
