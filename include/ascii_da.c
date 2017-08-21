/***********************************************************************}
{                                                                       }
{  Purpose:                                                             }
{                                                                       }
{       Routines and structures for ASCII data files.                   }
{                                                                       }
{  Content:                                                             }
{                                                                       }
{       type:    ascii_data_type                                        }
{                                                                       }
{       data:    ASCII_next                                             }
{                                                                       }
{       routine: ASCII_next_word destruct_ascii_data ascii_file_column  }
{                ascii_file_column_l read_ascii_data_file               }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__ASCII_DATA_C__)

#define __ASCII_DATA_C__

#include "ascii_da.h"

#include "console.h"


char * ASCII_next_word () {

  static char * next, * end, c;

  if (ASCII_next ? ASCII_next [0] == 0 : 0)

    ASCII_next = NULL;

  if (!ASCII_next)

    return next = end = NULL;

  if (end)

    * end = c;

  for (next = ASCII_next; isspace (*next); next++);

  for (end = next; !isspace (*end) && *end; end++);

  for (ASCII_next = end; isspace (*ASCII_next); ASCII_next++);

  c = *end;

  *end = 0;

  return next;

  }

void destruct_ascii_data (ascii_data_type * A) {

  int c;

  free_check (A->filename);

  for (c = 0; c < A_MAXCOLUMNS; c++) {

    free_check (A->heading [c]);

    free_check (A->column [c]);

    }

  free_check (A->command);

  destruct_comment (&A->comment);

  }

int ascii_file_column (char heading [], ascii_data_type * A) {

  int c;

  for (c = 0; c < A_MAXCOLUMNS; c++)

    if (A->heading [c])

      if (match (A->heading [c], heading) && A->column [c])

        return c;

  return -1;

  }

int ascii_file_column_l (char * heading_list [], ascii_data_type * A,
                         int  * matched) {

  int i, c;

  for (i = 0; heading_list [i]; i++)

    if ((c = ascii_file_column (heading_list [i], A)) >= 0) {

      if (matched)

        *matched = i;

      return c;

      }

  return -1;

  }

int read_ascii_data_file (char filename [], ascii_data_type * A) {

  int    comment, comment0, comment_line, column,
         c_start, c_end, c_column, i, line_number,
         h_start, cols, c, eol, end_command;

  char line [A_MAXLINE];

  double d [A_MAXCOLUMNS];

  FILE * file;

  file = fopen (filename, "r");

  if (file == NULL) {

    cwarn ("can't open experimental data file %s", filename);

    return 0;

    }

  A->filename = strdup (filename);

  A->comment = NULL;

  A->command = NULL;

  A->columns = A->entries = 0;

  for (i = 0; i < A_MAXCOLUMNS; i++) {

    A->heading [i] = NULL;

    A->column [i] = NULL;

    }

  end_command = 0;


    /* Go through the file line by line.

       No line should exceed A_MAXLINE - 2 characters.

       Save any comments and delete them before parsing.

       A comment is any text bounded by one of the following sequences:

         % .... <eol>
         ! .... <eol>

         # .... #

         */

  comment = line_number = 0;

  while (fgets (line, A_MAXLINE, file) != NULL && !end_command) {

      /* Save and delete any comments */

    line_number++;

    column = c_column = c_start = comment_line = 0;

    for (i = 0; line [i] && line [i] != '\n'; i++) {

      comment0 = comment;

      eol = strchr ("\n", line [i + 1]) != NULL;

      if (!comment_line) {

        comment_line = !comment && (line [i] == '!' || line [i] == '%');

        comment = comment_line || (comment ^ line [i] == '#');

        }
      else

        comment = !eol;

      if (!comment0 && comment) {

        c_start = i;

        c_column = column;

        }

      if (eol)

        line [i + 1] = 0;

      if ((comment0 && !comment) || (comment && eol)) {

          /* Save the comment */

        c = line [i + 1]; line [i + 1] = 0;

        add_comment (&A->comment, line_number, c_column, line + c_start);

        line [i + 1] = c;


          /* Delete the comment (use spaces or tabs) */

        for (; c_start <= i; c_start++)

          line [c_start] = line [c_start] == '\t' ? '\t' : ' ';

        }

      if (line [i] == '\t')

        column += 8 - column % 8;

      else

        column++;

      }


      /* Remove any superfluous spaces */

    for (i = 0; line [i]; i++)

      while (isspace (line [i]) && isspace (line [i + 1])) {

        delete_string (line + i + 1, 1);

        line [i] = ' ';

        }

    if (isspace (line [0])) delete_string (line, 1);

    if (isspace (line [max (0, (signed) strlen (line) - 1)]))

        line [(signed) strlen (line) - 1] = 0;


      /* Check if the line contains a command

         (Any text not on the last non-blank line before the data) */

    if (sscanf (line, " %lf", d) <= 0) {

        /* if the line contains the end command, cut the
           line after the command and stop reading at this line */

      for (c_start = 0; line [c_start]; c_start = c_end) {

        while (isspace (line [c_start])) c_start++;

        c_end = c_start;

        while (!isspace (line [c_end]) && line [c_end]) c_end++;

        if (c_end - c_start == 3 && match (line + c_start, "end")) {

          line [c_end] = 0;

          end_command = 1;

          }
        }

      if (!isspace (line [0]) && line [0]) {

        h_start = A->command ? strlen (A->command) : 0;

        A->command = realloc_check (A->command,
                                       h_start + strlen (line) + 4);

        A->command [h_start] = 0;

        if (h_start > 0)

          strcat (A->command, " ");

        strcat (A->command, line);

        }
      }


      /* Check if the line is a data line (starts with a number) */

    if (sscanf (line, " %lf", d) > 0) {

        /* Get the headings from the last non-blank line. */

      if (A->heading [0] == NULL && A->command) {

        char * word;

        ASCII_next = NULL;

        ASCII_next_word ();

        ASCII_next = A->command + h_start;

        for (word = ASCII_next_word (), i = 0; word && i < A_MAXCOLUMNS;
             word = ASCII_next_word (), i++)

          A->heading [i] = strdup (word);

        A->command [h_start] = 0;

        }


        /* Read in the data and add it to the current data.

           If the number of columns changes report an error and terminate */

/*#if defined (__TURBOC__) || defined (__WATCOMC__)*/

        // This is a workaround for a bug in the %n field in sscanf

      strcat (line, " @");

/*#endif*/

      cols = sscan (line, " %l[%n ]f", A_MAXCOLUMNS, d);

      if (A->columns > 0 && A->columns != cols) {

        cwarn ("inconsistent data columns (%s: %d -> %d, line %d)",
                filename, A->columns, cols, line_number);

        return 0;

        }

      for (c = 0; c < cols && c < A_MAXCOLUMNS; c++) {

        A->column [c] = realloc (A->column [c], sizeof (double) *
                                  (1 + (1 + A->entries / 128) * 128));

        A->column [c][A->entries] = d [c];

        }

      A->columns = cols;
      A->entries++;

      }
    }

  fclose (file);

  return A->entries;

  }

#endif /* __ASCII_DATA_C__ */
