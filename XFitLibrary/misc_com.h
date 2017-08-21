/***********************************************************************}
{                                                                       }
{  Miscellaneous commands                                               }
{                                                                       }
{***********************************************************************/

#if !defined (__MISC_COMMANDS_H__)

#define __MISC_COMMANDS_H__

#include "read_exa.h"
#include "exafs_ty.h"
#include "paramete.h"

#include "models.h"
#include "input.h"


  /*********************************************************************}
  {                                                                     }
  {  Execute a system command                                           }
  {                                                                     }
  {*********************************************************************/

int system_command (char command []);


  /*********************************************************************}
  {                                                                     }
  {  Find a file                                                        }
  {                                                                     }
  {*********************************************************************/

const char * find_file (char filename []);


  /*********************************************************************}
  {                                                                     }
  {  Open the help file                                                 }
  {                                                                     }
  {*********************************************************************/

int help_command (const char * topic);


  /*********************************************************************}
  {                                                                     }
  {  Change the log file name                                           }
  {                                                                     }
  {*********************************************************************/

int log_command (const char * filename);


  /*********************************************************************}
  {                                                                     }
  {  Copy the saved model back into the model (restore the model)       }
  {                                                                     }
  {*********************************************************************/

int restore_command (parameter_set * P);


  /*********************************************************************}
  {                                                                     }
  {  Read in a new exafs data set                                       }
  {                                                                     }
  {*********************************************************************/

int exafs_command (parameter_set * P, exafs_list ** exafs_l,

                                      char filename []);


  /*********************************************************************}
  {                                                                     }
  {  Set a parameter value                                              }
  {                                                                     }
  {*********************************************************************/

int assignment_command (parameter_set * P, node * x, vector4 value);


  /*********************************************************************}
  {                                                                     }
  {  Clean up any temporary structures used by the exafs calculation    }
  {                                                                     }
  {*********************************************************************/

int clean_command (void);


#endif /* __MISC_COMMANDS_H__ */
