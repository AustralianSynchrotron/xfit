/***********************************************************************}
{                                                                       }
{  This file contains the parameter types and manipulation routines     }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__PARAMETERS_C__)

#define __PARAMETERS_C__

#include "paramete.h"


  /*********************************************************************}
  {                                                                     }
  {  Functions to manipulate parameter sets                             }
  {                                                                     }
  {*********************************************************************/

void clear_parameter_set (parameter_set * set) {

  int O;

  for (O = 0; O < 16; O++) {

    set->jrhvector [O] = 0;

    set->vectors [O] = 0;

    set->key [O] = 0;

    }

  set->current = 0;

  }


  /*********************************************************************}
  {                                                                     }
  {  Create a new parameter vector in the set O (0 .. 15)               }
  {                                                                     }
  {  Return the key                                                     }
  {                                                                     }
  {*********************************************************************/

double create_parameter_vector (parameter_set * set, int O) {

  int count;

  if (O < 0 || O > 15)

    return 0;

  count = set->vectors [O];

  set->jrhvector [O] = realloc (set->jrhvector [O], (count + 1) * sizeof (vector));

  set->key [O] = realloc (set->key [O], (count + 1) * sizeof (double));

  if (set->jrhvector [O] == NULL || set->key [O] == NULL) {

    set->vectors [O] = 0;

    return 0;

    }

  set->vectors [O]++;

  set->jrhvector [O][count] = null_vector;

  set->key [O][count] = ++set->current;

  return set->key [O][count];

  }


  /*********************************************************************}
  {                                                                     }
  {  Delete parameter vector i in the set O (0 .. 15)                   }
  {                                                                     }
  {  Return 0 if it doesn't exist                                       }
  {                                                                     }
  {*********************************************************************/

int delete_parameter_vector_Oi (parameter_set * set, int O, int i) {

  int count;

  if (O < 0 || O > 15)

    return 0;

  count = set->vectors [O];

  if (i < 0 || i >= count)

    return 0;

  for (i++; i < count; i++) {

    set->jrhvector [O][i - 1] = set->jrhvector [O][i];

    set->key [O][i - 1] = set->key [O][i];

    }

  set->vectors [O] = count - 1;

  return 1;

  }


  /*********************************************************************}
  {                                                                     }
  {  Delete the parameter vector with the matching key                  }
  {                                                                     }
  {  Return 0 if it doesn't exist                                       }
  {                                                                     }
  {*********************************************************************/

int delete_parameter_vector (parameter_set * set, double key) {

  int O, i;


    /* Go through all of the vectors, looking for a matching key */

  for (O = 0; O < 16; O++)

    for (i = 0; i < set->vectors [O]; i++)

      if (set->key [O][i] == key)

        return delete_parameter_vector_Oi (set, O, i);

  return 0;

  }


  /*********************************************************************}
  {                                                                     }
  {  Return a pointer to vector i in set O                              }
  {                                                                     }
  {*********************************************************************/

vector * parameter_vector_Oi (parameter_set * set, int O, int i) {

  if (!set)

    return NULL;

  if (O < 0 || O > 15)

    return NULL;

  if (i < 0 || i >= set->vectors [O])

    return NULL;

  return set->jrhvector [O] + i;

  }


  /*********************************************************************}
  {                                                                     }
  {  Return the key to vector i in set O                                }
  {                                                                     }
  {*********************************************************************/

double parameter_vector_key_Oi (parameter_set * set, int O, int i) {

  if (O < 0 || O > 15)

    return 0;

  if (i < 0 || i >= set->vectors [O])

    return 0;

  return set->key [O][i];

  }


  /*********************************************************************}
  {                                                                     }
  {  Given the key, return a pointer to the parameter vector            }
  {                                                                     }
  {*********************************************************************/

vector * parameter_vector (parameter_set * set, double key) {

  int O, i;


    /* Go through all of the vectors, looking for a matching key */

  for (O = 0; O < 16; O++)

    for (i = 0; i < set->vectors [O]; i++)

      if (set->key [O][i] == key)

        return set->jrhvector [O] + i;

  return NULL;

  }


  /*********************************************************************}
  {                                                                     }
  {  Resize a vector while retaining the current contents               }
  {                                                                     }
  {*********************************************************************/

