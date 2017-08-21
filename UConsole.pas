unit UConsole;

interface

uses
  SysUtils, Classes, QGraphics, QControls, SyncObjs,
  QForms, QDialogs, QStdCtrls, QComCtrls, QMenus,
  Qt,   // For key codes
  UXfit, UData, QTypes, QExtCtrls;

type
  TConsole = class (TForm)
    Status:      TStatusBar;
    Menu:        TPopupMenu;
    ChangeFont:  TMenuItem;
    BackC:       TMenuItem;
    FontDialog:  TFontDialog;
    ColorDialog: TColorDialog;
    OptionsItem: TMenuItem;
    ModelItem:   TMenuItem;
    N1:          TMenuItem;
    Exit:        TMenuItem;
    DoPrint:     TMenuItem;
    N2:          TMenuItem;
    DoCopy:      TMenuItem;
    DoPaste:     TMenuItem;
    DoSelectAll: TMenuItem;
    OpenFile:    TOpenDialog;
    SaveFile:    TSaveDialog;
    GetHelp:     TMenuItem;
    Edit: TMemo;

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
    procedure DoPrintClick        (Sender: TObject);
    procedure OptionsItemClick    (Sender: TObject);
    procedure DoSelectAllClick    (Sender: TObject);
    procedure DoCopyClick         (Sender: TObject);
    procedure MenuPopup           (Sender: TObject);
    procedure DoPasteClick        (Sender: TObject);
    procedure GetHelpClick(Sender: TObject);

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

    sofar: string;     { incomplete final line }

    function LineStart: Integer;
    function PrevLineEnd: Integer;

    procedure DoOut (var C: string);
    procedure OnOut (Sender: TObject);
    procedure Term (Sender: TObject);

  public

    lCal: TCalThread;    //The thread belongs to the console

      { Public declarations }

    procedure GetIn (var InC: string; var InOK: Boolean);

  end;

var
  Console: TConsole;

  Critical: TCriticalSection;

implementation

uses UModel, UOptions, QClipbrd;



{$R *.xfm}

{
The Console is an instantiation of the TMemo class, and
is a real cow (apology to cows) of a thing to get right.
We note that in Qt3.0 they have beefed up the support for
multiline text controls, so hopefully a lot of the junk
here will be unnecessary.

What we know so far: the Append method of "Edit" (the TMemo
instance) prepends a line feed, and both it and use of the
selection property normalise any EOL characters found in the
text, so we can just pass these along.  Note that when we
try and select the end of a line with CR/LF (Windows), the
system will overwrite the last two characters, ie remove the
terminating line feed.  So we sometimes need to put in two
CR/LF sets to maintain our linefeed.}

//
// This procedure is an event handler for keys which have
// implications for editing the input
//
procedure TConsole.EditKeyDown (Sender: TObject; var Key: Word;
                                Shift: TShiftState);
var
  Count: Integer;
  C: WideChar;
  T: String;
  checkval: Integer;
