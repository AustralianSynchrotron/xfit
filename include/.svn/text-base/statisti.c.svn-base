/***********************************************************************}
{                                                                       }
{  Copyright (C) Paul J. Ellis, University of Sydney, 1993.             }
{                                                                       }
{***********************************************************************/

#if !defined (__STATISTICS_C__)

#define __STATISTICS_C__

#include "statisti.h"


double Mean (int N, double x []) {

  double sum;

  int i;

  for (i = 0, sum = 0; i < N; i++)

    sum += x [i];

  return sum / N;

  }

double Variance (int N, double x []) {

  double sum, mean;

  int i;

  mean = Mean (N, x);

  for (i = 0, sum = 0; i < N; i++)

    sum += (x [i] - mean) * (x [i] - mean);

  return sum / N;

  }

double UVariance (int N, double x []) {

  return (Variance (N, x) * N) / (N - 1);

  }

double Covariance (int N, double x [], double y []) {

  double sum, meanx, meany;

  int i;

  meanx = Mean (N, x);
  meany = Mean (N, y);

  for (i = 0, sum = 0; i < N; i++)

    sum += (x [i] - meanx) * (y [i] - meany);

  return sum / N;

  }

double UCovariance (int N, double x [], double y []) {

  return (Covariance (N, x, y) * N) / (N - 1);

  }

double EsdEsd (int N, double esd) {

  return esd / sqrt (2.0 * N);

  }


#endif /* __STATISTICS_C__ */
