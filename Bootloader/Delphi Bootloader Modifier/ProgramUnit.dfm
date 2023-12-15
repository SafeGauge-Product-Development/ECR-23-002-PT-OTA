object Form1: TForm1
  Left = 1756
  Top = 57
  Width = 1315
  Height = 792
  BorderIcons = [biSystemMenu]
  Caption = 'MultiTool Programmer Rev 6.02'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 112
    Width = 143
    Height = 19
    Caption = 'Original Flash Space'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 440
    Top = 112
    Width = 147
    Height = 19
    Caption = 'Modified Flash Space'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 888
    Top = 112
    Width = 180
    Height = 19
    Caption = 'Modified Hex File Created'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 24
    Top = 80
    Width = 426
    Height = 19
    Caption = 'Note: The file paths may need to change in the Delphi code.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Memo4: TMemo
    Left = 888
    Top = 144
    Width = 393
    Height = 593
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object Memo2: TMemo
    Left = 24
    Top = 144
    Width = 393
    Height = 593
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 1
  end
  object ConvertFile: TButton
    Left = 24
    Top = 16
    Width = 1265
    Height = 49
    Caption = 'Convert BootLoader File'
    TabOrder = 2
    OnClick = ConvertFileClick
  end
  object Memo1: TMemo
    Left = 440
    Top = 144
    Width = 425
    Height = 593
    ScrollBars = ssVertical
    TabOrder = 3
  end
end
