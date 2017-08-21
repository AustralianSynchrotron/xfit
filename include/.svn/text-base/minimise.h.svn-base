/***********************************************************************}
{                                                                       }
{  These routines are modified from chapter 10 of                       }
{                                                                       }
{       "Numerical Recipes"                                             }
{                                                                       }
{  William H. Press, Brian P. Flannery, Saul A. Teukolsky, and          }
{  William T. Vetterling                                                }
{                                                                       }
{  Cambridge University Press, 1986                                     }
{                                                                       }
{  NB: there is an error in the parabolic fit given in this book        }
{                                                                       }
{***********************************************************************}

{***********************************************************************}
{                                                                       }
{  This file uses the definitions in "matrix.h"                         }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1993.             }
{                                                                       }
{***********************************************************************/

#if !defined (__MINIMISE_H__)

#define __MINIMISE_H__

#include "matrix.h"

#if !defined (CHECK_DERIVATIVES)

#define CHECK_DERIVATIVES 0

#endif

#if defined (__MINIMISE_C__)

int check_derivatives = CHECK_DERIVATIVES;

#else

extern int check_derivatives;

#endif

  /*********************************************************************}
  {                                                                     }
  {  Given the function f and a bracketing triple  xa, xb, and xc,      }
  {  this routine isolates the minimum to a fractional tolerance        }
  {  of about tol, using Brent's method.                                }
  {                                                                     }
  {  The number of function evaluations is returned                     }
  {                                                                     }
  {*********************************************************************/

long brent (double (* f) (double _x),

            double xa, double xb, double xc,

            double tol, double * x_min);


  /*********************************************************************}
  {                                                                     }
  {  Given the function f and distinct initial points xa and xb,        }
  {  this routine returns xa, xb and xc bracketing the minimum of f.    }
  {                                                                     }
  {  The number of function evaluations is returned                     }
  {                                                                     }
  {*********************************************************************/

long scalar_bracket (double (* f) (double _x),

                      double * xa, double * xb, double * xc);


  /*********************************************************************}
  {                                                                     }
  {  Find the x giving the minimum of the function f.                   }
  {                                                                     }
  {  The number of function evaluations is returned                     }
  {                                                                     }
  {*********************************************************************/

long scalar_minimise (double (* f) (double _x), double x,

                       double step, double tolerance, double * x_min);


  /*********************************************************************}
  {                                                                     }
  {  Find the vector v giving the minimum of the function f searching   }
  {  along the direction delta                                          }
  {                                                                     }
  {  The number of function evaluations is returned                     }
  {                                                                     }
  {*********************************************************************/

long vector_minimise (double (*f) (vector _v), vector v,

                       vector delta, double step, double tolerance);


  /*********************************************************************}
  {                                                                     }
  {  The rest of this file are used for the Levenberg-Marquardt         }
  {  minimisation routines.                                             }
  {                                                                     }
  {*********************************************************************/

  /*********************************************************************}
  {                                                                     }
  {  Given the function f and an initial point v0, this function uses   }
  {  the Levenberg-Marquardt method to change v0, minimising chi**2.    }
  {                                                                     }
  {  lambda is set to < 0 to initialise the function : lambda = 0.001   }
  {                                                                     }
  {                         2                                           }
  {  successful step   : chi  decreases : lambda = lambda / 10          }
  {                                                                     }
  {                         2                                           }
  {  unsuccessful step : chi  increases : lambda = lambda * 10          }
  {                                                                     }
  {                                                    2                }
  {  If this function is called with solve < 0 then chi  is evaluated   }
  {  with no change to any parameters.                                  }
  {                                                                     }
  {                                                                     }
  {  This routine must be called repeatedly until convergence is        }
  {  achieved.                                                          }
  {                                                                     }
  {  A final call with lambda = 0 will set covar to the covariance      }
  {  matrix and alpha to the curvature matrix.                          }
  {                                                                     }
  {  The function f has as argument a vector v and a list of vectors:   }
  {                                                                     }
  {    chi : chi values                                                 }
  {                                                                     }
  {    dchi_dv [] : derivatives of chi with respect to the elements     }
  {                 of v                                                }
  {                                                                     }
  {  If any element of dchi_dv [] is NULL, it is not evaluated          }
  {                                                                     }
  {  f return 0 on error                                                }
  {                                              2                      }
  {  The routine returns the current value of chi  or < 0 on error      }
  {                                                                     }
  {                                                                     }
  {  If solve is 0 gaussian elimination is used to solve the linear     }
  {  system, if solve is 1 singular value decomposition is used         }
  {                                                                     }
  {*********************************************************************/

double marquardt_minimise_G (short (* f) (vector v,
                                          vector * chi, vector dchi_dv [],
                                          int status),

                             vector v0,

                             matrix * covar, matrix * alpha,

                             double * lambda, int solve);


  /*********************************************************************}
  {                                                                     }
  {  This routine is the same as marquardt_minimise_G, save that        }
  {  Gaussian elimination is always used to solve the matrix inversion  }
  {                                                                     }
  {*********************************************************************/

double marquardt_minimise (short (* f) (vector v,
                                        vector * chi,
                                        vector * dchi_dv, int status),

                               vector v0,

                               matrix * covar, matrix * alpha,

                               double * lambda);


  /*********************************************************************}
  {                                                                     }
  {  This routine is the same as marquardt_minimise_G, save that        }
  {  Singular Value Decomposition is always used to solve the matrix    }
  {  inversion.                                                         }
  {                                                                     }
  {*********************************************************************/

double marquardt_minimise_SVD (short (* f) (vector v,
                                            vector * chi,
                                            vector * dchi_dv, int status),

                               vector v0,

                               matrix * covar, matrix * alpha,

                               double * lambda);

#endif /* __MINIMISE_H__ */
