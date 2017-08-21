/***********************************************************************}
{                                                                       }
{  This file contains the parameter types and manipulation routines     }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__PARAMETERS_H__)

#define __PARAMETERS_H__

#include "matrix.h"


  /*********************************************************************}
  {                                                                     }
  {  Define the structure used to hold the parameters                   }
  {                                                                     }
  {*********************************************************************/

typedef struct {

  vector * jrhvector [16];

  int vectors [16];

  double * key [16], current;

  } parameter_set;


  /*********************************************************************}
  {                                                                     }
  {  Functions to manipulate parameter sets                             }
  {                                                                     }
  {*********************************************************************/

void clear_parameter_set (parameter_set * set);

  /*********************************************************************}
  {                                                                     }
  {  Create a new parameter vector in the set O (0 .. 15)               }
  {                                                                     }
  {  Return the key                                                     }
  {                                                                     }
  {*********************************************************************/

double create_parameter_vector (parameter_set * set, int O);


  /*********************************************************************}
  {                                                                     }
  {  Delete parameter vector i in the set O (0 .. 15)                   }
  {                                                                     }
  {  Return 0 if it doesn't exist                                       }
  {                                                                     }
  {*********************************************************************/

int delete_parameter_vector_Oi (parameter_set * set, int O, int i);


  /*********************************************************************}
  {                                                                     }
  {  Delete the parameter vector with the matching key                  }
  {                                                                     }
  {  Return 0 if it doesn't exist                                       }
  {                                                                     }
  {*********************************************************************/

int delete_parameter_vector (parameter_set * set, double key);


  /*********************************************************************}
  {                                                                     }
  {  Return a pointer to vector i in set O                              }
  {                                                                     }
  {*********************************************************************/

vector * parameter_vector_Oi (parameter_set * set, int O, int i);


  /*********************************************************************}
  {                                                                     }
  {  Return the key to vector i in set O                                }
  {                                                                     }
  {*********************************************************************/

double parameter_vector_key_Oi (parameter_set * set, int O, int i);

  /*********************************************************************}
  {                                                                     }
  {  Given the key, return a pointer to the parameter vector            }
  {                                                                     }
  {*********************************************************************/

vector * parameter_vector (parameter_set * set, double key);


  /*********************************************************************}
  {                                                                     }
  {  Define the structure used to hold descriptions of the parameters   }
  {                                                                     }
  {*********************************************************************/

typedef enum { ATOM_t, NORMAL, VECTOR, PNORMAL, PVECTOR } parameter_t;

typedef struct {

  char         * name;

  short          offset,        /* Offset of the first instance */

                 shell0,        /* Lowest index */

                 shell_offset,  /* Shell size */

                 index;         /* Offset within a structure */

  int            vector;        /* The set of vectors this parameter is in */

  double         key;           /* The key into the correct vector */

  int            command;       /* The parser token for this parameter */

  parameter_t    type;          /* The type of the parameter */

  short          lex_state,

                 refine;        /* <> 0 : parameter can be refined */

  } parameter_type;

#if !defined (__PARAMETERS_C__)

extern const parameter_type null_parameter;

#else

const parameter_type null_parameter = { 0 };

#endif


  /*********************************************************************}
  {                                                                     }
  {  Resize a vector while retaining the current contents               }
  {                                                                     }
  {*********************************************************************/

int resize_parameter_vector (vector * parameters, int rows);


  /*********************************************************************}
  {                                                                     }
  {  Set the value of a parameter to be equal to a short vector value   }
  {                                                                     }
  {  The parameter is assumed to exist                                  }
  {                                                                     }
  {*********************************************************************/

int set_parameter (parameter_set * set,

                    parameter_type parameter, vector4 value);


  /*********************************************************************}
  {                                                                     }
  {  Set the value of a parameter to be equal to a short vector value   }
  {                                                                     }
  {  If the parameter does not exist, it is created                     }
  {                                                                     }
  {  If the parameter vector is MODEL, vector MODEL0 is also set        }
  {                                                                     }
  {*********************************************************************/

int assign_parameter (parameter_set * set,

                       parameter_type parameter, vector4 value);


  /*********************************************************************}
  {                                                                     }
  {  Get the value of a parameter                                       }
  {                                                                     }
  {  The parameter is assumed to exist                                  }
  {                                                                     }
  {*********************************************************************/

vector4 parameter_value (parameter_set * set, parameter_type parameter);


  /*********************************************************************}
  {                                                                     }
  {  Get the derivative of a parameter with respect to another          }
  {                                                                     }
  {*********************************************************************/

vector4 dparameter_value (parameter_set * set,

                          parameter_type f, parameter_type x);


  /*********************************************************************}
  {                                                                     }
  {  Return the parameter index and a copy of the the name entry given  }
  {  the set of names and a token to be matched                         }
  {                                                                     }
  {*********************************************************************/

int parameter_index (char token [], parameter_type   name [],

                                    parameter_type * name_entry);


  /*********************************************************************}
  {                                                                     }
  {  Return the parameter name given the set of names and an index      }
  {                                                                     }
  {*********************************************************************/

char * parameter_name (parameter_type name [], int index);

#endif /* __PARAMETERS_H__ */
