/***********************************************************************}
{                                                                       }
{  Check for any events and take appropriate action                     }
{                                                                       }
{***********************************************************************/

#if !defined (__EVENT_C__)

#define __EVENT_C__

// #include <sys/time.h>

#include "event.h"
#include "input.h"
#include "global.h"
#include "exafs_ty.h"
#include "console.h"

//#include <i86.h>

#if defined (USE_GRAPH)

#include "draw_mod.h"

#endif



  /*********************************************************************}
  {                                                                     }
  {  Look for an event including input from the console                 }
  {                                                                     }
  {  console <> 0 : get input from the console if available             }
  {                                                                     }
  {  Return the next line from the console / 0                          }
  {                                                                     }
  {*********************************************************************/

extern int BackGround;

char * Poll_event (int console) {


  static char buffer [BUF_SIZE];

  static int first = 1;

  char * c;
  
  struct exafs_list_struct * exafs;


#if !defined (USE_GRAPH)

  int model_Gid;
  
    /* We never use the windows */

  BackGround = 1;

#endif


  if (first) {

      /* BackGround <> 0 : the displays are not used

         BackGround =  0 : (1) set up the console

                           (2) set up the model window  */


    model_Gid = 0;

#if defined (USE_GRAPH)

    if (!BackGround) {

        /* Open the console */

      openconsole ();


        /* Open the model window */

      model_Gid = setup_model_window ();

      }

#endif

    first = 0;

    }


    /* Redraw the windows if necessary */

#if defined (USE_GRAPH)

  if (!BackGround) {

    redraw_model ();

    if (global_data.exafs)

      for (exafs = *(global_data.exafs); exafs; exafs = exafs->next)

        check_exafs (exafs);

    }

#endif


    /* Set the returned pointer to 0 */

  c = NULL;


    /* Get text from the console */

  if (console) {

    fgets (buffer, BUF_SIZE, stdin);

      
      /* Set the returned pointer to the string */

    c = buffer;

    }
//  else

  //  delay (50);

    /* Return the string from the console / 0 */

  return c;

  }


#endif /* __EVENT_C__ */

