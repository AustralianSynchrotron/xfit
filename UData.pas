unit UData;

interface

uses
  SysUtils, Classes, QGraphics, QControls, QForms,
  QDialogs, QComCtrls, QStdCtrls, QButtons, QExtCtrls,
  UDraw, QMenus, UVector, QTypes;

type
  { Doublel = array [1..MaxInt div SizeOf (Double)] of Double;

  PDouble = ^Doublel; }

  XafsData = packed record

    Gid: Pointer;

    No, N, Nc, NR: Integer;

    k, R,

    ko, chio, sigmao, smootho, chioi, chiof,
    kc, chic,                  chici, chicf,

    k_window, R_window,

    FTo, FTc: PDouble;

    kmax, Rmax: Double;
  end;

  PXafsData = ^XafsData;


  TData = class (TForm)
    Menu:          TPopupMenu;
    XafsCurves:    TMenuItem;
    Observed:      TMenuItem;
    Calculated:    TMenuItem;
    XafsResidual:  TMenuItem;
    RawO:          TMenuItem;
    InterpolatedO: TMenuItem;
    WindowedO:     TMenuItem;
    FilteredO:     TMenuItem;
    RawC:          TMenuItem;
    WindowedC:     TMenuItem;
    FilteredC:     TMenuItem;
    XafsWindow:    TMenuItem;
    FTCurves:      TMenuItem;
    FTObserved:    TMenuItem;
    FTCalculated:  TMenuItem;
    FTResidual:    TMenuItem;
    FTWindow:      TMenuItem;
    FTO:           TMenuItem;
    WindowedFTO:   TMenuItem;
    RealFTO:       TMenuItem;
    ImaginaryFTO:  TMenuItem;
    FTC:           TMenuItem;
    WindowedFTC:   TMenuItem;
    RealFTC:       TMenuItem;
    ImaginaryFTC:  TMenuItem;
    PageControl:   TPageControl;
    XafsPage:      TTabSheet;
    DataPanel:     TPanel;
    DataDraw:      TDrawWindow;
    FTPage:        TTabSheet;
    FTPanel:       TPanel;
    FTDraw:        TDrawWindow;
    BothPages:     TTabSheet;
    BothPanel:     TPanel;
    BothDraw:      TDrawWindow;
    N1:            TMenuItem;
    WidePen:       TMenuItem;

    procedure DataDrawPaint (Sender: TObject);
    procedure FormCreate    (Sender: TObject);
    procedure FormDestroy   (Sender: TObject);
    procedure FTDrawPaint   (Sender: TObject);
    procedure DataClick     (Sender: TObject);
    procedure FTCurveClick  (Sender: TObject);
    procedure PageControlChanging (Sender: TObject;
                                   var AllowChange: Boolean);
    procedure BothDrawPaint (Sender: TObject);
    procedure WidePenClick  (Sender: TObject);

  private

    lData: XafsData;

    lD, lDa, lDb, lDc: PDouble;
    lN: Integer;

    lCPage: TTabSheet;


    procedure Modulus (D, Da: PDouble; N: Integer);
    function  RealPart (D: PDouble; N: Integer): PDouble;
    function  ImagPart (D: PDouble; N: Integer): PDouble;
    procedure Curve (Draw: TDrawWindow; x, y: PDouble; N: Integer; Color: TColor);
    function  Product (Da, Db: PDouble; N: Integer): PDouble;
    function  Difference (Da, Db: PDouble; N: Integer): PDouble;
    procedure MaxPoint (D: PDouble; N: Integer; var Omax: Double);
    procedure MinPoint (D: PDouble; N: Integer; var Omin: Double);
    function  Points (D: PDouble; N: Integer; Dmax: Double): Integer;
    procedure DrawFT (Draw: TDrawWindow; Ymin, Ymax: Double);
    procedure DrawData (Draw: TDrawWindow; Ymin, Ymax: Double);
    procedure SetBounds (Draw: TDrawWindow; Ymin, Ymax: Double);
    function  SetHeight: Boolean;

    procedure GetD  (Data: PXafsData);
    procedure CopyD (var D: PDouble; ND: Integer;
                         S: PDouble; NS: Integer);
    procedure CopyData  (Data: PXafsData);

  public

    procedure ChangeData (Data: PXafsData);

    procedure PageRefresh;

  end;

