
/***********************************************************************}
{                                                                       }
{  draw the model                                                       }
{                                                                       }
{***********************************************************************/

#if !defined (DRAW_MODEL_C)

#define DRAW_MODEL_C

#include "draw_mod.h"

#include "draw_3d.h"

#include "grad.h"

#include "buttons.h"

#include "constant.h"

#include "global.h"


button_type model_button [] = {

  { 0.05, 0.11, 0.89, 0.95, 0  , 0.6, 0.6, 0, "B" },
  { 0.14, 0.20, 0.89, 0.95, 0.6, 0.6, 0.6, 1, "S" },
  { 0.23, 0.29, 0.89, 0.95, 0.6, 0.6, 0.6, 1, "N" },
  { 0.32, 0.38, 0.89, 0.95, 0.4, 0.4, 0.4, 1, "A" },

  { 0 }

  };

const int draw_Bond = 0, draw_Symbol = 1, draw_Number = 2, draw_Axes = 3;


  /* Draw a grey line from (x_A, y_A, z_A) -> (x_B, y_B, z_B).

     Draw a light grey gradation mark every 0.5 units.

     Draw a white mark every unit.

     */

void draw_graduated_line (double x_A, double y_A, double z_A,
                          double x_B, double y_B, double z_B) {

  int i;

  double l, m [3], x [3], dx [3];

  x [0] = x_A;
  x [1] = y_A;
  x [2] = z_A;


    /* Get the vector A -> B */

  dx [0] = x_B - x_A;
  dx [1] = y_B - y_A;
  dx [2] = z_B - z_A;


    /* Get the length and normalise the vector */

  l = normalise3 (dx);


    /* The steps between marks are 0.5 units */

  dx [0] *= 0.5;
  dx [1] *= 0.5;
  dx [2] *= 0.5;


    /* The marks are 1 / 20 units long */

  m [0] = dx [0] / 20;
  m [1] = dx [1] / 20;
  m [2] = dx [2] / 20;


    /* Draw the line */

  Begin_line ();

  Colour (0.3, 0.3, 0.3);

  G_vertex (x);

  for (i = 1; i * 0.5 <= l + 0.001; i++) {

    x [0] += dx [0];
    x [1] += dx [1];
    x [2] += dx [2];

    Colour (0.3, 0.3, 0.3);

    G_vertex3 (x [0] - m [0], x [1] - m [1], x [2] - m [2]);

    if (i % 2)

      Colour (0.75, 0.75, 0.75);

    else

      Colour (1, 1, 1);

    G_vertex3 (x [0] + m [0], x [1] + m [1], x [2] + m [2]);

    }

  End_line ();

  }


  /* Draw 6 graduated lines radiation l units along the axes from 0 */

void draw_axis_lines (double l, double x, double y, double z) {

  draw_graduated_line (x, y, z, x - l, y, z);
  draw_graduated_line (x, y, z, x, y - l, z);
  draw_graduated_line (x, y, z, x, y, z - l);

  draw_graduated_line (x, y, z, x + l, y, z);
  draw_graduated_line (x, y, z, x, y + l, z);
  draw_graduated_line (x, y, z, x, y, z + l);

  }


  /* Draw the atom and the atom label */

void draw_atom (double l, int atomic_number, int shell,

                          double x [], int connected) {

  double  d;

  char name [16], * symbol;

  if (!connected) {

      /* If the atom is not connected to anything ... */

    Colour (0, 1, 0);

    G_line3 (x [0], x [1], x [2], x [0] - l, x [1], x [2]);
    G_line3 (x [0], x [1], x [2], x [0], x [1] - l, x [2]);
    G_line3 (x [0], x [1], x [2], x [0], x [1], x [2] - l);

    G_line3 (x [0], x [1], x [2], x [0] + l, x [1], x [2]);
    G_line3 (x [0], x [1], x [2], x [0], x [1] + l, x [2]);
    G_line3 (x [0], x [1], x [2], x [0], x [1], x [2] + l);

    }


    /* Draw the label in white */

  Colour (1, 1, 1);


    /* Draw the atomic symbol */

  if (model_button [draw_Symbol].status) {

    symbol = atom_symbol (atomic_number);

    if (symbol) {

      sprintf (name, "%s", symbol);

      G_draw_string (x [0] + 0.0 * l, x [1] + 0.0 * l, x [2],
                           1.0 * l, 1.0 * l, name, "R2C0", 0);

      }
    }


    /* Draw the shell number */

  if (model_button [draw_Number].status)

    G_draw_number (x [0] + 0.0 * l, x [1] + 0.0 * l, x [2],
                         0.7 * l, 0.7 * l, "%.0f", shell, "L3D0", 0);

  }


  /* Draw the axis labels */

