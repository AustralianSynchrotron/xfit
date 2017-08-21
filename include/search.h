/***********************************************************************}
{                                                                       }
{  Functions used when searching for files                              }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__SEARCH_H)

#define __SEARCH_H

#include "header.h"

  /**************************************************************}
  {                                                              }
  {  Each file is characterised by :                             }
  {                                                              }
  {     key (double-precision)                                   }
  {                                                              }
  {     index (int)                                              }
  {                                                              }
  {**************************************************************/

  /**************************************************************}
  {                                                              }
  {  Define the structure used to hold the results of a search.  }
  {                                                              }
  {**************************************************************/

typedef struct FSR_node_struct {

  double key;

  int size;

  struct FSR_node_struct * next;

  char status [4];

  } FSR_node;

void destruct_FSR_map (FSR_node ** FSR_n);

  /**************************************************************}
  {                                                              }
  {  Open a file.                                                }
  {                                                              }
  {  If a previous attempt to open the file failed, return NULL  }
  {                                                              }
  {**************************************************************/

FILE * FSR_open_file (char filename [], double key, int index,

                      FSR_node ** map, int size, int ignore_found);


#endif /* __SEARCH_H */