var
  Data: TData;


implementation

uses UConsole;

{$R *.xfm}



function TData.Points (D: PDouble; N: Integer; Dmax: Double): Integer;
var
  C, Cmax: Integer;
begin
  Cmax := 0;

  if D <> nil then

    for C := 1 to N do

      if D [C] <= Dmax then

        Cmax := C

      else

        Break;

  Points := Cmax;
end;


function TData.Product (Da, Db: PDouble; N: Integer): PDouble;
var
  C: Integer;
begin
  if (Da = nil) or (Db = nil) then

    Product := nil

  else
  begin
    for C := 1 to N do

      lD [C] := Da [C] * Db [C];

    Product := lD;
  end;
end;


procedure TData.Modulus (D, Da: PDouble; N: Integer);
var
  C: Integer;
begin
  if Da <> nil then

    for C := 1 to N do

      D [C] := Sqrt (Da [C + C - 1] * Da [C + C - 1] +
                     Da [C + C] * Da [C + C])

  else

    for C := 1 to N do

      D [C] := 0;
end;


function TData.Difference (Da, Db: PDouble; N: Integer): PDouble;
var
  C: Integer;
begin
  if (Da = nil) or (Db = nil) then

    Difference := nil

  else
  begin
    for C := 1 to N do

      lD [C] := Da [C] - Db [C];

    Difference := lD;
  end;
end;


function TData.RealPart (D: PDouble; N: Integer): PDouble;
var
  C: Integer;
begin
  if D = nil then

    RealPart := nil

  else
  begin
    for C := 1 to N do

      lD [C] := D [2 * C - 1];

    RealPart := lD;
  end;
end;


function TData.ImagPart (D: PDouble; N: Integer): PDouble;
var
  C: Integer;
begin
  if D = nil then

    ImagPart := nil

  else
  begin
    for C := 1 to N do

      lD [C] := D [2 * C];

    ImagPart := lD;
  end;
end;


procedure TData.MaxPoint (D: PDouble; N: Integer; var Omax: Double);
var
  C: Integer;
begin
  if D <> nil then

    for C := 1 to N do

      if Abs (D [C]) > Omax then

        Omax := Abs (D [C]);
end;


procedure TData.MinPoint (D: PDouble; N: Integer; var Omin: Double);
var
  C: Integer;
begin
  if D <> nil then

    for C := 1 to N do

      if D [C] < Omin then

        Omin := D [C];
end;


procedure TData.Curve (Draw: TDrawWindow;
                       x, y: PDouble; N: Integer; Color: TColor);
var
  Cx, Cy: ColumnvectorC;
begin
  if (x <> nil) and (y <> nil) then
  begin
    Cx.Rows := N;
    Cy.Rows := N;

    Cx.Row := x;
    Cy.Row := y;

    Draw.Color := Color;

    Draw.DrawCurve (@Cx, @Cy, nil);
  end;
end;


{ 0..1    ymin                 ymax
  0..0.5  ymin                 ymax + ymax - ymin
  0.5..1  ymin + ymin - ymax   ymax }

procedure TData.SetBounds (Draw: TDrawWindow; Ymin, Ymax: Double);
var
  D: Double;
begin
  D := Draw.Ymax - Draw.Ymin;

  Draw.Ymin := Draw.Ymin - 2 * Ymin * D;
  Draw.Ymax := Draw.Ymax + 2 * (1 - Ymax) * D;
end;


function TData.SetHeight: Boolean;
var
  Change: Boolean;
