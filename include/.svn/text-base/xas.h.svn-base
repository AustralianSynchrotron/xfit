/***********************************************************************}
{									}
{  Purpose:								}
{									}
{  	read XAS data from the Stanford Synchrotron Radiation		}
{	Laboratory (XAS format).					}
{									}
{  Contents:								}
{									}
{	type: XAS_type = MIDAS_type					}
{									}
{	funtion: read_XAS						}
{									}
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.		}
{									}
{***********************************************************************/

#if !defined (__XAS_H__)

#define __XAS_H__

#include "header.h"
#include "scan.h"
#include "MIDAS.h"

typedef MIDAS_type XAS_type;

#define XAS_MAXPOINTS 2048

#define XAS_MAXCHANNELS M_MAXCHANNELS

#define clear_XAS(XAS) clear_MIDAS (XAS)
#define empty_XAS(XAS) empty_MIDAS (XAS)

int XAS_read_dataset      (char line [], FILE *file, XAS_type *XAS);
int XAS_read_offsets      (char line [], FILE *file, XAS_type *XAS);
int XAS_read_observations (char line [], FILE *file, XAS_type *XAS);

MIDAS_block_type XAS_block [] = {

  { "SSRL - EXAFS Data Collector", XAS_read_dataset        },
  { "Offsets:",                    XAS_read_observations   },
  { "Data:",                       XAS_read_observations   },

  { NULL }

  };

#define read_line(line, file) XAS_read_line (line, file)

int XAS_read_line (char line [], FILE *file) {

  int i;

  line [0] = 0;

  if (feof (file)) return FALSE;

  fgets (line, 1024, file);

  if (strchr (line, '\n'))

    *strchr (line, '\n') = 0;

  for (i = strlen (line) - 1; i >= 0; i--)

    if (line [i] == ' ' || line [i] == '\t')

      line [i] = 0;

    else

      break;

  if (line [0] == 0)

    return FALSE;

  for (i = 0; XAS_block [i].title; i++)
 
    if (match (line, XAS_block [i].title))

      return FALSE;

  return TRUE;

  }


int read_XAS (char filename [], XAS_type *XAS) {

  /* Read a SSRL XAS ASCII data file.

     Sample file:
  
  -------------------------------------------------------------------------

   SSRL - EXAFS Data Collector 1.1       
   Fri Jul  7 18:22:57 1995              
   PTS: 778 COLS: 6                      
   TRANS.DET                             
   FEAK17.RGN                            
   7-3 unfocused  1.790 -5  Si(220)  1.920128     8000                           
   4 2000 25 RST 0 0                     
   Fe(OC2O)(1-MeIm)  powder, prepared by Carla Slebodnik, sample 1               
   trans, 10K                                                                    
   detuned 50% @8258eV, mono slits 1x20 mm, hutch slits 2x10 mm                  

   Weights: 
    1.000  1.000  1.000  1.000  1.000  1.000 
   Offsets: 
    0.000  0.000  0.000  605.500  777.100  458.700 
   Data: 
   Real time clock   
   Requested energy  
   Achieved energy   
   I0                
   I1                
   I2                

    1.000  6785.000  6785.006  51348.500  20077.900  23857.301 
    1.000  6791.000  6791.004  51274.500  20246.900  24138.301 
    1.000  6797.000  6796.987  51211.500  20413.900  24430.301 
    ...

    9.636  8217.626  8217.621  840554.188  1519570.500  326976.781 
    9.696  8224.165  8224.174  847690.750  1541202.875  333771.219 
    9.757  8230.723  8230.736  854569.250  1562531.000  340510.563 
    9.817  8237.301  8237.312  861395.625  1584104.000  347176.813 
    9.878  8243.897  8243.897  868359.875  1605956.875  354122.969 
    9.939  8250.515  8250.495  875441.000  1628318.500  361063.031 
    10.000  8257.151  8257.147  882360.000  1650380.000  368116.000 

  <<EOF>>

  -------------------------------------------------------------------------
  */

  FILE *file;

  char line [1024];

  int i;

  comment_line = 0;

    /* Set the MIDAS structure to empty. */

  empty_XAS (XAS);

  strcpy (XAS->filename, extract_file_name (filename));

  file = fopen (filename, "r");

  if (file == NULL)

    return FALSE;

  read_line (line, file);

  while (!feof (file)) {

    for (i = 0; XAS_block [i].title; i++)

      if (match (line, XAS_block [i].title))

        break;

    if (XAS_block [i].title)

      XAS_block [i].read (line, file, XAS);

    else

      read_line (line, file);

    }

  fclose (file);

  strcpy (XAS->type, "XAS");

  return TRUE;

  }

