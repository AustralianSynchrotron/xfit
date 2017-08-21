/***********************************************************************}
{                                                                       }
{  Commands operating on lists                                          }
{                                                                       }
{***********************************************************************/

#if !defined (__LIST_COMMANDS_C__)

#define __LIST_COMMANDS_C__

#include "list_com.h"

#include "console.h"

#include "draw_xas.h"


char * filter_edge_name_c (filter_edge_type e) {

  int i;

  for (i = 0; filter_edge_name [i].edge; i++)

    if (filter_edge_name [i].edge == e)

      break;

  if (filter_edge_name [i].edge)

    return filter_edge_name [i].name;

  return NULL;

  }

int printf_window (window_type window) {

  if (filter_edge_name_c (window.left_edge))

    cout ("         left  :  %-10s (%10.5f ) at %10.5f\n", filter_edge_name_c (window.left_edge),
                                                           window.left_width, window.x_left);

  else

    cout ("         left  :  undefined  (%10.5f ) at %10.5f\n", window.left_width, window.x_left);

  if (filter_edge_name_c (window.right_edge))

    cout ("         right :  %-10s (%10.5f ) at %10.5f\n", filter_edge_name_c (window.right_edge),
                                                           window.right_width, window.x_right);

  else

    cout ("         right :  undefined  (%10.5f ) at %10.5f\n", window.right_width, window.x_right);

  return 1;

  }


  /*********************************************************************}
  {                                                                     }
  {  List model parameters, xafs files, restraints or constraints      }
  {                                                                     }
  {*********************************************************************/

