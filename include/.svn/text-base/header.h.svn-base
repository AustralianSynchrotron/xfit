/***********************************************************************}
{                                                                       }
{  General definitions                                                  }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__HEADER_H__)

#define __HEADER_H__

#define TERM_WIDTH 80

#if defined (ultrix)

#define __STDC__

#define const

#endif

  /* include files */

#include "console.h"

#include <stdlib.h>
#include <stdio.h>

#if defined (ultrix) && !defined (__STDC__)

#define __STDC__

#include <math.h>

#undef __STDC__

#else

#include <math.h>

#endif

#include <ctype.h>
#include <string.h>
#include <time.h>

#if defined (ultrix) || defined (VAXC)

char * cftime (char buf [], char fmt [], time_t * clock);

#endif

#if defined (VAXC) || defined (__TURBOC__)

#define rint(d) (floor ((d) + 0.5))

#endif

#if defined (VAXC)

#define MAXINT 2147483647

#include <float.h>

#define MAX_DOUBLE DBL_MAX
#define MIN_DOUBLE DBL_MIN

#else

#if !defined (__GCC__)

// #include <values.h>

#else

#define MAXINT 2147483647

#include <float.h>

#endif

#endif

#define TERM_WIDTH 80

#define WHITESPACE " \t\r\n"


  /* varargs */

#if defined (__TURBOC__) || defined (__GCC__)

#include <stdarg.h>

#else

#if defined (ultrix)

#undef __STDC__

#endif

#include <stdarg.h>

#if defined (ultrix)

#define __STDC__

#endif

#endif

  /* Numerical constants */

#if !defined (M_PI)

#define M_PI        3.14159265358979323846
#define M_PI_2      1.57079632679489661923
#define M_PI_4      0.785398163397448309616
#define M_1_PI      0.318309886183790671538
#define M_2_PI      0.636619772367581343076
#define M_1_SQRTPI  0.564189583547756286948
#define M_2_SQRTPI  1.12837916709551257390

#endif

#if !defined (M_E)

#define M_E         2.71828182845904523536
#define M_LOG2E     1.44269504088896340736
#define M_LOG10E    0.434294481903251827651

#endif

#if !defined (M_LN2)

#define M_LN2       0.693147180559945309417
#define M_LN10      2.30258509299404568402

#endif

#if !defined (M_SQRT2)

#define M_SQRT2     1.41421356237309504880
#define M_SQRT_2    0.707106781186547524401

#endif

#if !defined (TRUE)

#define TRUE 1

#endif

#if !defined (FALSE)

#define FALSE 0

#endif


  /* MINDOUBLE / MAXDOUBLE */

#if !defined (MINDOUBLE)

#define MINDOUBLE   2.225074E-308
#define MAXDOUBLE   1.797693E+308

#endif


  /* hyperbolic functions */

#if !defined (VAXC)

/* double sinh (double x);

double cosh (double x);

double tanh (double x); */

#endif

#if defined (__TURBOC__) || defined (__GCC__)

double asinh (double x);

double acosh (double x);

double atanh (double x);

#endif


  /* min + max */

#if !defined (min)

#define min(A, B) (((A) <= (B)) ? (A) : (B))

#endif

#if !defined (max)

#define max(A, B) (((A) >= (B)) ? (A) : (B))

#endif


  /* time and date */

#define print_time(A) { time_t time_; char date_ [32]; \
    time (&time_); cftime (date_, A, &time_); cout ("%s", date_); }

#define sprint_time(S, A) { time_t time_; char date_ [32]; \
    time (&time_); cftime (date_, A, &time_); sprintf (S, "%s", date_); }

#define fprint_time(F, A) { time_t time_; char date_ [32]; \
    time (&time_); cftime (date_, A, &time_); fcout (F, "%s", date_); }


  /* swapping */

#define swap_double(A, B) { double xxxx; xxxx = (A); (A) = (B); (B) = xxxx; }
#define swap_float(A, B)  { float  xxxx; xxxx = (A); (A) = (B); (B) = xxxx; }
#define swap_short(A, B)  { short  xxxx; xxxx = (A); (A) = (B); (B) = xxxx; }
#define swap_long(A, B)   { long   xxxx; xxxx = (A); (A) = (B); (B) = xxxx; }
#define swap_int(A, B)    { int    xxxx; xxxx = (A); (A) = (B); (B) = xxxx; }

