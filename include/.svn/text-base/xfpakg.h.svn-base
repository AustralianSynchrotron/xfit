/***********************************************************************}
{									}
{  This file contains the definition of the XFPAKG_BIN_type and the	}
{  functions read_XFPAKG_BIN, write_XFPAKG_BIN, and XFPAKG_BIN_column.	}
{									}
{  Copyright (C) Paul J. Ellis, University of Sydney, 1993.		}
{									}
{  The data structure all use double-precision floating point even	}
{  though binary data files use single-precision.			}
{									}
{***********************************************************************/

#if !defined (__XFPAKG_H__)

#define __XFPAKG_H__

#if defined (VAXC)

#include <rms.h>

#endif

#include "header.h"
#include "exafs.h"

#if !defined (X_MAXCOLUMNS)

#define X_MAXCOLUMNS 32

#endif

#if !defined (X_MAXCHANNELS)

#define X_MAXCHANNELS 20

#endif

typedef struct {

	char	header_line [16][61];
	
	int	NCOL, NPTS, NS, VRSN, TYPE;
	double	D, STP_DEG, CU, ST, STP, DLND [5], DLT [5], S [5],
		ENERGY, CURRENT;
	char	CHANNEL [X_MAXCHANNELS][3], COLUMN [10][7];

	double	*column [X_MAXCOLUMNS];

	} XFPAKG_BIN_type;

int setup_XFPAKG_BIN (XFPAKG_BIN_type *XFP) {

  int i;

  if (XFP == NULL)
    return (FALSE);

  for (i = 0; i < 16; i++)
    strcpy (XFP->header_line [i], NSPACES (60));
    
  XFP->NCOL	= 
  XFP->NPTS	= 
  XFP->NS	=
  XFP->VRSN	=
  XFP->TYPE	=
  XFP->D	=
  XFP->STP_DEG	=
  XFP->CU	=
  XFP->ST	=
  XFP->STP	= 0;

  for (i = 0; i < 5; i++)

    XFP->DLND [i] =
    XFP->DLT  [i] =
    XFP->S    [i] = 0;

  XFP->ENERGY	=
  XFP->CURRENT	= 0;

  for (i = 0; i < X_MAXCHANNELS; i++)
    strcpy (XFP->CHANNEL [i], "  ");

  for (i = 0; i < 10; i++)
    strcpy (XFP->COLUMN  [i], "      ");

  for (i = 0; i < X_MAXCOLUMNS; i++)
    XFP-> column [i] = NULL;

  return TRUE;

  }

int read_XFPAKG_BIN (char filename [], XFPAKG_BIN_type *XFP) {

    /* Read from an XFPAKG BIN-1 or BIN-2 type data file.

     Sample header (from pchi.AVG)
  "
  10  NPTS: 481,NS: 4,D:1.92000,STP/DEG: 2000,CU: 41204,VRSN:3
  ST-STP/DLND: 43732- 39124/ 42198, 41953, 39124,     0,     0
  DLT/S:   1,   0,   0,   0,   0/ 1.00, 1.00, 1.00,10.00, 0.00
  pchi21.DAX 18-Sep-90>14:17 BLIV2  3.0  27.0/TYPE: 2/NCOL:10 
  PLASTOCYANIN BAND 1 HIGH PHSLITS 11.5X1.95DETUNED 50% CW AT 
  MS I0 II II FF 00 00 00 00 00 00                            
  MS    EV    K     RAW   NORMALEXAFS VICTORI0    PREEDGSPLINE
                                                              
                                                              
  EVCONV(14:39 18-Sep-90)  42150.4= 8980.3  9000. COL 5/COL 2 
  DIRAVG(14:47 18-Sep-90)  Pc band I high pH, averaged nonunit
  PREEDG(14:53 18-Sep-90)  9047- 9640EV(264 PTS) ORD= 2       
  SPLINE: E(K=0)= 9000 [ 9025(2) 9138(3) 9343(3) 9640]        
                                                              
                                                              
                                                              "
    = 16 lines of 60 characters */
  
  char (* line) [61];
  FILE *infile;
  float record [X_MAXCOLUMNS];
  char dummy [128];
  int i, j, record_length;

  infile = fopen (filename, "rb");

  if (infile == NULL)
    return (FALSE);

    /* Get the header lines into XFPAKG_data.
       Short lines extended with spaces. */

  line = XFP->header_line;

  for (i = 0; i < 16; i++) {

    fread (line [i], sizeof (char), 60, infile);
    line [i][60] = '\0';

    for (j = strlen (line [i]); j < 60; j++)
      line [i][j] = ' ';

    }

    /* Extract the header data. */

  sscanf (line [0]     , "%3d", &XFP->NCOL);
  sscanf (line [0] +  9, "%4d", &XFP->NPTS);
  sscanf (line [0] + 17, "%2d", &XFP->NS);
  sscanf (line [0] + 22, "%7lf", &XFP->D);
  sscanf (line [0] + 38, "%5lf", &XFP->STP_DEG);
  sscanf (line [0] + 47, "%6lf", &XFP->CU);
  sscanf (line [0] + 59, "%1d", &XFP->VRSN);

  sscanf (line [1] + 12, "%6lf", &XFP->ST);
  sscanf (line [1] + 19, "%6lf", &XFP->STP);

  for (i = 0; i < 5; i++) {

    sscanf (line [1] + 26 + i * 7, "%6lf", &XFP->DLND [i]);

    sscanf (line [2] +  6 + i * 5, "%4lf", &XFP->DLT [i]);
    sscanf (line [2] + 31 + i * 6, "%5lf", &XFP->S [i]);

    }

  sscanf (line [3] + 32, "%5lf", &XFP->ENERGY);
  sscanf (line [3] + 37, "%6lf", &XFP->CURRENT);
  sscanf (line [3] + 49, "%2d", &XFP->TYPE);
 
  for (i = 0; i < X_MAXCHANNELS; i++) {

    strncpy (XFP->CHANNEL [i], line [5] + i * 3, 2);
    XFP->CHANNEL [i][2] = '\0';

    }

  for (i = 0; i < 10; i++) {

    strncpy (XFP->COLUMN [i], line [6] + i * 6, 6);
    XFP->COLUMN [i][6] = '\0';

    }

  record_length = XFP->NCOL * 4; /* sizeof (float) */

    /* Read past any dummy data inserted to align to
                                            the end of the record. */

  if ((960 % record_length) != 0)
    fread (dummy, 1, record_length - (960 % record_length), infile);

    /* Deallocate any storage currently allocated for binary data. */

  for (i = 0; i < X_MAXCOLUMNS; i++)
    if (XFP->column [i] != NULL) {

      free (XFP->column[i]);
      XFP->column [i] = NULL;

      }

    /* Allocate storage for the new data. */

  for (i = 0; i < XFP->NCOL; i++)
    XFP->column [i] = calloc_check (XFP->NPTS, sizeof (double));

    /* Read the binary data. */

  for (i = 0; i < XFP->NPTS; i++) {

    fread (record, sizeof (float), XFP->NCOL, infile);

    for (j = 0; j < XFP->NCOL; j++)
      XFP->column [j][i] = record [j];

    }

  fclose (infile);

  return TRUE;

  }

