/***********************************************************************}
{                                                                       }
{  Calculate the constraint matrix                                      }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__C_CONSTRAINT_MATRIX_C__)

#define __C_CONSTRAINT_MATRIX_C__

#include "c_constr.h"

#include "console.h"


  /*********************************************************************}
  {                                                                     }
  {  Calculate the constraint matrix                                    }
  {                                                                     }
  {  All constraints must be scalar                                     }
  {                                                                     }
  {  All variable parameters must be scalar                             }
  {                                                                     }
  {  Return : number of free parameters + 1                             }
  {                                                                     }
  {           0 on failure                                              }
  {                                                                     }
  {*********************************************************************/

int c_constraint_matrix (matrix * Cm, restraint_type * C,

                                      parameter_set * P, node * x) {

  restraint_type * C_0;

  node * x_0;

  int N_x, N_ind;

  double * T;

  static matrix full   = { 0 },

                MTM    = { 0 };

  static vector v      = { 0 };

  vector4 v4;

  int xc, c, xc_, i;

  double dC_dx_max;

  char * reduced;

  long * index;

  double determinant;


    /* Save the constraints and variable parameters */

  C_0 = C;

  x_0 = x;


    /* Check that there are some variable parameters */

  if (x_0 == NULL)

    return 0;


    /* Generate the full constraint matrix :

         C is N (constraints) x M (variable parameters + 1)

         C [i, j] = d (constraint expression i) / d (variable parameter j)

         C [i, M] = constraint expression i */

  N_x = calculate_constraint_matrix (&full, C, P, x_0);

    
    /* The derivatives must be constant :

       To check, calculate the matrix with random offsets in the variables

       Save the parameter values */

  T = calloc (N_x, sizeof (double));

  if (T) {

    for (xc = 0, x = x_0; x; xc++, x = x->n [0]) {

        /* Save the parameter */

      v4 = parameter_value (P, x->parameter);

      T [xc] = scal (v4);


        /* Add a random offset */

      set_parameter (P, x->parameter,

                     make_vector1 (T [xc] + frandom () - frandom ()));

      }

      /* Get the new constraint matrix */

    calculate_constraint_matrix (&MTM, C, P, x_0);


      /* Compare the derivatives for each constraint.

           <error> = 1 : the constraint cannot be satisfied

           <error> = 2 : the derivatives are not constant */

    for (C = C_0, c = 0; C; C = C->next, c++) {

        /* Get the maximum derivative for constraint C */

      for (dC_dx_max = xc = 0; xc < N_x; xc++)

        dC_dx_max = max (dC_dx_max, fabs (m_el (full, c, xc)));


        /* If the derivatives are all 0 and the value <> 0 <error> = 1 */

      C->error = (dC_dx_max == 0 && m_el (full, c, xc) != 0);


        /* Compare the derivatives */

      dC_dx_max *= 1e-8;

      for (xc = 0; xc < N_x && !C->error && dC_dx_max != 0; xc++)

        if (fabs (m_el (full, c, xc) - m_el (MTM, c, xc)) > dC_dx_max)

          C->error = 2;

      }


      /* Restore the parameters */

    for (xc = 0, x = x_0; x; xc++, x = x->n [0])

      set_parameter (P, x->parameter, make_vector1 (T [xc]));

    free (T);

    }


    /* Return if any of the constraints have errors */

  for (C = C_0, c = 0; C; C = C->next, c++)

    if (C->error)

      return 0;


    /* "full" is the full linear constraint matrix :

       Since the last column is just the current values of the
       constraint equations, the parameters used are relative
       to the current parameters.

       The constraining equation is :

           full * delta (parameters) = 0
                           1

       Row-reduce full: */


  row_reduce_matrix (full, 1e-12, &reduced, 0);


    /* Check all rows for insoluble constraints

       (the first nonzero entry is in the last column) */

  for (C = C_0, c = 0; C; C = C->next, c++)

    for (xc = 0; xc < full.cols; xc++)

      if (m_el (full, c, xc) != 0) {

        if (xc == full.cols - 1)

          C->error = 1;

        break;

        }


    /* All parameters in reduced columns are dependent,
       all others are independent.

       Count the dependent and independent parameters. */

  for (xc = N_ind = 0; xc < N_x + 1; xc++)

    N_ind += !reduced [xc];


    /* Resize and clear the solution matrix */

  resize_matrix (Cm, N_x, N_ind);

  clear_matrix (*Cm);


    /* Construct the matrix */

  for (xc = N_ind = 0; xc < N_x + 1; xc++)

    if (!reduced [xc]) {

        /* This is independent parameter N_ind.

           Add the dependence of all parameters on this one */

      if (xc < N_x)

        m_el (*Cm, xc, N_ind) = 1;

      for (xc_ = 0; xc_ < N_x; xc_++)

        if (reduced [xc_]) {

          for (c = 0; c < full.rows; c++)

            if (m_el (full, c, xc_) != 0)

              break;

          m_el (*Cm, xc_, N_ind) = -m_el (full, c, xc) / m_el (full, c, xc_);

          }

      N_ind++;

      }


    /* Return if any of the constraints are insoluble */

  for (C = C_0; C; C = C->next)

    if (C->error)

      return 0;


    /* Bring the parameters as close as possible to the initial values

       (in a least-squares sense).

       If M is the matrix and d the change from initial parameters

       (the final row of the constraint matrix):

                  T  -1  T
            f = (M M)   M (-d)

       and the final change in the parameters is:

                      T  -1  T
            M f = M (M M)   M (-d)

       and the parameter vector returned is

                              T  -1  T
            d + M f = d + M (M M)   M (-d)

                  T
       Calculate M M:                 */

  resize_matrix (&MTM, Cm->cols - 1, Cm->cols - 1);

  for (xc = 0; xc < MTM.cols; xc++)

    for (xc_ = 0; xc_ <= xc; xc_++) {

      m_el (MTM, xc, xc_) = 0;

      for (i = 0; i < Cm->rows; i++)

        m_el (MTM, xc, xc_) += m_el (*Cm, i, xc) * m_el (*Cm, i, xc_);

      m_el (MTM, xc_, xc) = m_el (MTM, xc, xc_);

      }


    /*             T
       Calculate -M d:  */

  resize_vector (&v, Cm->rows);

  for (c = 0; c < Cm->rows; c++)

    v_el (v, c) = -m_el (*Cm, c, MTM.cols);

  transpose_matrix (&v);

  mul_matrix (&v, v, *Cm);

  transpose_matrix (&v);

  v.rows--;


    /*         T  -1   T
       Solve (M M)  (-M d):  */

  index = calloc_check (MTM.rows, sizeof (long));

  LU_decompose (MTM, index, &determinant);

  LU_backsubstitute (MTM, index, v);

  free_check (index);


    /*             T  -1 T
       Get d - M (M M)  M d:  */

  resize_vector_retain (&v, v.rows + 1);

  v_el (v, v.rows - 1) = 1;

  mul_matrix_vector (&v, *Cm, v);


    /* Add this to the original values and substitute the last row */

  for (xc = 0, x = x_0; x; xc++, x = x->n [0]) {

    v4 = parameter_value (P, x->parameter);

    m_el (*Cm, xc, Cm->cols - 1) = scal (v4) + v_el (v, xc);

    }


    /* Return the number of free parameters + 1 */

  return Cm->cols;

  }


  /*********************************************************************}
  {                                                                     }
  {  Print the errors for the constraints C                             }
  {                                                                     }
  {  Return the number of errors                                        }
  {                                                                     }
  {*********************************************************************/

int fprintf_constraint_error (FILE * file, restraint_type * C) {

  int count, error_count;

  error_count = 0;


    /* Go through the constraints and print the error messages */

  for (count = 1; C; C = C->next, count++)

    if (C->error) {

      error_count++;

      fcout (file, " constraint %3d :\n\n ", count);

      fprintf_expression (file, C->n, 0);

      switch (C->error) {

        case 1 :

          fcout (file, "\n\n cannot be satisfied\n\n");

          break;

        case 2 :

          fcout (file, "\n\n is not linear\n\n");

          break;

        }
      }

  return error_count;

  }


#endif /* C_CONSTRAINT_H */
