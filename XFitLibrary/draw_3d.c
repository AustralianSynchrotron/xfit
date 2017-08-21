/***********************************************************************}
{                                                                       }
{  drawing routines for 3-D lines and strings                           }
{                                                                       }
{***********************************************************************/

#if !defined (DRAW_3D_C)

#define DRAW_3D_C

#include <stdio.h>

#include "draw.h"

#include "draw_3d.h"

#include "char_set.h"


matrix4 G_orientation = I_matrix4;

int G_rotate = 1;
int G_colour = 0;

void Begin_line (void) {

  beginline ();

  }

void End_line (void) {

  endline ();

  }

void Ortho (double x_min, double x_max,  double y_min, double y_max,

              double z_min, double z_max) {

  drawspace (x_min, x_max, y_min, y_max, z_min, z_max);

  }

void Ortho2 (double x_min, double x_max, double y_min, double y_max) {

  Ortho (x_min, x_max, y_min, y_max, -1, 1);

  }

static float colour_0 [3];

void Colour (double R, double G, double B) {

  colour_0 [0] = R;
  colour_0 [1] = G;
  colour_0 [2] = B;

  }

void Clear_RGB (double R, double G, double B) {

    /* Clear the current window to (R, G, B) */

  RGBcolour (R, G, B);

  clearwindow ();

  }

vector4 G_orient (double v []) {

  vector4 X;

  X = vector3_1 (v);

  if (G_rotate)

    X = mul_matrix4_vector4 (G_orientation, X);

  return X;

  }

void G_vertex (double v []) {

  vector4 X;

  double z, * x;

  X = G_orient (v);

  x = & v_el (X, 0);

  z = 1.0;

  if (G_rotate || G_colour)

    z = max (min (x [2] / 5 + 0.6, 1), 0.1);

  RGBcolour (z * colour_0 [0], z * colour_0 [1], z * colour_0 [2]);

    /* printf (" G(%.2f,%.2f,%.2f)", x [0], x [1], x [2]);

  fflush (stdout); */

  move (x [0], x [1], x [2]);

  }

void G_vertex3 (double x, double y, double z) {

  double v [3];

  v [0] = x;
  v [1] = y;
  v [2] = z;

  G_vertex (v);

  }

void G_vertex2 (double x, double y) {

  G_vertex3 (x, y, 0);

  }


void G_line (double A [], double B []) {

  Begin_line ();

  G_vertex (A);
  G_vertex (B);

  End_line ();

  }


void G_line3 (double x_A, double y_A, double z_A,

              double x_B, double y_B, double z_B) {

  Begin_line ();

  G_vertex3 (x_A, y_A, z_A);
  G_vertex3 (x_B, y_B, z_B);

  End_line ();

  }

void G_line2 (double x_A, double y_A, double x_B, double y_B) {

  Begin_line ();

  G_vertex2 (x_A, y_A);
  G_vertex2 (x_B, y_B);

  End_line ();

  }

void G_polyline2 (int N, double x [], double y []) {

  int i;

  if (N <= 0)

    return;

  Begin_line ();

  for (i = 0; i < N; i++)

    G_vertex2 (x [i], y [i]);

  End_line ();

  }


void G_draw_character (double x0, double y0, double z0,

                                double dx, double dy, int c) {

    /* Draw the character using the vector character set __VCS */

  vector_character * v;

  int i, cx, cy;

  if (c < 0 || c > 127)

    return;

  v = vector_c [c];

  if (v == NULL)

    return;

  cx = v->x;
  cy = v->y;

  v++;

  dx *= cx * 0.0001 / 0.77;
  dy *= cy * 0.0001 / 0.86;

  Begin_line ();

  for (; ; v++)

    if (v->x == -1) {

      End_line ();

      if (v->y == 0)

        Begin_line ();

      else

        break;

      }
    else

      G_vertex3 ((v->x - 50) * dx + x0,
                  v->y       * dy + y0,
                                    z0);

  }

