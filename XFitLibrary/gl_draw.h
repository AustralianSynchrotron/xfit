/***********************************************************************}
{                                                                       }
{  System-specific drawing routines :                                   }
{                                                                       }
{  int initialise ()                                                    }
{                                                                       }
{  int openwindow (char [], long, long (* callback) (long, long, long)) }
{                                                                       }
{  void closewindow (int)                                               }
{                                                                       }
{  void invalidatewindow (int)                                          }
{                                                                       }
{  void beginline ()                                                    }
{                                                                       }
{  void endline ()                                                      }
{                                                                       }
{  void move (double, double, double)                                   }
{                                                                       }
{  void RGBcolour (double, double, double)                              }
{                                                                       }
{  void clearwindow ()                                                  }
{                                                                       }
{  void drawspace (double, double, double, double, double, double)      }
{                                                                       }
{  void swapbuffers ()                                                  }
{                                                                       }
{  int mouse (double *, double *)                                       }
{                                                                       }
{  void check ()                                                        }
{                                                                       }
{***********************************************************************/

#if !defined (DRAW_H)

#define DRAW_H


#if defined (DRAW_C) || defined (CONSOLE_C)

typedef struct handles_t {

  LONG    cWidth, cHeight;

  long    thread_ID;

  HWND    framehwnd, hwnd;
  HPS     hps;
  HDC     hdc;

  LONG    control;

  BOOL    firstPaint;

  long (* callback) (long, long, long);

  LONG    linecount;

  POINTL  line [100];

  LONG    colour;

  double  Dspace [3][2];

  POINTL  mouse;

  BOOL    redraw_sent;

  struct handles_t * next_handles;

  } handles;

handles * GetWindowHandles (HWND hwnd);

#endif


  /* Callback calls */

#define CB_OPEN      1
#define CB_CLOSE     2
#define CB_PAINT     3
#define CB_SIZE      4

#define CB_MOUSE_MO  5
#define CB_MOUSE_UP  6
#define CB_MOUSE_DN  7
#define CB_KEY       8
#define CB_HSCROLL   9
#define CB_VSCROLL   10

#define CB_BACKSPACE  8
#define CB_EOL        13

#define CB_DELETE     264
#define CB_INSERT     265

#define CB_UP         256
#define CB_RIGHT      257
#define CB_DOWN       258
#define CB_LEFT       259

#define CB_HOME       260
#define CB_END        261
#define CB_PGUP       262
#define CB_PGDOWN     263

#define OW_SQUARE     0x01
#define OW_HSCROLL    0x02
#define OW_VSCROLL    0x04


int initialise ();


  /*********************************************************************}
  {                                                                     }
  {  Open a square window with the given name                           }
  {                                                                     }
  {*********************************************************************/

int openwindow (char [], long, long (* callback) (long, long, long));


  /*********************************************************************}
  {                                                                     }
  {  Close a window                                                     }
  {                                                                     }
  {*********************************************************************/

void closewindow (int);


  /*********************************************************************}
  {                                                                     }
  {  Clear a window                                                     }
  {                                                                     }
  {*********************************************************************/

void clearwindow ();


  /*********************************************************************}
  {                                                                     }
  {  Swap the buffers                                                   }
  {                                                                     }
  {*********************************************************************/

void swapbuffers ();


  /*********************************************************************}
  {                                                                     }
  {  Set the colour                                                     }
  {                                                                     }
  {*********************************************************************/

void RGBcolour (double, double, double);


  /*********************************************************************}
  {                                                                     }
  {  Begin a line                                                       }
  {                                                                     }
  {*********************************************************************/

void beginline ();


  /*********************************************************************}
  {                                                                     }
  {  End a line                                                         }
  {                                                                     }
  {*********************************************************************/

void endline ();


  /*********************************************************************}
  {                                                                     }
  {  Set a line vertex                                                  }
  {                                                                     }
  {*********************************************************************/

void move (double, double, double);


  /*********************************************************************}
  {                                                                     }
  {  The following devices must be defined :                            }
  {                                                                     }
  {    LEFT_MOUSE                                                       }
  {    MIDDLE_MOUSE                                                     }
  {    RIGHT_MOUSE                                                      }
  {                                                                     }
  {    MOUSE_X                                                          }
  {    MOUSE_Y                                                          }
  {                                                                     }
  {    REDRAW                                                           }
  {                                                                     }
  {    FOCUS                                                            }
  {                                                                     }
  {*********************************************************************/


  /*********************************************************************}
  {                                                                     }
  {  Puts a REDRAW entry on the queue                                   }
  {                                                                     }
  {*********************************************************************/

void invalidatewindow (int);


  /*********************************************************************}
  {                                                                     }
  {  Return the coordinates of the mouse in the current window          }
  {                                                                     }
  {  Coordinates of the window are 0 .. 1                               }
  {                                                                     }
  {*********************************************************************/

int mouse (double * x, double * y);


  /*********************************************************************}
  {                                                                     }
  {  Set the coordinate range of a window                               }
  {                                                                     }
  {*********************************************************************/

void drawspace (double x_min, double x_max,  double y_min, double y_max,

                double z_min, double z_max);


#endif /* DRAW_H */

