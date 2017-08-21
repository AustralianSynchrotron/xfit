/***********************************************************************}
{                                                                       }
{  This file contains the routines common to the refinements            }
{                                                                       }
{***********************************************************************/

#if !defined (__REFINE_C__)

#define __REFINE_C__

#include "refine.h"
#include "common.h"

#include "console.h"


  /*********************************************************************}
  {                                                                     }
  {  Convert any non-scalar parameters in the list to their scalar      }
  {  components                                                         }
  {                                                                     }
  {  Return the number of parameters                                    }
  {                                                                     }
  {*********************************************************************/

int expand_refinement_parameters (parameter_type * name_list, node * x) {

  int parameters, i;

  node * c [3];

  char * name;

  double key;


    /* Go through all of the parameters */

  for (parameters = 0; x; x = x->n [0], parameters++)

    if (x->parameter.command == VECTOR_ID) {

        /* If the parameter is a vector ...

           Make two new nodes for the two further components */

      key = x->parameter.key;

      c [0] = x;

      c [1] = make_node (ID);
      c [2] = make_node (ID);

      c [0]->type = ID;


        /* Get the scalar parameters */

      for (i = 0; i < 3; i++) {

        name = parameter_name (name_list, c [0]->parameter.offset + i);

        parameter_index (name, name_list, &(c [i]->parameter));

        c [i]->parameter.key = key;

        }


        /* Add the new components to the list */

      c [2]->n [0] = c [0]->n [0];
      c [1]->n [0] = c [2];
      c [0]->n [0] = c [1];

      parameters += 2;


        /* Move to the last component */

      x = c [2];

      }


    /* Return the number of parameters */

  return parameters;

  }


  /*********************************************************************}
  {                                                                     }
  {  Print the list of parameters                                       }
  {                                                                     }
  {  Return the number of parameters                                    }
  {                                                                     }
  {*********************************************************************/

int list_refinement_parameters (node * x) {

  int count;

  char * name;

  static char * line = NULL;

  if (!line)

    line = malloc (TERM_WIDTH + 32);
  

    /* List the parameters */

  for (line [0] = count = 0; x; x = x->n [0], count++) {

    name = expression_string (x, 0);

    if (strlen (line) + strlen (name) + 1 >= TERM_WIDTH || count == 0) {

      cout ("%s\n", line);

      strcpy (line, "          ");

      }

    strcat (line, " ");

    strcat (line, name);

    }

  if (line [0])

    cout ("%s\n", line);

  return count;

  }


#endif /* __REFINE_C__ */
