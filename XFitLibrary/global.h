/***********************************************************************}
{                                                                       }
{  global data structure                                                }
{                                                                       }
{***********************************************************************/

#if !defined (GLOBAL_H)

#define GLOBAL_H

#include "exafs_ty.h"
#include "restrain.h"
#include "paramete.h"

#include "matrix.h"
#include "commands.h"
#include "models.h"


typedef struct {

  parameter_set       * parameters;

  exafs_list         ** exafs;

  restraints_type     * restraints;

  command_name_type   * command_name_list;

  model_type          * model;

  parameter_type      * parameter_name_list;

  chi_calc_function     chi_calculate;

  rationalise_function  rationalise;

  } global_data_type;


#if defined (__XFIT_C__)

global_data_type global_data = { 0 };

#else

extern global_data_type global_data;

#endif

#endif /* GLOBAL_H */