void G_draw_string (double x0, double y0, double z0, double dx, double dy,

                             char c [], char justify [], int rotate) {

  int i, n, G_rotate_o;

  double x [3], sx, sy;

  vector4 O;

  v_el (O, 0) = x0;
  v_el (O, 1) = y0;
  v_el (O, 2) = z0;


    /* Save the current rotation */

  G_rotate_o = G_rotate;


    /* If the text should always lie in the viewer's xy plane ... */

  if (!rotate) {

      /* Get the rotated coordinates of the origin */

    O = G_orient (&v_el (O, 0));


      /* Turn off the rotation */

    G_rotate = 0;

    }

  n = strlen (c);

  sx = sy = 1;

  if (strchr ("LlRr", justify [0])) {

    sx = (strchr ("Rr", justify [0]) ? -0.5 * n :
                                        0.5 * n );

    v_el (O, 0) += sx * dx;

    sx = sx < 0 ? -1 : 1;

    }

  if (strchr ("0123456789", justify [1]))

    v_el (O, 0) += sx * (0.1 * (justify [1] - '0')) * dx;

  if (strchr ("UuCcDd", justify [2]) && justify [2]) {

    sy = (strchr ("Dd", justify [2]) ? -0.8 :
          strchr ("Uu", justify [2]) ?  0.5 :
                                       -0.5 );

    v_el (O, 1) += sy * dy;

    sy = sy < 0 ? -1 : 1;

    }

  if (strchr ("0123456789", justify [3]))

    v_el (O, 1) += sy * (0.1 * (justify [3] - '0')) * dy;

  for (i = 0; c [i]; i++)

    G_draw_character (v_el (O, 0) - (0.5 * (n - 1) - i) * dx, v_el (O, 1),
                                 v_el (O, 2), 0.77 * dx, 1.0 * dy, c [i]);


    /* Restore the rotation */

  G_rotate = G_rotate_o;

  }


void G_draw_number (double x0, double y0, double z0, double dx, double dy,

                    char format [], double D, char justify [], int rotate) {

  int n;

  char c [32];

    /* Print the number into c */

  n = sprintf (c, format, D);

  if (n >= 4)

    if (c [n - 2] == '0' &&
       (c [n - 3] == 'e' || c [n - 4] == 'e')) {

      c [n - 2] = c [n - 1];
      c [n - 1] = 0;

      }

  G_draw_string (x0, y0, z0, dx, dy, c, justify, rotate);

  }

void G_draw_string_v (double v0 [], double v [], double dx, double dy,

                    char c []) {

  matrix4 G_o;

  int G_rotate0, i;

  vector4 O0, O, X, Y, Z;

  G_o = G_orientation;

  G_rotate0 = G_rotate;


    /* Get the rotated coordinates of the line */

  O0 = G_orient (v0);
  O  = G_orient (v);


    /* Turn on the rotation */

  G_rotate = 1;

  X = normal_vector4 (sub_vector4 (O0, O));

  if (abs_vector4 (X) == 0)

    return;

  if (v_el (X, 0) < 0)

    X = neg_vector4 (X);

  Y = normal_vector4 (cross_vector4 (X, e_vector3 [2]));

  if (abs_vector4 (Y) == 0)

    Y = e_vector3 [1];

  if (v_el (Y, 1) < 0)

    Y = neg_vector4 (Y);

  Z = cross_vector4 (X, Y);

  for (i = 0; i < 3; i++) {

    m_el (G_orientation, i, 0) = v_el (X, i);
    m_el (G_orientation, i, 1) = v_el (Y, i);
    m_el (G_orientation, i, 2) = v_el (Z, i);

    m_el (G_orientation, i, 3) = (v_el (O0, i) + v_el (O, i)) / 2;

    }

    /* Draw the string */

  G_draw_string (0, 0, 0, dx, dy, c, "C0U0", 1);

  G_rotate = G_rotate0;

  G_orientation = G_o;

  }


#endif /* DRAW_3D_C */

