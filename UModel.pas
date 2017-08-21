unit UModel;

interface

uses
  SysUtils, Classes, QGraphics,
  QControls, QForms, QDialogs,
  QExtCtrls, QComCtrls, UDraw, QMenus, UChar,
  SyncObjs, QTypes;   //For critical sections

  //This information is necessary for the drawing
  //routines.  The C library actually fills in the
  //values as necessary
type
  AtomData = packed record
    Symbol: PChar;
    Covalent, X, Y, Z, Sigma2: Double;
    Number, Connected: Integer;
  end;

  AtomDatal = array [1..MaxInt div SizeOf (AtomData)] of AtomData;

  PAtomData = ^AtomDatal;

  ModelData = packed record
    Atoms: Integer;  //Number of atoms
    Atom: PAtomData; //Array of atoms
  end;

  PModelData = ^ModelData;  //Point to the model data

  TModel = class (TForm)

    Top:       TPanel;
    Status:    TStatusBar;
    Menu:      TPopupMenu;
    BondDistances: TMenuItem;
    Symbols:   TMenuItem;
    Numbers:   TMenuItem;
    Axes:      TMenuItem;
    WidePen:   TMenuItem;
    Exit:      TMenuItem;
    ZoomPanel: TPanel;
    Zoom:      TSpinEdit;
    N1: TMenuItem;
    Draw: TDrawWindow;

    procedure DrawMouseDown (Button: TMouseButton; Shift: TShiftState;
                             X, Y: Double);
    procedure DrawMouseUp   (Button: TMouseButton; Shift: TShiftState;
                             X, Y: Double);
    procedure DrawMouseMove (Shift: TShiftState; X, Y: Double);
    procedure FormCreate    (Sender: TObject);
    procedure DrawPaint     (Sender: TObject);
    procedure MenuClick     (Sender: TObject);
    procedure ZoomClick (Sender: TObject; NewVal:integer);

    procedure FormResize    (Sender: TObject);
    procedure FormClose     (Sender: TObject; var Action: TCloseAction);
    procedure FormShow      (Sender: TObject);
    procedure FormDestroy(Sender: TObject);

  private

    { Private declarations }

    lMouseDown: Boolean;

    lOMouse, lODraw: array [0..1] of Double;

    lData: ModelData;
    zoomLevel: integer;   // keep track of zoom
    procedure DrawAtoms;
    procedure GraduatedLine (Xa, Ya, Za, Xb, Yb, Zb: Double);
    procedure DrawAtom      (Za: String; Shell: Integer; X, Y, Z: Double;
                                 Connected: Boolean; l: Double);

  public

      { Public declarations }

    procedure Change (Data: PModelData);
  end;

var
  Model: TModel;

implementation

uses UConsole;

{$R *.xfm}

  { Change the model.  New data in Data }

procedure TModel.Change (Data: PModelData);
var
  C: Integer;
begin
  Critical.Enter; //Don't want other threads trying to
                  //change our model data

    { }

  if lData.Atoms > 0 then

    FreeMem (lData.Atom); //Ditch the old stuff

  GetMem (lData.Atom, Data.Atoms * SizeOf (AtomData));

  lData.Atoms := Data.Atoms;  //Transfer number

  for C := 1 to Data.Atoms do //Transfer info
  begin
    lData.Atom [C].Symbol   := Data.Atom [C].Symbol;
    lData.Atom [C].Covalent := Data.Atom [C].Covalent;
    lData.Atom [C].X        := Data.Atom [C].X;
    lData.Atom [C].Y        := Data.Atom [C].Y;
    lData.Atom [C].Z        := Data.Atom [C].Z;
    lData.Atom [C].Sigma2   := Data.Atom [C].Sigma2;
  end;

  Critical.Leave;
end;


procedure TModel.DrawMouseDown (Button: TMouseButton; Shift: TShiftState;
                                X, Y: Double);
begin
  if Button = mbLeft then
  begin
    lMouseDown := True;

    lOMouse [0] := X;
    lOMouse [1] := Y;
  end;

  Draw.MoveTo (X, Y, X - Y);

  Status.Panels [0].Text := Format ('(%.2f, %.2f)', [X, Y]);
end;


procedure TModel.DrawMouseUp (Button: TMouseButton; Shift: TShiftState;
                              X, Y: Double);
begin
  if Button = mbLeft then

    lMouseDown := False;

  Status.Panels [0].Text := Format ('(%.2f, %.2f)', [X, Y]);
end;


procedure TModel.DrawMouseMove (Shift: TShiftState; X, Y: Double);
begin
  if lMouseDown then
  begin
    Draw.Rotate ((lOMouse [1] - Y) * 0.4, (X - lOMouse [0]) * 0.4);

    lOMouse [0] := X;
    lOMouse [1] := Y;

    Draw.Refresh;
  end;

  Status.Panels [0].Text := Format ('(%.2f, %.2f)', [X, Y]);
end;


