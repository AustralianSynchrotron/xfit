/************************************************
*                                               *
*       Feff_ms.h                               *
*       Multiple scattering routines common to  *
*       all FEFF versions >= 6                  *
*                                               *
************************************************/

#if !defined(__FEFF_68)
#define __FEFF_68

#include <stdio.h>          /* for FILE type */
#define MAXLEGS 8

#if !defined (HEADER_LINES)

#define HEADER_LINES 32

#endif

#define COORDINATE_FORMAT "%14.9f"

#if defined (MAXPOINTS)
#undef MAXPOINTS
#endif

/* changed from 50 to 100 for feff7 */
#define MAXPOINTS 100  

#if defined (WIN32)
#define PATH_SEP "\\"
#else
#define PATH_SEP "/"
#endif

int pcopy (char path [], char from [], char to []);

int premove (char path [], char name []);

FILE * pfopen (char path [], char name [], char mode []);

#if defined (SHORT_NAMES)

#define PHASE_NAME "ph%02d.bin"

#else

#define PHASE_NAME "phases%02d.bin"

#endif

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

#endif /*__FEFF_68*/
