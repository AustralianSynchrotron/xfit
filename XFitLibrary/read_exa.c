/***********************************************************************}
{                                                                       }
{  This file contains the routine used to read in experimental exafs.   }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__EXPERIMENT_C__)

#define __EXPERIMENT_C__

#include "read_exa.h"
#include "misc_com.h"

#include "console.h"


#define PE_next ASCII_next

#define PE_next_word ASCII_next_word

int parse_exafs_commands (char command [], exafs_data_type * E) {

    /* The valid commands in an exafs file are:

       EZERO / E0 / THRESHOLD       <threshold energy> <unit>

       EZERO / E0 / THRESHOLD       <threshold energy in eV>

       EDGE                         <absorber> <edge type>

       EDGE                         <absorber> <edge type>

       EDGE                         <edge type>

       EDGE                         <absorber>

       POLARISATION / POLARIZATION  <electric field vector>

       END

       */

  char * word;

  double x, y, z, xn, yn, zn, el;

  PE_next = NULL;

  PE_next_word ();

  PE_next = command;

  for (word = PE_next_word (); word; word = word ? word : PE_next_word ()) {

    if (match_l (word, "EZERO", "E0", "THRESHOLD", 0)) {

      if (word = PE_next_word ())

        if (sscanf (word, " %lf", &(E->E0)) > 0)

          if (word = PE_next_word ()) {

            if (match (word, "RYD")) E->E0 *= RYDBERG;

            if (match (word, "HAR")) E->E0 *= HARTREE;

            if (match_l (word, "EV", "RYD", "HAR", 0)) word = NULL;

            }

      continue;

      }

    if (match (word, "EDGE")) {

      int Z [2], X [2];

      Z [0] = Z [1] = X [0] = X [1] = 0;

      if (word = PE_next_word ()) {

        Z [0] = atomic_number (word);
        X [0] = X_ray_edge_code (word);

        if (Z [0] || X [0])

          if (word = PE_next_word ()) {

            Z [1] = atomic_number (word);
            X [1] = X_ray_edge_code (word);

            }
         }

      if ((Z [1] && X [0]) || (X [1] && Z [0]))

        word = NULL;

      if (X [0] || X [1])

        E->edge = X [X [1] && Z [0]];

      if ((Z [0] || Z [1]) && (Z [1] || !X [0] || X [1]))

        E->absorber = Z [!Z [0] || (X [0] && !X [1])];

      continue;

      }

    if (match_l (word, "POLARISATION", "POLARIZATION", 0)) {

      if (word = PE_next_word ())

        if (sscanf (word, " %lf", &x) > 0) {

          double l;

          y = z = xn = yn = zn = el = 0;

          if (word = PE_next_word ())

            if (sscanf (word, " %lf", &y) > 0)

              if (word = PE_next_word ())

                if (sscanf (word, " %lf", &z) > 0)

                  if (word = PE_next_word ())

                    if (sscanf (word, " %lf", &xn) > 0)

                      if (word = PE_next_word ())

                        if (sscanf (word, " %lf", &yn) > 0)

                          if (word = PE_next_word ())

                            if (sscanf (word, " %lf", &zn) > 0)

                              if (word = PE_next_word ())

                                if (sscanf (word, " %lf", &el) > 0)

                                  word = NULL;

          l = sqrt (xn * xn + yn * yn + zn * zn);

          if (l) {

            xn /= l;
            yn /= l;
            zn /= l;

            }

          l = sqrt (x * x + y * y + z * z);

          if (l > 0) {

            x /= l;
            y /= l;
            z /= l;

            E->polarisation = realloc (E->polarisation, 3 * sizeof (double));
            E->normal       = realloc (E->normal,       3 * sizeof (double));
            E->ellipticity  = realloc (E->ellipticity, sizeof (double));

            E->O = realloc (E->O, sizeof (double));

            if (E->polarisation) {

              E->polarisation [0] = x;
              E->polarisation [1] = y;
              E->polarisation [2] = z;

              E->normal [0] = xn;
              E->normal [1] = yn;
              E->normal [2] = zn;

              E->ellipticity [0] = el;

              E->O [0] = 1;

              E->shells = 1;

              }
            }
          }

      continue;

      }

    word = NULL;

    }

  return 1;

  }

