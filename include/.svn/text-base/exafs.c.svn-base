/***********************************************************************}
{                                                                       }
{  Routines used by EXAFS programs                                      }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__EXAFS_C__)

#define __EXAFS_C__

#include "exafs.h"

double monochromator_energy (double theta, double d) {

  return _HC_ / (2 * d * sin (theta * M_PI / 180.0));

  }

double monochromator_angle (double energy, double d) {

  return 180.0 * asin (_HC_ / (2 * d * energy)) / M_PI;

  }

double old_monochromator_energy (double theta, double d) {

  return _OLD_HC_ / (2 * d * sin (theta * M_PI / 180.0));

  }

double old_monochromator_angle (double energy, double d) {

  return 180.0 * asin (_OLD_HC_ / (2 * d * energy)) / M_PI;

  }

double wave_vector (double energy, double E0) {

  return energy <= E0 ? 0 : _KEV_ * sqrt (energy - E0);

  }

double k_energy (double k, double E0) {

  return k * k / (_KEV_ * _KEV_) + E0;

  }

#endif /* __EXAFS_C__ */
