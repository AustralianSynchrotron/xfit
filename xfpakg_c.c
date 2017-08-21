/***********************************************************************}
{                                                                       }
{  Single-scattering XFPAKG exafs calculation for xfit                  }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__XFPAKG_C_C)

#define __XFPAKG_C_C

#include "xfpakg_c.h"

#include "console.h"


#define _SIZE_      16
#define _SIZE0_     2
#define _K_        -1
#define _E0_        0
#define _shells_    1
#define _Z_(i)      (_SIZE0_ + _SIZE_ * (i) + 0)
#define _N_(i)      (_SIZE0_ + _SIZE_ * (i) + 1)
#define _x_(i)      (_SIZE0_ + _SIZE_ * (i) + 2)
#define _y_(i)      (_SIZE0_ + _SIZE_ * (i) + 3)
#define _z_(i)      (_SIZE0_ + _SIZE_ * (i) + 4)
#define _sigma2_(i) (_SIZE0_ + _SIZE_ * (i) + 5)
#define _scale_(i)  (_SIZE0_ + _SIZE_ * (i) + 6)
#define _trans_(i)  (_SIZE0_ + _SIZE_ * (i) + 7)
#define _c0_(i)     (_SIZE0_ + _SIZE_ * (i) + 8)
#define _c1_(i)     (_SIZE0_ + _SIZE_ * (i) + 9)
#define _c2_(i)     (_SIZE0_ + _SIZE_ * (i) + 10)
#define _c3_(i)     (_SIZE0_ + _SIZE_ * (i) + 11)
#define _a_1_(i)    (_SIZE0_ + _SIZE_ * (i) + 12)
#define _a0_(i)     (_SIZE0_ + _SIZE_ * (i) + 13)
#define _a1_(i)     (_SIZE0_ + _SIZE_ * (i) + 14)
#define _a2_(i)     (_SIZE0_ + _SIZE_ * (i) + 15)


  /*********************************************************************}
  {                                                                     }
  {  To match the conventional use of the "shells" parameter:           }
  {                                                                     }
  {  "shells" = maximum shell index.                                    }
  {                                                                     }
  {  The parameters common to each shell in the model are:              }
  {                                                                     }
  {               default                                               }
  {                                                                     }
  {    E0      :  0                                                     }
  {    SHELL0  :  0                                                     }
  {                                                                     }
  {  Each shell has the following parameters:                           }
  {                                                                     }
  {               default                                               }
  {                                                                     }
  {    Z       :  0                                                     }
  {    N       :  1                                                     }
  {    x       :  0                                                     }
  {    y       :  0                                                     }
  {    z       :  0                                                     }
  {    sigma2  :  0                                                     }
  {    scale   :  1                                                     }
  {    trans   :  1                                                     }
  {    c(0)    :  1                                                     }
  {    c(1)    :  0                                                     }
  {    c(2)    :  0                                                     }
  {    c(3)    :  0                                                     }
  {    a(-1)   :  0                                                     }
  {    a(0)    :  0                                                     }
  {    a(1)    :  0                                                     }
  {    a(2)    :  0                                                     }
  {                                                                     }
  {*********************************************************************/

#define M MODEL
#define I IDENTIFIER
#define V VECTOR_ID

#define SC NORMAL
#define AT ATOM_t
#define VE VECTOR
#define PO PVECTOR

