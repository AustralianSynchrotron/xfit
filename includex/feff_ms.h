/***********************************************************************}
{                                                                       }
{  This file contains the routine common to FEFF5 and FEFF6.            }
{                                                                       }
{  Copyright (C) 1994  Paul J. Ellis  University of Sydney              }
{                                                                       }
{***********************************************************************/

#if !defined (__FEFF_MS_H)

#define __FEFF_MS_H

#include "header.h"

#include "exafs.h"

#if defined MAX_PATH
#undef MAX_PATH
#endif

#define MAX_PATH 16


  /*********************************************************************}
  {                                                                     }
  {  The N (occupancy) factor used for the scattering path              }
  {  starting on atom 0 is:                                             }
  {                                                                     }
  {        max (1, |N [...]|) * min (1, |N [...]|) * sign (N [...])     }
  {                                                                     }
  {    where N [...] is the list of the N factors of the atoms.         }
  {                                                                     }
  {    All N are divided to make N [0] = 1.                             }
  {                                                                     }
  {    The maximum value returned is 1000.                              }
  {                                                                     }
  {*********************************************************************/

double N_factor (int legs, double N []);

char * file_index (char name [], int index);


  /*********************************************************************}
  {                                                                     }
  {  The following functions use angstrom**-1 and eV as input and       }
  {  output.  The internal workings are in atomic units.                }
  {                                                                     }
  {*********************************************************************/

double xk2xkp (double xk, double Vrcorr);

double xkp2xk (double xkp, double Vrcorr);


  /*********************************************************************}
  {                                                                     }
  {  sigms version 1.1 (24 March 94)                                    }
  {                                                                     }
  {    calculates the Debye-waller factor for a multiple-scattering     }
  {    path given the single-scattering factors for each atom.          }
  {                                                                     }
  {  The calculation make the following assumptions:                    }
  {                                                                     }
  {    (1) The vibration of all of the atoms are isotropic              }
  {    (2) The relative vibrations are isotropic                        }
  {               *                                                     }
  {  (1) => < u  u  > = c   I          c   scalar                       }
  {           ~i ~i      ii  3x3        ii                              }
  {                                                                     }
  {               *                                                     }
  {  (2) => < u  u  > = c   I          c   scalar                       }
  {           ~i ~j      ij  3x3        ij                              }
  {                                                                     }
  {  if i' = i + 1 the Debye-Waller factor for a scattering path L is   }
  {                                                                     }
  {     2                             ^    2                            }
  {  sig        = < [sum  (u - u  ) . R   ]  >                          }
  {     L               i  ~i  ~i'    ~ii'                              }
  {                                                                     }
  {                      ^*        *   ^       ^*        *   ^          }
  {             = sum  [ R   < u  u  > R    -  R   < u  u  > R          }
  {                  ij  ~jj'  ~j ~i   ~ii'    ~jj'  ~j'~i   ~ii'       }
  {                                                                     }
  {                      ^*        *   ^       ^*        *   ^          }
  {                    - R   < u  u  > R    +  R   < u  u  > R    ]     }
  {                      ~jj'  ~j ~i'  ~ii'    ~jj'  ~j'~i'  ~ii'       }
  {                                                                     }
  {                      ^*          *   ^         ^*        *   ^      }
  {             = sum  [ R     < u  u  > R      -  R   < u  u  > R      }
  {                  ij  ~j'j''  ~j'~i'  ~i'i''    ~jj'  ~j'~i'  ~i'i'' }
  {                                                                     }
  {                      ^*          *   ^       ^*        *   ^        }
  {                    - R     < u  u  > R    +  R   < u  u  > R    ]   }
  {                      ~j'j''  ~j'~i'  ~ii'    ~jj'  ~j'~i'  ~ii'     }
  {                                                                     }
  {                       ^*      ^*          *    ^       ^            }
  {             = sum  [ (R     - R   ) < u  u  > (R     - R   ) ]      }
  {                  ij   ~j'j''  ~jj'    ~j'~i'   ~i'i''  ~ii'         }
  {                                                                     }
  {  (1), (2)  =>                                                       }
  {                                                                     }
  {     2                 ^       ^        ^       ^                    }
  {  sig        = sum  [ (R     - R   ) . (R     - R   ) c    ]         }
  {     L            ij   ~j'j''  ~jj'     ~i'i''  ~ii'   i'j'          }
  {                                                                     }
  {                            2                                        }                                                       }
  {  If we define the shell sig  parameters to be                       }
  {                                                                     }
  {     2                                                               }
  {  sig        = 4 c    =>                                             }
  {     i            ii                                                 }
  {                                                                     }
  {                  2                                                  }
  {  c          = sig  / 4                                              }
  {   ii             i                                                  }
  {                                                                     }
  {  The single-scattering temperature factor for atom i is             }
  {                                                                     }
  {     2                                                               }
  {  sig        = 4 (c   - 2 c   + c  )                                 }
  {     0i            00      0i    ii                                  }
  {                                                                     }
  {  If two atoms i and j form part of a rigid group                    }
  {                                                                     }
  {     2                                                               }
  {  sig        = 4 (c   - 2 c   + c  ) = 0   =>                        }
  {     ij            ii      ij    jj                                  }
  {                                                                     }
  {  c          = (c   + c  ) / 2                                       }
  {   ij            ii    jj                                            }
  {                                                                     }
  {  If the two atoms are isolated from each other                      }
  {                                                                     }
  {  c          = 0                                                     }
  {   ij                                                                }
  {                                                                     }
  {  c   is modelled using the expression                               }
  {   ij                                                                }
  {                                                                     }
  {  c          = min ( (c   + c  ) / 2 * tanh (E |R  |) / (E |R  |),   }
  {   ij                  ii    jj                  ij          ij      }
  {                                                                     }
  {                      c  , c   )                                     }
  {                       ii   jj                                       }
  {                                                                     }
  {  where E is an empirical parameter                                  }
  {                                                                     }
  {*********************************************************************/

  /*********************************************************************}
  {                                                                     }
  {  legs   : number of legs in the path                                }
  {  R      : position of each atom in path                             }
  {  sigma2 : Debye-Waller factor of each atom in path                  }
  {  group  : indicates the group to which the atom belongs             }
  {                                                                     }
  {*********************************************************************/

