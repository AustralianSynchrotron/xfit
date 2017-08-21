
#if !defined DDATADLL_CPP

#define DDATADLL_CPP

#include "data.h"


  // Win32 DLL used by xfit to read general "D" data

#define MODULE_NAME "DDATA"


struct TLD {

  int OK;

  int ID;

  };


  // STRICT type checking

#define  STRICT

#include <math.h>
#include <stdio.h>
#include <stddef.h>


  // Error function

void ErrorExit (char message []) {

  char title [128];

  sprintf (title, "ERROR somewhere in XFIT\n");

  exit(-1);

  }

  // DLL functions

extern "C" void * ReadDData (char * filename) {

  DataFile * D;

  D = new DataFile;
  
  if (D->read (filename))

    return D;

  delete D;

  return NULL;

  }


#endif // DDATADLL_CPP

