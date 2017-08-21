/***********************************************************************}
{                                                                       }
{  This file contains code for the construction and evaluation of       }
{  expression trees                                                     }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__EXPRESSION_H__)

#define __EXPRESSION_H__

#include "matrix.h"

#include "paramete.h"


typedef enum { NO_OP = 0,

                 /* Arithmetic */

               ADD, SUB, MUL, DIV, NEG,

                 /* Absolute value */

               ABS, VABS,

                 /* Vector cast */

               VCAST,

                 /* Vector arithmetic */

               VADD, VSUB, VMUL, VDIV, VNEG, VCRO, VDOT,

                 /* Vector normalisation */

               CARET,

                 /* Trigonometric operations */

               SIN, ASIN, COS, ACOS, TAN, ATAN,

                 /* Hyperbolic operations */

               SINH, ASINH, COSH, ACOSH, TANH, ATANH,

                 /* Logarithmic operations */

               SQRT, EXP, LN, EXP10, LOG, POW,

                 /* Miscellaneous operations */

               PI_C, RAND0, ID, V_ID, CONST,

                 /* Relational operations */

               LTHAN, GTHAN, AEQTO, EQUTO } node_type;



  /*********************************************************************}
  {                                                                     }
  {  Define the structure for a node in an expression tree              }
  {                                                                     }
  {*********************************************************************/

typedef struct node_struct {

  node_type type;

  vector4 f, df_dx;

  parameter_type parameter;

  struct node_struct * n [3];

  } node;

#define NULL_node { NO_OP, O_vector3, O_vector3, { 0 }, NULL, }

#if !defined (__EXPRESSION_C__)

extern node null_node;

#else

node null_node = NULL_node;

#endif


  /*********************************************************************}
  {                                                                     }
  {  Given a node and the parameter x, evaluate f and df_dx             }
  {                                                                     }
  {  Return the dimensionality of f                                     }
  {                                                                     }
  {*********************************************************************/

int operation (node * n, parameter_type x, parameter_set * parameters);


  /*********************************************************************}
  {                                                                     }
  {  Creating trees and nodes                                           }
  {                                                                     }
  {*********************************************************************/

node * make_node (node_type type);

void free_node (node * n);

void free_tree (node * n);

node * onod3 (node_type type, node * a, node * b, node * c);

node * onod2 (node_type type, node * a, node * b);

node * onod1 (node_type type, node * a);

node * cnode (node_type type, double d);

node * inode (node_type type, parameter_type id);

node * node_op2 (node * n, node * a, node * b);

node * node_op1 (node * n, node * a);

node * add_node1 (node ** n, node * a);


  /*********************************************************************}
  {                                                                     }
  {  Evaluate an expression tree                                        }
  {                                                                     }
  {  For each node, reduce the branches from left to right              }
  {                                                                     }
  {*********************************************************************/

int reduce_tree (node * n, parameter_type x, parameter_set * parameters);

vector4 vector_value_ (node * n, parameter_set * parameters);

vector4 vector_value (node * n, parameter_set * parameters);

double scalar_value_ (node * n, parameter_set * parameters);

double scalar_value (node * n, parameter_set * parameters);

node ** node_list (node * n_l [], node * n);


  /*********************************************************************}
  {                                                                     }
  {  Writes the expression as a string                                  }
  {                                                                     }
  {*********************************************************************/

char * expression_string (node * n, int bind);


  /*********************************************************************}
  {                                                                     }
  {  Prints the expression to a file                                    }
  {                                                                     }
  {*********************************************************************/

int fprintf_expression (FILE * file, node * n, int bind);

#endif /* __EXPRESSION_H__ */

