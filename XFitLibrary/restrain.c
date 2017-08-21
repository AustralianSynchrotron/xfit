/***********************************************************************}
{                                                                       }
{  Data structure used to store restraints and constraints.             }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__RESTRAINTS_C__)

#define __RESTRAINTS_C__

#include "restrain.h"


  /*********************************************************************}
  {                                                                     }
  {  Add a restraint or constraint to the list                          }
  {                                                                     }
  {*********************************************************************/

restraint_type * add_restraint (restraint_type ** restraint, node * n) {

  for (; *restraint; restraint = &((*restraint)->next));

  *restraint = calloc (1, sizeof (restraint_type));

  (*restraint)->next = NULL;

  (*restraint)->n = n;

  return *restraint;

  }


  /*********************************************************************}
  {                                                                     }
  {  Calculate the Jacobian constraint / restraint matrix               }
  {                                                                     }
  {  Return the number of variable parameters (x)                       }
  {                                                                     }
  {*********************************************************************/

int calculate_constraint_matrix (matrix * J, restraint_type * C,

                                         parameter_set * P, node * x) {

  int N_x, constraints, constraint, parameter;

  restraint_type * C_0;

  node * x_0;


    /* Count the variable parameters */

  for (x_0 = x, N_x = 0; x; x = x->n [0], N_x++);


    /* Count the constraints */

  for (C_0 = C, constraints = 0; C; C = C->next, constraints++);


    /* Set the matrix to be constraints x variable parameters + 1 */

  resize_matrix (J, constraints, N_x + 1);

  if (constraints == 0)

    J->cols = N_x + 1;


  for (constraint = 0, C = C_0; C; C = C->next, constraint++) {

    for (parameter = 0, x = x_0; x; x = x->n [0], parameter++) {

        /* Calculate the value of the expression and the derivative */

      reduce_tree (C->n, x->parameter, P);

      m_el (*J, constraint, parameter) = scal (C->n->df_dx);

      }

    m_el (*J, constraint, N_x) = scal (C->n->f);

    }

  return N_x;

  }


#endif /* __RESTRAINTS_C__ */





