unit UOptions;

interface

uses
  SysUtils, Classes, QGraphics, QControls, QForms, QDialogs,
  QStdCtrls, QButtons, QComCtrls, QExtCtrls;

type
  TOptions = class (TForm)

    ButtonPanel:  TPanel;
    OptionsPanel: TPanel;
    OptionPages:  TPageControl;
    Directories:  TTabSheet;
    RPanel:       TPanel;
    OK:           TBitBtn;
    Cancel:       TBitBtn;
    Help:         TBitBtn;
    DirBevel:     TBevel;
    SPLabel:      TLabel;
    SearchPath:   TMemo;
    LogFile: TEdit;
    LogLabel:     TLabel;
    ClearLog:     TBitBtn;
    Bevel1:       TBevel;
    Bevel2:       TBevel;
    BrowseLog: TBitBtn;

    procedure FormShow  (Sender: TObject);
    procedure FormClose (Sender: TObject; var Action: TCloseAction);
    procedure ClearLogClick(Sender: TObject);
    procedure LogFileDblClick(Sender: TObject);
    procedure LogFileChange(Sender: TObject);

  private

      { Private declarations }

    lData: String;
    lLog:  String;

  public

      { Public declarations }

  published

    property Data: String  read lData write lData;
    property Log:  String  read lLog  write lLog;

  end;

var
  Options: TOptions;

implementation

uses UConsole;

{$R *.xfm}

procedure TOptions.FormShow (Sender: TObject);
begin

    { Copy the options to the form }

  SearchPath.Text := lData;
  LogFile.Text    := lLog;

  LogFileChange (Sender);
end;


procedure TOptions.FormClose (Sender: TObject; var Action: TCloseAction);
begin

    { Save the options }

  lData := SearchPath.Text;
  lLog  := LogFile.Text;
end;


  { Clear the log file }

procedure TOptions.ClearLogClick (Sender: TObject);
var
  F: TextFile;
begin
  AssignFile (F, LogFile.Text);

  try
    Reset (F);
    if MessageDlg('Erase ' + LogFile.Text + '?',
        mtConfirmation, [mbYes, mbNo], 0) = mrYes then
    begin
      CloseFile (F);
      Erase (F);
    end;

  except
    on EInOutError do
      MessageDlg ('File I/O error.', mtError, [mbOk], 0);
  end;

  LogFileChange (Sender);
end;


  { Browse }

procedure TOptions.LogFileDblClick (Sender: TObject);
begin
  Console.OpenFile.InitialDir := ExtractFileDir (LogFile.Text);

  if Console.OpenFile.Execute then

    LogFile.Text := Console.OpenFile.FileName;
end;


procedure TOptions.LogFileChange (Sender: TObject);
begin
  ClearLog.Enabled := FileExists (LogFile.Text);
end;

end.
