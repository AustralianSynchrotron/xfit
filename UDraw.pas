unit UDraw;

interface

uses
  Classes, QGraphics, QExtCtrls, QControls, Types,
  UVector;

type
  TDMouseDown = procedure (Button: TMouseButton; Shift: TShiftState;
                           X, Y: Double) of object;
  TDMouseMove = procedure (Shift: TShiftState;
                           X, Y: Double) of object;
  TDMouseUp   = procedure (Button: TMouseButton; Shift: TShiftState;
                           X, Y: Double) of object;

  TDrawWindow = class (TPaintBox)

  public
    procedure   DrawAxes (xmina, xmaxa, ymina, ymaxa, dx, dy: Double;
                          xsteps, ysteps: Integer);
    procedure   DrawXScale (Xstart, Xend, Xstep, Y: Double);
    function    TextWidth (const Text: string): Double;
    procedure   TextOut (X, Y: Double; const Text: string);
    procedure   DrawCharacter (X, Y, Z, dX, dY: Double; C: Char);
    procedure   DrawString (X, Y, Z, dX, dY: Double;
                               const Text: String; Justify: String; Rotate: Boolean);
    procedure   DrawStringx (Xa, Ya, Za, Xb, Yb, Zb, dX, dY: Double;
                               const Text, Justify: String);

    constructor Create (Owner: TComponent); override;

    procedure   GetCurveLimits (const X, Y, Z: Columnvector);
    procedure   DrawCurve (const X, Y, Z: Columnvector);
    procedure   Border (X, Y, Z: Double);
    procedure   Circle (X, Y, Z: Double; Radius: Integer);
    procedure   MoveTo (X, Y, Z: Double);
    procedure   LineTo (X, Y, Z: Double);
    procedure   Rotate (X, Y: Double);

  private
    lColor: TColor;

    lUseMatrix: Boolean;

    lX, lY, lZ, lOZ: Double;

    lMin, lMax, lCentre: array [0..2] of Double;
    lView: array [0..2, 0..2] of Double;

    lOnMouseDown: TDMouseDown;
    lOnMouseMove: TDMouseMove;
    lOnMouseUp:   TDMouseUp;

    function    Division (xmin, xmax: Double; steps: Integer): Double;
    function    Normalize (var X, Y, Z: Double): Double;
    procedure   ApplyView (X, Y, Z: Double);
    procedure   GetLimits (const D: Columnvector; var Dmin, Dmax: Double);
    function    GetPixelCoordinates (X, Y, Z: Double): TPoint;
    procedure   GetCoordinates (X, Y: Integer);
    procedure   SetColor (C: Double);

    procedure   MouseDown (Button: TMouseButton; Shift: TShiftState;
                           X, Y: Integer); override;
    procedure   MouseMove (Shift: TShiftState;
                           X, Y: Integer); override;
    procedure   MouseUp   (Button: TMouseButton; Shift: TShiftState;
                           X, Y: Integer); override;

  published
    property Color:  TColor  read lColor write lColor default $0FFFFFF;

    property Xmin:   Double  read lMin [0]  write lMin [0];
    property Xmax:   Double  read lMax [0]  write lMax [0];
    property Ymin:   Double  read lMin [1]  write lMin [1];
    property Ymax:   Double  read lMax [1]  write lMax [1];
    property Zmin:   Double  read lMin [2]  write lMin [2];
    property Zmax:   Double  read lMax [2]  write lMax [2];

    property UseMatrix: Boolean read lUseMatrix  write lUseMatrix;

    property OnMouseDown: TDMouseDown read lOnMouseDown write lOnMouseDown;
    property OnMouseMove: TDMouseMove read lOnMouseMove write lOnMouseMove;
    property OnMouseUp:   TDMouseUp   read lOnMouseUp   write lOnMouseUp;
    // and properties from the TPaintBox component
    property Canvas;
    end;

procedure Register;


implementation

uses
  SysUtils, UChar;


procedure Register;
begin
  RegisterComponents ('Samples', [TDrawWindow]);
end;


