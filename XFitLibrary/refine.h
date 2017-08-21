/***********************************************************************}
{                                                                       }
{  This file contains the Monte-Carlo error estimation routine.         }
{                                                                       }
{***********************************************************************/

#if !defined (__REFINE_H__)

#define __REFINE_H__

#include "paramete.h"
#include "expressi.h"


  /*********************************************************************}
  {                                                                     }
  {  Convert any non-scalar parameters in the list to their scalar      }
  {  components                                                         }
  {                                                                     }
  {  Return the number of parameters                                    }
  {                                                                     }
  {*********************************************************************/

int expand_refinement_parameters (parameter_type * name_list, node * x);


  /*********************************************************************}
  {                                                                     }
  {  Print the list of parameters                                       }
  {                                                                     }
  {  Return the number of parameters                                    }
  {                                                                     }
  {*********************************************************************/

int list_refinement_parameters (node * x);


#endif /* __REFINE_H__ */
