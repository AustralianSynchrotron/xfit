/***********************************************************************}
{                                                                       }
{  This file contains routines used to interface to FEFF 6.01           }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__FEFF6_H)

#define __FEFF6_H

#include "header.h"

#if !defined (__TURBOC__)

#include <unistd.h>
// #include <sys/wait.h>

#endif

#include "feff_.h"

#if !defined (HEADER_LINES)

#define HEADER_LINES 32

#endif

#define COORDINATE_FORMAT "%14.9f"

#define MAXLEGS 8

#if defined (MAXPOINTS)
#undef MAXPOINTS
#endif

/* changed to 100 for FEFF7 */
#define MAXPOINTS 100 

char * FEFF6_name (char format [], int P, int A, double C []);


  /*********************************************************************}
  {                                                                     }
  {  Define the structure to hold a multiple scattering path.           }
  {                                                                     }
  {*********************************************************************/

typedef struct path_l_s {

  int number, legs, use, points,

      atom [MAXLEGS], match;

  double degeneracy, v [MAXLEGS][3], potential [MAXLEGS], Z [MAXLEGS];

  double k          [MAXPOINTS],
         real_2_phc [MAXPOINTS],
         mag_feff   [MAXPOINTS],
         phase      [MAXPOINTS],
         red_factor [MAXPOINTS],
         lambda     [MAXPOINTS],
         real_p     [MAXPOINTS];

  struct path_l_s * next;

  } path_l;

  /* 3176 bytes */ 


void destruct_path_l (path_l ** path);


  /*********************************************************************}
  {                                                                     }
  {  Given the following numbers (in the indicated ranges) :            }
  {                                                                     }
  {      number:      0 .. 719                                          }
  {      symmetry:    0 .. 989                                          }
  {                                                                     }
  {      x:         -45 .. 44                                           }
  {      y:         -45 .. 44                                           }
  {      z:         -45 .. 44                                           }
  {                                                                     }
  {  this routine generates a unique 6-character ASCII atom label       }
  {                                                                     }
  {  Avoid using :   32 (SPACE)                                         }
  {                  34 (")                                             }
  {                  39 (')                                             }
  {                  44 (COMMA)                                         }
  {                 127 (BACKSPACE)                                     }
  {                                                                     }
  {*********************************************************************/

char * FEFF6_label (int number, int symmetry, int x, int y, int z);


  /*********************************************************************}
  {                                                                     }
  {  Given a label generated by FEFF6_label, this routine will          }
  {  translate it back into the original numbers                        }
  {                                                                     }
  {*********************************************************************/

int translate_FEFF6_label (char label [], int * number,

                           int * symmetry, int * x, int * y, int * z);


  /*********************************************************************}
  {                                                                     }
  {  Find the instance "instance" of the atom with atomic number ZC.    }
  {                                                                     }
  {  This is the central atom.                                          }
  {                                                                     }
  {  If ZC is 0, use the first atom.                                    }
  {                                                                     }
  {  If the instance does not exist, return 0.                          }
  {                                                                     }
  {*********************************************************************/

int rationalise_FEFF6_potentials (char name [],

                                   double ZC, int instance,

                                   int shells, double potential [],

                                               double Z []);


  /*********************************************************************}
  {                                                                     }
  {  Create a FEFF6 input file.                                         }
  {                                                                     }
  {*********************************************************************/

int make_FEFF6 (char name [],

                 char path [],

                 double ZC, int instance, int hole,

                 double polar  [],
                 double normal [],
                 double ellipticity,

                 int    max_legs,
                 double max_length,

                 double dE,
                 double dEi,

                 double filter_CW,
                 double filter_PW,

                 int    shells,
                 double potential [],
                 double Z         [],
                 double N         [],
                 double v         [][3],
                 double s         [],

                 double S02);


  /*********************************************************************}
  {                                                                     }
  {  Execute FEFF6.  The executable must be in the PATH or in the       }
  {  directory specified by FEFF_DIRECTORY.                             }
  {                                                                     }
  {*********************************************************************/

int execute_FEFF6 (char name [],

                    char path [],

                    int potph, int paths, int genfmt, int ff2chi,

                    int print);


  /*********************************************************************}
  {                                                                     }
  {  Clean up after FEFF6.                                              }
  {                                                                     }
  {*********************************************************************/

int clean_FEFF6 (char name [], char path []);


  /*********************************************************************}
  {                                                                     }
  {  Read paths from paths.dat and feffxxxx.dat into the list           }
  {                                                                     }
  {  If the path exists, it is overwritten.                             }
  {                                                                     }
  {  All paths are in numerical order.                                  }
  {                                                                     }
  {*********************************************************************/

int read_paths (path_l ** path_list, int read_data,

                int Nam,

                double potentialm [], double Zm [], double Rm [][3]);


  /*********************************************************************}
  {                                                                     }
  {  Save the binary phaseshift file (phase.bin)                        }
  {                                                                     }
  {*********************************************************************/

int save_FEFF6_files (char name [], int phase, int index);


  /*********************************************************************}
  {                                                                     }
  {  Manage the binary phaseshift files                                 }
  {                                                                     }
  {             (phase.bin / ph<index>.bin / phases<index>.bin)         }
  {                                                                     }
  {*********************************************************************/

int manage_FEFF6_files (char name [], int calc, int index);


  /*********************************************************************}
  {                                                                     }
  {  Rewrite the paths.dat file with the new coordinates (R [][3]).     }
  {                                                                     }
  {  The paths are taken from the path list passed.  Only the marked    }
  {  paths are used.                                                    }
  {                                                                     }
  {*********************************************************************/

int rewrite_FEFF6_paths (char name [], path_l * path, int all, double R [][3],

                         int * count);


#endif /* __FEFF6_H */

