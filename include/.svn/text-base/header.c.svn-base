/***********************************************************************}
{                                                                       }
{  General definitions                                                  }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__HEADER_C__)

#define __HEADER_C__

#include "header.h"

#include "console.h"

#if defined (ultrix) || defined (VAXC)

char * cftime(char buf [], char fmt [], time_t * clock) {

  if (!strcmp (fmt, "%H:%M %e-%b-%y") ||
      !strcmp (fmt, "%H:%M %d-%b-%y"))

    sprintf (buf, "00:00 01-Jan-00");

  if (!strcmp (fmt, "%H:%M"))

    sprintf (buf, "00:00");

  if (!strcmp (fmt, "%e-%b-%y") ||
      !strcmp (fmt, "%e-%b-%y"))

    sprintf (buf, "01-Jan-00");

  }

#endif


  /* hyperbolic functions */

#if !defined (VAXC) && 0

double sinh (double x) {

  double e;

  e = exp (x);

  return (e - 1 / e) / 2;

  }

double cosh (double x) {

  double e;

  e = exp (x);

  return (e + 1 / e) / 2;

  }

double tanh (double x) {

  double e;

  e = exp (x);

  return (e - 1 / e) / (e + 1 / e);

  }

#endif

#if defined (__TURBOC__) || defined (__GCC__)

double asinh (double x) {

  return log (x + sqrt (x * x + 1));

  }

double acosh (double x) {

  return log (x + sqrt (x * x - 1));

  }

double atanh (double x) {

  return log (sqrt ((1 + x) / (1 - x)));

  }

#endif


  /* arrays */

void get_bounds (double x [], int N, double * xmin, double * xmax) {

    /* Finds the minimum and maximum values of x. */

  int i;

  *xmin = *xmax = x [0];

  for (i = 1, x++; i < N; i++, x++) {

    if (*x < *xmin)

      *xmin = *x;

    if (*x > *xmax)

      *xmax = *x;

    }
  }

int get_range (double x [], int N, double xmin, double xmax,
                                    int * imin, int * imax) {

    /* Get the region x [imin] .. x [imax] bounded by xmin and xmax. */

  int i_min, i_max;

  if (xmin > xmax)

    swap (xmin, xmax);

  if (x [N - 1] > x [0]) {

    if (xmin > x [N - 1] || xmax < x [0]) return 0;

    for (i_min = 0;     x [i_min] < xmin; i_min++);

    for (i_max = N - 1; x [i_max] > xmax; i_max--);

    }
  else {

    if (xmin > x [0] || xmax < x [N - 1]) return 0;

    for (i_min = 0;     x [i_min] > xmax; i_min++);

    for (i_max = N - 1; x [i_max] < xmin; i_max--);

    }

  if (imin)

    *imin = i_min;

  if (imax)

    *imax = i_max;

  return 1;

  }

int nearest (double x [], int N, double a) {

    /* returns the index (0 .. N-1) to the entry in the array x,
       nearest to a. */

  double d, d_min;

  int i, i_min;

  if (N <= 0)

    return (-1);

  d_min = fabs (x [0] - a);

  for (i = 1, i_min = 0; i < N; i++) {

    d = fabs (x [i] - a);

    if (d < d_min) {

      d_min = d;
      i_min = i;

      }
    }

  return i_min;

  }


  /* memory allocation */

void * calloc_check_ (size_t nelem, size_t elsize, char * id) {

  void * v;

  v = calloc (nelem, elsize);

  if (v == NULL)

    cwarn ("%s : unable to allocate heap space (%d x %d bytes)", id, nelem, elsize);

#ifdef DEBUG_M

  cout (" calloc  : 0x%p = %d x %d\n", v, nelem, elsize);

#endif

  return v;

  }

void * calloc_check (size_t nelem, size_t elsize) {

  void * v;

  v = calloc (nelem, elsize);

  if (v == NULL)

    cerror ("calloc_check : unable to allocate heap space (%d x %d bytes)", 
                            nelem, elsize);

#ifdef DEBUG_M

  cout (" calloc  : 0x%p = %d x %d\n", v, nelem, elsize);

#endif

  return v;

  }

void * realloc_check (void * p, size_t size) {

  void * v;

  v = realloc (p, size);

  if (v == NULL && size > 0)

    cerror ("realloc_check : unable to allocate heap space (%d bytes)", size);

#ifdef DEBUG_M

  cout (" realloc_check  : 0x%p = %d\n", v, size);

#endif

  return v;

  }

void free_check (void * v) {

#ifdef DEBUG_M

  cout (" free    : 0x%p\n", v);

#endif

  if (v != NULL)

    free (v);

  }

