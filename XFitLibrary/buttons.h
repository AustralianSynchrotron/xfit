/***********************************************************************}
{                                                                       }
{  buttons                                                              }
{                                                                       }
{***********************************************************************/

#if !defined (BUTTONS_H)

#define BUTTONS_H

typedef struct {

  double x_min, x_max, y_min, y_max,

         R, G, B;

  int status;

  char * text;

  } button_type;

void draw_button (button_type B);

void draw_buttons (button_type * button);

int check_button (button_type * B, double x, double y);

int check_buttons (button_type * button, double x, double y);

#endif /* BUTTONS_H */

