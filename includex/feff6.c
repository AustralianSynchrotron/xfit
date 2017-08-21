/***********************************************************************}
{                                                                       }
{  This file contains routines used to interface to FEFF 6.01           }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__FEFF6_C)

#define __FEFF6_C

#include "feff6.h"

#include "console.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char * FEFF6_name (char format [], int P, int A, double C []) {

  static char name [32];

  return name;

  }


  /*********************************************************************}
  {                                                                     }
  {  Given the following numbers (in the indicated ranges) :            }
  {                                                                     }
  {      number:      0 .. 719                                          }
  {      symmetry:    0 .. 989                                          }
  {                                                                     }
  {      x:         -45 .. 44                                           }
  {      y:         -45 .. 44                                           }
  {      z:         -45 .. 44                                           }
  {                                                                     }
  {  this routine generates a unique 6-character ASCII atom label       }
  {                                                                     }
  {  Avoid using :   32 (SPACE)                                         }
  {                  34 (")                                             }
  {                  39 (')                                             }
  {                  44 (COMMA)                                         }
  {                 127 (BACKSPACE)                                     }
  {                                                                     }
  {*********************************************************************/

char * FEFF6_label (int number, int symmetry, int x, int y, int z) {

  static char label [7];

  int i;

  number   = min (max (number, 0), 719);
  symmetry = min (max (symmetry, 0), 989);

  x = min (max (x, -45), 44);
  y = min (max (y, -45), 44);
  z = min (max (z, -45), 44);

  label [0] = 33 + number % 90;
  label [1] = 33 + symmetry % 90;
  label [2] = 33 + (number / 90) + (symmetry / 90) * 8;

  label [3] = x + 78;
  label [4] = y + 78;
  label [5] = z + 78;

  for (i = 0; i < 6; i++) {

    label [i] += (label [i] >= '\"');
    label [i] += (label [i] >= '\'');
    label [i] += (label [i] >= ',');
    label [i] += (label [i] >= '\\');

    }

  label [6] = 0;

  return label;

  }


  /*********************************************************************}
  {                                                                     }
  {  Given a label generated by FEFF6_label, this routine will          }
  {  translate it back into the original numbers                        }
  {                                                                     }
  {*********************************************************************/

int translate_FEFF6_label (char label [], int * number,

                           int * symmetry, int * x, int * y, int * z) {

  int u [6], i;

  if (!strncmp (label, "      ", 6))

    return 0;

  for (i = 0; i < 6; i++) {

    u [i] = (unsigned) (label [i]);

    u [i] -= (u [i] > '\\');
    u [i] -= (u [i] > ',');
    u [i] -= (u [i] > '\'');
    u [i] -= (u [i] > '\"');

    }

  if (number)

    *number   = u [0] - 33 + ((u [2] - 33) % 8) * 90;

  if (symmetry)

    *symmetry = u [1] - 33 + ((u [2] - 33) / 8) * 90;

  if (x)

    *x = u [3] - 78;

  if (y)

    *y = u [4] - 78;

  if (z)

    *z = u [5] - 78;

  return 1;

  }


  /*********************************************************************}
  {                                                                     }
  {  Find the instance "instance" of the atom with atomic number ZC.    }
  {                                                                     }
  {  This is the central atom.                                          }
  {                                                                     }
  {  If ZC is 0, use the first atom.                                    }
  {                                                                     }
  {  If the instance does not exist, return 0.                          }
  {                                                                     }
  {*********************************************************************/

