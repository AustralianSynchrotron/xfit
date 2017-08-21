/***********************************************************************}
{                                                                       }
{  This file contains code for the construction and evaluation of       }
{  expression trees                                                     }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__EXPRESSION_C__)

#define __EXPRESSION_C__

#include "expressi.h"

#include "console.h"


typedef struct {

  char * name;

  node_type type;

  } node_operation_name;

node_operation_name operation_name [] =

  { { "(%1 + %2)",      ADD   },
    { "(%1 - %2)",      SUB   },
    { "(%1 * %2)",      MUL   },
    { "(%1 / %2)",      DIV   },

    { "-%1",            NEG   },

    { "|%1|",           ABS   },
    { "|%1|",           VABS  },

    { "[%1, %2, %3]",   VCAST },

    { "(%1 + %2)",      VADD  },
    { "(%1 - %2)",      VSUB  },
    { "(%1 * %2)",      VMUL  },
    { "(%1 / %2)",      VDIV  },

    { "-%1",            VNEG  },

    { "(%1 x %2)",      VCRO  },
    { "(%1 . %2)",      VDOT  },

    { "%1^",            CARET },

    { "sin (%1)",       SIN   },
    { "asin (%1)",      ASIN  },
    { "cos (%1)",       COS   },
    { "acos (%1)",      ACOS  },
    { "tan (%1)",       TAN   },
    { "atan (%1)",      ATAN  },

    { "sinh (%1)",      SINH  },
    { "asinh (%1)",     ASINH },
    { "cosh (%1)",      COSH  },
    { "acosh (%1)",     ACOSH },
    { "tanh (%1)",      TANH  },
    { "atanh (%1)",     ATANH },

    { "sqrt (%1)",      SQRT  },
    { "exp (%1)",       EXP   },
    { "ln (%1)",        LN    },
    { "(10**%1)",       EXP10 },
    { "log (%1)",       LOG   },
    { "(%1**%2)",       POW   },

    { "pi",             PI_C  },
    { "random",         RAND0 },
    { "%%",             ID    },
    { "%%",             V_ID  },
    { "%%",             CONST },

    { "%1 < %2 {%3}",   LTHAN },
    { "%1 > %2 {%3}",   GTHAN },
    { "%1 ~= %2 {%3}",  AEQTO },
    { "%1 == %2",       EQUTO },

    { "undefined",      NO_OP } };


  /*********************************************************************}
  {                                                                     }
  {  Given a node and the parameter x, evaluate f and df_dx             }
  {                                                                     }
  {  Return the dimensionality of f                                     }
  {                                                                     }
  {*********************************************************************/