parameter_type XFPAKG_parameter_name [] =

  { { "E0",          _E0_,         0,      0, 0, M, 0, I, SC, 0, 1 },
    { "E0(0)",       _E0_,         0,      0, 0, M, 0, I, SC, 0, 1 },

    { "SHELLS",      _shells_,     0,      0, 0, M, 0, I, SC, 0, 0 },

    { "SHELL(%d)",   _Z_ (0),      0, _SIZE_, 0, M, 0, I, AT, 0, 0 },
    { "(Z)(%d)",     _Z_ (0),      0, _SIZE_, 0, M, 0, I, AT, 0, 0 },
    { "N(%d)",       _N_ (0),      0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "x(%d)",       _x_ (0),      0, _SIZE_, 1, M, 0, I, VE, 0, 1 },
    { "y(%d)",       _x_ (0),      0, _SIZE_, 2, M, 0, I, VE, 0, 1 },
    { "z(%d)",       _x_ (0),      0, _SIZE_, 3, M, 0, I, VE, 0, 1 },
    { "v(%d)",       _x_ (0),      0, _SIZE_, 0, M, 0, V, VE, 0, 1 },
    { "r(%d)",       _x_ (0),      0, _SIZE_, 1, M, 0, I, PO, 0, 1 },
    { "T(%d)",       _x_ (0),      0, _SIZE_, 2, M, 0, I, PO, 0, 1 },
    { "P(%d)",       _x_ (0),      0, _SIZE_, 3, M, 0, I, PO, 0, 1 },
    { "Q(%d)",       _x_ (0),      0, _SIZE_, 0, M, 0, V, PO, 0, 1 },
    { "sigma2(%d)",  _sigma2_ (0), 0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "s(%d)",       _sigma2_ (0), 0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "scale(%d)",   _scale_ (0),  0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "trans(%d)",   _trans_ (0),  0, _SIZE_, 0, M, 0, I, SC, 0, 1 },

    { "CENTRAL",     _Z_ (0),      0,      0, 0, M, 0, I, AT, 0, 0 },

    { "SHELL%d",     _Z_ (0),      0, _SIZE_, 0, M, 0, I, AT, 0, 0 },
    { "(Z)%d",       _Z_ (0),      0, _SIZE_, 0, M, 0, I, AT, 0, 0 },
    { "N%d",         _N_ (0),      0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "x%d",         _x_ (0),      0, _SIZE_, 1, M, 0, I, VE, 0, 1 },
    { "y%d",         _x_ (0),      0, _SIZE_, 2, M, 0, I, VE, 0, 1 },
    { "z%d",         _x_ (0),      0, _SIZE_, 3, M, 0, I, VE, 0, 1 },
    { "v%d",         _x_ (0),      0, _SIZE_, 0, M, 0, V, VE, 0, 1 },
    { "r%d",         _x_ (0),      0, _SIZE_, 1, M, 0, I, PO, 0, 1 },
    { "T%d",         _x_ (0),      0, _SIZE_, 2, M, 0, I, PO, 0, 1 },
    { "P%d",         _x_ (0),      0, _SIZE_, 3, M, 0, I, PO, 0, 1 },
    { "Q%d",         _x_ (0),      0, _SIZE_, 0, M, 0, V, PO, 0, 1 },
    { "sigma2%d",    _sigma2_ (0), 0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "s%d",         _sigma2_ (0), 0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "scale%d",     _scale_ (0),  0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "trans%d",     _trans_ (0),  0, _SIZE_, 0, M, 0, I, SC, 0, 1 },

    { "c0(%d)",      _c0_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "c1(%d)",      _c1_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "c2(%d)",      _c2_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "c3(%d)",      _c3_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "a-1(%d)",     _a_1_ (0),    0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "a0(%d)",      _a0_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "a1(%d)",      _a1_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "a2(%d)",      _a2_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },

    { "c(0)(%d)",    _c0_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "c(1)(%d)",    _c1_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "c(2)(%d)",    _c2_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "c(3)(%d)",    _c3_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "a(-1)(%d)",   _a_1_ (0),    0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "a(0)(%d)",    _a0_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "a(1)(%d)",    _a1_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "a(2)(%d)",    _a2_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },

    { "c(0)%d",      _c0_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "c(1)%d",      _c1_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "c(2)%d",      _c2_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "c(3)%d",      _c3_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "a(-1)%d",     _a_1_ (0),    0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "a(0)%d",      _a0_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "a(1)%d",      _a1_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "a(2)%d",      _a2_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },

    { "c0%d",        _c0_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "c1%d",        _c1_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "c2%d",        _c2_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "c3%d",        _c3_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "a-1%d",       _a_1_ (0),    0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "a0%d",        _a0_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "a1%d",        _a1_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },
    { "a2%d",        _a2_ (0),     0, _SIZE_, 0, M, 0, I, SC, 0, 1 },

    { 0 } };