begin
    { }

  Change := False;

  if lCPage <> nil then
  begin
    if lCPage <> PageControl.ActivePage then
    begin
      if lCPage = BothPages then
      begin
        Change := True;

        if PageControl.ActivePage = XafsPage then

          Height := Height - DataDraw.Height div 2

        else

          Height := Height - FTDraw.Height div 2;
      end;

      if PageControl.ActivePage = BothPages then
      begin
        Change := True;

        Height := Height + BothDraw.Height;
      end;
    end;

    lCPage := nil;
  end;

  if Change then

    Refresh;

  SetHeight := Change;
end;


procedure TData.DrawFT (Draw: TDrawWindow; Ymin, Ymax: Double);
var
  C, N_l: Integer;
  Omax, Omin: Double;
begin
  Critical.Enter;

  with lData do
  begin

      { Get the number of points to display }

    N_l := Points (R, NR, Rmax);


      { Calculate the observed modulus }

    if Self.FTO.Checked or WindowedFTO.Checked then

      Modulus (lDa, FTO, N_l);


      { Calculate the calculated modulus }

    if Self.FTC.Checked or WindowedFTC.Checked then

      Modulus (lDb, FTC, N_l);


      { Calculate the difference modulus }

    if FTResidual.Checked then

      Modulus (lDc, Difference (FTO, FTC, 2 * N_l), N_l);


      { Get the maximum displayed magnitude }

    Omin := 0;
    Omax := 0;


      { Observed }

    if Self.FTO.Checked then

      MaxPoint (lDa, N_l, Omax);


    if Omax = 0 then

      if WindowedFTO.Checked then

        MaxPoint (Product (lDa, R_window, N_l), N_l, Omax);


      { Observed }

    if Self.FTO.Checked then

      MaxPoint (lDa, N_l, Omax);


    if Omax = 0 then

      if WindowedFTO.Checked then

        MaxPoint (Product (lDa, R_window, N_l), N_l, Omax);


    if Self.FTC.Checked then

      MaxPoint (lDb, N_l, Omax);


    if RealFTO.Checked or ImaginaryFTO.Checked or
       RealFTC.Checked or ImaginaryFTC.Checked then

      Omin := -Omax;


      { Draw the scale }

    if Omax = 0 then

      Omax := 1;

    if lData.Rmax > 0 then
    begin
      Draw.Xmin := Rmax * -0.1;
      Draw.Xmax := Rmax * 1.04;

      if Omin = 0 then
      begin
        Draw.Ymin := -Omax * 0.15;
        Draw.Ymax := Omax * 1.08;
      end
      else
      begin
        Omin := -Omax;

        Draw.Ymin := Omin * 1.15;
        Draw.Ymax := Omax * 1.15;
      end;

      SetBounds (Draw, Ymin, Ymax);

      Draw.Color := $0FF0000;

      Draw.DrawAxes (0, Rmax, Omin, Omax,
                                   0.02 * Rmax, 0.05 * (Omax - Omin), 10, 7);


        { Draw the real part of the experimental FT }

      if Self.RealFTO.Checked then

        Curve (Draw, R, RealPart (FTO, N_l), N_l, $0C00000);


        { Draw the imaginary part of the experimental FT }

      if Self.ImaginaryFTO.Checked then

        Curve (Draw, R, ImagPart (FTO, N_l), N_l, $000C000);


        { Draw the real part of the calculated FT }

      if Self.RealFTC.Checked then

        Curve (Draw, R, RealPart (FTC, N_l), N_l, $0FF0000);


        { Draw the imaginary part of the calculated FT }

      if Self.ImaginaryFTC.Checked then

        Curve (Draw, R, ImagPart (FTC, N_l), N_l, $000FF00);


        { Draw the modulus of the experimental FT }

      if Self.FTO.Checked then

        Curve (Draw, R, lDa, N_l, $000C0C0);


        { Draw the modulus of the windowed experimental FT }

      if Self.WindowedFTO.Checked then

        Curve (Draw, R, Product (lDa, R_window, N_l), N_l, $000FFFF);


        { Draw the modulus of the calculated FT }

      if Self.FTC.Checked and (FTC <> nil) then

        Curve (Draw, R, lDb, N_l, $0C0C000);


        { Draw the modulus of the windowed calculated FT }

      if Self.WindowedFTC.Checked then

        Curve (Draw, R, Product (lDb, R_window, N_l), N_l, $0FFFF00);


        { Draw the modulus of the difference FT }

      if FTResidual.Checked and (FTC <> nil) then

        Curve (Draw, R, lDc, N_l, $00000FF);


        { Window }

      Draw.Ymin := Draw.Ymin / Omax;
      Draw.Ymax := Draw.Ymax / Omax;

      if FTWindow.Checked then

        Curve (Draw, R, R_window, N_l, $00000FF);
    end;
  end;

  Critical.Leave;
