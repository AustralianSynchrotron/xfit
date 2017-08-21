/***********************************************************************}
{                                                                       }
{  This file contains the routine used to read in experimental exafs.   }
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1994.             }
{                                                                       }
{***********************************************************************/

#if !defined (__EXPERIMENT_H__)

#define __EXPERIMENT_H__

#include "ascii_da.h"
#include "exafs_ty.h"

#include "common.h"

int read_exafs_file (char filename [], exafs_data_type * E);

#endif /* __EXPERIMENT_H__ */