#undef M
#undef I
#undef V

#undef SC
#undef AT
#undef VE
#undef PO


  /*********************************************************************}
  {                                                                     }
  {  Define a structure matching the arrangement of the vector          }
  {                                                                     }
  {*********************************************************************/

typedef struct {

  double Z, N, x, y, z, sigma2, scale, trans,

         c0, c1, c2, c3, a_1, a0, a1, a2;

  } XFPAKG_shell_type;

typedef struct {

  double E0, shells;

  XFPAKG_shell_type shell [2];

  } XFPAKG_parameters_type;


#define XFPAKG_TMP_SIZE_STEP 1024

long __XFPAKG_tmp_size = 0;

double * __XFPAKG_tmp = NULL;

long resize_XFPAKG_tmp (long N) {

  return (realloc_increment ((void **) &__XFPAKG_tmp, &__XFPAKG_tmp_size,

                             N, sizeof (double), XFPAKG_TMP_SIZE_STEP));

  }


  /*********************************************************************}
  {                                                                     }
  {  Calculate the exafs                                                }
  {                                                                     }
  {*********************************************************************/

int XFPAKG_sincos_EXAFS (vector v, double polar [],

                 int N, double k_ [], double sin_chi [], double cos_chi []) {

  /*

              CN*Scf*Trf
  EXAFS = p * ---------- * Amp(k) * exp[-2 sigma2 * k**2] * sin[2 k R + Phs(k)]
               k*(R**2)

        p = 1 if unpolarised, 3 * (x.polar / |x|)**2  otherwise

        Amp(k) = c0 * exp[c1*k + c2*(k**2)] * k**c3
        Phs(k) = a(-1)/k + a0 + a1*k + a2*(k**2)

  k <= 0 -> EXAFS = 0

  k is assumed to be monotonic increasing

  */

  int shell, shells, i, N0;

  double k, r, amp, phs, C, C2, p, E0, k0;

  XFPAKG_parameters_type * X;

  XFPAKG_shell_type S_0, S;


  X = (XFPAKG_parameters_type *) v.i;

  shells = X->shells;

  if (shells <= 0)

    return 0;

  shells++;

  k0 = k_EXAFS (E0 = X->E0);

  for (N0 = N - 1; N0 >= 0; N0--)

    if (k_ [N0] <= k0)

      break;

  N0++;

  if (sin_chi)

    for (i = 0; i < N; i++)

      sin_chi [i] = 0.0;

  if (cos_chi)

    for (i = 0; i < N; i++)

      cos_chi [i] = 0.0;

  S_0 = X->shell [0];

  for (shell = 1; shell < shells; shell++) {

    S = X->shell [shell];

    S.x -= S_0.x;
    S.y -= S_0.y;
    S.z -= S_0.z;

    S.sigma2 += S_0.sigma2;

    r = sqrt (S.x * S.x + S.y * S.y + S.z * S.z);

    if (polar) {

      p = (S.x * polar [0] + S.y * polar [1] + S.z * polar [2]) / r;

      p = 3 * p * p;

      }
    else

      p = 1;

    C = S.N * S.scale * S.trans / (r * r);

    C2 = S.c2 - 2 * S.sigma2;

    for (i = N0; i < N; i++) {

      k = k_EXAFS (E_EXAFS (k_ [i]) - E0);

      amp = S.c0 * exp ((S.c1 + C2 * k) * k) * pow (k, S.c3);

      phs = S.a_1 / k + S.a0 + (S.a1 + S.a2 * k) * k;

      if (sin_chi)

        sin_chi [i] += p * C / k * amp * sin (2 * k * r + phs);

      if (cos_chi)

        cos_chi [i] += p * C / k * amp * cos (2 * k * r + phs);

      }
    }

  return N;

  }


  /*********************************************************************}
  {                                                                     }
  {  Calculate derivatives of exafs wrt a parameter                     }
  {                                                                     }
  {*********************************************************************/

