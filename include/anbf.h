/***********************************************************************}
{									}
{  Purpose:								}
{									}
{  	read XAS data from the Australian National Beamline Facility.	}
{									}
{  Contents:								}
{									}
{	type: ANBF_type = MIDAS_type					}
{									}
{	funtion: read_ANBF						}
{									}
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.		}
{									}
{***********************************************************************/

#if !defined (__ANBF_H__)

#define __ANBF_H__

#include "header.h"
#include "scan.h"
#include "MIDAS.h"

typedef MIDAS_type ANBF_type;

#define ANBF_MAXCHANNELS M_MAXCHANNELS

#define clear_ANBF(ANBF) clear_MIDAS (ANBF)
#define empty_ANBF(ANBF) empty_MIDAS (ANBF)

int ANBF_read_dataset        (char line [], FILE *file, ANBF_type *ANBF);
int ANBF_read_identification (char line [], FILE *file, ANBF_type *ANBF);
int ANBF_read_observations   (char line [], FILE *file, ANBF_type *ANBF);

MIDAS_block_type ANBF_block [] = {

  { "#S", ANBF_read_dataset        },
  { "#C", ANBF_read_identification },
  { "#D", ANBF_read_identification },
  { "#L", ANBF_read_observations   },

  { NULL }

  };

#define read_line(line, file) MIDAS_read_line (line, file, "#")

int ANBF_current_scan, ANBF_scan_limit [2], ANBF_maximum_block,
                                            ANBF_maximum_size;

int read_ANBF (char filename [], ANBF_type *ANBF, int limit []) {

  /* Read a ANBF ASCII data file.

     Sample file:
  
  -------------------------------------------------------------------------

  #F ./data/ga2
  #E 732435378
  #C twoc  User = guest
  #O0 Two Theta  Theta  Monochrom.  Cassette  Slit Top  Slit Bottom  Slit Left  Slit
  Right 
  #C Thu Mar 18 01:17:00 1993.  gano3 soln 0.3m.

  #S 1  ascan  mono 11.3 9.8  1500 -100000
  #D Thu Mar 18 01:17:22 1993
  #M 100000  (Monitor)
  #G0 1 1 90
  #Q 0 0
  #P0 0 60.0001 9.8 0 0 0 0 0
  #N 9
  #L Monochrom.  H  K  Encoder  Epoch  Monitor  Counter 3  Seconds  Detector
  11.298 0 0    11.3152  86 100001 0 1.12017 11164
  11.297 0 0    11.3142  87 100001 0 1.1201 11169
  11.296 0 0    11.3133  89 100001 0 1.12032 11173
  11.295 0 0    11.3124  90 100001 0 1.12095 11184
  ...
  9.803 0 0     9.8384  2595 100001 0 1.60477 13106
  9.802 0 0     9.8374  2597 100001 0 1.6052 13120
  9.801 0 0     9.8364  2599 100001 0 1.60569 13132
  9.8 0 0     9.8353  2601 100001 0 1.60626 13145
  <<EOF>>  

  -------------------------------------------------------------------------
  */
  
  FILE *file;

  char line [1024];

  int i;

  comment_line = 0;

    /* Set the ANBF structure to empty. */

  empty_ANBF (ANBF);

  ANBF_current_scan = 0;

  ANBF_scan_limit [0] = 1;
  ANBF_scan_limit [1] = MAXINT;

  if (limit)

    if (limit [0] || limit [1]) {

      ANBF_scan_limit [0] = limit [0];
      ANBF_scan_limit [1] = limit [1];

      }

  ANBF_maximum_block =
  ANBF_maximum_size  = 0;

  strcpy (ANBF->filename, extract_file_name (filename));

  file = fopen (filename, "r");

  if (file == NULL)

    return FALSE;

  read_line (line, file);

  while (!feof (file)) {

    for (i = 0; ANBF_block [i].title; i++)

      if (match (line, ANBF_block [i].title))

        break;

    if (ANBF_block [i].title)

      ANBF_block [i].read (line, file, ANBF);

    else

      read_line (line, file);

    }

  fclose (file);

  strcpy (ANBF->type, "ANBF");

  return TRUE;

  }

