/***********************************************************************}
{                                                                       }
{  This file contains extentions to the printf function                 }
{                                                                       }
{***********************************************************************/

#if !defined (__PRINT_C__)

#define __PRINT_C__

#include "print.h"

#include "console.h"

#include <stdarg.h>


int _printf_fixed_double (char c_ [], int width, int prec, double x);

int _printf_fixed_string (char c_ [], int width, int prec, char x []);


  /*********************************************************************}
  {                                                                     }
  {  _printfx adds the following format flag to _printf                 }
  {                                                                     }
  {     @ : fixed point                                                 }
  {                                                                     }
  {*********************************************************************/

int _printfx (char c [], char format [], va_list * va_l) {

  char * c_0, flag [9], xformat [32];

  int width, prec, flags, fixed, x;


    /* Save the start of the string */

  c_0 = c;


    /* Go through the format looking for format specifiers */

  for (; *format; format++)

    if (*format == '%') {

      flags = fixed = 0;

      width = prec = -1;


        /* Go through to the type-char

           Get the values of the fields as the specifier is traversed */

      for (format++; *format; format++) {

        x = -1;

        if (strchr ("diouxXfeEgGcsnp%", *format)) {

            /* Reached the type-char */

          if (fixed && strchr ("dioucfeEgGs", *format)) {

            double x_;

            char * c_, c2 [2];

            c_ = c2 + 1;

            c2 [1] = 0;

            if (strchr ("diou", *format))

              if (format [-1] == 'l')

                x_ = va_arg ((*va_l), long);

              else

                x_ = va_arg ((*va_l), int);

            if (strchr ("feEgG", *format))

              x_ = va_arg ((*va_l), double);

            if (*format == 's')

              c_ = va_arg ((*va_l), char *);

            if (*format == 'c') {

              c2 [0] = va_arg ((*va_l), int);

              c_ = c2;

              }

            if (c_ == c2 + 1)

              _printf_fixed_double (c, width, prec, x_);

            else

              _printf_fixed_string (c, width, prec, c_);

            }
          else {

              /* Reconstruct the format specifier */

            flag [flags] = 0;

            sprintf (xformat, "%%%s", flag);

            if (width >= 0)

              sprintf (xformat + strlen (xformat), "%d", width);

            if (prec >= 0)

              sprintf (xformat + strlen (xformat), ".%d", prec);

            sprintf (xformat + strlen (xformat), "%c", *format);

            if (strchr ("diouxXc", *format))

              if (format [-1] == 'l')

                sprintf (c, xformat, va_arg ((*va_l), long));

              else

                sprintf (c, xformat, va_arg ((*va_l), int));

            if (strchr ("feEgG", *format))

              sprintf (c, xformat, va_arg ((*va_l), double));

            if (strchr ("snp", *format))

              sprintf (c, xformat, va_arg ((*va_l), char *));

            if (*format == '%')

              sprintf (c, xformat);

            }

          c += strlen (c);

          break;

          }

        if (strchr ("-+ #0@", *format) && width < 0) {

            /* Flag character */

          if (*format == '@')

            fixed = 1;

          else

            if (flags < 8)

              flag [flags++] = *format;

          }
        else

          if (strchr ("0123456789", *format))

              /* Next field width or precision digit */

            x = *format - '0';


        if (*format == '.')

            /* Start of the precision field */

          prec = 0;


        if (*format == '*')

            /* Get the field width or precision from the arguments */

          x = va_arg ((*va_l), int);


          /* Add the next digit to the field width or precision */

        if (x >= 0)

          if (prec < 0)

              /* Field width */

            width = max (width, 0) * 10 + x;

          else

              /* Precision */

            prec = prec * 10 + x;

        }
      }
    else  {

        /* Print the character */

      *c = *format;

      c++;

      }


    /* Terminate the string */

  c [0] = 0;


    /* Return the number of characters printed */

  return strlen (c_0);

  }


int _printf_fixed_double (char c_ [], int width, int prec, double x) {

  char f [32], e [32];

  double fx, ex;

  int c, i;


    /* Print the number in scientific notation */

  sprintf (e, "%#*.*e", width + 1, prec - 4, x);

  if (e [strlen (e) - 5] != 'e')

    sprintf (e, "%#*.*e", width + 1, prec - 5, x);

  c = strlen (e);
  
  e [c - 3] = e [c - 2];
  e [c - 2] = e [c - 1];
  e [c - 1] = 0;


    /* If the number will fit in the field, print it in normal notation */

  if (fabs (x) < pow (10, width - prec)) {

    sprintf (f, "%*.*f", width, prec, x);


      /* Get the number of significant figures */

    i = strlen (f) - strspn (f, " -0.");


      /* Scan the number back in */

    sscanf (f, " %lf", &fx);
    sscanf (e, " %lf", &ex);


      /* Use the format giving the closest representation */

    if (fabs (fx - x) <= fabs (ex - x) && i >= prec - 3 &&

        strlen (f) <= width || x == 0) {

        /* Remove any trailing 0 */

      for (c = '0', i = strlen (f) - 1;

           i > 0 && (f [i] == '0' || f [i] == '.') && c != '.'; i--) {

        c = f [i];

        f [i] = ' ';

        }

      strcpy (e, f);

      }
    }

  sprintf (c_, "%s", e);

  return strlen (c_);

  }

int _printf_fixed_string (char c [], int width, int prec, char x []) {

  char name [32];

  int i;


  sprintf (name, "%*s", width - prec - 1, x);

  i = sprintf (c, "%-*s", width, name);

  return i;

  }


  /*********************************************************************}
  {                                                                     }
  {  <s/f>printfx adds the following format flag to <s/f>printf         }
  {                                                                     }
  {     @ : fixed point                                                 }
  {                                                                     }
  {*********************************************************************/

int sprintfx (char c [], char format [], ...) {

  int count;

  va_list va_l;

  va_start (va_l, format);

  count = _printfx (c, format, &va_l);

  va_end (va_l);

  return count;

  }

int fprintfx (FILE * file, char format [], ...) {

  char buffer [1024];

  int count;

  va_list va_l;

  va_start (va_l, format);

  count = _printfx (buffer, format, &va_l);

  va_end (va_l);

  fcout (file, "%s", buffer);

  return count;

  }

int printfx (char format [], ...) {

  char buffer [1024];

  int count;

  va_list va_l;

  va_start (va_l, format);

  count = _printfx (buffer, format, &va_l);

  va_end (va_l);

  cout ("%s", buffer);

  return count;

  }


#endif /* __PRINT_C__ */
