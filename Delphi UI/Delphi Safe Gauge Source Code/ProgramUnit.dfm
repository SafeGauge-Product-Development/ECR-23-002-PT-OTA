object Form1: TForm1
  Left = 601
  Top = 139
  Width = 280
  Height = 729
  BorderIcons = [biSystemMenu]
  Caption = 'Safe Gauge Bootloader V1.2'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object linkStatusLed: TShape
    Left = 16
    Top = 16
    Width = 17
    Height = 17
  end
  object blueToothStatusLed: TShape
    Left = 16
    Top = 40
    Width = 17
    Height = 17
  end
  object IdOSLed: TShape
    Left = 16
    Top = 328
    Width = 17
    Height = 17
    Brush.Color = clSilver
  end
  object Label1: TLabel
    Left = 40
    Top = 328
    Width = 57
    Height = 13
    Caption = 'OS Revision'
  end
  object MULTITOOL_led: TShape
    Left = 16
    Top = 176
    Width = 17
    Height = 17
    Brush.Color = clSilver
  end
  object PT20_led: TShape
    Left = 16
    Top = 200
    Width = 17
    Height = 17
    Brush.Color = clSilver
  end
  object PT25_led: TShape
    Left = 16
    Top = 224
    Width = 17
    Height = 17
    Brush.Color = clSilver
  end
  object PT100_led: TShape
    Left = 16
    Top = 248
    Width = 17
    Height = 17
    Brush.Color = clSilver
  end
  object PT300_led: TShape
    Left = 16
    Top = 272
    Width = 17
    Height = 17
    Brush.Color = clSilver
  end
  object PT600_led: TShape
    Left = 16
    Top = 296
    Width = 17
    Height = 17
    Brush.Color = clSilver
  end
  object DI10_led: TShape
    Left = 96
    Top = 176
    Width = 17
    Height = 17
    Brush.Color = clSilver
  end
  object TM1000_led: TShape
    Left = 96
    Top = 200
    Width = 17
    Height = 17
    Brush.Color = clSilver
  end
  object Label2: TLabel
    Left = 40
    Top = 176
    Width = 42
    Height = 13
    Caption = 'MultiTool'
  end
  object Label3: TLabel
    Left = 40
    Top = 296
    Width = 30
    Height = 13
    Caption = 'PT600'
  end
  object Label4: TLabel
    Left = 40
    Top = 224
    Width = 24
    Height = 13
    Caption = 'PT25'
  end
  object Label5: TLabel
    Left = 40
    Top = 248
    Width = 30
    Height = 13
    Caption = 'PT100'
  end
  object Label6: TLabel
    Left = 40
    Top = 272
    Width = 30
    Height = 13
    Caption = 'PT300'
  end
  object Label9: TLabel
    Left = 40
    Top = 200
    Width = 24
    Height = 13
    Caption = 'PT20'
  end
  object Label10: TLabel
    Left = 120
    Top = 176
    Width = 23
    Height = 13
    Caption = 'DI10'
  end
  object Label11: TLabel
    Left = 120
    Top = 200
    Width = 38
    Height = 13
    Caption = 'TM1000'
  end
  object TS1000_led: TShape
    Left = 96
    Top = 224
    Width = 17
    Height = 17
    Brush.Color = clSilver
  end
  object LD30_led: TShape
    Left = 96
    Top = 248
    Width = 17
    Height = 17
    Brush.Color = clSilver
  end
  object UNKNOWN_led: TShape
    Left = 96
    Top = 272
    Width = 17
    Height = 17
    Brush.Color = clSilver
  end
  object Label8: TLabel
    Left = 120
    Top = 224
    Width = 36
    Height = 13
    Caption = 'TS1000'
  end
  object Label12: TLabel
    Left = 120
    Top = 248
    Width = 24
    Height = 13
    Caption = 'LD30'
  end
  object Label7: TLabel
    Left = 120
    Top = 272
    Width = 52
    Height = 13
    Caption = 'UNKNOWN'
  end
  object cpuDisplay: TLabel
    Left = 184
    Top = 328
    Width = 32
    Height = 13
    Caption = '--------'
  end
  object CPU: TLabel
    Left = 152
    Top = 328
    Width = 24
    Height = 13
    Caption = 'CPU:'
  end
  object Label13: TLabel
    Left = 152
    Top = 304
    Width = 72
    Height = 13
    Caption = 'Code Revision:'
  end
  object CodeRevision: TLabel
    Left = 232
    Top = 304
    Width = 8
    Height = 13
    Caption = '--'
  end
  object Label14: TLabel
    Left = 112
    Top = 16
    Width = 131
    Height = 13
    Caption = 'Set Dongle USB Port to 127'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object ProgramButton: TButton
    Left = 16
    Top = 392
    Width = 241
    Height = 33
    Caption = 'Program Device'
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = ProgramButtonClick
  end
  object Memo: TMemo
    Left = 16
    Top = 440
    Width = 241
    Height = 241
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 1
  end
  object StaticText1: TStaticText
    Left = 40
    Top = 16
    Width = 45
    Height = 17
    Caption = 'DataLink'
    TabOrder = 2
  end
  object StaticText2: TStaticText
    Left = 40
    Top = 40
    Width = 38
    Height = 17
    Caption = 'Module'
    TabOrder = 3
  end
  object RadioButton1: TRadioButton
    Left = 16
    Top = 72
    Width = 65
    Height = 25
    Caption = 'Multitool'
    Checked = True
    TabOrder = 4
    TabStop = True
    OnClick = RadioButton1Click
  end
  object RadioButton2: TRadioButton
    Left = 16
    Top = 96
    Width = 89
    Height = 25
    Caption = 'SG Sensor Id'
    TabOrder = 5
    OnClick = RadioButton2Click
  end
  object SensorIdString: TEdit
    Left = 128
    Top = 96
    Width = 89
    Height = 25
    BevelEdges = []
    BevelInner = bvNone
    BevelOuter = bvNone
    BorderStyle = bsNone
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 6
    Text = '666666'
    Visible = False
  end
  object UpLoadData: TButton
    Left = 16
    Top = 128
    Width = 113
    Height = 33
    Caption = 'Press to Connect'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    OnClick = UpLoadDataClick
  end
  object FileSelectCombo: TComboBox
    Left = 16
    Top = 360
    Width = 241
    Height = 21
    Enabled = False
    ItemHeight = 13
    TabOrder = 8
    Text = 'No File Selected'
    OnChange = FileSelectComboChange
  end
  object Timer1: TTimer
    Interval = 500
    OnTimer = Timer1Timer
    Left = 120
    Top = 32
  end
  object VaComm1: TVaComm
    Baudrate = br4800
    FlowControl.OutCtsFlow = False
    FlowControl.OutDsrFlow = False
    FlowControl.ControlDtr = dtrDisabled
    FlowControl.ControlRts = rtsDisabled
    FlowControl.XonXoffOut = False
    FlowControl.XonXoffIn = False
    FlowControl.DsrSensitivity = False
    FlowControl.TxContinueOnXoff = False
    DeviceName = 'COM%d'
    Buffers.ReadSize = 10000
    Buffers.WriteSize = 10000
    Buffers.ReadTimeout = 10000
    Buffers.WriteTimeout = 10000
    SettingsStore.RegRoot = rrCURRENTUSER
    SettingsStore.Location = slINIFile
    OnRxChar = VaComm1RxChar
    Version = '2.1.0.1'
    Left = 224
    Top = 64
  end
  object DataSendTimer: TTimer
    Enabled = False
    Interval = 100
    Left = 88
    Top = 32
  end
  object OpenPictureDialog1: TOpenPictureDialog
    Left = 224
    Top = 32
  end
  object BlueTimer: TTimer
    Interval = 250
    OnTimer = BlueTimerTimer
    Left = 152
    Top = 32
  end
  object ResetDongleTimer: TTimer
    Enabled = False
    OnTimer = ResetDongleTimerTimer
    Left = 184
    Top = 32
  end
end
