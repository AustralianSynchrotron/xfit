/***********************************************************************}
{                                                                       }
{  This file contains the Monte-Carlo error estimation routine.         }
{                                                                       }
{***********************************************************************/

#if !defined (__MONTECARLO_C__)

#define __MONTECARLO_C__

#include "montecar.h"

#include "console.h"


int montecarlo_command

          ( node                * refine,
            node                * too [],

            int                   MC_N,
            int                   max_N,

            restraints_type     * restraint,
            parameter_set       * P,
            exafs_list         ** exafs,
            parameter_type      * name_list ) {

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

  static node * too_0 = NULL, * x;

  int i, j, n, ntoo, parameters, order;

  int exafs_error;

  exafs_list * e;

  double ** refined, * o_exafs, lambda, chi2, chi2_0;

  int MC_cycle;

  static vector formal = { 0 };

  static matrix alpha  = { 0 },

                covar  = { 0 };

  int count, no_change;

  char name [16];

  double Cij, Cii, Cjj;

  double av, esd, esdesd;


    /* Check that too is at least an empty list */

  if (too == NULL)

    too = &too_0;


    /* Count the number of too scalar expressions */

  ntoo = 0;

  for (i = 0; too [i]; i++)

    ntoo += reduce_tree (too [ntoo], null_parameter, P);


    /* Expand any non-scalar parameters to the individual components */

  parameters = expand_refinement_parameters (name_list, refine);


    /* Check that there are parameters to refine */

  if (parameters == 0) {

    cout ("\n Monte-Carlo : no parameters have been specified\n\n");

    return 0;

    }


    /* Set the set of refinement parameters */

  restraint->refine = refine;


    /* List the parameters */

  cout ("\n Monte-Carlo : %d parameters :\n\n         ", parameters);

  list_refinement_parameters (refine);

  cout ("\n");


    /* Get the linear constraint matrix */

  i = c_constraint_matrix (&(restraint->formal_actual), restraint->constraint,

                             P, refine);

  if (i == 0) {

    fprintf_constraint_error (stdout, restraint->constraint);

    return 0;

    }


    /* Break the constraint matrix into a matrix and a vector */

  resize_vector (&(restraint->actual_0), restraint->formal_actual.rows);

  for (i = 0; i < restraint->formal_actual.rows; i++)

    v_el (restraint->actual_0, i) =

       m_el (restraint->formal_actual, i, restraint->formal_actual.cols - 1);

  restraint->formal_actual.cols--;


    /* Limit the number of cycles in each refinement */

  max_N = min (abs (max_N), 100);

  if (max_N == 0)

    return 0;


    /* Check that the exafs curves include error estimates */

  exafs_error = 0;

  for (e = *exafs; e; e = e->next) {

    if (e->exafs.sigma == NULL) {

      cout (" Monte-Carlo: XAFS file %s is missing sigma\n\n",
                e->exafs.ascii_file.filename);

      exafs_error = 1;

      }
    }

  if (exafs_error)

    return 0;


    /* Save the exafs curves */

  for (e = *exafs; e; e = e->next) {

    e->exafs.save = calloc_check (e->exafs.N_exp, sizeof (double));

    for (i = 0; i < e->exafs.N_exp; i++)

      e->exafs.save [i] = e->exafs.chi_exp [i];

    }


    /* A minimum of 2 Monte-Carlo cycles must be executed */

  MC_N = max (2, MC_N);


    /* Get space to save the refined parameters and additional values */

  refined = calloc_check (parameters + ntoo, sizeof (double *));

  for (i = 0; i < parameters + ntoo; i++)

    refined [i] = calloc_check (MC_N, sizeof (double));


    /* Execute MC_N Monte-Carlo cycles */

  for (MC_cycle = 0; MC_cycle < MC_N; MC_cycle++) {

    cout ("\n Monte-Carlo cycle: %3d\n\n", MC_cycle + 1);


      /* Generate the exafs data for this cycle:

         Add noise to the (smoothed) exafs */

    for (e = *exafs; e; e = e->next) {

      o_exafs = e->exafs.save;

      if (e->exafs.schi_exp)

        o_exafs = e->exafs.schi_exp;

      for (i = 0; i < e->exafs.N_exp; i++)

        e->exafs.chi_exp [i] =  e->exafs.sigma [i] * grandom () + o_exafs [i];

      }


      /* Set the starting set of formal parameters to 0 */

    resize_vector (&formal, restraint->formal_actual.cols);

    clear_vector (formal);


      /* Refine the formal parameters using at most max_N cycles */

    lambda = -1;

    chi2 = chi2_0 = 1;

    no_change = 0;

    for (count = 0; chi2 > 1e-20 && no_change < 10 && count < max_N; count++) {

      chi2 = marquardt_minimise_SVD (calculate_chi, formal,

                                              &covar, &alpha, &lambda);


        /* Check for any events on the queue */

      Poll_event (0);


        /* Count the number of consecutive cycles with no improvement */

      if (chi2 == chi2_0 && count > 0)

        no_change++;

      else

        no_change = 0;


        /* Print the result of this cycle */

      printfx (" cycle %3d: chi2 = %@10.5f", count, chi2);

      if (no_change || count == 0)

        cout ("                   ");

      else

        printfx (" dchi2 = %@10.5f", chi2 - chi2_0);

      if (no_change)

        cout (" (%3d)", no_change);

      else

        cout ("      ");

      printfx (" l = %@10.5f\n", lambda);


        /* Save chi**2 */

      chi2_0 = chi2;

      }


      /* Get the covariance and curvature / free any temporary storage */

    lambda = 0;

      /* Check for any events on the queue */

    Poll_event (0);

    chi2 = marquardt_minimise_SVD (calculate_chi, formal,

                                             &covar, &alpha, &lambda);

      /* Check for any events on the queue */

    Poll_event (0);

    printfx ("\n Monte-Carlo cycle %3d terminated at cycle"
             " %d: chi2 = %@12.6f\n", MC_cycle + 1, count, chi2);

    if (lambda)

      cout ("\n warning : curvature matrix was ill-behaved\n");


      /* Calculate the final parameter values */

    mul_matrix_vector (&formal, restraint->formal_actual, formal);

    for (i = 0, x = refine; x; i++, x = x->n [0])

      v_el (formal, i) += v_el (restraint->actual_0, i);


    for (i = 0, x = refine; x; i++, x = x->n [0]) {

        /* Save the final parameter values in refined [...][MC_cycle] */

      refined [i][MC_cycle] = v_el (formal, i);


        /* Update the parameter set */

      set_parameter (P, x->parameter, make_vector1 (v_el (formal, i)));

      }


      /* Add the extra values to refined [...][MC_cycles] */

    ntoo = 0;

    for (i = 0; too [i]; i++) {

      order = reduce_tree (too [ntoo], null_parameter, P);

      for (j = 0; j < order; j++)

        refined [ntoo++ + parameters][MC_cycle] = v_el (too [i]->f, j);

      }
    }


    /* Allocate space for the variance / covariance matrix */

  resize_matrix (&covar, parameters + ntoo, parameters + ntoo);

  clear_matrix (covar);


    /* Calculate the covariance matrix */

  for (i = 0; i < parameters + ntoo; i++)

    for (j = 0; j <= i; j++)

      m_el (covar, i, j) = m_el (covar, j, i)

                         = UCovariance (MC_N, refined [i], refined [j]);


    /* Print out the correlation matrix */

  cout ("\n correlation matrix :\n\n");


    /* If order > 0 this is the number of elements of x left to print */

  order = 0;

  for (i = n = 0, x = NULL; i < parameters + ntoo; i++) {

      /* Get the next expression */

    if (order <= 0) {

      if (x)

        x = x->n [0];

      if (i == 0)

        x = refine;

      if (i >= parameters)

        x = too [n++];


        /* Check the number of elements */

      if (i >= parameters)

        order = x->f.rows;


        /* Print the expression */

      if (i < parameters) {

        sprintf (name, x->parameter.name, x->parameter.shell_offset);

        cout ("   %-10s:", name);

        }
      else {

        cout ("\n   ");

        fprintf_expression (stdout, x, 0);

        cout (" :\n              ");

        }
      }

    for (j = 0, count = 14; j <= i; j++, count += 11) {

      if (count + 11 >= TERM_WIDTH) {

        cout ("\n              ");

        count = 14;

        }

      cout ("  ");

      Cij = m_el (covar, i, j);
      Cii = m_el (covar, i, i);
      Cjj = m_el (covar, j, j);

      if (i == j)

        Cij = Cij > 0 ? 1 : (Cij < 0 ? -1 : 0);

      else

        if (Cii != 0 && Cjj != 0)

          Cij /= (sqrt (fabs (Cii * Cjj)));

        else

          Cij = 0;

      if (fabs (Cij) >= 1e-12)

        printfx ("%@10.5f", Cij);

      else

        printfx ("%@10.5f", 0.0);

      }

    cout ("\n");

    order--;

    }


    /* Print out the parameter average  and esd values. */

  cout ("\n final parameters : \n\n");

  for (i = 0, x = NULL; i < parameters + ntoo; i++) {

    if (x)

      x = x->n [0];

    if (i == 0)

      x = refine;

    if (i >= parameters)

      x = too [i - parameters];

    if (i < parameters) {

      sprintf (name, x->parameter.name, x->parameter.shell_offset);

      cout ("   %-10s:", name);

      }
    else {

      cout ("\n   ");

      fprintf_expression (stdout, x, 0);

      cout (" :\n              ");

      }


      /* Get the average */

    for (av = 0, MC_cycle = 0; MC_cycle < MC_N; MC_cycle++)

      av += refined [i][MC_cycle];

    av /= MC_N;

    esd = sqrt (m_el (covar, i, i));

    esdesd = EsdEsd (MC_N, esd);

    if (i < parameters) {

        /* Set the parameters back to the original value */

      set_parameter (P, x->parameter,

                        make_vector1 (v_el (restraint->actual_0, i)));

      }


      /* Print the average, the esd and the esd of the esd */

    printfx ("%@12.6f (%@12.6f (%@12.6f))\n", av, esd, esdesd);

    }

  cout ("\n");


    /* Free the memory */

  for (i = 0; i < parameters + ntoo; i++)

    free_check (refined [i]);

  free_check (refined);


    /* Restore the exafs curves */

  for (e = *exafs; e; e = e->next) {

    for (i = 0; i < e->exafs.N_exp; i++)

      e->exafs.chi_exp [i] = e->exafs.save [i];

    free_check (e->exafs.save);

    e->exafs.save = NULL;

    }


    /* Return 0 */

  return 0;

  }

#endif /* __MONTECARLO_H__ */
