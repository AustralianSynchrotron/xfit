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

#if !defined (__MATRIX_C__)

#define __MATRIX_C__

#include "matrix.h"

#include "console.h"


short valid_matrix (matrix m) {

  return ((m.rows >= 0 && m.cols >= 0) &&
           m.nc >= m.cols && m.size >= (m.rows * m.nc) &&
           ((m.i == NULL && m.size == 0) || (m.i != NULL && m.size > 0)));

  }

short valid_vector (vector v) {

  if (v.cols > 1) return (FALSE);

  return (valid_matrix (v));

  }


short check_matrix (matrix M, char id []) {

  if (!valid_matrix (M)) {

    cwarn ("%s :\n\
         matrix : %1d x %1d, size = %1d, nc = %1d, i = %#.8lx",
             id, M.rows, M.cols, M.size, M.nc, *((long *) &M.i));

    return (FALSE);

    }

  return (TRUE);

  }

short check_vector (vector v, char id []) {

  if (!valid_vector (v)) {

    cwarn ("%s :\n\
         vector : %1d x %1d, size = %1d, nc = %1d, i = %#.8lx",
             id, v.rows, v.cols, v.size, v.nc, *((long *) &v.i));

    return (FALSE);

    }

  return (TRUE);

  }

short check_matrix_size (matrix M, long rows, long cols, char id []) {

  if (!check_matrix (M, id)) return (FALSE);

  if (M.rows != rows || M.cols != cols) {

    cwarn ("%s : invalid size\n\
         matrix : %1d x %1d - require : %1d x %1d",
             id, M.rows, M.cols, rows, cols);

    return (FALSE);

    }

  return (TRUE);

  }

short free_matrix (vector * v) {

  if (valid_matrix (*v)) free_check (v->i);

  *v = null_matrix;

  return (TRUE);

  }

short free_vector (vector * M) {

  return (free_matrix (M));

  }

short setup_vector (vector * v, long rows) {

  free_vector (v);

  if (rows == 0) return (TRUE);

  v->i = calloc (rows, sizeof (double));

  if (v->i == NULL) {

    cwarn ("setup_vector :\n\
         cannot allocate memory for %1d elements\
 of %1d bytes", rows, sizeof (double));

    return (FALSE);

    }

  v->size = v->rows = rows;
  v->cols = v->nc = 1;

  return (TRUE);

  }

short setup_matrix (matrix * m, long rows, long cols) {

  if (setup_vector (m, rows * cols)) {

    m->rows = rows;
    m->cols = m->nc = cols;

    return (TRUE);

    }

  return (FALSE);

  }

short resize_vector (vector * v, long rows) {

  if (!check_matrix (*v, "resize_vector")) return (FALSE);

  if (rows <= v->size && rows > 0) {

    v->rows = rows;
    v->cols = v->nc = 1;

    return (TRUE);

    }

  return (setup_vector (v, rows));

  }

short resize_matrix (matrix * m, long rows, long cols) {

  if (!check_matrix (*m, "resize_matrix")) return (FALSE);

  if (m->size == 0) return (setup_matrix (m, rows, cols));

  if (cols <= m->nc && rows * m->nc <= m->size) {

    m->rows = rows;
    m->cols = cols;

    return (TRUE);

    }

  if (rows * cols <= m->size) {

    m->rows = rows;
    m->cols = cols;

    m->nc = (sqrt ((cols - rows) * (cols - rows) + 4 * m->size)
                        + (cols - rows)) / 2;

    return (TRUE);

    }

  return (setup_matrix (m, rows, cols));

  }

short resize_vector_retain (vector * v, long rows) {

  vector v2;

  long i;

  if (!check_vector (*v, "resize_vector_retain")) return (FALSE);

  if (v->nc >= 1)
    if (rows <= (v->size / v->nc)) {

      for (i = v->rows; i < rows; i++)
        m_el (*v, i, 0) = 0;

      v->rows = rows;
      v->cols = 1;

      return (TRUE);

      }

  v2 = null_vector;

  if (!setup_vector (&v2, rows)) return (FALSE);

  for (i = 0; i < v->rows; i++)
    v_el (v2, i) = v_el (*v, i);

  for (i = v->rows; i < rows; i++)
    v_el (v2, i) = 0;

  free_vector (v);

  *v = v2;

  return (TRUE);

  }

double invert_matrix (matrix M, vector v) {

  long   i, j, k, l, m, row, col;
  long   N, *row_index, *col_index, *pivot_i;
  double *pivot, Mmax, t;

  double determ;

  if (!(check_matrix (M, "invert_matrix") &&
        check_vector (v, "invert_matrix")))
    return (0.0);

  if ((M.rows != M.cols) || (M.rows != v.rows)) {

    cwarn ("invert_matrix : inconsistent dimensions :\n\
         matrix : %1d x %1d - vector %1d x 1",
                     M.rows, M.cols, v.rows);

    return (0.0);

    }

    /* INITIALISATION */

  N = M.rows;

  row_index = calloc (N, sizeof (long));
  col_index = calloc (N, sizeof (long));
  pivot_i   = calloc (N, sizeof (long));
  pivot     = calloc (N, sizeof (double));

  if (row_index == NULL || pivot_i == NULL ||
      col_index == NULL || pivot   == NULL) {

    cwarn ("invert_matrix : unable to allocate temporary storage :\n\
         row_index : %1d x %1d bytes\n\
         col_index : %1d x %1d bytes\n\
         pivot_i   : %1d x %1d bytes\n\
         pivot     : %1d x %1d bytes",
          N, sizeof (long), N, sizeof (long), N, sizeof (long),
          N, sizeof (double));

    free_check (pivot);
    free_check (pivot_i);
    free_check (col_index);
    free_check (row_index);

    return (0.0);

    }

  determ = 1.0;

  for (i = 0; i < N; i++) pivot_i [i] = 0;

  for (i = 0; i < N; i++) {

      /* SEARCH FOR THE PIVOT ELEMENT */

    Mmax = 0.0;

    for (j = 0; j < N; j++) {

      if (pivot_i [j] != 1)

        for (k = 0; k < N; k++) {

          if (pivot_i [k] > 1) {

            free_check (pivot);
            free_check (pivot_i);
            free_check (col_index);
            free_check (row_index);

            return (0.0);

            }

          if (pivot_i [k] < 1)
            if (fabs (Mmax) < fabs (m_el (M, j, k))) {

              row = j;
              col = k;
              Mmax = m_el (M, j, k);

              }
          }
      }

    if (Mmax == 0.0) {

      free_check (pivot);
      free_check (pivot_i);
      free_check (col_index);
      free_check (row_index);

      return (0.0);

      }

    pivot_i [col]++;

      /* INTERCHANGE ROWS TO PUT PIVOT ELEMENT ON DIAGONAL */

    if (row != col) {

      determ = -determ;

      for (l = 0; l < N; l++)
        swap_double (m_el (M, row, l), m_el (M, col, l));

      swap_double (v_el (v, row), v_el (v, col));

      }

    row_index [i] = row;
    col_index [i] = col;

    pivot [i] = m_el (M, col, col);

    if (determ * pivot [i] == 0)
      determ = ((determ < 0) ^ (pivot [i] < 0) ? -MINDOUBLE : MINDOUBLE);
    else
      determ *= pivot [i];

      /* DIVIDE PIVOT ROW BY PIVOT ELEMENT */

    m_el (M, col, col) = 1.0;

    for (l = 0; l < N; l++)
      m_el (M, col, l) /= pivot [i];

    v_el (v, col) /= pivot [i];

      /* REDUCE NON-PIVOT ROWS */

    for (m = 0; m < N; m++)
      if (m != col) {

        t = m_el (M, m, col);

        m_el (M, m, col) = 0.0;

        for (l = 0; l < N; l++)
          m_el (M, m, l) -= m_el (M, col, l) * t;

        v_el (v, m) -= v_el (v, col) * t;

        }
    }

    /* INTERCHANGE COLUMNS */

  for (i = 0; i < N; i++) {

    l = N - i - 1;

    if (row_index [l] != col_index [l]) {

      row = row_index [l];
      col = col_index [l];

      for (k = 0; k < N; k++)
        swap_double (m_el (M, k, row), m_el (M, k, col));

      }
    }

  free_check (pivot);
  free_check (pivot_i);
  free_check (col_index);
  free_check (row_index);

  return (determ);

  }