constructor TDrawWindow.Create (Owner: TComponent);
begin
  inherited Create (Owner);

  lMin [0] := -1;
  lMin [1] := -1;
  lMin [2] := -1;

  lMax [0] := 1;
  lMax [1] := 1;
  lMax [2] := 1;

  lCentre [0] := 0;
  lCentre [1] := 0;
  lCentre [2] := 0;

  lView [0][0] := 1;
  lView [1][0] := 0;
  lView [2][0] := 0;
  lView [0][1] := 0;
  lView [1][1] := 1;
  lView [2][1] := 0;
  lView [0][2] := 0;
  lView [1][2] := 0;
  lView [2][2] := 1;

  lColor := $0FFFFFF;
end;



  { Work out the basic gradation divisions }

function TDrawWindow.Division (xmin, xmax: Double; steps: Integer): Double;
var
  Step, Order: Double;
begin
  Step := (xmax - xmin) / steps;

  Order := Round (Ln (Step) / Ln (10) - 0.5);

  Step := Step / Exp (Order * Ln (10));

  if Step < 1.4142 then

    Step := 1

  else

    if Step < 3.1623 then

      Step := 2

    else

      Step := 5;

  Division := Step * Exp (Order * Ln (10));
end;


  { Draw the axes for a graph extending over

    xmina .. xmaxa and ymina .. ymaxa }

procedure TDrawWindow.DrawAxes (xmina, xmaxa, ymina, ymaxa, dx, dy: Double;
                                xsteps, ysteps: Integer);
var
  x, y, xmajor, ymajor, Bigstep, Smallstep: Double;
  Cmin, Cmax: Integer;
  Big, Major, DrawMinor: Boolean;
  PMode: TPenMode;
begin
  DrawMinor := True;

  PMode := Canvas.Pen.Mode;

  Canvas.Pen.Mode := pmMerge;

    { Increasing }

  if xmina > xmaxa then
  begin
    x     := xmina;
    xmina := xmaxa;
    xmaxa := x;
  end;

  if ymina > ymaxa then
  begin
    y     := ymina;
    ymina := ymaxa;
    ymaxa := y;
  end;


    { Draw the y-axis }

  Bigstep   := Division (ymina, ymaxa, ysteps);
  Smallstep := Division (ymina, ymaxa, ysteps * 5);

  xmajor := xmina;

  y := Round (ymina / Smallstep + 0.499) * Smallstep;

  while y < ymaxa do
  begin
    Big := Abs (Abs (y) / Bigstep - Int (Abs (y) / Bigstep + 0.5)) < 0.01;

    Major := Big and (Abs (y - ymajor) < 0.01 * Smallstep);


      { Draw the graph lines }

    if Big then

      if Major then

        Color := $0C0C0C0

      else

        Color := $0606060

    else

      Color := $0404040;

    if DrawMinor or Big then
    begin
      MoveTo (xmina, y, 0);
      LineTo (xmaxa, y, 0);
    end;


      { Draw the gradation marks }

    Canvas.Pen.Mode := pmCopy;

    Color := $0C0C0C0;

    MoveTo (xmajor, y, 0);
    LineTo (xmajor + 0.25 * dx, y, 0);


      { Draw the numbers }

    if Abs (y) < 1E-10 then

      y := 0;

    Color := $0FFFFFF;

    if Big then

      DrawString (xmajor - 0.8 * dx, y, 0, dx, dy,
                       Format ('%.2g', [y]), 'R0C0', False);


    y := y + Smallstep;

    Canvas.Pen.Mode := pmMerge;
  end;


    { Draw the x-axis }

  Bigstep   := Division (xmina, xmaxa, xsteps);
  Smallstep := Division (xmina, xmaxa, xsteps * 5);

  if ymina * ymaxa <= 0 then

    ymajor := 0

  else

    ymajor := ymina;

  x := Round (xmina / Smallstep + 0.499) * Smallstep;

  while x < xmaxa do
  begin
    Big := Abs (Abs (x) / Bigstep - Int (Abs (x) / Bigstep + 0.5)) < 0.01;

    Major := Big and (x < (xmaxa - xmina) * 0.001 + xmina);


      { Draw the graph lines }

    if Big then

      if Major then

        Color := $0C0C0C0

      else

        Color := $0606060

    else

      Color := $0404040;

    if DrawMinor or Big then
    begin
      MoveTo (x, ymina, 0);
      LineTo (x, ymaxa, 0);
    end;


      { Draw the gradation marks }

    Canvas.Pen.Mode := pmCopy;

    Color := $0C0C0C0;

    MoveTo (x, ymajor, 0);
    LineTo (x, ymajor - 0.25 * dy, 0);


      { Draw the numbers }

    Color := $0FFFFFF;

    if Big and not Major then

      DrawString (x, ymajor - 0.8 * dy, 0, dx, dy,
                       Format ('%.2g', [x]), 'C0D0', False);


    x := x + Smallstep;

    Canvas.Pen.Mode := pmMerge;
  end;

  Canvas.Pen.Mode := PMode;
