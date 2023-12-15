program SafeGauge_Programmer_103d;

uses
  Forms,
  ProgramUnit in 'ProgramUnit.pas' {Form1};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
