/***********************************************************************}
{                                                                       }
{  The main refinement command                                          }
{                                                                       }
{***********************************************************************/

#if !defined (__REFINE_COMMAND_C__)

#define __REFINE_COMMAND_C__

#include "input.h"
#include "event.h"
#include "global.h"

#include "console.h"

#include "refine_c.h"

#include "draw_mod.h"
#include "draw_xas.h"


#include <time.h>

#if !defined (CLOCKS_PER_SEC)

#define CLOCKS_PER_SEC 1000000

#endif

/* added by JRH by popular demand */

#define MAX_CYCLES 1000


int refine_command (

            node                * refine,

            int                   max_N,

            restraints_type     * restraint,
            parameter_set       * P,
            parameter_type      * name_list,

            int                 print ) {

  /*********************************************************************}
  {                                                                     }
  {  Change the parameter in the list to minimise chi**2                }
  {                                                                     }
  {*********************************************************************/

  static node * x;

  int i, j, parameters;

  int no_Q;

  double lambda, chi2, chi2_0, chi2_R;

  static vector formal = { 0 },

                current = { 0 };

  static matrix alpha  = { 0 },

                covar  = { 0 };

  int count, count_0, no_change;

  char name [16], l [128];

  double Cij, Cii, Cjj;

  FILE * file;

  clock_t clock0, clock_;

  double clock_D;

  double clock_time0, clock_time;


  clock0 = clock ();

  clock_time0 = 0;

  set_symbol ("INTERRUPT", "False");
  

    /* Expand any non-scalar parameters to the individual components */

  parameters = expand_refinement_parameters (name_list, refine);


    /* Check that there are parameters to refine */

  if (parameters == 0) {

    cout ("\n refine : no parameters have been specified\n\n");

    return 0;

    }


    /* Set the set of refinement parameters */

  restraint->refine = refine;


    /* List the parameters */

  cout ("\n refine : %d parameters :\n", parameters);

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


    /* Limit the number of cycles in each set */

  no_Q = BackGround;

  if (max_N < 0)

    print = 2;

  max_N = min (abs (max_N), MAX_CYCLES);

  count_0 = 0;


    /* Check to see if there are any parameters to refine */

  if (restraint->formal_actual.cols == 0) {

    cout ("\n No free variables to refine: XAFS not recalculated\n");


      /* Update the parameter set */

    for (i = 0, x = refine; x; i++, x = x->n [0])

      set_parameter (P, x->parameter,
                        make_vector1 (v_el (restraint->actual_0, i)));


      /* Print out the final parameter values */

    cout ("\n final parameters : \n\n");

    for (i = 0, x = refine; x; i++, x = x->n [0]) {

      sprintf (name, x->parameter.name, x->parameter.shell_offset);

      printfx ("   %-11s: %@12.6f (    0        )\n",

                name, v_el (formal, i));

      }

    return 0;

    }

  if (!max_N)

    return 0;


    /* Set the starting set of formal parameters to 0 */

  resize_vector (&formal, restraint->formal_actual.cols);

  clear_vector (formal);


    /* Refine the formal parameters using at most max_N cycles / set */

  lambda = -1;

  chi2 = chi2_0 = 1;

  no_change = 0;

  for (count = 0; max_N && chi2 > 1e-20 && no_change < 20; count++) {

    if ((count > count_0 && (count - count_0) % max_N == 0)

                         || get_symbol ("INTERRUPT")) {

        /* At the end of a set of cycles */

      if (get_symbol ("INTERRUPT"))

        max_N = 0;

      else

        cout (" \n Continue refinement [ <CR> = %ld cycles, 0 = exit ] : ",
                    max_N);  /* a space to avoid output issues */

      set_symbol ("INTERRUPT", "False");

      if (max_N)
      
        if (no_Q) {

          cout ("0\n");

          max_N = 0;

          }
        else {

          char * number;

          flush_cin ();

          number = Poll_event (1);

          sscanf (number, " %ld", &max_N);

          no_Q = max_N < 0;

          }

      cout ("\n");

      max_N = min (abs (max_N), MAX_CYCLES);

      if (max_N == 0)

        break;

      count_0 = count;

      }

      /* Check for any events on the queue */

    Poll_event (0);

    chi2 = marquardt_minimise_SVD (calculate_chi, formal,
                                              &covar, &alpha, &lambda);

      /* Count the number of consecutive cycles with no improvement */

    if (chi2 == chi2_0 && count > 0)

      no_change++;

    else {

        /* Update the windows */
        
      exafs_list * exafs;

      no_change = 0;

      for (exafs = *global_data.exafs; exafs; exafs = exafs->next)

        exafs->changed = 1;

      Poll_event (0);

      }


      /* If there was an improvement, save the current values */

    if (!no_change && count > 0) {

        /* (1) Calculate the current parameter values */

      mul_matrix_vector (&current, restraint->formal_actual, formal);

      for (i = 0, x = refine; x; i++, x = x->n [0])

        v_el (current, i) += v_el (restraint->actual_0, i);


        /* (2) Print the values to the file current.par */

      file = fopen ("current.par", "w");

      if (!file)

        cwarn ("can't open current.par");

      else {

        fcout (file, " \n# Current Parameters Values:\n\n");

        fcout  (file, " Cycle          : %d\n\n", count);

        fprintfx (file, " Lambda         : %@12.6f\n\n", lambda);

        fprintfx (file, " Chi2           : %@12.6f\n\n", chi2);

        fprintfx (file, " Chi2 - Chi2 (0): %@12.6f #\n\n", chi2 - chi2_0);

        for (i = 0, x = refine; x; i++, x = x->n [0]) {

          sprintf (name, x->parameter.name, x->parameter.shell_offset);

          fprintfx (file, "%-11s = %@12.6f\n", name, v_el (current, i));

          }

        fclose (file);

        }
      }


      /* Check for any events on the queue */

    Poll_event (0);


      /* Print the result of this cycle */

    sprintfx (l, " cycle %3d: chi2 = %@10.5f", count, chi2);

    if (no_change || count == 0)

      sprintf (l + strlen (l), "                   ");

    else

      sprintfx (l + strlen (l), " dchi2 = %@10.5f", chi2 - chi2_0);

    if (no_change)

      sprintf (l + strlen (l), " (%3d)", no_change);

    else

      sprintf (l + strlen (l), "      ");

    sprintfx (l + strlen (l), " l = %@10.5f", lambda);

    clock_ = clock ();

    clock_D = clock_ - clock0;

    if (clock_D < 0)

      clock_D += 4294967296.0;

    clock_time = clock_D / CLOCKS_PER_SEC + clock_time0;

    sprintf (l + strlen (l), " %8.1f\n", clock_time);

    clock0 = clock_;

    clock_time0 = clock_time;

    cout ("%s", l);

    file = fopen ("history.ref", "a");

    if (file) {

      fcout (file, "%s", l);

      fclose (file);

      }


      /* Save chi**2 */

    chi2_0 = chi2;

    }


    /* Get the covariance and curvature / free any temporary storage */

  lambda = 0;

  chi2 = marquardt_minimise_SVD (calculate_chi, formal,

                                             &covar, &alpha, &lambda);

  printfx ("\n refinement terminated at cycle %d: chi2 = %@12.6f\n",
                                              count, chi2);

  if (lambda)

    cout ("\n warning : curvature matrix was ill-behaved\n");


    /* Calculate chi2_R (chi**2 value omitting the exafs) */

  __zero_chi__ = 2;

  chi2_R = marquardt_minimise_SVD (calculate_chi, formal,

                                                  NULL, NULL, NULL);


    /* Calculate chi2_0 (chi**2 value when the calculated exafs == 0) */

  __zero_chi__ = 1;

  chi2_0 = marquardt_minimise_SVD (calculate_chi, formal,

                                                  NULL, NULL, NULL);


    /* Restore the final exafs */

  __zero_chi__ = 0;

  marquardt_minimise_SVD (calculate_chi, formal, NULL, NULL, NULL);


    /* Calculate the final parameter values */

  mul_matrix_vector (&formal, restraint->formal_actual, formal);

  for (i = 0, x = refine; x; i++, x = x->n [0])

    v_el (formal, i) += v_el (restraint->actual_0, i);


    /* Update the parameter set */

  for (i = 0, x = refine; x; i++, x = x->n [0])

    set_parameter (P, x->parameter, make_vector1 (v_el (formal, i)));


    /* Calculate the variance / covariance matrix

       If N is the number of parameters and n the actual number of
       degrees of freedom, the refinement matrix (R) is N x n,
       the correlation matrix (C) is n x n.

       The full correlation matrix (F) is given:

                 F = R C R*

                                                      */

  mul_matrix (&covar, restraint->formal_actual, covar);

  transpose_matrix (&(restraint->formal_actual));

  mul_matrix (&covar, covar, restraint->formal_actual);


    /* Print out the correlation matrix (print level >= 2) */

  if (print >= 2) {

    cout ("\n correlation matrix :\n\n");


    for (i = 0, x = refine; x; i++, x = x->n [0]) {

        /* Print the expression */

      sprintf (name, x->parameter.name, x->parameter.shell_offset);

      cout ("   %-11s:", name);

      for (j = 0, count = 15; j <= i; j++, count += 11) {

        if (count + 11 >= TERM_WIDTH) {

          cout ("\n               ");

          count = 15;

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

      }
    }


    /* Print out the final parameter values */

  cout ("\n final parameters : \n\n");

  for (i = 0, x = refine; x; i++, x = x->n [0]) {

    sprintf (name, x->parameter.name, x->parameter.shell_offset);

    printfx ("   %-11s: %@12.6f (%@12.6f )\n",

              name, v_el (formal, i), sqrt (m_el (covar, i, i)));

    }


    /* Print various chi**2 and R values */

  printfx ("\n chi2 (restraints) : %@12.6f\n", chi2_R);

  printfx (" chi2 (xafs)       : %@12.6f\n", chi2 - chi2_R);

  printfx (" chi2              : %@12.6f\n\n", chi2);

  printfx (" chi2 (0)          : %@12.6f\n", chi2_0);

  printfx (" chi2 (xafs 0)     : %@12.6f\n\n", chi2_0 - chi2_R);

  if (chi2 * chi2_0 > 0)

/* note the space after the final percent sign: this works around a bug in Kylix
   on Linux (not sure about Windows) where it fails to properly interpret the output line */

    printfx (" R                 : %@12.6f %% \n", 100 * sqrt (chi2 / chi2_0));

  if ((chi2_0 - chi2_R) * (chi2 - chi2_R) > 0)
  
    printfx (" R (xafs)          : %@12.6f %% \n",

                           100 * sqrt ((chi2 - chi2_R) / (chi2_0 - chi2_R)));

  printfx (" \n");

    /* Return 0 */

  return 0;

  }


#endif /* __REFINE_COMMAND_H__ */