end;


procedure TDrawWindow.Border (X, Y, Z: Double);
begin
  X := (lMax [0] - lMin [0]) * X;

  lMin [0] := lMin [0] - X;
  lMax [0] := lMax [0] + X;

  Y := (lMax [1] - lMin [1]) * Y;

  lMin [1] := lMin [1] - Y;
  lMax [1] := lMax [1] + Y;

  Z := (lMax [2] - lMin [2]) * Z;

  lMin [2] := lMin [2] - Z;
  lMax [2] := lMax [2] + Z;
end;


procedure TDrawWindow.GetLimits (const D: Columnvector; var Dmin, Dmax: Double);
var
  C: Integer;
begin
  if D <> nil then

    with D^ do

      if Rows > 0 then
      begin
        Dmin := Row [1];
        Dmax := Dmin;

        for C := 1 to Rows do
        begin
          if Row [C] < Dmin then

            Dmin := Row [C];

          if Row [C] > Dmax then

            Dmax := Row [C];
        end;
      end;
end;


procedure TDrawWindow.GetCurveLimits (const X, Y, Z: Columnvector);
begin
  GetLimits (X, lMin [0], lMax [0]);
  GetLimits (Y, lMin [1], lMax [1]);
  GetLimits (Z, lMin [2], lMax [2]);
end;


procedure TDrawWindow.ApplyView (X, Y, Z: Double);
begin
  if lUseMatrix then
  begin

      { Translate }

    X := X - lCentre [0];
    Y := Y - lCentre [1];
    Z := Z - lCentre [2];

      { Rotate }

    lX := lView [0, 0] * X + lView [1, 0] * Y + lView [2, 0] * Z;
    lY := lView [0, 1] * X + lView [1, 1] * Y + lView [2, 1] * Z;
    lZ := lView [0, 2] * X + lView [1, 2] * Y + lView [2, 2] * Z;

      { Translate }

    lX := lX + lCentre [0];
    lY := lY + lCentre [1];
    lZ := lZ + lCentre [2];
  end
  else
  begin
    lX := X;
    lY := Y;
    lZ := Z;
  end;
end;


function TDrawWindow.GetPixelCoordinates (X, Y, Z: Double): TPoint;
begin

    { Apply the view transformation }

  ApplyView (X, Y, Z);

    { Convert }

  lX := (lX - lMin [0]) / (lMax [0] - lMin [0]);
  lY := (lY - lMin [1]) / (lMax [1] - lMin [1]);
  lZ := (lZ - lMin [2]) / (lMax [2] - lMin [2]);

  GetPixelCoordinates.X := Round (lX * (ClientWidth - 1));
  GetPixelCoordinates.Y := Round ((1 - lY) * (ClientHeight - 1));
end;


procedure TDrawWindow.GetCoordinates (X, Y: Integer);
begin
  lX := (X / (ClientWidth - 1)) * (lMax [0] - lMin [0]) + lMin [0];
  lY := (Y / (ClientHeight - 1)) * (lMin [1] - lMax [1]) + lMax [1];
  lZ := (lMax [2] + lMin [2]) * 0.5;
end;


  { Set the pen colour: 0 = Black, 1 = Nominal }

procedure TDrawWindow.SetColor (C: Double);
var
  R, G, B, MaxRGB: Integer;
begin
  R := (lColor and $0FF0000) div $010000;
  G := (lColor and $000FF00) div $0100;
  B := (lColor and $00000FF);

  MaxRGB := R;

  if G > MaxRGB then

    MaxRGB := G;

  if B > MaxRGB then

    MaxRGB := B;

  if (MaxRGB = 0) or (C < 0) then

    C := 0

  else

    if C > 255 / MaxRGB then

      C := 255 / MaxRGB;

  Canvas.Pen.Color := Round (R * C) * $010000 +
                      Round (G * C) * $0100 +
                      Round (B * C) + $00000000;
