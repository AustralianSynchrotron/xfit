/***********************************************************************}
{                                                                       }
{  Miscellaneous commands                                               }
{                                                                       }
{***********************************************************************/

#if !defined (__MISC_COMMANDS_C__)

#define __MISC_COMMANDS_C__

#include "misc_com.h"

#include "console.h"

#include "draw_xas.h"

#include <sys/types.h>

#if !defined(WIN32)
#include <unistd.h>
#include <sys/wait.h>
#else
#include <process.h>
#endif

#include <errno.h>
#define _MAX_PATH 80

  /*********************************************************************}
  {                                                                     }
  {  Execute a system command.  We get a string which we split into
     fields.                                                            }
  {                                                                     }
  {*********************************************************************/

#undef DELETE
#include <string.h>

int system_command (char command []) {

#if !defined(WIN32)
  pid_t newpid;
#else

#endif
  int status, execstatus, i;
  int curtoken;
  char * commandlist[20];     /* no more than 20 arguments of length 20 */
  char * nextpos;

  curtoken = 0;                    /* extract commands */
  nextpos = strtok(command, " ");
  while(nextpos != NULL) {
      commandlist[curtoken] = calloc(1, 20 * sizeof(char));
      strncpy(commandlist[curtoken], nextpos,20);
      curtoken++;
      nextpos = strtok(NULL," ");
      }

  commandlist[curtoken] = NULL;      /* must be null terminated */

#if !defined(WIN32)
  if((newpid = fork())!= -1) {
      if(newpid == 0) {     /* child process */
          printf("Executing %s\n",commandlist[0]);
          execstatus = execvp(commandlist[0],commandlist);
          printf ("Failed to execute: %d\n", execstatus);
          }
      else {
          waitpid(newpid,&status,0);
          printf ("%s exited with status %d,\n",commandlist[0],status);
          }
      }
#else
   printf("Executing %s\n",commandlist[0]);
   _spawnvp(_P_WAIT,commandlist[0],commandlist+1);
#endif
  }


  /*********************************************************************}
  {                                                                     }
  {  Find a file                                                        }
  {                                                                     }
  {*********************************************************************/

const char * find_file (char filename []) {

  char * path;
  char * findex;
  char localcopy[_MAX_PATH];           /* copy into here to start*/
  static char qualified[_MAX_PATH];    /* we return this address, so make it static */

#if defined(WIN32)
  char drive[_MAX_DRIVE], dir[_MAX_DIR]; /* for Windows path extraction */
  char ext[3];
  char fname[_MAX_FNAME];
#endif

  int curtoken,i,qualpos;
  int cycle;
  FILE * file;

    /* If the filename is fully qualified */
#if !defined(WIN32)

  if (filename[0] == '/')
        
      if (file = fopen (filename, "r")) {

        fclose (file);
        
        return (filename);

        }

#endif
/* Look for a fully qualified name in Windows */
#if defined(WIN32)

  _splitpath (filename, drive, dir, fname, ext);
  if (drive != NULL) {   /* ie fully qualified */
      if (file = fopen (filename, "r")) {
		  fclose (file);
		  return(filename);
	  }
  }
  strncpy(localcopy,filename,_MAX_PATH);
  qualified[0] = '/';              /* full name starts with / */
#endif
  /* findex = rindex(localcopy,'/');
  strncpy(fname,findex+1,_MAX_PATH);
  findex[0] = 0;                   /* terminate here 
  strncpy(qualified+1,localcopy,_MAX_PATH-1);  /* put rest elsewhere
*/

    /* If the filename is partially qualified */
    /* we use environment variables */

  path = getenv ("XFIT_DATA_DIR");

/* bonus under Unix: we can get the current directory */

  if (!path) path = getenv ("PWD");

  if (path) {

      char * s, * d;

      s = path;

      file = NULL;

      while (*s == ' ')

          s++;                /* remove blanks */

      for (d = qualified; *s && *s != ';'; s++, d++)

          *d = *s;            /* copy until semicolon or null */
      /* put in the blank*/
      d[0]=0x0;

      while (d > qualified && d[-1] == ' ')
          {
          d[-1] = 0x0;         /*remove trailing blanks*/
          d--;
          }

      if (d [-1] != '/') /* no separator */

          strcat (qualified, "/"); /* add */

      strcat (qualified, filename);  /* add name on */

      file = fopen (qualified, "r");

      if (file) {

          fclose (file);

          return qualified;

        }
    }

  return NULL;

  }


  /*********************************************************************}
  {                                                                     }
  {  Open the help file                                                 }
  {                                                                     }
  {*********************************************************************/

