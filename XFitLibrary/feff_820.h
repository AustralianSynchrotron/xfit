/***********************************************************************}
{                                                                       }
{  Multiple-scattering FEFF exafs calculation (FEFF 6.01) for xfit      }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__FEFF_820_C_H)

#define __FEFF_820_C_H

#include "exafs.h"
#include "feff8.h"
#include "ff8_chi.h"
#include "common.h"
#include "paramete.h"


  /* To match the conventional use of the "shells" parameter:

     "shells" = maximum shell index.

     The parameters common to each shell in the model are:

                  default

       LEGS    :  4
       RMAX    :  4
       PW      :  0
       CW      :  0
       E       :  100
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


#if !defined (__FEFF_820_C_C)

extern parameter_type FEFF_820_parameter_name [];

#endif


  /************************************}
  {                                    }
  {  Rationalise the parameter vector  }
  {                                    }
  {************************************/

int FEFF_820_rationalise (parameter_set * set);


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

int FEFF_820_calculate (vector model,

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


#endif /* __FEFF_820_C_H */