end;


procedure TDrawWindow.MoveTo (X, Y, Z: Double);
var
  Point: TPoint;
begin
  Point := GetPixelCoordinates (X, Y, Z);

  lOZ := lZ;

  SetColor (2 * (1 - lZ));

  Canvas.MoveTo (Point.X, Point.Y);
end;


procedure TDrawWindow.LineTo (X, Y, Z: Double);
var
  Point: TPoint;
  Za: Double;
begin
  Za := lOZ;

  Point := GetPixelCoordinates (X, Y, Z);

  lOZ := lZ;

  Za := Za + lZ;

  Za := 2 - Za; { 0: black  1: nominal }

  SetColor (Za);

  Canvas.LineTo (Point.X, Point.Y);
end;


procedure TDrawWindow.Circle (X, Y, Z: Double; Radius: Integer);
var
  Point: TPoint;
begin
  Point := GetPixelCoordinates (X, Y, Z);

  Canvas.Ellipse (Point.X - Radius, Point.Y - Radius,
                  Point.X + Radius, Point.Y + Radius);
end;


procedure TDrawWindow.MouseDown (Button: TMouseButton; Shift: TShiftState;
                                 X, Y: Integer);
begin
  inherited MouseDown (Button, Shift, X, Y);

  GetCoordinates (X, Y);

  if Assigned (lOnMouseDown) then

    lOnMouseDown (Button, Shift, lX, lY);
end;


procedure TDrawWindow.MouseMove (Shift: TShiftState;
                                 X, Y: Integer);
begin
  inherited MouseMove (Shift, X, Y);

  GetCoordinates (X, Y);

  if Assigned (lOnMouseMove) then

    lOnMouseMove (Shift, lX, lY);
end;


procedure TDrawWindow.MouseUp (Button: TMouseButton; Shift: TShiftState;
                               X, Y: Integer);
begin
  inherited MouseUp (Button, Shift, X, Y);

  GetCoordinates (X, Y);

  if Assigned (lOnMouseUp) then

    lOnMouseUp (Button, Shift, lX, lY);
end;


procedure TDrawWindow.DrawCurve (const X, Y, Z: Columnvector);
var
  Data: array [0..64] of TPoint;
  SI, I, Rows, IRows: Integer;
begin
  with Canvas do
  begin
    Pen.Color := Color;

    SI := 0;

    Rows := X.Rows;

    if Y.Rows < Rows then

      Rows := Y.Rows;

    while SI < (Rows - 1) do
    begin
      IRows := Rows - 1 - SI;

      if IRows > 64 then

        IRows := 64;

      for I := 0 to IRows do

        Data [I] := GetPixelCoordinates (X.Row [I + SI + 1],
                                         Y.Row [I + SI + 1], 0);

      PolyLine (Slice (Data, IRows + 1));

      SI := SI + IRows;
    end;
  end;
end;


procedure TDrawWindow.Rotate (X, Y: Double);
var
  Cosa, Sina, Cosb, Sinb, D: Double;
  C: Integer;
begin

  Cosa := Cos (X);
  Sina := Sin (X);

  Cosb := Cos (Y);
  Sinb := Sin (Y);

    { Rotate the view around the x axis }

  for C := 0 to 2 do
  begin
    D            := lView [C, 1] * Cosa + lView [C, 2] * Sina;
    lView [C, 2] := lView [C, 2] * Cosa - lView [C, 1] * Sina;
    lView [C, 1] := D;
  end;

    { Rotate the view around the y axis }

  for C := 0 to 2 do
  begin
    D            := lView [C, 0] * Cosb - lView [C, 2] * Sinb;
    lView [C, 2] := lView [C, 2] * Cosb + lView [C, 0] * Sinb;
    lView [C, 0] := D;
  end;
end;


function TDrawWindow.TextWidth (const Text: string): Double;
begin
  TextWidth := (Canvas.TextWidth (Text) / (ClientWidth - 1)) * (lMax [0] - lMin [0]);
end;


procedure TDrawWindow.TextOut (X, Y: Double; const Text: string);
var
  Point: TPoint;