double sigms (int legs, double E, double R [][3], double sigma2 [],

              int group []);


  /*********************************************************************}
  {                                                                     }
  {  This routine calculates the polarisation factor as in              }
  {                                                                     }
  {    Gurman, S. J.: J. Phys. C. 21, 1988, 3699-3717                   }
  {                                                                     }
  {    "The small-atom approximation in EXAFS and surface EXAFS"        }
  {                                                                     }
  {    for K and L(1) edge EXAFS:                                       }
  {                                                                     }
  {        ^   ^   ^   ^             ^   ^                              }
  {      -(R . e) (R . e) / P (acos (R . R ))                           }
  {         i       f        1        i   f                             }
  {                                                                     }
  {    where:                                                           }
  {                                                                     }
  {      e  is the polarisation vector                                  }
  {                                                                     }
  {      R  is the initial leg of the path                              }
  {       i                                                             }
  {      R  is the final leg                                            }
  {       f                                                             }
  {      P  is the Legendre polynomial.                                 }
  {       1                                                             }
  {                                                                     }
  {  This is also the factor calculated in                              }
  {                                                                     }
  {    John J. Boland, Stephen E. Crane, and John D. Baldeschwieler:    }
  {                                                                     }
  {    J. Chem. Phys. 77(1), 1 July 1982, 142-153.                      }
  {                                                                     }
  {         ^   ^   ^   ^      ^   ^                                    }
  {      <-(R . e) (R . e)> = -R . R  / 3                               }
  {          i       f          i   f                                   }
  {                                                                     }
  {   So we normalise the polarised EXAFS by dividing by this factor.   }
  {                                                                     }
  {    Note that this expression cannot match exactly the expression    }
  {    used implicitely in FEFF 5.05 as the magnitude of the EXAFS is   }
  {    not zero when the initial and final legs of the path are         }
  {    normal.  To avoid grossly overestimating the importance of       }
  {    paths approximating this case, the factor actually used is:      }
  {                                                                     }
  {              ^   ^                                                  }
  {       { x = -R . R  }                                               }
  {               i   f                                                 }
  {                                                                     }
  {                       -1    -1  -1                                  }
  {      f = (|x| + (l|x|)   - l   )   sign (x) / 3                     }
  {                                                                     }
  {                                                                     }
  {       l = 239.73 (3 f maximum = 8):                                 }
  {                                                                     }
  {       0  : |x| = 1      : 3 f = 1      :    0.00%                   }
  {       10 :       0.9848 :       1.0154 :   -0.01%                   }
  {       20 :       0.9397 :       1.0639 :   -0.03%                   }
  {       30 :       0.8660 :       1.1538 :   -0.07%                   }
  {       40 :       0.7660 :       1.3032 :   -0.17%                   }
  {       50 :       0.6428 :       1.5501 :   -0.36%                   }
  {       60 :       0.5000 :       1.9835 :   -0.83%                   }
  {       70 :       0.3420 :       2.8568 :   -2.29%                   }
  {       80 :       0.1736 :       5.1680 :  -10.26%                   }
  {       90 :       0.0000 :       0.0000 : -100.00%                   }
  {                                                                     }
  {*********************************************************************/

double polarisation_factor (int legs, double R [][3], double polar []);


#endif /* __FEFF_MS_H */