#define swap(A, B)        { double xxxx; xxxx = (A); (A) = (B); (B) = xxxx; }


  /* arrays */

void get_bounds (double x [], int N, double *xmin, double * xmax);

int get_range (double x [], int N, double xmin, double xmax,
                                     int * imin, int * imax);

int nearest (double x [], int N, double a);


  /* memory allocation */

void * calloc_check_ (size_t nelem, size_t elsize, char * id);

void * calloc_check (size_t nelem, size_t elsize);

void * realloc_check (void * p, size_t size);

void free_check (void * v);

#define n_step(n,s) (((((long) (n)) + ((long) (s)) - 1) / ((long) (s))) \
                     * ((long) (s)))

long realloc_increment (void ** p, long * size,
                                   long nelem, long elsize, long step);


  /* random number */

double frandom (void);

double grandom (void);


  /* string operations */

#if defined (__TURBOC__)

#define strncasecmp(s1,s2,n) strncmpi (s1, s2, n)
#define strcasecmp(s1,s2) strcmpi (s1, s2)

#elif defined (VAXC)

int strncasecmp (const char * s1, const char * s2, int n);

#define strcasecmp(s1,s2) strncasecmp (s1, s2, 2147483647)
#define stricmp(s1,s2) strcasecmp (s1, s2)
#define strncmpi(s1,s2,n) strncasecmp (s1, s2, n)
#define randomize() srand (time (NULL))

#elif defined (__GCC__)

#define strcasecmp(s1,s2)    stricmp (s1, s2)
#define strncasecmp(s1,s2,n) strnicmp (s1, s2, n)

int getch ();

#else

//#define stricmp(s1,s2) strcasecmp (s1, s2)
//#define strncmpi(s1,s2,n) strncasecmp (s1, s2, n)
#define randomize() srandom (time (NULL))

int getch ();

#endif

short strlencmp (const char * s1, const char * s2);

short strlencmp_delim (const char * s1, const char * s2, const char * delim);

short strlencasecmp (const char * s1, const char * s2);

short strlencasecmp_delim (const char * s1, const char * s2,
                                            const char * delim);

short uniquestrlencasecmp_delim  (const char * s1, const char * s2,
                                  const char * delim, short nmin);

short uniquestrlencasecmp (const char * s1, const char * s2, short nmin);

short string_index (const char * s1, const char * s2);

char * insert_string (char * s1, const char * s2);

char * delete_string (char * s1, int length);

char * extract_file_name (char * path);

int expand_line (char line [], int size);

#if defined (__HEADER_C__)

char SPACES [] =

 "\040\040\040\040\040\040\040\040\040\040\040\040\040\040\040\040"
 "\040\040\040\040\040\040\040\040\040\040\040\040\040\040\040\040"
 "\040\040\040\040\040\040\040\040\040\040\040\040\040\040\040\040"
 "\040\040\040\040\040\040\040\040\040\040\040\040\040\040\040\040"
 "\040\040\040\040\040\040\040\040\040\040\040\040\040\040\040\040"
 "\040\040\040\040\040\040\040\040\040\040\040\040\040\040\040\040"
 "\040\040\040\040\040\040\040\040\040\040\040\040\040\040\040\040"
 "\040\040\040\040\040\040\040\040\040\040\040\040\040\040\040\040";

#else

extern char SPACES [];

#endif

#define NSPACES(N) (SPACES + 128 - (N))


  /* varargs */

#if defined (__TURBOC__)

#define va_initialise(ap, p) va_start (ap, p)

#elif defined (__GCC__)

#define va_initialise(ap, p) va_start (ap, p)

#else

#define va_initialise(ap, p) ap = ((char *) &(p)) + sizeof (char *)

#endif


  /* 3-vectors */

double length3 (double R []);

double distance3 (double v1 [], double v2 []);

double dot3 (double v1 [], double v2 []);

double angle3 (double v1 [], double v2 []);

double vertex_angle3 (double R1 [], double R2 [], double R3 []);

double normalise3 (double v []);

double torsion_angle3 (double R1 [], double R2 [],
                                     double R3 [], double R4 []);


  /* files */

int copy (char from [], char to []);

#endif /* _HEADER_H_ */