begin
  if (Key = 86) and (Shift = [ssCtrl]) then
  begin
    DoPasteClick (Sender);

    Key := 0;
  end;

  if Key = Key_Pause then   //User requests stop

    lCal.Int;   // Set flag to stop calculating

  if lNoDel <> 0 then    //a line with user input
  begin
    if Key = Key_Home then //to start of line
    begin
      // We check that our starting character is
      // after the first character of the line
      if Edit.SelStart >= LineStart then
      begin
        Edit.SelStart := LineStart + lNoDel;
             //Start of line is after prompt (lNoDel
             // characters
        Key := 0;
      end;
    end;
    if (Key = Key_Up) or (Key = Key_Down) then
    begin
      {A key up or down implies scrolling through
      the lines available. We initialise SelStart
      to be the start of the line, and the actual
      selected text to be the bit after the start
      of the line.  Then we blank it by setting the
      selected text to ''}
      Edit.SelStart := LineStart + lNoDel;
      Edit.SelLength := Edit.GetTextLen - Edit.SelStart;
    end;

    if Key = Key_Up then
    begin

        { Look for a line matching the first lNoDel characters.
        This will select lines starting with the prompt
        characters. We can't use the Lines property to get
        the value of the last line, as it is not updated until
        a carriage return is entered, it seems }

      for Count := lPrev - 1 downto 1 do //search all previous lines
        begin
        if StrLComp(PChar(Edit.Lines [Edit.Lines.Count - 1]),
                    PChar(Edit.Lines [Count - 1]),lNoDel) = 0 then
        begin  //we matched at line number Count
          lPrev := Count;  //current match

          Key := 0;        //clear key value

          T := Edit.Lines [Count - 1];  //Line display

          T := TrimRight(T); // No linefeeds allowed

          Edit.SelText := Copy (T, lNoDel + 1, MaxInt);
                          //Copied maximum possible
          Edit.SelLength := 0;
          Break;          //stop looping
        end;
      end;
      if Key <> 0 then    //nothing found

        lPrev := 0;       //beginning of line list

      Key := 0;           //blank it out
    end;

    if Key = Key_Down then
    begin

        { Look for a line matching the first lNoDel characters.
        Loop up to total number of lines }

      for Count := lPrev + 1 to Edit.Lines.Count - 1 do

        if StrLComp (PChar (Edit.Lines [Edit.Lines.Count - 1]),
                     PChar (Edit.Lines [Count - 1]), lNoDel) = 0 then
        begin
          lPrev := Count;

          Key := 0;

          T := Edit.Lines [Count - 1];

          T := TrimRight(T);

          Edit.SelText := Copy (T, lNoDel + 1, MaxInt);
          Edit.SelLength := 0;

          Break;
        end;

      if Key <> 0 then       //nothing found
      begin
        lPrev := Edit.Lines.Count;  //current line
        Edit.SelLength := 0; //reset to zero
        Key := 0;
      end;
    end;
  end;

  if (Key = Key_Backspace) then

    if Edit.GetTextLen > 0 then  //Have something
    begin

        { Look for LF within lNoDel characters }

      if Edit.GetTextLen <= lNoDel then  //have a current line

        Key := 0      //Flag that we have taken care of it

      else

        for Count := 0 to lNoDel do  //move along the line
        begin
          C := Edit.Text [Edit.SelStart - Count];

          if C = Chr (10) then   //found a line feed
          begin
            Key := 0;       //flag it and quit looking

            Break;
          end;
        end;
    end;
end;

// Called when the calculation finishes
procedure TConsole.Term (Sender: TObject);
begin

    { }

  GetDir (0, lCal.lCurrentDir);

  Status.Panels [0].Text := '';

  Close;
end;

{ The following procedure handles printout of
data from the low-level library.  Apart from
the prompt, this will be in line-based form. We
cannot assume that the previous call finished
the line, as the library sometimes outputs a
series of dashes when it indicates errors}

procedure TConsole.OnOut (Sender: TObject);
var
  NOut: AnsiString;
  outcount,Count,outlen: Integer;
  nolines: Integer;   //for debugging
  C: Char;
