unit UMultiLineEdit;

interface
uses QStdCtrls,Classes;

procedure Register; // So we add it to forms

type TMultiLineEdit = class (TMemo)
    private
    linedata: array of WideString;  //storage for lines
    datalen: integer;   // lines currently stored
    function GetArrayLength:integer;
    procedure SetArrayLength(nolines:integer);
    function GetLine(lineno:integer):WideString;
    procedure SetLine(lineno:integer; line:WideString);
    public
    procedure Enter(line: WideString);
    procedure LineDelete(pos: integer);
    procedure Initialise(totallines: integer);
    property LineCount:integer read GetArrayLength write SetArrayLength;
    property Lines[lineno:integer]:WideString read GetLine write SetLine;
    end;

implementation

procedure Register;
begin
    RegisterComponents('Samples',[TMultiLineEdit]);
end;

// Initialise the line array
procedure TMultiLineEdit.Initialise(totallines: integer);
begin
   SetLength(linedata,1);
   datalen := 1;
   linedata[0] := '';   //our flag for end of data
end;

//This procedure deletes the first pos lines, ie the oldest
//lines, usually to make way for new ones
procedure TMultiLineEdit.LineDelete(pos:integer);
var nowpos: integer;
begin
    if LineCount > pos then
    begin
        nowpos := 0+pos -1;
        repeat
            nowpos := nowpos+1 ;
            linedata[nowpos-pos] := linedata[nowpos] ;
        until nowpos = datalen;
        datalen := datalen-pos;
    end;
end;

// We add another line to the array
procedure TMultiLineEdit.Enter(line: WideString);
begin
    if Length(linedata) = datalen then
        SetLength(linedata,datalen+1);
    datalen := datalen+1;
    Lines[datalen] := line;
end;

function TMultiLineEdit.GetArrayLength:integer;
begin
    GetArrayLength := datalen
end;

procedure TMultiLineEdit.SetArrayLength(nolines:integer);
begin
    SetLength(linedata,nolines);
end;

function TMultiLineEdit.GetLine(lineno:integer):WideString;
begin
    if lineno <= datalen then
       GetLine := linedata[lineno]
    else GetLine := '';
end;

procedure TMultiLineEdit.SetLine(lineno:integer; line:WideString);
begin
    if lineno <= datalen then
        linedata[lineno] := line;
end;
end.
