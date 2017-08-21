/***********************************************************************}
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__INTERPOLATE_H__)

#define __INTERPOLATE_H__

#define I_SIMPLE 0
#define I_LIN_BIN 1

#include "header.h"

  /*********************************************************************}
  {                                                                     }
  {  Interpolate from (x, y) to (X, Y)                                  }
  {                                                                     }
  {  Both x and X are assumed to be increasing monotonically            }
  {                                                                     }
  {    method may be :                                                  }
  {                                                                     }
  {      < 1 : interpolate linearly                                     }
  {      > 0 : interpolate linearly using bins                          }
  {                                                                     }
  {  All unknown points are set to 0                                    }
  {                                                                     }
  {*********************************************************************/

int interpolate (int method, int n, double x [], double y [],
                             int N, double X [], double Y []);


  /*********************************************************************}
  {                                                                     }
  {  Calculate the derivative of the interpolated curve with respect    }
  {  to some variable upon which the positions of the original points   }
  {  depend, given the derivative of the positions with respect to the  }
  {  variable.                                                          }
  {                                                                     }
  {  Both x and X are assumed to be increasing monotonically            }
  {                                                                     }
  {    method may be :                                                  }
  {                                                                     }
  {      < 1 : interpolate linearly                                     }
  {      > 0 : interpolate linearly using bins                          }
  {                                                                     }
  {  All unknown points are set to 0                                    }
  {                                                                     }
  {*********************************************************************/

int dinterpolate_dx (int method, int n, double x [], double dx [], double y [],
                                 int N, double X [], double dY []);


#endif /* __INTERPOLATE_H__ */
