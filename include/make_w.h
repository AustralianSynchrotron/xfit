/* Copyright (C) Paul J. Ellis, University of Sydney, 1993. */

#if !defined (__MAKE_WINDOWS_H__)

#define __MAKE_WINDOWS_H__

#if !defined (M_MAX_OBJECTS)

#define M_MAX_OBJECTS 64

#endif

void Make_Shell (Display *display, char shell_name [],
                         char widget_name [], Widget *widget) {

    /* Create a new toplevel shell. */

  Widget toplevel_shell, widget0;

  if (widget == NULL) widget = &widget0;

  toplevel_shell = XtAppCreateShell (shell_name, "Shell",
                                    applicationShellWidgetClass,
                                    display, NULL, 0);
  *widget = NULL;

  if (MrmFetchWidget (s_MrmHierarchy, widget_name,
                      toplevel_shell, widget, &class) != MrmSUCCESS)

    printf ("can't fetch widget: %s::%s\n", shell_name, widget_name);

  XtManageChild (*widget);

  XtRealizeWidget (toplevel_shell);

  }

#define Make_Drawing_Shell Make_Shell

int    W_object_tag [M_MAX_OBJECTS];
Widget W_object     [M_MAX_OBJECTS];

int W_object_entry (int tag) {

  static int first = TRUE;

  int W_number;

  if (first) {

    for (W_number = 0; W_number < M_MAX_OBJECTS; W_number++)
      W_object_tag [W_number] = MAXINT;

    first = FALSE;

    }

  for (W_number = 0; W_number < M_MAX_OBJECTS; W_number++)
    if (tag == W_object_tag [W_number])
      return W_number;

  for (W_number = 0; W_number < M_MAX_OBJECTS; W_number++)
    if (W_object_tag [W_number] == MAXINT) {

      W_object_tag [W_number] = tag;

      return W_number;

      }

  return 0;

  }

#define Object xxxxObjectxxxx

Widget Object (int tag) {

  return W_object [W_object_entry (tag)];

  }

static void Create_Object (Widget w, int *tag, unsigned long *reason);

static void Create_Procedure (Widget w, int *tag, unsigned long *reason) {

  W_object [W_object_entry (*tag)] = w;

  /* printf (" Create_Procedure: reason = %lu\n", *reason); */

  Create_Object (w, tag, reason);

  }

#endif /* __MAKE_WINDOWS_H__ */