end;


procedure TData.DrawData (Draw: TDrawWindow; Ymin, Ymax: Double);
var
  C, N_l, No_l, Nc_l: Integer;
  Omax: Double;
begin
  Critical.Enter;

  with lData do
  begin

      { Get the number of points to display }

    N_l := Points (k, N, kmax);

    No_l := Points (ko, No, kmax);

    Nc_l := Points (kc, Nc, kmax);


      { Get the maximum displayed magnitude }

    Omax := 0;


      { Observed }

    if RawO.Checked or InterpolatedO.Checked then

      MaxPoint (chio, No_l, Omax);


    if Omax = 0 then

      if WindowedO.Checked then

        MaxPoint (Product (chioi, k_window, N_l), N_l, Omax);

    if FilteredO.Checked then

      MaxPoint (chiof, N_l, Omax);


      { Calculated }

    if RawC.Checked then

      MaxPoint (chic, Nc_l, Omax)

    else

      if WindowedC.Checked then

        MaxPoint (Product (chici, k_window, N_l), N_l, Omax);

    if FilteredC.Checked then

      MaxPoint (chicf, N_l, Omax);


      { Difference }

    if XafsResidual.Checked and (chici <> nil) then

      MaxPoint (Difference (chioi, chici, N_l), N_l, Omax);


    if Omax = 0 then

      Omax := 1.001;


      { Draw the scale }

    if lData.kmax > 0 then
    begin
      Draw.Xmin := kmax * -0.1;
      Draw.Xmax := kmax * 1.04;

      Draw.Ymin := Omax * -1.15;
      Draw.Ymax := Omax *  1.15;

      SetBounds (Draw, Ymin, Ymax);

      Draw.Color := $0FF0000;

      Draw.DrawAxes (0, kmax, -Omax, Omax,
                                   0.02 * kmax, 0.1 * Omax, 10, 7);
    end;


      { Draw the interpolated experimental xafs }

    if InterpolatedO.Checked then

      Curve (Draw, k, chioi, N_l, $00C000);


      { Draw the windowed interpolated experimental xafs }

    if WindowedO.Checked then

      Curve (Draw, k, Product (chioi, k_window, N_l), N_l, $00C0C0);


      { Raw experimental xafs }

    if RawO.Checked then

      Curve (Draw, ko, chio, No_l, $00FF00);


      { Draw the filtered experimental xafs }

    if FilteredO.Checked then

      Curve (Draw, k, chiof, N_l, $00FFFF);


      { Windowed calculated xafs }

    if WindowedC.Checked then

      Curve (Draw, k, Product (chici, k_window, N_l), N_l, $0C0C000);


      { Raw calculated xafs }

    if RawC.Checked then

      Curve (Draw, kc, chic, Nc_l, $0FF0000);


      { Filtered calculated xafs }

    if FilteredC.Checked then

      Curve (Draw, k, chicf, N_l, $0FFFF00);


      { Difference xafs }

    if XafsResidual.Checked then

      Curve (Draw, k, Difference (chioi, chici, N_l), N_l, $00000FF);


      { Window }

    Draw.Ymin := Draw.Ymin / Omax;
    Draw.Ymax := Draw.Ymax / Omax;

    if XafsWindow.Checked then

      Curve (Draw, k, k_window, N_l, $00000FF);
  end;

  Critical.Leave;
