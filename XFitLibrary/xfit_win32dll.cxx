
#if !defined XFITDLL_CPP

#define XFITDLL_CPP


  // Win32 DLL used by xfit to calculate empirical xafs

#define MODULE_NAME "XFIT"




struct TLD {

  int OK;

  int ID;

  };


  // STRICT type checking

#define  STRICT

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stddef.h>


  // Exported function type

#define DLL_EXPORT __declspec(dllexport)

  // Error function

void ErrorExit (char message []);

  // The Win32 DLL initialization function

static DWORD TlsIndex;

BOOL APIENTRY DllMain (HANDLE hModule,       // A module handle
                       DWORD  dwReason,      // An enumerated type which indicates
                                             // which of four reasons the LibMain
                                             // procedure is being called:
                                             //   Process attach
                                             //   Thread attach
                                             //   Thread detach
                                             //   Process detach
                       LPVOID lpReserved) {

    // Initialise local data for each thread

  TLD * PTLD;


  switch (dwReason) {

    case DLL_PROCESS_ATTACH:

      TlsIndex = TlsAlloc ();

      if (TlsIndex == 0xFFFFFFFF)

        return FALSE;


    case DLL_THREAD_ATTACH:

        // Allocate data space for the new thread

      PTLD = new TLD;

      PTLD->ID = 0;

      if (!TlsSetValue (TlsIndex, (LPVOID) PTLD))

        ErrorExit ("TlsSetValue error");

      break;


    case DLL_PROCESS_DETACH:

    case DLL_THREAD_DETACH:

        // Release the dynamic memory

      PTLD = (TLD *) TlsGetValue (TlsIndex);

      if (PTLD)

        delete PTLD;

      if (dwReason == DLL_PROCESS_DETACH)

        TlsFree (TlsIndex);

      break;

    }


    // Success
  
  return TRUE;

  }

BOOL APIENTRY LibMain (HANDLE hModule, DWORD dwReason, LPVOID lpReserved) {

  return DllMain (hModule, dwReason, lpReserved);

  }


  // Error function

void ErrorExit (char message []) {

  char title [128];

  sprintf (title, "ERROR in ");

  GetModuleFileName (GetModuleHandle (MODULE_NAME),
                     title + strlen (title), 128 - strlen (title));

  MessageBox (NULL, message, title, MB_ICONSTOP | MB_OK);

  ExitProcess (0);

  }


  // Retrieve the data pointer for the current thread

TLD * GetTLD () {

  TLD * PTLD;

  PTLD = (TLD *) TlsGetValue (TlsIndex);

  if (PTLD == NULL)

    ErrorExit ("Missing thread local data");

  return PTLD;

  }


  // DLL functions

extern "C" {

#include "xdraw_mo.h"

int check_all_exafs ();

int xmain (int argc, char * argv []);

typedef int __stdcall fCout (void *, char *);
typedef int __stdcall fDataW (void *, void *, char *);
typedef char * __stdcall fSymbol (void *, char *, char *);

void  * PCal;
fCout * Cout;
fDataW * DataW;
fSymbol * GetSymbol;

}

typedef char * __stdcall fPoll (void *, int, void *);

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

extern "C" DLL_EXPORT int  __stdcall Xfit (void * ct, fPoll * po, fCout * co, fDataW * dw, fSymbol * sym) {

  PCal = ct;
  Poll = po;
  Cout = co;
  DataW = dw;
  GetSymbol = sym;

    // Execute xfit

  _control87 (EM_INVALID,    EM_INVALID);
  _control87 (EM_ZERODIVIDE, EM_ZERODIVIDE);
  _control87 (EM_OVERFLOW,   EM_OVERFLOW);
  _control87 (EM_UNDERFLOW,  EM_UNDERFLOW);
  _control87 (EM_DENORMAL,   EM_DENORMAL);

  return xmain (0, NULL);

  }


extern "C" DLL_EXPORT double __stdcall StrToDouble (char * value) {

  double D [2];

  int OK;

  D [0] = 0;
  
  sscanf (value, " %lf", D);

  D [1] = 1;
  
  sscanf (value, " %lf", D + 1);

  GetTLD ()->OK = D [0] == D [1];
  
  return D [0];

  }


extern "C" DLL_EXPORT int __stdcall Success () {

  return GetTLD ()->OK;
  
  }


#endif // XFPAKGDLL_CPP