int help_command (const char * topic) {

  printf("Oh dear, no help available yet\n");
  return("None");

  }


  /*********************************************************************}
  {                                                                     }
  {  Change the log file name                                           }
  {                                                                     }
  {*********************************************************************/

int log_command (const char * filename) {

  printf("Can't change log file name.\n");
  return 1;

  }


  /*********************************************************************}
  {                                                                     }
  {  Copy the saved model back into the model (restore the model)       }
  {                                                                     }
  {*********************************************************************/

int restore_command (parameter_set * P) {

  vector * saved_model, * model;

  int i;


    /* Get the vectors */

  saved_model = parameter_vector_Oi (P, MODEL + COPY, 0);

  model = parameter_vector_Oi (P, MODEL, 0);


    /* Copy the saved model */

  for (i = 0; i < saved_model->rows; i++)

    v_el (*model, i) = v_el (*saved_model, i);


    /* Return 0 */

  return 0;

  }


  /*********************************************************************}
  {                                                                     }
  {  Read in a new exafs data set                                       }
  {                                                                     }
  {*********************************************************************/

int exafs_command (parameter_set * P, exafs_list ** exafs_l,

                                      char filename []) {

  exafs_list * exafs;

  vector * v, * vc;

  int N, n;

  double key;


    /* get the filename if necessary */

  if (filename)

    if (filename [0] == 0)

      filename = NULL;


  if (!filename)

    filename = get_symbol ("DATA FILE");

  if (filename)

    if (filename [0] == 0)

      filename = NULL;


  if (!filename) {

    cout ("\n no file specified\n\n");

    return 0;

    }


    /* find the pointer to the last entry in the list of exafs data files */

  for (n = 0; *exafs_l; exafs_l = &(*exafs_l)->next, n++);


    /* Allocate space for the new exafs structure */

  exafs = calloc_check_ (1, sizeof (exafs_list), "exafs_command");

  if (!exafs)

    return -2;


    /* Clear the structure */

  clear_exafs (&(exafs->exafs));

  exafs->next = NULL;

  exafs->changed = 1;


    /* Read the data file */

  N = read_exafs_file (filename, &(exafs->exafs));


        /* Check for errors */

  if (N == 0) {

    cwarn ("error reading XAFS file %s\n\n", filename);

    free_check (exafs);

    return -1;

    }

  cout ("\n Read %ld points from XAFS file %s into entry %ld\n\n",
              N, filename, n + 1);


    /* Make new parameter vectors */

  exafs->exafs.key = create_parameter_vector (P, DATA_SET);

  key = create_parameter_vector (P, DATA_SET + COPY);


    /* Copy the current parameters into the vectors */

  copy_exafs_data (P, exafs->exafs);

  v = parameter_vector (P, exafs->exafs.key);

  vc = parameter_vector (P, key);

  copy_vector (vc, *v);

  rationalise_exafs (P, n, &(exafs->exafs.shells_0));


    /* Add the data set to the list */

  *exafs_l = exafs;


    /* Make a new window */

//  if (!BackGround) {

    exafs->Gid = setup_exafs_window (filename);

    Poll_event (0);

//    }


    /* Return 0 */

  return 0;

  }


  /*********************************************************************}
  {                                                                     }
  {  Set a parameter value                                              }
  {                                                                     }
  {*********************************************************************/

int assignment_command (parameter_set * P, node * x, vector4 value) {

  int c1, c2, c;

  vector * v, * vc;

  exafs_list * exafs;


  c2 = 0;

  v = parameter_vector (P, x->parameter.key);

  c1 = assign_parameter (P, x->parameter, value);


    /* Get the number of the vector in the set */

  for (c = 0; ; c++) {

    vc = parameter_vector_Oi (P, x->parameter.vector, c);

    if (vc == v || !vc)

      break;

    }


    /* Save a copy */

  if (vc) {

    x->parameter.vector += COPY;

    x->parameter.key = parameter_vector_key_Oi (P, x->parameter.vector, c);

    c2 = assign_parameter (P, x->parameter, value);

    }


  if (global_data.rationalise)

    global_data.rationalise (global_data.parameters);

  for (c = 0, exafs = *global_data.exafs; exafs; exafs = exafs->next, c++)

    rationalise_exafs (global_data.parameters, c, &(exafs->exafs.shells_0));


    /* Return 0 on failure */

  return !c1 && !c2;

  }


  /*********************************************************************}
  {                                                                     }
  {  Clean up any temporary structures used by the exafs calculation    }
  {                                                                     }
  {*********************************************************************/

int clean_command (void) {

  global_data.chi_calculate (null_vector, 0, 0, 0, NULL, NULL,NULL, NULL,
                                          0, NULL, NULL, NULL,
                                                         null_matrix, -1);

  return 0;

  }


#endif /* __MISC_COMMANDS_C__ */
