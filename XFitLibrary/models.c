/***********************************************************************}
{                                                                       }
{  This file contains the structures and routines used with models      }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__MODELS_C__)

#define __MODELS_C__

#include "models.h"

#include "feff_406.h"
#include "feff_601.h"
#include "feff_820.h"           /* Added May 2003 */
#include "xfpakg_c.h"

#include "misc_com.h"
#include "list_com.h"
#include "expressi.h"


  /*********************************************************************}
  {                                                                     }
  {  This is the list of all models used in this version                }
  {                                                                     }
  {  The first model in the list is the default model                   }
  {                                                                     }
  {*********************************************************************/

model_type model_list [] = {

  { "FEFF 8.20 (ab initio, multiple-scattering, \n"
    "          accurate polarisation, SCF and XANES)",

    "FEFF820", 5,

     FEFF_820_parameter_name,
     FEFF_820_calculate,
     FEFF_820_rationalise     },

  { "FEFF 6.01 (ab initio, multiple-scattering, \n"
    "                                      accurate polarisation)",

    "FEFF601", 5,

     FEFF_601_parameter_name,
     FEFF_601_calculate,
     FEFF_601_rationalise     },

  { "FEFF 4.06 (ab-initio, single-scattering\n"
    "                                      approximate plane polarisation)",

    "FEFF406", 3,

     FEFF_406_parameter_name,
     FEFF_406_calculate,
     FEFF_406_rationalise     },


  { "XFPAKG (empirical, single-scattering, \n"
    "                                      approximate plane polarisation)",

    "XFPAKG", 1,

     XFPAKG_parameter_name,
     XFPAKG_calculate,
     XFPAKG_rationalise       },

  { 0 }

  };


int clean_command (void);


  /*********************************************************************}
  {                                                                     }
  {  Change the model in use                                            }
  {                                                                     }
  {*********************************************************************/

int change_model (global_data_type * data, model_type * model) {

    /* Change the model in use to "model"

       All parameters which have matching names in the
       original and final models are copied            */

  parameter_set * P, P_old;

  vector old_model;

  parameter_type A, B;

  int iA;

  char * name;

  double key, key_B;

  node n;


    /* If there is no change, do nothing */

  if (data->model == model)

    return 1;


    /* Delete the restraints and constraints */

  delete_command (L_CONSTRAINTS | L_RESTRAINTS, 0, 1000000000, NULL, NULL, 
                  data->restraints);


    /* Clear the temporary parameter set */

  clear_parameter_set (&P_old);


    /* Get the parameter set */

  P = data->parameters;


  if (data->model) {

      /* Clean up after the existing model */

    clean_command ();


      /* Make space for the model vector */

    key = create_parameter_vector (&P_old, MODEL);


      /* Save the existing model vector in the temporary set */

    old_model = *parameter_vector_Oi (P, MODEL, 0);

    *parameter_vector (&P_old, key) = old_model;


      /* Empty the model vectors */

    *parameter_vector_Oi (P, MODEL, 0) = null_vector;

    free_vector (parameter_vector_Oi (P, MODEL + COPY, 0));


      /* Rationalise these empty vectors */

    if (model->rationalise)

      model->rationalise (P);


      /* Go through all of the parameters in the old model

         Copy any parameters with the same name in both models */

    key_B = parameter_vector_key_Oi (P, MODEL, 0);

    for (iA = 0; iA < old_model.rows; iA++) {

      name = parameter_name (data->parameter_name_list, iA);

      if (parameter_index (name, model->parameter_name_list, &B)) {

        B.key = key_B;

        parameter_index (name, data->parameter_name_list, &A);

        A.key = key;

        n.parameter = B;


          /* Change the rationalisation function */

        data->rationalise = model->rationalise;

        assignment_command (P, &n, parameter_value (&P_old, A));

        if (model->rationalise)

          model->rationalise (P);

        }
      }


      /* Free the old vector */

    free_vector (parameter_vector (&P_old, key));

    delete_parameter_vector (&P_old, key);

    }
  else

    if (model->rationalise)

      model->rationalise (P);


    /* Copy the links into the global data */

  data->parameter_name_list = model->parameter_name_list;
  data->chi_calculate       = model->chi_calculate;
  data->rationalise         = model->rationalise;

  data->model               = model;


    /* Return */

  return 1;

  }

#endif /* __MODELS_C__ */





