unit UConsoleS;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ComCtrls, Menus, UXfit, UData;

type
  TConsole = class (TForm)

    Edit:        TRichEdit;
    Status:      TStatusBar;
    Menu:        TPopupMenu;
    ChangeFont:  TMenuItem;
    BackC:       TMenuItem;
    FontDialog:  TFontDialog;
    ColorDialog: TColorDialog;
    Properties:  TMenuItem;
    ModelItem:   TMenuItem;
    N1:          TMenuItem;
    Exit:        TMenuItem;

    procedure EditKeyDown         (Sender: TObject; var Key: Word;
                                   Shift: TShiftState);
    procedure FormCreate          (Sender: TObject);
    procedure ChangeFontClick     (Sender: TObject);
    procedure BackCClick          (Sender: TObject);
    procedure EditKeyPress        (Sender: TObject; var Key: Char);
    procedure EditSelectionChange (Sender: TObject);
    procedure ModelItemClick      (Sender: TObject);
    procedure FormDestroy         (Sender: TObject);
    procedure ExitClick           (Sender: TObject);
    procedure EditChange          (Sender: TObject);

  private

      { Private declarations }

    lCount: Integer;

    lNoDel: Integer;

    lPrev: Integer;

    lCommand: array [0..63] of String;

    lComCount, lCCommand: Integer;

    lIn: ^string;

    lInOK: ^Boolean;

    lInput: Boolean;

    lData: TData;

    function LineStart: Integer;

    procedure OnOut (Sender: TObject);
    procedure Term (Sender: TObject);

  public

    lCal: TCalThread;

      { Public declarations }

    procedure GetIn (var InC: string; var InOK: Boolean);

  end;

var
  Console: TConsole;

  Critical: TRTLCriticalSection;

implementation

uses UModel;



{$R *.DFM}

procedure TConsole.EditKeyDown (Sender: TObject; var Key: Word;
                                Shift: TShiftState);
var
  Count: Integer;
  C: Char;
