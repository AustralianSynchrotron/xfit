/***********************************************************************}
{                                                                       }
{  This file contains the definitions of various functions used for     }
{  pattern matching and extracting values from strings.                 }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__SCAN_C__)

#define __SCAN_C__

#include "scan.h"

#include "header.h"


int match (const char string [], const char pattern []) {

  int i, p;

  for (i = p = 0; string [i] && pattern [p]; p++)

    if (pattern [p] == ' ')

      while (string [i] && strchr (WHITESPACE, string [i]))

        i++;

    else

      if (tolower (pattern [p]) != tolower (string [i]))

        return 0;

      else

        i++;

  if (strlen (pattern + p) != strspn (pattern + p, " "))

    return 0;

  return i;

  }

int match_l (const char string [], char pattern [], ...) {

  va_list ap;

  char * p;

  int m;

  va_start (ap, pattern);

  for (m = 0, p = pattern; m <= 0 && p; p = va_arg (ap, char *))

    m = match (string, p);

  va_end (ap);

  return m;

  }


  /*********************************************************************}
  {                                                                     }
  {  sscan adds the following format specifications to sscanf :         }
  {                                                                     }
  {      "%[*][width][h|l][[%n[separator]]]                             }
  {                        [d|D|o|O|i|I|u|U|x|X|e|E|f|g|G|s|c]"         }
  {                                                                     }
  {  The "[%n[separator]]" sequence before the type character causes    }
  {  the specifier to be expanded as a sequence scanning a set of       }
  {  fields :                                                           }
  {                                                                     }
  {      "%3[%n , ]d", 3, d -> "%3d , %3d , %3d",                       }
  {                              &d [0], &d [1], &d [2]                 }
  {                                                                     }
  {*********************************************************************/

int sscan (const char s [], const char format [], ...) {

  char eformat [256];

  int ei, i, io, start, end, cut, array, nread, nass, have_format;

  char type, arg_type;

  va_list ap;

  va_start (ap, format);


    /* Go through the format, finding all % format specifiers */

  for (nread = nass = ei = i = 0, io = -1; format [i];) {

    if (io == i) i++;

    eformat [ei++] = format [io = i];
    eformat [ei] = format [i + 1];

    have_format = 0;
    array = 0;

    if ((format [i] == '\\') ||
        (format [i] == '%' && format [i + 1] == '%')) {

      ei++;
      i += 2;

      continue;

      }

    if (format [i + 1] == '%' || format [i + 1] == 0) {

      i++;
      type = '*';
      eformat [ei] = 0;
      have_format = 1;

      }

    if (format [i] == '%' && !have_format) {

        /* start of a format specifier */

      for (ei = 0; format [i]; ei++, i++) {

        if (!strncmp (format + i, "[%n", 3)) {

          array = 1;
          i += 3;
          start = i;

          for (end = start; format [end] && format [end] != ']'; end++);

          if (format [end] == 0)

            return -1;

          ei--;
          i += end - start;

          continue;

          }

        eformat [ei] = format [i];

        if (strchr ("dDoOiIuUxXeEfgGscnp", format [i])) {

          eformat [ei + 1] = 0;
          cut = strlen (eformat);

          if (array) {

            strncat (eformat, format + start, end - start);
            eformat [cut + end - start] = 0;

            }

          have_format = 1;
          type = eformat [cut - 1];
          arg_type = 0;

          if (strchr ("hl", eformat [cut - 2]))

            arg_type = eformat [cut - 2];

          i++;

          break;

          }
        }
      }

    if (have_format) {

        /* we have a format specifier in eformat

           cut : point in eformat before the delimiter
           array : true if this is an array
           arg_type : h, l or ' '
           type : one of dDoOiIuUxXeEgGscnp% or *
           nread : characters scanned so far
           nass : fields assigned so far

         */

      char * p;

      int size, n, nr, na, x;

      ei = 0;

      size = sizeof (char);

      if (arg_type == 'h')

        size = sizeof (short);

      else

        switch (type) {

          case 'd' : case 'D' :
          case 'o' : case 'O' :
          case 'i' : case 'I' :
          case 'u' : case 'U' :
          case 'x' : case 'X' :

            size = arg_type == 'l' ? sizeof (long) : sizeof (int);

            break;

          case 'e' : case 'E' :
          case 'f' :
          case 'g' : case 'G' :

            size = arg_type == 'l' ? sizeof (double) : sizeof (float);

            break;

          case 'n' : case 'p' :

            size = sizeof (void *);

          }

      if (array && type != '*')

        n = va_arg (ap, int);

      else

        n = 1;

      if (type != '*') {

        p = va_arg (ap, char *);

        if (p == NULL)

          return -1;

        }

      strcat (eformat, "%n");

      for (x = 0; x < n; x++) {

        if (x == n - 1 && array) {

          eformat [cut] = 0;
          strcat (eformat, "%n");

          }

        if (type == '*') {

          na = sscanf (s + nread, eformat, &nr);

          if (na < 0)

            return nass;

          }
        else {

          na = sscanf (s + nread, eformat, p + x * size, &nr);

          if (na < 1)

            return nass;

          }

        if (type == 'n')

          if (arg_type == 'l')

            * ((long *) p) += nread;

          else

            * ((int *) p) += nread;

        nass += na;
        nread += nr;

        }
      }
    }

  va_end (ap);

  return nass;

  }

#endif /* __SCAN_C__ */