short mul_matrix (matrix * M, matrix M1, matrix M2) {

  long i, j, k;

  matrix m;

  if (!check_matrix (M1, "mul_matrix") ||
      !check_matrix (M2, "mul_matrix")) return (FALSE);

  if (M1.cols != M2.rows) {

    cwarn ("mul_matrix : inconsistent dimensions :\n\
         matrix 1 : %1d x %1d - matrix 2 : %1d x %1d",
                     M1.rows, M1.cols, M2.rows, M2.cols);

    return (FALSE);

    }

  m.size = 0;

  if (!setup_matrix (&m, M1.rows, M2.cols)) {

    cwarn ("mul_matrix : unable to allocate space for\
 temporary matrix : %1d x %1d",
                     M1.rows, M2.cols);

    return (FALSE);

    };

  for (i = 0; i < M1.rows; i++)
    for (j = 0; j < M2.cols; j++) {

      m_el (m, i, j) = 0.0;

      for (k = 0; k < M1.cols; k++)
        m_el (m, i, j) += m_el (M1, i, k) * m_el (M2, k, j);

      }

  resize_matrix (M, M1.rows, M2.cols);

  for (i = 0; i < M1.rows; i++)
    for (j = 0; j < M2.cols; j++)
      m_el (*M, i, j) = m_el (m, i, j);

  free_matrix (&m);

  return (TRUE);

  }

short mul_matrix_vector (vector * vd, matrix M, vector v) {

  return (mul_matrix (vd, M, v));

  }

double mul_vector (vector v1, vector v2) {

  long i;

  double a;

  if (!check_vector (v1, "mul_vector") ||
      !check_vector (v2, "mul_vector")) return (0.0);

  if (v1.rows != v2.rows) {

    cwarn ("mul_vector : inconsistent dimensions :\n\
         vector 1 : %1d x %1d - vector 2 : %1d x %1d",
                     v1.rows, v1.cols, v2.rows, v2.cols);

    return (0.0);

    }

  a = 0.0;

  for (i = 0; i < v1.rows; i++)
    a += v_el (v1, i) * v_el (v2, i);

  return (a);

  }

double normalise_vector (vector v) {

  long i;

  double d;

  for (d = i = 0; i < v.rows; i++)
    d += v_el (v, i) * v_el (v, i);

  if (d == 0) return (0);

  d = sqrt (d);

  for (i = 0; i < v.rows; i++)
    v_el (v, i) /= d;

  return (d);

  }

double abs_vector (vector v) {

  long i;

  double d;

  for (d = i = 0; i < v.rows; i++)
    d += v_el (v, i) * v_el (v, i);

  return (sqrt (d));

  }

short clear_matrix (matrix m) {

  long i, j;

  if (!check_matrix (m, "clear_matrix")) return (FALSE);

  for (i = 0; i < m.rows; i++)
    for (j = 0; j < m.cols; j++)
      m_el (m, i, j) = 0;

  return (TRUE);

  }

short unit_matrix (matrix m) {

  long i;

  if (!clear_matrix (m)) return (FALSE);

  for (i = 0; i < m.rows && i < m.cols; i++)
    m_el (m, i, i) = 1;

  return (TRUE);

  }

short clear_vector (vector v) {

  long i;

  if (!check_vector (v, "clear_vector")) return (FALSE);

  for (i = 0; i < v.rows; i++)
    v_el (v, i) = 0;

  return (TRUE);

  }

short copy_matrix (matrix * md, matrix m) {

  long i, j;

  if (!check_matrix (m, "copy_matrix")) return (FALSE);

  if (!resize_matrix (md, m.rows, m.cols)) return (FALSE);

  for (i = 0; i < m.rows; i++)
    for (j = 0; j < m.cols; j++)
      m_el (*md, i, j) = m_el (m, i, j);

  return (TRUE);

  }

short copy_vector (vector * vd, vector v) {

  long i;

  if (!check_vector (v, "copy_vector")) return (FALSE);

  if (!resize_vector (vd, v.rows)) return (FALSE);

  for (i = 0; i < v.rows; i++)
    v_el (*vd, i) = v_el (v, i);

  return (TRUE);

  }

short transpose_matrix (matrix * m) {

    /* Transpose the matrix m.

       If the transposed matrix has only 1 column, then put it
       into vector format.

       */

  long i, j;

  matrix m_;

  m_ = null_matrix;

  if (!check_matrix (*m, "transpose_matrix")) return (FALSE);

  if (m->rows == 1) {

    m->rows = m->cols;
    m->cols = 1;
    m->nc = 1;

    return (TRUE);

    }

  if (m->rows == m->cols) {

    for (i = 1; i < m->rows; i++)

      for (j = 0; j < i; j++)

        swap (m_el (*m, i, j), m_el (*m, j, i));

    return (TRUE);

    }

  if (!copy_matrix (&m_, *m)) return (FALSE);

  resize_matrix (m, m_.cols, m_.rows);

  for (i = 0; i < m_.rows; i++)
    for (j = 0; j < m_.cols; j++)
      m_el (*m, j, i) = m_el (m_, i, j);

  free_matrix (&m_);

  return (TRUE);

  }