int rationalise_FEFF6_potentials (char name [],

                                   double ZC, int instance,

                                   int shells, double potential [],

                                               double Z []) {

  int shell0, shell, shellC, potentials;

  char * done, temp [256];

  if (shells <= 256)

    done = temp;

  else

    done = calloc_check (shells, sizeof (char));

  ZC = floor (ZC + 0.5);

  for (shell = 0; shell < shells; shell++)

    done [shell] = 0;


    /* Find the central atom */

  for (shellC = 0; ZC && shellC < shells; shellC++)

    if (Z [shellC] == ZC) {

      if (instance <= 0)

        break;

      instance--;

      }

  if (instance != 0 || shellC >= shells) {

    if (done != temp)
    
      free_check (done);

    return 0;

    }


    /* Change the potentials to be 0 = central, 1, 2, 3 .... */

  potential [shellC] = 0;

  done [shellC] = 1;

  for (potentials = 1, shell0 = 0; shell0 < shells; shell0++)

    if (!done [shell0]) {

      for (shell = shells - 1; shell >= shell0; shell--)

        if (!done [shell] &&
             potential [shell] == potential [shell0] &&
             Z [shell] == Z [shell0]) {

          potential [shell] = potentials;

          done [shell] = 1;

          }

      potentials++;

      }

  if (done != temp)
  
    free_check (done);

  return potentials;

  }


  /*********************************************************************}
  {                                                                     }
  {  Create a FEFF6 input file.                                         }
  {                                                                     }
  {*********************************************************************/

int make_FEFF6 (char name [],

                 char path [],

                 double ZC, int instance, int hole,

                 double polar  [],
                 double normal [],
                 double ellipticity,

                 int    max_legs,
                 double max_length,

                 double dE,
                 double dEi,

                 double filter_CW,
                 double filter_PW,

                 int    shells,
                 double potential [],
                 double Z         [],
                 double N         [],
                 double v         [][3],
                 double s         [],

                 double S02) {

  FILE * feff_inp;

  int shell0, shell, shellC, ipotential, potentials, number;

  double * Z_potential, d, current_d, next_d;

  for (shell = 0; shell < shells; shell++)

    Z [shell] = floor (Z [shell] + 0.5);

  potentials = rationalise_FEFF6_potentials (name, ZC, instance, shells,
                                                       potential, Z);

  if (potentials == 0)

    return 0;


    /* Open the file */

  strcat (path, "\\feff.inp");

  feff_inp = fopen (path, "w");

  path [strlen (path) - 9] = 0;

  if (feff_inp == NULL) {

    cwarn (" make_FEFF6 (%s): can't open %s\\feff.inp",  name, path);

    return 0;

    }


    /* Check the polarisation information */

  if (polar)

    if (normalise3 (polar) == 0)

      polar = normal = NULL;

  if (normal)

    if (normalise3 (normal) == 0)

      normal = NULL;

  if (ellipticity == 0)

    normal = NULL;


    /* Find the Z corresponding to each potential */

  Z_potential = calloc_check (potentials, sizeof (double));

  for (shell = 0; shell < shells; shell++) {

    if (potential [shell] == 0)

      shellC = shell;

    Z_potential [(int) potential [shell]] = Z [shell];

    }


    /* Print the file */

  fprintf (feff_inp, "* FEFF 6 command file generated by %s\n\n", name);

  fprintf (feff_inp, "CONTROL 1 1 1 1\n");

  fprintf (feff_inp, "PRINT   0 0 0 0\n\n");

  fprintf (feff_inp, "HOLE         %3d       %9.5f\n", hole, S02);

  fprintf (feff_inp, "SIG2           0.00000\n");

  if (polar)

    fprintf (feff_inp, "POLARIZATION %9.5f %9.5f %9.5f\n",
                                     polar [0], polar [1], polar [2]);

  if (normal)

    fprintf (feff_inp, "ELLIPTICITY  %9.5f %9.5f %9.5f %9.5f\n",
                                     ellipticity,
                                     normal [0], normal [1], normal [2]);


  fprintf (feff_inp, "CRITERIA     %9.5f %9.5f\n", filter_CW, filter_PW);

  fprintf (feff_inp, "NLEG         %3d\n", max_legs);

  fprintf (feff_inp, "RMAX         %9.5f\n", max_length);

  fprintf (feff_inp, "CORRECTIONS  %9.5f %9.5f\n", dE, dEi);

  fprintf (feff_inp, " \nPOTENTIALS\n");

  for (ipotential = 0; ipotential < potentials; ipotential++)

    fprintf (feff_inp, "     %3d  %3.0f\n",
                        ipotential, Z_potential [ipotential]);

  fprintf (feff_inp, " \nATOMS\n");

  for (shell = 0; shell < shells; shell++)

    fprintf (feff_inp, " %15.10f %15.10f %15.10f %3.0f\n",
                        v [shell][0], v [shell][1], v [shell][2],
                        potential [shell]);


    /* specify the OVERLAP for every unique atomic potential.

       Since there is a maximum of 8 overlap shells allowed,
       pass the closest 8 shells.                            */

  for (ipotential = 0; ipotential < potentials; ipotential++) {

    fprintf (feff_inp, " \nOVERLAP %d\n", ipotential);


      /* find the first instance of an atom with this potential */

    for (shell0 = 0; shell0 < shells; shell0++)

      if (potential [shell0] == ipotential)

        break;

    for (number = 0, current_d = -1, next_d = 0.001;
                     current_d < next_d && number < 8;)

      for (current_d = next_d, next_d = MAXDOUBLE, shell = 0;
                                          shell < shells; shell++) {

        d = distance3 (v [shell0], v [shell]);

        if (d == current_d && number < 8) {

          fprintf (feff_inp, "      %3.0f %4.0f %11.6f\n",
                                    potential [shell],
                                    max (1, fabs (N [shell] /
                                       max (0.1, fabs (N [shell0])))),
                                    d);

          number++;

          }

        if (d > current_d && d < next_d)

          next_d = d;

        }
    }

  fprintf (feff_inp, " \nEND\n");

  fclose (feff_inp);

  free_check (Z_potential);

  return 1;

  }


  /*********************************************************************}
  {                                                                     }
  {  Execute FEFF6.  The executable must be in the PATH or in the       }
  {  directory specified by FEFF_DIRECTORY.                             }
  {                                                                     }
  {*********************************************************************/

