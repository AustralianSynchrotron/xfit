/* This file contains the routines used to generate PostScript files.

   Copyright (C) Paul J. Ellis, University of Sydney, 1993. */

#if !defined (__POSTSCRIPT_H__)

#define __POSTSCRIPT_H__

#include "header.h"

#define PS_Page_x 792.0
#define PS_Page_y 612.0

#define PS_LEFT   -1
#define PS_CENTRE  0
#define PS_RIGHT   1

int PS_start_line = 1;

double PS_grey = 0;

  /* Default: font, font size and angle */

char   *PS_font       = "Courier-Bold";
double	PS_font_size  = 1,
        PS_font_angle = 0;

int	PS_font_OK = 0;

void PostScript_header (FILE *file) {

  fprintf (file,
        "%%!\n"
        " /CurrFontSize 1.0 def\n"
        " /Show_ {\n"
        " exch 1 index stringwidth\n"
        " 2 index 0 eq { 2 copy -2 div exch -2 div exch rmoveto } if\n"
        " 2 index 1 eq { 2 copy neg exch neg exch rmoveto } if\n"
        " 2 copy dup mul exch dup mul add sqrt\n"
        " dup 3 1 roll div CurrFontSize mul 0.3 mul\n"
        " 3 1 roll div CurrFontSize mul -0.3 mul\n"
        " rmoveto pop show } def\n"
        " 1 setlinecap 1 setlinejoin\n"
        " 90.0 rotate\n"
        " 0 -612 translate\n"
        " 0.58 setlinewidth 0.00 setgray\n");

  }

void PostScript_footer (FILE *file) {

  fprintf (file,
        " showpage\n"
        " 1 setlinecap 1 setlinejoin\n"
        " 90.0 rotate\n"
        " 0 -612 translate\n");

  }

void PostScript_vertex (FILE *file, double x, double y) {

  if (PS_start_line) {

    fprintf (file,
        " %.1f %.1f moveto\n", x, y);

    PS_start_line = 0;

    }
  else
    fprintf (file,
        " %.1f %.1f lineto\n", x, y);

  }

void PostScript_end_line (FILE *file) {

  fprintf (file, " stroke\n");

  PS_start_line = 1;

  }

void PostScript_draw_line (FILE *file, double x0, double y0,
                                       double x1, double y1) {

  PostScript_vertex   (file, x0, y0);
  PostScript_vertex   (file, x1, y1);
  PostScript_end_line (file);

  }

void PostScript_line_style (FILE *file, double on_1, double off_1,
                                        double on_2, double off_2) {

  fprintf (file,
        " [ %.1f %.1f %.1f %.1f ] 0 setdash\n",
                                        on_1, off_1, on_2, off_2);

  }

void PostScript_font (FILE *file, char font []) {

  if (font && strcmp (PS_font, font)) {

    PS_font = font;

    PS_font_OK = 0;

    }
  }

void PostScript_font_size (FILE *file, double size) {

  if (size > 0 && PS_font_size != size) {

    PS_font_size = size;

    PS_font_OK = 0;

    }
  }

void PostScript_font_angle (FILE *file, double angle) {

  if (PS_font_angle != angle) {

    PS_font_angle = angle;

    PS_font_OK = 0;

    }
  }

void check_PostScript_font (FILE *file) {

  if (PS_font_OK)
    return;

  fprintf (file,
        " /%s findfont %.1f scalefont\n", PS_font, PS_font_size);

  if (fabs (PS_font_angle) >= 0.05)
    fprintf (file,
        " %.1f matrix rotate makefont\n", PS_font_angle);

  fprintf (file,
        " setfont\n"
        " /CurrFontSize %.1f def\n", PS_font_size);

  PS_font_OK = 1;

  }

void PostScript_string (FILE *file, char line [], double x, double y,
                                                  int justification) {
  int i;

  check_PostScript_font (file);

  fprintf (file,
        " %.1f %.1f moveto %d (", x, y, justification);

  for (i = 0; line [i]; i++) {

    if (line [i] == '(' || line [i] == ')')
      fprintf (file, "\\");

    fprintf (file, "%c", line [i]);

    }

  fprintf (file,
        ") Show_\n");

  }

void PostScript_pen (FILE *file, double level) {

  if (level >= -0.001)
    PS_grey = 1 - level;

  fprintf (file,
      " %.2f setgray\n", PS_grey);

  }

void PostScript_pen_width (FILE *file, double width) {

  fprintf (file,
        " %.2f setlinewidth\n", width);

  }

void PostScript_square (FILE *file, double x, double y,
                                    double size, double level) {

  size /= 2;

  fprintf (file,
        " %.2f setgray\n"
        " %.1f %.1f moveto\n"
        " %.1f %.1f lineto\n"
        " %.1f %.1f lineto\n"
        " %.1f %.1f lineto closepath\n",
        1 - level,
        x - size, y - size, x + size, y - size,
        x + size, y + size, x - size, y + size);

  PostScript_pen (file, -1);

  fprintf (file,
        " %.1f %.1f moveto\n"
        " %.1f %.1f lineto\n"
        " %.1f %.1f lineto\n"
        " %.1f %.1f lineto closepath stroke\n",
        x - size, y - size, x + size, y - size,
        x + size, y + size, x - size, y + size);

  }

void PostScript_circle (FILE *file, double x, double y,
                                    double size, double level) {

  size /= 2;

  fprintf (file,
        " %.2f setgray\n"
        " %.1f %.1f %.1f 0 360 arc fill\n",
        1 - level, x, y, size);

  PostScript_pen (file, -1);

  fprintf (file,
        " %.1f %.1f %.1f 0 360 arc stroke\n",
        x, y, size);

  }

#endif /* __POSTSCRIPT_H__ */

