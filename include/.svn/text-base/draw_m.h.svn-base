
/* Copyright (C) Paul J. Ellis, University of Sydney, 1993. */

#if !defined (__DRAW_M_H__)

#define __DRAW_M_H__

#define BLACK  0
#define RED    1
#define GREEN  2
#define YELLOW 3
#define BLUE   4
#define PURPLE 5
#define CYAN   6
#define WHITE  7
#define GREY   8

int GREYSCALE [] = {  0,  9, 10, 11, 12, 13, 14, 15,
                      8, 16, 17, 18, 19, 20, 21, 22, 7 };

XColor XCOLOUR [] = {

  {  0,     0,     0,     0 },
  {  0, 65535,     0,     0 },
  {  0,     0, 65535,     0 },
  {  0, 65535, 65535,     0 },

  {  0,     0,     0, 65535 },
  {  0, 65535,     0, 65535 },
  {  0,     0, 65535, 65535 },
  {  0, 65535, 65535, 65535 },

  {  0, 32767, 32767, 32767 },

  {  0,  4096,  4096,  4096 },
  {  0,  8192,  8192,  8192 },
  {  0, 12288, 12288, 12288 },
  {  0, 16384, 16384, 16384 },
  {  0, 20480, 20480, 20480 },
  {  0, 24576, 24576, 24576 },
  {  0, 28672, 28672, 28672 },
  {  0, 36863, 36863, 36863 },
  {  0, 40959, 40959, 40959 },
  {  0, 45055, 45055, 45055 },
  {  0, 49151, 49151, 49151 },
  {  0, 53247, 53247, 53247 },
  {  0, 57343, 57343, 57343 },
  {  0, 61439, 61439, 61439 }

  };

int W_define_colour (Widget w, XColor colour []) {

  if (!XAllocColor (XtDisplay (w),
                    XDefaultColormapOfScreen (XtScreen (w)),
                    colour)) {

    fprintf (stderr, " ERROR: Colour [%d,%d,%d] not allocated\n",
                     colour->red, colour->green, colour->blue);

    return 0;

    }

  /* printf (" Colour [%d,%d,%d] = %d\n",
                     colour->red, colour->green, colour->blue,
                     colour->pixel); */

  return 1;

  }

int W_define_colours (Widget w) {

  int n;

  for (n = 0; n < 23; n++)

    if (!W_define_colour (w, &XCOLOUR [n])) return 0;

  return 1;

  }

#define M_MAX_WINDOWS 8

Widget W_widget [M_MAX_WINDOWS];
double W_coord  [M_MAX_WINDOWS][8];

int W_entry (Widget w) {

  static int first = TRUE;

  int W_number;

  if (first) {

    W_define_colours (w);

    for (W_number = 0; W_number < M_MAX_WINDOWS; W_number++)
      W_widget [W_number] = NULL;

    first = FALSE;

    }

  for (W_number = 0; W_number < M_MAX_WINDOWS; W_number++)
    if (w == W_widget [W_number])
      return W_number;

  for (W_number = 0; W_number < M_MAX_WINDOWS; W_number++)
    if (W_widget [W_number] == NULL) {

      W_widget [W_number] = w;

      return W_number;

      }

  return 0;

  }

void Set_Window_Coordinates (Widget drawing_area,
                        double x0, double x1, double y0, double y1) {

  Arg arg [3];
  Dimension w, h;
  int n;

  n = W_entry (drawing_area);

  XtSetArg (arg [0], XmNunitType, XmPIXELS);

  XtSetValues (drawing_area, arg, 1);

  XtSetArg (arg [0], XmNheight, &h);
  XtSetArg (arg [1], XmNwidth,  &w);

  XtGetValues (drawing_area, arg, 2);

  W_coord [n][0] =  w / (x1 - x0);
  W_coord [n][1] = -x0 * W_coord [n][0];

  W_coord [n][2] = h / (y0 - y1);
  W_coord [n][3] = h - y0 * W_coord [n][2];

  W_coord [n][4] = min (x0, x1); W_coord [n][5] = max (x0, x1);
  W_coord [n][6] = min (y0, y1); W_coord [n][7] = max (y0, y1);

  }

void W_map (int n, double *x, double *y, double x0, double y0) {

  *x = x0 * W_coord [n][0] + W_coord [n][1];
  *y = y0 * W_coord [n][2] + W_coord [n][3];

  }

void Map (Widget w, double *x, double *y, double x0, double y0) {

  int n;

  n = W_entry (w);

  W_map (n, x, y, x0, y0);

  }

void W_unmap (int n, double *x0, double *y0, double x, double y) {

  *x0 = (x - W_coord [n][1]) / W_coord [n][0];
  *y0 = (y - W_coord [n][3]) / W_coord [n][2];

  *x0 = min (max (*x0, W_coord [n][4]), W_coord [n][5]);
  *y0 = min (max (*y0, W_coord [n][6]), W_coord [n][7]);

  }

void Unmap (Widget w, double *x0, double *y0, double x, double y) {

  int n;

  n = W_entry (w);

  W_unmap (n, x0, y0, x, y);

  }

