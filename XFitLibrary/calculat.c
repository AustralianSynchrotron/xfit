/***********************************************************************}
{                                                                       }
{  The function used by the minimising routine to calculate:            }
{                                                                       }
{    * chi                                                              }
{                                                                       }
{    * derivatives of chi                                               }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__CALCULATE_CHI_C__)

#define __CALCULATE_CHI_C__

#include "calculat.h"

#include "input.h"

//#if defined (USE_GRAPH)
//
//#include "draw_3d.h"
//
//#endif


  /*********************************************************************}
  {                                                                     }
  {  calculate_chi is a function of the form called by the              }
  {  minimisation function and has arguments :                          }
  {                                                                     }
  {    * formal : a vector of formal (independent) parameters           }
  {                                                                     }
  {    * chi    : exafs                                                 }
  {                                                                     }
  {    * dchi_dformal [] : derivatives of chi with respect to the       }
  {                        elements of formal                           }
  {                                                                     }
  {    * status : status is passed to the calculation routines          }
  {                                                                     }
  {  calculate_chi returns 0 on error                                   }
  {                                                                     }
  {    chi = NULL : chi is not calculated.                              }
  {                                                                     }
  {                 Currently this will terminate the routine           }
  {                                                                     }
  {    dchi_dformal = NULL : derivatives are not calculated             }
  {                                                                     }
  {  calculate_chi uses the global variables :                          }
  {                                                                     }
  {    global_data and __zero_chi__                                     }
  {                                                                     }
  {  __zero_chi__ = 0 : normal calculation                              }
  {                                                                     }
  {                 1 : chi [..] = 0                                    }
  {                                                                     }
  {                 2 : restraints only                                 }
  {                                                                     }
  {                                                                     }
  {*********************************************************************/