void draw_axis_labels (double l, double x, double y, double z) {

  double dl;

  dl = l < 0 ? -0.2 : 0.2;

  G_line3 (x + l - dl, y + dl, z, x + l + dl, y - dl, z);
  G_line3 (x + l + dl, y + dl, z, x + l - dl, y - dl, z);

  G_line3 (x, y + l, z, x - dl, y + l + dl, z);
  G_line3 (x, y + l, z, x + dl, y + l + dl, z);
  G_line3 (x, y + l - dl, z, x, y + l, z);

  G_line3 (x - dl, y + dl, z + l, x + dl, y + dl, z + l);
  G_line3 (x + dl, y + dl, z + l, x - dl, y - dl, z + l);
  G_line3 (x - dl, y - dl, z + l, x + dl, y - dl, z + l);

  }


  /* Draw the axis lines (grey) and labels (red = +, blue = -) */

void draw_axes (double l, double x, double y, double z) {

  Colour (0.5, 0.5, 0.5);

  draw_axis_lines (l, x, y, z);

  Colour (1, 0, 0);

  draw_axis_labels (3, x, y, z);

  Colour (0, 0, 1);

  draw_axis_labels (-3, x, y, z);

  }


typedef struct {

  int shells;

  char   * connected;
  double * atom;
  int    * atomic;

  } model_infot;

static model_infot model_info = { 0, NULL, NULL, NULL };

void setup_dummy () {

  static double d_atom   [] = { 0, 0, 0, 2, 0, 0, -1, -1, -1, 1, -1, 1 };
  static int    d_atomic [] = { 29, 16, 7, 8 };

  static char   d_connected [4];

  model_info.shells    = 4;
  model_info.atom      = d_atom;
  model_info.atomic    = d_atomic;
  model_info.connected = d_connected;
 
  }


  /*********************************************************************}
  {                                                                     }
  {  Redraw the model window (external)                                 }
  {                                                                     }
  {*********************************************************************/

void * redraw_model () {

  vector model;

  vector * pmodel;


  int i, j, shell, shells, shell_size, x_0, x_1, atomic_0, atom_type, changed;

  double * x;


  changed = 0;


    /* Get the model vector */

  pmodel = parameter_vector_Oi (global_data.parameters, MODEL, 0);

  if (!pmodel)

    return NULL;

  model = *pmodel;


    /* Get the indices of the atomic numbers and the coordinates */

  atomic_0 =

        parameter_index ("shell0", global_data.parameter_name_list, NULL);

  x_0 = parameter_index ("x0", global_data.parameter_name_list, NULL);
  x_1 = parameter_index ("x1", global_data.parameter_name_list, NULL);


    /* Get the shell size and the true offsets */

  shell_size = x_1 - x_0;

  atomic_0 -= x_0;

  x_0--;


    /* Get the number of shells */

  shells = (model.rows - x_0 - 3) / shell_size + 1;


    /* Allocate space for the temporary arrays */

  if (shells != model_info.shells || !model_info.connected
                                  || !model_info.atom
                                  || !model_info.atomic) {

    changed = 1;

    free_check (model_info.connected);
    free_check (model_info.atom);
    free_check (model_info.atomic);

    model_info.connected = calloc_check (shells, sizeof (char));
    model_info.atom      = calloc_check (shells, sizeof (double) * 3);
    model_info.atomic    = calloc_check (shells, sizeof (int));

    model_info.shells = shells;

    }


    /* Copy the model into the arrays, comparing with the old values */

  for (i = x_0, shell = 0; shell < shells; i += shell_size, shell++) {

    x = & (v_el (model, i + 0));

    atom_type = v_el (model, i + atomic_0);

    if (!changed)

      changed = model_info.atom [shell * 3 + 0] != x [0] ||
                model_info.atom [shell * 3 + 1] != x [1] ||
                model_info.atom [shell * 3 + 2] != x [2] ||
                model_info.atomic [shell] != atom_type;

    if (changed) {

      model_info.atom [shell * 3 + 0] = x [0];
      model_info.atom [shell * 3 + 1] = x [1];
      model_info.atom [shell * 3 + 2] = x [2];

      model_info.atomic [shell] = atom_type;

      }

    model_info.connected [shell] = 0;

    }


    /* Signal to redraw the window */

  if (changed)

    invalidatewindow (model_Gid);


    /* Return */

  return &model_info;

  }


  /*********************************************************************}
  {                                                                     }
  {  Redraw the model window (internal)                                 }
  {                                                                     }
  {*********************************************************************/