short add_matrix_row (matrix * m, vector v) {

  long i, j;

  matrix m_;

  if (!check_matrix (*m, "add_matrix_row") ||
      !check_vector ( v, "add_matrix_row")) return (FALSE);

  if (m->cols == 0 && m->rows == 0) {

    if (!resize_matrix (m, 1, v.rows)) return (FALSE);

    for (i = 0; i < v.rows; i++)
      m_el (*m, 0, i) = v_el (v, i);

    return (TRUE);

    }

  if (m->cols != v.rows) {

    cwarn ("add_matrix_row : inconsistent dimensions :\n\
         matrix : %1d x %1d - vector* : %1d x %1d",
                     m->rows, m->cols, v.cols, v.rows);

    return (FALSE);

    }

  m_ = null_matrix;

  if (!copy_matrix (&m_, *m)) return (FALSE);

  if (!resize_matrix (m, m->rows + 1, m->cols)) {

    free_matrix (&m_);

    return (FALSE);

    };

  for (i = 0; i < m_.rows; i++)
    for (j = 0; j < m_.cols; j++)
      m_el (*m, i, j) = m_el (m_, i, j);

  for (j = 0; j < m_.cols; j++)
    m_el (*m, i, j) = v_el (v, j);

  free_matrix (&m_);

  return (TRUE);

  }

short add_matrix_col (matrix * m, vector v) {

  long i, j;

  matrix m_;

  if (!check_matrix (*m, "add_matrix_col") ||
      !check_vector ( v, "add_matrix_col")) return (FALSE);

  if (m->cols == 0 && m->rows == 0)
    return (copy_matrix (m, v));

  if (m->rows != v.rows) {

    cwarn ("add_matrix_col : inconsistent dimensions :\n\
         matrix : %1d x %1d - vector : %1d x %1d",
                     m->rows, m->cols, v.rows, v.cols);

    return (FALSE);

    }

  m_ = null_matrix;

  if (!copy_matrix (&m_, *m)) return (FALSE);

  if (!resize_matrix (m, m->rows, m->cols + 1)) {

    free_matrix (&m_);

    return (FALSE);

    };

  for (i = 0; i < m_.rows; i++)
    for (j = 0; j < m_.cols; j++)
      m_el (*m, i, j) = m_el (m_, i, j);

  for (i = 0; i < m_.rows; i++)
    m_el (*m, i, j) = v_el (v, i);

  free_matrix (&m_);

  return (TRUE);

  }

short add_matrix_0_row (matrix * m) {

  long ok;

  vector v;

  if (!check_matrix (*m, "add_matrix_0_row")) return (FALSE);

  v = null_vector;

  setup_vector (&v, m->cols);

  clear_vector (v);

  ok = add_matrix_row (m, v);

  free_vector (&v);

  return (ok);

  }

short add_matrix_0_col (matrix * m) {

  long ok;

  vector v;

  if (!check_matrix (*m, "add_matrix_0_col")) return (FALSE);

  v = null_vector;

  setup_vector (&v, m->rows);

  clear_vector (v);

  ok = add_matrix_col (m, v);

  free_vector (&v);

  return (ok);

  }

short remove_matrix_row (matrix * m, long row) {

  long col;

  if (!check_matrix (*m, "remove_matrix_row")) return (FALSE);

  if (row >= m->rows || row < 0) {

    cwarn ("remove_matrix_row : inconsistent dimensions :\n\
         matrix : %1d x %1d - remove row : %1d",
                     m->rows, m->cols, row + 1);

    return (FALSE);

    }

  if (--m->rows == 0) *m = null_matrix;

  for (; row < m->rows; row++)
    for (col = 0; col < m->cols; col++)
      m_el (*m, row, col) = m_el (*m, row + 1, col);

  return (TRUE);

  }

short remove_matrix_col (matrix * m, long col) {

  long row;

  if (!check_matrix (*m, "remove_matrix_col")) return (FALSE);

  if (col >= m->cols || col < 0) {

    cwarn ("remove_matrix_col : inconsistent dimensions :\n\
         matrix : %1d x %1d - remove col : %1d",
                     m->rows, m->cols, col + 1);

    return (FALSE);

    }

  if (--m->cols == 0) *m = null_matrix;

  for (; col < m->cols; col++)
    for (row = 0; row < m->rows; row++)
      m_el (*m, row, col) = m_el (*m, row, col + 1);

  return (TRUE);

  }

int column_to_reduce (matrix M, int row, int last_c) {

  double Mmax;

  int i, col;

    /* find the column with the entry of maximum absolute value. */

  for (i = 0, Mmax = 0, col = -1; i < (M.cols - !last_c); i++) {

    if (Mmax < fabs (m_el (M, row, i)))

      Mmax = fabs (m_el (M, row, col = i));

    }

  return col;

  }

int row_reduce_limit (matrix M, int row, double limit, int last_c) {

    /* Reduces the matrix M with the row passed, returns the column reduced. */

  double Mmax;

  int i, j, col;

  col = column_to_reduce (M, row, last_c);

  if (col < 0) return -1;

  Mmax = m_el (M, row, col);

    /* If the element is too small, set the row to 0 */

  if (fabs (Mmax) < limit) {

    for (col = 0; col < M.cols; col++)

      m_el (M, row, col) = 0;

    return -1;

    }

    /* Divide the row by the selected entry. */

  for (i = 0; i < M.cols; i++)

    m_el (M, row, i) /= Mmax;

    /* Now do the reduction. */

  for (i = 0; i < M.rows; i++)

    if (i != row) {

      Mmax = m_el (M, i, col);

      for (j = 0; j < M.cols; j++)

        m_el (M, i, j) -= Mmax * m_el (M, row, j);

      }

  return col;

  }

int row_reduce (matrix M, short row) {

  return row_reduce_limit (M, row, 0, 1);

  }

int row_reduce_matrix (matrix M, double limit,
                                 char ** reduced, int last_c) {

  static char *reduced_l = NULL;

  int row, col;

  vector v;

  if (reduced) {

    reduced_l = realloc_check (reduced_l, M.cols * sizeof (char));

    for (col = 0; col < M.cols; col++)

      reduced_l [col] = 0;

    *reduced = reduced_l;

    }


    /* Normalise all the rows */

  v.size = M.cols;
  v.nc   = 1;
  v.rows = M.cols;
  v.cols = 1;

  for (row = 0; row < M.rows; row++) {

    v.i = &m_el (M, row, 0);

    normalise_vector (v);

    }


  limit = fabs (limit);

    /* Set to 0 any entries < limit */

  /* for (row = 0; row < M.rows; row++)

    for (col = 0; col < (M.cols - !last_c); col++)

      if (limit > fabs (m_el (M, row, col)))

        m_el (M, row, col) = 0; */


    /* Row-reduce the matrix */

  for (row = 0; row < M.rows; row++) {

    col = row_reduce_limit (M, row, limit, last_c);

    if (col >= 0 && reduced)

      reduced_l [col] = 1;

    }


    /* If the last column was omitted, set to 0 any rows with
       all 0 save for the last element if this is < limit */

  for (row = 0; row < M.rows && !last_c; row++)

    for (col = 0; col < M.cols; col++)

      if (m_el (M, row, col) != 0) {

        if (col == M.cols - 1)

          if (fabs (m_el (M, row, col)) < limit)

            m_el (M, row, col) = 0;

        col = M.cols;

        }


  return 1;

  }

short list_matrix (matrix m, char format []) {

  long i, j;

  for (i = 0; i < m.rows; i++) {

    cout (" | ");

    for (j = 0; j < m.cols; j++)
      cout (format, m_el (m, i, j));

    cout (" |\n");

    }

  return (TRUE);

  }

