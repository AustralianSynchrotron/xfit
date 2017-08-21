/***********************************************************************}
{									}
{  Purpose:								}
{									}
{  	read XAS data from the Stanford Synchrotron Radiation		}
{	Laboratory.							}
{									}
{  Contents:								}
{									}
{	type: MIDAS_type						}
{									}
{	funtion: read_MIDAS						}
{									}
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.		}
{									}
{***********************************************************************/

#if !defined (__MIDAS_H__)

#define __MIDAS_H__

#include "header.h"
#include "scan.h"
#include "comment.h"

#if !defined (M_MAXCHANNELS)

#define M_MAXCHANNELS 64

#endif

typedef struct {

  char filename [256],

       type [16], 
       date [12],
       time [12];

  comment_type *comment;

  int N, channels;

  char channel_label [M_MAXCHANNELS][16];

  double *channel [M_MAXCHANNELS];

  } MIDAS_type;

void fprint_MIDAS (FILE *file, MIDAS_type *M) {

  int i, j;

  fprintf (file, " MIDAS file: %s\n", M->filename);
  fprintf (file, "       type: %s\n\n", M->type);
  fprintf (file, "       time: %s %s\n\n", M->date, M->time);

  fprintf (file, " comments:\n\n");

  fprint_comment (file, "  %4d: ", M->comment);

  fprintf (file, " \n points: %d\n", M->N);
  fprintf (file, " channels: %d\n\n", M->channels);

  for (i = 0; i < M->channels; i++)
    fprintf (file, " %s", M->channel_label [i]);

  fprintf (file, " \n\n");

  for (i = 0; i < M->N; i++) {

    for (j = 0; j < M->channels; j++)

      fprintf (file, " %8.2e", M->channel [j][i]);

    fprintf (file, " \n");

    }

  fflush (file);

  }

void empty_MIDAS (MIDAS_type *MIDAS) {

  int channel;

  MIDAS->N        =
  MIDAS->channels = 0;

  strcpy (MIDAS->date, "00-000-0000");
  strcpy (MIDAS->time, "00:00:00.00");

  MIDAS->filename [0] =
  MIDAS->type     [0] = 0;

  MIDAS->comment = NULL;

  for (channel = 0; channel < M_MAXCHANNELS; channel++) {

    free_check (MIDAS->channel [channel]);

    MIDAS->channel_label [channel][0] = 0;
    MIDAS->channel [channel] = NULL;

    }
  }

void clear_MIDAS (MIDAS_type *MIDAS) {

  int channel;

  for (channel = 0; channel < M_MAXCHANNELS; channel++)

    MIDAS->channel [channel] = NULL;

  empty_MIDAS (MIDAS);

  }

int MIDAS_channel (MIDAS_type *MIDAS, char label []) {

  /* Finds the channel number of the channel with the given channel label.
     The column number is 0 .. channels - 1.  If the labelled
     channel is not found, -1 is returned.  If label is NULL, the routine
     searches past the previous channel using the previous label. */

  static int channel = -1;

  static char slabel [32] = { "" };

  slabel [32] = 0;

  if (label != NULL) {

    strncpy (slabel, label, 31);
    channel = 0;

    }

  if (channel < 0)

    return (-1);

  for (; channel < MIDAS->channels; channel++)

    if (match (MIDAS->channel_label [channel], slabel))

      return (channel++);

  return (-1);

  }

typedef int MIDAS_read_function (char line [], FILE *file, MIDAS_type *MIDAS);

int MIDAS_ignore              (char line [], FILE *file, MIDAS_type *MIDAS);
int MIDAS_read_title          (char line [], FILE *file, MIDAS_type *MIDAS);
int MIDAS_read_identification (char line [], FILE *file, MIDAS_type *MIDAS);
int MIDAS_read_observations   (char line [], FILE *file, MIDAS_type *MIDAS);

typedef struct {

  char *title;

  MIDAS_read_function *read;

  } MIDAS_block_type;

MIDAS_block_type MIDAS_block [] = {

  { "*** SDATA dataset",        MIDAS_ignore              },
  { "*** Identification block", MIDAS_read_identification },
  { "*** Title block",          MIDAS_read_title          },
  { "*** Subtitle block",       MIDAS_read_title          },
  { "*** Data block",           MIDAS_ignore              },
  { "*** Observations",         MIDAS_read_observations   },

  { NULL }

  };

int MIDAS_read_line (char line [], FILE *file, char command []) {

  line [0] = 0;

  if (feof (file)) return FALSE;

  fgets (line, 1024, file);

  if (strchr (line, '\n'))

    *strchr (line, '\n') = 0;

  if (match (line, command) || line [0] == 0) return FALSE;

  return TRUE;

  }

#define read_line(line, file) MIDAS_read_line (line, file, "***")

int comment_line;