begin
  Point := GetPixelCoordinates (X, Y, 0);

  Canvas.TextOut (Point.X, Point.Y, Text);
end;


procedure TDrawWindow.DrawXScale (Xstart, Xend, Xstep, Y: Double);
var
  C, Cmin, Cmax: Integer;
  XPixel, YPixel: Double;
  Xtext: Double;
  Xlabel: string;
begin
  MoveTo (Xstart, Y, 0);
  LineTo (Xend, Y, 0);

  Cmin := Round (Xstart / Xstep + 0.5);
  Cmax := Round (Xend / Xstep - 0.5);

  XPixel := (lMax [0] - lMin [0]) / (ClientWidth - 1);
  YPixel := (lMin [1] - lMax [1]) / (ClientHeight - 1);

  for C := Cmin to Cmax do
  begin
    MoveTo (C * Xstep, Y, 0);
    LineTo (C * Xstep, Y + YPixel * 8.2, 0);

    if C * Xstep >= 100 then

      Xlabel := Format ('%.0f', [C * Xstep])

    else

      Xlabel := Format ('%.1f', [C * Xstep]);

    Xtext := C * Xstep - TextWidth (Xlabel) / 2;
    TextOut (Xtext, Y + YPixel * 11.2, Xlabel);
  end;

  Xstep := Xstep / 5;

  Cmin := Round (Xstart / Xstep + 0.5);
  Cmax := Round (Xend / Xstep - 0.5);

  for C := Cmin to Cmax do
  begin
    MoveTo (C * Xstep, Y, 0);
    LineTo (C * Xstep, Y + YPixel * 5, 0);
  end;
end;


  { Draw the character using the vector character set }

procedure TDrawWindow.DrawCharacter (X, Y, Z, dX, dY: Double; C: Char);
var
  VC: PCEA;
  CP: CE;
  Count, Cx, Cy: Integer;
  PDown: Boolean;
begin
  if (Ord (C) >= 0) and (Ord (C) <= 127) then
  begin
    VC := VECTOR_C [Ord (C)];

    if VC <> nil then
    begin
      Count := 0;

      Cx := VC [Count][0];
      Cy := VC [Count][1];

      Inc (Count);

      dX := dX * Cx * 0.0001 / 0.77;
      dY := dY * Cy * 0.0001 / 0.86;

      PDown := False;

      while (True) do
      begin
        CP := VC [Count];

        Inc (Count);

        if CP [0] = -1 then
        begin
          PDown := False;

          if not (CP [1] = 0) then

            Break;
        end
        else
        begin
          if PDown then

            LineTo ((CP [0] - 50) * dX + X,
                     CP [1]       * dY + Y,
                                         Z)

          else

            MoveTo ((CP [0] - 50) * dX + X,
                     CP [1]       * dY + Y,
                                         Z);

          PDown := True;
        end;
      end;
    end;
  end;
end;


procedure TDrawWindow.DrawString (X, Y, Z, dX, dY: Double;
                                  const Text: String; Justify: String;
                                  Rotate: Boolean);
var
  OUseMatrix: Boolean;
  l, C: Integer;
  sX, sY: Double;
begin
  OUseMatrix := lUseMatrix;

  if not Rotate then
  begin

      { Get the rotated coordinates of the origin }

    ApplyView (X, Y, Z);

    X := lX;
    Y := lY;
    Z := lZ;


      { Turn off the rotation }

    lUseMatrix := False;
  end;


    { Justification }

  l := Length (Text);

  Justify := UpperCase (Justify);

  sX := 1;
  sY := 1;


    { Horizonal justification }

  if (Justify [1] = 'L') or (Justify [1] = 'R') then
  begin

    if Justify [1] = 'L' then

      sX := l * 0.5

    else

      sX := -l * 0.5;

    X := X + sX * dX;

    if sX < 0 then

      sX := -1

    else

      sX := 1;
  end;

  X := X + sX * 0.1 * (Ord (Justify [2]) - Ord ('0')) * dX;


    { Vertical justification }

  if (Justify [3] = 'U') or (Justify [3] = 'C') or (Justify [3] = 'D') then
  begin
    if Justify [3] = 'D' then

      sY := -1.0

    else

      if Justify [3] = 'U' then

        sY := 0.5

      else

        sY := -0.5;

    Y := Y + sY * dY;

    if sY < 0 then

      sY := -1

    else

      sY := 1;
  end;

  Y := Y + sY * 0.1 * (Ord (Justify [4]) - Ord ('0')) * dY;


    { Draw the string }

  for C := 1 to l do

    DrawCharacter (X - (0.5 * (l + 1) - C) * dX, Y, Z,
                        0.77 * dX, 1.0 * dY, Text [C]);

  lUseMatrix := OUseMatrix;
