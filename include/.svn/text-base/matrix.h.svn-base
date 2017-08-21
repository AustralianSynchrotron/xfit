/***********************************************************************}
{                                                                       }
{  This file contains definitions of and operations on matrices and     }
{  vectors                                                              }
{                                                                       }
{  The matrices and vectors defined in this file have elements          }
{  addressed as simple one-dimensional arrays.                          }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1993.             }
{                                                                       }
{***********************************************************************}
{                                                                       }
{       matrix inversion uses code from :                               }
{                                                                       }
{       MATINV                                                          }
{                                                                       }
{       VERSION 1.0             19-JAN-82                               }
{                                                                       }
{       MATRIX INVERSION WITH SOLUTION OF LINEAR EQUATIONS :            }
{                                                                       }
{       SOLVES THE MATRIX EQUATION  [A]*[X] = [B],  WHERE [A] IS A      }
{       SQUARE COEFFICIENT MATRIX (N x N) AND [B] IS A VECTOR OF        }
{       CONSTANTS (N LONG).  THE INVERSE OF [A] IS RETURNED IN [A],     }
{       THE [X] VECTOR IS RETURNED IN [B], AND THE DETERMINANT OF       }
{       [A] IS RETURNED IN DETERM.                                      }
{                                                                       }
{***********************************************************************}
{                                                                       }
{  This file also contains routines for :                               }
{                                                                       }
{    * LU decomposition, backsubstitution, and solution improvement     }
{                                                                       }
{    * Singular Value Decomposition                                     }
{                                                                       }
{  These routines are from :                                            }
{                                                                       }
{       "Numerical Recipes"                                             }
{                                                                       }
{  William H. Press, Brian P. Flannery, Saul A. Teukolsky and           }
{  William T. Vetterling                                                }
{                                                                       }
{  Cambridge University Press, 1986                                     }
{                                                                       }
{***********************************************************************/

#if !defined (__MATRIX_H__)

#define __MATRIX_H__

#include "header.h"

#define matrix_header long rows, cols, nc, size

#define vector_header matrix_header

typedef struct {

        matrix_header;

        double *i;

        } matrix;

typedef matrix vector;

#define matrix_n(r,c) struct { matrix_header; double i [r][c]; }
#define vector_n(n)   struct { matrix_header; double i [n];    }

typedef matrix_n (4, 4) matrix4;
typedef vector_n (4)    vector4;

#define NULL_matrix  { 0, 0, 0, 0, NULL }
#define NULL_vector  { 0, 0, 0, 0, NULL }

#define O_vector3 { 3, 1, 1, 4, { 0, } }
#define O_vector4 { 4, 1, 1, 4, { 0, } }

#define E_vector3(i) { 3, 1, 1, 4, { 0 == i, 1 == i, 2 == i, 0 } }
#define E_vector4(i) { 4, 1, 1, 4, { 0 == i, 1 == i, 2 == i, 3 == i } }

#define O_matrix3 { 3, 3, 4, 16, { 0, } }
#define O_matrix4 { 4, 4, 4, 16, { 0, } }

#define I_matrix3 { 3, 3, 4, 16, { { 1, 0, 0, 0 }, { 0, 1, 0, 0 },     \
                                   { 0, 0, 1, 0 }, { 0, 0, 0, 1 } } }
#define I_matrix4 { 4, 4, 4, 16, { { 1, 0, 0, 0 }, { 0, 1, 0, 0 },     \
                                   { 0, 0, 1, 0 }, { 0, 0, 0, 1 } } }

#if defined (__MATRIX_C__)

const matrix null_matrix = NULL_matrix;
const vector null_vector = NULL_vector;

const vector4 o_vector3 = O_vector3;
const vector4 o_vector4 = O_vector4;

const vector4 e_vector3 [3] = { E_vector3 (0), E_vector3 (1), E_vector3 (2) };
const vector4 e_vector4 [4] = { E_vector4 (0), E_vector4 (1),
                                E_vector4 (2), E_vector4 (3) };

const matrix4 o_matrix3 = O_matrix3;
const matrix4 o_matrix4 = O_matrix4;

const matrix4 i_matrix3 = I_matrix3;
const matrix4 i_matrix4 = I_matrix4;

#else

extern const matrix null_matrix;
extern const vector null_vector;

extern const vector4 o_vector3;
extern const vector4 o_vector4;

extern const vector4 e_vector3 [3];
extern const vector4 e_vector4 [4];

extern const matrix4 o_matrix3;
extern const matrix4 o_matrix4;

extern const matrix4 i_matrix3;
extern const matrix4 i_matrix4;

#endif

#define m_el(m, r, c)  (((double *) (m).i) [(r) * (m).nc + (c)])
#define v_el(v, r)     (((double *) (v).i) [r])

#define m_el0(m, r, c) ((m).rows > (r) && (m).cols > (c) ? \
                                         m_el ((m), (r), (c)) : 0.0)
#define v_el0(v, r)    ((v).rows > (r) ? v_el ((v), (r)) : 0.0)

