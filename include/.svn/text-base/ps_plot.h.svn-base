/* This file contains the routines used to generate PostScript files.

   Copyright (C) Paul J. Ellis, University of Sydney, 1993. */

#if !defined (__POSTSCRIPT_PLOT_H__)

#define __POSTSCRIPT_PLOT_H__

#include "PS.h"
#include "grad.h"

double PS (double x_min, double x_max,
           double PS_min, double PS_max, double x) {

  return ((x - x_min) / (x_max - x_min)) * (PS_max - PS_min) + PS_min;

  }

void PostScript_title (FILE *file, char line1 [], char line2 []) {

  int lines, l1, length;

  double font_size;

  lines = length = 0;

  if (line1)
    lines += (l1 = line1 [strspn (line1, WHITESPACE)] != 0);

  if (line2)
    lines += line2 [strspn (line2, WHITESPACE)] != 0;

  if (lines == 1 && !l1)
    line1 = line2;

  if (!lines)
    return;

  if (lines)
    length = strlen (line1);

  if (lines == 2)
    length = max (length, strlen (line2));

  PostScript_font_angle (file, 0);

  PostScript_font_size (file, min (20.0, 800.0 / length));
 
  if (lines)
    PostScript_string (file, line1, 415.4, lines == 2 ? 569.9 : 560.3,
                                    PS_CENTRE);

  if (lines == 2)
    PostScript_string (file, line2, 415.4, 548.0, PS_CENTRE);

  }

void PostScript_axes (FILE *file,
                      double x_min, double x_max, char x_label [],
                      double y_min, double y_max, char y_label []) {

  double ps_grad, grad, length, d; char value [64];

  PostScript_vertex (file, 122.4, 118.8);
  PostScript_vertex (file, 712.0, 118.8);
  PostScript_vertex (file, 712.0, 520.2);
  PostScript_vertex (file, 122.4, 520.2);
  PostScript_vertex (file, 122.4, 118.8);

  PostScript_end_line (file);

  PostScript_font      (file, "Courier-Bold");
  PostScript_font_size (file, 14);
  PostScript_pen_width (file, 0.58);

  for (grad = next_gradation_division (x_min, x_max, 7);
       continue_gradations;
       grad = next_gradation_division (0, 0, 0)) {

    ps_grad = PS (x_min, x_max, 122.4, 712.0, grad);

    length = major_gradation ? 7 : 3.5;

    PostScript_draw_line (file, ps_grad, 118.8, ps_grad, 118.8 + length);
    PostScript_draw_line (file, ps_grad, 520.2, ps_grad, 520.2 - length);

    if (major_gradation) {

      sprintf (value, "%.4g", grad);

      PostScript_string (file, value, ps_grad, 107.1, PS_CENTRE);

      }
    }

  for (grad = next_gradation_division (y_min, y_max, 4);
       continue_gradations;
       grad = next_gradation_division (0, 0, 0)) {

    ps_grad = PS (y_min, y_max, 118.4, 520.2, grad);

    length = major_gradation ? 7 : 3.5;

    PostScript_draw_line (file, 122.4, ps_grad, 122.4 + length, ps_grad);
    PostScript_draw_line (file, 712.0, ps_grad, 712.0 - length, ps_grad);

    if (major_gradation) {

      sprintf (value, "%.4g", grad);

      PostScript_string (file, value, 115.0, ps_grad, PS_RIGHT);

      }
    }

  PostScript_font_size (file, 20);
  PostScript_font_angle (file, 90);

  PostScript_string (file, y_label, 44.6, 305.4, PS_CENTRE);

  PostScript_font_angle (file, 0);

  PostScript_string (file, x_label, 405.5, 72.7, PS_CENTRE);

  }

#define PS_LINE    0
#define PS_SQUARES 1
#define PS_CIRCLES 2

void PostScript_plot (FILE *file, int points, double x [], double y [],
                                  double x_min, double x_max,
                                  double y_min, double y_max,
                                  int type, double size, double level) {

  int i;
  double ps_x, ps_y, l;

  for (i = 0; i < points; i++) {

    ps_x = PS (x_min, x_max, 122.4, 712.0, x [i]);
    ps_y = PS (y_min, y_max, 118.8, 520.2, y [i]);

    l = level;

    if (l < 0)
      l = -l * i;
      
    if (type == PS_LINE)
      PostScript_vertex (file, ps_x, ps_y);

    if (type == PS_SQUARES)
      PostScript_square (file, ps_x, ps_y, size, l);

    if (type == PS_CIRCLES)
      PostScript_circle (file, ps_x, ps_y, size, l);

    }

  if (type == PS_LINE)
    PostScript_end_line (file);

  }

#endif /* __POSTSCRIPT_PLOT_H__ */

