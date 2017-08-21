/***********************************************************************}
{                                                                       }
{  This file contains the routines used by the lexer to control input   }
{                                                                       }
{  The routines input and unput are used by lex.yy.c                    }
{                                                                       }
{  The routine yyerror is used by y.tab.c                               }
{                                                                       }
{***********************************************************************/

#if !defined (__INPUT_C__)

#define __INPUT_C__

#include "input.h"

#include "console.h"


  /*********************************************************************}
  {                                                                     }
  {  Include command called by the lexer                                }
  {                                                                     }
  {*********************************************************************/

int include_command (char filename [], file_stack_type ** i_stack) {

  int i;

  if (*filename == 0)

    filename = get_symbol ("INPUT FILE");


  i = 0;

  if (filename)

    if (!*filename)

      filename = NULL;

  if (filename) {

    i = include_file (filename, i_stack);

    if (i)

      cout ("\n reading from %s\n\n", (*i_stack)->file_name);

    else

      cout ("\n invalid command file : %s\n\n", filename);

    }
  else

    cout ("\n no command file\n\n");

  return i;

  }


  /*********************************************************************}
  {                                                                     }
  {  input is called by the lexer (lex.yy.c) to get a character         }
  {                                                                     }
  {*********************************************************************/

int xf_input (void) {

  int c, c_old, c_i, i;

  char * buffer;


  c = 0;

    /* Wait for a character */

  while (c == 0) {

      /* Get the current position in the input file buffer */

    c_i = (command_in->old + command_in->saved) % BUF_SIZE;


      /* Get the character at this point */

    c_old = command_in->c [c_i];


      /* If input is from a file or the stack, get the next character */

    if (command_in->stack) {

      c = command_in->c [c_i];

      if (c == 0)

        c = EOF;

      command_in->stack--;

      }
    else

      if (command_in->file != stdin) {

        c = getc (command_in->file);

        if (c == 0)

          c = EOF;

        command_in->c [c_i] = c;

        }


      /* Check for events including console input */

    if (c == 0) {

      buffer = Poll_event (command_in->file == stdin);

      /* for debugging */

      /* cout("Input: %s\n",buffer); */

        /* If there is console text ... */

      if (buffer) {

          /* Put the text onto the stack */

        for (i = 0; buffer [i]; i++) {

          command_in->c [(c_i + i) % BUF_SIZE] = buffer [i];

          if (command_in->saved < BUF_SIZE)

            command_in->saved++;

          else {

            command_in->old = (command_in->old + 1) % BUF_SIZE;

            move_position (&(command_in->line), &(command_in->column), c_old);

            c_old = command_in->c [command_in->old];

            }
          }

        command_in->stack = strlen (buffer);

        command_in->saved -= command_in->stack;

        }
      }
    }


    /* We have a character

       Save it */

  command_in->c [c_i] = c;

  if (command_in->saved < BUF_SIZE)

      /* If the buffer isn't full increment the size */

    command_in->saved++;

  else {

      /* Otherwise the oldest character is lost

         Change the index to this character and update the position */

    command_in->old = (command_in->old + 1) % BUF_SIZE;

    move_position (&(command_in->line), &(command_in->column), c_old);

    }


    /* <end-of-file> = 0 */

  if (c == EOF)

    c = 0;


    /* Return the character */

  return c;

  }


  /*********************************************************************}
  {                                                                     }
  {  unput is called by the lexer (lex.yy.c) to return a character      }
  {                                                                     }
  {*********************************************************************/

void unput (int c) {

    /* Move the current point back one */

  if (command_in->saved)

    command_in->saved--;


    /* Save the character */

  command_in->c [(command_in->old + command_in->saved) % BUF_SIZE] = c;


    /* Increment the size of the stack of unused characters */

  command_in->stack++;

  }


  /*********************************************************************}
  {                                                                     }
  {  yyerror is called by the parser (y.tab.c) to report any error      }
  {                                                                     }
  {*********************************************************************/

void yyerror (const char * message) {

  long line, column, column_1, column_2, column_0;

  int i, c, lc;

  fflush (stdout);


    /* Get the last character

       If it is 0x13 or 0 return it */

  lc = command_in->c [(command_in->old + command_in->saved - 1) % BUF_SIZE];

  if (lc == '\n' || lc == 0)

    unput (lc);


    /* Get the current position in the file */

  get_position (&line, &column, command_in);


    /* Print the error message */

  cout ("\n %s, line %d: %s ", command_in->file_name, line, message);

  if (lc == '\n')

    cout ("(unexpected end-of-line)");

  if (lc == 0)

    cout ("(unexpected end-of-file)");

  cout ("\n\n");


    /* Find the start of the line */

  for (i = command_in->saved - 1; i >= 0; i--)

    if (command_in->c [(command_in->old + i) % BUF_SIZE] == '\n')

      break;


    /* Get the indentation of the start of the line */

  column_0 = 0;

  if (i < 0)

    column_0 = command_in->column;


    /* Print the line */

  if (column_0 > 0)

    cout (" ...");

  cout (" ");

  for (i++, column_1 = column_2 = column_0; i < command_in->saved; i++) {

    c = command_in->c [(command_in->old + i) % BUF_SIZE];

    move_position (&line, &column_2, c);

    cout ("%-*c", column_2 - column_1, c);

    column_1 = column_2;

    }


    /* Read up until the end of the line */

  while (c = xf_input (), c != '\n' && c != 0) {

    move_position (&line, &column_2, c);

    cout ("%-*c", column_2 - column_1, c);

    column_1 = column_2;

    }


    /* Return the last character <end-of-line> / <end-of-file> */

  if (c == 0)

    unput (c);

  else {

    unput ('\n');

    if (lc != '\n')
    
      unput ('\n');
      
    }


    /* Print the pointer to the error */

  if (column_0)

    cout ("\n ...");

  cout ("\n ");

  for (i = column - 1; i > column_0; i--)

    cout ("-");

  if (lc == '\n' || lc == 0)

    cout ("-");

  cout ("^\n\n");

  }


#endif /* __INPUT_C__ */