int XAS_ignore (char line [], FILE *file, XAS_type *XAS) {

    /* Ignore the current command line. */

  read_line (line, file);

  return TRUE;

  }

int XAS_read_dataset (char line [], FILE *file, XAS_type *XAS) {

    /* Read the "SSRL - EXAFS Data Collector 1.1" block. */

  char D [5][16];

  int day, year;

  if (!read_line (line, file))

    return TRUE;

  strcpy (XAS->date, "00-000-0000");
  strcpy (XAS->time, "00:00:00.00");

  sscanf (line, " %3s %3s %d %8s %d", D [0], D [1], &day, D [3], &year);

  strcpy (XAS->time, D [3]);
  strcpy (XAS->time + 8, ".00");

  sprintf (XAS->date, "%02d-%s-%4d", day, D [1], year);

  do

    if (!read_line (line, file))

      return TRUE;

    while (!match (line, "PTS:"));

  sscanf (line, " PTS: %d COLS: %d", &(XAS->N), &(XAS->channels));


    /* Read comments */

  while (read_line (line, file))

    add_comment (&XAS->comment, ++comment_line, 0, line);

  return TRUE;

  }

int XAS_read_observations (char line [], FILE *file, XAS_type *XAS) {

    /* read the "Offsets:" or "Data:" blocks. */ 


  int i, j, m, n, channel, n_used, N, first;

  double channels [XAS_MAXCHANNELS];

  static double offset [XAS_MAXCHANNELS] = { 0 };

  if (match (line, "Offsets:")) {

    if (!read_line (line, file))

      return TRUE;

    for (i = 0; i < XAS_MAXCHANNELS; i++)

      offset [i] = 0;

    sscan (line, " %l[%n ]f", XAS_MAXCHANNELS, offset);

    return FALSE;

    }


    /* Read the channel headings */

  for (channel = 0; channel < XAS_MAXCHANNELS; channel++) {

    if (!read_line (line, file))

      break;

    sscanf (line, " %15s", XAS->channel_label [channel]);

    }

  XAS->channels = channel;


    /* Read the data */

  N = XAS->N;

  channel = XAS->channels;

  XAS->N = 0;

  for (i = 0; i < M_MAXCHANNELS; i++)

    free_check (XAS->channel [i]);

  /* printf (" N = %d, channels = %d\n", N, channel); */

  for (i = 0; i < XAS->channels; i++)

    XAS->channel [i] = calloc_check (N, sizeof (double));

  first = TRUE;

  for (i = 0; i < N; i++) {

    if (!read_line (line, file))

      if (i > 0)

        break;

      else

        if (!read_line (line, file))

          break;

    if (channel = sscan (line, " %l[%n ]f", XAS->channels, channels)) {

      /* printf ("\"%s\"\n", line);

      printf (" %d: ", channel);

      for (j = 0; j < XAS->channels; j++)

        printf (" %f", channels [j]);

      printf ("\n"); */

      for (j = 0; j < XAS->channels; j++)

        XAS->channel [j][i] = channels [j] - offset [j];

      }
    }

  XAS->N = i;

  return TRUE;

  }

#undef read_line

#endif /* __XAS_H__ */