#define scal(v) v_el (v, 0)

short valid_matrix (matrix m);

short valid_vector (vector v);

short check_matrix (matrix M, char id []);

short check_vector (vector v, char id []);

short check_matrix_size (matrix M, long rows, long cols, char id []);

short free_matrix (vector * v);

short free_vector (vector * M);

short setup_vector (vector * v, long rows);

short setup_matrix (matrix * m, long rows, long cols);

short resize_vector (vector * v, long rows);

short resize_matrix (matrix * m, long rows, long cols);

short resize_vector_retain (vector * v, long rows);

double invert_matrix (matrix M, vector v);

short mul_matrix (matrix * M, matrix M1, matrix M2);

short mul_matrix_vector (vector * vd, matrix M, vector v);

double mul_vector (vector v1, vector v2);

double normalise_vector (vector v);

double abs_vector (vector v);

short clear_matrix (matrix m);

short unit_matrix (matrix m);

short clear_vector (vector v);

short copy_matrix (matrix * md, matrix m);

short copy_vector (vector * vd, vector v);

    /* Transpose the matrix m.

       If the transposed matrix has only 1 column, then put it
       into vector format.

       */

short transpose_matrix (matrix * m);

short add_matrix_row (matrix * m, vector v);

short add_matrix_col (matrix * m, vector v);

short add_matrix_0_row (matrix * m);

short add_matrix_0_col (matrix * m);

short remove_matrix_row (matrix * m, long row);

short remove_matrix_col (matrix * m, long col);

int row_reduce (matrix M, short row);

int row_reduce_matrix (matrix M, double limit,
                                 char ** reduced, int last_c);

short list_matrix (matrix m, char format []);

short list_vector (vector v, char format []);

short list_matrix_0 (matrix m, char format [], char format_0 []);

short list_vector_0 (vector v, char format [], char format_0 []);


/**********************************************************************

        Operations with the fixed-size matrices and vectors

**********************************************************************/

vector4 add_vector4 (vector4 a, vector4 b);

vector4 sub_vector4 (vector4 a, vector4 b);

vector4 neg_vector4 (vector4 v);

vector4 mul_vector4 (vector4 v, double d);

vector4 div_vector4 (vector4 v, double d);

vector4 normal_vector4 (vector4 v);

double abs_vector4 (vector4 v);

matrix4 mul_matrix4 (matrix4 m1, matrix4 m2);

vector4 mul_matrix4_vector4 (matrix4 m, vector4 v);

double dot_vector4 (vector4 a, vector4 b);

vector4 cross_vector4 (vector4 a, vector4 b);

matrix4 orientation (vector4 alpha);

  /* m = orientation matrix:

       | 1    0      0    ||  cos b  0  sin b || cos t  -sin t  0 |
       | 0  cos a  -sin a ||    0    1   0    || sin t   cos t  0 |
       | 0  sin a   cos a || -sin b  0  cos b ||   0      0     1 | */

matrix4 dorientation (long x, vector4 alpha);

  /* eg. m = d (orientation matrix) / d (beta):

        d   | 1    0      0    ||  cos b  0  sin b || cos t  -sin t  0 |
        -   | 0  cos a  -sin a ||    0    1   0    || sin t   cos t  0 |
        db  | 0  sin a   cos a || -sin b  0  cos b ||   0      0     1 |

            | 1    0      0    || -sin b  0  cos b || cos t  -sin t  0 |
        =   | 0  cos a  -sin a ||    0    1   0    || sin t   cos t  0 |
            | 0  sin a   cos a || -cos b  0 -sin b ||   0      0     1 |

        =   orientation (alpha, beta + PI / 2, tau) ... */

matrix4 rotation_matrix4 (matrix4 R, vector4 o);

vector4 pick_vector3 (vector v, long index);

vector4 pick_vector3_1 (vector v, long index);

vector4 vector3 (double d []);

vector4 vector3_1 (double d []);

vector4 make_vector4 (double a, double b, double c, double d);

vector4 make_vector3 (double a, double b, double c);

vector4 make_vector2 (double a, double b);

vector4 make_vector1 (double a);

long copy_vector4 (double d [], vector4 v);

vector4 polar_vector4 (vector4 v);

matrix4 dpolar_dvector3 (vector4 v);

vector4 vector_polar4 (vector4 v);

matrix4 dvector_dpolar3 (vector4 v);

matrix4 matrix4_matrix (matrix m);

matrix matrix_matrixN (matrix4 * m4);

#define matrix_matrix4(m) matrix_matrixN ((matrix4 *) (&(m)))


/**********************************************************************

        LU decomposition

**********************************************************************/

short LU_decompose (matrix A, long index [], double * d);

short LU_backsubstitute (matrix A, long index [], vector b);

short LU_inverse (matrix A, long index [], matrix * Y);


/**********************************************************************

        Singular Value Decomposition

**********************************************************************/

short SV_decompose (matrix A, vector * w, matrix * V);

short SV_decompose_ (matrix * A, vector * w);

#endif /* __MATRIX_H__ */