GC Setup_GC (Widget drawing_area) { /* set up the graphics context. */

  int n, foreground, background;
  Arg arg [10];

  XGCValues vals;

    /* get the current foreground and background colors. */

  n = 0;

  XtSetArg (arg [n], XmNforeground, &foreground); n++;
  XtSetArg (arg [n], XmNbackground, &background); n++;

  XtGetValues (drawing_area, arg, n);

    /* create the copy GC. */

  vals.foreground = foreground;
  vals.background = background;

  return XCreateGC (XtDisplay (drawing_area), XtWindow (drawing_area),
                                 GCForeground | GCBackground, &vals);

  }

void Colour (Widget drawing_area, GC graph_context,
                                            unsigned long colour) {

  W_entry (drawing_area);

  XSetForeground (XtDisplay (drawing_area), graph_context,
                  XCOLOUR [colour].pixel);

  }

void Draw_Lines (Widget drawing_area, GC graph_context,
                                      int N, double x [], double y []) {

  XPoint *point; double x_point, y_point; int i, n;

  n = W_entry (drawing_area);

  point = calloc_check (N, sizeof (XPoint));

  for (i = 0; i < N; i++) {

    W_map (n, &x_point, &y_point, x [i], y [i]);

    point [i].x = rint (x_point);
    point [i].y = rint (y_point);

    }

  XDrawLines (XtDisplay (drawing_area), XtWindow (drawing_area),
              graph_context, point, N, CoordModeOrigin);

  XFlush (XtDisplay (drawing_area));

  free_check (point);

  }

void Draw_Line (Widget drawing_area, GC graph_context,
                          double x0, double y0, double x1, double y1) {

  double x [2], y [2];

  x [0] = x0;
  y [0] = y0;
  x [1] = x1;
  y [1] = y1;

  Draw_Lines (drawing_area, graph_context, 2, x, y);

  }

void Set_Label (Widget label, char text []) {

  Arg arg [2];

  XtSetArg (arg [0], XmNlabelString, XmStringLtoRCreate (text, ""));
  XtSetValues (label, arg, 1);

  XFlush (XtDisplay (label));

  }

void Clear_Area (Widget drawing_area) {

  XClearArea (XtDisplay (drawing_area), XtWindow (drawing_area),
              0, 0, 0, 0, False);

  XFlush (XtDisplay (drawing_area));

  }

int flush_FR = 1;

void Fill_Rectangle (Widget drawing_area, GC graph_context,
                            double x0, double y0, double x1, double y1) {

  int n, x, y, dx, dy;

  n = W_entry (drawing_area);

  W_map (n, &x0, &y0, x0, y0);
  W_map (n, &x1, &y1, x1, y1);

  x = rint (min (x0, x1));
  y = rint (min (y0, y1));

  dx = rint (max (x0, x1)) - x + 1;
  dy = rint (max (y0, y1)) - y + 1;

  XFillRectangle (XtDisplay (drawing_area), XtWindow (drawing_area),
                  graph_context, x, y, dx, dy);

  if (flush_FR)

    XFlush (XtDisplay (drawing_area));

  }

typedef struct {

  int x_size, y_size;

  char *data;

  } dm_bitmap;

dm_bitmap *Resize_Bitmap (dm_bitmap *bitmap, int x_size, int y_size) {

  bitmap->data = realloc (bitmap->data, x_size * y_size * sizeof (char));

  if (bitmap->data == NULL) {

    free (bitmap);

    return NULL;

    }

  bitmap->x_size = x_size;
  bitmap->y_size = y_size;

  return bitmap;

  }

dm_bitmap *Create_Bitmap (int x_size, int y_size) {

  dm_bitmap *bitmap;

  bitmap = calloc (1, sizeof (dm_bitmap));

  if (bitmap == NULL) return NULL;

  bitmap->data = NULL;

  return Resize_Bitmap (bitmap, x_size, y_size);

  }

#define dmb_pixel(bm,x,y) ((bm)->data [(x) * (bm)->y_size + (y)])

void Draw_Bitmap (Widget drawing_area, GC graph_context,
                                                 dm_bitmap *bitmap) {

  int x, y, c, c0; double x_scale, y_scale;

  if (bitmap == NULL) return;

  if (bitmap->x_size <= 0 ||
      bitmap->y_size <= 0 ||
      bitmap->data == NULL) return;

  x_scale = 1.0 / bitmap->x_size;
  y_scale = 1.0 / bitmap->y_size;

  flush_FR = 0;

  for (x = 0; x < bitmap->x_size; x++) {

    c0 = -1;

    for (y = 0; y < bitmap->y_size; y++) {

      if ((c = dmb_pixel (bitmap, x, y)) != c0)

        Colour (drawing_area, graph_context, c);

      Fill_Rectangle (drawing_area, graph_context,
                                x * x_scale, y * y_scale,
                               (x + 1) * x_scale, (y + 1) * y_scale);

      c0 = c;

      }
    }

  flush_FR = 1;

  XFlush (XtDisplay (drawing_area));

  }

#endif /* __DRAW_M_H__ */

