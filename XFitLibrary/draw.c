/***********************************************************************}
{                                                                       }
{  basic drawing routines : OS/2 Presentation Manager version           }
{                                                                       }
{  int initialise ()                                                    }
{                                                                       }
{  long openwindow (char [], long,                                      }
{                            long (* callback) (long, long, long))      }
{                                                                       }
{  void closewindow (int)                                               }
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
{***********************************************************************/


#if !defined (DRAW_C)

#define DRAW_C

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stddef.h>

/* #define INCL_GPI
#define INCL_WIN
#define INCL_PM
#define INCL_DOS
#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES */

#include "draw.h"


#include <stdlib.h>

#define max(a,b) ((a) >= (b) ? (a) : (b))

#define PALETTE 112
#define GAMMA   5.0


        /* Global data */


static UCHAR   clientClass [] = "Client Window";

static handles * handle1 = NULL;


  /* Each window has its own process and queue */

handles * GetWindowHandles (HWND hwnd);
handles * GetWindowHandlesTID ();

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM);

void openwindow_TF (void * OWD);


typedef struct {

  long thread_ID;

  char * title;

  long control;

  long hwnd;

  long (* callback) (long, long, long);

  } openwindow_data;


static HAB     hab = NULLHANDLE;       /* program's anchor block */

int initialise () {

  static HMQ hmq = NULLHANDLE;


    /* Initialise the window manager */

  if (!hab)

    hab = WinInitialize (0);


    /* Create a message queue for thread 1 */

  if (!hmq) 

    hmq = WinCreateMsgQueue (hab, 0);


  if (!hab || !hmq)

    return 0;

  return 1;

  }


int openwindow (char title [], long control, 
                               long (* callback) (long, long, long)) {

  openwindow_data OWD;


  initialise ();


    /* Save the data */

  OWD.title    = title;
  OWD.control  = control;
  OWD.callback = callback;

  OWD.hwnd     = 0;


    /* Create the window thread */

  OWD.thread_ID = _beginthread (openwindow_TF, NULL, 16384, (void *) &OWD);

  if (OWD.thread_ID == -1) {

    fprintf (stderr, " failed to create thread for window \"%s\"\n", title);

    exit (0);

    }


    /* Wait until the window is opened */

  while (OWD.hwnd == 0)

    DosSleep (50);


  return OWD.hwnd;

  }


void set_windowform (HWND framehwnd, int control, int size);


void openwindow_TF (void * OWD) {

  HWND  hwnd,       /* handle to frame window  */
        hwndClient; /* handle to client window */
  HMQ   hmq;        /* handle to message queue */
  QMSG  qmsg;       /* message queue element   */

  handles * HP;

  ULONG createFlags;

  int fixed_form;


    /* Create message queue */

  hmq = WinCreateMsgQueue (hab, 10);


    /* Register window class */

  WinRegisterClass (hab, clientClass, ClientWndProc, CS_SIZEREDRAW, 0);


    /* Create standard window and client */

  createFlags =  FCF_TITLEBAR | FCF_SYSMENU  | FCF_SIZEBORDER
                              | FCF_MINMAX   | FCF_SHELLPOSITION
                              | FCF_AUTOICON | FCF_TASKLIST;

  if (((openwindow_data *) OWD)->control & OW_HSCROLL)

    createFlags |= FCF_HORZSCROLL;

  if (((openwindow_data *) OWD)->control & OW_VSCROLL)

    createFlags |= FCF_VERTSCROLL;

  fixed_form = ((openwindow_data *) OWD)->control & OW_SQUARE;


  hwnd = WinCreateStdWindow (HWND_DESKTOP,
                             fixed_form ? 0 : WS_VISIBLE,
                             &createFlags,
                             clientClass,
                             (UCHAR *) ((openwindow_data *) OWD)->title,
                             0L,
                             0,   
                             0,
                             &hwndClient);


  if (fixed_form)

    set_windowform (hwnd, ((openwindow_data *) OWD)->control, 0);


    /* Move the window into the foreground */

  WinShowWindow (hwnd, TRUE);

  WinSetFocus (HWND_DESKTOP, hwndClient);



    /* Create the window handles */

  HP = GetWindowHandles (hwndClient);


    /* Attach the thread ID, callback routine, control and frame */

  HP->thread_ID = ((openwindow_data *) OWD)->thread_ID;

  HP->callback = ((openwindow_data *) OWD)->callback;

  HP->framehwnd = hwnd;

  HP->control = ((openwindow_data *) OWD)->control;


  (HP->callback) (hwndClient, CB_OPEN, 0);


    /* Signal that the client process has been created */

  ((openwindow_data *) OWD)->hwnd = hwndClient;


    /* Message dispatch loop */

  while (WinGetMsg (hab, &qmsg, 0, 0, 0))

    WinDispatchMsg (hab, &qmsg);


    /* Destroy frame window */

  WinDestroyWindow (hwnd);   


    /* Destroy message queue */

  WinDestroyMsgQueue (hmq);  


    /* Terminate PM usage */

  WinTerminate (hab);

  }



