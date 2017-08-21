unit UVector;

interface

    { Columnvector class }

type
  Doublel = array [1..MaxInt div SizeOf (Double)] of Double;

  PDouble = ^Doublel;

  ColumnvectorC = packed record
    Rows: Integer;
    Row:  PDouble;
  end;

  Columnvector = ^ColumnvectorC;

  Columnvectorl = array [1..MaxInt div SizeOf (Columnvector)] of Columnvector;

  PColumnvector = ^Columnvectorl;

implementation

end.