int XFPAKG_dE_dx (vector v, double polar [],

                        int parameter, int N, double k_ [], double dchi []) {

  /*
              CN*Scf*Trf
  EXAFS = p * ---------- * Amp(k) * exp[-2 sigma2 * k**2] * sin[2 k R + Phs(k)]
               k*(R**2)

        p = 1 if unpolarised, 3 * (x.polar / |x|)**2  otherwise

        Amp(k) = c0 * exp[c1*k + c2*(k**2)] * k**c3
        Phs(k) = a-1/k + a0 + a1*k + a2*(k**2)

        R = |x|

  cos_EXAFS = EXAFS * cot[2 k R + Phs(k)]

  dEXAFS / CN, Scf, Trf = EXAFS / CN, Scf, Trf

  dEXAFS / dx [0] = dp / dx [0] * EXAFS / p + dEXAFS / dR * dR / dx [0]

       dR / dx [0] = x [0] / R

       dp / dx [0] = 3 * 2 * (x.polar / |x|**2) *
                         (polar [0] - x [0] * x.polar / |x|**2)

       dEXAFS / dR = EXAFS * (-2 / R) + cos_EXAFS * 2 * k

  dEXAFS / dk = sum [ EXAFS * ((c3 - 1) / k + c1 + (2 c2 - 4 sigma**2) k)
                      + cos_EXAFS * (2 R + -a(-1) / k**2 + a1 + 2 k a2) ]

  dEXAFS / sigma**2 = EXAFS * -2 * k * k

  dEXAFS / dc0 = EXAFS / c0

  dEXAFS / dc1 = EXAFS * k

  dEXAFS / dc2 = EXAFS * k * k

  dEXAFS / dc3 = EXAFS * ln (k)

  dEXAFS / da(-1) = cos_EXAFS / k

  dEXAFS / da0 = cos_EXAFS

  dEXAFS / da1 = cos_EXAFS * k

  dEXAFS / da2 = cos_EXAFS * k * k

  */

  int shell, shells, i, N0;

  double amp, phs, C, C2, p, dp, r, dr, r2, * cos_EXAFS, E0, k0, k;

  XFPAKG_parameters_type * X, Xs;

  XFPAKG_shell_type S;


  X = (XFPAKG_parameters_type *) v.i;

  shells = X->shells;

  if (shells <= 0)

    return 0;

  shells++;


  for (i = 0; i < N; i++)

    dchi [i] = 0.0;


  Xs.E0 = X->E0;

  Xs.shell [0] = X->shell [0];

  Xs.shells = 1;

  k0 = k_EXAFS (E0 = X->E0);

  for (N0 = N - 1; N0 >= 0; N0--)

    if (k_ [N0] <= k0)

      break;

  N0++;

  if (parameter == _K_ || parameter == _E0_) {

      /* dchi / dk */

    if (!resize_XFPAKG_tmp (N))

      return 0;

    v.i = (double *) &Xs;

    for (shell = 1; shell < shells; shell++) {

      Xs.shell [1] = S = X->shell [shell];

      r = sqrt (S.x * S.x + S.y * S.y + S.z * S.z);

      N = XFPAKG_sincos_EXAFS (v, polar, N, k_, __XFPAKG_tmp, NULL);

      for (i = N0; i < N; i++) {

        k = k_EXAFS (E_EXAFS (k_ [i]) - E0);

        dchi [i] += __XFPAKG_tmp [i] *

              ((S.c3 - 1) / k + S.c1 + (2 * S.c2 - 4 * S.sigma2) * k);

        }

      N = XFPAKG_sincos_EXAFS (v, polar, N, k_, NULL, __XFPAKG_tmp);

      for (i = N0; i < N; i++) {

        k = k_EXAFS (E_EXAFS (k_ [i]) - E0);

        dchi [i] += __XFPAKG_tmp [i] *

                  (2 * r - S.a_1 / (k * k) + S.a1 + 2 * k * S.a2);

        }
      }

    if (parameter == _K_)

        /* dchi / dk(in) = dchi / dk(internal) * dk(internal) / dk(in) */

      for (i = N0; i < N; i++) {

        k = k_EXAFS (E_EXAFS (k_ [i]) - E0);

        dchi [i] *= k_ [i] / k;

        }

    if (parameter == _E0_)

        /* dchi / dE0 = dchi / dk * dk / dE0 */

      for (i = N0; i < N; i++) {

        k = k_EXAFS (E_EXAFS (k_ [i]) - E0);

        dchi [i] *= (-0.5 * 0.5123143 * 0.5123143) / k;

        }

    return N;

    }

  for (shell = 0; shell < shells; shell++)

    if (parameter >= _N_ (shell) && parameter <= _a2_ (shell))

      break;

  if (shell >= shells)

    return 0;

  if (shell == 0) {

    cwarn ("no dX / dcentral yet");

    return 0;

    }

  Xs.shell [1] = S = X->shell [shell];

  v.i = (double *) &Xs;

  if (parameter == _N_     (shell) ||
      parameter == _scale_ (shell) ||
      parameter == _trans_ (shell) ||
      parameter == _c0_    (shell)) {

    N = XFPAKG_sincos_EXAFS (v, polar, N, k_, dchi, NULL);

    for (i = N0; i < N; i++)

      dchi [i] /= v_el (v, parameter - _N_ (shell) + _N_ (1));

    return N;

    }

  if (parameter == _c1_ (shell)) {

    N = XFPAKG_sincos_EXAFS (v, polar, N, k_, dchi, NULL);

    for (i = N0; i < N; i++) {

      k = k_EXAFS (E_EXAFS (k_ [i]) - E0);

      dchi [i] *= k;

      }

    return N;

    }

  if (parameter == _c2_ (shell)) {

    N = XFPAKG_sincos_EXAFS (v, polar, N, k_, dchi, NULL);

    for (i = N0; i < N; i++) {

      k = k_EXAFS (E_EXAFS (k_ [i]) - E0);

      dchi [i] *= k * k;

      }

    return N;

    }

  if (parameter == _c3_ (shell)) {

    N = XFPAKG_sincos_EXAFS (v, polar, N, k_, dchi, NULL);

    for (i = N0; i < N; i++) {

      k = k_EXAFS (E_EXAFS (k_ [i]) - E0);

      dchi [i] *= log (k);

      }

    return N;

    }

  if (parameter == _sigma2_ (shell)) {

    N = XFPAKG_sincos_EXAFS (v, polar, N, k_, dchi, NULL);

    for (i = N0; i < N; i++) {

      k = k_EXAFS (E_EXAFS (k_ [i]) - E0);

      dchi [i] *= -2 * k * k;

      }

    return N;

    }

  if (parameter == _x_ (shell) ||
      parameter == _y_ (shell) ||
      parameter == _z_ (shell)) {

    if (!resize_XFPAKG_tmp (N))

      return 0;


      /* dEXAFS / dx [0] = dp / dx [0] * EXAFS / p + dEXAFS / dR * dR / dx [0]

         dR / dx [0] = x [0] / R

         dp / dx [0] = 3 * 2 * (x.polar / |x|**2) *
                         (polar [0] - x [0] * x.polar / |x|**2)

         dEXAFS / dR = EXAFS * (-2 / R) + cos_EXAFS * 2 * k  */

    r = sqrt (r2 = (S.x * S.x + S.y * S.y + S.z * S.z));

    if (parameter == _x_ (shell)) { dr = S.x; dp = (polar ? polar [0] : 0); }
    if (parameter == _y_ (shell)) { dr = S.y; dp = (polar ? polar [1] : 0); }
    if (parameter == _z_ (shell)) { dr = S.z; dp = (polar ? polar [2] : 0); }

    if (polar) {

      p = S.x * polar [0] + S.y * polar [1] + S.z * polar [2];

      dp = 6 * p * (dp - dr * p / r2) / r2;

      p = 3 * p * p / r2;

      }

    dr /= r;

    N = XFPAKG_sincos_EXAFS (v, NULL, N, k_, dchi, __XFPAKG_tmp);

    if (polar)

      for (i = N0; i < N; i++) {

        k = k_EXAFS (E_EXAFS (k_ [i]) - E0);

        dchi [i] = dp * dchi [i] +
                   p * (dchi [i] * -2 / r + __XFPAKG_tmp [i] * 2 * k) * dr;

        }
    else

      for (i = N0; i < N; i++) {

        k = k_EXAFS (E_EXAFS (k_ [i]) - E0);

        dchi [i] = (dchi [i] * -2 / r + __XFPAKG_tmp [i] * 2 * k) * dr;

        }

    return N;

    }

  if (parameter == _a_1_ (shell)) {

    N = XFPAKG_sincos_EXAFS (v, polar, N, k_, NULL, dchi);

    for (i = N0; i < N; i++) {

      k = k_EXAFS (E_EXAFS (k_ [i]) - E0);

      dchi [i] /= k;

      }

    return N;

    }

  if (parameter == _a0_ (shell)) {

    N = XFPAKG_sincos_EXAFS (v, polar, N, k_, NULL, dchi);

    return N;

    }

  if (parameter == _a1_ (shell)) {

    N = XFPAKG_sincos_EXAFS (v, polar, N, k_, NULL, dchi);

    for (i = N0; i < N; i++) {

      k = k_EXAFS (E_EXAFS (k_ [i]) - E0);

      dchi [i] *= k;

      }

    return N;

    }

  if (parameter == _a2_ (shell)) {

    N = XFPAKG_sincos_EXAFS (v, polar, N, k_, NULL, dchi);

    for (i = N0; i < N; i++) {

      k = k_EXAFS (E_EXAFS (k_ [i]) - E0);

      dchi [i] *= k * k;

      }

    return N;

    }

  return 0;

  }


  /************************************}
  {                                    }
  {  Rationalise the parameter vector  }
  {                                    }
  {************************************/

