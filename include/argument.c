/***********************************************************************}
{                                                                       }
{  Parse command-line arguments                                         }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__ARGUMENTS_C__)

#define __ARGUMENTS_C__

#include "argument.h"


int get_argument (int * argc, char * argv [],
                       char pattern [], char type [], void * value) {

  static char type0 [] = "";

  int n, m, c, i;


    /* Find the first argument matching the pattern and copy the value
       of the following argument depending on the type. */

  if (type == NULL)

    type = type0;

  for (c = 0, n = 1; n < *argc; n++)

    if (argv [n][0] == '-')

      if ((m = match (argv [n] + 1, pattern)) > 0) {

        strcpy (argv [n] + 1, argv [n] + m + 1);

        if (argv [n][1] == 0)

          c += 1;

        if (match (type, "string") || match (type, "number"))

          c += 2 * (*argc > n + 1);

        if (match (type, "string"))

          *((char **) value) = 0;

        if (match (type, "number"))

          *((double *) value) = 0;

        if (c & 2)

          if (match (type, "string"))

            *((char **) value) = argv [n + 1];

          else

            if (match (type, "number"))

              *((double *) value) = atof (argv [n + 1]);

        if (!(c & 1))

          n++;

        c = (c + 1) / 2;

        if (c)

          for (i = n + c; i < *argc; i++)

            argv [i - c] = argv [i];

        *argc -= c;

        return 1;

        }

  return 0;

  }

#endif /* __ARGUMENTS_C__ */