short list_vector (vector v, char format []) {

  long i;

  cout (" [ ");

  for (i = 0; i < v.rows; i++)
    cout (format, v_el (v, i));

  cout (" ]\n");

  return (TRUE);

  }

short list_matrix_0 (matrix m, char format [], char format_0 []) {

  long i, j;

  for (i = 0; i < m.rows; i++) {

    cout (" | ");

    for (j = 0; j < m.cols; j++)
      if (m_el (m, i, j) != 0.0)
        cout (format, m_el (m, i, j));
      else
        cout (format_0);

    cout (" |\n");

    }

  return (TRUE);

  }

short list_vector_0 (vector v, char format [], char format_0 []) {

  long i;

  cout (" [ ");

  for (i = 0; i < v.rows; i++)
    if (v_el (v, i) != 0.0)
      cout (format, v_el (v, i));
    else
      cout (format_0);

  cout (" ]\n");

  return (TRUE);

  }


/**********************************************************************

        Operations with the fixed-size matrices and vectors

**********************************************************************/

vector4 add_vector4 (vector4 a, vector4 b) {

  long i;

  for (i = 0; i < a.rows; i++)
    v_el (a, i) += v_el (b, i);

  return (a);

  }

vector4 sub_vector4 (vector4 a, vector4 b) {

  long i;

  for (i = 0; i < a.rows; i++)
    v_el (a, i) -= v_el (b, i);

  return (a);

  }

vector4 neg_vector4 (vector4 v) {

  long i;

  for (i = 0; i < v.rows; i++)
    v_el (v, i) = -v_el (v, i);

  return (v);

  }

vector4 mul_vector4 (vector4 v, double d) {

  long i;

  for (i = 0; i < v.rows; i++)
    v_el (v, i) *= d;

  return (v);

  }

vector4 div_vector4 (vector4 v, double d) {

  long i;

  for (i = 0; i < v.rows; i++)
    v_el (v, i) /= d;

  return (v);

  }

vector4 normal_vector4 (vector4 v) {

  long i;

  double d;

  for (d = i = 0; i < v.rows; i++)
    d += v_el (v, i) * v_el (v, i);

  if (d == 0) return (v);

  d = sqrt (d);

  for (i = 0; i < v.rows; i++)
    v_el (v, i) /= d;

  return (v);

  }

double abs_vector4 (vector4 v) {

  long i;

  double d;

  for (d = i = 0; i < v.rows; i++)
    d += v_el (v, i) * v_el (v, i);

  return (sqrt (d));

  }

matrix4 mul_matrix4 (matrix4 m1, matrix4 m2) {

  long i, j, k;

  matrix4 m;

  m = o_matrix4;

  m.rows = m1.rows;
  m.cols = m2.cols;

  for (i = 0; i < m1.rows; i++)
    for (j = 0; j < m2.cols; j++)
      for (k = 0; k < m1.cols; k++)
        m_el (m, i, j) += m_el (m1, i, k) * m_el (m2, k, j);

  return (m);

  }

vector4 mul_matrix4_vector4 (matrix4 m, vector4 v) {

  long i, j;

  vector4 a;

  a = o_vector4;
  a.rows = m.rows;

  for (i = 0; i < m.rows; i++)
    for (j = 0; j < m.cols; j++)
      v_el (a, i) += m_el (m, i, j) * v_el (v, j);

  return (a);

  }

double dot_vector4 (vector4 a, vector4 b) {

  double d; long i;

  for (d = i = 0; i < a.rows; i++)
    d += v_el (a, i) * v_el (b, i);

  return (d);

  }

vector4 cross_vector4 (vector4 a, vector4 b) {

  vector4 v;

  v = o_vector3;

  v_el (v, 0) = v_el (a, 1) * v_el (b, 2) - v_el (a, 2) * v_el (b, 1);
  v_el (v, 1) = v_el (a, 2) * v_el (b, 0) - v_el (a, 0) * v_el (b, 2);
  v_el (v, 2) = v_el (a, 0) * v_el (b, 1) - v_el (a, 1) * v_el (b, 0);

  return (v);

  }

matrix4 orientation (vector4 alpha) {

  /* m = orientation matrix:

       | 1    0      0    ||  cos b  0  sin b || cos t  -sin t  0 |
       | 0  cos a  -sin a ||    0    1   0    || sin t   cos t  0 |
       | 0  sin a   cos a || -sin b  0  cos b ||   0      0     1 | */

  matrix4 a, m;

  m = i_matrix3;

  m_el (m, 1, 1) =   m_el (m, 2, 2) = cos (v_el (alpha, 0));
  m_el (m, 1, 2) = -(m_el (m, 2, 1) = sin (v_el (alpha, 0)));

  a = i_matrix3;

  m_el (a, 0, 0) =   m_el (a, 2, 2) = cos (v_el (alpha, 1));
  m_el (a, 2, 0) = -(m_el (a, 0, 2) = sin (v_el (alpha, 1)));

  m = mul_matrix4 (m, a);

  a = i_matrix3;

  m_el (a, 0, 0) =   m_el (a, 1, 1) = cos (v_el (alpha, 2));
  m_el (a, 0, 1) = -(m_el (a, 1, 0) = sin (v_el (alpha, 2)));

  m = mul_matrix4 (m, a);

  return (m);

  }

matrix4 dorientation (long x, vector4 alpha) {

  /* eg. m = d (orientation matrix) / d (beta):

        d   | 1    0      0    ||  cos b  0  sin b || cos t  -sin t  0 |
        -   | 0  cos a  -sin a ||    0    1   0    || sin t   cos t  0 |
        db  | 0  sin a   cos a || -sin b  0  cos b ||   0      0     1 |

            | 1    0      0    || -sin b  0  cos b || cos t  -sin t  0 |
        =   | 0  cos a  -sin a ||    0    1   0    || sin t   cos t  0 |
            | 0  sin a   cos a || -cos b  0 -sin b ||   0      0     1 |

        =   orientation (alpha, beta + PI / 2, tau) ... */

  v_el (alpha, x % 3) += M_PI_2;

  return (orientation (alpha));

  }

matrix4 rotation_matrix4 (matrix4 R, vector4 o) {

  long n;

  o = sub_vector4 (o, mul_matrix4_vector4 (R, o));

  for (n = 0; n < 3; n++) {

    m_el (R, n, 3) = v_el (o, n);
    m_el (R, 3, n) = 0;

    }

  m_el (R, 3, 3) = 1;

  R.rows = R.cols = 1;

  return (R);

  }

vector4 pick_vector3 (vector v, long index) {

  static vector4 v4 = O_vector3;

  v_el (v4, 0) = v_el0 (v, index);
  v_el (v4, 1) = v_el0 (v, index + 1);
  v_el (v4, 2) = v_el0 (v, index + 2);

  return (v4);

  }

vector4 pick_vector3_1 (vector v, long index) {

  static vector4 v4 = E_vector4 (3);

  v_el (v4, 0) = v_el0 (v, index);
  v_el (v4, 1) = v_el0 (v, index + 1);
  v_el (v4, 2) = v_el0 (v, index + 2);

  return (v4);

  }