int exafs_energy_column (ascii_data_type * A, double * scale) {

    /* Find the energy column in the ascii data file.

       energy column      scale:

         energy           size of the energy entries in eV

         k                -1

         missing          unchanged

         */

  static char * heading_l [] = { "EV", "ELEC", "HART", "RYDB", "K", 0 };

  static double scale_l []   = {   1,      1, HARTREE, RYDBERG,  -1 };

  int c, unit;

  if ((c = ascii_file_column_l (heading_l, A, &unit)) >= 0)

    *scale = scale_l [unit];

  else

    cwarn ("%s is missing the energy", A->filename);

  return c;

  }

int exafs_exafs_column (ascii_data_type * A) {

    /* Find the absorbance (exafs) column in the ascii data file. */

  static char * heading_l [] = { "EXAF", "XAFS", "ABS", "CHI", 0 };

  int c;

  if ((c = ascii_file_column_l (heading_l, A, NULL)) < 0)

    cwarn ("%s is missing the XAFS", A->filename);

  return c;

  }

int exafs_esd_column (ascii_data_type * A) {

    /* Find the esd (sigma) column in the ascii data file. */

  static char * heading_l [] = { "SIGM", "ESD", 0 };

  return ascii_file_column_l (heading_l, A, NULL);

  }

int exafs_smooth_column (ascii_data_type * A) {

    /* Find the smoothed exafs column in the ascii data file. */

  static char * heading_l [] = { "SMOO", "FEXA", 0 };

  return ascii_file_column_l (heading_l, A, NULL);

  }


int defaults (exafs_data_type * E) {

    // Set the default polarisation and O 

  E->shells = 0;

  E->polarisation = realloc (E->polarisation, 3 * sizeof (double));
  E->normal       = realloc (E->normal,       3 * sizeof (double));

  E->ellipticity  = realloc (E->ellipticity, sizeof (double));

  E->O = realloc (E->O, sizeof (double));

  if (E->polarisation && E->normal && E->ellipticity && E->O) {

    E->polarisation [0] = 0;
    E->polarisation [1] = 0;
    E->polarisation [2] = 0;

    E->normal [0] = 0;
    E->normal [1] = 0;
    E->normal [2] = 0;

    E->ellipticity [0] = 0;

    E->O [0] = 1;

    E->shells = 1;

    }

  return 1;
  
  }


  /* Attempt to read the XAFS data as a general data file */

extern void *  ReadDData   (char * filename);
extern void    FreeDData   (void * D);
extern int     DDataColumn (void * D,
                                     const char * ColName,
                                     double ** ColD);
extern char *  DDataEntry  (void * D,
                                     const char * Section,
                                     const char * Key);


