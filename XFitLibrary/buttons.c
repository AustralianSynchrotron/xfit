/***********************************************************************}
{                                                                       }
{  buttons                                                              }
{                                                                       }
{***********************************************************************/

#if !defined (BUTTONS_C)

#define BUTTONS_C


#include "buttons.h"

#include "draw_3d.h"


void draw_button (button_type B) {

  int G_rotate_o;


    /* Turn off the rotation */

  G_rotate_o = G_rotate;

  G_rotate = 0;


    /* Set the colour */

  Colour (B.R, B.G, B.B);


    /* Draw the border */

  G_line2 (B.x_min, B.y_min, B.x_max, B.y_min);
  G_line2 (B.x_max, B.y_min, B.x_max, B.y_max);
  G_line2 (B.x_max, B.y_max, B.x_min, B.y_max);
  G_line2 (B.x_min, B.y_max, B.x_min, B.y_min);


    /* Draw the text */

  G_draw_string ((B.x_min + B.x_max) / 2, (B.y_min + B.y_max) / 2, 0,
                 (B.x_max - B.x_min) / 2, (B.x_max - B.x_min) / 2,
                  B.text, "C0C0", 0);


    /* Restore the rotation */

  G_rotate = G_rotate_o;

  }

void draw_buttons (button_type * button) {

  for (; button->text; button++)

    draw_button (*button);

  }

int check_button (button_type * B, double x, double y) {

  if (x >= B->x_min && x <= B->x_max && y >= B->y_min && y <= B->y_max) {

    B->status = !B->status;

    return 1;

    }

  return 0;

  }

int check_buttons (button_type * button, double x, double y) {

  int changed;

  for (changed = 0; button->text; button++)

    changed |= check_button (button, x, y);

  return changed;

  }


#endif /* BUTTONS_C */

