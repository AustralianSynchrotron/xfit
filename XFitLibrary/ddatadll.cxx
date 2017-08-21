
#if !defined DDATADLL_CPP

#define DDATADLL_CPP

#include "data.h"
#include <string.h>


  // Win32 DLL used by xfit to read general "D" data

#define MODULE_NAME "DDATA"


  // STRICT type checking

#define  STRICT

// #include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stddef.h>

  // Error function -  we have one elsewhere

/* void ErrorExit (char message []) {

  char title [128];

  sprintf (title, "ERROR in XFIT\n");

  exit(-1);

  } */


  // DLL functions

extern "C" void * ReadDData (char * filename) {

  DataFile * D;

  D = new DataFile;
  
  if (D->read (filename))

    return D;

  delete D;

  return NULL;

  }


extern "C" void * WriteDData (void * D, char * filename) {

  if (D)
  
    if (((DataFile *) D)->write (filename))

      return D;

  return NULL;

  }


extern "C" void * NewDData () {

  return new DataFile;

  }


extern "C" void  FreeDData (void * D) {

  if (D)

    delete (DataFile *) D;

  }


extern "C" int  AddDDataColumn (void * D,
                                                  const char * ColName,
                                                  int ColN, double * ColD) {

  if (((DataFile *) D)->add_column (ColName)) {

    Columnvector * Col;

    Col = ((DataFile *) D)->column (ColName);
    
    Col->ReFrame (ColN);

    if (ColD)
    
      *Col << ColD;

    return 1;

    }

  return 0;

  }


extern "C" int  AddDDataAlias (void * D,
                                                 const char * ColName,
                                                 const char * Alias) {

  if (D)

    return ((DataFile *) D)->add_alias (ColName, Alias);

  return 0;

  }


extern "C" int  AddDDataEntry (void * D,
                                                 const char * Section,
                                                 const char * Entry) {

  if (D)

    return ((DataFile *) D)->add_data (Section, Entry);

  return 0;

  }


extern "C" int  DDataColumn (void * D,
                                               const char * ColName,
                                               double ** ColD) {

  Columnvector * Col;

  Col = ((DataFile *) D)->column (ColName);

  if (Col) {

    if (ColD)
    
      *ColD = Col->Store ();

    return Col->Nrows ();

    }

  return 0;

  }


extern "C" char * DDataEntry (void * D,
                                                 const char * Section,
                                                 const char * Key) {

  static char line [256];
  
  std::string word;

  word = ((DataFile *) D)->data (Section, Key);

  if (word != "") {

    strncpy (line, word.c_str(), 255);

    line [255] = 0;

    return line;

    }

  return NULL;

  }


#endif // DDATADLL_CPP