void closewindow (int hwnd) {

  WinPostMsg (hwnd, WM_CLOSE, 0, 0);

  }


void invalidatewindow (int hwnd) {

  handles * HP;

  HP = GetWindowHandles (hwnd);

  if (HP)

    if (!HP->redraw_sent) {

      HP->redraw_sent = TRUE;

      WinInvalidateRect (hwnd, NULL, FALSE);

      }
  }


void drawspace (double xmin, double xmax, double ymin, double ymax,
                                          double zmin, double zmax) {

  handles * HP;

  HP = GetWindowHandlesTID ();

  HP->Dspace [0][0] = xmin;
  HP->Dspace [0][1] = xmax;
  HP->Dspace [1][0] = ymin;
  HP->Dspace [1][1] = ymax;
  HP->Dspace [2][0] = zmin;
  HP->Dspace [2][1] = zmax;

  }


void beginline () {

  handles * HP;

  HP = GetWindowHandlesTID ();

  HP->linecount = 0;

  }


void endline () {

  int i;

  handles * HP;

  HP = GetWindowHandlesTID ();

  if (HP->linecount >= 2) {

    GpiMove (HP->hps, HP->line);

    GpiPolyLine (HP->hps, HP->linecount - 1, HP->line + 1);

    }

  HP->linecount = -1;

  }


void flushline () {

  LONG count;

  handles * HP;

  HP = GetWindowHandlesTID ();

  count = HP->linecount;

  endline ();

  beginline ();

  HP->linecount = 1;

  HP->line [0] = HP->line [count - 1];

  }



void move (double x, double y, double z) {

    /* translate to the Dspace (ignore z for the moment) */

  handles * HP;

  HP = GetWindowHandlesTID ();

  x = (x - HP->Dspace [0][0]) / (HP->Dspace [0][1] - HP->Dspace [0][0]);

  y = (y - HP->Dspace [1][0]) / (HP->Dspace [1][1] - HP->Dspace [1][0]);

  HP->line [HP->linecount].x = (LONG) (x * HP->cWidth + 0.5);
  HP->line [HP->linecount].y = (LONG) (y * HP->cHeight + 0.5);

  HP->linecount++;

  if (HP->linecount >= 100)

    flushline ();

  }


void RGBcolour (double RD, double GD, double BD) {

    /* is the colour requested of the form (0/1, 0/1, 0/1) * depth */

  int R, G, B, maxRGB, depth, colour;

  R = (int) (RD * 255.5);
  G = (int) (GD * 255.5);
  B = (int) (BD * 255.5);

  maxRGB = max (max (max (R, G), B), 1);

  if ((R == maxRGB || !R) && (G == maxRGB || !G) && (B == maxRGB || !B)) {

    handles * HP;

    HP = GetWindowHandlesTID ();

    depth = (maxRGB * 16) / 256;

    colour = R / maxRGB + 2 * G / maxRGB + 4 * B / maxRGB;

    if (colour == 0 || depth == 0)

      colour = 0;

    else

      colour = (colour - 1) * 16 + depth;

    if (HP->linecount > 0 && colour != HP->colour)

      flushline ();

    GpiSetColor (HP->hps, colour);

    HP->colour = colour;

    }
  else

    printf (" RGBcolour: cannot set colour (%.2f,%.2f,%.2f)\n", RD, GD, BD);

  }