int read_exafs_D (char filename [], exafs_data_type * E) {

  int count, c, c0, energy, data, sigma, smooth;

  double energy_scale, * Energy, * Data, * Sigma, * Smooth;

  void * D;

  char * word;
  

  D = ReadDData (filename);

  if (!D)

    return 0;


    /* Get the energy, exafs, sigma and smoothed exafs columns */

  energy_scale = 0;

  c0 = 0;

  energy = DDataColumn (D, "EV", &Energy);

  if (!energy)

    energy = DDataColumn (D, "ENERGY", &Energy);
  
  if (!energy) {

    energy = DDataColumn (D, "K", &Energy);

    for (; c0 < energy; c0++)

      if (Energy [c0] > 0)

        break;

    energy_scale = -1;

    }

  data  = DDataColumn (D, "XAFS", &Data);

  if (!data)
  
    data  = DDataColumn (D, "EXAFS", &Data);

  if (!data)
  
    data  = DDataColumn (D, "CHI", &Data);

  sigma  = DDataColumn (D, "SIGMA", &Sigma);

  if (!sigma)
  
    sigma  = DDataColumn (D, "ESD", &Sigma);

  smooth  = DDataColumn (D, "SMOOTH", &Smooth);

  if (!smooth)
  
    smooth  = DDataColumn (D, "SMOOTHED", &Smooth);


  if (energy && data) {

    count = energy;

    if (count > data)

      count = data;

    if (count > sigma || count > smooth)

      sigma = smooth = 0;

    else

      sigma = smooth = count;


    if (count > c0) {

        /* Set the default polarisation and O */

      defaults (E);

      E->ascii_file.filename = strdup (filename);


        /* Allocate space for the columns */

      E->energy   = calloc_check (count - c0, sizeof (double));
      E->chi_exp  = calloc_check (count - c0, sizeof (double));

      E->sigma = E->schi_exp = NULL;

      if (sigma && smooth) {
        
        E->sigma    = calloc_check (sigma - c0, sizeof (double));
        E->schi_exp = calloc_check (smooth - c0, sizeof (double));

        }


        /* Copy the columns */

      for (c = c0; c < count; c++) {

        if (energy_scale == -1) 

          E->energy [c - c0] = E_EXAFS (Energy [c]);

        else

          E->energy [c - c0]  = Energy [c];

        E->chi_exp [c - c0] = Data [c];

        if (sigma && smooth) {

          E->sigma [c - c0] = Sigma [c];
          E->schi_exp [c - c0] = Smooth [c];
  
          }
        }


        /* absorber, edge, E0 */

      word = DDataEntry (D, "SPLINE", "ABSORBER");

      if (word)
      
        E->absorber = atomic_number (word);

      word = DDataEntry (D, "XFIT", "ABSORBER");

      if (word)
      
        E->absorber = atomic_number (word);

      word = DDataEntry (D, "SPLINE", "EDGE");

      if (word)
      
        E->edge = X_ray_edge_code (word);

      word = DDataEntry (D, "XFIT", "EDGE");

      if (word)
      
        E->edge = X_ray_edge_code (word);

      word = DDataEntry (D, "SPLINE", "E0");

      if (word && energy_scale >= 0)
      
        sscanf (word, " %lf", &E->E0);

      word = DDataEntry (D, "XFIT", "E0");

      if (word && energy_scale >= 0) {

        E->E0 = 0;
        
        sscanf (word, " %lf", &E->E0);

        }


      FreeDData (D);

      return E->N_exp = count - c0;

      }
    }

  FreeDData (D);

  return 0;

  }


int read_exafs_file (char filename [], exafs_data_type * E) {

  int i0, i, energy, exafs, sigma, smooth;

  double energy_scale;

  ascii_data_type * A;
  char *nfilename;

  A = &E->ascii_file;


    /* Search for the file */

  nfilename = find_file (filename);

  if (!nfilename)

    return 0;


    /* Try to read the file as general data */

  i = read_exafs_D (nfilename, E);

  if (i)

    return i;


    /* Try to read the file as an old Xfit file */

  i = read_ascii_data_file (nfilename, A);

  if (i <= 0)

    return i;


    /* Set the default polarisation and O */

  defaults (E);


    /* Parse the commands */

  parse_exafs_commands (A->command, E);


    /* Get the energy, exafs, sigma and smoothed exafs columns */

  energy = exafs_energy_column (A, &energy_scale);

  exafs  = exafs_exafs_column (A);

  sigma  = exafs_esd_column (A);

  smooth = exafs_smooth_column (A);


    /* Check we have at least energy and exafs */

  if (energy < 0 || exafs < 0) return 0;


    /* If the energy scale is k, find first k > 0 */

  i0 = 0;

  if (energy_scale < 0)

    for (; i0 < A->entries; i0++)

      if (A->column [energy][i0] > 0) break;


    /* Allocate space for the columns */

  E->energy  = calloc_check (A->entries - i0 + 1, sizeof (double));
  E->chi_exp = calloc_check (A->entries - i0 + 1, sizeof (double));

  E->sigma = sigma >= 0 ? calloc_check (A->entries - i0 + 1,
                                              sizeof (double)) : NULL;

  E->schi_exp = smooth >= 0 ? calloc_check (A->entries - i0 + 1,
                                              sizeof (double)) : NULL;


    /* Copy the columns */

  for (i = i0; i < A->entries; i++) {

    E->energy [i - i0] = energy_scale < 0
                            ? E_EXAFS (A->column [energy][i])
                            : (A->column [energy][i] * energy_scale);

    E->chi_exp [i - i0] = A->column [exafs][i];

    if (sigma >= 0) E->sigma [i - i0] = A->column [sigma][i];

    if (smooth >= 0) E->schi_exp [i - i0] = A->column [smooth][i];

    }


    /* Free the memory allocated for the original data. */

  for (i = 0; i < A->columns; i++) {

    free_check (A->column [i]);

    A->column [i] = NULL;

    }

  return E->N_exp = A->entries - i0;

  }

#endif /* __EXPERIMENT_C__ */