end;


procedure TData.FTDrawPaint (Sender: TObject);
begin
  if not SetHeight then

    DrawFT (FTDraw, 0, 1);
end;


procedure TData.BothDrawPaint (Sender: TObject);
begin
  if not SetHeight then
  begin
    DrawFT (BothDraw, 0, 0.5);

    DrawData (BothDraw, 0.5, 1);
  end;
end;


procedure TData.DataDrawPaint (Sender: TObject);
begin
  if not SetHeight then

    DrawData (DataDraw, 0, 1);
end;


procedure TData.CopyD (var D: PDouble; ND: Integer;
                           S: PDouble; NS: Integer);
var
  C: Integer;
begin
  if S = nil then

    NS := 0;

  if (ND <> NS) or (D = nil) then
  begin
    if D <> nil then

      FreeMem (D);

    if NS > 0 then

      GetMem (D, NS * SizeOf (Double))

    else

      D := nil;
  end;

  for C := 1 to NS do

    D [C] := S [C];
end;


procedure TData.GetD (Data: PXafsData);
var
  Cmax: Integer;
begin
  Cmax := 0;

  if Data.No > Cmax then

    Cmax := Data.No;

  if Data.N > Cmax then

    Cmax := Data.N;

  if Data.Nc > Cmax then

    Cmax := Data.Nc;

  if 2 * Data.NR > Cmax then

    Cmax := 2 * Data.NR;

  if Cmax > lN then
  begin
    if lD <> nil then

      FreeMem (lD);

    if lDa <> nil then

      FreeMem (lDa);

    if lDb <> nil then

      FreeMem (lDb);

    if lDc <> nil then

      FreeMem (lDc);

    GetMem (lD, Cmax * SizeOf (Double));
    GetMem (lDa, Cmax * SizeOf (Double));
    GetMem (lDb, Cmax * SizeOf (Double));
    GetMem (lDc, Cmax * SizeOf (Double));

    lN := Cmax;
  end;
end;


procedure TData.ChangeData (Data: PXafsData);
begin

  CopyData (Data);

end;


procedure TData.CopyData (Data: PXafsData);
begin

    { }

  GetD (Data);

  CopyD (lData.ko, lData.No, Data.ko, Data.No);
  CopyD (lData.chio, lData.No, Data.chio, Data.No);
  CopyD (lData.sigmao, lData.No, Data.sigmao, Data.No);
  CopyD (lData.smootho, lData.No, Data.smootho, Data.No);

  lData.No := Data.No;

  CopyD (lData.k, lData.N, Data.k, Data.N);
  CopyD (lData.chioi, lData.N, Data.chioi, Data.N);
  CopyD (lData.chiof, lData.N, Data.chiof, Data.N);
  CopyD (lData.chici, lData.N, Data.chici, Data.N);
  CopyD (lData.chicf, lData.N, Data.chicf, Data.N);
  CopyD (lData.k_window, lData.N, Data.k_window, Data.N);

  lData.N := Data.N;

  CopyD (lData.kc, lData.Nc, Data.kc, Data.Nc);
  CopyD (lData.chic, lData.Nc, Data.chic, Data.Nc);

  lData.Nc := Data.Nc;

  CopyD (lData.R, lData.NR, Data.R, Data.NR);
  CopyD (lData.R_window, lData.NR, Data.R_window, Data.NR);

  CopyD (lData.FTo, lData.NR * 2, Data.FTo, Data.NR * 2);
  CopyD (lData.FTc, lData.NR * 2, Data.FTc, Data.NR * 2);

  lData.NR := Data.NR;

  lData.kmax := Data.kmax;
  lData.Rmax := Data.Rmax;
end;