int list_command (list_command_type type,

                   int i_from, int i_to,

                   parameter_set * P,

                   exafs_list * exafs,

                   restraints_type * restraints,

                   parameter_type * name_list) {

  int i, n;

  char * name, line [256];

  parameter_type entry;

  restraint_type * C;

  vector model, model_0;

  int shell, shell_0;

  _v3_ * x3, * n3;


     /* Model - lists model parameters and their saved values */

  if (type & L_MODEL) {

    shell_0 = 2000000000;

    model = *parameter_vector_Oi (P, MODEL, 0);

    model_0 = *parameter_vector_Oi (P, MODEL + COPY, 0);

    if (model.rows)

      cout ("\n  Current model parameter values :\n"
              "\n    parameter       value      saved value\n\n");

    else

      cout ("\n  No current model parameters\n");

    line [0] = 0;

    for (i = 0; i < model.rows; i++) {

      name = parameter_name (name_list, i);

      parameter_index (name, name_list, &entry);

      if (i > 0 && strchr (entry.name, '%') &&

                          entry.shell_offset != shell_0) {

        shell_0 = entry.shell_offset;

        strcat (line, "\n");

        }

      sprintf (line + strlen (line), "    %-11s: ", name);

      if (entry.type == ATOM_t && atom_symbol (v_el0 (model, i)))

        sprintfx (line + strlen (line), "%@12.6s ", atom_symbol (v_el0 (model, i)));

      else

        sprintfx (line + strlen (line), "%@12.6f ", v_el0 (model, i));

      if (entry.type == ATOM_t && atom_symbol (v_el0 (model_0, i)))

        sprintfx (line + strlen (line), "%@12.6s\n", atom_symbol (v_el0 (model_0, i)));

      else

        sprintfx (line + strlen (line), "%@12.6f\n", v_el0 (model_0, i));

      if (line [0])

        if (line [strlen (line) - 1] == '\n') {

          cout ("%s", line);

          line [0] = 0;

          }
      }
    }


     /* Data set - lists exafs data set parameters */

  if (type & L_DATA_SET) {

    if (exafs)

      cout ("\n  Current XAFS data files :\n");

    else

      cout ("\n  No current XAFS data files\n");

    for (i = 1; exafs; exafs = exafs->next, i++) {

      if (i < i_from || i > i_to)

        continue;


        /* Update the exafs structure from the parameter set */

      copy_set_exafs_data (&(exafs->exafs), P);


        /* Print the information :

           file name */

      cout ("\n  %2ld : %s\n", i, exafs->exafs.ascii_file.filename);


        /* Comments in the file */

      fprint_comment (stdout, " \n   %4d: ", exafs->exafs.ascii_file.comment);


        /* Commands in the file */

      cout ("\n      commands: \"%s\"\n", exafs->exafs.ascii_file.command);


        /* Absorbing atom */

      if (atom_symbol (exafs->exafs.absorber))

        cout ("\n       absorber: %s", atom_symbol (exafs->exafs.absorber));

      else

        cout ("\n       absorber: undefined");


        /* X-ray edge */

      if (X_ray_edge_symbol (exafs->exafs.edge))

        cout ("    edge: %s\n", X_ray_edge_symbol (exafs->exafs.edge));

      else

        cout ("    edge: undefined\n");


        /* E0 energy */

      cout ("\n       E0 = %.3f eV\n", exafs->exafs.E0);


        /* Curves obtained from the file */

      cout ("\n       %ld points (energy, XAFS", exafs->exafs.N_exp);

      if (exafs->exafs.schi_exp)

        cout (", smoothed XAFS");

      if (exafs->exafs.sigma)

        cout (", esd");

      cout (")\n");


        /* Polarisation and occupancy */

      if (exafs->exafs.shells) {

        x3 = (_v3_ *) exafs->exafs.polarisation;
        n3 = (_v3_ *) exafs->exafs.normal;

        cout ("\n       shell  "
                "      polarisation       "
                "         normal       "
                "ellipticity  O\n\n");

        for (shell = 0; shell < exafs->exafs.shells; shell++) {

          cout ("       %3d   ", shell);

          if (length3 (x3 [shell]))

            printfx ("(%7.3f,%7.3f,%7.3f) ",

                        x3 [shell][0], x3 [shell][1], x3 [shell][2]);

          else

            cout ("       unpolarised        ");

          if (length3 (n3 [shell]))

            printfx ("(%7.3f,%7.3f,%7.3f)",

                        n3 [shell][0], n3 [shell][1], n3 [shell][2]);

          else

            cout ("                         ");

          if (exafs->exafs.ellipticity [shell])

            printfx ("%7.3f", exafs->exafs.ellipticity [shell]);

          else

            cout ("       ");

          printfx (" %7.3f\n", exafs->exafs.O [shell]);

          }
        }


        /* W and kW */

      cout ("\n       weight (k) = %.3f k ** %.3f\n",

                                      exafs->exafs.W, exafs->exafs.kW);

        /* FFT information */

      cout ("\n       filter : N = %d, maximum r = %.3f\n",

                                      exafs->exafs.N, exafs->exafs.r_max);

      cout ("\n                (r step = %.5f, k step = %.5f)\n",

                  delta_r (exafs->exafs.r_max, exafs->exafs.N),
                  delta_k (exafs->exafs.r_max));


        /* Filter windows */

      cout ("\n       window in k-space:\n\n");

      printf_window (exafs->exafs.k_window);

      cout ("\n       window in r-space:\n\n");

      printf_window (exafs->exafs.r_window);

      }
    }


     /* Restraints and constraints - list restraints and constraints */

  if (type & (L_RESTRAINTS | L_CONSTRAINTS)) {

    if (type & L_RESTRAINTS) {

      if (restraints->restraint)

        cout ("\n  Current restraints :\n\n");

      else

        cout ("\n  No current restraints\n");

      for (n = 1, C = restraints->restraint; C; C = C->next, n++) {

        char * exp;
        
        exp = expression_string (C->n, 0);

        if (exp)
        
          cout ("   %3d : %s\n", n, exp);

        }
      }

    if (type & L_CONSTRAINTS) {

      if (restraints->constraint)

        cout ("\n  Current constraints :\n\n");

      else

        cout ("\n  No current constraints\n");

      for (n = 1, C = restraints->constraint; C; C = C->next, n++) {

        char * exp;
        
        exp = expression_string (C->n, 0);

        if (exp)
        
          cout ("   %3d : %s\n", n, exp);

        }
      }
    }

  cout ("\n");

  return 0;

  }


  /*********************************************************************}
  {                                                                     }
  {  Save the model, exafs data sets, restraints or constraints         }
  {                                                                     }
  {  The file produced consists of xfit commands                        }
  {                                                                     }
  {*********************************************************************/