int operation (node * n, parameter_type x, parameter_set * parameters) {

  vector4 a, b, c, da_dx, db_dx, dc_dx, df_da;

  double d;

  int i;

  switch (n->type) {

    case PI_C :

      scal (n->f) = M_PI;

    case CONST :

      return 1;

    case RAND0 :

      scal (n->df_dx) = (scal (n->f) = frandom ()) - frandom ();

      return 1;

    }

  n->f = n->df_dx = o_vector3;

  if (n->n [0]) {

    a     = n->n [0]->f;

    da_dx = n->n [0]->df_dx;

    }

  if (n->n [1]) {

    b     = n->n [1]->f;

    db_dx = n->n [1]->df_dx;

    }

  if (n->n [2]) {

    c     = n->n [2]->f;

    dc_dx = n->n [2]->df_dx;

    }

  switch (n->type) {

    case   ID :

    case V_ID :

      n->f = parameter_value (parameters, n->parameter);

      n->df_dx = dparameter_value (parameters, n->parameter, x);

      return n->f.rows;

    case ABS :

      scal (n->f)     = scal (a);
      scal (n->df_dx) = scal (da_dx);

      if (scal (a) >= 0)

        return 1;

    case NEG :

      scal (n->f)     = -scal (a);
      scal (n->df_dx) = -scal (da_dx);

      return 1;

    case VNEG :

      n->f     = neg_vector4 (a);
      n->df_dx = neg_vector4 (da_dx);

      return 3;

    case VABS :

    case CARET :

      scal (n->f) = sqrt (d = dot_vector4 (a, a));

      df_da = div_vector4 (a, scal (n->f));

      scal (n->df_dx) = dot_vector4 (df_da, da_dx);

      if (n->type == VABS)

        return 1;

      n->df_dx = sub_vector4 (div_vector4 (da_dx, scal (n->f)),
                              mul_vector4 (a, scal (n->df_dx) / d));
      n->f = df_da;

      return 3;

    case VADD :

      n->f     = add_vector4 (a, b);
      n->df_dx = add_vector4 (da_dx, db_dx);

      return 3;

    case VSUB :

      n->f     = sub_vector4 (a, b);
      n->df_dx = sub_vector4 (da_dx, db_dx);

      return 3;

    case VMUL :

      n->f     = mul_vector4 (a, scal (b));
      n->df_dx = add_vector4 (mul_vector4 (da_dx, scal (b)),
                                 mul_vector4 (a, scal (db_dx)));

      return 3;

    case VDIV :

      n->f     = div_vector4 (a, scal (b));
      n->df_dx = sub_vector4 (div_vector4 (da_dx, scal (b)),
                              mul_vector4 (a, scal (db_dx) /
                                        (scal (b) * scal (b))));

      return 3;

    case VDOT :

      scal (n->f)     = dot_vector4 (a, b);
      scal (n->df_dx) = dot_vector4 (da_dx, b) +
                        dot_vector4 (a, db_dx);

      return 1;

    case VCRO :

      n->f     = cross_vector4 (a, b);
      n->df_dx = add_vector4 (cross_vector4 (da_dx, b),
                              cross_vector4 (a, db_dx));

      return 3;

    case ADD :

      scal (n->f)     = scal (a) + scal (b);
      scal (n->df_dx) = scal (da_dx) + scal (db_dx);

      return 1;

    case SUB :

      scal (n->f)     = scal (a) - scal (b);
      scal (n->df_dx) = scal (da_dx) - scal (db_dx);

      return 1;

    case MUL :

      scal (n->f)     = scal (a) * scal (b);
      scal (n->df_dx) = scal (da_dx) * scal (b) +
                        scal (a) * scal (db_dx);

      return 1;

    case DIV :

      scal (n->f)     = scal (a) / scal (b);
      scal (n->df_dx) = (scal (da_dx) - scal (a) * scal (db_dx)
                                    / scal (b)) / scal (b);

      return 1;

    case VCAST :

      for (i = 0; i < 3; i++) {

        if (n->n [i]) {

          v_el (n->f,     i) = scal (n->n [i]->f);
          v_el (n->df_dx, i) = scal (n->n [i]->df_dx);

          }
        }

      return 3;

    case SIN :

      scal (n->f)     = sin (scal (a));
      scal (n->df_dx) = cos (scal (a)) * scal (da_dx);

      return 1;

    case COS :

      scal (n->f)     =  cos (scal (a));
      scal (n->df_dx) = -sin (scal (a)) * scal (da_dx);

      return 1;

    case TAN :

      scal (n->f)     = tan (scal (a));
      scal (n->df_dx) = (1 + scal (n->f) * scal (n->f)) * scal (da_dx);

      return 1;

    case ASIN :

      scal (n->f)     = asin (scal (a));
      scal (n->df_dx) = scal (da_dx) / sqrt (1 - scal (a) * scal (a));

      return 1;

    case ACOS :

      scal (n->f)     =  acos (scal (a));
      scal (n->df_dx) = -scal (da_dx) / sqrt (1 - scal (a) * scal (a));

      return 1;

    case ATAN :

      scal (n->f)     = atan (scal (a));
      scal (n->df_dx) = scal (da_dx) / (1 + scal (a) * scal (a));

      return 1;

    case SINH :

      scal (n->f)     = sinh (scal (a));
      scal (n->df_dx) = cosh (scal (a)) * scal (da_dx);

      return 1;

    case COSH :

      scal (n->f)     = cosh (scal (a));
      scal (n->df_dx) = sinh (scal (a)) * scal (da_dx);

      return 1;

    case TANH :

      scal (n->f)     = tanh (scal (a));
      scal (n->df_dx) = (1 - scal (n->f) * scal (n->f)) * scal (da_dx);

      return 1;

    case ASINH :

      scal (n->f)     = asinh (scal (a));
      scal (n->df_dx) = scal (da_dx) / sqrt (scal (a) * scal (a) + 1);

      return 1;

    case ACOSH :

      scal (n->f)     = acosh (scal (a));
      scal (n->df_dx) = scal (da_dx) / sqrt (scal (a) * scal (a) - 1);

      return 1;

    case ATANH :

      scal (n->f)     = acosh (scal (a));
      scal (n->df_dx) = scal (da_dx) / (1 - scal (a) * scal (a));

      return 1;

    case SQRT :

      scal (n->f)     = sqrt (scal (a));
      scal (n->df_dx) = 0.5 / scal (n->f) * scal (da_dx);

      return 1;

    case EXP :

      scal (n->f)     = exp (scal (a));
      scal (n->df_dx) = scal (n->f) * scal (da_dx);

      return 1;

    case EXP10 :

      scal (n->f)     = pow (10.0, scal (a));
      scal (n->df_dx) = M_LN10 * scal (n->f) * scal (da_dx);

      return 1;

    case POW :

      scal (n->f)     = pow (scal (a), scal (b));
      scal (n->df_dx) = log (scal (a)) * scal (n->f) * scal (db_dx) +
                           scal (b) * pow (scal (a), scal (b) - 1)
                           * scal (da_dx);

      return 1;

    case LN :

      scal (n->f)     = log (scal (a));
      scal (n->df_dx) = scal (da_dx) / scal (a);

      return 1;

    case LOG :

      scal (n->f)     = log10 (scal (a));
      scal (n->df_dx) = M_LOG10E * scal (da_dx) / scal (a);

      return 1;

    case GTHAN :

      df_da = a;
      a     = b;
      b     = df_da;

      df_da = da_dx;
      da_dx = db_dx;
      db_dx = df_da;

    case LTHAN :

    case AEQTO :

      d = scal (a) - scal (b);

      if (n->type != AEQTO && d < 0)

        return 1;

      scal (n->f) = d / scal (c);

      scal (n->df_dx) = (scal (da_dx) - scal (db_dx) -
                         scal (dc_dx) * scal (n->f)) / scal (c);

      return 1;

    case EQUTO :

      scal (n->f)     = scal (a) - scal (b);
      scal (n->df_dx) = scal (da_dx) - scal (db_dx);

      return 1;

    }

  return 0;

  }


  /*********************************************************************}
  {                                                                     }
  {  Creating trees and nodes                                           }
  {                                                                     }
  {*********************************************************************/