begin
  if lNoDel <> 0 then
  begin
    if Key = VK_HOME then
    begin
      if Edit.SelStart >= LineStart - 1 then
      begin
        Edit.SelStart := LineStart + lNoDel - 1;

        Key := 0;
      end;
    end;

    if (Key = VK_UP) or (Key = VK_DOWN) then
    begin
      Edit.SelStart := LineStart + lNoDel - 1;
      Edit.SelLength := Edit.GetTextLen - LineStart;

      Edit.SelText := '';
    end;

    if Key = VK_UP then
    begin

        { Look for a line matching the first lNoDel characters }

      for Count := lPrev - 1 downto 1 do

        if StrLComp (PChar (Edit.Lines [Edit.Lines.Count - 1]),
                     PChar (Edit.Lines [Count - 1]), lNoDel) = 0 then
        begin
          lPrev := Count;

          Edit.SelText := Copy (Edit.Lines [Count - 1], lNoDel + 1, MaxInt);

          Break;
        end;

      Key := 0;
    end;

    if Key = VK_DOWN then
    begin

        { Look for a line matching the first lNoDel characters }

      for Count := lPrev + 1 to Edit.Lines.Count - 1 do

        if StrLComp (PChar (Edit.Lines [Edit.Lines.Count - 1]),
                     PChar (Edit.Lines [Count - 1]), lNoDel) = 0 then
        begin
          lPrev := Count;

          Key := 0;

          Edit.SelText := Copy (Edit.Lines [Count - 1], lNoDel + 1, MaxInt);

          Break;
        end;

      if Key <> 0 then
      begin
        lPrev := Edit.Lines.Count;

        Key := 0;
      end;
    end;
  end;

  if (Key = VK_BACK) then

    if Edit.GetTextLen > 0 then
    begin

        { Look for CR or LF within lNoDel characters }

      if Edit.GetTextLen <= lNoDel then

        Key := 0

      else

        for Count := 0 to lNoDel do
        begin
          C := Edit.Text [Edit.SelStart - Count];

          if (C = #13) or (C = #10) then
          begin
            Key := 0;

            Break;
          end;
        end;
    end;
end;


procedure TConsole.Term (Sender: TObject);
begin
    { }

  Status.Panels [0].Text := '';

  Close;
end;


procedure TConsole.OnOut (Sender: TObject);
var
  Out: string;
  Count: Integer;
  C: Char;
begin
  Out := lCal.NextOut;

  if Length (Out) > 0 then
  begin

      { Keep at least 1000 lines }


    while Edit.Lines.Count > 1000 do
    begin
      Edit.Enabled := False;

      Edit.Lines.Delete (0);
    end;

    if not Edit.Enabled then
    begin
      Edit.Enabled := True;

      Edit.SetFocus;
    end;

    Edit.SelStart := Edit.GetTextLen;
    Edit.SelLength := 0;

    Edit.SelText := Out;

      { Set NoDel to the current line length }

    for Count := Edit.GetTextLen downto 1 do
    begin
      C := Edit.Text [Count];

      if (C = #13) or (C = #10) then
      begin
        lNoDel := Edit.GetTextLen - Count;

        Break;
      end;
    end;
  end;
end;


procedure TConsole.FormCreate (Sender: TObject);
begin
  InitializeCriticalSection (Critical);

  lCount := 0;

  lNoDel := 2;

  lComCount := 0;

  lCCommand := 0;

  Edit.Lines [0] := '';

  lCal := TCalThread.Create (Status.Panels [0]);

  lCal.OnOut := OnOut;

  lCal.OnTerminate := Term;

  lInput := False;
end;


procedure TConsole.GetIn (var InC: string; var InOK: Boolean);
begin

    { }

  lIn := @InC;
  lInOK := @InOK;

  lInput := True;

  lPrev := Edit.Lines.Count;

  Edit.SelStart := Edit.GetTextLen;

  Edit.ReadOnly := False;
end;


procedure TConsole.ChangeFontClick (Sender: TObject);
var
  O, l: Integer;
begin
  FontDialog.Font := Edit.Font;

  if FontDialog.Execute then
  begin
    O := Edit.SelStart;
    l := Edit.SelLength;

    Edit.Font := FontDialog.Font;

    Edit.SelStart := O;
    Edit.SelLength := l;
  end;
end;


procedure TConsole.BackCClick (Sender: TObject);
begin
  ColorDialog.Color := Edit.Color;

  if ColorDialog.Execute then

    Edit.Color := ColorDialog.Color;
end;


procedure TConsole.EditKeyPress (Sender: TObject; var Key: Char);
begin
  if not Edit.ReadOnly then

    if Key = #13 then
    begin

        { }

      Edit.SelStart := Edit.GetTextLen;

        { Copy the characters from NoDel }

      lIn^ := Copy (Edit.Text, LineStart + lNoDel,
                               Edit.GetTextLen) + #10;

      lInOK^ := True;

      lInput := False;
    end;
end;


function TConsole.LineStart: Integer;
var
  Count, Count0: Integer;
  C: Char;
begin

    { Find the start of the current line }

  Count0 := 1;

  for Count := Edit.GetTextLen downto 1 do
  begin
    C := Edit.Text [Count];

    if (C = #13) or (C = #10) then
    begin
      Count0 := Count + 1;

      Break;
    end;
  end;

  LineStart := Count0;
end;


  { Edit read-only? }

procedure TConsole.EditSelectionChange (Sender: TObject);
var
  OK: Boolean;
begin
  OK := lInput;

  if OK then

      { Check that the cursor is in the input region }

    if Edit.SelStart < LineStart + lNoDel - 1 then

      OK := False;

  if Edit.ReadOnly <> not OK then

    Edit.ReadOnly := not OK;
end;


procedure TConsole.ModelItemClick (Sender: TObject);
begin
  if ModelItem.Checked then

    Model.Close

  else

    Model.Show;
end;


procedure TConsole.FormDestroy (Sender: TObject);
begin
  DeleteCriticalSection (Critical);
end;


procedure TConsole.ExitClick (Sender: TObject);
begin
  Close;
end;


procedure TConsole.EditChange (Sender: TObject);
begin
  { }
end;

end.