long realloc_increment (void ** p, long * size,
                                   long nelem, long elsize, long step) {

    /* Reallocates the space pointed to by *p to be nelem elements of elsize.
       The final space will be the multiple of step next above nelem or
       *size, whichever is larger.  Any old elements are preserved */

  void * v;

  if (nelem < 0 || elsize < 0 || *size < 0 || step <= 0)

    return 0;

  if (nelem <= *size || elsize == 0)

    return nelem;

  nelem = n_step (nelem, step);

#if defined (DEBUG_M)

  v = realloc_check (*p, nelem * elsize);

#else

  v = realloc (*p, nelem * elsize);

#endif

  if (v == NULL)

    return 0;

  *p = v;

  *size = nelem;

  return nelem;

  }


  /* random number */

double frandom () {

#if defined (RAND_MAX)

  return (rand () / (double) RAND_MAX);

#elif defined (VAXC)

  return (rand () * 4.6566128752457969e-10);

#else

  return (random () * 4.6566128752457969e-10);

#endif

  }

double grandom () {

    /* Return a random number with a Gaussian distribution and
       RMS deviation around 0 of 1.

       If the number is generated by adding frandom N times,
       divide the resulting number by sqrt (N / 12) */

  return (frandom () + frandom () + frandom () + frandom () +
          frandom () + frandom () + frandom () + frandom () - 4)

          / 0.8164965809277260;

  }


  /* string operations */

#if defined (__TURBOC__)

#elif defined (VAXC)

int strncasecmp (const char * s1, const char * s2, int n) {

  int i;

  for (i = 0; s1 [i] && s2 [i] && i < n; i++)

    if (s1 [i] != s2 [i])

      return s1 [i] - s2 [i];

  if (i == n)

    return 0;

  if (s1 [i])

    return s1 [i];

  return - (int) s2 [i];

  }

#else

/*int getch () {

  fflush (stdin);

  getchar ();

  }*/

#if !defined (__GCC__)

/*short strlwr (char * s) {

  for (; *s; s++)

    s [0] = tolower (s [0]);

  }*/

#endif

#endif

short strlencmp (const char * s1, const char * s2) {

  return strncmp (s1, s2, strlen (s2));

  }

short strlencmp_delim (const char * s1, const char * s2, const char * delim) {

  short i;

  if ((i = strncmp (s1, s2, strlen (s2))) != 0)

    return i;

  if (!strchr (delim, s1 [strlen (s2)]))

    return 1;

  return 0;

  }

short strlencasecmp (const char * s1, const char * s2) {

  return strncasecmp (s1, s2, strlen (s2));

  }

short strlencasecmp_delim (const char * s1, const char * s2,
                                            const char * delim) {

  short i;

  if ((i = strncasecmp (s1, s2, strlen (s2))) != 0)

    return i;

  if (!strchr (delim, s1 [strlen (s2)]))

    return 1;

  return 0;

  }

short uniquestrlencasecmp_delim (const char * s1, const char * s2,
                                 const char * delim, short nmin) {

  short i, d;

  if (nmin > 0) {

    if ((i = strncasecmp (s1, s2, nmin)) != 0)

      return i;

    for (i = nmin; !strchr (delim, s1 [i]) && s2 [i]; i++)

      if ((d = (tolower (s1 [i]) - tolower (s2 [i]))) != 0)

        return d;

    if (!strchr (delim, s1 [i]))

      return 1;

    return 0;

    }

  return strlencasecmp_delim (s1, s2, delim);

  }

short uniquestrlencasecmp (const char * s1, const char * s2, short nmin) {

  return uniquestrlencasecmp_delim (s1, s2, "", nmin);

  }

short string_index (const char * s1, const char * s2) {

  int i;

  if (s1 == NULL || s2 == NULL)

    return 0;

  for (i = 0; s1 + i < s2; i++);

  for (     ; s1 + i > s2; i--);

  return i;

  }

char * insert_string (char * s1, const char * s2) {

  int i, l;

  i = strlen (s1);
  l = strlen (s2);

  s1 [i + l] = '\0';

  for (i = strlen (s1) - 1; i >= 0; i--)

    s1 [i + l] = s1 [i];

  for (i = 0; i < l; i++)

    s1 [i] = s2 [i];

  return s1;

  }

char * delete_string (char * s1, int length) {

  int i, l;

  if (length > 0) {

    l = strlen (s1);

    for (i = 0; i < l - length; i++)

      s1 [i] = s1 [i + length];

    s1 [i] = 0;

    }

  return s1;

  }

