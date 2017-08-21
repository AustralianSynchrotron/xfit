unit UXfit;

interface

uses
  UDLL,Classes, QComCtrls, SysUtils, UData, QForms, IniFiles;

type
  TCalDone = procedure (Cal: Integer) of object;

  TCalThread = class (TThread)

  private
    lInt:         Boolean;
    lCal:         Integer;
    lOut:         string;

    lCData:       TData;

    lOnDone:   TCalDone;
    lOnOut:    TNotifyEvent;

    lInOK:     Boolean;
    lInC:      string;
    lIn:       PChar;

    lSymbol: PChar;
    xfithome: string;   //Xfit.ini file location

    procedure Working;
    procedure Done;
    procedure Out;
    procedure Input;
    procedure ModelChanged;
    procedure DeleteData;
    procedure CreateData;
    procedure ChangedData;
    procedure CallHelp;
    procedure SelectFile;
    procedure OutputFile;

    procedure Log (Out: String);

    procedure Comma (var Path: String);

  public
    lPanel:       TStatusPanel;

    lDataDir,
    lLogFile,
    lCurrentDir: string;

    lSelFile,
    lOutputFile:   PChar;

    lHelp: PChar;

    procedure   Int;
    function    Interrupted: Boolean;

    function    NextOut: string;

    constructor Create (Panel: TStatusPanel);

    destructor  Destroy; override;

  protected
    procedure Execute; override;

  published
    property OnDone: TCalDone read lOnDone write lOnDone;
    property OnOut:  TNotifyEvent read lOnOut write lOnOut;
  end;

  PCalThread = ^TCalThread;


implementation

uses
  UConsole, UModel;


  { Look for an event including input from the console

    console <> 0 : get input from the console if available

    Return the next line from the console / nil.
    This procedure called by the Poll function in the context
    of the main thread to avoid synchronisation problems.
    The variables passed tell the GetIn routine which flags
    to set and where to put the input }

procedure TCalThread.Input;
begin
  Console.GetIn (lInC, lInOK);
end;


procedure TCalThread.ModelChanged;
begin
  Model.Refresh;
end;


procedure TCalThread.DeleteData;
begin
  Critical.Enter;

  lCData.Free;

  lCData := nil;

  Critical.Leave;
end;


procedure TCalThread.ChangedData;
begin
  Critical.Enter;

  lCData.PageRefresh;

  lCData := nil;

  Critical.Leave;
end;


procedure TCalThread.CallHelp;
begin
  if lHelp <> nil then

    // Application.HelpCommand (HELP_PARTIALKEY, LongInt (lHelp));

  lHelp := nil;
end;


procedure TCalThread.OutputFile;
var
  C: PChar;
begin
  if Console.SaveFile.Execute then

    C := StrNew (PChar (Console.SaveFile.FileName))

  else

    C := StrNew (PChar (''));

  lOutputFile := C;
end;


procedure TCalThread.SelectFile;
var
  C: PChar;
begin
  if Console.OpenFile.Execute then

    C := StrNew (PChar (Console.OpenFile.FileName))

  else

    C := StrNew (PChar (''));

  lSelFile := C;
end;


procedure TCalThread.CreateData;
begin
  Critical.Enter;

  lCData := TData.Create (Console.Owner);

  lCData.Caption := lSelFile;

  lSelFile := nil;

  lCData.Show;

  Critical.Leave;
end;


const
  Kinterrupt: PChar = 'INTERRUPT';
  K0:         PChar = '0';
  K1:         PChar = '1';
  KFalse:     PChar = 'False';
  KTrue:      PChar = 'True';
  KHelp:      PChar = '';


  { Get or set symbols }

function Symbol (Cal: PCalThread; Key: PChar; Csymbol: PChar): PChar; stdcall;
var
  F: TextFile;
