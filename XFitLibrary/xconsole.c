
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
#if defined(WIN32)
#include <conio.h>
#endif
#include <stdarg.h>
#include <string.h>

#if defined(WIN32)
#include <windows.h>

typedef int __stdcall fCout (void *, char *);
typedef char * __stdcall fSymbol (void *, char *, char *);

#else
typedef int  fCout (void *, char *);
typedef char * fSymbol (void *, char *, char *);

#endif

extern void *  PCal;
extern fCout * Cout;
extern fSymbol * GetSymbol;


const char * get_symbol (const char key []) {

  /* for debugging */
  char * ret_val;
  printf ("GetSymbol (%x/%x) called, value %s ",GetSymbol,PCal,key);
  if (GetSymbol && PCal)
    {

    ret_val = GetSymbol (PCal, key, NULL);
    printf("...Returned %s\n",ret_val);
    return ret_val;
    }

  return NULL;

  }


const char * set_symbol (const char key [], const char value []) {

  if (GetSymbol && PCal)

    return GetSymbol (PCal, key, value);

  return NULL;

  }


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
  int count;


  if (file != stdout && file != stderr && file)

    return vfprintf (file, format, arg_ptr);


  line = cout_line (format, arg_ptr);

  if (!line)

    return EOF;

  /* for debugging */
  /* count = strlen (line);*/

  Cout (PCal, line);
  /* printf("%s: %d\n",line,count);*/

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

#if defined(WIN32)
  MessageBox (NULL, line, error ? "ERROR" : "WARNING",
                         (error ? MB_ICONERROR : MB_ICONWARNING) | MB_SYSTEMMODAL);
  /* We could call a graphics routine here, but for now we just output a warning
     message to the console */
#else
  cout(line);
  cout("\n");
#endif
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


int openconsole () {

  return 1;

  }


#endif // CONSOLE_C


