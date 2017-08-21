
#if !defined XFITDLL_CPP

#define XFITDLL_CPP


#define MODULE_NAME "XFIT"


  // STRICT type checking

#define  STRICT

// #include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stddef.h>

  // DLL functions

extern "C" {

#include "xdraw_mo.h"

int check_all_exafs ();

int xmain (int argc, char * argv []);

typedef int  fCout (void *, char *);
typedef int  fDataW (void *, void *, char *);
typedef char *  fSymbol (void *, char *, char *);

void  * PCal;
fCout * Cout;
fDataW * DataW;
fSymbol * GetSymbol;

}

typedef char *  fPoll (void *, int, void *);

fPoll * Poll;


  // Look for an event including input from the console
  //
  // console <> 0 : get input from the console if available
  //
  // Return the next line from the console / 0

extern "C" char * Poll_event (int console) {

    // Check the xafs

  check_all_exafs ();


    // Redraw the model and get input

  if (Poll)

    return Poll (PCal, console, redraw_model ());

  return NULL;

  }


#include <float.h>

extern "C" int Xfit (void * ct, fPoll * po, fCout * co, fDataW * dw, fSymbol * sym) {

 PCal = ct;
 Poll = po;
 Cout = co;
 DataW = dw;
 GetSymbol = sym;

 // Execute xfit

 return xmain (0, NULL);

 }


extern "C" double  StrToDouble (char * value) {

  double D [2];

  int OK;

  D [0] = 0;
  
  sscanf (value, " %lf", D);

  D [1] = 1;
  
  sscanf (value, " %lf", D + 1);

  return D [0];

  }


#endif // XFPAKGDLL_CPP

