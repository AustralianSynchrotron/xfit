/***********************************************************************}
{									}
{  Purpose:								}
{									}
{  	read XAS data from the Daresbury Radiation Laboratory.		}
{									}
{  Contents:								}
{									}
{	type: DSRS_type = MIDAS_type					}
{									}
{	funtion: read_DSRS						}
{									}
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.		}
{									}
{***********************************************************************/

#if !defined (__DSRS_H__)

#define __DSRS_H__

#include "header.h"
#include "scan.h"
#include "MIDAS.h"

typedef MIDAS_type DSRS_type;

#define DSRS_MAXPOINTS 2048

#define DSRS_MAXCHANNELS M_MAXCHANNELS

#define clear_DSRS(DSRS) clear_MIDAS (DSRS)
#define empty_DSRS(DSRS) empty_MIDAS (DSRS)

int DSRS_read_dataset      (char line [], FILE *file, DSRS_type *DSRS);
int DSRS_read_observations (char line [], FILE *file, DSRS_type *DSRS);

MIDAS_block_type DSRS_block [] = {

  { " &SRS", DSRS_read_dataset        },
  { " &END", DSRS_read_observations   },

  { NULL }

  };

#define read_line(line, file) MIDAS_read_line (line, file, " &")

int read_DSRS (char filename [], DSRS_type *DSRS) {

  /* Read a DSRS ASCII data file.

     Sample file:
  
  -------------------------------------------------------------------------

   &SRS
   SRSRUN= 4891,SRSDAT=961027,SRSTIM=032943,
   SRSSTN='EX1A',SRSPRJ='11111111',SRSEXP='AAAAAAAA',
   SRSTLE='NI(ET2DTC)2 SOLID DISC                                      ',
   SRSCN1='150     ',SRSCN2='19      ',SRSCN3='03.39   ',
   &END
    14000.50        1000.      427109.      609312.        1374. 
    13990.50        1000.      474304.      682410.        1367. 
    13980.50        1000.      528588.      766795.        1379. 
    13970.50        1000.      556933.      814290.        1375. 
    13960.50        1000.      572163.      843067.        1379. 
    ...
    12600.50        7893.     3407651.     1031675.       10704. 
    12594.50        7928.     3417607.     1043219.       10803. 
    12588.50        7964.     3427797.     1053884.       10959. 
    12582.50        8000.     3437466.     1065806.       10909. 
    12576.50        8036.     3448274.     1076586.       11094. 
  END OF DATA
  <<EOF>>

  -------------------------------------------------------------------------
  */
  
  FILE *file;

  char line [1024];

  int i;

  comment_line = 0;

    /* Set the DSRS structure to empty. */

  empty_DSRS (DSRS);

  strcpy (DSRS->filename, extract_file_name (filename));

  file = fopen (filename, "r");

  if (file == NULL)

    return FALSE;

  read_line (line, file);

  while (!feof (file)) {

    for (i = 0; DSRS_block [i].title; i++)

      if (match (line, DSRS_block [i].title))

        break;

    if (DSRS_block [i].title)

      DSRS_block [i].read (line, file, DSRS);

    else

      read_line (line, file);

    }

  fclose (file);

  strcpy (DSRS->type, "DSRS");

  return TRUE;

  }

int DSRS_read_dataset (char line [], FILE *file, DSRS_type *DSRS) {

    /* read the collection parameters:

     &SRS
     SRSRUN= 4891,SRSDAT=961027,SRSTIM=032943,
     SRSSTN='EX1A',SRSPRJ='11111111',SRSEXP='AAAAAAAA',
     SRSTLE='NI(ET2DTC)2 SOLID DISC                                      ',
     SRSCN1='150     ',SRSCN2='19      ',SRSCN3='03.39   ',

     */

  while (read_line (line, file)) {

      /* Traverse the line looking for any parameters of interest. */

    static char *month [] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                              "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };

    static char *name [] = { "SRSDAT=", "SRSTIM=", "SRSTLE=", "SRSCN",
                              NULL };

    static int   code [] = {  1, 2, 3, 3 };

    char *c, *c0;

    unsigned long n;

    for (c = line; *c; c++) {

      for (n = 0; name [n]; n++)
 
        if (match (c, name [n])) {

          c0 = c;

          c += strlen (name [n]);

          switch (code [n]) {

            case 1:

              sscanf (c, " %lu", &n);

              sprintf (DSRS->date, "%2d-%s-19%02d", n % 100,
                        month [min (max ((n / 100) % 100, 1), 12) - 1],
                        (n / 10000) % 100);

              break;

            case 2:

              sscanf (c, " %lu", &n);

              sprintf (DSRS->time, "%02d:%02d:%02d.00", n / 10000,
                               (n / 100) % 100, n % 100);

              break;

            case 3:

              while (*c != '\'' && *c) c++;

              if (*c) c++;

              while (*c != '\'' && *c) c++;

              if (*c) c++;

              n = *c;

              *c = 0;

              add_comment (&DSRS->comment, ++comment_line, 0, c0 + 3);

              *c = n;

              c--;

              break; 
              
            }

          break;

          }
      }
    }

  return TRUE;

  }

int DSRS_read_observations (char line [], FILE *file, DSRS_type *DSRS) {

    /* read the data block:

     &END
      14000.50        1000.      427109.      609312.        1374. 
      13990.50        1000.      474304.      682410.        1367. 
      ...

      Put the following headings on the columns:

      "step"  "time"  "i0"  "i1"  "f0" ... "fn"

    */

  int c, n, N, channel0, nchannels, ichannels;

  double channels [DSRS_MAXCHANNELS];

  N = DSRS_MAXPOINTS;

  DSRS->N = 0;

  for (n = ichannels = 0; ; ) {

    if (!read_line (line, file))
 
      return TRUE;

    if (match (line, " END OF DATA")) {

      read_line (line, file);

      return TRUE;

      }

    if (n <= N) {

      if (nchannels = sscan (line, " %l[%n ]f", DSRS_MAXCHANNELS,
                                                           channels)) {

        if (!ichannels)

          ichannels = nchannels;

        if (nchannels == ichannels)

          channel0 = 0;

        if (channel0 + nchannels > DSRS_MAXCHANNELS)

          nchannels = DSRS_MAXCHANNELS - channel0;

        if (DSRS->channels < channel0 + nchannels)

          DSRS->channels = channel0 + nchannels;

        for (c = 0; c < DSRS->channels; c++)

          if (!DSRS->channel [c]) {

            static char *label [] = { "step", "time", "i0", "i1", "i2" };

            DSRS->channel [c] = calloc_check (N, sizeof (double));

            if (c < 5)

              strcpy (DSRS->channel_label [c], label [c]);

            if (c == 5) {

              strcpy (DSRS->channel_label [4], "ftotal");

              strcpy (DSRS->channel_label [5], "i2");

              }

            if (c > 5)

              sprintf (DSRS->channel_label [c], "f%d", c - 6);

            }

        if (channel0 == 0 && nchannels > 0)

          n++;

        for (c = 0; c < nchannels; c++)

          DSRS->channel [c + channel0][n - 1] = channels [c];

        channel0 += nchannels;

        if (channel0 >= DSRS->channels)

          DSRS->N = n;

        }
      }
    }
  }

#undef read_line

#endif /* __DSRS_H__ */
