/***********************************************************************}
{                                                                       }
{  Program xfit : analyse exafs                                         }
{                                                                       }
{***********************************************************************/

#if !defined (__XFIT_C__)

#define __XFIT_C__

#include "common.h"

#include "parse_da.h"
#include "argument.h"
#include "event.h"

#include "models.h"
#include "commands.h"


int xmain (int argc, char * argv []) {

  char command_file [128];

  int command;

  parameter_set set;

  exafs_list * exafs;

  restraints_type restraints;


    /* Check the mode of operation */

  BackGround = get_argument (&argc, argv, "background", 0, 0);

  NoAntialias    = get_argument (&argc, argv, "noantialias", 0, 0);
  NoDoubleBuffer = get_argument (&argc, argv, "nodoublebuffer", 0, 0);


    /* Call the event check to open the console and model windows */

  Poll_event (0);


    /* Get the command file name */

  strcpy (command_file, "stdin");

  if (argc >= 2)

    strcpy (command_file, argv [1]);


    /* Redirect the output file */

  if (argc >= 3)

    freopen (argv [2], "w", stdout);


    /* Execute the commands */

  command = read_command_file (command_file,

                               &set, &exafs, &restraints,

                               __command_name,

                               model_list);

  return command;

  }


#endif /* __XFIT_C__ */