short calculate_chi (vector formal, vector * chi, vector dchi_dformal [],

                     int status) {


  static vector actual = { 0 },

                dchi = { 0 };

  static matrix restraint = { 0 },

                dv = { 0 };

  static double ** dchi_df = 0, * k = 0;

  static long dchi_df_s = 0, k_s = 0;


  int i, N, N_max, N_sum, N_set, N_chi, nochi, f, a, used,

      model_derivs, model_size, index, model_parameter, PQ_parameter;

  matrix            formal_actual;

  vector            actual_0,

                  * model;

  exafs_list * exafs_data, * exafs_data_0;

  exafs_data_type * exafs;

  restraints_type * restraints;

  parameter_set * P;

  node * x;

  double Q, model_key;


    /* Get the global data :

       Get the parameter set */

  P = global_data.parameters;


    /* Get the key to the model vector */

  model_key = parameter_vector_key_Oi (P, MODEL, 0);


    /* Get the model vector */

  model = parameter_vector (P, model_key);


    /* Get the restraints / constraints / variable */

  restraints = global_data.restraints;


    /* If __zero_chi__ = 2 don't calculate any exafs */

  if (__zero_chi__ == 2)

    exafs_data_0 = 0;

  else

    exafs_data_0 = * (global_data.exafs);


    /* Check that this is a valid calculation */

  if (chi == 0)

    return 0;


    /* Get the components of the constraint matrix */

  formal_actual = restraints->formal_actual;

  actual_0      = restraints->actual_0;


    /* Calculate the new parameters :

       actual' = formal_actual * formal + actual_0 */

  if (!mul_matrix_vector (&actual, formal_actual, formal))

    return 0;

  for (i = 0, x = restraints->refine; i < actual.rows; x = x->n [0], i++) {

    v_el (actual, i) += v_el (actual_0, i);


      /* Update the parameter set */

    set_parameter (P, x->parameter, make_vector1 (v_el (actual, i)));

    }


    /* Calculate the current value of the restraint matrix.

       N restraints in n free real variables :

         R is N x (n + 1)    R(ij) = drestraint (i) / dvariable (j)

                             R(in) = restraint (i)  */

  calculate_constraint_matrix (&restraint, restraints->restraint,

                                         P, restraints->refine);


    /* Calculate the space required for the data structures */

  N_max = N_sum = 0;

  for (exafs_data = exafs_data_0; exafs_data;

                                       exafs_data = exafs_data->next) {

      /* Update the exafs structure from the parameter set */

    exafs = &(exafs_data->exafs);

    copy_set_exafs_data (exafs, P);

    N_set = exafs->N;

    N_max = max (N_max, N_set);

    N_sum += N_set;

    }


    /* Calculate the total number of chi points */

  N_chi = N_sum + restraint.rows;

  nochi = N_chi == 0;

  if (nochi)

    N_chi = 1;


    /* Allocate space for k [], chi and dchi_dformal [] and dchi */

  realloc_increment ((void *) &k, &k_s, N_max, sizeof (double), 1024);

  if (chi) {

    resize_vector (chi, n_step (N_chi, 1024));

    chi->rows = N_chi;

    clear_vector (*chi);

    }

  resize_vector (&dchi, n_step (N_max, 1024));

  dchi.rows = N_max;

  for (f = 0; f < formal.rows && dchi_dformal; f++) {

    resize_vector (dchi_dformal + f, n_step (N_chi, 1024));

    dchi_dformal [f].rows = N_chi;

    clear_vector (dchi_dformal [f]);

    }


    /* If there is nothing to calculate, exit */

  if (nochi)

    return 1;


    /* Allocate space for the array of pointers

       (dchi_df has an extra entry for k: [-1]) */

  if (dchi_dformal) {

    if (dchi_df)

      dchi_df--;

      realloc_increment ((void *) &dchi_df, &dchi_df_s,

                          formal.rows + 1, sizeof (double *), 64);

    if (dchi_df)

      dchi_df++;

    }


    /* Calculate chi and the derivatives of chi for each EXAFS data set */

  N_sum = 0;

  for (exafs_data = exafs_data_0; exafs_data; exafs_data = exafs_data->next) {

      /* Calculate the chi (error) vector prior to filtering */

    exafs = &(exafs_data->exafs);

    N = exafs->N;


      /* Calculate k */

    Q = delta_k (exafs->r_max);

    for (i = 0; i < N; i++)

        k [i] = i * Q;


      /* Calculate chi */

    if (chi) {

        /* If __zero_chi__ = 1 set exafs to 0 */

      if (__zero_chi__ == 1)

        for (i = 0; i < N; i++)

          v_el (*chi, N_sum + i) = 0;

      else

        if (global_data.chi_calculate (*model,

                                       exafs->absorber, exafs->edge,

                                       exafs->shells,

                                       exafs->polarisation, exafs->normal,
                                       exafs->ellipticity,

                                       exafs->O,

                                       N, k, &v_el (*chi, N_sum),

                                       NULL, null_matrix,

                                       status) < N)

          return 0;

      }

      /* Calculate the derivatives */

    if (dchi_dformal) {

        /* Count the number of formal parameter vectors including model
           parameters as components and the maximum offset + index of
           the model parameters used */

      model_derivs = model_size = 0;

      for (f = 0; f < formal.rows; f++) {

        used = 0;

        for (a = 0, x = restraints->refine;

                    a < actual.rows; a++, x = x->n [0]) {

            /* calculate chi can calculate derivatives for :

                 * model parameters

                 * polarisation, normal, ellipticity and Q from the data  set */

          model_parameter = x->parameter.key == model_key;

          PQ_parameter = x->parameter.key == exafs->key &&

                          x->parameter.offset >= exafs->PQ_offset;

          if ((model_parameter || PQ_parameter)

                               && m_el (formal_actual, a, f) != 0) {

            index = x->parameter.offset + max (x->parameter.index, 1);

            if (PQ_parameter)

              index = index + model->rows - exafs->PQ_offset;

            model_size = max (model_size, index);

            used = 1;

            }
          }

        model_derivs += used;

        }


        /* Allocate space for the matrix of model parameter
           vectors specifying the derivatives required */

      resize_matrix (&dv, model_size, model_derivs);

      clear_matrix (dv);

      dchi_df [-1] = 0;


        /* Generate the matrix of model parameter vectors and
           set up the pointers */

      model_derivs = 0;

      for (f = 0; f < formal.rows; f++) {

        used = 0;

        for (a = 0, x = restraints->refine;

                    a < actual.rows; a++, x = x->n [0]) {

          model_parameter = x->parameter.key == model_key;

          PQ_parameter = x->parameter.key == exafs->key &&

                          x->parameter.offset >= exafs->PQ_offset;

          if ((model_parameter || PQ_parameter)

                               && m_el (formal_actual, a, f) != 0) {

            index = x->parameter.offset + max (x->parameter.index, 1);

            if (PQ_parameter)

              index = index + model->rows - exafs->PQ_offset;

            m_el (dv, index - 1, model_derivs) =

                                      m_el (formal_actual, a, f);

            used = 1;

            }
          }


          /* Point to the storage associated with the model vectors */

        if (used)

          dchi_df [model_derivs] = &v_el (dchi_dformal [f], N_sum);

        model_derivs += used;

        }


        /* Calculate the derivatives if there are any */

      if (model_derivs)

        global_data.chi_calculate (*model,

                                   exafs->absorber, exafs->edge,

                                   exafs->shells,

                                   exafs->polarisation, exafs->normal,
                                   exafs->ellipticity,

                                   exafs->O,

                                   N, k, 0,

                                   dchi_df, dv, 0);


        /* Generate the full set of pointers */

      for (f = 0; f < formal.rows; f++)

        dchi_df [f] = &v_el (dchi_dformal [f], N_sum);

      }


      /* chi [N_sum] is chi (calc),

         dchi_dformal [][N_sum] are derivatives

         Do filtering and add the derivatives w.r.t filter parameters */

    if (dchi_dformal)

      apply_filter (&(exafs_data->exafs), P,

                     &v_el (*chi, N_sum), dchi_df,

                     formal_actual, restraints->refine);

    else

      apply_filter (&(exafs_data->exafs), P,

                     &v_el (*chi, N_sum), 0, null_matrix, 0);


      /* The window is damaged */

// #if defined (USE_GRAPH)
//
//      if (!BackGround && !__zero_chi__)
//        ;
//        /* queue_enter (REDRAW, exafs_data->Gid); */
//
// #endif


      /* Add the number of points to the total */

    N_sum += N;

    }


    /* Add the restraint errors and derivatives

       The error is the last column of the restraint matrix and the
       derivative with respect to the formal_parameters is given by

       D (restraints * formal) = restraint (restraints * actual) *

                                 formal_actual (actual * formal) */

  if (chi)

    for (i = 0; i < restraint.rows; i++)

      v_el (*chi, i + N_sum) = m_el (restraint, i, restraint.cols - 1);

  restraint.cols--;

  if (dchi_dformal) {

    mul_matrix (&restraint, restraint, formal_actual);

    for (f = 0; f < formal.rows; f++)

      for (i = 0; i < restraint.rows; i++)

        v_el (dchi_dformal [f], i + N_sum) = m_el (restraint, i, f);

    }


    /* Return the number of chi values */

  return N_chi;

  }


#endif /* __CALCULATE_CHI_C__ */