void clearwindow () {

  RECTL rcl;

  handles * HP;

  HP = GetWindowHandlesTID ();

  WinQueryWindowRect (HP->hwnd, &rcl);

  WinFillRect (HP->hps, &rcl, HP->colour);

  }


void swapbuffers () {

  }


int mouse (double * x, double * y) {

  double P;

  handles * HP;

  HP = GetWindowHandlesTID ();

  P = (HP->mouse.x / (double) HP->cWidth);

  *x = P;

  /* *x = HP->Dspace [0][0] + P * (HP->Dspace [0][1] - HP->Dspace [0][0]); */

  P = (HP->mouse.y / (double) HP->cHeight);

  *y = P;

  /* *y = HP->Dspace [1][0] + P * (HP->Dspace [1][1] - HP->Dspace [1][0]); */

  return 1;

  }




/* Remember to use semaphores!!!! */


handles * GetWindowHandlesTID () {

  int thread_ID;

  unsigned * Pthread_ID;

  handles * HP;


    /* get the thread ID */

  if (Pthread_ID = _threadid)

    thread_ID = *Pthread_ID;

  else

    return NULL;


    /* find the correct handles for the thread */

  for (HP = handle1; HP; HP = HP->next_handles)

    if (HP->thread_ID == thread_ID)

      break;

  return HP;

  }


handles * GetWindowHandles (HWND hwnd) {

 
    /* find the correct handles for the window */

  handles ** HPP, * HP;

  for (HPP = &handle1; *HPP; HPP = &((*HPP)->next_handles))

    if ((*HPP)->hwnd == hwnd)

      break;


    /* add a new set if required */

  if (!(*HPP)) {

    (*HPP) = calloc (1, sizeof (handles));

    HP = (*HPP);

    HP->thread_ID = -1;

    HP->hwnd = hwnd;

    HP->framehwnd = NULLHANDLE;

    HP->control = 0;

    HP->hps     = NULLHANDLE;
    HP->hdc     = NULLHANDLE;

    HP->callback = NULL;

    HP->linecount = -1;

    HP->colour = 0;

    HP->Dspace [0][0] = HP->Dspace [1][0] = HP->Dspace [2][0] = -1;
    HP->Dspace [0][1] = HP->Dspace [1][1] = HP->Dspace [2][1] =  1;

    HP->mouse.x = 0;
    HP->mouse.y = 0;

    HP->firstPaint = TRUE;

    HP->redraw_sent = FALSE;

    HP->next_handles = NULL;

    }
  else

    HP = *HPP;

  return HP;

  }


void DeleteWindowHandles (handles * HP) {

  handles ** HPP;

  for (HPP = &handle1; *HPP; HPP = &((*HPP)->next_handles))

    if ((*HPP) == HP)

      break;

  if (!(*HPP)) {

    printf (" error deleting window handles\n");

    exit (0);

    }

  HP = HP->next_handles;

  free (*HPP);

  (*HPP) = HP;

  }


int WindowHandles () {

  handles * HP;

  int count;

  count = 0;

  for (HP = handle1; HP; HP = HP->next_handles)

    count++;

  return count;

  }


void set_windowform (HWND framehwnd, int control, int size) {

   RECTL rcl;

   LONG  scrWid, scrHi, place;


   place = size <= 0;

   if (place) {

     HDC   hdcScr;

     hdcScr = GpiQueryDevice (WinGetPS (HWND_DESKTOP));

     DevQueryCaps (hdcScr, CAPS_WIDTH, 1L, &scrWid);
     DevQueryCaps (hdcScr, CAPS_WIDTH, 1L, &scrWid);
     DevQueryCaps (hdcScr, CAPS_HEIGHT, 1L, &scrHi);

     size = scrWid;

     if (size > scrHi)

       size = scrHi;

     size /= 2;

     }

   rcl.xLeft   = 0;
   rcl.xRight  = size;
   rcl.yBottom = 0;
   rcl.yTop    = size;

   WinCalcFrameRect (framehwnd, &rcl, FALSE);

   if (place)

     WinSetWindowPos (framehwnd,
                      NULLHANDLE,
                      (scrWid - rcl.xRight + rcl.xLeft) / 2,
                      (scrHi - rcl.yTop + rcl.yBottom) / 2,
                      (rcl.xRight - rcl.xLeft),
                      (rcl.yTop - rcl.yBottom),
                      SWP_SHOW | SWP_MOVE | SWP_SIZE);

   else

     WinSetWindowPos (framehwnd,
                      NULLHANDLE,
                      0,
                      0,
                      (rcl.xRight - rcl.xLeft),
                      (rcl.yTop - rcl.yBottom),
                      SWP_SHOW | SWP_SIZE);

  }





MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {

  static HPAL   hpal = NULLHANDLE;

  static LONG   colorsToShow, colorsAvail;

  static BOOL   createPalette = 1;

  handles * HP;

  HP = GetWindowHandles (hwnd);

  switch (msg) {

    case WM_CREATE: {

      LONG  addlGraphics;

      SIZEL sizl = {0L, 0L};

      HP->hdc = WinOpenWindowDC (hwnd);

      DevQueryCaps (HP->hdc, CAPS_COLORS, 1L, &colorsAvail);
      DevQueryCaps (HP->hdc, CAPS_ADDITIONAL_GRAPHICS, 1L,
                             &addlGraphics);

      HP->hps = GpiCreatePS (hab, HP->hdc, &sizl,
                                    PU_PELS    | GPIF_DEFAULT |
                                    GPIT_MICRO | GPIA_ASSOC);

        /* Create and use palette */

      if (createPalette) {

        static BYTE Palette [PALETTE][4];

        int i, j, R, G, B;

        createPalette = 0;

        for (i = 1; i < 8; i++) {

          R = ((i & 1) != 0);
          G = ((i & 2) != 0);
          B = ((i & 4) != 0);
 
          for (j = 0; j < 16; j++) {

            Palette [(i - 1) * 16 + j][3] = 0;
            Palette [(i - 1) * 16 + j][2] = (int) (log (R * GAMMA * j / 15.0 + 1) /
                                                   log (GAMMA + 1) * 255.5);
            Palette [(i - 1) * 16 + j][1] = (int) (log (G * GAMMA * j / 15.0 + 1) /
                                                   log (GAMMA + 1) * 255.5);
            Palette [(i - 1) * 16 + j][0] = (int) (log (B * GAMMA * j / 15.0 + 1) /
                                                   log (GAMMA + 1) * 255.5);

            }
          }

        hpal = GpiCreatePalette (hab, 0L, LCOLF_CONSECRGB,
                                          PALETTE,
                                          (PULONG) Palette);

        }

      GpiSelectPalette (HP->hps, hpal);

      return (MRESULT) FALSE;

      }


    case WM_PAINT: {

      WinBeginPaint (hwnd, HP->hps, NULLHANDLE);

      HP->redraw_sent = FALSE;

      if (HP->firstPaint) {

        ULONG palSize = PALETTE;

        if (WinRealizePalette (hwnd, HP->hps, &palSize)) {

          WinInvalidateRect (hwnd, NULL, FALSE);

          }

        HP->firstPaint = FALSE;

        }

      GpiSelectPalette (HP->hps, hpal);

      if (HP->callback)

        (HP->callback) (hwnd, CB_PAINT, 0);

      WinEndPaint (HP->hps);

      return (MRESULT) FALSE;

      }


    case WM_MOUSEMOVE:

      HP->mouse.x = SHORT1FROMMP (mp1);
      HP->mouse.y = SHORT2FROMMP (mp1);

      if (HP->callback)

        (HP->callback) (hwnd, CB_MOUSE_MO, 0);

      return WinDefWindowProc (hwnd, msg, mp1, mp2);


    case WM_BUTTON1DOWN:

      HP->mouse.x = SHORT1FROMMP (mp1);
      HP->mouse.y = SHORT2FROMMP (mp1);

      if (HP->callback)

        (HP->callback) (hwnd, CB_MOUSE_DN, 1);

      return (MRESULT) FALSE;


    case WM_BUTTON2DOWN:

      HP->mouse.x = SHORT1FROMMP (mp1);
      HP->mouse.y = SHORT2FROMMP (mp1);

      if (HP->callback)

        (HP->callback) (hwnd, CB_MOUSE_DN, 2);

      return (MRESULT) FALSE;


    case WM_BUTTON1UP:

      HP->mouse.x = SHORT1FROMMP (mp1);
      HP->mouse.y = SHORT2FROMMP (mp1);

      if (HP->callback)

        (HP->callback) (hwnd, CB_MOUSE_UP, 1);

      return (MRESULT) FALSE;


    case WM_BUTTON2UP:

      HP->mouse.x = SHORT1FROMMP (mp1);
      HP->mouse.y = SHORT2FROMMP (mp1);

      if (HP->callback)

        (HP->callback) (hwnd, CB_MOUSE_UP, 2);

      return (MRESULT) FALSE;


    case WM_REALIZEPALETTE:

      return (MRESULT) FALSE;


    case WM_SIZE:

      HP->cWidth  = (ULONG) SHORT1FROMMP (mp2);
      HP->cHeight = (ULONG) SHORT2FROMMP (mp2);

      if ((HP->control & OW_SQUARE) && abs (HP->cWidth - HP->cHeight) > 1) {

          /* Keep the window square */

        int size;

        RECTL rcl;

        size = HP->cWidth;

        if (size > HP->cHeight)

          size = HP->cHeight;

        if (size < 32)

          size = 32;

        set_windowform (HP->framehwnd, HP->control, size);

        }

      if (HP->callback)

        (HP->callback) (hwnd, CB_SIZE, (long) MPFROM2SHORT (HP->cWidth, HP->cHeight));

      break;


    case WM_CHAR: {

      int flags, c;

        /* Send codes for all normal keys (32-126), arrows, ins, del, BS */

      flags = SHORT1FROMMP (mp1);

      if (flags & KC_KEYUP)

        break;

      c = 0;

      if (flags & KC_VIRTUALKEY) 

        switch (SHORT2FROMMP (mp2)) {

          case VK_HOME:

            c = CB_HOME;

            break;

          case VK_END:

            c = CB_END;

            break;

          case VK_PAGEUP:

            c = CB_PGUP;

            break;

          case VK_PAGEDOWN:

            c = CB_PGDOWN;

            break;

          case VK_UP:

            c = CB_UP;

            break;

          case VK_DOWN:

            c = CB_DOWN;

            break;

          case VK_LEFT:

            c = CB_LEFT;

            break;

          case VK_RIGHT:

            c = CB_RIGHT;

            break;

          case VK_BACKSPACE:

            c = CB_BACKSPACE;

            break;

          case VK_INSERT:

            c = CB_INSERT;

            break;

          case VK_DELETE:

            c = CB_DELETE;

            break;

          case VK_SPACE:

            c = ' ';

            break;

          case VK_NEWLINE:
          case VK_ENTER:

            c = CB_EOL;

            break;

          }

      else

        c = SHORT1FROMMP (mp2);

      if (c && HP->callback)

        (HP->callback) (hwnd, CB_KEY, c);
 
      break;

      }


    case WM_HSCROLL:

      if (HP->callback)

        (HP->callback) (hwnd, CB_HSCROLL, LONGFROMMP (mp2));

      return (MRESULT) FALSE;


    case WM_VSCROLL:

      if (HP->callback)

        (HP->callback) (hwnd, CB_VSCROLL, LONGFROMMP (mp2));

      return (MRESULT) FALSE;


    case WM_ERASEBACKGROUND:

      return (MRESULT) FALSE;


    case WM_DESTROY: 

      GpiSelectPalette (HP->hps, NULLHANDLE);


        /* delete the palette if this is the last window */

      if (WindowHandles () == 1)

        if (hpal) {

          GpiDeletePalette (hpal);

          createPalette = 1;

          }


        /* delete the resources */

      GpiDestroyPS (HP->hps);


        /* delete the handles */

      DeleteWindowHandles (HP);

      return (MRESULT) FALSE;


    case WM_CLOSE:

      if (HP->callback)

        (HP->callback) (hwnd, CB_CLOSE, 0);

      return WinDefWindowProc (hwnd, msg, mp1, mp2);


    default: 

      return WinDefWindowProc (hwnd, msg, mp1, mp2);

    }
  }


void check () {

  DosSleep (0);

  }


#endif /* DRAW_C */