int XFPAKG_rationalise (parameter_set * set) {

  static int shells_0 = -1;

  int shell, shells, shell_size, i, i0;

  vector * model, * model_0;


    /* Get the vectors */

  model   = parameter_vector_Oi (set, MODEL, 0);

  model_0 = parameter_vector_Oi (set, MODEL + COPY, 0);

  shells = shell_size = 0;

  i0 = model->rows;

  if (i0 > _shells_) {

    shells = v_el (*model, _shells_);

    shell_size = (i0 - _SIZE0_ - 1) / _SIZE_;

    }

  if (shells == shells_0)

    shells = max (shell_size, 0);

  shells++;


    /* Total number of shells (including shell 0) */

  resize_parameter_vector (model  , _SIZE0_ + shells * _SIZE_);

  resize_parameter_vector (model_0, _SIZE0_ + shells * _SIZE_);

  for (i = i0; i < model->rows; i++)

    v_el (*model, i) = v_el (*model_0, i) = 0;

  i0--;


    /* Default values */

  for (shell = shells_0 + 1; shell < shells; shell++) {

    if (_N_ (shell) != i0)

      v_el (*model, _N_ (shell)) = v_el (*model_0, _N_ (shell)) = 1;

    if (_scale_ (shell) != i0)

      v_el (*model, _scale_ (shell)) = v_el (*model_0, _scale_ (shell)) = 1;

    if (_trans_ (shell) != i0)

      v_el (*model, _trans_ (shell)) = v_el (*model_0, _trans_ (shell)) = 1;

    if (_c0_ (shell) != i0)

      v_el (*model, _c0_ (shell)) = v_el (*model_0, _c0_ (shell)) = 1;

    }

  for (shell = 0; shell < shells; shell++)

    v_el (*model, _Z_ (shell)) = rint (v_el (*model, _Z_ (shell)));

  shells--;

  v_el (*model, _shells_) = v_el (*model_0, _shells_) = shells_0 = shells;

  return shells;

  }


  /*********************************************************************}
  {                                                                     }
  {  Each exafs calculation uses two sets of parameters:                }
  {                                                                     }
  {    (1) model:                                                       }
  {                                                                     }
  {               E0, SHELL0, SHELLS                                    }
  {                                                                     }
  {             [ Z, N, x, y, z, sigma2, scale, trans,                  }
  {                                                                     }
  {               c0, c1, c2, c3, a_1, a0, a1, a2 ]                     }
  {                                                                     }
  {    (2) The data set parameters                                      }
  {                                                                     }
  {               ZA, hole                                              }
  {                                                                     }
  {             [ polar, normal, ellipticity, Q ]                       }
  {                                                                     }
  {  Together these make up the parameter vector of size                }
  {                                                                     }
  {               SHELLS * 26 - 13                                      }
  {                                                                     }
  {  Only the first polarisation vector is used                         }
  {                                                                     }
  {  normal, ellipticity and Q are ignored                              }
  {                                                                     }
  {  The polarisation and Q cannot be refined                           }
  {                                                                     }
  {*********************************************************************/

