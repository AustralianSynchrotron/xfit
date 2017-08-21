/***********************************************************************}
{                                                                       }
{  Routines for reading the feff##.dat files written by FEFF 4.06 and   }
{  for calculating EXAFS and the derivatives of EXAFS with respect to   }
{  the parameters, using the data read.                                 }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__FF4_CHI_H__)

#define __FF4_CHI_H__

#if !defined M_2PI

#define M_2PI (2 * M_PI)

#endif

#include "header.h"

#include "exafs.h"


  /*********************************************************************}
  {                                                                     }
  {  Define the structure used to hold the data from a FEFF 4.06 file   }
  {                                                                     }
  {*********************************************************************/

#if defined (MAXPOINTS)
#undef MAXPOINTS
#endif

#define MAXPOINTS 64

typedef struct {

  int    points;

  double k          [MAXPOINTS],
         phi_c      [MAXPOINTS],
         feff       [MAXPOINTS],
         phase_feff [MAXPOINTS],
         red_factor [MAXPOINTS],
         lambda     [MAXPOINTS],
         real_p     [MAXPOINTS];

  int    ZA, ZS, hole;

  double R;

  int    lines;

  char ** line;

  } FEFF_406_data;


  /**********************************************}
  {                                              }
  {  Functions to manipulate the data structure  }
  {                                              }
  {**********************************************/

void setup_FEFF_406 (FEFF_406_data * FEFF);


  /***************************************}
  {                                       }
  {  Read the data from a FEFF 4.06 file  }
  {                                       }
  {***************************************/

int read_FEFF_406 (FILE * file, char filename [], FEFF_406_data * FEFF);


  /*********************************************************************}
  {                                                                     }
  {  Calculate the exafs given the exafs data and model parameters      }
  {                                                                     }
  {*********************************************************************/

int calculate_FEFF_406_EXAFS (FEFF_406_data * FEFF [],

                              double polar [], double Q,

                              double E0, double S02,

                              double N, double v [], double sigma2,

                              int N_, double k [], double Chi []);

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

                                int X);

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


#endif /* __FF4_CHI_H__ */
