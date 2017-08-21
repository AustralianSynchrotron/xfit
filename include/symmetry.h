/***********************************************************************}
{									}
{  This file contains routines used to read a set of symmetry		}
{  operations from a file and apply them to any coordinates		}
{									}
{***********************************************************************/

#if !defined (__SYMMETRY_H__)

#define __SYMMETRY_H__

#include "matrix.h"

#define S_OPERATIONS 192

  /*****************************************************************}
  {								    }
  {  Define the structure to hold the description of a space group  }
  {								    }
  {*****************************************************************/

typedef struct {

  int index, Z, Z_special, N;

  char name [32], type [32];

  matrix4 O [S_OPERATIONS];

  } space_group;


void clear_space_group (space_group * group) {

  group->index     =
  group->Z         =
  group->Z_special =
  group->N         = 1;

  strcpy (group->name, "P1");
  strcpy (group->type, "TRICLINIC");

  group->O [0] = i_matrix4;

  m_el (group->O [0], 3, 3) = 0;

  }

int read_space_group (space_group group [], char filename [], int N);

vector3 operation (space_group * group, int n, vector3 v) {

  vector4 o;

  o = v;

  o.rows = 4;

  v_el (o, 3) = 1;

  o = mul_matrix4_vector4 (group->O [n], o);

  o.rows = 3;

  return o;

  } 


int parse_symmetry_operation (char operation [],
                              double x [], double y [], double z [], double d []);

int read_space_group (space_group group [], char filename [], int index) {

  char line [256], line_0 [256], *filename0;
  int op, error;
  FILE *file;

  clear_space_group (group);

  file = fopen (filename0 = filename, "r");

  if (file == NULL)

    if (filename = getenv ("SYMMETRY"))

      file = fopen (filename, "r");

  if (file == NULL)

    file = fopen (filename = "symlib.sym", "r");

  if (file == NULL)

    if (filename = getenv ("DBIN")) {

      strcpy (line, filename);
      strcat (line, "/symlib.sym");

      filename = line;

      file = fopen (filename, "r");

      }

  if (file == NULL) {

    fprintf (stderr, " read_space_group: cannot open symmetry file:\n"
                     "     ./%s  $SYMMETRY  ./symlib.sym"
                     "  $DBIN/symlib.sym\n", filename0);

    return 0;

    }

    /* Find the require space group : line = index number number string string */

  while (fgets (line, 256, file) != NULL) {

    strcpy (line_0, line);

    if (sscanf (line, " %d %d %d %s %s", &group->index, &group->Z, &group->Z_special,
                                         group->name, group->type) >= 3)
      if (group->index == index ||
                 index == 0) {

          /* Read Z symmetry operations (maximum of S_OPERATIONS) */

        if (group->Z > S_OPERATIONS) {

          fprintf (stderr,
              " read_space_group: warning: maximum of %d operations,\n"
              "                   group %d in file %s contains %d (truncated)\n",
              S_OPERATIONS, filename, group->Z);

          group->Z = S_OPERATIONS;

          }

        for (op = 0; op < group->Z; op++) {

          if (fgets (line, 256, file) == NULL) {

            fprintf (stderr, " error: unexpected EOF reading operation %d,"
                             " file %s\n", op + 1, filename);

            group->Z = op;

            return 0;

            }

          strcpy (line_0, line);

          error = parse_symmetry_operation (line, group->x [op], group->y [op],
                                                group->z [op], group->d [op]);

          if (error) {

            fprintf (stderr, "\n error: syntax error reading operation %d,"
                             " file %s\n", op + 1, filename);

            fprintf (stderr, "\n %s ", line_0);
            for (; error > 1; error--)
              fprintf (stderr, "-");
            fprintf (stderr, "^\n\n");

            return (0);

            }
          }

        fclose (file);

        return group->Z;

        }
    }

  fprintf (stderr, " error: unable to find space-group %d in file %s\n",
                     index, filename);

  fclose (file);

  return 0;

  }

int parse_coordinate_operation (char operation [],
                                double x [], double y [], double z [], double d []);

int parse_symmetry_operation (char operation [],
                              double x [], double y [], double z [], double d []) {

  char *coordinate_operation;

  int error;

  coordinate_operation = strtok (operation, ",;\n");

  error = parse_coordinate_operation (coordinate_operation,
	 		              x + 0, y + 0, z + 0, d + 0);

  if (!error) {

    coordinate_operation = strtok (NULL, ",;\n");

    error = parse_coordinate_operation (coordinate_operation,
			              x + 1, y + 1, z + 1, d + 1);

    if (!error) {

      coordinate_operation = strtok (NULL, ",;\n");

      error = parse_coordinate_operation (coordinate_operation,
			                x + 2, y + 2, z + 2, d + 2);

      }
    }

  if (error)

    if (coordinate_operation == NULL)
      error = strlen (operation);
    else
      error += string_index (operation, coordinate_operation);

  return error;

  }

int parse_coordinate_operation (char operation [], vector4 * O) {

  char *conv_err, token [64][32];
  int token_index [64], sign, divide, number, i, n, tokens;
  double v [4], dn, d_;


    /* Check that the string exists */

  if (!operation)

    return 1;


    /* Check that the string consists of:

       " \r\n\t+-0123456789.eExyzXYZ/". */

  if (strspn (operation, " \r\n\t+-0123456789.eExyzXYZ/")

      < strlen (operation))

    return 1;


    /* Break the string up into tokens (+, -, /, x, y, z, number). */

  for (tokens = i = 0; tokens < 64 && operation [i]; i++) {

    if (operation [i] != ' '  && operation [i] != '\r' &&
        operation [i] != '\n' && operation [i] != '\t') {

        /* Get the next token. */

      token_index [tokens] = i + 1;

      switch (tolower (operation [i])) {

        case 'x' :
        case 'y' :
        case 'z' :

        case '+' :
        case '-' :
        case '/' :

          token [tokens++][0] = tolower (operation [i]);
          break;

        default :

          strtod (operation + i, &conv_err);

          if (conv_err == operation + i)
            return (token_index [i]);

          for (n = 0; n < 31 && operation + i != conv_err; i++, n++)
            token [tokens][n] = operation [i];

          token [tokens][n] = 0;
          tokens++;
          i--;
          break;

        }
      }
    }  

    /* Now we have "tokens" tokens.

       Abort if any of x, y, z, and number are not separated by an operator. */

    /* Go through the tokens. */

  v [0] = v [1] = v [2] = v [3] = 0; sign = 1; divide = 0; number = 0;

  for (i = 0, X = 3, M = D = S = 1; i < tokens; i++) {

    if (i == tokens || token [i][0] == '+' || token [i][0] == '-') {

      v [X] += M / D * S;

      X = 3;

      M = D = S = 1;

      }

    if (i == tokens)

      break;

    switch (token [i][0]) {

      case '-' :

        S = -S;

      case '+' :

        break;

      case '/' :

        D_ = 1;

        break;

      case 'x' :
      case 'y' :
      case 'z' :

        X = token [i][0] - 'x';

        break;

      default :

        DE = strtod (token [i], &conv_err);

        if (conv_err == token [i])

          return (token_index [i]);

        if (D_) {

          if (DE == 0)

            return (index);

          D *= DE;

          }
      }
    }


    /* Save the result */

  v_el (*O, 0) = rint (v [0]);
  v_el (*O, 1) = rint (v [1]);
  v_el (*O, 2) = rint (v [2]);
  v_el (*O, 3) = rint (v [3]);


    /* Return no error */

  return 0;

  }

#endif /* __SYMMETRY_H__ */