begin

    { }

  with Cal^ do
  begin
    if lSymbol <> nil then

      StrDispose (lSymbol);

    lSymbol := nil;

    if StrIComp (Key, Kinterrupt) = 0 then
    begin
      if Csymbol <> nil then
      begin
        if (StrIComp (Csymbol, K0) = 0) or
           (StrIComp (Csymbol, KFalse) = 0) then

          lInt := False

        else

          if (StrIComp (Csymbol, K1) = 0) or
             (StrIComp (Csymbol, KTrue) = 0) then

            lInt := True;
      end;

      if lInt then

        lSymbol := StrNew (K1)
    end;

    if StrIComp (Key, PChar ('HELP')) = 0 then
    begin
      if Csymbol <> nil then

        lHelp := Csymbol

      else

        lHelp := KHelp;

      Synchronize (CallHelp);

      while lHelp <> nil do

        Sleep (0);
    end;


    if StrIComp (Key, PChar ('LOG FILE')) = 0 then
    begin
      if Csymbol <> nil then

        lLogFile := ExpandFileName (Csymbol)

      else

        lLogFile := '';

      if lLogFile <> '' then

        if FileExists (lLogFile) then
        begin
          AssignFile (F, lLogFile);
          Reset (F);
          CloseFile (F);
          Erase (F);
        end;

      lSymbol := nil;
    end;

    if StrIComp (Key, PChar ('DATA DIRECTORY')) = 0 then
    begin
      if Csymbol <> nil then

        lDataDir := Csymbol;

      Comma (lDataDir);

      lSymbol := StrNew (PChar (lDataDir));
    end;

    if (StrIComp (Key, PChar ('INPUT FILE')) = 0) or
       (StrIComp (Key, PChar ('DATA FILE')) = 0) then
    begin
      Cal.lSelFile := nil;

      Cal.Synchronize (Cal.SelectFile);

      while Cal.lSelFile = nil do

        Sleep (0);

      lSymbol := Cal.lSelFile;

      Cal.lSelFile := nil;
    end;

    if StrIComp (Key, PChar ('OUTPUT FILE')) = 0 then
    begin
      Cal.lOutputFile := nil;

      Cal.Synchronize (Cal.OutputFile);

      while Cal.lOutputFile = nil do

        Sleep (0);

      lSymbol := Cal.lOutputFile;

      Cal.lOutputFile := nil;
    end;

    Symbol := lSymbol;
  end;
end;

// This function is called by the Xfit C library to get input

function Poll (Cal: PCalThread; console: Integer; Mo: PModelData): PChar; stdcall;
var
  OK: Boolean;
  C: string;
begin

  if Mo <> nil then    // We have new model data
  begin
    Model.Change (Mo); //Update our model

    Cal.Synchronize (Cal.ModelChanged); //Execute in main context
  end;

  //Do the poll until we get something, as flagged by lInOK

  if console <> 0 then     //Requests poll
  begin

      { Get the input string }

    Cal.lInOK := False;

    Cal.Synchronize (Cal.Input);  //Execute in main thread

    repeat

      Sleep (0);

    until Cal.lInOK;


      { Copy the input string }

    OK := False;

    if Cal.lIn <> nil then

      OK := StrBufSize (Cal.lIn) - 1 > Length (Cal.lInC);

    if not OK then
    begin
      StrDispose (Cal.lIn);

      Cal.lIn := StrAlloc (Length (Cal.lInC) + 1);
    end;

    Cal.Log (Cal.lInC);

    StrCopy (Cal.lIn, PChar (Cal.lInC));

    Poll := Cal.lIn;    //The result of our polling
  end
  else

    Poll := nil;        //No poll requested
end;


  { Create or modify an xafs window }

function DataW (Cal: PCalThread; Data: PXafsData; Title: PChar): Pointer; stdcall;
var
  C, D, M: Boolean;
  W: TData;
begin
  C := False;
  D := False;
  M := False;

  W := nil;

  while Cal.lCData <> nil do

    Sleep (0);

  Critical.Enter;

    { }

  if Title <> nil then

    if Data <> nil then
    begin
      Cal.lCData := Data.Gid;

      D := True;

      Data.Gid := nil;
    end
    else

      C := True

  else
  begin
    Cal.lCData := Data.Gid;

    if Cal.lCData <> nil then

      Cal.lCData.ChangeData (Data);

    M := True;
  end;

  Critical.Leave;

  if D then

    Cal.Synchronize (Cal.DeleteData);

  if M then

    Cal.Synchronize (Cal.ChangedData);

  if C then
  begin
    Cal.lSelFile := Title;

    Cal.Synchronize (Cal.CreateData);

    while Cal.lCData = nil do

      Sleep (0);

    W := Cal.lCData;

    Cal.lCData := nil;
  end;

  DataW := W;
