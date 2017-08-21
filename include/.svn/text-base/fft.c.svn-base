/***********************************************************************}
{                                                                       }
{  FFT version 3.0 (C)                                                  }
{                                                                       }
{  This include file contains routines used to do fast discrete         }
{  Fourier and inverse-Fourier transforms on arrays of type double      }
{  or complex.                                                          }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1993.             }
{                                                                       }
{***********************************************************************/

#if !defined (__FFT_C__)

#define __FFT_C__

#include "fft.h"

#include "console.h"


const unsigned int PRIME [] =

           {   2,   3,   5,   7,  11,  13,  17,  19,  23,  29,  31,  37,
              41,  43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,
              97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
             157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223,
             227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
             283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359,
             367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433,
             439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503,
             509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593,
             599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659,
             661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743,
             751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827,
             829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911,
             919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997, 0 };

           /* This list allows complete factorisation up to 1018080 */

typedef struct {

  int N, factors, * factor, * reverse;

  complex * e;

  } FFT_data_type;

  /* Global FFT variables */

#define FFT_DATA_SPACE_STEP 16
#define FFT_N_MAX_STEP 1024

long            FFT_data_space = 0, FFT_datas = 0, FFT_N_max = 0;

FFT_data_type * FFT_data = NULL;
complex       * FFT_tmp  = NULL;


  /* setup_FFT_data sets up the arrays necessary to do an
     FFT on a complex array of length N.                  */

int setup_FFT_data (FFT_data_type data [], int N) {

  int i, j, maxf, factors, factor [64], a, b;

  if (N <= 0) {

    cwarn ("setup_FFT_data : invalid N (%d)", N);

    return 0;

    }

  data->N = N;


    /* first factorise N */

  factors = 0;

  factor [0] = N;

  for (i = 0, maxf = sqrt (N + 1);
              (PRIME [i] && PRIME [i] <= maxf && (N > 1));)

    if ((N % PRIME [i]) == 0) {

      factor [++factors] = PRIME [i];

      N /= PRIME [i];

      }
    else

      i++;

  if (N > 1 || factors == 0)

    factor [++factors] = N;

  N = data->N;

  data->factor  = calloc (factors + 1, sizeof (int));
  data->reverse = calloc (          N, sizeof (int));

  data->e       = (complex *) calloc (N, sizeof (complex));

  if (!data->factor || !data->reverse || !data->e) {

    cwarn ("setup_FFT_data : can't allocate :"
           " factor : %1d x %1d - reverse : %1d x %1d - e : %1d x %1d",
             factors + 1, sizeof (int), N, sizeof (int), N, sizeof (complex));

    if (data->e)

      free (data->e);

    if (data->reverse)

      free (data->reverse);

    if (data->factor)

      free (data->factor);

    return 0;

    }

  for (i = 0; i <= factors; i++)

    data->factor [i] = factor [i];

  data->factors = factors;


    /* generate reversing array */

  for (i = 0; i < N; i++) {

    a = i;
    b = 0;

    for (j = 1; j <= factors; j++) {

      b = (b * data->factor [j]) + (a % data->factor [j]);
      a /= data->factor [j];

      }

    data->reverse [i] = b;

    }


    /* now generate array of e ** (2PIi * (0 .. N - 1) / N) values. */

  for (i = 0; i < N; i++) {

    data->e [i].Re = cos (i * (2 * M_PI) / N);
    data->e [i].Im = sin (i * (2 * M_PI) / N);

    }

  return N;

  }