int write_XFPAKG_BIN (char filename [], XFPAKG_BIN_type *XFP) {

    /* Write to an XFPAKG BIN-1 or BIN-2 type data file. */

  char (* line) [61];
  float record [X_MAXCOLUMNS];
  int i, j, record_length;

#if defined (VAXC)

    /* outfile and rab are VAX-specific structures. */

  unsigned char buffer [256];
  struct FAB outfile;
  struct RAB rab;
  long rms_status;

#else

  FILE *outfile;

#endif

  record_length = XFP->NCOL * 4; /* sizeof (float) */

#if defined (VAXC)

    /* Open the output file with the correct record length. */
 
  outfile = cc$rms_fab;

  outfile.fab$l_fna = filename;
  outfile.fab$b_fns = strlen (filename);
  outfile.fab$w_mrs = record_length;
  outfile.fab$b_rfm = FAB$C_FIX;

  rab = cc$rms_rab;
  rab.rab$l_fab = &outfile;

  rms_status = sys$create (&outfile);

  if (rms_status != RMS$_NORMAL && rms_status != RMS$_CREATED)
    exit (1);

  rms_status = sys$connect (&rab);

  if (rms_status != RMS$_NORMAL)
    exit (1);

#else

  outfile = fopen (filename, "wb");

#endif

    /* Reconstruct lines 1 - 4, 6, and 7 of the header. */
  
  line = XFP->header_line;

  sprintf (line [0],
     "%2d  NPTS:%4d,NS:%2d,D:%7.5f,STP/DEG:%5d,CU:%6d,VRSN:%1d", XFP->NCOL,
     XFP->NPTS, XFP->NS, XFP->D, (int) XFP->STP_DEG, (int) XFP->CU, XFP->VRSN);
  sprintf (line [1],
     "ST-STP/DLND:%6d-%6d/%6d,%6d,%6d,%6d,%6d", (int) XFP->ST, (int) XFP->STP,
     (int) XFP->DLND [0], (int) XFP->DLND [1], (int) XFP->DLND [2],
     (int) XFP->DLND [3], (int) XFP->DLND [4]);
  sprintf (line[2],
     "DLT/S:%4d,%4d,%4d,%4d,%4d/%5.2f,%5.2f,%5.2f,%5.2f,%5.2f",
     (int) XFP->DLT [0], (int) XFP->DLT [1], (int) XFP->DLT [2],
     (int) XFP->DLT [3], (int) XFP->DLT [4],
     XFP->S [0], XFP->S [1], XFP->S [2], XFP->S [3], XFP->S [4]);

  for (i = 0; i < 32; i++)
    if (line [3][i] == '\0')
      line [3][i] = ' ';

  sprintf (line [3] + 32,
     " %4.1f%6.1f/TYPE:%2d/NCOL:%2d ",
     XFP->ENERGY, XFP->CURRENT, XFP->TYPE, XFP->NCOL);

    /* Write a minimum of 11 channel codes.  If the file is type 1 and
       NCOL > 11, write NCOL codes. */

  j = 11;

  if ((XFP->TYPE == 1) && (XFP->NCOL > 11))
    j = XFP->NCOL;

  strcpy (line [5], NSPACES (60));

  for (i = 0; i < j; i++) {

    if (XFP->CHANNEL [i][0] != '\0') {

      line [5][i * 3] = XFP->CHANNEL [i][0];

      if (XFP->CHANNEL [i][1] != '\0')
        line [5][i * 3 + 1] = XFP->CHANNEL [i][1];

      }
    }

  for (i = XFP->NCOL; i < 10; i++)
    strcpy (XFP->COLUMN [i], "      ");

  sprintf (line [6],
     "%s%s%s%s%s%s%s%s%s%s",
     XFP->COLUMN [0], XFP->COLUMN [1], XFP->COLUMN [2], XFP->COLUMN [3],
     XFP->COLUMN [4], XFP->COLUMN [5], XFP->COLUMN [6], XFP->COLUMN [7],
     XFP->COLUMN [8], XFP->COLUMN [9]);

    /* Write the header. */

#if defined (VAXC)

  rab.rab$l_rbf = buffer;
  rab.rab$w_rsz = record_length;

  for (i = 0; i < 960; i++) {

    buffer [i % record_length] = line [i / 60][i % 60];
  
    if (((i + 1) % record_length) == 0)
      rms_status = sys$put (&rab);

    }

  if ((i % record_length) != 0)
    rms_status = sys$put (&rab);

#else

  for (i = 0; i < 16; i++)
    fwrite (line [i], sizeof (char), 60, outfile);

    /* Align to end of record if necessary. */

  if ((960 % record_length) != 0)
    fwrite (line [15], 1, record_length - (960 % record_length), outfile);

#endif

    /* Write the binary data. */

#if defined (VAXC)

  rab.rab$l_rbf = record;

  for (i = 0; i < XFP->NPTS; i++) {

    for (j = 0; j < XFP->NCOL; j++)
      record [j] = XFP->column [j][i];

    rms_status = sys$put (&rab);

    }

  rms_status = sys$close (&outfile);

#else

  for (i = 0; i < XFP->NPTS; i++) {

    for (j = 0; j < XFP->NCOL; j++)
      record [j] = XFP->column [j][i];

    fwrite (record, sizeof (float), XFP->NCOL, outfile);

    }

  fclose (outfile);

#endif

  return TRUE;

  }