int draw_atoms () {

  double x [3], l;

  char c [32];

  int i, j;


    /* Don't depth-cue if rotation is turned off */

  G_colour = 0;


    /* Set the window dimensions */

  Ortho (-5.0, 5.0, -5.0, 5.0, -10.0, 10.0);


    /* Clear the window */

  Clear_RGB (0, 0, 0);


    /* Draw the axes */

  if (model_button [draw_Axes].status)

    draw_axes (6, 0, 0, 0);


    /* Depth-cue even if rotation is turned off */

  G_colour = 1;


    /* Draw a line to all atoms within covalent bond distance + 20% */

  Ortho (-5.0, 5.0, -5.0, 5.0, -10.0, 10.0);

  for (i = 0; i < model_info.shells; i++)

    model_info.connected [i] = 0;

  for (i = 0; i < model_info.shells; i++) {

    for (j = i + 1; j < model_info.shells; j++) {

        /* Get the distance from atom i to atom j */

      x [0] = model_info.atom [i * 3 + 0] - model_info.atom [j * 3 + 0];
      x [1] = model_info.atom [i * 3 + 1] - model_info.atom [j * 3 + 1];
      x [2] = model_info.atom [i * 3 + 2] - model_info.atom [j * 3 + 2];

      l = length3 (x);


        /* The atoms are bonded if ... */

      if (l <= (covalent_radius (model_info.atomic [i]) +
                covalent_radius (model_info.atomic [j])) * 1.2) {

        model_info.connected [i] = model_info.connected [j] = 1;


          /* Draw a green line between the atoms */

        Colour (0, 1, 0);

        G_line (&(model_info.atom [i * 3]), &(model_info.atom [j * 3]));


          /* Draw the distance in angstrom */

        if (model_button [draw_Bond].status) {

          Colour (0, 1, 1);

          sprintf (c, "%.3f", l);

          G_draw_string_v (&(model_info.atom [i * 3]), &(model_info.atom [j * 3]),
                           0.135, 0.135, c);

          }
        }
      }


      /* Draw atom i */

    Colour (0, 1, 0);

    draw_atom (0.2, model_info.atomic [i], i, &(model_info.atom [i * 3]),
                                              model_info.connected [i]);

    }


    /* Draw the buttons controlling the display of the text */

  Ortho (0.0, 1.0, 0.0, 1.0, -10.0, 10.0);

  draw_buttons (model_button);


    /* Swap the buffers */

  swapbuffers ();

  return 0;

  }


  /*********************************************************************}
  {                                                                     }
  {  Set up the model window                                            }
  {                                                                     }
  {*********************************************************************/


long atomwin_CB (long Gid, long msg, long mp) {

  static int mouse_button [2] = { 0, 0 };

  static double o_mouse [2] = { 0, 0 };

  int i;

  model_Gid = Gid;

  switch (msg) {

    case CB_OPEN:

      return 0;


    case CB_PAINT:

      draw_atoms ();

      return 0;


    case CB_MOUSE_DN: {

      double x, y;

      mouse (&x, &y);

      if (mp == 1) {

        if (check_buttons (model_button, x, y))

          invalidatewindow (model_Gid);

        }
      else {

        o_mouse [0] = x;
        o_mouse [1] = y;

        }
 
      mouse_button [mp - 1] = 1;

      return 0;

      }


    case CB_MOUSE_UP:

      mouse_button [mp - 1] = 0;

      return 0;


    case CB_MOUSE_MO: {

      double x, y;

      mouse (&x, &y);

      if (mouse_button [1]) {

        vector4 rot;

        rot = o_vector3;

        v_el (rot, 0) = (o_mouse [1] - y) * M_PI / 2;
        v_el (rot, 1) = (x - o_mouse [0]) * M_PI / 2;

        G_orientation = mul_matrix4 (orientation (rot), G_orientation);

        o_mouse [0] = x;
        o_mouse [1] = y;

        invalidatewindow (model_Gid);

        }

      return 0;

      }


    case CB_CLOSE:

      model_Gid = 0;

      return 0;

    }
  }


int setup_model_window () {

  return openwindow ("model", OW_SQUARE, atomwin_CB);

  }


#endif /* DRAW_MODEL_C */

