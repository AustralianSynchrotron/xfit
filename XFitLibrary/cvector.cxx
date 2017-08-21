
#if !defined CVECTOR_CPP

#define CVECTOR_CPP

#include "cvector.h"

//#include "common.h"

#define min(A,B) ((A) <= (B) ? (A) : (B))


double Columnvector::dummy = 0;


Columnvector::Columnvector () {
    
  Rows = TotalRows = 0;

  Row = NULL;

  }


Columnvector::Columnvector (const Columnvector& O) {
    
  Rows = TotalRows = 0;

  Row = NULL;

  *this = O;

  }


Columnvector::Columnvector (int size) {
    
  Rows = TotalRows = 0;

  Row = NULL;

  ReDimension (size);

  }


Columnvector::~Columnvector () {
    
  Clear ();

  }


void Columnvector::Clear () {
    
  if (Row)

    delete[] Row;

  Row = NULL;

  Rows = TotalRows = 0;

  }


Columnvector& Columnvector::operator = (const Columnvector& O) {

  ReDimension (O.Nrows ());

  for (int c = 1; c <= Rows; c++)

    Row [c - 1] = O (c);

  return *this;

  }
  

Columnvector& Columnvector::operator = (double O) {

  for (int c = 0; c < Rows; c++)

    Row [c] = O;

  return *this;

  }


Columnvector Columnvector::operator + (double O) const {

  Columnvector R;

  R = *this;

  for (int c = 0; c < Rows; c++)

    R.Row [c] += O;

  return R;
  
  }

  
Columnvector Columnvector::operator - (double O) const {

  Columnvector R;

  R = *this;

  for (int c = 0; c < Rows; c++)

    R.Row [c] -= O;

  return R;
  
  }

  
Columnvector Columnvector::operator + (const Columnvector& O) const {

  Columnvector R;

  int Rows = min (this->Rows, O.Rows);

  R = *this;

  for (int c = 0; c < Rows; c++)

    R.Row [c] += O.Row [c];

  return R;
  
  }

  
Columnvector Columnvector::operator - (const Columnvector& O) const {

  Columnvector R;

  int Rows = min (this->Rows, O.Rows);

  R = *this;

  for (int c = 0; c < Rows; c++)

    R.Row [c] -= O.Row [c];

  return R;

  }


Columnvector Columnvector::operator - () const {

  Columnvector R;

  R = *this;

  for (int c = 0; c < R.Rows; c++)

    R.Row [c] = -R.Row [c];

  return R;

  }


Columnvector Columnvector::operator / (double O) const {

  Columnvector R;

  R = *this;

  for (int c = 0; c < R.Rows; c++)

    R.Row [c] /= O;

  return R;

  }


Columnvector Columnvector::operator * (double O) const {

  Columnvector R;

  R = *this;

  for (int c = 0; c < R.Rows; c++)

    R.Row [c] *= O;

  return R;

  }


Columnvector& Columnvector::operator << (double * O) {

  for (int c = 0; c < Rows; c++)

    Row [c] = O [c];

  return *this;

  }


int Columnvector::operator == (const Columnvector& O) const {

  if (Rows != O.Rows)

    return 0;

  for (int c = 0; c < Rows; c++)

    if (Row [c] != O.Row [c])

      return 0;

  return 1;
  
  }


double * Columnvector::Store () const {

  return Row;
  
  }


int Columnvector::ReDimension (int size) {

  if (TotalRows < 0 || Rows < 0 || Rows > TotalRows)

    return 0;

  if (size <= TotalRows)

    return ReFrame (size);

  Clear ();

  Row = new double [size];

  for (int c = 0; c < size; c++)

    Row [c] = 0;

  Rows = TotalRows = size;

  return 1;
      
  }               


int Columnvector::ReFrame (int size) {

  if (size < 0)

    return 0;

  if (size == 0) {

    Clear ();

    return 1;

    }

  if (size <= TotalRows) {

    for (; Rows < size; Rows++)

      Row [Rows] = 0;

    Rows = size;
    
    return 1;
    
    }


  double * oRow = Row;

  int oRows = Rows;
  
  Row = NULL;

  Rows = TotalRows = 0;

  ReDimension (size);  

  for (int c = 0; c < oRows; c++)

    Row [c] = oRow [c];

  delete[] oRow;

  return 1;
  
  }
  

#endif // CVECTOR_CPP

