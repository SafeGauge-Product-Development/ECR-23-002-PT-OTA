object Form1: TForm1
  Left = 197
  Top = 171
  Width = 267
  Height = 706
  BorderIcons = [biSystemMenu]
  Caption = 'Secure Xcode Translator'
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
    Top = 48
    Width = 17
    Height = 17
  end
  object ProgramButton: TButton
    Left = 16
    Top = 80
    Width = 225
    Height = 33
    Caption = 'PROGRAM'
    Enabled = False
    TabOrder = 0
    OnClick = ProgramButtonClick
  end
  object Memo2: TMemo
    Left = 16
    Top = 128
    Width = 225
    Height = 537
    ScrollBars = ssVertical
    TabOrder = 1
  end
  object StaticText1: TStaticText
    Left = 40
    Top = 16
    Width = 38
    Height = 17
    Caption = 'SG Link'
    TabOrder = 2
  end
  object StaticText2: TStaticText
    Left = 40
    Top = 48
    Width = 38
    Height = 17
    Caption = 'Module'
    TabOrder = 3
  end
  object Timer1: TTimer
    Interval = 50
    OnTimer = Timer1Timer
    Left = 152
    Top = 32
  end
  object VaComm1: TVaComm
    FlowControl.OutCtsFlow = False
    FlowControl.OutDsrFlow = False
    FlowControl.ControlDtr = dtrDisabled
    FlowControl.ControlRts = rtsDisabled
    FlowControl.XonXoffOut = False
    FlowControl.XonXoffIn = False
    FlowControl.DsrSensitivity = False
    FlowControl.TxContinueOnXoff = False
    DeviceName = 'COM%d'
    Buffers.WriteSize = 8192
    SettingsStore.RegRoot = rrCURRENTUSER
    SettingsStore.Location = slINIFile
    OnRxChar = VaComm1RxChar
    Version = '2.0.3.0'
    Left = 192
    Top = 32
  end
end