vector4 vector3 (double d []) {

  static vector4 v = O_vector3;

  if (!d) return (v);

  v_el (v, 0) = d [0];
  v_el (v, 1) = d [1];
  v_el (v, 2) = d [2];

  return (v);

  }

vector4 vector3_1 (double d []) {

  static vector4 v = E_vector4 (3);

  if (!d) return (v);

  v_el (v, 0) = d [0];
  v_el (v, 1) = d [1];
  v_el (v, 2) = d [2];

  return (v);

  }

vector4 make_vector4 (double a, double b, double c, double d) {

  static vector4 v = O_vector4;

  v_el (v, 0) = a;
  v_el (v, 1) = b;
  v_el (v, 2) = c;
  v_el (v, 3) = d;

  return (v);


  }

vector4 make_vector3 (double a, double b, double c) {

  vector4 v;

  v = make_vector4 (a, b, c, 0);

  v.rows = 3;

  return (v);


  }

vector4 make_vector2 (double a, double b) {

  vector4 v;

  v = make_vector4 (a, b, 0, 0);

  v.rows = 2;

  return (v);


  }

vector4 make_vector1 (double a) {

  vector4 v;

  v = make_vector4 (a, 0, 0, 0);

  v.rows = 1;

  return (v);


  }

long copy_vector4 (double d [], vector4 v) {

  long i;

  for (i = 0; i < v.rows; i++)
    d [i] = v_el (v, i);

  return 1;

  }

vector4 polar_vector4 (vector4 v) {

  double r, theta, phi;

  r = abs_vector4 (v);

  if (r == 0) return (v);

  switch (v.rows) {

    case 0 :
    case 1 : return (v);

    case 2 :

      v_el (v, 1) = atan2 (v_el (v, 1), v_el (v, 0));
      v_el (v, 0) = r;

      return (v);

    case 3 :

      phi   = atan2 (v_el (v, 1), v_el (v, 0));
      theta = acos (v_el (v, 2) / r);

      v_el (v, 0) = r;
      v_el (v, 1) = theta;
      v_el (v, 2) = phi;

      return (v);

    }

  return o_vector3;

  }

matrix4 dpolar_dvector3 (vector4 v) {

  matrix4 dp_dv;

  double r, d;

  d = v_el (v, 0) * v_el (v, 0) + v_el (v, 1) * v_el (v, 1);

  r = sqrt (d + v_el (v, 2) * v_el (v, 2));

  if (r == 0) return (o_matrix3);

  dp_dv = o_matrix3;

  m_el (dp_dv, 0, 0) = v_el (v, 0) / r;
  m_el (dp_dv, 0, 1) = v_el (v, 1) / r;
  m_el (dp_dv, 0, 2) = v_el (v, 2) / r;

  if (d == 0) {

    m_el (dp_dv, 1, 0) = m_el (dp_dv, 1, 1) = 1 / r;
    m_el (dp_dv, 1, 2) = 0;

    m_el (dp_dv, 2, 0) = -1000000;
    m_el (dp_dv, 2, 1) =  1000000;

    }
  else {

    m_el (dp_dv, 1, 0) = v_el (v, 0) * v_el (v, 2) / (sqrt (d) * r * r);
    m_el (dp_dv, 1, 1) = v_el (v, 1) * v_el (v, 2) / (sqrt (d) * r * r);
    m_el (dp_dv, 1, 2) = -sqrt (d) / (r * r);

    m_el (dp_dv, 2, 0) = -v_el (v, 1) / d;
    m_el (dp_dv, 2, 1) =  v_el (v, 0) / d;

    }

  return (dp_dv);

  }

vector4 vector_polar4 (vector4 v) {

  double x, y, z;

  switch (v.rows) {

    case 0 :
    case 1 : return (v);

    case 2 :

      x = v_el (v, 0) * cos (v_el (v, 1));
      y = v_el (v, 0) * sin (v_el (v, 1));

      v_el (v, 0) = x;
      v_el (v, 1) = y;

      return (v);

    case 3 :

      x = y = v_el (v, 0) * sin (v_el (v, 1));
      z = v_el (v, 0) * cos (v_el (v, 1));

      x *= cos (v_el (v, 2));
      y *= sin (v_el (v, 2));

      v_el (v, 0) = x;
      v_el (v, 1) = y;
      v_el (v, 2) = z;

      return (v);

    }

  return o_vector3;

  }

matrix4 dvector_dpolar3 (vector4 v) {

  matrix4 dv_dp;

  double r, sin_theta, cos_theta, sin_phi, cos_phi;

  dv_dp = o_matrix3;

  r = v_el (v, 0);

  sin_theta = sin (v_el (v, 1));
  cos_theta = cos (v_el (v, 1));
  sin_phi   = sin (v_el (v, 2));
  cos_phi   = cos (v_el (v, 2));

  m_el (dv_dp, 0, 0) = sin_theta * cos_phi;
  m_el (dv_dp, 1, 0) = sin_theta * sin_phi;
  m_el (dv_dp, 2, 0) = cos_theta;

  m_el (dv_dp, 0, 1) =  r * cos_theta * cos_phi;
  m_el (dv_dp, 1, 1) =  r * cos_theta * sin_phi;
  m_el (dv_dp, 2, 1) = -r * sin_theta;

  m_el (dv_dp, 0, 2) = -r * sin_theta * sin_phi;
  m_el (dv_dp, 1, 2) =  r * sin_theta * cos_phi;

  return (dv_dp);

  }

matrix4 matrix4_matrix (matrix m) {

  matrix4 m4;

  long i, j;

  m4 = o_matrix4;

  m4.rows = min (4, m.rows);
  m4.cols = min (4, m.cols);

  for (i = 0; i < m4.rows; i++)
    for (j = 0; j < m4.cols; j++)
      m_el (m4, i, j) = m_el (m, i, j);

  return (m4);

  }

matrix matrix_matrixN (matrix4 * m4) {

  matrix m;

  m.rows = m4->rows;
  m.cols = m4->cols;
  m.nc   = m4->nc;
  m.size = m4->size;
  m.i    = (double *) m4->i;

  return (m);

  }

#define TINY 1.0e-20
#define MAXQRIT 30


/**********************************************************************

        LU decomposition

**********************************************************************/

