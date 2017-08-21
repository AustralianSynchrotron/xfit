/***********************************************************************}
{                                                                       }
{  3-D lines and strings.                                               }
{                                                                       }
{***********************************************************************/

#if !defined (DRAW_3D_H)

#define DRAW_3D_H

#include "draw.h"

#include "matrix.h"


#if !defined (DRAW_3D_C)

extern matrix4 G_orientation;

extern int G_rotate;

extern int G_colour;

#endif


void Begin_line (void);

void End_line (void);


void Ortho (double x_min, double x_max,  double y_min, double y_max,

              double z_min, double z_max);

void Ortho2 (double x_min, double x_max,  double y_min, double y_max);


void Colour (double R, double G, double B);

void Clear_RGB (double R, double G, double B);


void G_vertex (double x []);

void G_vertex3 (double x, double y, double z);

void G_vertex2 (double x, double y);

void G_line (double A [], double B []);

void G_line3 (double x_A, double y_A, double z_A,

              double x_B, double y_B, double z_B);

void G_line2 (double x_A, double y_A, double x_B, double y_B);

void G_polyline2 (int N, double x [], double y []);


void G_draw_character (double x0, double y0, double z0,

                                double dx, double dy, int c);

void G_draw_string (double x0, double y0, double z0, double dx, double dy,

                             char c [], char justify [], int rotate);

void G_draw_number (double x0, double y0, double z0, double dx, double dy,

                    char format [], double D, char justify [], int rotate);

void G_draw_string_v (double v0 [], double v [], double dx, double dy,

                      char c []);


#endif /* DRAW_3D_H */

