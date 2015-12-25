program icReaderPrj;

uses
  Forms,windows,
  reader in 'reader.pas' {frmReader};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmReader, frmReader);
  Application.Run;
end.