short LU_decompose (matrix A, long index [], double * d) {

  long i, j, k, n, imax;

  double big, sum, dum, *vv;

  if (!check_matrix (A, "LU_decompose")) return (FALSE);

  if (A.rows != A.cols) {

    cwarn ("LU_decompose : matrix A (%1d x %1d) must be square", A.rows, A.cols);

    return (FALSE);

    }

  n = A.rows;

  vv = calloc (n, sizeof (double));

  if (vv == NULL) {

    cwarn ("LU_decompose : cannot allocate memory for %1d elements of %1d bytes",
            n, sizeof (double));

    return (FALSE);

    }

  *d = 1.0;

  for (i = 0; i < n; i++) {

    big = 0.0;

    for (j = 0; j < n; j++)
      if (fabs (m_el (A, i, j)) > big)
        big = fabs (m_el (A, i, j));

    if (big == 0.0) {

      cwarn ("LU_decompose : matrix A is singular");

      goto error_exit;

      }

    vv [i] = 1.0 / big;

    }

  for (j = 0; j < n; j++) {

    for (i = 0; i < j; i++) {

      sum = m_el (A, i, j);

      for (k = 0; k < i; k++)
        sum -= m_el (A, i, k) * m_el (A, k, j);

      m_el (A, i, j) = sum;

      }

    big = 0.0;

    for (i = j; i < n; i++) {

      sum = m_el (A, i, j);

      for (k = 0; k < j; k++)
        sum -= m_el (A, i, k) * m_el (A, k, j);

      m_el (A, i, j) = sum;

      dum = vv [i] * fabs (sum);

      if (dum > big) {

        big = dum;
        imax = i;

        }
      }

    if (j != imax) {

      for (k = 0; k < n; k++) {

        dum = m_el (A, imax, k);
        m_el (A, imax, k) = m_el (A, j, k);
        m_el (A, j, k) = dum;

        }

      *d = -(*d);
      vv [imax] = vv [j];

      }

    index [j] = imax;

    if (j != n - 1) {

      if (m_el (A, j, j) == 0.0)
        m_el (A, j, j) = TINY;

      dum = 1.0 / m_el (A, j, j);

      for (i = j + 1; i < n; i++)
        m_el (A, i, j) *= dum;

      }

    }

  if (m_el (A, n - 1, n - 1) == 0.0)
    m_el (A, n - 1, n - 1) = TINY;

  free_check (vv);

  return (TRUE);

error_exit:

  free_check (vv);

  return (FALSE);

  }

short LU_backsubstitute (matrix A, long index [], vector b) {

  long i, ii, ip, n, j;

  double sum;

  if (!check_matrix (A, "LU_backsubstitute") ||
      !check_vector (b, "LU_backsubstitute")) return (FALSE);

  if (A.rows != A.cols || A.rows != b.rows) {

    cwarn ("LU_backsubstitute : inconsistent dimensions\n\
         LU matrix : %1d x %1d - vector %1d x %1d",
                     A.rows, A.cols, b.rows, b.cols);

    return (FALSE);

    }

  n = A.rows;

  ii = -1;

  for (i = 0; i < n; i++) {

    ip = index [i];

    sum = v_el (b, ip);

    v_el (b, ip) = v_el (b, i);

    if (ii != -1)
      for (j = ii; j < i; j++)
        sum -= m_el (A, i, j) * v_el (b, j);
    else
      if (sum != 0.0)
        ii = i;

    v_el (b, i) = sum;

    }

  for (i = n - 1; i >= 0; i--) {

    sum = v_el (b, i);

    for (j = i + 1; j < n; j++)
      sum -= m_el (A, i, j) * v_el (b, j);

    v_el (b, i) = sum / m_el (A, i, i);

    }

  return (TRUE);

  }

short LU_inverse (matrix A, long index [], matrix * Y) {

  long i, j, n;

  vector v;

  if (!check_matrix (A, "LU_inverse")) return (FALSE);

  if (A.rows != A.cols) {

    cwarn ("LU_inverse : matrix (%1d x %1d) must be square", A.rows, A.cols);

    return (FALSE);

    }

  n = A.rows;

  v = null_vector;

  setup_vector (&v, A.rows);

  if (!check_vector (v, "LU_inverse")) return (FALSE);

  if (!resize_matrix (Y, A.cols, A.rows)) {

    cwarn ("LU_inverse : cannot resize matrix Y (%1d x %1d)", A.cols, A.rows);

    free_vector (&v);

    return (FALSE);

    }
  
  for (j = 0; j < n; j++) {

    clear_vector (v);

    v_el (v, j) = 1;

    LU_backsubstitute (A, index, v);

    for (i = 0; i < n; i++)
      m_el (*Y, i, j) = v_el (v, i);

    }

  free_vector (&v);

  return (TRUE);

  }


/**********************************************************************

        Singular Value Decomposition

**********************************************************************/

