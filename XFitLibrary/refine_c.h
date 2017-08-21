/***********************************************************************}
{                                                                       }
{  This file contains the refinement routine.                           }
{                                                                       }
{***********************************************************************/

#if !defined (__REFINE_COMMAND_H__)

#define __REFINE_COMMAND_H__

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
  {  Change the parameter in the list to minimise chi**2                }
  {                                                                     }
  {*********************************************************************/

int refine_command (

            node                * refine,

            int                   max_N,

            restraints_type     * restraint,
            parameter_set       * P,
            parameter_type      * name_list,

            int                   print );


#endif /* __REFINE_COMMAND_H__ */
