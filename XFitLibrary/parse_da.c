/***********************************************************************}
{                                                                       }
{  This file contains the function calling yyparse.                     }
{                                                                       }
{***********************************************************************/

#if !defined (__PARSE_DATA_C__)

#define __PARSE_DATA_C__

#include "parse_da.h"

int yyparse (void);


int read_command_file (char command_filename [],

                       parameter_set * set,

                       exafs_list ** exafs,

                       restraints_type * restraints,

                       command_name_type * command_name_list,

                       model_type * model_0) {

  static int first = 1;


  if (first) {


      /* Set the input file to the command file */

    command_in = NULL;

    if (!include_command (command_filename, &command_in))

      exit (0);

    yyin = NULL;


      /* Create the parameter vectors */

    clear_parameter_set (set);

    create_parameter_vector (set, MODEL);
    create_parameter_vector (set, MODEL + COPY);


      /* Set the list of exafs data sets to empty */

    *exafs = NULL;


      /* Set the list of restraints and constraints to empty */

    restraints->restraint  =
    restraints->constraint = NULL;


      /* No parameters to refine */

    restraints->refine     = NULL;

    restraints->formal_actual = null_matrix;
    restraints->actual_0 = null_vector;


    first = 0;

    }


    /* Create the global data global_data */

  global_data.parameters     = set;
  global_data.exafs          = exafs;
  global_data.restraints     = restraints;

  global_data.command_name_list   = command_name_list;

  global_data.parameter_name_list = NULL;
  global_data.chi_calculate       = NULL;
  global_data.rationalise         = NULL;


    /* Change the model if necessary */

  change_model (&global_data, model_0);


    /* Parse the file */

  return yyparse ();

  }


#endif /* __PARSE_DATA_C__ */
