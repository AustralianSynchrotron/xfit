/***********************************************************************}
{                                                                       }
{  Command names and command name lexer                                 }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__COMMANDS_C__)

#define __COMMANDS_C__

#include "commands.h"


  /*********************************************************************}
  {                                                                     }
  {  Define the list of commands                                        }
  {                                                                     }
  {  These tokens are declared in xfit.y :                              }
  {                                                                     }
  {  INCLUDE EDGE EXAFSW FTW DATA KWEIGHT RESTORE REFINE LIST SAVE      }
  {  DELETE CLEAN EXIT MONTECARLO C_MODEL PRINT CHECK_DERIVS INVALID    }
  {                                                                     }
  {*********************************************************************/

command_name_type __command_name [] =

  { { "EXAFS",       5, DATA,         0 },
    { "XAFS",        3, DATA,         0 },
    { "DATA",        3, DATA,         0 },

    { "RESTORE",     3, RESTORE,      0 },

    { "REFINE",      3, REFINE,       0 },

    { "CLEAN",       3, CLEAN,        0 },

    { "MONTE-CARLO", 3, MONTECARLO,   0 },
    { "MONTECARLO",  3, MONTECARLO,   0 },

    { "LIST",        3, LIST,         1 },

    { "SAVE",        3, SAVE,         1 },

    { "DELETE",      3, DELETE,       1 },
    { "REMOVE",      3, DELETE,       1 },

    { "QUIT",        3, EXIT,         0 },
    { "EXIT",        3, EXIT,         0 },

    { "MODEL",       3, C_MODEL,      4 },
    { "THEORY",      3, C_MODEL,      4 },

    { "PRINT",       3, PRINT,        0 },

    { "SYSTEM",      3, SYSTEM,       0 },

    { "CHECK_DERIVATIVES",

                     7, CHECK_DERIVS, 0 },

    { 0 } };


  /*********************************************************************}
  {                                                                     }
  {  Return <> 0 if the token matches a command name                    }
  {                                                                     }
  {  The matching name structure is copied to command_entry             }
  {                                                                     }
  {*********************************************************************/

int command_name (char token [], command_name_type name [],

                                  command_name_type * command_entry) {

  for (; name->name; name++)

    if (!uniquestrlencasecmp (token, name->name, name->unique)) {

      if (command_entry)

        *command_entry = *name;

      return 1;

      }

  return 0;

  }


#endif /* __COMMANDS_C__ */





