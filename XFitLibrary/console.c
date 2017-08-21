
/***********************************************************************}
{                                                                       }
{    CONSOLE.C : console routines                                       }
{                                                                       }
{    A circular buffer is used to store lines.                          }
{                                                                       }
{***********************************************************************/

#if !defined (CONSOLE_C)

#define CONSOLE_C

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <stdarg.h>
#include <string.h>
#include <windows.h>
#include <i86.h>


void flush_cin () {

  fflush (stdin);

  }


char * cout_line (const char format [], va_list arg_ptr) {

  static char s_line [1024];

  int count;

  count = vsprintf (s_line, format, arg_ptr);

  if (count == EOF)

    return NULL;

  else

    return s_line;

  }



int fcout_G (FILE * file, const char format [], va_list arg_ptr) {

  char * line;


  if (file != stdout && file != stderr && file)

    return vfprintf (file, format, arg_ptr);


  line = cout_line (format, arg_ptr);

  if (!line)

    return EOF;


  printf ("%s", line);

  return strlen (line);
 
  }


int fcout (FILE * file, const char format [], ...) {

  int count;


  va_list arg_ptr;

  va_start (arg_ptr, format);

  count = fcout_G (file, format, arg_ptr);

  va_end (arg_ptr);


  return count;

  }


int cout (const char format [], ...) {

  int count;

  va_list arg_ptr;


  va_start (arg_ptr, format);

  count = fcout_G (stdout, format, arg_ptr);

  va_end (arg_ptr);


  return count;

  }


int c_we (int error, const char format [], va_list arg_ptr) {

  int count;

  char * line;

  static char line_nomsg [] = "(invalid error message)";


  line = cout_line (format, arg_ptr);

  if (!line) {

    line = line_nomsg;

    count = EOF;

    }
  else

    count = strlen (line);

  /* MessageBox (NULL, line, error ? "ERROR" : "WARNING",
                         (error ? MB_ICONERROR : MB_ICONWARNING) | MB_SYSTEMMODAL);*/

  return count;

  }


int cwarn (const char format [], ...) {

  int count;

  va_list arg_ptr;


  va_start (arg_ptr, format);

  count = c_we (0, format, arg_ptr);

  va_end (arg_ptr);

  return count;

  }


int cerror (const char format [], ...) {

  va_list arg_ptr;


  va_start (arg_ptr, format);

  c_we (1, format, arg_ptr);

  va_end (arg_ptr);

  exit (1);

  return 0;

  }


int cin (int wait) {

  int c;


    // Get the next character from the keyboard

  while (wait && !kbhit ())

    delay (50);


  if (kbhit ())

    c = getche ();


  return c;

  }


int openconsole () {

  return 1;

  }


#endif // CONSOLE_C