node * make_node (node_type type) {

  node * n;

  n = calloc (1, sizeof (node));

  * n = null_node;

  n->type = type;

  return n;

  }

void free_node (node * n) {

  free_check (n);

  }

void free_tree (node * n) {

    /* For each node, free the branches from right to left */

  if (n == NULL) return;

  free_tree (n->n [2]);
  free_tree (n->n [1]);
  free_tree (n->n [0]);

  free_node (n);

  }

node * onod3 (node_type type, node * a, node * b, node * c) {

  node * n;

  n = make_node (type);

  n->n [0] = a;
  n->n [1] = b;
  n->n [2] = c;

  return n;

  }

node * onod2 (node_type type, node * a, node * b) {

  return onod3 (type, a, b, 0);

  }

node * onod1 (node_type type, node * a) {

  return onod3 (type, a, 0, 0);

  }

node * cnode (node_type type, double d) {

  node * n;

  n = make_node (type);

  scal (n->f) = d;

  return n;

  }

node * inode (node_type type, parameter_type id) {

  node * n;

  n = make_node (type);

  n->parameter = id;

  return n;

  }

node * node_op2 (node * n, node * a, node * b) {

  n->n [0] = a;
  n->n [1] = b;

  return n;

  }

node * node_op1 (node * n, node * a) {

  n->n [0] = a;

  return n;

  }

node * add_node1 (node ** n, node * a) {

  node * n0;

  for (n0 = (*n ? *n : a); *n; n = &((*n)->n [0]));

  *n = a;

  return n0;

  }


  /*********************************************************************}
  {                                                                     }
  {  Evaluate an expression tree                                        }
  {                                                                     }
  {  For each node, reduce the branches from left to right              }
  {                                                                     }
  {*********************************************************************/