FFT_data_type * get_FFT_data (int N) {

  int i, j;

  FFT_data_type data_tmp;

  for (i = 0; i < FFT_datas; i++)

    if (N <= FFT_data [i].N)

      break;

  if (i < FFT_datas)

    if (N == FFT_data [i].N)

      return FFT_data + i;

  if (FFT_data_space == 0 || FFT_data_space <= FFT_datas) {

    FFT_data_type * FFT_data_tmp;

    if ((FFT_data_tmp = calloc (FFT_data_space + FFT_DATA_SPACE_STEP,
                                 sizeof (FFT_data_type))) == NULL)

      return NULL;

    for (i = 0; i < FFT_datas; i++)

      FFT_data_tmp [i] = FFT_data [i];

    if (FFT_data)

      free (FFT_data);

    FFT_data = FFT_data_tmp;
    FFT_data_space += FFT_DATA_SPACE_STEP;

    }

  if (!setup_FFT_data (&data_tmp, N))

    return NULL;

  for (j = FFT_datas; j > i; j--)

    FFT_data [j] = FFT_data [j - 1];

  FFT_data [i] = data_tmp;

  FFT_datas++;

  if (N > FFT_N_max) {

    N = ((N + FFT_N_MAX_STEP - 1) / FFT_N_MAX_STEP) * FFT_N_MAX_STEP;

    if (FFT_tmp)

      free (FFT_tmp);

    if ((FFT_tmp = (complex *) calloc (N, sizeof (complex))) == NULL)

      return NULL;

    FFT_N_max = N;

    }

  return FFT_data + i;

  }


  /*********************************************************************}
  {                                                                     }
  {  inverse_Fourier_transform does the fast Fourier transform of       }
  {  the complex array c :                                              }
  {                                                                     }
  {                                      +2PIi h.(x / N)                }
  {  c [h] = Sum (x = 0 .. N - 1) c [x] e                               }
  {                                                                     }
  {*********************************************************************/

int inverse_Fourier_transform (complex * c, int N) {

  int * reverse, base, prev_base, factor, a, b, ei, ej, estep, x, xN, m, h;

  complex * e, C;

  FFT_data_type * data;


    /* Check N */

  if (N <= 0) {

    cwarn ("inverse_Fourier_transform : invalid N (%d) in FFT call", N);

    return 0;

    }


    /* Get the data for this transform */

  if ((data = get_FFT_data (N)) == NULL) {

    cwarn ("inverse_Fourier_transform : cannot get FFT data for N = %d", N);

    return 0;

    }

  reverse = data->reverse;
  e       = data->e;

  base = N;

  for (m = data->factors; m > 0; m--) {

    factor = data->factor [m];
    prev_base = base;
    base /= factor;
    xN = N / factor;

    if (factor == 2)

      for (b = 0; b < N; b += prev_base) {

        for (a = 0; a < base; a++) {

          C.Re = c [a + b + base].Re * e [reverse [b] * base].Re -
                 c [a + b + base].Im * e [reverse [b] * base].Im;
          C.Im = c [a + b + base].Re * e [reverse [b] * base].Im +
                 c [a + b + base].Im * e [reverse [b] * base].Re;

          c [a + b + base].Re = c [a + b].Re - C.Re;
          c [a + b + base].Im = c [a + b].Im - C.Im;
          c [a + b       ].Re += C.Re;
          c [a + b       ].Im += C.Im;

          }
        }

    else

      for (b = 0; b < N; b += prev_base) {

        estep = reverse [b] * base;

        for (a = 0; a < base; a++) {

          for (x = 0; x < factor; x++) {

            FFT_tmp [x] = c [a + b + x * base];
            c [a + b + x * base] = FFT_tmp [0];

            }

          for (h = 0, ej = estep; h < factor; h++, ej += xN)

            for (x = 1, ei = ej; x < factor; x++, ei = (ei + ej) % N) {

              C.Re = FFT_tmp [x].Re * e [ei].Re - FFT_tmp [x].Im * e [ei].Im;
              C.Im = FFT_tmp [x].Re * e [ei].Im + FFT_tmp [x].Im * e [ei].Re;

              c [a + b + h * base].Re += C.Re;
              c [a + b + h * base].Im += C.Im;

              }
          }
        }
    }

  for (x = 1; x < N - 1; x++)

    FFT_tmp [x] = c [x];

  for (x = 1; x < N - 1; x++)

    c [reverse [x]] = FFT_tmp [x];


    /* c is the inverse fourier transform of the array passed. */

  return N;

  }


  /*********************************************************************}
  {                                                                     }
  {  Fourier_transform does the fast Fourier transform of the           }
  {  complex array c :                                                  }
  {                                                                     }
  {                                      -2PIi h.(x / N)                }
  {  c [h] = Sum (x = 0 .. N - 1) c [x] e                               }
  {                                                                     }
  {*********************************************************************/