end;


function TDrawWindow.Normalize (var X, Y, Z: Double): Double;
var
  D: Double;
begin
  D := X * X + Y * Y + Z * Z;

  if D > 0 then
  begin
    D := Sqrt (D);

    X := X / D;
    Y := Y / D;
    Z := Z / D;
  end;

  Normalize := D;
end;


procedure TDrawWindow.DrawStringx (Xa, Ya, Za, Xb, Yb, Zb, dX, dY: Double;
                                   const Text, Justify: String);
var
  OUseMatrix: Boolean;
  OView: array [0..2, 0..2] of Double;
  X, Y, Z, C: array [0..2] of Double;
  l: Double;
begin

    { Save the view }

  OUseMatrix := lUseMatrix;

  OView [0, 0] := lView [0, 0];
  OView [0, 1] := lView [0, 1];
  OView [0, 2] := lView [0, 2];
  OView [1, 0] := lView [1, 0];
  OView [1, 1] := lView [1, 1];
  OView [1, 2] := lView [1, 2];
  OView [2, 0] := lView [2, 0];
  OView [2, 1] := lView [2, 1];
  OView [2, 2] := lView [2, 2];

    { Get the rotated coordinates of the line }

  ApplyView (Xa, Ya, Za);

  Xa := lX;
  Ya := lY;
  Za := lZ;

  ApplyView (Xb, Yb, Zb);

  Xb := lX;
  Yb := lY;
  Zb := lZ;

    { Get the centre }

  C [0] := (Xa + Xb) * 0.5;
  C [1] := (Ya + Yb) * 0.5;
  C [2] := (Za + Zb) * 0.5;

    { Turn on the rotation }

  lUseMatrix := True;

    { }

  X [0] := Xb - Xa;
  X [1] := Yb - Ya;
  X [2] := Zb - Za;

  l := Normalize (X [0], X [1], X [2]);

  if l > 0 then
  begin
    if X [0] <= 0 then
    begin
      X [0] := -X [0];
      X [1] := -X [1];
      X [2] := -X [2];
    end;

    Y [0] :=  X [1];
    Y [1] := -X [0];
    Y [2] :=  0;

    l := Normalize (Y [0], Y [1], Y [2]);

    if l = 0 then

      Y [1] := 1;

    if Y [1] <= 0 then
    begin
      Y [0] := -Y [0];
      Y [1] := -Y [1];
      Y [2] := -Y [2];
    end;

    Z [0] := X [1] * Y [2] - X [2] * Y [1];
    Z [1] := X [2] * Y [0] - X [0] * Y [2];
    Z [2] := X [0] * Y [1] - X [1] * Y [0];

    lView [0, 0] := X [0];
    lView [0, 1] := X [1];
    lView [0, 2] := X [2];
    lView [1, 0] := Y [0];
    lView [1, 1] := Y [1];
    lView [1, 2] := Y [2];
    lView [2, 0] := Z [0];
    lView [2, 1] := Z [1];
    lView [2, 2] := Z [2];

    lCentre [0] := C [0];
    lCentre [1] := C [1];
    lCentre [2] := C [2];

      { Draw the string }

    DrawString (C [0], C [1], C [2], dX, dY, Text, Justify, True);

    lCentre [0] := 0;
    lCentre [1] := 0;
    lCentre [2] := 0;
  end;

    { Restore the view }

  lView [0, 0] := OView [0, 0];
  lView [0, 1] := OView [0, 1];
  lView [0, 2] := OView [0, 2];
  lView [1, 0] := OView [1, 0];
  lView [1, 1] := OView [1, 1];
  lView [1, 2] := OView [1, 2];
  lView [2, 0] := OView [2, 0];
  lView [2, 1] := OView [2, 1];
  lView [2, 2] := OView [2, 2];

  lUseMatrix := OUseMatrix;
end;

end.