int execute_FEFF6 (char name [],

                    char path [],

                    int potph, int paths, int genfmt, int ff2chi,

                    int print) {

  int i;
  pid_t newpid;

  FILE * feff_inp;

  char line [256], * p, * d, localpath [256];


    /* if print is 0, then route the output of FEFF to /dev/null */

  if (potph)

    potph = 1;

  if (paths)

    paths = 1;

  if (genfmt)

    genfmt = 1;

  if (ff2chi)

    ff2chi = 1;


    /* Modify the CONTROL card in feff.inp */

  feff_inp = fopen ("feff.inp", "r+");

  if (feff_inp == NULL) {

    cwarn ("execute_FEFF6 (%s): can't open feff.inp", name);

    return 0;

    }

  while (p = fgets (line, 256, feff_inp)) {

    if (!strncmp (line, "CONTROL", 7)) {

        /* Go back to the beginning of the line */

#if defined (__GCC__)

      fseek (feff_inp, -strlen (line) - 1, SEEK_CUR);

#else

      fseek (feff_inp, -strlen (line), SEEK_CUR);

#endif

      fprintf (feff_inp, "CONTROL %1d %1d %1d %1d",
                          potph, paths, genfmt, ff2chi);

      break;

      }
    }

  fclose (feff_inp);

  if (p == NULL) {

    cwarn ("execute_FEFF6 (%s): feff.inp is missing the CONTROL card", name);

    return 0;

    }


    /* Execute FEFF6.  The executable must be in the PATH or in the
       directory specified by FEFF_DIRECTORY */

  fflush (stdout);

  localpath [0] = 0;

  d = getenv ("FEFF_DIRECTORY");

  if (d) {

    strcat (localpath, d);

#if defined (BS_SEPARATOR)

    strcat (localpath, "\\");

#else

    strcat (localpath, "/");

#endif

    }

  strcat (localpath, "FEFF6");

  if (!print)

    strcat (localpath, " > FEFF6.out");

  printf (" FEFF6: %s\n", path);

  /* { static int count = 0; count++; if (count >= 3) exit (0); } */
  newpid = fork();
  if(newpid == 0) {        /* we are the child */
      execl(localpath,NULL);
  }
  else {
     if(newpid == -1) {
         cerror("Unable to execute (%d): %s",i,localpath);
     }
     else {
         waitpid(newpid, &i,0);
     }
  }

  if (WIFEXITED(i))

    cerror ("Abnormal termination (%d): %s", i, localpath);

  return i == 0;

  }


  /*********************************************************************}
  {                                                                     }
  {  Clean up after FEFF6.                                              }
  {                                                                     }
  {*********************************************************************/

