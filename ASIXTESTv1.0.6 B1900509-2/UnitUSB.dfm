object Form1: TForm1
  Left = 161
  Top = 127
  ClientHeight = 315
  ClientWidth = 767
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clBlue
  Font.Height = -16
  Font.Name = #32048#26126#39636
  Font.Style = []
  Icon.Data = {
    0000010001002020100000000000E80200001600000028000000200000004000
    0000010004000000000000020000000000000000000000000000000000000000
    000000008000008000000080800080000000800080008080000080808000C0C0
    C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFF
    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF99999FFFFFFFFFFFFFFFFFFFFFFFF
    FFF99999FFFFFFFFFFFFFFFFFFFFFFFFFFF99999999FFFFFFFFFFFFFFFFFFFFF
    FFF99999F999FFFFFFFFFFFFFFFFFFFFFFF99999FFF99FFFFFFFFFFFFFFFFFFF
    FFFFFFFFFFFF99FFCCCCCFFFFFFFFFFF9FFFFFFFFFFFF99FCCCCCFFFFFFFFFF9
    9FFFFFFFFFFFFCCCCCCCCFF99FFFFF999FFFFFFFFFFFCCC9CCCCCF9999FF9999
    99999999999CC999CCCCC999999FF9999999999999CC999999999999999FFF99
    9FFFFFFFFCCFFFFFFF99F999999FFFF99FFFFFFFCCFFFFFFFF9FFF9999FFFFCC
    9CFFFFFCCFFFFFFFF99FFFFC9CFFFCCCCCCCCCCCCCCCCCCC99CCCCCCCCCCFCCC
    CCCCC999CCCCCCC99CCCCCFCCCCFFCCCCCCF99999FFFFF99FFFFFFFCCCFFFFCC
    CCFF99999999999FFFFFFFFCCFFFFFFCFFFF99999F9999FFFFFFFFFCFFFFFFFF
    FFFFF999FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCCFFFFFFFCCCFFFFFFFFFFFFF
    FFFFFFFFCCFFFFFCCCCCFFFFFFFFFFFFFFFFFFFFFCCCCCCCCCCCFFFFFFFFFFFF
    FFFFFFFFFFCCCCFCCCCCFFFFFFFFFFFFFFFFFFFFFFFFFFFFCCCFFFFFFFFFFFFF
    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    000000000000000000000000000000000000000000000000000000000000}
  OldCreateOrder = False
  Position = poDefault
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object GroupBox1: TGroupBox
    Left = 0
    Top = 49
    Width = 793
    Height = 465
    Caption = 'USB Port List'
    TabOrder = 0
    object Splitter1: TSplitter
      Left = 409
      Top = 18
      Width = 5
      Height = 445
    end
    object TreeView1: TTreeView
      Left = 2
      Top = 18
      Width = 407
      Height = 445
      Align = alLeft
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Indent = 19
      ParentFont = False
      TabOrder = 0
      OnClick = TreeView1Click
    end
    object ListBox1: TListBox
      Left = 414
      Top = 18
      Width = 226
      Height = 445
      Align = alClient
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ItemHeight = 20
      ParentFont = False
      TabOrder = 1
    end
    object Memo1: TMemo
      Left = 640
      Top = 18
      Width = 151
      Height = 445
      Align = alRight
      Lines.Strings = (
        'Memo1')
      TabOrder = 2
    end
  end
  object cmdGetUSB: TButton
    Left = 3
    Top = 0
    Width = 97
    Height = 41
    Caption = 'Get USB'
    TabOrder = 1
    OnClick = cmdGetUSBClick
  end
  object cmdSave: TButton
    Left = 103
    Top = 0
    Width = 97
    Height = 41
    Caption = 'Save Text'
    TabOrder = 2
    OnClick = cmdSaveClick
  end
  object clear: TButton
    Left = 534
    Top = 18
    Width = 75
    Height = 25
    Caption = 'clear'
    TabOrder = 3
    OnClick = clearClick
  end
  object GetDeviceInfo: TButton
    Left = 385
    Top = 18
    Width = 146
    Height = 25
    Caption = 'GetDeviceInfo'
    TabOrder = 4
    OnClick = GetDeviceInfoClick
  end
  object GetAmount: TButton
    Left = 615
    Top = 18
    Width = 96
    Height = 25
    Caption = 'GetAmount'
    TabOrder = 5
    OnClick = GetAmountClick
  end
  object Result: TButton
    Left = 224
    Top = 18
    Width = 75
    Height = 25
    Caption = 'Result'
    TabOrder = 6
  end
  object SaveDialog1: TSaveDialog
    Left = 312
    Top = 8
  end
end