begin
  NOut := AnsiString(lCal.NextOut);   //Waiting output text
  outlen := Length(NOut);
  if outlen > 0 then  //Have something
  begin

      { Keep at least 500 lines }


    if Edit.Lines.Count > 500 then
    begin
      Edit.Enabled := False;

      while Edit.Lines.Count > 500 do
      begin
        Edit.Lines.Delete (0);
        Edit.Lines.Delete (0);
        Edit.Lines.Delete (0);
        Edit.Lines.Delete (0);
      end;

      Edit.Enabled := True;

      Edit.SetFocus;
    end;
    for outcount := 1 to outlen do
    begin
        if (NOut[outcount] = #10) then
        begin
           {We output the final line as a line,
            replacing the previous final line.  Seems
            like the only way to get around the
            multi-system problem is to explicitly check.}
           {$IFDEF MSWINDOWS}
           Edit.Lines[Edit.Lines.Count] := sofar;
           {$ENDIF}
           {$IFDEF LINUX}
           Edit.Lines[Edit.Lines.Count] := sofar;
           {$ENDIF}
           sofar := '';
        end
        else
           sofar := sofar + NOut[outcount];
    end;

    Edit.SelStart := Length(Edit.Text);
  end;
end;


procedure TConsole.FormCreate (Sender: TObject);
begin
  // Critical.Acquire();
  Critical:= TCriticalSection.Create;       // Make sure we have a variable

  lCount := 0;

  lNoDel := 2;   //2 non-deletable characters

  lComCount := 0;

  lCCommand := 0;

  // This line serves a dual purpose by working around an
  // apparent bug in the TMemo code
  
  Edit.Append('XFit Version 1.2(beta), Copyright ASRP 1996-2003');

  lCal := TCalThread.Create (Status.Panels [0]);

  SetCurrentDir (lCal.lCurrentDir);

  lCal.OnOut := OnOut;     //Point to output routine

  lCal.OnTerminate := Term;//Finish up

  lInput := False;         //No input allowed
  sofar := '';             //Nothing on last line

  Exit.Enabled := False;   //Not allowed to exit right now
  lCal.Resume;             //Get Xfit up and polling
end;

//
// This procedure is called from the TCalThread object to
// switch on input from the Console.  If we have any previous
// output pending, we can now print it out
//
procedure TConsole.GetIn (var InC: string; var InOK: Boolean);
begin

    { }

  lIn := @InC;              //Where to put chars
  lInOK := @InOK;           //How to flag chars presence

  lInput := True;           //Accepting input

  Exit.Enabled := True;     //Can exit if requested

  //If we are waiting for input, we can output any incomplete
  //lines (eg prompts).  This will always start at the
  //beginning of the current line, so we blank it (if
  //necessary) before proceeding

  if Length(sofar) > 0 then
      begin
      if LineStart < Length(Edit.Text) then
          begin
              Edit.SelStart := LineStart+1;
              Edit.SelLength := Length(Edit.Text)-LineStart-1;
          end
          else
          begin
              Edit.SelStart := Length(Edit.Text);
              Edit.SelLength := 0;
          end;
      Edit.SelText := sofar;
      lNoDel := Length(Edit.Text) - LineStart;
      sofar := '';    //No more system output expected on this line
  end;
  lPrev := Edit.Lines.Count;  //number of previous lines

  Edit.SelStart := Length(Edit.Text);  //Add from here

  Edit.ReadOnly := False;   //Can edit freely
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

//
// When any ascii key is pressed, this routine is
// called.
//
procedure TConsole.EditKeyPress (Sender: TObject; var Key: Char);
var
  C: string;
begin
  if Key <> Chr (13) then     //Not carriage return
  begin
    if Key <> ' ' then        //Not space

      C := Trim (Key)         //Get rid of spaces/control chars

    else

      C := Key;               //Preserve the space

    if C <> '' then           //Something is left
    begin
      DoOut (C);              //Add the new text

      Key := Chr (0);         //Blank the key
    end;
  end;

  if not Edit.ReadOnly then   //We can take input

    if Key = Chr (13) then    //Accept string for processing
    begin

        { }

      Exit.Enabled := False;  //Disable exit on menu

        { Copy the characters starting at NoDel.  We append
        a linefeed to signal to the parser that this is the end
        of the input.  }

      lIn^ := Copy (Edit.Text, LineStart + lNoDel + 1,
                               Length(Edit.Text)) + Chr (10);

      // Now we complete the last line by appending (which takes
      // care of system-dependent line feeds) the contents of
      // sofar over the last line

      sofar := Copy (Edit.Text,LineStart+1,Length(Edit.Text));
      Edit.SelStart := LineStart;
      Edit.SelLength := Length(Edit.Text)-LineStart;
      Edit.ClearSelection();
      {$IFDEF MSWINDOWS}
      Edit.Append(sofar);
      {$ENDIF}
      {$IFDEF LINUX}
      Edit.Append(sofar);
      {$ENDIF}
      sofar := '';       //Blank it out
      Edit.SelStart := Edit.GetTextLen;
      Edit.SelLength := 0  ;  //Select the last line feed for overwriting

      Key := Chr (0);    //Blank the key

      // Now flag that we can process.  We do this after our
      // console output to avoid overlap

                         {we are given the lIn pointer by
                         the call to InOut from the calculation
                         thread.  Here we write it in and flag
                         that fact}
      lInOK^ := True;    //Flag that we can process

      lInput := False;   //No more inputting for now

    end;
end;


function TConsole.LineStart: Integer;
var
  Count, Count0: Integer;
  C,D: Char;
begin

    { Find the start of the current line. The value
    returned is the index of the first character in
    the line -1}

  Count0 := 1;

  for Count := Edit.GetTextLen downto 1 do
  begin
    C := Char(Edit.Text [Count]);

    if (C = Chr (10)) or (C = Chr (13)) then  //end of line is 0A or 0D
    begin
      Count0 := Count;
      Break;
    end;
  end;
LineStart := Count0; //final eol character
end;

function TConsole.PrevLineEnd: Integer;
var
  Count, Count0: Integer;
  C: Char;
begin

    { Find the end of the previous line }

  Count0 := 1;

  for Count := LineStart-1 downto 1 do
  begin
    C := Char(Edit.Text [Count]);

    if (C <> Chr (10)) and (C <> Chr (13)) then  //end of line is 0A or 0D
    begin
      Count0 := Count;   //The Count variable goes out of scope

      Break;
    end;
  end;

  PrevLineEnd := Count0+1; //following eol character
end;

  { Someone changed the selection }

procedure TConsole.EditSelectionChange (Sender: TObject);
var
  OK: Boolean;
begin
  OK := lInput;    // Alright to edit?

  if OK then

      { Check that the cursor is in the input region }

    if Edit.SelStart < LineStart + lNoDel  then

      OK := False;   // Editing old stuff; forget it

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
  Critical.Release();
end;

//Called when user bails out via menu

procedure TConsole.ExitClick (Sender: TObject);
begin
  Exit.Enabled := False;   //Can't do it twice

  while not lInput do      //Not accepting input

    Sleep (0);             //Wait a while

  lIn^ := 'end' + Chr (10); //send 'end' command

  lInOK^ := True;          //Flag input waiting

  lInput := False;         //Disallow further input
end;


procedure TConsole.DoPrintClick (Sender: TObject);
begin
  // Edit.Print ('Xfit');
end;


procedure TConsole.OptionsItemClick (Sender: TObject);
begin
  Options.Data := lCal.lDataDir;
  Options.Log  := lCal.lLogFile;

  Options.ShowModal;

  if Options.ModalResult = mrOK then
  begin
    lCal.lDataDir := Options.Data;
    lCal.lLogFile := Options.Log;
  end;
end;


procedure TConsole.DoSelectAllClick (Sender: TObject);
begin
  Edit.SelectAll;
end;


procedure TConsole.DoCopyClick (Sender: TObject);
begin
  Edit.CopyToClipboard;
end;


procedure TConsole.MenuPopup (Sender: TObject);
begin
  DoCopy.Enabled := Edit.SelLength > 0;

  DoPaste.Enabled := Clipboard.Provides ('text/plain');
end;

{This is called when a character or string needs to be
output to the currently selected position.  We never emit
more than one line.

We adopt the principle that we never emit an end-of-line
character ourselves, to enable cross-platform portability
which seems otherwise elusive.  The Append method prepends
an EOL, and any internal newlines/linefeeds in select-related
methods are automatically translated to EOLs for the appropriate
platform.}

{More comments: we try line-based insertion etc.}

procedure TConsole.DoOut (var C: string);
var
   addtoend: Boolean;
   endline: integer;
begin
  if (Edit.SelLength = 0) or Edit.ReadOnly then
     begin
       Edit.SelStart := Edit.GetTextLen;
       endline := Length(Edit.Text);
       Edit.SelLength := 0;
       Edit.SelText := C;
       Edit.SelStart := Edit.GetTextLen;
       Edit.SelLength := 0;
       // Append to our running total line
       sofar := sofar + C;
     end
  else  Edit.SelText := C;
end;


  { Paste ONE line from the clipboard }

procedure TConsole.DoPasteClick (Sender: TObject);
var
  C: String;
begin
  if Clipboard.Provides ('text/plain') then
  begin
    C := Clipboard.AsText;

    DoOut (C);
  end;
end;


procedure TConsole.GetHelpClick (Sender: TObject);
begin
  Application.ContextHelp (1);
end;

end.
