object frmReader: TfrmReader
  Left = 219
  Top = 107
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'IC'#21345#25805#20316
  ClientHeight = 321
  ClientWidth = 468
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pgcReader: TPageControl
    Left = 0
    Top = 25
    Width = 468
    Height = 296
    ActivePage = tbsConfig
    Align = alTop
    MultiLine = True
    TabIndex = 0
    TabOrder = 0
    TabPosition = tpBottom
    object tbsConfig: TTabSheet
      Caption = #21442#25968#35774#32622
      ImageIndex = 1
      object gbCommSet: TGroupBox
        Left = 32
        Top = 30
        Width = 169
        Height = 209
        Caption = #20018#21475#35774#32622
        TabOrder = 0
        object lblComName: TLabel
          Left = 23
          Top = 30
          Width = 44
          Height = 13
          AutoSize = False
          Caption = #20018#21475#21495
        end
        object lblBaudRate: TLabel
          Left = 23
          Top = 65
          Width = 44
          Height = 13
          AutoSize = False
          Caption = #27874#29305#29575
        end
        object lblParity: TLabel
          Left = 23
          Top = 101
          Width = 44
          Height = 13
          AutoSize = False
          Caption = #26657#39564#20301
        end
        object lblByteSize: TLabel
          Left = 23
          Top = 136
          Width = 44
          Height = 13
          AutoSize = False
          Caption = #25968#25454#20301
        end
        object lblStopBits: TLabel
          Left = 23
          Top = 172
          Width = 44
          Height = 13
          AutoSize = False
          Caption = #20572#27490#20301
        end
        object cbComName: TComboBox
          Left = 71
          Top = 26
          Width = 75
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 0
          Text = 'COM1'
          Items.Strings = (
            'COM1'
            'COM2'
            'COM3'
            'COM4'
            'COM5'
            'COM6'
            'COM7'
            'COM8'
            'COM9')
        end
        object cbBaudRate: TComboBox
          Left = 71
          Top = 61
          Width = 75
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          ItemIndex = 3
          TabOrder = 1
          Text = '9600'
          Items.Strings = (
            '1200'
            '2400'
            '4800'
            '9600'
            '19200'
            '38400')
        end
        object cbParity: TComboBox
          Left = 71
          Top = 97
          Width = 75
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 2
          Text = 'None'
          Items.Strings = (
            'None'
            'Odd'
            'Even'
            'Mark'
            'Space')
        end
        object cbByteSize: TComboBox
          Left = 71
          Top = 132
          Width = 75
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          ItemIndex = 3
          TabOrder = 3
          Text = '_8'
          Items.Strings = (
            '_5'
            '_6'
            '_7'
            '_8')
        end
        object cbStopBits: TComboBox
          Left = 71
          Top = 168
          Width = 75
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 4
          Text = '_1'
          Items.Strings = (
            '_1'
            '_1_5'
            '_2')
        end
      end
      object btnChComm: TButton
        Left = 232
        Top = 216
        Width = 75
        Height = 25
        Caption = #20462#25913#20018#21475
        TabOrder = 1
        OnClick = btnChCommClick
      end
      object btnTestReader: TButton
        Left = 344
        Top = 216
        Width = 75
        Height = 25
        Caption = #27979#35797#35835#21345#22120
        TabOrder = 2
        OnClick = btnTestReaderClick
      end
      object memComSet: TMemo
        Left = 232
        Top = 36
        Width = 185
        Height = 169
        ScrollBars = ssVertical
        TabOrder = 3
      end
    end
    object tbsLowLevel: TTabSheet
      Caption = #20302#32423#25805#20316
      object gbLoweOper: TGroupBox
        Left = 17
        Top = 8
        Width = 185
        Height = 241
        Caption = #20302#32423#25805#20316
        TabOrder = 0
        object btnSeekCard: TButton
          Left = 55
          Top = 40
          Width = 75
          Height = 25
          Caption = #23547#21345
          TabOrder = 0
          OnClick = btnSeekCardClick
        end
        object btnCollision: TButton
          Left = 55
          Top = 88
          Width = 75
          Height = 25
          Caption = #38450#20914#31361
          TabOrder = 1
          OnClick = btnCollisionClick
        end
        object btnSelect: TButton
          Left = 55
          Top = 136
          Width = 75
          Height = 25
          Caption = #36873#25321
          TabOrder = 2
          OnClick = btnSelectClick
        end
        object btnTerminate: TButton
          Left = 55
          Top = 184
          Width = 75
          Height = 25
          Caption = #32456#27490
          TabOrder = 3
          OnClick = btnTerminateClick
        end
      end
      object gbLowRe: TGroupBox
        Left = 257
        Top = 8
        Width = 185
        Height = 241
        Caption = #20449#24687#26174#31034
        TabOrder = 1
        object memLowRe: TMemo
          Left = 12
          Top = 25
          Width = 160
          Height = 200
          TabOrder = 0
          OnDblClick = memLowReDblClick
        end
      end
    end
    object tbsPassDown: TTabSheet
      Caption = #23494#30721#19979#36733
      ImageIndex = 2
      object gbPasswordDown: TGroupBox
        Left = 8
        Top = 2
        Width = 447
        Height = 223
        Caption = #35831#24405#20837#23494#30721
        TabOrder = 0
        object lblPDSector0: TLabel
          Left = 27
          Top = 23
          Width = 35
          Height = 13
          AutoSize = False
          Caption = #25159#21306'0'
        end
        object lblPDSector1: TLabel
          Left = 27
          Top = 48
          Width = 35
          Height = 13
          AutoSize = False
          Caption = #25159#21306'1'
        end
        object lblPDSector2: TLabel
          Left = 27
          Top = 73
          Width = 35
          Height = 13
          AutoSize = False
          Caption = #25159#21306'2'
        end
        object lblPDSector3: TLabel
          Left = 27
          Top = 98
          Width = 35
          Height = 13
          AutoSize = False
          Caption = #25159#21306'3'
        end
        object lblPDSector4: TLabel
          Left = 27
          Top = 124
          Width = 35
          Height = 13
          AutoSize = False
          Caption = #25159#21306'4'
        end
        object lblPDSector5: TLabel
          Left = 27
          Top = 149
          Width = 35
          Height = 13
          AutoSize = False
          Caption = #25159#21306'5'
        end
        object lblPDSector6: TLabel
          Left = 27
          Top = 174
          Width = 35
          Height = 13
          AutoSize = False
          Caption = #25159#21306'6'
        end
        object lblPDSector7: TLabel
          Left = 27
          Top = 200
          Width = 35
          Height = 13
          AutoSize = False
          Caption = #25159#21306'7'
        end
        object lblPDSector8: TLabel
          Left = 244
          Top = 23
          Width = 35
          Height = 13
          AutoSize = False
          Caption = #25159#21306'8'
        end
        object lblPDSector9: TLabel
          Left = 245
          Top = 48
          Width = 35
          Height = 13
          AutoSize = False
          Caption = #25159#21306'9'
        end
        object lblPDSectorA: TLabel
          Left = 244
          Top = 73
          Width = 35
          Height = 13
          AutoSize = False
          Caption = #25159#21306'A'
        end
        object lblPDSectorB: TLabel
          Left = 244
          Top = 98
          Width = 35
          Height = 13
          AutoSize = False
          Caption = #25159#21306'B'
        end
        object lblPDSectorC: TLabel
          Left = 244
          Top = 124
          Width = 35
          Height = 13
          AutoSize = False
          Caption = #25159#21306'C'
        end
        object lblPDSectorD: TLabel
          Left = 244
          Top = 149
          Width = 35
          Height = 13
          AutoSize = False
          Caption = #25159#21306'D'
        end
        object lblPDSectorE: TLabel
          Left = 244
          Top = 174
          Width = 35
          Height = 13
          AutoSize = False
          Caption = #25159#21306'E'
        end
        object lblPDSectorF: TLabel
          Left = 244
          Top = 200
          Width = 35
          Height = 13
          AutoSize = False
          Caption = #25159#21306'F'
        end
        object edtSecPwd0: TEdit
          Left = 66
          Top = 19
          Width = 138
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 0
        end
        object edtSecPwd1: TEdit
          Left = 66
          Top = 44
          Width = 138
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 1
        end
        object edtSecPwd2: TEdit
          Left = 66
          Top = 69
          Width = 138
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 2
        end
        object edtSecPwd3: TEdit
          Left = 66
          Top = 94
          Width = 138
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 3
        end
        object edtSecPwd4: TEdit
          Left = 66
          Top = 120
          Width = 138
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 4
        end
        object edtSecPwd5: TEdit
          Left = 66
          Top = 145
          Width = 138
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 5
        end
        object edtSecPwd6: TEdit
          Left = 66
          Top = 170
          Width = 138
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 6
        end
        object edtSecPwd7: TEdit
          Left = 66
          Top = 196
          Width = 138
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 7
        end
        object edtSecPwd8: TEdit
          Left = 283
          Top = 19
          Width = 138
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 8
        end
        object edtSecPwd9: TEdit
          Left = 283
          Top = 44
          Width = 138
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 9
        end
        object edtSecPwd10: TEdit
          Left = 283
          Top = 69
          Width = 138
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 10
        end
        object edtSecPwd11: TEdit
          Left = 283
          Top = 94
          Width = 138
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 11
        end
        object edtSecPwd12: TEdit
          Left = 283
          Top = 120
          Width = 138
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 12
        end
        object edtSecPwd13: TEdit
          Left = 283
          Top = 145
          Width = 138
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 13
        end
        object edtSecPwd14: TEdit
          Left = 283
          Top = 170
          Width = 138
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 14
        end
        object edtSecPwd15: TEdit
          Left = 283
          Top = 195
          Width = 138
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 15
        end
      end
      object rgSePwdOrg: TRadioGroup
        Left = 72
        Top = 225
        Width = 185
        Height = 32
        Caption = #35831#36873#25321#19979#36733#23494#30721#32452
        Columns = 2
        ItemIndex = 0
        Items.Strings = (
          'A'#32452#23494#30721
          'B'#32452#23494#30721)
        TabOrder = 1
      end
      object btnPwdDwn: TButton
        Left = 312
        Top = 232
        Width = 75
        Height = 25
        Caption = #19979#36733
        TabOrder = 2
        OnClick = btnPwdDwnClick
      end
    end
    object tbsDataOper: TTabSheet
      Caption = #25968#25454#35835#20889
      ImageIndex = 3
      object gbRWSector: TGroupBox
        Left = 13
        Top = 42
        Width = 289
        Height = 169
        Caption = #31532'0'#25159#21306
        TabOrder = 0
        object lblBlock0: TLabel
          Left = 24
          Top = 30
          Width = 20
          Height = 13
          AutoSize = False
          Caption = #22359'0'
        end
        object lblBlock1: TLabel
          Left = 24
          Top = 64
          Width = 20
          Height = 13
          AutoSize = False
          Caption = #22359'1'
        end
        object lblBlock2: TLabel
          Left = 24
          Top = 99
          Width = 20
          Height = 13
          AutoSize = False
          Caption = #22359'2'
        end
        object lblBlock3: TLabel
          Left = 24
          Top = 134
          Width = 20
          Height = 13
          AutoSize = False
          Caption = #22359'3'
        end
        object edtBlock0: TEdit
          Left = 57
          Top = 26
          Width = 207
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 32
          TabOrder = 0
        end
        object edtBlock1: TEdit
          Left = 57
          Top = 60
          Width = 207
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 32
          TabOrder = 1
        end
        object edtBlock2: TEdit
          Left = 57
          Top = 95
          Width = 207
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 32
          TabOrder = 2
        end
        object edtBlock3: TEdit
          Left = 57
          Top = 130
          Width = 207
          Height = 21
          CharCase = ecUpperCase
          ReadOnly = True
          TabOrder = 3
        end
      end
      object gbRWSeSec: TGroupBox
        Left = 319
        Top = 42
        Width = 129
        Height = 71
        Caption = #35831#36873#25321#25159#21306#21495
        TabOrder = 1
        object cbRWSec: TComboBox
          Left = 13
          Top = 29
          Width = 103
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 0
          Text = #31532'0'#25159#21306
          OnChange = cbRWSecChange
          Items.Strings = (
            #31532'0'#25159#21306
            #31532'1'#25159#21306
            #31532'2'#25159#21306
            #31532'3'#25159#21306
            #31532'4'#25159#21306
            #31532'5'#25159#21306
            #31532'6'#25159#21306
            #31532'7'#25159#21306
            #31532'8'#25159#21306
            #31532'9'#25159#21306
            #31532'10'#25159#21306
            #31532'11'#25159#21306
            #31532'12'#25159#21306
            #31532'13'#25159#21306
            #31532'14'#25159#21306
            #31532'15'#25159#21306)
        end
      end
      object btnBlockRd: TButton
        Left = 346
        Top = 145
        Width = 75
        Height = 25
        Caption = #35835#20986
        TabOrder = 2
        OnClick = btnBlockRdClick
      end
      object btnBlockWt: TButton
        Left = 346
        Top = 185
        Width = 75
        Height = 25
        Caption = #20889#20837
        TabOrder = 3
        OnClick = btnBlockWtClick
      end
    end
    object tbsBlockOper: TTabSheet
      Caption = #22359#20540#25805#20316
      ImageIndex = 4
      object gbReOrWt: TGroupBox
        Left = 25
        Top = 46
        Width = 257
        Height = 121
        Caption = #35835#20889#20869#23481
        TabOrder = 0
        object lblCurValue: TLabel
          Left = 20
          Top = 34
          Width = 36
          Height = 13
          Caption = #24403#21069#20540
        end
        object lblOpeValue: TLabel
          Left = 20
          Top = 74
          Width = 36
          Height = 13
          Caption = #25805#20316#20540
        end
        object edtCurValue: TEdit
          Left = 76
          Top = 30
          Width = 161
          Height = 21
          TabOrder = 0
        end
        object edtOpeValue: TEdit
          Left = 76
          Top = 70
          Width = 161
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 8
          TabOrder = 1
        end
      end
      object gbBkSec: TGroupBox
        Left = 305
        Top = 46
        Width = 130
        Height = 50
        Caption = #25159#21306#21495
        TabOrder = 1
        object cbBSecSe: TComboBox
          Left = 12
          Top = 19
          Width = 106
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 0
          Text = #31532'0'#25159#21306
          Items.Strings = (
            #31532'0'#25159#21306
            #31532'1'#25159#21306
            #31532'2'#25159#21306
            #31532'3'#25159#21306
            #31532'4'#25159#21306
            #31532'5'#25159#21306
            #31532'6'#25159#21306
            #31532'7'#25159#21306
            #31532'8'#25159#21306
            #31532'9'#25159#21306
            #31532'10'#25159#21306
            #31532'11'#25159#21306
            #31532'12'#25159#21306
            #31532'13'#25159#21306
            #31532'14'#25159#21306
            #31532'15'#25159#21306)
        end
      end
      object gbBlokSe: TGroupBox
        Left = 305
        Top = 117
        Width = 130
        Height = 50
        Caption = #22359#21495
        TabOrder = 2
        object cbBSe: TComboBox
          Left = 12
          Top = 19
          Width = 106
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          ItemIndex = 1
          TabOrder = 0
          Text = #31532'1 '#22359
          Items.Strings = (
            #31532'0 '#22359
            #31532'1 '#22359
            #31532'2 '#22359)
        end
      end
      object btnBlockInit: TButton
        Left = 49
        Top = 198
        Width = 75
        Height = 25
        Caption = #21021#22987#21270
        TabOrder = 3
        OnClick = btnBlockInitClick
      end
      object btnBlockRead: TButton
        Left = 145
        Top = 198
        Width = 75
        Height = 25
        Caption = #35835#21462
        TabOrder = 4
        OnClick = btnBlockReadClick
      end
      object btnBlockAdd: TButton
        Left = 241
        Top = 198
        Width = 75
        Height = 25
        Caption = #21152#20540
        TabOrder = 5
        OnClick = btnBlockAddClick
      end
      object btnBlockSub: TButton
        Left = 337
        Top = 198
        Width = 75
        Height = 25
        Caption = #20943#20540
        TabOrder = 6
        OnClick = btnBlockSubClick
      end
    end
    object tbsPassCh: TTabSheet
      Caption = #20462#25913#23494#30721
      ImageIndex = 5
      object gbEntryPwd: TGroupBox
        Left = 29
        Top = 18
        Width = 217
        Height = 81
        Caption = #35831#36755#20837#23494#30721
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object lblPwdA: TLabel
          Left = 20
          Top = 24
          Width = 31
          Height = 13
          Caption = #23494#30721'A'
        end
        object lblPwdB: TLabel
          Left = 20
          Top = 53
          Width = 31
          Height = 13
          Caption = #23494#30721'B'
        end
        object edtPwdA: TEdit
          Left = 60
          Top = 20
          Width = 137
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 0
        end
        object edtPwdB: TEdit
          Left = 60
          Top = 49
          Width = 137
          Height = 21
          CharCase = ecUpperCase
          MaxLength = 12
          TabOrder = 1
        end
      end
      object gbConBit: TGroupBox
        Left = 29
        Top = 114
        Width = 217
        Height = 135
        Caption = #35831#36755#20837#25511#20214#20301
        TabOrder = 1
        object lblConBit0: TLabel
          Left = 20
          Top = 27
          Width = 30
          Height = 13
          Caption = #31532'0'#22359
        end
        object lblConBit1: TLabel
          Left = 20
          Top = 54
          Width = 30
          Height = 13
          Caption = #31532'1'#22359
        end
        object lblConBit2: TLabel
          Left = 20
          Top = 81
          Width = 30
          Height = 13
          Caption = #31532'2'#22359
        end
        object lblConBit3: TLabel
          Left = 20
          Top = 109
          Width = 30
          Height = 13
          Caption = #31532'3'#22359
        end
        object edtConBit0: TEdit
          Left = 60
          Top = 23
          Width = 137
          Height = 21
          TabOrder = 0
        end
        object edtConBit1: TEdit
          Left = 60
          Top = 50
          Width = 137
          Height = 21
          TabOrder = 1
        end
        object edtConBit2: TEdit
          Left = 60
          Top = 77
          Width = 137
          Height = 21
          TabOrder = 2
        end
        object edtConBit3: TEdit
          Left = 60
          Top = 105
          Width = 137
          Height = 21
          TabOrder = 3
        end
      end
      object gbChPwdSec: TGroupBox
        Left = 300
        Top = 18
        Width = 130
        Height = 50
        Caption = #25159#21306#21495
        TabOrder = 2
        object cbChPwdSec: TComboBox
          Left = 12
          Top = 19
          Width = 106
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 0
          Text = #31532'0'#25159#21306
          Items.Strings = (
            #31532'0'#25159#21306
            #31532'1'#25159#21306
            #31532'2'#25159#21306
            #31532'3'#25159#21306
            #31532'4'#25159#21306
            #31532'5'#25159#21306
            #31532'6'#25159#21306
            #31532'7'#25159#21306
            #31532'8'#25159#21306
            #31532'9'#25159#21306
            #31532'10'#25159#21306
            #31532'11'#25159#21306
            #31532'12'#25159#21306
            #31532'13'#25159#21306
            #31532'14'#25159#21306
            #31532'15'#25159#21306)
        end
      end
      object btnChPwd: TButton
        Left = 321
        Top = 144
        Width = 89
        Height = 25
        Caption = #20462#25913#23494#30721
        TabOrder = 3
        OnClick = btnChPwdClick
      end
      object btnChCon: TButton
        Left = 321
        Top = 201
        Width = 89
        Height = 25
        Caption = #20462#25913#25511#21046#20301
        Enabled = False
        TabOrder = 4
      end
    end
    object tbsSeRe: TTabSheet
      Caption = #21457#36865#25509#25910
      ImageIndex = 6
      object gbComSendRec: TGroupBox
        Left = 14
        Top = 11
        Width = 432
        Height = 241
        Caption = #20018#21475#20449#24687#21457#36865#19982#25509#25910
        TabOrder = 0
        object lblExplain: TLabel
          Left = 147
          Top = 16
          Width = 162
          Height = 13
          Caption = '==>>'#21040#19979#20301#26426'      <<=='#21040#19978#20301#26426
        end
        object memComSeRe: TMemo
          Left = 8
          Top = 40
          Width = 417
          Height = 193
          ScrollBars = ssVertical
          TabOrder = 0
        end
      end
    end
  end
  object pnlTitle: TPanel
    Left = 0
    Top = 0
    Width = 468
    Height = 25
    Align = alTop
    Caption = 'IC'#21345#35835#20889#25805#20316#31243#24207
    TabOrder = 1
  end
  object comReader: TComm
    CommName = 'COM1'
    BaudRate = 9600
    ParityCheck = False
    Outx_CtsFlow = False
    Outx_DsrFlow = False
    DtrControl = DtrEnable
    DsrSensitivity = False
    TxContinueOnXoff = False
    Outx_XonXoffFlow = False
    Inx_XonXoffFlow = False
    ReplaceWhenParityError = False
    IgnoreNullChar = False
    RtsControl = RtsDisable
    XonLimit = 500
    XoffLimit = 500
    ByteSize = _8
    Parity = None
    StopBits = _1
    XonChar = #17
    XoffChar = #19
    ReplacedChar = #0
    ReadIntervalTimeout = 100
    ReadTotalTimeoutMultiplier = 0
    ReadTotalTimeoutConstant = 0
    WriteTotalTimeoutMultiplier = 0
    WriteTotalTimeoutConstant = 0
    OnReceiveData = comReaderReceiveData
    Left = 1
    Top = 25
  end
end
