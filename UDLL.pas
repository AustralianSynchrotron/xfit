unit UDLL;

interface

{ uses
  UVector; }

type

    {  }

  EEDataC = packed record

    E,
    Data,
    DF: Pointer;

  end;

  EEData = ^EEDataC;

    { Functions and procedures imported from spline.dll }

    {$IFDEF LINUX}
function Xfit (Cal, Poll, Cout, DataW, Sym: Pointer): Integer; cdecl;
  external 'libxfit.so.0' name 'Xfit';
    {$ENDIF}
    {$IFDEF MSWINDOWS}
function Xfit (Cal, Poll, Cout, DataW, Sym: Pointer): Integer; cdecl;
  external 'XFIT_WIN32.DLL' name '_Xfit@20';
  {$ENDIF}
// function Success: Integer; stdcall;
//  external 'libxfit.so' name 'Success';

implementation

end.
