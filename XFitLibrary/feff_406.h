/***********************************************************************}
{                                                                       }
{  Single-scattering FEFF exafs calculation (FEFF 4.06) for xfit        }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__FEFF_406_C_H__)

#define __FEFF_406_C_H__

#include "paramete.h"


#if defined (__TURBOC__)

#include "search.h"

#else

#include "search.h"

#endif


#include "feff_.h"
#include "ff4_chi.h"
#include "common.h"


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

#if !defined (__FEFF_406_C_C__)

extern parameter_type FEFF_406_parameter_name [];

#endif


  /************************************}
  {                                    }
  {  Rationalise the parameter vector  }
  {                                    }
  {************************************/

int FEFF_406_rationalise (parameter_set * set);


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
  {*********************************************************************/


#endif /* __FEFF_406_C_H__ */