int Fourier_transform (complex * c, int N) {

  int i, n;

  for (i = 0; i < N; i++)

    c [i].Im = -c [i].Im;

  n = inverse_Fourier_transform (c, N);

  for (i = 0; i < N; i++)

    c [i].Im = -c [i].Im;

  if (n == 0) {

    cwarn ("Fourier_transform : cannot do transform");

    return 0;

    }

  return N;

  }


  /*********************************************************************}
  {                                                                     }
  {  Transform a real array of length N, returning half + 1 of the      }
  {  hermitian-symmetric complex transform.                             }
  {                                                                     }
  {*********************************************************************/

int Fourier_transform_real (complex * c, int N) {


  complex a, b, C, * e_N;

  int i;

  FFT_data_type * data;

  if (N <= 0 || (N % 2) != 0) {

    if ((N % 2) != 0)

      cwarn ("Fourier_transform_real : invalid N (%d) for real transform (non-even)", N);

    else
 
      cwarn ("Fourier_transform_real : invalid N (%d) for real transform", N);

    return 0;

    }

  data = get_FFT_data (N);

  if (data == NULL) {

    cwarn ("Fourier_transform_real : cannot get FFT data for N = %d", N);

    return 0;

    }

  e_N = data->e;

  if (!inverse_Fourier_transform (c, N / 2))

    return 0;

  N /= 2;

  c [N].Re = c [0].Re - c [0].Im;
  c [0].Re = c [0].Re + c [0].Im;
  c [N].Im = 0;
  c [0].Im = 0;

  for (i = 1; i <= N / 2; i++) {

    a.Im =  (c [i].Im - c [N - i].Im) / 2;
    a.Re =  (c [i].Re + c [N - i].Re) / 2;
    b.Im = -(c [i].Re - c [N - i].Re) / 2;
    b.Re =  (c [i].Im + c [N - i].Im) / 2;

    C.Re = b.Re * e_N [i].Re - b.Im * e_N [i].Im;
    C.Im = b.Re * e_N [i].Im + b.Im * e_N [i].Re;

    c [i].Re =   a.Re + C.Re;
    c [i].Im = -(a.Im + C.Im);
    c [N - i].Re = a.Re - C.Re;
    c [N - i].Im = a.Im - C.Im;

    }

  return N;

  }

int inverse_Fourier_transform_hermi (complex * c, int N) {

  complex a, b, C, * e_N;

  int i;

  FFT_data_type * data;

  if (N % 2 != 0) {

    cwarn ("inverse_Fourier_transform_hermitian : N must be even (N = %d)", N);

    return 0;

    }

  data = get_FFT_data (N);

  if (data == NULL) {

    cwarn ("inverse_Fourier_transform_hermitian : cannot get FFT data for N = %d", N);

    return 0;

    }

  e_N = data->e;

  N /= 2;

  a.Re = c [0].Re;

  c [0].Re = c [0].Re + c [0].Im;
  c [0].Im = a.Re     - c [0].Im;

  for (i = 1; i <= N / 2; i++) {

    a.Re = c [i].Re + c [N - i].Re;
    a.Im = c [i].Im - c [N - i].Im;
    b.Re = c [i].Re - c [N - i].Re;
    b.Im = c [i].Im + c [N - i].Im;

    C.Re = e_N [i].Re * b.Re - e_N [i].Im * b.Im;
    C.Im = e_N [i].Re * b.Im + e_N [i].Im * b.Re;

    c [i].Re = a.Re - C.Im;
    c [i].Im = a.Im + C.Re;
    c [N - i].Re =  a.Re + C.Im;
    c [N - i].Im = -a.Im + C.Re;

    }

  if (!inverse_Fourier_transform (c, N)) {

    cwarn ("inverse_Fourier_transform_hermitian : cannot do transform");

    return 0;

    }

  return 2 * N;

  }

#endif /* __FFT_C__ */