procedure TModel.FormCreate (Sender: TObject);
var
  C: Integer;
  CP: PChar;
begin
  lMouseDown := False;

  lODraw [0] := Draw.Width;
  lODraw [1] := Draw.Height;

  lData.Atoms := 0;
  zoomLevel := 5;
  Zoom.Value := zoomLevel;
  lData.Atom := nil;

  Show;

  Console.ModelItem.Checked := True;
end;


  { Draw a grey line with a light grey 0.5 mark and a white 1.0 mark }

procedure TModel.GraduatedLine (Xa, Ya, Za, Xb, Yb, Zb: Double);
var
  x, dx, m: array [0..2] of Double;
  l: Double;
  C: Integer;
begin

    { Copy the vector }

  x [0] := Xa;
  x [1] := Ya;
  x [2] := Za;

  dx [0] := Xb - Xa;
  dx [1] := Yb - Ya;
  dx [2] := Zb - Za;


    { Get the length and normalise the vector }

  l := Sqrt (dx [0] * dx [0] + dx [1] * dx [1] + dx [2] * dx [2]);

  dx [0] := dx [0] / l;
  dx [1] := dx [1] / l;
  dx [2] := dx [2] / l;


    { The steps between marks are 0.5 units }

  dx [0] := dx [0] * 0.5;
  dx [1] := dx [1] * 0.5;
  dx [2] := dx [2] * 0.5;


    { The marks are 1 / 20 units long }

  m [0] := dx [0] / 20;
  m [1] := dx [1] / 20;
  m [2] := dx [2] / 20;


    { Draw the line }

  Draw.Color := $0808080;

  Draw.MoveTo (x [0], x [1], x [2]);

  C := 1;

  while C * 0.5 < l + 0.001 do
  begin
    x [0] := x [0] + dx [0];
    x [1] := x [1] + dx [1];
    x [2] := x [2] + dx [2];

    Draw.Color := $0808080;

    Draw.LineTo (x [0] - m [0], x [1] - m [1], x [2] - m [2]);

    if C mod 2 = 1 then

      Draw.Color := $0C0C0C0

    else

      Draw.Color := $0FFFFFF;

    Draw.LineTo (x [0] + m [0], x [1] + m [1], x [2] + m [2]);

    Inc (C);
  end;
end;


  { Draw the atom and the atom label }

procedure TModel.DrawAtom (Za: String; Shell: Integer; X, Y, Z: Double;
                           Connected: Boolean; l: Double);
var
  D: Double;
begin
  with Draw do
  begin

      { If the atom is not connected to anything ... }

    if not Connected then
    begin
      Color := $000FF00;

      MoveTo (X - l, Y, Z);
      LineTo (X    , Y, Z);
      LineTo (X + l, Y, Z);

      MoveTo (X, Y - l, Z);
      LineTo (X, Y    , Z);
      LineTo (X, Y + l, Z);

      MoveTo (X, Y, Z - l);
      LineTo (X, Y, Z    );
      LineTo (X, Y, Z + l);
    end;


      { Draw the label in white }

    Color := $0FFFFFF;


      { Draw the atomic symbol }

    if Symbols.Checked then

      DrawString (X, Y, Z, l, l, Za, 'R2D2', False);


      { Draw the shell number }

    if Numbers.Checked then

      DrawString (X, Y, Z, 0.8 * l, 0.8 * l,
                        Format ('%d', [Shell]), 'L2D5', False);

  end;
end;


  { Draw the atoms }

procedure TModel.DrawAtoms;
var
  Ca, Cb: Integer;
  D: array [0..2] of Double;
  l: Double;
begin

    { Draw a line to all atoms within covalent bond distance + 20% }

  Critical.Enter;

  for Ca := 1 to lData.Atoms do

    lData.Atom [Ca].Connected := 0;

  for Ca := 1 to lData.Atoms do
  begin

    for Cb := Ca + 1 to lData.Atoms do
    begin

        { Get the distance from atom a to atom b }

      D [0] := lData.Atom [Ca].X - lData.Atom [Cb].X;
      D [1] := lData.Atom [Ca].Y - lData.Atom [Cb].Y;
      D [2] := lData.Atom [Ca].Z - lData.Atom [Cb].Z;

      l := Sqrt (D [0] * D [0] + D [1] * D [1] + D [2] * D [2]);


        { The atoms are bonded if ... }

      if l <= (lData.Atom [Ca].Covalent + lData.Atom [Cb].Covalent) * 1.2 then
      begin

          { Mark them }

        lData.Atom [Ca].Connected := 1;
        lData.Atom [Cb].Connected := 1;


          { Draw a green line between the atoms }

        Draw.Color := $000FF00;

        Draw.MoveTo (lData.Atom [Ca].X, lData.Atom [Ca].Y, lData.Atom [Ca].Z);
        Draw.LineTo (lData.Atom [Cb].X, lData.Atom [Cb].Y, lData.Atom [Cb].Z);


          { Draw the distance in angstrom }

        Draw.Color := $0FFFF00;

        if BondDistances.Checked then

          Draw.DrawStringx (lData.Atom [Ca].X, lData.Atom [Ca].Y, lData.Atom [Ca].Z,
                            lData.Atom [Cb].X, lData.Atom [Cb].Y, lData.Atom [Cb].Z,
                            0.15, 0.15, Format ('%.3f', [l]), 'C0U0');
      end;
    end;


    { Draw the atom }

  Draw.Color := $000FF00;

  DrawAtom (lData.Atom [Ca].Symbol, Ca - 1,
            lData.Atom [Ca].X, lData.Atom [Ca].Y, lData.Atom [Ca].Z,
            lData.Atom [Ca].Connected <> 0, 0.2);
  end;

  Critical.Leave;