int reduce_tree (node * n, parameter_type x, parameter_set * parameters) {

  int i;

  if (!n)

    return 0;

  for (i = 0; i < 3; i++)

    if (n->n [i])

      reduce_tree (n->n [i], x, parameters);

  return (n->f.rows = n->df_dx.rows = operation (n, x, parameters));

  }

vector4 vector_value_ (node * n, parameter_set * parameters) {

  vector4 v;

  reduce_tree (n, null_parameter, parameters);

  v = n->f;

  return v;

  }

vector4 vector_value (node * n, parameter_set * parameters) {

  vector4 v;

  v = vector_value_ (n, parameters);

  free_tree (n);

  return v;

  }

double scalar_value_ (node * n, parameter_set * parameters) {

  vector4 v;

  v = vector_value_ (n, parameters);

  return scal (v);

  }

double scalar_value (node * n, parameter_set * parameters) {

  vector4 v;

  v = vector_value (n, parameters);

  return scal (v);

  }

node ** node_list (node * n_l [], node * n) {

  int l;

  l = 0;

  if (n_l)

    for (; n_l [l]; l++);

  n_l = realloc_check (n_l, sizeof (node *) * (l + 2));

  n_l [l] = n;

  n_l [l + 1] = NULL;

  return n_l;

  }


  /*********************************************************************}
  {                                                                     }
  {  Writes the expression as a string (recursive routine)              }
  {                                                                     }
  {*********************************************************************/

char * expression_string_rec (node * n, int bind, int start) {

  int i, c, cl, cr, bind_op;

  static char * expression = NULL;

  node_operation_name operation;


    /* Get the name of this operation */

  for (i = 0; operation_name [i].type != NO_OP; i++)

    if (n)

      if (operation_name [i].type == n->type)

        break;

  operation = operation_name [i];


    /* If this is the first call allocate space for the expression */

  if (!expression) {

    expression = realloc (expression, 256);

    if (!expression)

      return NULL;

    expression [0] = 0;

    }


  if (start)

      /* Set the expression string to be empty */

    expression [0] = 0;


    /* Allocate in steps of 256 (always have 128 bytes free) */

  expression = realloc (expression,

                          ((strlen (expression) + 128) / 256 + 1) * 256);


  if (!expression)

    return NULL;


    /* For a null expression, print "0" */

  if (!n) {

    sprintf (expression + strlen (expression), "0");

    return expression;

    }

  for (i = 0; c = operation.name [i]; i++) {

    if (c == '%') {

      if ((c = operation.name [++i]) == '%')

        break;

      cl = operation.name [i - 2];
      cr = operation.name [i + 1];

      bind_op = !((cl == '(' && cr == ')') ||
                  (cl == '(' && cr == ',') ||
                  (cl == ' ' && cr == ')') ||
                  (cl == '|' && cr == '|') ||
                  (cl == '[' && cr == ',') ||
                  (cl == ' ' && cr == ',') ||
                  (cl == ' ' && cr == ']'));

      bind_op = 1;

      if (!expression_string_rec (n->n [c - '1'], bind_op, 0))

        return NULL;

      }
    else

      if (bind || operation.name [0] != '(' || (c != '(' && c != ')'))

        sprintf (expression + strlen (expression), "%c", c);

    }

  switch (n->type) {

    case CONST :

      sprintf (expression + strlen (expression), "%.7g", scal (n->f));

      break;

    case   ID :

    case V_ID :

      sprintf (expression + strlen (expression), n->parameter.name,
                                                 n->parameter.shell_offset);

      break;

    }

  return expression;

  }


  /*********************************************************************}
  {                                                                     }
  {  Writes the expression as a string                                  }
  {                                                                     }
  {*********************************************************************/

char * expression_string (node * n, int bind) {

  return expression_string_rec (n, bind, 1);

  }


  /*********************************************************************}
  {                                                                     }
  {  Prints the expression to a file                                    }
  {                                                                     }
  {*********************************************************************/

int fprintf_expression (FILE * file, node * n, int bind) {

  char * expression;


    /* Get the expression */

  expression = expression_string (n, bind);


    /* Print it */

  if (!expression)

    return 0;

  return fcout (file, "%s", expression);

  }

#endif /* __EXPRESSION_C__ */

