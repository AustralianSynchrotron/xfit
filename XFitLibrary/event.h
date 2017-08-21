/***********************************************************************}
{                                                                       }
{  Check for any events and take appropriate action                     }
{                                                                       }
{***********************************************************************/

#if !defined (__EVENT_H__)

#define __EVENT_H__

#include "include.h"

#if defined (__TURBOC__)

/* #include <winsock.h> */

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


char * Poll_event (int console);


#endif /* __EVENT_H__ */