end;


procedure TModel.DrawPaint (Sender: TObject);
begin

    { Draw the axes }

  if Axes.Checked then
  begin
    GraduatedLine (0, 0, 0, 0, 0, 3);
    GraduatedLine (0, 0, 0, 0, 3, 0);
    GraduatedLine (0, 0, 0, 3, 0, 0);
    GraduatedLine (0, 0, 0, 0, 0, -3);
    GraduatedLine (0, 0, 0, 0, -3, 0);
    GraduatedLine (0, 0, 0, -3, 0, 0);

    Draw.Color := $0FF0000;

    Draw.DrawStringx (2, 0, 0, 3, 0, 0, 0.3, 0.3, '+X', 'C0U0');
    Draw.DrawStringx (0, 2, 0, 0, 3, 0, 0.3, 0.3, '+Y', 'C0U0');
    Draw.DrawStringx (0, 0, 2, 0, 0, 3, 0.3, 0.3, '+Z', 'C0U0');

    Draw.Color := $00000FF;

    Draw.DrawStringx (-2, 0, 0, -3, 0, 0, 0.3, 0.3, '-X', 'C0U0');
    Draw.DrawStringx (0, -2, 0, 0, -3, 0, 0.3, 0.3, '-Y', 'C0U0');
    Draw.DrawStringx (0, 0, -2, 0, 0, -3, 0.3, 0.3, '-Z', 'C0U0');
  end;

    { Draw the atoms }

  DrawAtoms;

end;


procedure TModel.MenuClick (Sender: TObject);
begin
  if Sender = Exit then

    Close

  else
  begin
    if Sender = BondDistances then

      BondDistances.Checked := not BondDistances.Checked;

    if Sender = Symbols then

      Symbols.Checked := not Symbols.Checked;

    if Sender = Numbers then

      Numbers.Checked := not Numbers.Checked;

    if Sender = Axes then

      Axes.Checked := not Axes.Checked;

    if Sender = WidePen then
    begin
      WidePen.Checked := not WidePen.Checked;

      if WidePen.Checked then

        Draw.Canvas.Pen.Width := 2

      else

        Draw.Canvas.Pen.Width := 1;
    end;

    Draw.Refresh;
  end;
end;

procedure TModel.ZoomClick(Sender: TObject; NewVal: integer);
var
 D: Double;
begin
   if NewVal > zoomLevel then
   begin    // Zoom out
       zoomLevel := NewVal;
       D := Sqrt (Sqrt (2));
   end
   else
   begin    // Zoom in
       zoomLevel := NewVal;
       D := 1 / Sqrt (Sqrt (2));
   end;
   Draw.Xmin := Draw.Xmin * D;
   Draw.Xmax := Draw.Xmax * D;
   Draw.Ymin := Draw.Ymin * D;
   Draw.Ymax := Draw.Ymax * D;
   Draw.Zmin := Draw.Zmin * D;
   Draw.Zmax := Draw.Zmax * D;
   Draw.Refresh;
end;


procedure TModel.FormResize (Sender: TObject);
var
  D, Dc: Double;
begin

    { }

  if (Draw.Width > 0) and (Draw.Height > 0) then
  begin
    D  := lODraw [1] / lODraw [0];
    Dc := Draw.Height / Draw.Width;

    if D > Dc then
    begin
      Draw.Xmin := Draw.Xmin * D / Dc;
      Draw.Xmax := Draw.Xmax * D / Dc;
    end
    else
    begin
      Draw.Ymin := Draw.Ymin * Dc / D;
      Draw.Ymax := Draw.Ymax * Dc / D;
    end;

    Draw.Zmin := -Sqrt (Draw.Xmin * Draw.Ymin);
    Draw.Zmax :=  Sqrt (Draw.Xmax * Draw.Ymax);

    lODraw [0] := Draw.Width;
    lODraw [1] := Draw.Height;
  end;
end;

procedure TModel.FormClose (Sender: TObject; var Action: TCloseAction);
begin
  Console.ModelItem.Checked := False;
end;

procedure TModel.FormShow (Sender: TObject);
begin
  Console.ModelItem.Checked := True;
end;

procedure TModel.FormDestroy (Sender: TObject);
begin
  { }
end;

end.
