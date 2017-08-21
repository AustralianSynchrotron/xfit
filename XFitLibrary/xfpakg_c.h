/***********************************************************************}
{                                                                       }
{  Single-scattering XFPAKG exafs calculation for xfit                  }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__XFPAKG_C_H)

#define __XFPAKG_C_H

#include "exafs.h"
#include "common.h"
#include "paramete.h"
#include "header.h"


#if !defined (__XFPAKG_C_C)

extern parameter_type XFPAKG_parameter_name [];

#endif


  /************************************}
  {                                    }
  {  Rationalise the parameter vector  }
  {                                    }
  {************************************/

int XFPAKG_rationalise (parameter_set * set);


  /*********************************************************************}
  {                                                                     }
  {  Each exafs calculation uses two sets of parameters:                }
  {                                                                     }
  {    (1) model:                                                       }
  {                                                                     }
  {               E0, SHELL0, SHELLS                                    }
  {                                                                     }
  {             [ Z, N, x, y, z, sigma2, scale, trans,                  }
  {                                                                     }
  {               c0, c1, c2, c3, a_1, a0, a1, a2 ]                     }
  {                                                                     }
  {    (2) The data set parameters                                      }
  {                                                                     }
  {               ZA, hole                                              }
  {                                                                     }
  {             [ polar, normal, ellipticity, Q ]                       }
  {                                                                     }
  {  Together these make up the parameter vector of size                }
  {                                                                     }
  {               SHELLS * 26 - 13                                      }
  {                                                                     }
  {  Only the first polarisation vector is used                         }
  {                                                                     }
  {  normal, ellipticity and Q are ignored                              }
  {                                                                     }
  {  The polarisation and Q cannot be refined                           }
  {                                                                     }
  {*********************************************************************/

int XFPAKG_calculate (vector model,

                        int ZA, int hole,

                        int N_Q, double polar [], double normal [],
                                 double ellipticity [], double Q [],

                        int N, double k [], double chi [],

                        double * dchidv [], matrix dv,

                        int status);

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


#endif /* __XFPAKG_C_H */