short SV_decompose (matrix A, vector * w, matrix * V) {

  long   l, m, n, it, i, j, k, nm;
  double c, f, g, h, s, x, y, z, scale, Anorm, *rv1, gf, fg, hf, fh;


  if (!check_matrix (A, "SV_decompose")) return (FALSE);

  m = A.rows;
  n = A.cols;

  if (m < n) {

    cwarn ("SV_decompose : matrix A ( %1d x %1d ) has fewer rows than columns", m, n);

    return (FALSE);

    }

  if ((rv1 = (double *) calloc_check_ (n, sizeof (double),
                                           "SV_decompose")) == NULL)
    return (FALSE);

  if (!resize_vector (w, n)) {

    cwarn ("SV_decompose : cannot resize vector w (%1d x 1)", n);

    return (FALSE);

    }

  if (!resize_matrix (V, n, n)) {

    cwarn ("SV_decompose : cannot resize matrix V (%1d x %1d)", n, n);

    return (FALSE);

    }
  
   /* Householder reduction to bidiagonal form */

  g = scale = Anorm = 0.0;

  for (i = 0; i < n; i++) {

    l = i + 1;
    rv1 [i] = scale * g;

    g = s = scale = 0.0;

    for (k = i; k < m; k++)
      scale += fabs (m_el (A, k, i));

    if (scale != 0.0) {

      for (k = i; k < m; k++) {

        m_el (A, k, i) /= scale;
        s += m_el (A, k, i) * m_el (A, k, i);

        }

      f = m_el (A, i, i);

      if (f < 0.0)
        g = sqrt (s);
      else
        g = -sqrt (s);

      h = f * g - s;

      m_el (A, i, i) = f - g;

      for (j = l; j < n; j++) {

        s = 0.0;

        for (k = i; k < m; k++)
          s += m_el (A, k, i) * m_el (A, k, j);

        f = s / h;

        for (k = i; k < m; k++)
          m_el (A, k, j) += f * m_el (A, k, i);

        }

      for (k = i; k < m; k++)
        m_el (A, k, i) *= scale;

      }

    v_el (*w, i) = scale * g;

    g = s = scale = 0.0;

    if (l != n) {

      for (k = l; k < n; k++)
        scale += fabs (m_el (A, i, k));

      if (scale != 0.0) {

        for (k = l; k < n; k++) {

          m_el (A, i, k) /= scale;
          s += m_el (A, i, k) * m_el (A, i, k);

          }

        f = m_el (A, i, l);

        if (f < 0.0)
          g = sqrt (s);
        else
          g = -sqrt (s);

        h = f * g - s;

        m_el (A, i, l) = f - g;

        for (k = l; k < n; k++)
          rv1 [k] = m_el (A, i, k) / h;

        for (j = l; j < m; j++) {

          s = 0.0;

          for (k = l; k < n; k++)
            s += m_el (A, j, k) * m_el (A, i, k);

          for (k = l; k < n; k++)
            m_el (A, j, k) += s * rv1 [k];

          }

        for (k = l; k < n; k++)
          m_el (A, i, k) *= scale;

        }
      }

    x = fabs (v_el (*w, i)) + fabs (rv1 [i]);

    if (x > Anorm)
      Anorm = x;

    }

   /* accumulation of right hand transformations */

  for (i = n - 1; i >= 0; i--) {

    l = i + 1;

    if (l < n) {

      if (g != 0.0) {

        for (j = l; j < n; j++)
          m_el (*V, j, i) = m_el (A, i, j) / m_el (A, i, l) / g;

        for (j = l; j < n; j++) {

          s = 0.0;

          for (k = l; k < n; k++)
            s += m_el (A, i, k) * m_el (*V, k, j);

          for (k = l; k < n; k++)
            m_el (*V, k, j) += s * m_el (*V, k, i);

          }
        }

      for (j = l; j < n; j++)
        m_el (*V, i, j) = m_el (*V, j, i) = 0.0;

      }

    m_el (*V, i, i) = 1.0;
    g = rv1 [i];

    }

   /* accumulation of left hand transformations */

  for (i = n - 1; i >= 0; i--) {

    l = i + 1;

    g = v_el (*w, i);

    for (j = l; j < n; j++)
      m_el (A, i, j) = 0.0;

    if (g != 0.0) {

      g = 1.0 / g;

      for (j = l; j < n; j++) {

        s = 0.0;

        for (k = l; k < m; k++)
          s += m_el (A, k, i) * m_el (A, k, j);

        f = (s / m_el (A, i, i)) * g;

        for (k = i; k < m; k++)
          m_el (A, k, j) += f * m_el (A, k, i);

        }

      for (j = i; j < m; j++)
        m_el (A, j, i) *= g;

      }
    else
      for (j = i; j < m; j++)
        m_el (A, j, i) = 0.0;

    m_el (A, i, i) += 1.0;

    }

   /* diagonalization of the bidiagonal form */

  for (k = n - 1; k >= 0; k--) {

    for (it = 0; it < MAXQRIT; it++) {  /* Test F splitting: */

      for (l = k; l >= 0; l--) {

        nm = l - 1;

        if (fabs (rv1 [l]) + Anorm == Anorm)
          goto TestFconvergence;

        if (fabs (v_el (*w, nm)) + Anorm == Anorm)
          break;

         /* goto Cancellation; */

        }

       /* Cancellation: */

      c = 0.0;
      s = 1.0;

      for (i = l; i <= k; i++) {

        f = s * rv1 [i];

        /* rv1 [i] *= c; added */

        if (fabs (f) + Anorm != Anorm) {

          g = v_el (*w, i);

          /* h = sqrt (f * f + g * g); */

          if (fabs (f) < fabs (g))
            { fg = f / g; h = fabs (g) * sqrt (1.0 + fg * fg); }
          else
            { gf = g / f; h = fabs (f) * sqrt (1.0 + gf * gf); }

          v_el (*w, i) = h;

          c = g / h;
          s = -f / h;

          for (j = 0; j < m; j++) {

            y = m_el (A, j, nm);
            z = m_el (A, j, i);
            m_el (A, j, nm) = y * c + z * s;
            m_el (A, j, i)  = z * c - y * s;

            }
          } /* else break; added */
        }

TestFconvergence:

      z = v_el (*w, k);

      if (l == k)
        goto Convergence;

       /* shift from bottom 2x2 minor */

      nm = k - 1;

      x = v_el (*w, l);
      y = v_el (*w, nm);
      g = rv1 [nm];
      h = rv1 [k];

      f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);

      g = sqrt (f * f + 1.0);

      if (f > 0.0)
        f = ((x - z) * (x + z) + h * (y / (f + g) - h)) / x;
      else
        f = ((x - z) * (x + z) + h * (y / (f - g) - h)) / x;

       /* next QR transformation */

      c = s = 1.0;

      for (j = l; j < k; j++) {

        i = j + 1;

        g = rv1 [i];
        y = v_el (*w, i);
        h = s * g;
        g *= c;

        /* z = sqrt (f * f + h * h); */

        if (fabs (f) < fabs (h))
          { fh = f / h; z = fabs (h) * sqrt (1.0 + fh * fh); }
        else
          if (f == 0.0)
            z = 0.0;
          else
            { hf = h / f; z = fabs (f) * sqrt (1.0 + hf * hf); }

        rv1 [j] = z;

        if (z == 0.0)
          f = z = 1.0; /* added */

        c = f / z;
        s = h / z;
        f = x * c + g * s;
        g = g * c - x * s;
        h = y * s;
        y *= c;

        for (nm = 0; nm < n; nm++) {

          x = m_el (*V, nm, j);
          z = m_el (*V, nm, i);
          m_el (*V, nm, j) = x * c + z * s;
          m_el (*V, nm, i) = z * c - x * s;

          }

        /* z = sqrt (f * f + h * h); */

        if (fabs (f) < fabs (h))
          { fh = f / h; z = fabs (h) * sqrt (1.0 + fh * fh); }
        else
          if (f == 0.0)
            z = 0.0;
          else
            { hf = h / f; z = fabs (f) * sqrt (1.0 + hf * hf); }

        v_el (*w, j) = z;

        if (z == 0.0)
          z = f = 1.0; /* added */

        c = f / z;
        s = h / z;
        f = c * g + s * y;
        x = c * y - s * g;

        for (nm = 0; nm < m; nm++) {

          y = m_el (A, nm, j);
          z = m_el (A, nm, i);
          m_el (A, nm, j) = y * c + z * s;
          m_el (A, nm, i) = z * c - y * s;

          }
        }

      rv1 [l] = 0.0;
      rv1 [k] = f;
      v_el (*w, k) = x;

      } /* it */

    if (it >= MAXQRIT) {

      rv1 [k] = 0.0;

      cout (" ERROR: SV_decompose : QR failed :No convergence in %1d\
 iterations\n", it);

      }

Convergence:

    if (z < 0.0) {

      v_el (*w, k) = -z;

      for (j = 0; j < n; j++)
        m_el (*V, j, k) = -m_el (*V, j, k);

      }

    } /* k */

  free_check (rv1);

  return (TRUE);

  }