int clean_FEFF6 (char name [], char path []) {

  remove ("feff.inp");
  remove ("geom.dat");
  remove ("paths.dat");
  remove ("files.dat");
  remove ("crit.dat");
  remove ("chi.dat");
  remove ("FEFF6.out");

#if defined (__GCC__)

  system ("del /f ph*.bin");
  system ("del /f feff*.dat");

#else

  system ("rm -fr phase.bin > /dev/null");
  system ("rm -fr ph[0-9]*.bin > /dev/null");
  system ("rm -fr feff[0-9]*.dat > /dev/null");

#endif

  return 1;

  }


void destruct_path_l (path_l ** path) {

  path_l * next;

  while (*path) {

    next = (*path)->next;

    free_check (*path);

    *path = next;

    }
  }


int read_FEFF6_path (FILE * paths_dat, path_l * path) {

  int leg, l;

  char line [256];


  path->number = 
  path->legs   = 
  path->points = 
  path->match  =
  path->use    = 0;

  path->next = NULL;

  if (!fgets (line, 256, paths_dat))

    return 0;

  if (sscanf (line, " %d %d %lf",
              &(path->number), &(path->legs), &(path->degeneracy)) <= 0)

    return 0;

  fgets (line, 256, paths_dat);


    /* Read in the leg vertices

       (shift by one position to put the initial atom at the start) */

  for (leg = 1; leg <= path->legs; leg++) {

    l = leg % path->legs;

    path->Z [l] = 0;

    path->potential [l] = -1;

    if (fgets (line, 256, paths_dat))

      sscanf (line, " %lf %lf %lf %lf",
                    &(path->v [l][0]), &(path->v [l][1]),
                    &(path->v [l][2]), &(path->potential [l]));

    if (path->potential [l] < 0) {

      cwarn ("read_FEFF6_path (path.dat): error reading path %d", path->number);

      return -1;

      }
    }

  path->use = 1;

  return 1;

  }

int write_FEFF6_path (FILE * paths_dat, path_l * path) {

  int l0, leg, l1, l2, l;

  double R_eff, R_leg, vertex, torsion;

  for (R_eff = 0, leg = 0; leg < path->legs; leg++)

     R_eff += distance3 (path->v [leg], path->v [(leg + 1)
                                                  % path->legs]);

  R_eff /= 2;

  fprintf (paths_dat, "%6d%5d%8.3f  index, nleg, degeneracy, r=%8.4f\n",
                path->number, path->legs, path->degeneracy, R_eff);

  fprintf (paths_dat, "      x           y           z     ipot"
                      "  label      rleg      beta        eta\n");


    /* Write out the leg vertices

       (shift by one position to put the initial atom at the end) */

  for (leg = 1; leg <= path->legs; leg++) {

    l0 = (leg + path->legs - 1) % path->legs;
    l  =  leg      % path->legs;
    l1 = (leg + 1) % path->legs;
    l2 = (leg + 2) % path->legs;

    R_leg = distance3 (path->v [l0], path->v [l]);

    vertex = vertex_angle3 (path->v [l0], path->v [l],
                                           path->v [l1]) * 180 / M_PI;

    torsion = torsion_angle3 (path->v [l0], path->v [l],
                          path->v [l1], path->v [l2]) * 180 / M_PI;

    fprintf (paths_dat, COORDINATE_FORMAT COORDINATE_FORMAT
                        COORDINATE_FORMAT
                        "%4.0f \'%s\' %10.4f%10.4f%10.4f\n",
                        path->v [l][0], path->v [l][1],
                        path->v [l][2], path->potential [l],
                        "      ", R_leg, vertex, torsion);

    }

  return 1;

  }


  /*********************************************************************}
  {                                                                     }
  {  Read paths from paths.dat and feffxxxx.dat into the list           }
  {                                                                     }
  {  If the path exists, it is overwritten.                             }
  {                                                                     }
  {  All paths are in numerical order.                                  }
  {                                                                     }
  {*********************************************************************/