int resize_parameter_vector (vector * parameters, int rows) {

  int l;

  if (rows > parameters->rows) {

    l = ((rows + 63) / 64) * 64;

    if (!resize_vector_retain (parameters, l))

      return 0;

    }

  parameters->rows = rows;

  return 1;

  }


  /*********************************************************************}
  {                                                                     }
  {  Set the value of a parameter to be equal to a short vector value   }
  {                                                                     }
  {  The parameter is assumed to exist                                  }
  {                                                                     }
  {*********************************************************************/

int set_parameter (parameter_set * set,

                    parameter_type parameter, vector4 value) {

  int index;

  vector4 v;

  vector * P;


    /* Get the parameter vector */

  P = parameter_vector (set, parameter.key);

  if (index = parameter.index)

    index--;

  switch (parameter.type) {

    case ATOM_t :

    case NORMAL :

      v_el (*P, parameter.offset) = scal (value);

      break;

    case VECTOR :

      copy_vector4 (&v_el (*P, parameter.offset + index), value);

      break;

    case PVECTOR :

      v = polar_vector4 (vector3 (&v_el (*P, parameter.offset)));

      copy_vector4 (&v_el (v, index), value);

      copy_vector4 (&v_el (*P, parameter.offset), vector_polar4 (v));

    }

  return 1;

  }


  /*********************************************************************}
  {                                                                     }
  {  Set the value of a parameter to be equal to a short vector value   }
  {                                                                     }
  {  If the parameter does not exist, it is created                     }
  {                                                                     }
  {*********************************************************************/

int assign_parameter (parameter_set * set,

                       parameter_type parameter, vector4 value) {

  int n;

  vector * P;


    /* Get the parameter vector */

  P = parameter_vector (set, parameter.key);

  n = parameter.offset + parameter.index + value.rows;

  if (parameter.index)

    n--;

  if (n > P->rows)

    n = resize_parameter_vector (P, n);

  if (n == 0)

    return -2;

  set_parameter (set, parameter, value);

  return 0;

  }


  /*********************************************************************}
  {                                                                     }
  {  Get the value of a parameter                                       }
  {                                                                     }
  {  The parameter is assumed to exist                                  }
  {                                                                     }
  {*********************************************************************/

vector4 parameter_value (parameter_set * set, parameter_type parameter) {

  vector4 v;

  vector * P;


    /* Get the parameter vector */

  P = parameter_vector (set, parameter.key);

  v = o_vector3;

  v.rows = 1;

  switch (parameter.type) {

    case ATOM_t :

    case NORMAL :

      return (make_vector1 (v_el0 (*P, parameter.offset)));

    case VECTOR :

      v = pick_vector3 (*P, parameter.offset);

      break;

    case PVECTOR :

      v = polar_vector4 (pick_vector3 (*P, parameter.offset));

    }

  if (!parameter.index)

    return v;

  return make_vector1 (v_el (v, parameter.index - 1));

  }


  /*********************************************************************}
  {                                                                     }
  {  Get the derivative of a parameter (f) with respect to another (x)  }
  {                                                                     }
  {*********************************************************************/

