program Xfit;

uses
  QForms,
  UModel in 'UModel.pas' {Model},
  UDraw in 'UDraw.pas',
  UData in 'UData.pas' {Data},
  UXfit in 'UXfit.pas',
  UConsole in 'UConsole.pas' {Console},
  UOptions in 'UOptions.pas' {Options};

{$R *.res}

begin
  Application.Initialize;
  Application.Title := 'Xfit';
  Application.HelpFile := 'Xfit.hlp';
  Application.CreateForm(TConsole, Console);
  Application.CreateForm(TOptions, Options);
  Application.CreateForm(TModel, Model);
  Application.Run;


 end.
