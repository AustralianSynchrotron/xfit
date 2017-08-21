/***********************************************************************}
{                                                                       }
{  Calculate the various curves used in the exafs window                }
{                                                                       }
{***********************************************************************/

#if !defined (__X_CURVES_H__)

#define __X_CURVES_H__

#if defined (__TURBOC__)

#include "complex.h"

#else

#include "complex.h"

#endif

#include "exafs_ty.h"


  /*********************************************************************}
  {                                                                     }
  {  Calculate the various curves used in the exafs window              }
  {                                                                     }
  {  All of the curves can be simply calculated from the following :    }
  {                                                                     }
  {    * experimental k                                                 }
  {                                                                     }
  {    * weighted experimental exafs                                    }
  {                                                                     }
  {    * interpolated experimental exafs                                }
  {                                                                     }
  {    * uniform k                                                      }
  {                                                                     }
  {    * k window                                                       }
  {                                                                     }
  {    * weighted calculated exafs                                      }
  {                                                                     }
  {    * fourier-transform of observed exafs                            }
  {                                                                     }
  {    * fourier-transform of calculated exafs                          }
  {                                                                     }
  {    * r window                                                       }
  {                                                                     }
  {    * filtered observed exafs                                        }
  {                                                                     }
  {    * filtered calculated exafs                                      }
  {                                                                     }
  {                                                                     }
  {  A call to this routine with exafs = 0 will free any space          }
  {                                                                     }
  {*********************************************************************/

int calculate_exafs_curves (exafs_data_type * exafs,

                            int * experimental,

                            int * calculated,

                            int * uniform,

                            int * fft,

                            double ** experimental_k,

                            double ** experimental_exafs,

                            double ** i_experimental_exafs,

                            double ** calculated_k,

                            double ** calculated_exafs,

                            double ** i_calculated_exafs,

                            double ** uniform_k,

                            double ** k_windo,

                            double ** uniform_r,

                            complex ** ft_observed_exafs,

                            complex ** ft_calculated_exafs,

                            double ** r_windo,

                            double ** f_observed_exafs,

                            double ** f_calculated_exafs);


#endif /* __X_CURVES_H__ */