int read_paths (path_l ** path_list, int read_data,

                int Nam,

                double potentialm [], double Zm [], double Rm [][3]) {

  FILE * paths_dat, * feff_dat;

  path_l path;

  path_l * entry;

  int i, l, imin;

  double d, dmin;

  char filename [16], line [256];


  paths_dat = fopen ("paths.dat", "r");

  if (!paths_dat)

    cerror ("read_paths: can't open paths.dat");


  readheader (paths_dat, NULL, 0);

  while (read_FEFF6_path (paths_dat, &path) > 0) {

      /* Find the entry with the correct index or add a new one */

    for (; *path_list; path_list = &((*path_list)->next))

      if ((*path_list)->number >= path.number)

        break;

 
    if (*path_list == NULL)

      *path_list = calloc_check (1, sizeof (path_l));

    else

      if ((*path_list)->number > path.number) {

        entry = *path_list;

        *path_list = calloc_check (1, sizeof (path_l));

        (*path_list)->next = entry;

        }

    entry = *path_list;


      /* Copy the path parameters */

    entry->number = path.number;

    entry->points = 0;

    entry->degeneracy = path.degeneracy;

    entry->legs = path.legs;


      /* Find the atoms on the paths */

    for (l = 0; l < path.legs; l++) {

        /* Get the nearest atom with the correct potential */

      for (dmin = MAXDOUBLE, imin = 0, i = 0; i < Nam; i++)

        if (floor (path.potential [l] + 0.5) == floor (potentialm [i] + 0.5)) {

          d = distance3 (path.v [l], Rm [i]);

          if (d < dmin) {

            imin = i;
            dmin = d;

            }
          }

      entry->atom [l] = imin;

      entry->v [l][0] = Rm [imin][0];
      entry->v [l][1] = Rm [imin][1];
      entry->v [l][2] = Rm [imin][2];

      entry->Z [l]    = Zm [imin];

      }


    for (l = 0; l < path.legs; l++)

      entry->potential [l] = path.potential [l];



      /* If required, get the exafs curves */

    if (read_data) {

      sprintf (filename, "feff%04d.dat", path.number);

      feff_dat = fopen (filename, "r");


      if (feff_dat) {

        readheader (feff_dat, NULL, 0);


          /* Discard the parameters of the atoms in the path */

        for (l = 0; l < entry->legs + 3; l++)

          fgets (line, 256, feff_dat);


          /* Read the EXAFS parameter curves */

        for (i = 0; !feof (feff_dat) && i < MAXPOINTS;)

          if (fgets (line, 256, feff_dat))

            if (sscanf (line, " %lf %lf %lf %lf %lf %lf %lf\n",

                          &(entry->k)          [i],
                          &(entry->real_2_phc) [i],
                          &(entry->mag_feff)   [i],
                          &(entry->phase)      [i],
                          &(entry->red_factor) [i],
                          &(entry->lambda)     [i],
                          &(entry->real_p)     [i]) > 0)

              i++;

        entry->points = i;

        fclose (feff_dat);

        remove (filename);

        }
      }
    }

  fclose (paths_dat);

  remove ("paths.dat");

  if (read_data)

    remove ("files.dat");

  return 1;

  }


  /*********************************************************************}
  {                                                                     }
  {  Save the binary phaseshift file (phase.bin)                        }
  {                                                                     }
  {*********************************************************************/

#if defined (SHORT_NAMES)

#define PHASE_NAME "ph%02d.bin"

#else

#define PHASE_NAME "phases%02d.bin"

#endif


int save_FEFF6_files (char name [], int phase, int index) {

  char phases [32];

  sprintf (phases, PHASE_NAME, index);

  if (phase)

    if (copy ("phase.bin", phases) != 0)

      cerror ("save_FEFF6_files (%s): can't copy phase.bin -> %s", name, phases);

  return 1;

  }


  /*********************************************************************}
  {                                                                     }
  {  Manage the binary phaseshift files                                 }
  {                                                                     }
  {             (phase.bin / ph<index>.bin / phases<index>.bin)         }
  {                                                                     }
  {*********************************************************************/