procedure TData.FormCreate (Sender: TObject);
begin
  lD := nil;
  lDa := nil;
  lDb := nil;
  lDc := nil;
  lN := 0;

  with lData do
  begin
    Gid := nil;

    No := 0;
    N  := 0;
    Nc := 0;
    NR := 0;

    k := nil;
    R := nil;

    ko      := nil;
    chio    := nil;
    sigmao  := nil;
    smootho := nil;
    chioi   := nil;
    chiof   := nil;

    kc    := nil;
    chic  := nil;
    chici := nil;
    chicf := nil;

    k_window := nil;
    R_window := nil;

    FTo := nil;
    FTc := nil;
  end;
end;


procedure TData.FormDestroy (Sender: TObject);
begin
  CopyD (lD, 1, nil, 0);

  CopyD (lData.ko, 1, nil, 0);
  CopyD (lData.chio, 1, nil, 0);
  CopyD (lData.sigmao, 1, nil, 0);
  CopyD (lData.smootho, 1, nil, 0);
  CopyD (lData.k, 1, nil, 0);
  CopyD (lData.chioi, 1, nil, 0);
  CopyD (lData.chiof, 1, nil, 0);
  CopyD (lData.chici, 1, nil, 0);
  CopyD (lData.chicf, 1, nil, 0);
  CopyD (lData.k_window, 1, nil, 0);
  CopyD (lData.kc, 1, nil, 0);
  CopyD (lData.chic, 1, nil, 0);
  CopyD (lData.R, 1, nil, 0);
  CopyD (lData.R_window, 1, nil, 0);
  CopyD (lData.FTo, 1, nil, 0);
  CopyD (lData.FTc, 1, nil, 0);
end;


procedure TData.PageRefresh;
begin
  if PageControl.ActivePage = XafsPage then

    DataDraw.Refresh

  else if PageControl.ActivePage = FTPage then

    FTDraw.Refresh

  else if PageControl.ActivePage = BothPages then

    BothDraw.Refresh;
end;


procedure TData.DataClick (Sender: TObject);
var
  O: TMenuItem;
begin
  O := nil;

  if Sender = XafsResidual then

    O := XafsResidual

  else if Sender = XafsWindow then

    O := XafsWindow

  else if Sender = RawO then

    O := RawO

  else if Sender = InterpolatedO then

    O := InterpolatedO

  else if Sender = WindowedO then

    O := WindowedO

  else if Sender = FilteredO then

    O := FilteredO

  else if Sender = RawC then

    O := RawC

  else if Sender = WindowedC then

    O := WindowedC

  else if Sender = FilteredC then

    O := FilteredC;

  if O <> nil then
  begin
    O.Checked := not O.Checked;

    PageRefresh;
  end;
end;


procedure TData.FTCurveClick (Sender: TObject);
var
  O: TMenuItem;
begin
  O := nil;

  if Sender = FTResidual then

    O := FTResidual

  else if Sender = FTWindow then

    O := FTWindow

  else if Sender = FTO then

    O := FTO

  else if Sender = WindowedFTO then

    O := WindowedFTO

  else if Sender = RealFTO then

    O := RealFTO

  else if Sender = ImaginaryFTO then

    O := ImaginaryFTO

  else if Sender = FTC then

    O := FTC

  else if Sender = WindowedFTC then

    O := WindowedFTC

  else if Sender = RealFTC then

    O := RealFTC

  else if Sender = ImaginaryFTC then

    O := ImaginaryFTC;

  if O <> nil then
  begin
    O.Checked := not O.Checked;

    PageRefresh;
  end;
end;


procedure TData.PageControlChanging (Sender: TObject;
                                     var AllowChange: Boolean);
begin
  lCPage := PageControl.ActivePage;
end;


procedure TData.WidePenClick (Sender: TObject);
begin
  WidePen.Checked := not WidePen.Checked;

  if WidePen.Checked then
  begin
    DataDraw.Canvas.Pen.Width := 2;
    FTDraw.Canvas.Pen.Width   := 2;
    BothDraw.Canvas.Pen.Width := 2;
  end
  else
  begin
    DataDraw.Canvas.Pen.Width := 1;
    FTDraw.Canvas.Pen.Width   := 1;
    BothDraw.Canvas.Pen.Width := 1;
  end;

  PageRefresh;
end;

end.
