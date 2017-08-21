/***********************************************************************}
{                                                                       }
{  Data structure used to store restraints and constraints.             }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__RESTRAINTS_H__)

#define __RESTRAINTS_H__

#include "expressi.h"
#include "paramete.h"

#include "matrix.h"


  /*********************************************************************}
  {                                                                     }
  {  Data structures used to store restraints and constraints.          }
  {                                                                     }
  {*********************************************************************/

typedef struct restraint_struct {

  node * n;

  int error;

  struct restraint_struct * next;

  } restraint_type;

typedef struct {

  restraint_type * restraint,

                 * constraint;

  matrix formal_actual;

  vector actual_0;

  node * refine;

  } restraints_type;


  /*********************************************************************}
  {                                                                     }
  {  Add a restraint or constraint to the list                          }
  {                                                                     }
  {*********************************************************************/

restraint_type * add_restraint (restraint_type ** restraint, node * n);


  /*********************************************************************}
  {                                                                     }
  {  Calculate the Jacobian constraint / restraint matrix               }
  {                                                                     }
  {  Return the number of variable parameters (x)                       }
  {                                                                     }
  {*********************************************************************/

int calculate_constraint_matrix (matrix * J, restraint_type * C,

                                         parameter_set * P, node * x);

#endif /* __RESTRAINTS_H__ */