int manage_FEFF6_files (char name [], int calc, int index) {

  static int Current_Phase = -1;

  char c [32];


    /* Clean */

  if (index < 0) {

    Current_Phase = -1;

    return 1;

    }


    /* Save the current phase file if the index has changed */

  if (Current_Phase >= 0 && index != Current_Phase) {

    sprintf (c, PHASE_NAME, Current_Phase);

    if (rename ("phase.bin", c) != 0)

      cerror ("manage_FEFF6_files (%s): can't rename phase.bin -> %s", name, c);

    Current_Phase = -1;

    }


    /* If the phases are being recalculated, delete the phase file */

  if (calc) {

    if (index == Current_Phase)

      remove ("phase.bin");

    else {

      sprintf (c, PHASE_NAME, index);

      remove (c);

      }

    Current_Phase = index;

    }


    /* If the phases are not being recalculated, rename the phase file */

  if (!calc && Current_Phase != index) {

    sprintf (c, PHASE_NAME, index);

    if (rename (c, "phase.bin") != 0)

      cerror ("manage_FEFF6_files (%s): can't rename %s -> phase.bin", name, c);

    Current_Phase = index;

    }


  return 1;

  }


  /*********************************************************************}
  {                                                                     }
  {  Rewrite the paths.dat file with the new coordinates (R [][3]).     }
  {                                                                     }
  {  The paths are taken from the path list passed.  Only the marked    }
  {  paths are used.                                                    }
  {                                                                     }
  {  If no paths are written, the file is deleted.                      }
  {                                                                     }
  {*********************************************************************/

int rewrite_FEFF6_paths (char name [], path_l * path, int all, double R [][3],

                         int * count) {

  FILE * paths_dat;

  double v_o [MAXLEGS][3];

  int l;

  /* printf (" rewrite_FEFF6_paths (\"%s\", %p, %d, ...)\n", name, path, all); */

  *count = 0;

  paths_dat = fopen ("paths.dat", "w");

  if (paths_dat == NULL) {

    cwarn ("rewrite_FEFF6_paths (%s): can't open paths.dat", name);

    return 0;

    }


    /* In this version, print an empty header */

  fprintf (paths_dat, " ---------------------------------------"
                      "----------------------------------------\n");


    /* Write each path */

  for (; path; path = path->next) {

    static char onumber [32], nnumber [32];

    int match, i, OK;


    path->match = 0;

    if (!path->use)

      continue;


      /* Compare the old and new coordinates for this path.

         (Do these look the same when written in the format
          (COORDINATE_FORMAT) used in write_FEFF6_path?) */

    for (match = !all, l = 0; l < path->legs && match; l++)

      for (i = 0; i < 3 && match; i++) {

        sprintf (onumber, COORDINATE_FORMAT, path->v [l][i]);
        sprintf (nnumber, COORDINATE_FORMAT, R [path->atom [l]][i]);

        match = !strcmp (onumber, nnumber);

        /* printf (" \"%s\" - \"%s\" : %d\n", onumber, nnumber, match); */

        }

    if (match) {

      path->match = 1;

      continue;

      }


      /* Save and update the coordinates */

    for (l = 0; l < path->legs; l++) {

      v_o [l][0] = path->v [l][0];
      v_o [l][1] = path->v [l][1];
      v_o [l][2] = path->v [l][2];

      path->v [l][0] = R [path->atom [l]][0];
      path->v [l][1] = R [path->atom [l]][1];
      path->v [l][2] = R [path->atom [l]][2];

      }


      /* Write the path */

    OK = write_FEFF6_path (paths_dat, path);


      /* Restore the coordinates */

    for (l = 0; l < path->legs; l++) {

      path->v [l][0] = v_o [l][0];
      path->v [l][1] = v_o [l][1];
      path->v [l][2] = v_o [l][2];

      }


    if (!OK) {

      cwarn ("error writing paths.dat");

      fclose (paths_dat);

      return 0;

      }


    (*count)++;

    }

  fclose (paths_dat);

  if (*count == 0)

    remove ("paths.dat");

  return 1;

  }

#endif /* __FEFF6_C */