end;


  { Add an output string }

function Cout (Cal: PCalThread; C: PChar): Integer; stdcall;
begin
  Critical.Enter;

  Cal.lOut := Cal.lOut + StrPas (C);

  Critical.Leave;

  Cal.Synchronize (Cal.Out);
end;


  { Add a string to the log file }

procedure TCalThread.Log (Out: String);
var
  F: TextFile;
  Exists: Boolean;
begin
  if lLogFile <> '' then
  begin
    Exists := FileExists (lLogFile);

    AssignFile (F, lLogFile);

    {$I-}

    if Exists then

      Append (F)

    else

      Rewrite (F);

    {$I+}

    if IOResult = 0 then
    begin
      Write (F, Out);

      CloseFile (F);
    end;
  end;
end;


  { Read the output string }

function TCalThread.NextOut: string;
begin
  Critical.Enter;

  Log (lOut);

  NextOut := lOut;

  lOut := '';

  Critical.Leave;
end;


procedure TCalThread.Int;
begin
  lInt := True;
end;


function TCalThread.Interrupted: Boolean;
begin
  Interrupted := lInt;
end;


procedure TCalThread.Comma (var Path: String);
begin

    { Substitute semicolons for any line breaks in the paths }

  Path := AdjustLineBreaks (Path);

  while Pos (Chr (13), Path) > 0 do

    Path [Pos (Char (13), Path)] := ';';

  Path := Trim (Path);
end;


destructor TCalThread.Destroy;
var
  Ini: TMemIniFile;
begin

    { Save the directories to the INI file }

  Comma (lDataDir);
  Comma (lCurrentDir);

  Ini := TMemIniFile.Create (xfithome+'xfit.ini');

  with Ini do
  begin
    Ini.WriteString ('Status', 'LogFile', lLogFile);
    Ini.WriteString ('Status', 'DataPath', lDataDir);
    Ini.WriteString ('Status', 'CurrentDirectory', lCurrentDir);
  end;

  Ini.UpdateFile;
  Ini.Free;

  inherited Destroy;
end;


constructor TCalThread.Create (Panel: TStatusPanel);
var
  Ini: TMemIniFile;

begin
  inherited Create (True);  // Don't execute immediately

  lSymbol := nil;

  lDataDir    := '';
  lCurrentDir := '';

    { Read the directories from the INI file }

    {The INI file is in directory HOME/.xfit
    on Linux, and not yet decided on Windows}
  xfithome := GetEnvironmentVariable('HOME');
  xfithome := IncludeTrailingPathDelimiter(xfithome);
  xfithome := xfithome +'.xfit';
  xfithome := IncludeTrailingPathDelimiter(xfithome);
  if not DirectoryExists(xfithome) then
      MkDir(xfithome);
  Ini := TMemIniFile.Create (xfithome+'xfit.ini');

  with Ini do
  begin
    lLogFile    := Ini.ReadString ('Status', 'LogFile', '');
    lDataDir    := Ini.ReadString ('Status', 'DataPath', '');
    lCurrentDir := Ini.ReadString ('Status', 'CurrentDirectory', '');
  end;

  Ini.Free;  // No longer needed

  lPanel := Panel;

  lInt := False;

  lCal := 0;

  lOut := '';

  lIn := nil;

  lCData := nil;

  FreeOnTerminate := True;
end;


    {  }
// We execute Xfit as a subthread.  Xfit uses the addresses of
// the Poll function for getting input, Cout for output, Symbol
// for Symbol writing.

procedure TCalThread.Execute;
var
  Cal: Integer;
begin

    { Execute xfit }

  Xfit (Addr (Self), Addr (Poll), Addr (Cout), Addr (DataW), Addr (Symbol));
end;


procedure TCalThread.Working;
begin
  if lPanel <> nil then

    lPanel.Text := Format ('Calculating: %d', [lCal]);
end;


procedure TCalThread.Out;
begin
  if Assigned (lOnOut) then  //Have somewhere to go?

    lOnOut (Self);

  Inc (lCal);
end;


procedure TCalThread.Done;
begin
  if Assigned (lOnDone) then   //wrap things up

    lOnDone (lCal);

  Inc (lCal);
end;


end.
