program SafeGauge_Programmer_6d01;

uses
  Forms,
  ProgramUnit in 'ProgramUnit.pas' {Form1};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
