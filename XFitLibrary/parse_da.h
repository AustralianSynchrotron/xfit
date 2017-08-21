/***********************************************************************}
{                                                                       }
{  This file contains the function calling yyparse.                     }
{                                                                       }
{***********************************************************************/

#if !defined (__PARSE_DATA_H__)

#define __PARSE_DATA_H__

#include "models.h"
#include "global.h"
#include "input.h"


int read_command_file (char command_filename [],

                       parameter_set * set,

                       exafs_list ** exafs,

                       restraints_type * restraints,

                       command_name_type * command_name_list,

                       model_type * model_0);


#endif /* __PARSE_DATA_H__ */