int XFPAKG_BIN_column (XFPAKG_BIN_type *XFP, char label []) {

  /* Finds the column number of the column with the given label.  The column
     number is from 0 to 9.  If the labelled column is not found, -1 is
     returned.  If label is NULL, the routine searches past the previous
     channel using the previous label. */

  static int column = -1;
  static char slabel [16] = { "" };

  slabel [15] = '\0';

  if (label != NULL) {

    strncpy (slabel, label, 15);
    column = 0;

    }

  if (column < 0)
    return -1;

  for (; column < XFP->NCOL; column++)
    if (!strcmp (XFP->COLUMN [column], slabel))
      return (column++);

  return -1;

  }

int XFPAKG_BIN_channel (XFPAKG_BIN_type *XFP, char label []) {

  /* Finds the column number of the column with the given channel label.
     The column number is from 0 to X_MAXCHANNELS - 1.  If the labelled
     channel is not found, -1 is returned.  If label is NULL, the routine
     searches past the previous channel using the previous label. */

  static int channel = -1;
  static char slabel [16] = { "" };

  slabel [15] = '\0';

  if (label != NULL) {

    strncpy (slabel, label, 15);
    channel = 0;

    }

  if (channel < 0)
    return -1;

  for (; channel < XFP->NCOL; channel++)
    if (!strcmp (XFP->CHANNEL [channel], slabel))
      return (channel++);

  return -1;

  }

double XFPAKG_calibrate (double MS, double d, double steps_per_degree,           
  				    double E_cal, double MS_cal) {

  /* This function returns the energy (in eV) corresponding to motor
     step MS, given that at MS_cal the energy is E_cal.  d-spacing is d,
     and there are steps_per_degree motor steps per degree. */

  double theta;

  theta = (MS - MS_cal) / steps_per_degree + monochromator_angle (E_cal, d);

  return monochromator_energy (theta, d);

  }

double XFPAKG_motor_step (double energy, double d, double steps_per_degree,
                                         double E_cal, double MS_cal) {

  /* This function is used to convert from energy (in eV) to the equivalent
     motor step value used by the Stanford XFPAKG software.  d is the
     d-spacing (in angstroms).  steps_per_degree is the number of motor
     steps per degree of monochromator rotation, E_cal is the energy of a
     single point of calibration (in eV), and MS_cal the motor-step value
     at the calibration point. */

  return MS_cal + steps_per_degree *

         (monochromator_angle (energy, d) - monochromator_angle (E_cal, d));

  }

#endif /* __XFPAKG_H__ */
