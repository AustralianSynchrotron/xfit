/* Routines common to both multiple scattering formalisms */
#define __FEFF_68_C

#include "header.h"
#include "feff_.h"
#include "feff_68.h"

void destruct_path_l (path_l ** path) {

  path_l * next;

  while (*path) {

    next = (*path)->next;

    free_check (*path);

    *path = next;

    }
  }


void print_paths (path_l * l) {

  int i;

  for (; l; l = l->next) {

    cout (" path %2d: %d legs:\n", l->number, l->legs);

    for (i = 0; i < l->legs; i++)

      cout (" atom %2d: %8.3f %8.3f %8.3f    potential %.0f  Z %.0f\n",
                l->atom [i], l->v [i][0], l->v [i][1], l->v [i][2],
                l->potential [i], l->Z [i]);

    cout ("\n");

    }
  }


  /*********************************************************************}
  {                                                                     }
  {  Save the binary phaseshift file (phase.bin)                        }
  {                                                                     }
  {*********************************************************************/

int pcopy (char path [], char from [], char to []) {

  FILE *source, *dest;

  char c [256];

  int n;

  if (!strcmp (from, to))

    return 1;

  source = dest = NULL;

  source = pfopen (path, from, "rb");

  if (source == NULL)

    return 1;

  dest = pfopen (path, to, "wb");

  if (dest == NULL)

    return 1;

  while (!feof (source)) {

    n = fread (c, sizeof (char), 256, source);

    if (n > 0)

      fwrite (c, sizeof (char), n, dest);

    }

  fclose (source);
  fclose (dest);

  return 0;

  }


int premove (char path [], char name []) {

  int o;
  

  strcat (path, PATH_SEP);

  strcat (path, name);

  o = remove (path);

  path [strlen (path) - 1 - strlen (name)] = 0;

  return o;

  }


FILE * pfopen (char path [], char name [], char mode []) {

  FILE * o;

  strcat (path, PATH_SEP);

  strcat (path, name);

  o = fopen (path, mode);

  path [strlen (path) - 1 - strlen (name)] = 0;

  return o;
  
  }


int prename (char path [], char from [], char to []) {

  char * fromc;

  int o;
  
  strcat (path, "\\");

  strcat (path, from);

  fromc = strdup (path);

  path [strlen (path) - strlen (from)] = 0;

  strcat (path, to);

  o = rename (fromc, path);

  path [strlen (path) - 1 - strlen (to)] = 0;

  free (fromc);
  
  return o;

  }
