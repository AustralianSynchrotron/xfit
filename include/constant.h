/***********************************************************************}
{                                                                       }
{  This file contains the definitions and declarations of data          }
{  structures containing constants such as X-ray edge types and atom    }
{  numbers and names and the routines used to access them.              }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1993.             }
{                                                                       }
{***********************************************************************/

#if !defined (__CONSTANTS_H__)

#define __CONSTANTS_H__

#include "exafs.h"

#include "header.h"

#define AVOGADRO        6.02219e23
#define FARADAY         96486.70
#define PLANCK          6.626196e-34
#define LIGHT_SPEED     2.9979250e8
#define PROTON_MASS     1.672614e-27
#define NEUTRON_MASS    1.674920e-27
#define ELECTRON_MASS   9.109558e-31
#define ELECTRON_CHARGE 1.6021917e-19
#define ELECTRON_RADIUS 2.817939e-15
#define BOHR_RADIUS     5.29177249e-11
#define BOHR_MAGNETON   9.274096e-24
#define GAS_CONSTANT    8.31434
#define BOLTZMANN       1.380622e-23

#define VACUUM_PERMITIVITY  8.8541853e-12
#define VACUUM_PERMEABILITY (4 * M_PI * 1e-7)

#define HARTREE         27.211396
#define RYDBERG         13.605698
#define BOHR            0.529177249

typedef struct {

  short code;

  char  * name;

  } edge_type;

typedef struct {

  short number;

  char  symbol [3];

  short unique;

  char  * name;

  } atom_type;


char * X_ray_edge_symbol (int edge);

int X_ray_edge_code (char name []);

char * atom_symbol (int Z);

char * atom_name (int Z);

int atomic_number (char name []);

typedef struct {

  short number;

  char symbol [3];

  double mass, VdW, atomic, metallic, covalent,

         edge [4], E0 [4], C [5], D [5];

  } X_ray_absorption_parameters_type;

  /* 24 * 8 * 96 = 18432 */


double VdW_radius (int Z);

double covalent_radius (int Z);

double atomic_mass (int Z);

double ionisation_energy (int Z, int edge);

double edge_energy (int Z, int edge);

double Klein_Nishina_absorbance (double energy);

double Victoreen_absorbance (int Z, double energy);

int Victoreen_coefficients (int Z, int edge,

                            double * C_b, double * D_b,
                            double * C_a, double * D_a);

double Victoreen (double E, double C_b, double D_b, double C_a, double D_a);

int find_edge_type (double edge_E, int * Z, int * edge);

#endif /* __CONSTANTS_H__ */

