/***********************************************************************}
{                                                                       }
{  Routine used to convert the files.dat and feffxxxx.dat files         }
{  generated by FEFF 8.20 into total EXAFS.                             }
{                                                                       }
{  It is a conversion of the subroutine ff2chi from FEFF 6.01           }
{                                                                       }
{  input: files.dat + files as listed therein                           }
{                                                                       }
{***********************************************************************/

#if !defined (__FF8_CHI_H)

#define __FF8_CHI_H

#if defined (__TURBOC__)

#include "complex.h"

#else

#include "complex.h"

#endif

#include "interpol.h"

#include "feff_.h"
#include "feff8.h"

  /*********************************************************************}
  {                                                                     }
  {  ff8_chi version 1.0
  {                                                                     }
  {    ff8_chi calculates the total multiple-scattering EXAFS per       }
  {    absorbing atom from the files "files.dat" and "feff????.dat"     }
  {    generated by FEFF 6 and the following parameters:                }
  {                                                                     }
  {       Vrcorr  : real part of the E0 correcttion                     }
  {       Vicorr  : imaginary part of the E0 correcttion                }
  {                                                                     }
  {       S02     : overall scale factor                                }
  {       E       : an empirical parameter used in sigms                }
  {                                                                     }
  {       Nam     : number of atoms in the model                        }
  {       pot...m : index of the atomic potential used                  }
  {       Zm      : atomic numbers of the atoms                         }
  {       Nm      : scaling factor for each atom                        }
  {       Rm      : atomic coordinates                                  }
  {       sigma2m : single-scattering temperature factors               }
  {                                                                     }
  {    All of the parameters except Nam and Rm may be varied without    }
  {    rerunning FEFF 8.                                                }
  {                                                                     }
  {*********************************************************************/

int ff8_chi (double print0,

             path_l * path,

             double polar [],

             double Vrcorr, double Vicorr,

             double S02, double E,

             int Nam, double potentialm [],
             double Zm [], double Nm [], double Rm [][3],
             double sigma2m [],

             int Nx, double k [], double chi []);

    /* Code units for this module are:

                           -1          2
         angstrom, angstrom  , angstrom , eV etc.

       Use complex momentum for Debye-Waller factors.

       */



#endif /* __FF8_CHI_H */
