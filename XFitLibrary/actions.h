/***********************************************************************}
{                                                                       }
{  This file contains the definitions of the actions for xfit.y         }
{                                                                       }
{***********************************************************************/

#if !defined (__ACTIONS_H__)

#define __ACTIONS_H__

#include "console.h"
#include "expressi.h"
#include "global.h"
#include "refine_c.h"
#include "montecar.h"
#include "list_com.h"
#include "misc_com.h"



double d;

#define print_act(file,set,u) print_command (*global_data.exafs, \
                                                (file), (set), (u))

#define assign(id,d) { assignment_command (global_data.parameters,        \
                                                              (id), (d)); }

long   leave [8], i;
double x     [8];

node *empty_node_list [1] = { NULL };

#define v_value(n) vector_value (n, global_data.parameters)
#define s_value(n) scalar_value (n, global_data.parameters)

#define add_cons(node) add_restraint (&(global_data.restraints->constraint),\
                                        node);
#define add_rest(node) add_restraint (&(global_data.restraints->restraint),\
                                        node);

#define exafs_act(file)  exafs_command (global_data.parameters, \
                                        global_data.exafs, (file))

#define restore_act      restore_command (global_data.parameters)

#define refine_act(n,list) refine_command ((list), \
                   (n), global_data.restraints, \
                   global_data.parameters, \
                   global_data.parameter_name_list, \
                   0);

#define montecarlo_act(N,n,list,too) montecarlo_command (list, \
                   too, \
                   N, n, global_data.restraints, \
                   global_data.parameters, global_data.exafs, \
                   global_data.parameter_name_list);

#define delete_act(type,i_from,i_to) \
                 delete_command ((type), (i_from), (i_to), \
                 global_data.parameters, \
                 global_data.exafs, \
                 global_data.restraints);

#define list_act(type,i_from,i_to) \
                 list_command ((type), (i_from), (i_to), \
                 global_data.parameters, \
                *global_data.exafs, \
                 global_data.restraints, \
                 global_data.parameter_name_list);

#define save_act(filename,type,i_from,i_to) \
                 save_command ((type), (i_from), (i_to), (filename), \
                 global_data.parameters, \
                *global_data.exafs, \
                 global_data.restraints, \
                 global_data.parameter_name_list);

#define clean_act  clean_command ();

#endif /* __ACTIONS_H__ */