int read_MIDAS (char filename [], MIDAS_type *MIDAS) {

  /* Read a MIDAS ASCII data file.

     Sample header:
  
  -------------------------------------------------------------------------
 
  *** SDATA dataset $TEMP2:[B_HODGSON.62A]CUCRYH3R_0114
  
  *** Identification block
  Experiment  : EXAFS
  Data file creating process
   User  name : B_HODGSON   
   Image name : _DUA0:[SSRL$LOCAL.][MIDAS]MIDAS.EXE
   Time/date  : 10-JUL-1991 02:28:19.44
  Data file collection process
   User  name : B_HODGSON   
   Image name : _DUA0:[SSRL$LOCAL.][MIDAS]MIDAS_COLLECTOR.EXE;4
   Time/date  : 10-JUL-1991 02:28:37.42
  
  *** Title block
  Title              :
  PLASTOCYANIN reduced crystal # 3                                
  pH 7.0, room temperature                                        
  DETUNED BY 55% CCW @9867 eV, 8.5 kG                             
  c//e, b perpendicular, a//beam, slits 3.0 x 2.5
  
  *** Subtitle block
  Subtitle           :
  Lytle detector
  
  *** Data block      1
  *** Observations    557    Offset correction applied by default
  Observtn.     C1*2000       C5/C4       C6/C4       C7/C4       C8/C4
          1   40525.002   51231.400  127482.300  155327.400   43103.100
          2   40493.000   51264.400  127804.300  155933.400   42776.100
  
  -------------------------------------------------------------------------
  */
  
  FILE *file;

  char line [1024];

  int i;

  comment_line = 0;

    /* Set the MIDAS structure to empty. */

  empty_MIDAS (MIDAS);

  strcpy (MIDAS->filename, extract_file_name (filename));

  file = fopen (filename, "r");

  if (file == NULL)

    return FALSE;

  read_line (line, file);

  while (!feof (file)) {

    for (i = 0; MIDAS_block [i].title; i++)

      if (match (line, MIDAS_block [i].title))

        break;

    if (MIDAS_block [i].title)

      MIDAS_block [i].read (line, file, MIDAS);

    else

      read_line (line, file);

    }

  fclose (file);

  strcpy (MIDAS->type, "MIDAS");

  return TRUE;

  }

int MIDAS_ignore (char line [], FILE *file, MIDAS_type *MIDAS) {

    /* Ignore the current command line. */

  read_line (line, file);

  return TRUE;

  }

int MIDAS_read_title (char line [], FILE *file, MIDAS_type *MIDAS) {

    /* Read the "*** Title block" or "*** Subtitle block" block. */

  do

    if (!read_line (line, file))

      return TRUE;

    while (!match (line, "Title  :") && !match (line, "Subtitle  :"));

  while (read_line (line, file))

    add_comment (&MIDAS->comment, ++comment_line, 0, line);

  return TRUE;

  }

int MIDAS_read_identification (char line [], FILE *file, MIDAS_type *MIDAS) {

    /* read the "*** Identification block" block. */

  int i;

  do

    if (!read_line (line, file))

      return TRUE;

    while (!match (line, "Data file collection process"));

  do

    if (!read_line (line, file))

      return TRUE;

    while (!(i = match (line, " Time/date  :")));

  strcpy (MIDAS->date, "00-000-0000");
  strcpy (MIDAS->time, "00:00:00.00");

  sscanf (line + i, " %11s %11s", MIDAS->date, MIDAS->time);

  if (MIDAS->date [1] == '-') {

    for (i = 10; i > 0; i--)

      MIDAS->date [i] = MIDAS->date [i - 1];

    MIDAS->date [0] = '0';

    }

  read_line (line, file);

  return TRUE;

  }

int MIDAS_read_observations (char line [], FILE *file, MIDAS_type *MIDAS) {

    /* read the "*** Observations" block. */

  int i, j, m, n, channel, n_used, N, first;

  double channels [M_MAXCHANNELS];

  sscanf (line + match (line, "*** Observations"), " %d", &N);

  do

    if (!read_line (line, file))

      return TRUE;

    while (!(n = match (line, "Observtn. ")));

  for (channel = 0, m = 1; channel < M_MAXCHANNELS && m >= 1;
                                           channel += m >= 1, n += n_used)

    m = sscanf (line + n, " %s %n", MIDAS->channel_label [channel], &n_used);

  MIDAS->channels = channel;

  MIDAS->N = 0;

  for (i = 0; i < M_MAXCHANNELS; i++)

    free_check (MIDAS->channel [i]);

  MIDAS->channels = 0;

  first = TRUE;

  for (i = 0, n_used = 0; i < N; i++) {

    if (!read_line (line, file))

      return TRUE;

    n = line [0] ? atol (line + 1) : 0;

    if (n > 0 && n <= N && n_used < N && line [0] == ' ') {

        /* observation n. */

      MIDAS->N = n_used + 1;

      for (j = 0; ((j * 12) + 9) < strlen (line) && (j < M_MAXCHANNELS); j++)

        channels [j] = strtod (line + j * 12 + 9, NULL);

      if (first) {

        MIDAS->channels = j;

        for (j = 0; j < MIDAS->channels; j++)

	  MIDAS->channel [j] = calloc_check (N, sizeof (double));

        first = FALSE;

        }

      for (j = 0; j < MIDAS->channels; j++)

        MIDAS->channel [j][n_used] = channels [j];

      n_used++;

      }
    }

  read_line (line, file);

  return TRUE;

  }

#undef read_line

#endif /* __MIDAS_H__ */
