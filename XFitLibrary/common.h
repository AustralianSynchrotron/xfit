/***********************************************************************}
{                                                                       }
{  Common definitions used in the exafs refinement package              }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__COMMON_H__)

#define __COMMON_H__

#include "console.h"

#define TERM_WIDTH 80

#define delta_r(r_max,N) (2 * (r_max) / (N))
#define delta_k(r_max)   (M_PI / (2 * (r_max)))

//#define YYLMAX 200
#define YYLMAX 8192

typedef double _v3_ [3];


  /***************}
  {               }
  {  Atomic data  }
  {               }
  {***************/

#include "constant.h"


  /*********************************************************************}
  {                                                                     }
  {  Parser token values : (declared in xfit.y) :                       }
  {                                                                     }
  {  INCLUDE EDGE EXAFSW FTW DATA KWEIGHT RESTORE REFINE LIST           }
  {  DELETE EXIT MONTECARLO INVALID                                     }
  {                                                                     }
  {*********************************************************************/

#include "matrix.h"

#include "expressi.h"


#if !defined (__Y_TAB_C__)

#include "xfity_tab.h"

#endif


  /*****************************}
  {                             }
  {  Sets of parameter vectors  }
  {                             }
  {*****************************/

typedef enum {

  MODEL    = 0,         /* Model set */

  DATA_SET = 2,         /* Exafs data set */

  COPY     = 1          /* Offset to copy (saved) set */

  } vector_set;


#endif /* __COMMON_H__ */