int XFPAKG_calculate (vector model,

                        int ZA, int hole,

                        int N_Q, double polar [], double normal [],
                                 double ellipticity [], double Q [],

                        int N, double k [], double chi [],

                        double * dchidv [], matrix dv,

                        int status) {

  /*********************************************************************}
  {                                                                     }
  {  This function returns :                                            }
  {                                                                     }
  {     (1) exafs                                                       }
  {                                                                     }
  {     (2) derivatives of exafs with respect to the columns of dv      }
  {                                                                     }
  {     chi        = NULL : no exafs                                    }
  {                                                                     }
  {     dchidv     = NULL : no derivatives                              }
  {                                                                     }
  {     dchidv [i] = NULL : no derivative w.r.t. dv [..][i]             }
  {                                                                     }
  {     dv [i]     = 0    : the matrix 1 is used.                       }
  {                                                                     }
  {  dchidv [-1] is dchi / dk                                           }
  {                                                                     }
  {  status: -1: clean up any temporary files.                          }
  {                                                                     }
  {           0: normal call.                                           }
  {                                                                     }
  {           1: recalculate the paths.                                 }
  {                                                                     }
  {           2: don't print to the console.                            }
  {                                                                     }
  {*********************************************************************/

  int error, parameter, parameters, derivative, derivatives,

      first, dv_is_1, i;

  double m, * dchi, polar_t [3];


  if (status < 0) {

    return 1;

    }

  if (N <= 0)

    return 0;


  if (polar && N_Q) {

    polar_t [0] = polar [0];
    polar_t [1] = polar [1];
    polar_t [2] = polar [2];

    polar = polar_t;

    if (normalise3 (polar) == 0)

      polar = 0;

    }
  else

    polar = 0;


    /* Calculate the EXAFS */

  if (chi != NULL)

    if (!XFPAKG_sincos_EXAFS (model, polar, N, k, chi, NULL)) {

      cwarn ("XFPAKG_calculate : cannot calculate chi");

      N = -1;

      }


    /* Calculate the derivatives */

  if (dchidv && N > 0) {

    dchi = calloc_check (N, sizeof (double));

    error = 0;

    if (dchidv [-1]) {

      if (!XFPAKG_dE_dx (model, polar, -1, N, k, dchidv [-1]))

        error = -1;

      }

    dv_is_1 = dv.rows <= 0 || dv.cols <= 0;

    if (dv_is_1) {

      derivatives =

      parameters  = model.rows;

      }
    else {

      derivatives = dv.cols;

      parameters = min (dv.rows, model.rows);

      }


      /* Go through the parameters, adding the derivatives w.r.t the
         model parameters to the derivatives w.r.t. the vectors.     */

    for (parameter = 0; parameter < parameters && !error; parameter++) {

      first = 1;

      for (derivative = 0; derivative < derivatives; derivative++) {

        if (dv_is_1)

          m = parameter == derivative;

        else

          m = m_el (dv, parameter, derivative);

        if (m != 0 && dchidv [derivative]) {

          if (first)

            if (!XFPAKG_dE_dx (model, polar, parameter, N, k, dchi)) {

              error = parameter + 1;

              break;

              }

          first = 0;

          for (i = 0; i < N; i++)

            dchidv [derivative][i] += m * dchi [i];

          }
        }
      }

    if (error) {

      cwarn ("XFPAKG_calculate : cannot calculate dchi / dparameter [%1d]",
                         error > 0 ? error - 1 : error);

      N = -1;

      }

    free_check (dchi);

    }


    /* Return the number of points */

  return N;

  }


#endif /* __XFPAKG_C_C */

