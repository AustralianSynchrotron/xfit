/***********************************************************************}
{                                                                       }
{  Command names and command name lexer                                 }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__COMMANDS_H__)

#define __COMMANDS_H__

#include "common.h"


  /*********************************************************************}
  {                                                                     }
  {  Define the structure holding the names of the commands             }
  {                                                                     }
  {*********************************************************************/

typedef struct {

  char  * name;

  short   unique;

  int     type;

  short   lex_state;

  } command_name_type;


  /*********************************************************************}
  {                                                                     }
  {  Define the list of commands                                        }
  {                                                                     }
  {*********************************************************************/

#if !defined (__COMMANDS_C__)

extern command_name_type __command_name [];

#endif


  /*********************************************************************}
  {                                                                     }
  {  Return <> 0 if the token matches a command name                    }
  {                                                                     }
  {  The matching name structure is copied to command_entry             }
  {                                                                     }
  {*********************************************************************/

int command_name (char token [], command_name_type name [],

                                  command_name_type * command_entry);


#endif /* __COMMANDS_H__ */





