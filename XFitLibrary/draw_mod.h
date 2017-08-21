/***********************************************************************}
{                                                                       }
{  draw the model                                                       }
{                                                                       }
{***********************************************************************/

#if !defined (DRAW_MODEL_H)

#define DRAW_MODEL_H

#if defined USE_GRAPH

#include "draw_3d.h"

#endif


#if defined (DRAW_MODEL_C)

long model_Gid = 0;

#else

extern long model_Gid;

#endif


  /*********************************************************************}
  {                                                                     }
  {  Set up the model window                                            }
  {                                                                     }
  {*********************************************************************/

int setup_model_window (void);


  /*********************************************************************}
  {                                                                     }
  {  Called whenever the mouse is used with this window                 }
  {                                                                     }
  {  Return 1 if the window should be redrawn                           }
  {                                                                     }
  {*********************************************************************/

/* int model_mouse_used (int mouse_device, int status); */


  /*********************************************************************}
  {                                                                     }
  {  Redraw the model window                                            }
  {                                                                     }
  {*********************************************************************/

void * redraw_model (void);


#endif /* DRAW_MODEL_H */