short SV_decompose_ (matrix * A, vector * w) {

  long   l, m, n, it, i, j, k, nm;
  double c, f, g, h, s, x, y, z, scale, Anorm, *rv1, gf, fg, hf, fh;

  matrix *V;

  if (!check_matrix (*A, "SV_decompose_")) return (FALSE);

  m = A->rows;
  n = A->cols;

  if (m < n) {

    cwarn ("SV_decompose_ : matrix A ( %1d x %1d ) has fewer rows than columns", m, n);

    return (FALSE);

    }

  if ((rv1 = (double *) calloc_check_ (n, sizeof (double),
                                           "SV_decompose")) == NULL)
    return (FALSE);

  if (!resize_vector (w, n)) {

    cwarn ("SV_decompose_ : cannot resize vector w (%1d x 1)", n);

    return (FALSE);

    }

  V = A;

  V->rows = n;
  V->cols = n;

   /* Householder reduction to bidiagonal form */

  g = scale = Anorm = 0.0;

  for (i = 0; i < n; i++) {

    l = i + 1;
    rv1 [i] = scale * g;

    g = s = scale = 0.0;

    for (k = i; k < m; k++)
      scale += fabs (m_el (*A, k, i));

    if (scale != 0.0) {

      for (k = i; k < m; k++) {

        m_el (*A, k, i) /= scale;
        s += m_el (*A, k, i) * m_el (*A, k, i);

        }

      f = m_el (*A, i, i);

      if (f < 0.0)
        g = sqrt (s);
      else
        g = -sqrt (s);

      h = f * g - s;

      m_el (*A, i, i) = f - g;

      for (j = l; j < n; j++) {

        s = 0.0;

        for (k = i; k < m; k++)
          s += m_el (*A, k, i) * m_el (*A, k, j);

        f = s / h;

        for (k = i; k < m; k++)
          m_el (*A, k, j) += f * m_el (*A, k, i);

        }

      for (k = i; k < m; k++)
        m_el (*A, k, i) *= scale;

      }

    v_el (*w, i) = scale * g;

    g = s = scale = 0.0;

    if (l != n) {

      for (k = l; k < n; k++)
        scale += fabs (m_el (*A, i, k));

      if (scale != 0.0) {

        for (k = l; k < n; k++) {

          m_el (*A, i, k) /= scale;
          s += m_el (*A, i, k) * m_el (*A, i, k);

          }

        f = m_el (*A, i, l);

        if (f < 0.0)
          g = sqrt (s);
        else
          g = -sqrt (s);

        h = f * g - s;

        m_el (*A, i, l) = f - g;

        for (k = l; k < n; k++)
          rv1 [k] = m_el (*A, i, k) / h;

        for (j = l; j < m; j++) {

          s = 0.0;

          for (k = l; k < n; k++)
            s += m_el (*A, j, k) * m_el (*A, i, k);

          for (k = l; k < n; k++)
            m_el (*A, j, k) += s * rv1 [k];

          }

        for (k = l; k < n; k++)
          m_el (*A, i, k) *= scale;

        }
      }

    x = fabs (v_el (*w, i)) + fabs (rv1 [i]);

    if (x > Anorm)
      Anorm = x;

    }

   /* accumulation of right hand transformations */

  for (i = n - 1; i >= 0; i--) {

    l = i + 1;

    if (l < n) {

      if (g != 0.0) {

        for (j = l; j < n; j++)
          m_el (*V, j, i) = m_el (*A, i, j) / m_el (*A, i, l) / g;

        for (j = l; j < n; j++) {

          s = 0.0;

          for (k = l; k < n; k++)
            s += m_el (*A, i, k) * m_el (*V, k, j);

          for (k = l; k < n; k++)
            m_el (*V, k, j) += s * m_el (*V, k, i);

          }
        }

      for (j = l; j < n; j++)
        m_el (*V, i, j) = m_el (*V, j, i) = 0.0;

      }

    m_el (*V, i, i) = 1.0;
    g = rv1 [i];

    }

   /* diagonalization of the bidiagonal form */

  for (k = n - 1; k >= 0; k--) {

    for (it = 0; it < MAXQRIT; it++) {  /* Test F splitting: */

      for (l = k; l >= 0; l--) {

        nm = l - 1;

        if (fabs (rv1 [l]) + Anorm == Anorm)
          goto TestFconvergence;

        if (fabs (v_el (*w, nm)) + Anorm == Anorm)
          break;

         /* goto Cancellation; */

        }

       /* Cancellation: */

      c = 0.0;
      s = 1.0;

      for (i = l; i <= k; i++) {

        f = s * rv1 [i];

        /* rv1 [i] *= c; added */

        if (fabs (f) + Anorm != Anorm) {

          g = v_el (*w, i);

          /* h = sqrt (f * f + g * g); */

          if (fabs (f) < fabs (g))
            { fg = f / g; h = fabs (g) * sqrt (1.0 + fg * fg); }
          else
            { gf = g / f; h = fabs (f) * sqrt (1.0 + gf * gf); }

          v_el (*w, i) = h;

          c = g / h;
          s = -f / h;

          } /* else break; added */
        }

TestFconvergence:

      z = v_el (*w, k);

      if (l == k)
        goto Convergence;

       /* shift from bottom 2x2 minor */

      nm = k - 1;

      x = v_el (*w, l);
      y = v_el (*w, nm);
      g = rv1 [nm];
      h = rv1 [k];

      f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);

      g = sqrt (f * f + 1.0);

      if (f > 0.0)
        f = ((x - z) * (x + z) + h * (y / (f + g) - h)) / x;
      else
        f = ((x - z) * (x + z) + h * (y / (f - g) - h)) / x;

       /* next QR transformation */

      c = s = 1.0;

      for (j = l; j < k; j++) {

        i = j + 1;

        g = rv1 [i];
        y = v_el (*w, i);
        h = s * g;
        g *= c;

        /* z = sqrt (f * f + h * h); */

        if (fabs (f) < fabs (h))
          { fh = f / h; z = fabs (h) * sqrt (1.0 + fh * fh); }
        else
          if (f == 0.0)
            z = 0.0;
          else
            { hf = h / f; z = fabs (f) * sqrt (1.0 + hf * hf); }

        rv1 [j] = z;

        if (z == 0.0)
          f = z = 1.0; /* added */

        c = f / z;
        s = h / z;
        f = x * c + g * s;
        g = g * c - x * s;
        h = y * s;
        y *= c;

        for (nm = 0; nm < n; nm++) {

          x = m_el (*V, nm, j);
          z = m_el (*V, nm, i);
          m_el (*V, nm, j) = x * c + z * s;
          m_el (*V, nm, i) = z * c - x * s;

          }

        /* z = sqrt (f * f + h * h); */

        if (fabs (f) < fabs (h))
          { fh = f / h; z = fabs (h) * sqrt (1.0 + fh * fh); }
        else
          if (f == 0.0)
            z = 0.0;
          else
            { hf = h / f; z = fabs (f) * sqrt (1.0 + hf * hf); }

        v_el (*w, j) = z;

        if (z == 0.0)
          z = f = 1.0; /* added */

        c = f / z;
        s = h / z;
        f = c * g + s * y;
        x = c * y - s * g;

        }

      rv1 [l] = 0.0;
      rv1 [k] = f;
      v_el (*w, k) = x;

      } /* it */

    if (it >= MAXQRIT) {

      rv1 [k] = 0.0;

      cout (" ERROR: SV_decompose_ : QR failed : No convergence in %1d\
 iterations\n", it);

      }

Convergence:

    if (z < 0.0) {

      v_el (*w, k) = -z;

      for (j = 0; j < n; j++)
        m_el (*V, j, k) = -m_el (*V, j, k);

      }

    } /* k */

  free_check (rv1);

  return (TRUE);

  }

#endif /* __MATRIX_C__ */
