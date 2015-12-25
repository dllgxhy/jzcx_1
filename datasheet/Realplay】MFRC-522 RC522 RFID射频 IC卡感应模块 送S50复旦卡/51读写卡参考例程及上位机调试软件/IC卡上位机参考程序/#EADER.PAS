unit reader;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ComCtrls, ExtCtrls, StdCtrls, SPComm;

type
  TfrmReader = class(TForm)
    pgcReader: TPageControl;
    tbsLowLevel: TTabSheet;
    tbsConfig: TTabSheet;
    tbsPassDown: TTabSheet;
    tbsDataOper: TTabSheet;
    tbsBlockOper: TTabSheet;
    tbsPassCh: TTabSheet;
    pnlTitle: TPanel;
    tbsSeRe: TTabSheet;
    gbLoweOper: TGroupBox;
    btnSeekCard: TButton;
    btnCollision: TButton;
    btnSelect: TButton;
    btnTerminate: TButton;
    gbLowRe: TGroupBox;
    memLowRe: TMemo;
    gbPasswordDown: TGroupBox;
    rgSePwdOrg: TRadioGroup;
    btnPwdDwn: TButton;
    lblPDSector0: TLabel;
    edtSecPwd0: TEdit;
    edtSecPwd1: TEdit;
    lblPDSector1: TLabel;
    edtSecPwd2: TEdit;
    lblPDSector2: TLabel;
    edtSecPwd3: TEdit;
    lblPDSector3: TLabel;
    edtSecPwd4: TEdit;
    lblPDSector4: TLabel;
    edtSecPwd5: TEdit;
    lblPDSector5: TLabel;
    edtSecPwd6: TEdit;
    lblPDSector6: TLabel;
    edtSecPwd7: TEdit;
    lblPDSector7: TLabel;
    edtSecPwd8: TEdit;
    lblPDSector8: TLabel;
    lblPDSector9: TLabel;
    edtSecPwd9: TEdit;
    lblPDSectorA: TLabel;
    edtSecPwd10: TEdit;
    lblPDSectorB: TLabel;
    edtSecPwd11: TEdit;
    lblPDSectorC: TLabel;
    edtSecPwd12: TEdit;
    lblPDSectorD: TLabel;
    edtSecPwd13: TEdit;
    lblPDSectorE: TLabel;
    edtSecPwd14: TEdit;
    lblPDSectorF: TLabel;
    edtSecPwd15: TEdit;
    gbRWSector: TGroupBox;
    lblBlock0: TLabel;
    edtBlock0: TEdit;
    lblBlock1: TLabel;
    edtBlock1: TEdit;
    lblBlock2: TLabel;
    edtBlock2: TEdit;
    lblBlock3: TLabel;
    edtBlock3: TEdit;
    gbRWSeSec: TGroupBox;
    cbRWSec: TComboBox;
    btnBlockRd: TButton;
    btnBlockWt: TButton;
    gbReOrWt: TGroupBox;
    lblCurValue: TLabel;
    lblOpeValue: TLabel;
    edtCurValue: TEdit;
    edtOpeValue: TEdit;
    gbBkSec: TGroupBox;
    gbBlokSe: TGroupBox;
    cbBSecSe: TComboBox;
    cbBSe: TComboBox;
    btnBlockInit: TButton;
    btnBlockRead: TButton;
    btnBlockAdd: TButton;
    btnBlockSub: TButton;
    gbEntryPwd: TGroupBox;
    lblPwdA: TLabel;
    edtPwdA: TEdit;
    lblPwdB: TLabel;
    edtPwdB: TEdit;
    gbConBit: TGroupBox;
    lblConBit0: TLabel;
    edtConBit0: TEdit;
    lblConBit1: TLabel;
    edtConBit1: TEdit;
    lblConBit2: TLabel;
    edtConBit2: TEdit;
    lblConBit3: TLabel;
    edtConBit3: TEdit;
    gbChPwdSec: TGroupBox;
    cbChPwdSec: TComboBox;
    btnChPwd: TButton;
    btnChCon: TButton;
    gbComSendRec: TGroupBox;
    lblExplain: TLabel;
    memComSeRe: TMemo;
    comReader: TComm;
    gbCommSet: TGroupBox;
    btnChComm: TButton;
    btnTestReader: TButton;
    lblComName: TLabel;
    lblBaudRate: TLabel;
    lblParity: TLabel;
    lblByteSize: TLabel;
    lblStopBits: TLabel;
    cbComName: TComboBox;
    cbBaudRate: TComboBox;
    cbParity: TComboBox;
    cbByteSize: TComboBox;
    cbStopBits: TComboBox;
    memComSet: TMemo;
    procedure FormShow(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure comReaderReceiveData(Sender: TObject; Buffer: Pointer;
      BufferLength: Word);
    procedure btnTestReaderClick(Sender: TObject);
    procedure btnSeekCardClick(Sender: TObject);
    procedure btnCollisionClick(Sender: TObject);
    procedure btnSelectClick(Sender: TObject);
    procedure btnTerminateClick(Sender: TObject);
    procedure btnPwdDwnClick(Sender: TObject);
    procedure btnBlockRdClick(Sender: TObject);
    procedure btnBlockWtClick(Sender: TObject);
    procedure btnBlockInitClick(Sender: TObject);
    procedure cbRWSecChange(Sender: TObject);
    procedure btnBlockReadClick(Sender: TObject);
    procedure btnBlockAddClick(Sender: TObject);
    procedure btnBlockSubClick(Sender: TObject);
    procedure btnChPwdClick(Sender: TObject);
    procedure btnChCommClick(Sender: TObject);
    procedure memLowReDblClick(Sender: TObject);
  private
    { Private declarations }
    function exchData(orderStr : string):string;
    procedure sendData();
    procedure checkOper();
  public
    { Public declarations }
  end;

var
    frmReader : TfrmReader;
    curOrderNo : integer=0;
    curOperNo : integer=0;
    orderLst,recDataLst : Tstrings;
    buffer : array [0..2048] of byte;

implementation

{$R *.dfm}
//转找发送数据格式
function TfrmReader.exchData(orderStr : string):string;
var
    ii,jj : integer;
    TmpStr : string;
    reTmpStr : string;
begin
    if (length(orderStr)=0) then
    begin
        MessageBox(handle,'传入参数不能为空!','错误',MB_ICONERROR+MB_OK);
        result:='';
        exit;
    end;
    if (length(orderStr) mod 2)<>0 then
    begin
        MessageBox(handle,'传入参数错误!','错误',MB_ICONERROR+MB_OK);
        result:='';
        exit;
    end;
    for ii:=1 to (length(orderStr) div 2) do
    begin
        tmpStr:=copy(orderStr,ii*2-1,2);
        jj:=strToint('$'+tmpStr);
        reTmpStr:=reTmpStr+chr(jj);
    end;
    result:=reTmpStr;
end;

//发送数据过程
procedure tFrmReader.sendData();
var
    orderStr : string;
begin
    if orderLst.Count>curOrderNo then
    begin
        orderStr:=orderLst.Strings[curOrderNo];
        memComSeRe.Lines.Add('==>> '+orderStr);
        orderStr:=exchData(orderStr);
        comReader.WriteCommData(pchar(orderStr),length(orderStr));
        inc(curOrderNo);
    end;
end;

//检查返回的数据
procedure tfrmReader.checkOper();
var
    i : integer;
    tmpStr : string;
begin
    case curOperNo of
        0 : begin
                for i:=0 to recDataLst.Count-1 do
                    if copy(recDataLst.Strings[i],3,2)<>'00' then
                    begin
                        memComSet.Lines.Add('读卡器连结失败');
                        memComSet.Lines.Add('');
                        exit;
                    end;
                memComSet.Lines.Add('读卡器连结成功');
                memComSet.Lines.Add('');
            end;
        1 : begin
                memLowRe.Lines.Add('命令: 寻卡');
                if copy(recDataLst.Strings[0],3,2)<>'00' then
                    memLowRe.Lines.Add('结果: 寻卡失败')
                else begin
                    memLowRe.Lines.Add('结果: 寻卡成功');
                    if copy(recDataLst.Strings[0],5,2)='04' then
                        memLowRe.Lines.Add('该卡片为Mifare one')
                    else
                        memLowRe.Lines.Add('该卡片为其它类型');
                end;
                memLowRe.Lines.Add('');
            end;
        2 : begin
                memLowRe.Lines.Add('命令: 防冲突');
                if copy(recDataLst.Strings[0],3,2)<>'00' then
                    memLowRe.Lines.Add('结果: 防冲突失败')
                else begin
                    memLowRe.Lines.Add('结果: 防冲突成功');
                    tmpStr:=recDataLst.Strings[0];
                    tmpStr:=copy(tmpStr,5,length(tmpStr)-4);
                    memLowRe.Lines.Add('序号: '+tmpStr);
                end;
                memLowRe.Lines.Add('');
            end;
        3 : begin
                memLowRe.Lines.Add('命令: 选择');
                if copy(recDataLst.Strings[0],3,2)<>'00' then
                    memLowRe.Lines.Add('结果: 选择失败')
                else
                    memLowRe.Lines.Add('结果: 选择成功');
                memLowRe.Lines.Add('');
            end;
        4 : begin
                memLowRe.Lines.Add('命令: 终止');
                if copy(recDataLst.Strings[0],3,2)<>'00' then
                    memLowRe.Lines.Add('结果: 终止失败')
                else
                    memLowRe.Lines.Add('结果: 终止成功');
                memLowRe.Lines.Add('');
            end;
        5 : begin
                for i:=0 to recDataLst.Count-1 do
                    if copy(recDataLst.Strings[i],3,2)<>'00' then
                    begin
                        MessageBox(handle,'密码下载失败','失败',MB_OK);
                        exit;
                    end;
                MessageBox(handle,'密码下载成功','成功',MB_OK);
            end;
        6 : begin
                for i:=0 to 3 do
                begin
                    if copy(recDataLst.Strings[i+4],3,2)<>'00' then
                    begin
                        gbRWSector.Caption:=cbRWSec.Text+'读取失败';
                        exit;
                    end;
                end;
                edtBlock0.Text:=copy(recDataLst.Strings[4],5,32);
                edtBlock1.Text:=copy(recDataLst.Strings[5],5,32);
                edtBlock2.Text:=copy(recDataLst.Strings[6],5,32);
                edtBlock3.Text:=copy(recDataLst.Strings[7],5,32);
                gbRWSector.Caption:=cbRWSec.Text+'读取成功';
            end;
        7 : begin
                for i:=0 to recDataLst.Count-1 do
                    if copy(recDataLst.Strings[i],3,2)<>'00' then
                    begin
                        gbRWSector.Caption:=cbRWSec.Text+'写入失败';
                        exit;
                    end;
                gbRWSector.Caption:=cbRWSec.Text+'写入成功';
            end;
        8 : begin
                for i:=0 to recDataLst.Count-1 do
                    if copy(recDataLst.Strings[i],3,2)<>'00' then
                    begin
                        MessageBox(handle,'块值初始化失败','失败',MB_OK);
                        exit;
                    end;
                MessageBox(handle,'块值初始化成功','成功',MB_OK);
            end;
        9 : begin
                for i:=0 to recDataLst.Count-1 do
                    if copy(recDataLst.Strings[i],3,2)<>'00' then
                    begin
                        MessageBox(handle,'块值读取失败','失败',MB_OK);
                        exit;
                    end;
                edtCurValue.Text:=copy(recDataLst.Strings[4],5,8);
                MessageBox(handle,'块值读取成功','成功',MB_OK);
            end;
        10 : begin
                for i:=0 to recDataLst.Count-1 do
                    if copy(recDataLst.Strings[i],3,2)<>'00' then
                    begin
                        MessageBox(handle,'块值加值失败','失败',MB_OK);
                        exit;
                    end;
                MessageBox(handle,'块值加值成功','成功',MB_OK);
            end;
        11 : begin
                for i:=0 to recDataLst.Count-1 do
                    if copy(recDataLst.Strings[i],3,2)<>'00' then
                    begin
                        MessageBox(handle,'块值减值失败','失败',MB_OK);
                        exit;
                    end;
                MessageBox(handle,'块值减值成功','成功',MB_OK);
            end;
        12 : begin
                for i:=0 to recDataLst.Count-1 do
                    if copy(recDataLst.Strings[i],3,2)<>'00' then
                    begin
                        MessageBox(handle,'密码修改失败','失败',MB_OK);
                        exit;
                    end;
                MessageBox(handle,'密码修改成功','成功',MB_OK);
            end;
    end;
end;

procedure TfrmReader.FormShow(Sender: TObject);
begin
    comReader.StartComm();
    orderLst:=TStringList.Create;
    recDataLst:=tStringList.Create;
end;

procedure TfrmReader.FormClose(Sender: TObject; var Action: TCloseAction);
begin
    orderLst.Free();
    recDataLst.Free();
    comReader.StopComm();
end;

procedure TfrmReader.comReaderReceiveData(Sender: TObject; Buffer: Pointer;
  BufferLength: Word);
var
    ii : integer;
    recStr : string;
    tmpStr : string;
begin
    recStr:='';
    SetLength(tmpStr, BufferLength);
    move(buffer^,pchar(tmpStr)^,BufferLength);
    for ii:=1 to BufferLength do
    begin
        recStr:=recStr+intTohex(ord(tmpStr[ii]),2);
    end;
    memComSeRe.Lines.Add('<<== '+recStr);
    recDataLst.Add(recStr);
    if curOrderNo<orderLst.Count then
        sendData()
    else begin
        checkOper();
        memComSeRe.Lines.Append('');
    end;
end;

procedure TfrmReader.cbRWSecChange(Sender: TObject);
begin
    gbRWSector.Caption:=cbRWSec.Text;
    edtBlock0.Text:='';
    edtBlock1.Text:='';
    edtBlock2.Text:='';
    edtBlock3.Text:='';
end;

//测试读卡器.
procedure TfrmReader.btnTestReaderClick(Sender: TObject);
begin
    orderLst.Clear();
    recDataLst.Clear();
    curOrderNo:=0;
    curOperNo:=0;
    memComSeRe.Lines.Add('测试读卡器');
    orderLst.Add('020B0F');
    sendData();
end;

procedure TfrmReader.btnSeekCardClick(Sender: TObject);
begin
    orderLst.Clear();
    recDataLst.Clear();
    curOrderNo:=0;
    curOperNo:=1;
    memComSeRe.Lines.Add('寻卡');
    orderLst.Add('020226');
    orderLst.Add('020B0F');
    sendData();
end;

procedure TfrmReader.btnCollisionClick(Sender: TObject);
begin
    orderLst.Clear();
    recDataLst.Clear();
    curOrderNo:=0;
    curOperNo:=2;
    memComSeRe.Lines.Add('防冲突');
    orderLst.Add('0103');
    orderLst.Add('020B0F');
    sendData();
end;

procedure TfrmReader.btnSelectClick(Sender: TObject);
begin
    orderLst.Clear();
    recDataLst.Clear();
    curOrderNo:=0;
    curOperNo:=3;
    memComSeRe.Lines.Add('选择');
    orderLst.Add('0104');
    orderLst.Add('020B0F');
    sendData();
end;

procedure TfrmReader.btnTerminateClick(Sender: TObject);
begin
    orderLst.Clear();
    recDataLst.Clear();
    curOrderNo:=0;
    curOperNo:=4;
    memComSeRe.Lines.Add('终止');
    orderLst.Add('0101');
    orderLst.Add('020B0F');
    sendData();
end;

procedure TfrmReader.btnPwdDwnClick(Sender: TObject);
var
    i,j : integer;
    nameStr,pwdStr,noStr,pwdOrg : string;
begin
    orderLst.Clear();
    recDataLst.Clear();
    curOrderNo:=0;
    curOperNo:=5;
    memComSeRe.Lines.Add('密码下载');

    if rgSePwdOrg.ItemIndex=0 then
        pwdOrg:='60'
    else
        pwdOrg:='61';
    for i:=0 to ComponentCount-1 do
    begin
        if components[i] is TEdit then
        begin
            nameStr:=components[i].Name;
            if copy(nameStr,1,9)='edtSecPwd' then
            begin
                noStr:=copy(nameStr,10,length(nameStr)-9);
                pwdStr:=(components[i] as TEdit).Text;
                 if pwdStr<>'' then
                 begin
                    noStr:=intTohex(strToint(noStr),2);                 
                    if length(pwdStr)<12 then
                    begin
                        MessageBox(handle,PChar(noStr+' 扇区密码位数据错误'),'错误',MB_OK);
                        exit;
                    end;
                    for j:=1 to length(pwdStr) do
                        if  not (pwdStr[j] in ['0'..'9','A'..'F']) then
                        begin
                            MessageBox(handle,PChar(noStr+' 扇区密码含有非法数据'),'错误',MB_OK);
                            exit;
                        end;
                    orderLst.Add('0906'+pwdOrg+noStr+pwdStr);
                 end;
            end;
        end;
    end;
    orderLst.Add('020B0F');
    sendData();
end;

procedure TfrmReader.btnBlockRdClick(Sender: TObject);
var
    i : integer;
    curSectorNo : string;
begin
    orderLst.Clear();
    recDataLst.Clear();
    curOrderNo:=0;
    curOperNo:=6;
    curSectorNo:=intTohex(cbRWSec.ItemIndex,2);
    memComSeRe.Lines.Add(intTostr(cbRWSec.ItemIndex)+'扇区内容读取');
    orderLst.Add('020252');
    orderLst.Add('0103');
    orderLst.Add('0104');
    orderLst.Add('040560'+curSectorNO+intTohex(cbRWSec.ItemIndex*4,2));
    for i:=0 to 3 do
        orderLst.Add('0208'+intTohex(cbRWSec.ItemIndex*4+i,2));
    orderLst.Add('020B0F');
    sendData();
end;

procedure TfrmReader.btnBlockWtClick(Sender: TObject);
var
    i : integer;
    blockCont : string;
    curSectorNo : string;
begin
    if (edtBlock0.Text<>'') or (edtBlock1.Text<>'') or (edtBlock2.Text<>'') then
    else
        exit;
    orderLst.Clear();
    recDataLst.Clear();
    curOrderNo:=0;
    curOperNo:=7;
    curSectorNo:=intTohex(cbRWSec.ItemIndex,2);
    memComSeRe.Lines.Add(intTostr(cbRWSec.ItemIndex)+'扇区内容写入');
    orderLst.Add('020252');
    orderLst.Add('0103');
    orderLst.Add('0104');
    orderLst.Add('040560'+curSectorNO+intTohex(cbRWSec.ItemIndex*4,2));

    blockCont:=edtBlock0.Text;
    if blockCont<>'' then
    begin
        if length(blockCont)<>32 then
        begin
            MessageBox(Handle,'块0数据位数错误!','错误',MB_OK);
            exit;
        end;
        for i:=1 to 32 do
            if not (blockCont[i] in ['0'..'9','A'..'F']) then
            begin
                MessageBox(Handle,'块0数据含有非法内容!','错误',MB_OK);
                exit;
            end;
        orderLst.Add('1209'+intTohex(cbRWSec.ItemIndex*4,2)+blockCont);
    end;

    blockCont:=edtBlock1.Text;
    if blockCont<>'' then
    begin
        if Length(blockCont)<>32 then
        begin
            MessageBox(Handle,'块1数据位数错误!','错误',MB_OK);
            exit;
        end;
        for i:=1 to 32 do
            if not (blockCont[i] in ['0'..'9','A'..'F']) then
            begin
                MessageBox(Handle,'块1数据含有非法内容!','错误',MB_OK);
                exit;
            end;
        orderLst.Add('1209'+intTohex(cbRWSec.ItemIndex*4+1,2)+blockCont);
    end;


    blockCont:=edtBlock2.Text;
    if blockCont<>'' then
    begin
        if Length(blockCont)<>32 then
        begin
            MessageBox(Handle,'块2数据位数错误!','错误',MB_OK);
            exit;
        end;
        for i:=1 to 32 do
            if not (blockCont[i] in ['0'..'9','A'..'F']) then
            begin
                MessageBox(Handle,'块2数据含有非法内容!','错误',MB_OK);
                exit;
        end;
        orderLst.Add('1209'+intTohex(cbRWSec.ItemIndex*4+2,2)+blockCont);
    end;

    orderLst.Add('020B0F');
    sendData();
end;

procedure TfrmReader.btnBlockInitClick(Sender: TObject);
var
    sectorNo,blockNo,i,tmpInt : integer;
    blockValue,tmpStr,iniValue : string;
begin
    orderLst.Clear();
    recDataLst.Clear();
    curOrderNo:=0;
    curOperNo:=8;
    sectorNo:=cbBSecSe.ItemIndex;
    blockNo:=cbBSe.ItemIndex;
    memComSeRe.Lines.Add(intTostr(cbRWSec.ItemIndex)+'块值初始化');

    blockValue:=edtOpeValue.Text;
    if blockValue='' then
    begin
        MessageBox(handle,'操作值不能为空!','错误',MB_OK);
        exit;
    end;
    if length(blockValue)<>8 then
    begin
        MessageBox(handle,'操作值位数错误!','错误',MB_OK);
        exit;
    end;
    for i:=1 to 8 do
        if not (blockValue[i] in ['0'..'9','A'..'F']) then
        begin
            MessageBox(Handle,'操作数据含有非法内容!','错误',MB_OK);
            exit;
        end;

    orderLst.Add('020252');
    orderLst.Add('0103');
    orderLst.Add('0104');
    orderLst.Add('040560'+intTohex(sectorNo,2)+intTohex(sectorNo*4,2));

    iniValue:='';
    for i:=0 to 3 do
    begin
        tmpStr:=copy(blockValue,(i*2+1),2);
        tmpInt:=255-strToint('$'+tmpStr);
        iniValue:=iniValue+intTohex(tmpInt,2);
    end;
    iniValue:=blockValue+iniValue+blockValue;

    tmpInt:=255-sectorNo*4+blockNo;
    tmpStr:=intTohex(sectorNo*4+blockNo,2)+intTohex(tmpInt,2);
    iniValue:=iniValue+tmpStr+tmpStr;

    orderLst.Add('1209'+intTohex(sectorNo*4+blockNo,2)+iniValue);
    orderLst.Add('020B0F');
    sendData();
end;

procedure TfrmReader.btnBlockReadClick(Sender: TObject);
begin
    orderLst.Clear();
    recDataLst.Clear();
    curOrderNo:=0;
    curOperNo:=9;
    memComSeRe.Lines.Add(intTostr(cbBse.ItemIndex+cbBSecSe.ItemIndex)+'块值读取');
    orderLst.Add('020252');
    orderLst.Add('0103');
    orderLst.Add('0104');
    orderLst.Add('040560'+intTohex(cbBSecSe.ItemIndex,2)+intTohex(cbBSecSe.ItemIndex*4,2));
    orderLst.Add('0208'+intTohex(cbBSecSe.ItemIndex*4+cbBse.ItemIndex,2));
    orderLst.Add('020B0F');
    sendData();
end;

procedure TfrmReader.btnBlockAddClick(Sender: TObject);
var
    i : integer;
    blockValue : string;
begin
    orderLst.Clear();
    recDataLst.Clear();
    curOrderNo:=0;
    curOperNo:=10;
    memComSeRe.Lines.Add(intTostr(cbBse.ItemIndex+cbBSecSe.ItemIndex)+'块值加值');

    blockValue:=edtOpeValue.Text;
    if blockValue='' then
    begin
        MessageBox(handle,'操作值不能为空!','错误',MB_OK);
        exit;
    end;
    if length(blockValue)<>8 then
    begin
        MessageBox(handle,'操作值位数错误!','错误',MB_OK);
        exit;
    end;
    for i:=1 to 8 do
        if not (blockValue[i] in ['0'..'9','A'..'F']) then
        begin
            MessageBox(Handle,'操作数据含有非法内容!','错误',MB_OK);
            exit;
        end;
    orderLst.Add('020252');
    orderLst.Add('0103');
    orderLst.Add('0104');
    orderLst.Add('040560'+intTohex(cbBSecSe.ItemIndex,2)+intTohex(cbBSecSe.ItemIndex*4,2));
    orderLst.Add('080AC1'+intTohex(cbBSecSe.ItemIndex*4+cbBse.ItemIndex,2)+blockValue+intTohex(cbBSecSe.ItemIndex*4+cbBse.ItemIndex,2));
    orderLst.Add('020B0F');
    sendData();
end;

procedure TfrmReader.btnBlockSubClick(Sender: TObject);
var
    i : integer;
    blockValue : string;
begin
    orderLst.Clear();
    recDataLst.Clear();
    curOrderNo:=0;
    curOperNo:=11;
    memComSeRe.Lines.Add(intTostr(cbBse.ItemIndex+cbBSecSe.ItemIndex)+'块值减值');

    blockValue:=edtOpeValue.Text;
    if blockValue='' then
    begin
        MessageBox(handle,'操作值不能为空!','错误',MB_OK);
        exit;
    end;
    if length(blockValue)<>8 then
    begin
        MessageBox(handle,'操作值位数错误!','错误',MB_OK);
        exit;
    end;
    for i:=1 to 8 do
        if not (blockValue[i] in ['0'..'9','A'..'F']) then
        begin
            MessageBox(Handle,'操作数据含有非法内容!','错误',MB_OK);
            exit;
        end;
    orderLst.Add('020252');
    orderLst.Add('0103');
    orderLst.Add('0104');
    orderLst.Add('040560'+intTohex(cbBSecSe.ItemIndex,2)+intTohex(cbBSecSe.ItemIndex*4,2));
    orderLst.Add('080AC0'+intTohex(cbBSecSe.ItemIndex*4+cbBse.ItemIndex,2)+blockValue+intTohex(cbBSecSe.ItemIndex*4+cbBse.ItemIndex,2));
    orderLst.Add('020B0F');
    sendData();
end;

procedure TfrmReader.btnChPwdClick(Sender: TObject);
var
    i : integer;
    pwdA,pwdB : string;
    curSectorNo,tmpStr : string;
begin
    if (edtPwdA.Text<>'') or (edtPwdB.Text<>'') then
    else
        exit;
    orderLst.Clear();
    recDataLst.Clear();
    curOrderNo:=0;
    curOperNo:=12;
    curSectorNo:=intTohex(cbChPwdSec.ItemIndex,2);
    memComSeRe.Lines.Add(intTostr(cbChPwdSec.ItemIndex)+'扇区密码写入');
    orderLst.Add('020252');
    orderLst.Add('0103');
    orderLst.Add('0104');
    orderLst.Add('040560'+curSectorNo+intTohex(cbRWSec.ItemIndex*4,2));

    pwdA:=edtPwdA.Text;
    if pwdA<>'' then
    begin
        if Length(pwdA)<>12 then
        begin
            MessageBox(Handle,'密码A位数错误!','错误',MB_OK);
            exit;
        end;
        for i:=1 to 12 do
            if not (pwdA[i] in ['0'..'9','A'..'F']) then
            begin
                MessageBox(Handle,'密码A含有非法内容!','错误',MB_OK);
                exit;
            end;
    end;

    pwdB:=edtPwdB.Text;
    if pwdB<>'' then
    begin
        if Length(pwdB)<>12 then
        begin
            MessageBox(Handle,'密码B位数错误!','错误',MB_OK);
            exit;
        end;
        for i:=1 to 12 do
            if not (pwdB[i] in ['0'..'9','A'..'F']) then
            begin
                MessageBox(Handle,'密码B含有非法内容!','错误',MB_OK);
                exit;
            end;
    end;
    if pwdB='' then
        for i:=1 to 12 do
        begin
            tmpStr:=copy(pwdA,i,1);
            tmpStr:=intTostr(15-strToint('$'+tmpStr));
            pwdB:=pwdB+tmpStr;
        end;

    orderLst.Add('1209'+intTohex(cbChPwdSec.ItemIndex*4+3,2)+pwdA+'ff078069'+pwdB);

    orderLst.Add('020B0F');
    sendData();

end;

procedure TfrmReader.btnChCommClick(Sender: TObject);
var
    tmpStr : string;
begin
    comReader.StopComm();

    tmpStr:=cbComName.Text;
    comReader.CommName:=tmpStr;
    memComSet.Lines.Add('串口为: '+tmpStr);

    tmpStr:=cbBaudRate.Text;
    comReader.BaudRate:=strToint(tmpStr);
    memComSet.Lines.Add('波特率: '+tmpStr);

    comReader.Parity:=tParity(cbParity.ItemIndex);
    memComSet.Lines.Add('检验位: '+cbParity.Text);

    comReader.ByteSize:=TByteSize(cbByteSize.ItemIndex);
    memComSet.Lines.Add('数据位: '+cbByteSize.Text);

    comReader.StopBits:=TStopBits(cbStopBits.ItemIndex);
    memComSet.Lines.Add('停止位: '+cbStopBits.Text);    

    comReader.StartComm();

    MessageBeep(10);
    memComSet.Lines.Add('串口参数设置成功');
    memComSet.Lines.Add(''); 

end;

procedure TfrmReader.memLowReDblClick(Sender: TObject);
begin
    memLowRe.Text:='';
end;

end.