int ANBF_read_dataset (char line [], FILE *file, ANBF_type *ANBF) {

    /* read the collection parameters:

    #S 1  ascan  mono 11.3 9.8  1500 -100000

    */

  int N;

  sscanf (&line [2], "%d %*s %*s %*f %*f %d", &ANBF_current_scan, &N);

  if (ANBF_current_scan >= ANBF_scan_limit [0] &&
      ANBF_current_scan <= ANBF_scan_limit [1])

    ANBF_maximum_block = N;

  read_line (line, file);

  return TRUE;

  }

int ANBF_read_identification (char line [], FILE *file, ANBF_type *ANBF) {

    /* read the identification blocks:

    #C twoc  User = guest
    #C Thu Mar 18 01:17:00 1993.  gano3 soln 0.3m.
    #D Thu Mar 18 01:17:22 1993
 
    */

  char day [4], month [4], date [4], time [12], year [8];

  switch (line [1]) {

    case 'C' :

      add_comment (&ANBF->comment, ++comment_line, 0,
                                     line + 2 + match (line + 2, " "));

      break;

    case 'D' :

      sscanf (&line [2], " %3s %3s %2s %8s %4s", day, month,
                                                 date, time, year);

      if (!date [1]) {

        date [2] = 0;
        date [1] = date [0];
        date [0] = '0';

        }

      sprintf (ANBF->date, "%.2s-%.3s-%.4s", date, month, year);
      sprintf (ANBF->time, "%.8s.00", time);

    }

  read_line (line, file);

  return TRUE;

  }

int ANBF_read_observations (char line [], FILE *file, ANBF_type *ANBF) {

    /* read the data block:

    #L Monochrom.  H  K  Encoder  Epoch  Monitor  Counter 3  Seconds  Detector
    11.298 0 0    11.3152  86 100001 0 1.12017 11164
    ...

    Up to this point, ANBF->N points have been read; the maximum number
    of additional points in this block is ANBF_maximum_block and the
    space currently allocated is ANBF_maximum_size.

    */

  int i, j, m, n, n_used, channel;

  double channels [ANBF_MAXCHANNELS];

  if (ANBF_current_scan < ANBF_scan_limit [0] ||
      ANBF_current_scan > ANBF_scan_limit [1]) {

    read_line (line, file);

    return TRUE;

    }

  ANBF_current_scan = 0;

  if (ANBF->N == 0) {

    for (channel = 0, m = n = 2; channel < ANBF_MAXCHANNELS && m >= 1;
                                 channel += m >= 1, n += n_used)

      m = sscanf (line + n, " %s %n", ANBF->channel_label [channel],
                                      &n_used);

    ANBF->channels = channel;

    for (channel = 0; channel < ANBF->channels - 1; channel++)

      if (!strcasecmp (ANBF->channel_label [channel], "Counter")) {

        strcat (ANBF->channel_label [channel],
                ANBF->channel_label [channel + 1]);

        for (n = channel + 2; n < ANBF->channels; n++)

          strcpy (ANBF->channel_label [n - 1], ANBF->channel_label [n]);

        ANBF->channel_label [n - 1][0] = 0;

        ANBF->channels--;

        }
    }

  for (n = 0;;) {

    if (!read_line (line, file))

      return TRUE;

    if (channel = sscan (line, " %l[%n ]f", ANBF->channels, channels)) {

      if (ANBF->N + 1 > ANBF_maximum_size) {

          /* Change allocation to the maximum of (ANBF->N + 64) and
             (ANBF->N + ANBF_maximum_block - n) */

        ANBF_maximum_size = max (ANBF->N + 64,
                                 ANBF->N + ANBF_maximum_block - n);

        for (channel = 0; channel < ANBF->channels; channel++)

          ANBF->channel [channel] = 

                    realloc_check (ANBF->channel [channel],
                                   ANBF_maximum_size * sizeof (double));

        ANBF_maximum_block = 0;

        }

      for (channel = 0; channel < ANBF->channels; channel++)

        ANBF->channel [channel][ANBF->N] = channels [channel];

      ANBF->N++;
      n++;

      }
    }
  }

#undef read_line

#endif /* __ANBF_H__ */
