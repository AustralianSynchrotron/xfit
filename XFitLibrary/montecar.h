/***********************************************************************}
{                                                                       }
{  This file contains the Monte-Carlo error estimation routine.         }
{                                                                       }
{***********************************************************************/

#if !defined (__MONTECARLO_H__)

#define __MONTECARLO_H__

#include "common.h"

#include "refine.h"

#include "calculat.h"
#include "statisti.h"
#include "paramete.h"
#include "expressi.h"
#include "c_constr.h"


#include "print.h"
#include "event.h"
#include "minimise.h"


  /*********************************************************************}
  {                                                                     }
  {  Do MC_N refinements starting from the current parameter values.    }
  {                                                                     }
  {  values.  Generate the exafs data for each refinement by adding     }
  {  noise with RMS magnitude at each point specified by the sigma      }
  {  value read from the exafs file.                                    }
  {                                                                     }
  {  Using the set of MC_N refined values, calculate a set of esd's     }
  {  and the correlation matrix.                                        }
  {                                                                     }
  {  The parameters are not changed by this command.                    }
  {                                                                     }
  {*********************************************************************/

int montecarlo_command

          ( node                * refine,
            node                * too [],

            int                   MC_N,
            int                   max_N,

            restraints_type     * restraint,
            parameter_set       * P,
            exafs_list         ** exafs,
            parameter_type      * name_list );


#endif /* __MONTECARLO_H__ */
