
#if !defined CVECTOR_H

#define CVECTOR_H

   // Columnvector class replacing newmat ColumnVectors


#include <stdlib.h>  
#include <math.h>

#if !defined MATRIX_LIB

typedef double Real;
typedef int Boolean;

#endif

#define TRUE  1
#define FALSE 0

class Columnvector {

  private:

    int                 Rows;

    double *            Row;

    int                 TotalRows;


      // Dummy entry

    static double       dummy;


      // Private routines

    void                Clear ();


  public:

      // Routines

                        Columnvector ();
                        Columnvector (const Columnvector&);
                        Columnvector (int);
                       ~Columnvector ();

    int                 ReDimension (int);               

    int                 ReFrame (int);               

    double&             operator () (int);
    double              operator () (int) const;

    Columnvector&       operator = (const Columnvector&);
    Columnvector&       operator = (double);

    int                 operator == (const Columnvector&) const;

    int                 Nrows () const;

    double *            Store () const;

    Columnvector        operator + (const Columnvector&) const;
    Columnvector        operator + (double) const;
    Columnvector        operator - (const Columnvector&) const;
    Columnvector        operator - (double) const;
    Columnvector        operator - () const;
    Columnvector        operator / (double) const;
    Columnvector        operator * (double) const;

    Columnvector&       operator << (double *);

  };


inline double& Columnvector::operator () (int entry) {

  return (entry >= 1 && entry <= Rows) ? Row [entry - 1] : dummy;

  }
  

inline double Columnvector::operator () (int entry) const {

  return (entry >= 1 && entry <= Rows) ? Row [entry - 1] : dummy;

  }
  

inline int Columnvector::Nrows () const {

  return Rows;

  }


  // FFTs

void FFT (const Columnvector& U, const Columnvector& V,
                Columnvector& X, Columnvector& Y);

void FFTI (const Columnvector& U, const Columnvector& V,
                 Columnvector& X, Columnvector& Y);

void RealFFT (const Columnvector& U, Columnvector& X, Columnvector& Y);

void RealFFTI (const Columnvector& A, const Columnvector& B, Columnvector& U);


#endif // CVECTOR_H

