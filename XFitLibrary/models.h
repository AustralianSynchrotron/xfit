/***********************************************************************}
{                                                                       }
{  This file contains the structures and routines used with models      }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__MODELS_H__)

#define __MODELS_H__

#include "paramete.h"


  /*********************************************************************}
  {                                                                     }
  {  Define the functions unique to each model type                     }
  {                                                                     }
  {*********************************************************************/

typedef int (* chi_calc_function) (vector,
                                   int, int,
                                   int, double [], double [],
                                        double [], double [],
                                   int, double [], double [],
                                   double *[], matrix,
                                   int);

typedef int (* rationalise_function) (parameter_set *);


  /*********************************************************************}
  {                                                                     }
  {  Define the structure used to hold the links to the models          }
  {                                                                     }
  {*********************************************************************/

typedef struct {

  char * description;

  char * name;

  short  unique;

  parameter_type * parameter_name_list;

  chi_calc_function     chi_calculate;

  rationalise_function  rationalise;

  } model_type;


  /*********************************************************************}
  {                                                                     }
  {  This is the list of all models used in this version                }
  {                                                                     }
  {  The first model in the list is the default model                   }
  {                                                                     }
  {*********************************************************************/

#if !defined (__MODELS_C__)

extern model_type model_list [];

#endif

#if !defined (GLOBAL_H)

#include "global.h"

  /*********************************************************************}
  {                                                                     }
  {  Change the model in use                                            }
  {                                                                     }
  {*********************************************************************/

int change_model (global_data_type * data, model_type * model);

#endif


#endif /* __MODELS_H__ */





