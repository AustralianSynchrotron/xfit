unit USelect;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Buttons, QStdCtrls, QButtons, QControls;

type
  TSelect = class(TForm)
    XAxis: TComboBox;
    YAxis: TComboBox;
    XAxisLabel: TLabel;
    YAxisLabel: TLabel;
    OK: TBitBtn;
    Cancel: TBitBtn;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Select: TSelect;

implementation

{$R *.xfm}

end.