int save_command (list_command_type type,

                   int i_from, int i_to,

                   char filename [],

                   parameter_set * P,

                   exafs_list * exafs,

                   restraints_type * restraints,

                   parameter_type * name_list) {

    /* Saves parameters, exafs files, restraints and constraints in a file */

  static char file_name [256];   /* do not confuse with "filename", and note
                                    that this is a static variable, so it
                                    is constant between invocations */

  int i;

  char * name;

  parameter_type data;

  restraint_type * C;

  FILE * file;

  vector model;

  _v3_ * x3, * n3;

  int shell;


    /* If the filename is defined, save it and exit */

  if (filename) {

    file_name [0] = 0;     /* i.e. our static name */
    

      /* If the filename is "" request it */

    if (!filename [0]) {           /* i.e. a null character */

      filename = get_symbol ("OUTPUT FILE");

      if (filename)

        strcpy (file_name, filename); /* copy to our local value */

      }
    else

      strcpy (file_name, filename);


      /* Clear the file */

    if (file_name [0]) {

      file = fopen (file_name, "w");

      fprintf (file, " # File generated by the XFIT SAVE command #\n\n");
    
      fclose (file);

      }

    return 0;

    }


  if (!file_name [0])

    return 0;

    
    /* Open the file */

  file = fopen (file_name, "a");


    /* Model - saves the current model */

  if (type & L_MODEL) {

      /* Save the model type */

    fcout (file, "theory %s\n\n", global_data.model->name);


      /* Get the model vector */

    model = *parameter_vector_Oi (P, MODEL, 0);


      /* Save the model parameters */

    for (i = 0; i < model.rows; i++) {

      name = parameter_name (name_list, i);

      parameter_index (name, name_list, &data);

      fcout (file, "%-11s= ", name);

      if (data.type == ATOM_t)

        fprintfx (file, "%@12.6s\n",

                         atom_symbol (v_el (model, i)));

      else

        fprintfx (file, "%@12.6f\n", v_el (model, i));

      }
    }


    /* Data set - save the exafs data sets */

  if (type & L_DATA_SET) {

    for (i = 1; exafs; exafs = exafs->next, i++) {

      if (i < i_from || i > i_to)

        continue;


        /* Update the exafs structure from the parameter set */

      copy_set_exafs_data (&(exafs->exafs), P);


        /* Print the information :

           file name */

      fcout (file, "\nxafs \"%s\"\n\n", exafs->exafs.ascii_file.filename);


        /* Absorbing atom */

      fcout (file, "absorber[0] = ");

      if (atom_symbol (exafs->exafs.absorber))

        fcout (file, atom_symbol (exafs->exafs.absorber));

      else

        fcout (file, "0");

      fcout (file, "\n");


        /* X-ray edge */

      fcout (file, "edge[0]     = ");

      if (X_ray_edge_symbol (exafs->exafs.edge))

        fcout (file, X_ray_edge_symbol (exafs->exafs.edge));

      else

        fcout (file, "0");

      fcout (file, "\n");


        /* E0 energy */

      fcout (file, "E0[0]       = %.3f\n\n", exafs->exafs.E0);


        /* Polarisation and occupancy */

      if (exafs->exafs.shells) {

        x3 = (_v3_ *) exafs->exafs.polarisation;
        n3 = (_v3_ *) exafs->exafs.normal;

        for (shell = 0; shell < exafs->exafs.shells; shell++) {

          fprintfx (file, "polarisation%1d[0] = [%@10.5f, %@10.5f, %@10.5f]\n",

                     shell, x3 [shell][0], x3 [shell][1], x3 [shell][2]);

          fprintfx (file, "normal%1d[0]       = [%@10.5f, %@10.5f, %@10.5f]\n",

                     shell, n3 [shell][0], n3 [shell][1], n3 [shell][2]);

          fprintfx (file, "ellipticity%1d[0]  = %@10.5f\n\n",

                     shell, exafs->exafs.ellipticity [shell]);

          fprintfx (file, "O%1d[0]            = %@10.5f\n\n",

                     shell, exafs->exafs.O [shell]);

          }
        }


        /* W and kW */

      fcout (file, "w[0]        = %.5f\n", exafs->exafs.W);
      fcout (file, "kw[0]       = %.5f\n", exafs->exafs.kW);


        /* FFT information */

      fcout (file, "N[0]        = %d\n", exafs->exafs.N);
      fcout (file, "rmax[0]     = %.5f\n", exafs->exafs.r_max);


        /* Filter windows */

      fcout (file, "k0[0]       = %.5f\n", exafs->exafs.k_window.x_left);
      fcout (file, "k1[0]       = %.5f\n", exafs->exafs.k_window.x_right);
      fcout (file, "kw0[0]      = %.5f\n", exafs->exafs.k_window.left_width);
      fcout (file, "kw1[0]      = %.5f\n", exafs->exafs.k_window.right_width);

      fcout (file, "ke0[0]      = ");

      if (filter_edge_name_c (exafs->exafs.k_window.left_edge))

        fcout (file, filter_edge_name_c (exafs->exafs.k_window.left_edge));

      else

        fcout (file, "undefined");

      fcout (file, "\n");

      fcout (file, "ke1[0]      = ");

      if (filter_edge_name_c (exafs->exafs.k_window.right_edge))

        fcout (file, filter_edge_name_c (exafs->exafs.k_window.right_edge));

      else

        fcout (file, "undefined");

      fcout (file, "\n");

      fcout (file, "r0[0]       = %.5f\n", exafs->exafs.r_window.x_left);
      fcout (file, "r1[0]       = %.5f\n", exafs->exafs.r_window.x_right);
      fcout (file, "rw0[0]      = %.5f\n", exafs->exafs.r_window.left_width);
      fcout (file, "rw1[0]      = %.5f\n", exafs->exafs.r_window.right_width);

      fcout (file, "re0[0]      = ");

      if (filter_edge_name_c (exafs->exafs.r_window.left_edge))

        fcout (file, filter_edge_name_c (exafs->exafs.r_window.left_edge));

      else

        fcout (file, "undefined");

      fcout (file, "\n");

      fcout (file, "re1[0]      = ");

      if (filter_edge_name_c (exafs->exafs.r_window.right_edge))

        fcout (file, filter_edge_name_c (exafs->exafs.r_window.right_edge));

      else

        fcout (file, "undefined");

      fcout (file, "\n");

      }
    }


    /* Restraints and constraints - save the restraints and constraints */

  if (type & (L_RESTRAINTS | L_CONSTRAINTS)) {

      /* Save the restraints */

    if (type & L_RESTRAINTS)

      for (i = 1, C = restraints->restraint; C; C = C->next, i++)

        if (i >= i_from && i <= i_to) {

          fprintf_expression (file, C->n, 0);

          fcout (file, "\n");

          }


      /* Save the constraints */

    if (type & L_CONSTRAINTS)

      for (i = 1, C = restraints->constraint; C; C = C->next, i++)

        if (i >= i_from && i <= i_to) {

          fprintf_expression (file, C->n, 0);

          fcout (file, "\n");

          }
    }

  fcout (file, "\n");

  fclose (file);


    /* Return 0 */

  return 0;

  }


  /*********************************************************************}
  {                                                                     }
  {  Delete exafs data sets, restraints or constraints                  }
  {                                                                     }
  {*********************************************************************/