char * extract_file_name (char * path) {

  if (strrchr (path, ']'))

    return strrchr (path, ']') + 1;

  if (strrchr (path, '/'))

    return strrchr (path, '/') + 1;

  if (strrchr (path, '\\'))

    return strrchr (path, '\\') + 1;

  return path;

  }

int expand_line (char line [], int size) {

  static char tab [] = "       ";

  int i, n;

    /* Expand the line, converting tabs to spaces and removing
       carriage returns and line feeds. */

  for (i = 0; i < size && line [i]; i++) {

    switch (line [i]) {

      case '\t':

        n = 7 - i % 8;

        if (i + 1 + n > size) {

          for (; i + 1 < size; i++)

            line [i] = ' ';

          line [i] = 0;

          i--;

          break;

          }

        if (strlen (line) + n > size)

          line [size - 1 - n] = 0;

        insert_string (&line [i], &tab [i % 8]);

        i += 7 - i % 8;
        line [i] = ' ';

        break;

      case '\r':
      case '\n':

        delete_string (&line [i], 1);

        i--;

      }
    }

  return i;

  }


  /* 3-vectors */

double length3 (double R []) {

  return sqrt (R [0] * R [0] + R [1] * R [1] + R [2] * R [2]);

  }

double distance3 (double v1 [], double v2 []) {

  double R [3];

  R [0] = v1 [0] - v2 [0];
  R [1] = v1 [1] - v2 [1];
  R [2] = v1 [2] - v2 [2];

  return sqrt (R [0] * R [0] + R [1] * R [1] + R [2] * R [2]);

  }

double dot3 (double v1 [], double v2 []) {

  return v1 [0] * v2 [0] + v1 [1] * v2 [1] + v1 [2] * v2 [2];

  }

double angle3 (double v1 [], double v2 []) {

  double l1, l2, c;

  l1 = length3 (v1);
  l2 = length3 (v2);

  if ((l1 * l2) < 1e-16)

    return 0;

  c = (v1 [0] * v2 [0] + v1 [1] * v2 [1] + v1 [2] * v2 [2])
             / (l1 * l2);

  return acos (min (max (c, -0.999999999999999),
                             0.999999999999999));

  }

double vertex_angle3 (double R1 [], double R2 [], double R3 []) {

  double d0 [3], d1 [3];

  d0 [0] = R1 [0] - R2 [0];
  d0 [1] = R1 [1] - R2 [1];
  d0 [2] = R1 [2] - R2 [2];

  d1 [0] = R2 [0] - R3 [0];
  d1 [1] = R2 [1] - R3 [1];
  d1 [2] = R2 [2] - R3 [2];

  return angle3 (d0, d1);

  }

double normalise3 (double v []) {

  double l;

  l = length3 (v);

  if (l == 0)

    return 0;

  v [0] /= l;
  v [1] /= l;
  v [2] /= l;

  return l;

  }

double torsion_angle3 (double R1 [], double R2 [],
                                     double R3 [], double R4 []) {

  double py, pz, d1 [3], d3 [3], x [3], y [3], z [3];

  d1 [0] = R2 [0] - R1 [0];
  d1 [1] = R2 [1] - R1 [1];
  d1 [2] = R2 [2] - R1 [2];

  x [0] = R3 [0] - R2 [0];
  x [1] = R3 [1] - R2 [1];
  x [2] = R3 [2] - R2 [2];

  d3 [0] = R4 [0] - R3 [0];
  d3 [1] = R4 [1] - R3 [1];
  d3 [2] = R4 [2] - R3 [2];

  normalise3 (x);

  y [0] = x [1] * d3 [2] - x [2] * d3 [1];
  y [1] = x [2] * d3 [0] - x [0] * d3 [2];
  y [2] = x [0] * d3 [1] - x [1] * d3 [0];

  normalise3 (y);

  z [0] = x [1] * y [2] - x [2] * y [1];
  z [1] = x [2] * y [0] - x [0] * y [2];
  z [2] = x [0] * y [1] - x [1] * y [0];

  py = d1 [0] * y [0] + d1 [1] * y [1] + d1 [2] * y [2];
  pz = d1 [0] * z [0] + d1 [1] * z [1] + d1 [2] * z [2];

  return -atan2 (py, pz);

  }


  /* files */

int copy (char from [], char to []) {

  FILE *source, *dest;

  char c [256];

  int n;

  if (!strcmp (from, to))

    return 1;

  if ((source = fopen (from, "rb")) == NULL)

    return 1;

  if ((dest   = fopen (to,   "wb")) == NULL)

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

#endif /* _HEADER_C_ */