vector4 dparameter_value (parameter_set * set,

                          parameter_type f, parameter_type x) {

  vector4 v;

  matrix4 df_dx;

  vector * P;


    /* Get the parameter vector */

  P = parameter_vector (set, f.key);

  v = o_vector3;

  if (f.key == x.key) {

    switch (f.type) {

      case ATOM_t :

      case NORMAL :

        return make_vector1 (f.offset == x.offset);

      case VECTOR :

      case PVECTOR :

        if ((x.type != VECTOR && x.type != PVECTOR) ||

             f.offset != x.offset)

          break;

        if (!x.index)

          return make_vector1 (0);

        if (f.type == x.type) {

          if (f.index)

            return make_vector1 (f.index == x.index);

          v_el (v, x.index - 1) = 1;

          return v;

          }

        if (f.type == PVECTOR)

          df_dx = dpolar_dvector3 (pick_vector3 (*P, f.offset));

        if (f.type == VECTOR)

          df_dx = dvector_dpolar3 (polar_vector4
                                  (pick_vector3 (*P, f.offset)));

        if (f.index)

          return make_vector1 (m_el (df_dx, f.index - 1,
                                            x.index - 1));

        v_el (v, 0) = m_el (df_dx, 0, x.index - 1);
        v_el (v, 1) = m_el (df_dx, 1, x.index - 1);
        v_el (v, 2) = m_el (df_dx, 2, x.index - 1);

        return v;

      }
    }

  if (!f.index)

    return v;

  return make_vector1 (0);

  }



  /*********************************************************************}
  {                                                                     }
  {  Given a parameter set, a token, the name set and a key return a    }
  {  pointer to the start of the structure                              }
  {                                                                     }
  {*********************************************************************/

double * token_P (parameter_set * set, double key,

                     char token [], parameter_type name []) {

  int i;

  vector * P;

  static double P3 [3];

  i = parameter_index (token, name, NULL);

  if (i == 0)

    return P3;

  P = parameter_vector (set, key);

  if (P == NULL)

    return P3;

  return &v_el (*P, 0) + abs (i) - 1;

  }


  /*********************************************************************}
  {                                                                     }
  {  Return the parameter index and a copy of the the name entry given  }
  {  the set of names and a token to be matched                         }
  {                                                                     }
  {*********************************************************************/

int parameter_index (char token [], parameter_type   name [],

                                    parameter_type * name_entry) {

  /*********************************************************************}
  {                                                                     }
  {  Parameter names consist of text to be matched (case is ignored)    }
  {                                                                     }
  {  The parameter name may include the symbol:                         }
  {                                                                     }
  {    %d : shell or entry number                                       }
  {                                                                     }
  {  The index returned is ((%d - shell0) * shell_offset + offset)      }
  {                                                                     }
  {*********************************************************************/

  int shell, n;

  char * pattern, * token0, * next;


    /* Go through the name set matching characters or %d symbols */

  for (token0 = token; name->name; name++) {

    for (token = token0, pattern = name->name, shell = 0; *token && *pattern;)

      if (pattern [0] == '%' && pattern [1] == 'd') {

        pattern += 2;

        shell = strtol (token, &next, 10);

        if (token == next)

          break;

        token = next;

        }
      else

        if (toupper (*token) == toupper (*pattern)) {

          token++;

          pattern++;

          }
        else

          break;

    if (!*token && !*pattern) {

      n = name->offset + name->shell_offset * (shell - name->shell0);

      if (name_entry) {

        *name_entry = *name;

        name_entry->offset = n;

        name_entry->shell_offset = shell;

        }

      if (n >= 0)

        return (n + 1);

      return (n - 1);

      }
    }


    /* Return 0 on failure */

  return 0;

  }


  /*********************************************************************}
  {                                                                     }
  {  Return the parameter name given the set of names and an index      }
  {                                                                     }
  {*********************************************************************/

char * parameter_name (parameter_type name [], int index) {

  static char c [32];

  int shell, ind, polar;


    /* A negative index will return the polar names */

  if (polar = index < 0)

    index = -index;

  for (; name->name; name++)

    if ((!polar && (name->type == NORMAL ||
                    name->type == ATOM_t ||

                   (name->type == VECTOR && name->index))) ||

         (polar && (name->type == PNORMAL ||

                   (name->type == PVECTOR && name->index)))) {

      if (ind = name->index)

        ind--;

      if (name->shell_offset)

        shell = (index - name->offset) / name->shell_offset;

      if (index == name->offset + shell * name->shell_offset + ind) {

        sprintf (c, name->name, shell + name->shell0);

        return c;

        }
      }


    /* return NULL on failure */

  return NULL;

  }

#endif /* __PARAMETERS_H__ */