int delete_command (list_command_type type,

                    int i_from, int i_to,

                    parameter_set * P,

                    exafs_list ** exafs,

                    restraints_type * restraints) {

    /* Deletes exafs files, restraints and constraints */

  int i;

  exafs_list * e;

  restraint_type ** C, * C_;


    /* Delete exafs data sets */

  if (type & L_DATA_SET) {

    for (i = 1; *exafs; i++)

      if (i >= i_from && i <= i_to) {

        close_exafs_window (*exafs);

        e = (*exafs)->next;

        destruct_ascii_data (&((*exafs)->exafs.ascii_file));

        free_check ((*exafs)->exafs.energy);
        free_check ((*exafs)->exafs.chi_exp);
        free_check ((*exafs)->exafs.sigma);
        free_check ((*exafs)->exafs.schi_exp);
        free_check ((*exafs)->exafs.chi_exp_f);
        free_check ((*exafs)->exafs.chi_f);

        free_check ((*exafs)->exafs.density);

        free_vector (parameter_vector (P, (*exafs)->exafs.key));

        delete_parameter_vector (P, (*exafs)->exafs.key);

        free_check (*exafs);

        (*exafs) = e;

        }
      else

        exafs = &(*exafs)->next;

    }


    /* Delete restraints */

  if (type & L_RESTRAINTS) {

    for (i = 1, C = &(restraints->restraint); *C; i++)

      if (i >= i_from && i <= i_to) {

        C_ = (*C)->next;

        free_tree ((*C)->n);
        free_check (*C);

        (*C) = C_;

        }
      else

        C = &(*C)->next;

    }


    /* Delete constraints */

  if (type & L_CONSTRAINTS) {

    for (i = 1, C = &(restraints->constraint); *C; i++)

      if (i >= i_from && i <= i_to) {

        C_ = (*C)->next;

        free_tree ((*C)->n);
        free_check (*C);

        (*C) = C_;

        }
      else

        C = &(*C)->next;

    }


    /* Return 0 */

  return 0;

  }

#endif /* __LIST_COMMANDS_C__ */
