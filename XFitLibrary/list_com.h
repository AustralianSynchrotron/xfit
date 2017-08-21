/***********************************************************************}
{                                                                       }
{  Commands operating on lists                                          }
{                                                                       }
{***********************************************************************/

#if !defined (__LIST_COMMANDS_H__)

#define __LIST_COMMANDS_H__

#include "common.h"

#include "fourier_.h"
#include "paramete.h"
#include "exafs_ty.h"
#include "expressi.h"
#include "restrain.h"


#include "print.h"
#include "global.h"


typedef enum {

  L_MODEL       = 1,
  L_DATA_SET    = 2,
  L_RESTRAINTS  = 4,
  L_CONSTRAINTS = 8

  } list_command_type;

typedef struct {

  char * name;

  short  unique;

  list_command_type type;

  } list_command_name_type;


#if !defined (__LIST_COMMANDS_C__)

extern list_command_name_type __list_command_name [];

#else

list_command_name_type __list_command_name [] =

  { { "ALL",         1, L_MODEL       |
                        L_DATA_SET    |
                        L_RESTRAINTS  |
                        L_CONSTRAINTS   },

    { "PARAMETERS",  1, L_MODEL         },
    { "MODEL",       1, L_MODEL         },
    { "EXAFS",       1, L_DATA_SET      },
    { "XAFS",        1, L_DATA_SET      },
    { "DATA",        1, L_DATA_SET      },
    { "RESTRAINTS",  1, L_RESTRAINTS    },
    { "CONSTRAINTS", 1, L_CONSTRAINTS   },

    { 0 } };

#endif


  /*********************************************************************}
  {                                                                     }
  {  List model parameters, exafs files, restraints or constraints      }
  {                                                                     }
  {*********************************************************************/

int list_command (list_command_type type,

                   int i_from, int i_to,

                   parameter_set * P,

                   exafs_list * exafs,

                   restraints_type * restraints,

                   parameter_type * name_list);


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

                   parameter_type * name_list);


  /*********************************************************************}
  {                                                                     }
  {  Delete exafs data sets, restraints or constraints                  }
  {                                                                     }
  {*********************************************************************/

int delete_command (list_command_type type,

                    int i_from, int i_to,

                    parameter_set * P,

                    exafs_list ** exafs,

                    restraints_type * restraints);


#endif /* __LIST_COMMANDS_H__ */
