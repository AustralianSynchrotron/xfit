
/***********************************************************************}
{                                                                       }
{  draw the model                                                       }
{                                                                       }
{***********************************************************************/

#if !defined (DRAW_MODEL_C)

#define DRAW_MODEL_C

#include "xdraw_mo.h"

#include "constant.h"

#include "global.h"

static ModelData Data = { 0, NULL };


  // Redraw the model window (external)

ModelData * redraw_model () {

  vector model;

  vector * pmodel;


  int i, shell, shells, shell_size, x_0, x_1, atomic_0, atom_type, changed;

  double * x;


  changed = 0;


    // Get the model vector

  pmodel = parameter_vector_Oi (global_data.parameters, MODEL, 0);

  if (!pmodel)

    return NULL;


  model = *pmodel;


    // Get the indices of the atomic numbers and the coordinates

  atomic_0 =

        parameter_index ("shell0", global_data.parameter_name_list, NULL);

  x_0 = parameter_index ("x0", global_data.parameter_name_list, NULL);
  x_1 = parameter_index ("x1", global_data.parameter_name_list, NULL);


    // Get the shell size and the true offsets 

  shell_size = x_1 - x_0;

  atomic_0 -= x_0;

  x_0--;


    // Get the number of shells

  shells = (model.rows - x_0 - 3) / shell_size + 1;


    // Allocate space for the data

  if (shells != Data.atoms) {

    changed = 1;

    free_check (Data.atom);

    Data.atom = calloc_check (shells, sizeof (AtomData));
    
    Data.atoms = shells;

    }


    // Copy the model, comparing with the old values

  for (i = x_0, shell = 0; shell < shells; i += shell_size, shell++) {

    x = & (v_el (model, i + 0));

    atom_type = v_el (model, i + atomic_0);

    if (!changed)

      changed = Data.atom [shell].x != x [0] ||
                Data.atom [shell].y != x [1] ||
                Data.atom [shell].z != x [2] ||
                Data.atom [shell].number != atom_type;

    if (changed) {

      Data.atom [shell].x = x [0];
      Data.atom [shell].y = x [1];
      Data.atom [shell].z = x [2];

      if (Data.atom [shell].number != atom_type) {

        Data.atom [shell].number = atom_type;

        Data.atom [shell].covalent = covalent_radius (atom_type);

        Data.atom [shell].symbol = atom_symbol (atom_type);

        }
      }
    }


    // Signal to redraw the window

  if (changed)

    return &Data;

  return NULL;

  }


#endif // DRAW_MODEL_C

