/***********************************************************************}
{                                                                       }
{  Routines used by EXAFS programs                                      }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__EXAFS_H__)

#define __EXAFS_H__

#include "header.h"

#if !defined (HARTREE)

#define HARTREE  27.211396
#define RYDBERG  13.605698

#define BOHR     0.529177249

#endif

#define _HC_     12398.5471
#define _KEV_    0.5123143

#define _OLD_D_  1.92
#define _OLD_HC_ 12399.55

double monochromator_energy (double theta, double d);

double monochromator_angle (double energy, double d);

double old_monochromator_energy (double theta, double d);

double old_monochromator_angle (double energy, double d);

#define k_EXAFS(E) ((E) <= 0 ? 0 : _KEV_ * sqrt (E))

#define E_EXAFS(k) ((k) * (k) / (_KEV_ * _KEV_))

#define dk_dE_EXAFS(E) ((E) <= 0 ? 0 : 0.5 * _KEV_ / sqrt (E))

double wave_vector (double energy, double E0);

double k_energy (double k, double E0);

#endif /* __EXAFS_H__ */
