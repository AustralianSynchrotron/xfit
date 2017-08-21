/***********************************************************************}
{                                                                       }
{  Apply the filter to chi and the derivatives of chi                   }
{                                                                       }
{***********************************************************************/

#if !defined (__FILTER_H__)

#define __FILTER_H__

#include "exafs_ty.h"
#include "paramete.h"
#include "expressi.h"


  /*********************************************************************}
  {                                                                     }
  {  Given the calculated exafs and derivatives prior to the filter,    }
  {  calculate chi and the derivatives after the filter                 }
  {                                                                     }
  {*********************************************************************/

int apply_filter (exafs_data_type * E, parameter_set * P,

                   double chi [], double * dchi_df [],

                   matrix formal_actual,

                   node * x_0);

    /* The derivatives are not calculated if any of :

         * dchi_df = 0

         * formal_actual.cols = 0

         * x_0 = 0                               */


#endif /* __FILTER_H__ */

