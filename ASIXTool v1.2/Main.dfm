object frmMain: TfrmMain
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'ASIXTool v1.2 (ActionStar)'
  ClientHeight = 223
  ClientWidth = 762
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 762
    Height = 225
    Align = alTop
    TabOrder = 0
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 610
      Height = 223
      Align = alLeft
      Color = clWhite
      ParentBackground = False
      TabOrder = 0
      object Panel3: TPanel
        Left = 1
        Top = 181
        Width = 608
        Height = 41
        Align = alBottom
        TabOrder = 0
        object ProgressBar: TProgressBar
          Left = 1
          Top = 1
          Width = 606
          Height = 39
          Align = alClient
          MarqueeInterval = 5
          TabOrder = 0
        end
      end
      object Panel4: TPanel
        Left = 1
        Top = 1
        Width = 80
        Height = 180
        Align = alLeft
        BevelOuter = bvNone
        Caption = 'Panel3'
        TabOrder = 1
        object btn_start: TButton
          Left = 0
          Top = 0
          Width = 80
          Height = 127
          Align = alTop
          Caption = #38283#22987
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -29
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          WordWrap = True
          OnClick = btn_startClick
        end
        object plSet: TPanel
          Left = 0
          Top = 127
          Width = 80
          Height = 53
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 1
          object btn_set: TButton
            Left = 0
            Top = 2
            Width = 79
            Height = 50
            Caption = #35373#23450
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -27
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            OnClick = btn_setClick
          end
        end
      end
      object Panel6: TPanel
        Left = 81
        Top = 1
        Width = 155
        Height = 180
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 2
        object GroupBox2: TGroupBox
          Left = 5
          Top = 125
          Width = 146
          Height = 53
          Caption = #28204#35430#26178#38291
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          object Panel10: TPanel
            Left = 2
            Top = 25
            Width = 142
            Height = 26
            Align = alClient
            BevelOuter = bvNone
            TabOrder = 0
            object Label2: TLabel
              Left = 116
              Top = 0
              Width = 26
              Height = 26
              Align = alRight
              Caption = 'sec'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -19
              Font.Name = 'Times New Roman'
              Font.Style = [fsBold]
              ParentFont = False
              ExplicitHeight = 22
            end
            object plTime: TPanel
              Left = 0
              Top = 0
              Width = 116
              Height = 26
              Align = alClient
              BevelOuter = bvNone
              TabOrder = 0
            end
          end
        end
        object Panel_mac: TPanel
          Left = 1
          Top = 2
          Width = 150
          Height = 55
          BevelKind = bkSoft
          BevelOuter = bvLowered
          BevelWidth = 5
          BiDiMode = bdRightToLeftNoAlign
          Caption = #29138#20837
          Color = clWhite
          Ctl3D = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -29
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentBiDiMode = False
          ParentBackground = False
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 1
          OnDblClick = Panel_macDblClick
        end
        object Panel_lan: TPanel
          Left = 1
          Top = -2
          Width = 150
          Height = 114
          BevelKind = bkSoft
          BevelOuter = bvLowered
          BevelWidth = 5
          BiDiMode = bdRightToLeftNoAlign
          Caption = #32178#36335#28204#35430
          Color = clWhite
          Ctl3D = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -29
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentBiDiMode = False
          ParentBackground = False
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 2
          OnDblClick = Panel_macDblClick
        end
      end
      object Panel5: TPanel
        Left = 514
        Top = 1
        Width = 95
        Height = 180
        Align = alRight
        Caption = 'Panel5'
        TabOrder = 3
        object pan_10: TPanel
          Left = 1
          Top = 1
          Width = 93
          Height = 60
          Align = alTop
          Caption = '10 M'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
        end
        object pan_100: TPanel
          Left = 1
          Top = 61
          Width = 93
          Height = 60
          Align = alTop
          Caption = '100 M'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 1
        end
        object pan_1000: TPanel
          Left = 1
          Top = 121
          Width = 93
          Height = 60
          Align = alTop
          Caption = '1000 M'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 2
        end
      end
      object Panel8: TPanel
        Left = 236
        Top = 1
        Width = 279
        Height = 180
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 4
        object lab_devicename: TLabel
          Left = 15
          Top = 40
          Width = 5
          Height = 22
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Panel9: TPanel
          Left = 0
          Top = 0
          Width = 279
          Height = 32
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 2
          object Label1: TLabel
            Left = 2
            Top = 10
            Width = 89
            Height = 22
            Caption = #29986#21697#21517#31281':'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -19
            Font.Name = 'Times New Roman'
            Font.Style = [fsBold]
            ParentFont = False
          end
        end
        object current_mac_group: TGroupBox
          Left = 0
          Top = 64
          Width = 279
          Height = 82
          Align = alBottom
          Caption = #30070#21069#35373#20633#34399
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -27
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          object current_mac: TLabel
            Left = 9
            Top = 35
            Width = 140
            Height = 33
            Caption = #26410#25554#20837#35373#20633
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBackground
            Font.Height = -27
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
        end
        object GroupBox1: TGroupBox
          Left = 0
          Top = 146
          Width = 279
          Height = 0
          Align = alBottom
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -27
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
          object new_mac: TLabel
            Left = 10
            Top = 37
            Width = 60
            Height = 33
            Caption = 'MAC'
          end
        end
        object plBarcodeVerifty: TPanel
          Left = 0
          Top = 146
          Width = 279
          Height = 34
          Align = alBottom
          Caption = 'MAC'#27604#23565' 00249B000000'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 3
        end
      end
    end
    object plResult: TPanel
      Left = 611
      Top = 1
      Width = 150
      Height = 223
      Align = alLeft
      BevelKind = bkSoft
      BevelOuter = bvLowered
      BevelWidth = 5
      BiDiMode = bdRightToLeftNoAlign
      Caption = 'Wait'
      Color = clCream
      Ctl3D = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -29
      Font.Name = 'Times New Roman'
      Font.Style = [fsBold]
      ParentBiDiMode = False
      ParentBackground = False
      ParentCtl3D = False
      ParentFont = False
      PopupMenu = PopupMenu1
      TabOrder = 1
    end
    object plDebug: TPanel
      Left = 761
      Top = 1
      Width = 0
      Height = 223
      Align = alClient
      Caption = 'plDebug'
      TabOrder = 2
      object moDebug: TMemo
        Left = 1
        Top = 1
        Width = 6
        Height = 171
        Align = alClient
        ScrollBars = ssVertical
        TabOrder = 0
      end
      object btnLoadMAC: TBitBtn
        Left = 1
        Top = 197
        Width = 0
        Height = 25
        Align = alBottom
        Caption = #36617#20837#27604#23565'MAC'
        DoubleBuffered = True
        ParentDoubleBuffered = False
        TabOrder = 1
        OnClick = btnLoadMACClick
      end
      object btnLoadNetworkCardName: TBitBtn
        Left = 1
        Top = 172
        Width = 0
        Height = 25
        Align = alBottom
        Caption = #36617#20837#27604#23565#32178#21345#21517#31281
        DoubleBuffered = True
        ParentDoubleBuffered = False
        TabOrder = 2
        OnClick = btnLoadNetworkCardNameClick
      end
      object Panel25: TPanel
        Left = -93
        Top = 1
        Width = 92
        Height = 171
        Align = alRight
        Color = clBtnHighlight
        ParentBackground = False
        TabOrder = 3
        object edtSetEmployeeID: TPanel
          Left = 1
          Top = 133
          Width = 90
          Height = 44
          Align = alTop
          TabOrder = 0
        end
        object edtSetModel: TPanel
          Left = 1
          Top = 89
          Width = 90
          Height = 44
          Align = alTop
          TabOrder = 1
        end
        object edtWorkStation: TPanel
          Left = 1
          Top = 45
          Width = 90
          Height = 44
          Align = alTop
          TabOrder = 2
        end
        object edtSetWorkOrderNumber: TPanel
          Left = 1
          Top = 1
          Width = 90
          Height = 44
          Align = alTop
          TabOrder = 3
        end
      end
    end
  end
  object plErrMsg: TPanel
    Left = 0
    Top = 225
    Width = 762
    Height = 41
    Align = alTop
    Caption = 'Test'
    Color = clCream
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -27
    Font.Name = #27161#26999#39636
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 1
  end
  object plSetting: TPanel
    Left = -3
    Top = 0
    Width = 612
    Height = 221
    Color = clSkyBlue
    ParentBackground = False
    TabOrder = 2
    object Panel7: TPanel
      Left = 8
      Top = 10
      Width = 597
      Height = 210
      Color = clWhite
      ParentBackground = False
      TabOrder = 0
      object Panel13: TPanel
        Left = 1
        Top = 136
        Width = 595
        Height = 41
        Align = alTop
        TabOrder = 0
        object ckbAutoMACVerifty: TPanel
          Left = 1
          Top = 1
          Width = 57
          Height = 39
          Align = alLeft
          Color = clCream
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -27
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
          OnClick = ckbBurnClick
        end
        object Panel30: TPanel
          Left = 58
          Top = 1
          Width = 151
          Height = 39
          Align = alLeft
          BevelOuter = bvNone
          Caption = 'MAC'#33258#21205#27604#23565
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object maskeditMACCount: TMaskEdit
          Left = 275
          Top = 1
          Width = 319
          Height = 39
          Align = alClient
          Alignment = taCenter
          Enabled = False
          EditMask = '!>AA:AA:AA:AA:AA:AA;1;_'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -27
          Font.Name = 'Tahoma'
          Font.Style = []
          ImeMode = imSAlpha
          MaxLength = 17
          ParentFont = False
          TabOrder = 2
          Text = '00:00:00:00:00:00'
          ExplicitHeight = 41
        end
        object Panel27: TPanel
          Left = 209
          Top = 1
          Width = 66
          Height = 39
          Align = alLeft
          BevelOuter = bvNone
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          object cbMACVeriftyCount: TComboBox
            Left = 0
            Top = 0
            Width = 66
            Height = 39
            Align = alClient
            BevelInner = bvNone
            BevelOuter = bvNone
            Style = csDropDownList
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -27
            Font.Name = 'Times New Roman'
            Font.Style = []
            ImeMode = imSAlpha
            ItemIndex = 2
            ParentFont = False
            TabOrder = 0
            Text = '8'
            OnChange = cbMACVeriftyCountChange
            Items.Strings = (
              '6'
              '7'
              '8'
              '9'
              '10'
              '11'
              '12')
          end
        end
      end
      object Panel15: TPanel
        Left = 1
        Top = 95
        Width = 595
        Height = 41
        Align = alTop
        TabOrder = 1
        object Panel11: TPanel
          Left = 1
          Top = 1
          Width = 593
          Height = 40
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object ckbNoMAC: TPanel
            Left = 209
            Top = 0
            Width = 57
            Height = 40
            Align = alLeft
            Color = clCream
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -27
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentBackground = False
            ParentFont = False
            TabOrder = 0
            OnClick = ckbBurnClick
          end
          object Panel22: TPanel
            Left = 266
            Top = 0
            Width = 95
            Height = 40
            Align = alLeft
            Caption = 'ZeroMAC'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -20
            Font.Name = 'Times New Roman'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object ckbLan: TPanel
            Left = 0
            Top = 0
            Width = 57
            Height = 40
            Align = alLeft
            Caption = #8730
            Color = clCream
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -27
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentBackground = False
            ParentFont = False
            TabOrder = 2
            OnClick = ckbBurnClick
          end
          object Panel18: TPanel
            Left = 57
            Top = 0
            Width = 152
            Height = 40
            Align = alLeft
            Caption = #32178#36335
            Color = clTeal
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -20
            Font.Name = #27161#26999#39636
            Font.Style = []
            ParentBackground = False
            ParentFont = False
            TabOrder = 3
          end
          object ckbBarcodeVerifty: TPanel
            Left = 361
            Top = 0
            Width = 57
            Height = 40
            Align = alLeft
            Color = clCream
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -27
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentBackground = False
            ParentFont = False
            TabOrder = 4
            OnClick = ckbBurnClick
          end
          object Panel26: TPanel
            Left = 418
            Top = 0
            Width = 127
            Height = 40
            Align = alLeft
            Caption = 'BarcodeVerifty(sec)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Times New Roman'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object cbVetiftyWaitTime: TComboBox
            Left = 545
            Top = 0
            Width = 48
            Height = 35
            Align = alClient
            BevelInner = bvNone
            BevelOuter = bvNone
            Style = csDropDownList
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -24
            Font.Name = 'Times New Roman'
            Font.Style = []
            ImeMode = imSAlpha
            ItemIndex = 5
            ParentFont = False
            TabOrder = 6
            Text = '10'
            OnChange = cbMACVeriftyCountChange
            Items.Strings = (
              '5'
              '6'
              '7'
              '8'
              '9'
              '10'
              '11'
              '12'
              '13'
              '14'
              '15'
              '16'
              '17'
              '18'
              '19'
              '20')
          end
        end
      end
      object Panel19: TPanel
        Left = 1
        Top = 1
        Width = 595
        Height = 41
        Align = alTop
        Caption = #29138#37636
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -20
        Font.Name = #27161#26999#39636
        Font.Style = []
        ParentBackground = False
        ParentFont = False
        TabOrder = 2
        object Panel17: TPanel
          Left = 1
          Top = 1
          Width = 208
          Height = 39
          Align = alLeft
          Caption = '      '#29138#37636
          Color = clTeal
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = #27161#26999#39636
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
          object ckbBurn: TPanel
            Left = 1
            Top = 1
            Width = 57
            Height = 37
            Align = alLeft
            Color = clCream
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -27
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentBackground = False
            ParentFont = False
            TabOrder = 0
            OnClick = ckbBurnClick
          end
        end
        object Edit_resetMAC: TMaskEdit
          Left = 209
          Top = 1
          Width = 385
          Height = 39
          Align = alClient
          Alignment = taCenter
          Enabled = False
          EditMask = '!>AA:AA:AA:AA:AA:AA;1;_'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -27
          Font.Name = 'Tahoma'
          Font.Style = []
          ImeMode = imSAlpha
          MaxLength = 17
          ParentFont = False
          TabOrder = 1
          Text = '00:24:9B:00:00:00'
          ExplicitHeight = 41
        end
      end
      object Panel16: TPanel
        Left = 1
        Top = 42
        Width = 595
        Height = 41
        Align = alTop
        TabOrder = 3
        object ckbBarcode: TPanel
          Left = 1
          Top = 1
          Width = 57
          Height = 39
          Align = alLeft
          Color = clCream
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -27
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
          OnClick = ckbBurnClick
        end
        object Panel21: TPanel
          Left = 58
          Top = 1
          Width = 219
          Height = 39
          Align = alLeft
          Caption = 'BarcodeMAC'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object ckbAutoAdd: TPanel
          Left = 277
          Top = 1
          Width = 57
          Height = 39
          Align = alLeft
          Color = clCream
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -27
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 2
          OnClick = ckbBurnClick
        end
        object Panel20: TPanel
          Left = 334
          Top = 1
          Width = 260
          Height = 39
          Align = alClient
          Caption = #33258#21205#36339#34399
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
      end
      object Panel23: TPanel
        Left = 1
        Top = 177
        Width = 595
        Height = 32
        Align = alClient
        Caption = 'Panel23'
        TabOrder = 4
        object btnEnd: TBitBtn
          Left = 458
          Top = 1
          Width = 136
          Height = 30
          Align = alRight
          Caption = #32080#26463#35373#23450
          DoubleBuffered = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = #27161#26999#39636
          Font.Style = []
          ParentDoubleBuffered = False
          ParentFont = False
          TabOrder = 0
          OnClick = btnEndClick
        end
        object edtPassWord: TEdit
          Left = 1
          Top = 1
          Width = 323
          Height = 30
          Align = alClient
          Alignment = taCenter
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -27
          Font.Name = 'Times New Roman'
          Font.Style = []
          ImeMode = imSAlpha
          ParentFont = False
          PasswordChar = '*'
          TabOrder = 1
          TextHint = 'PASSWORD'
          OnEnter = edtPassWordEnter
          ExplicitHeight = 39
        end
        object BitBtn2: TBitBtn
          Left = 324
          Top = 1
          Width = 134
          Height = 30
          Align = alRight
          Caption = 'Enter'
          DoubleBuffered = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = #27161#26999#39636
          Font.Style = []
          ParentDoubleBuffered = False
          ParentFont = False
          TabOrder = 2
          OnClick = BitBtn2Click
        end
      end
      object Panel24: TPanel
        Left = 1
        Top = 83
        Width = 595
        Height = 12
        Align = alTop
        Color = clSkyBlue
        ParentBackground = False
        TabOrder = 5
      end
    end
  end
  object plBarcode: TPanel
    Left = 8
    Top = 288
    Width = 495
    Height = 138
    Color = clWhite
    ParentBackground = False
    TabOrder = 3
    object lbBarcode: TLabel
      Left = 1
      Top = 1
      Width = 493
      Height = 48
      Align = alTop
      Alignment = taCenter
      AutoSize = False
      Caption = #35531#25475#25551'MAC'#26781#30908
      Color = clTeal
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -43
      Font.Name = #27161#26999#39636
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      Transparent = False
      ExplicitTop = 9
    end
    object Panel12: TPanel
      Left = 1
      Top = 96
      Width = 493
      Height = 41
      Align = alBottom
      Caption = 'Panel4'
      TabOrder = 0
      object btnBarcodeChanel: TBitBtn
        Left = 248
        Top = 1
        Width = 244
        Height = 39
        Align = alRight
        Caption = #21462#28040
        DoubleBuffered = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -33
        Font.Name = #27161#26999#39636
        Font.Style = []
        ParentDoubleBuffered = False
        ParentFont = False
        TabOrder = 0
        OnClick = btnBarcodeChanelClick
      end
      object btnBarcodeClear: TBitBtn
        Left = 0
        Top = 1
        Width = 248
        Height = 39
        Align = alRight
        Caption = #28165#38500
        DoubleBuffered = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -33
        Font.Name = #27161#26999#39636
        Font.Style = []
        ParentDoubleBuffered = False
        ParentFont = False
        TabOrder = 1
        OnClick = btnBarcodeClearClick
      end
    end
    object edtBarcodeMAC: TEdit
      Left = 1
      Top = 49
      Width = 493
      Height = 50
      Align = alTop
      Alignment = taCenter
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -37
      Font.Name = 'Times New Roman'
      Font.Style = []
      ImeMode = imSAlpha
      ParentFont = False
      TabOrder = 1
      OnChange = edtBarcodeMACChange
      OnKeyDown = edtBarcodeMACKeyDown
    end
  end
  object Panel14: TPanel
    Left = 8
    Top = 432
    Width = 495
    Height = 257
    TabOrder = 4
    object WRITE: TButton
      Left = 1
      Top = 44
      Width = 75
      Height = 25
      Caption = 'WRITE'
      TabOrder = 0
      OnClick = WRITEClick
    end
    object CHECK: TButton
      Left = 393
      Top = 13
      Width = 75
      Height = 25
      Caption = 'CHECK'
      TabOrder = 1
      OnClick = CHECKClick
    end
    object LOAD: TButton
      Left = 163
      Top = 44
      Width = 75
      Height = 25
      Caption = 'LOAD'
      TabOrder = 2
      OnClick = LOADClick
    end
    object SAVE: TButton
      Left = 82
      Top = 44
      Width = 75
      Height = 25
      Caption = 'SAVE'
      TabOrder = 3
    end
    object READ: TButton
      Left = 1
      Top = 13
      Width = 75
      Height = 25
      Caption = 'READ'
      TabOrder = 4
      OnClick = READClick
    end
    object MACP: TButton
      Left = 82
      Top = 12
      Width = 75
      Height = 25
      Caption = 'MAC ++'
      TabOrder = 5
      OnClick = MACPClick
    end
    object CHKDEVICE: TButton
      Left = 231
      Top = 13
      Width = 75
      Height = 25
      Caption = 'CHKDEVICE'
      TabOrder = 6
    end
    object changedevice: TButton
      Left = 150
      Top = 13
      Width = 75
      Height = 25
      Caption = #35373#20633#20999#25563
      TabOrder = 7
    end
    object SaveLog: TButton
      Left = 312
      Top = 13
      Width = 75
      Height = 25
      Caption = 'LOG'
      TabOrder = 8
    end
    object Edit1: TEdit
      Left = 244
      Top = 44
      Width = 34
      Height = 21
      TabOrder = 9
    end
    object Edit2: TEdit
      Left = 284
      Top = 44
      Width = 34
      Height = 21
      TabOrder = 10
    end
    object Edit3: TEdit
      Left = 336
      Top = 44
      Width = 34
      Height = 21
      TabOrder = 11
    end
    object Edit4: TEdit
      Left = 244
      Top = 71
      Width = 34
      Height = 21
      TabOrder = 12
    end
    object Edit5: TEdit
      Left = 284
      Top = 71
      Width = 34
      Height = 21
      TabOrder = 13
    end
    object Edit6: TEdit
      Left = 336
      Top = 81
      Width = 34
      Height = 21
      TabOrder = 14
    end
    object Edit9: TEdit
      Left = 440
      Top = 44
      Width = 34
      Height = 21
      TabOrder = 15
      Text = '01'
    end
    object Edit8: TEdit
      Left = 400
      Top = 44
      Width = 34
      Height = 21
      TabOrder = 16
      Text = '00'
    end
    object Edit7: TEdit
      Left = 376
      Top = 44
      Width = 34
      Height = 21
      TabOrder = 17
      Text = '00'
    end
    object Memo2: TMemo
      Left = 0
      Top = -2
      Width = 238
      Height = 251
      Lines.Strings = (
        'Memo2')
      ScrollBars = ssVertical
      TabOrder = 18
    end
    object SpeedTest: TCheckBox
      Left = 393
      Top = 71
      Width = 97
      Height = 17
      Caption = 'SpeedTest'
      TabOrder = 19
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 448
    object N1: TMenuItem
      Caption = #24489#21407'ZeroMAC'#35387#20874#34920
      OnClick = N1Click
    end
    object popRegDriver: TMenuItem
      Caption = #35387#20874#39493#21205
      OnClick = popRegDriverClick
    end
  end
  object TimerMain: TTimer
    Interval = 200
    OnTimer = TimerMainTimer
    Left = 560
    Top = 8
  end
  object IdIcmpClient: TIdIcmpClient
    ReceiveTimeout = 1000
    Protocol = 1
    ProtocolIPv6 = 58
    IPVersion = Id_IPv4
    PacketSize = 32
    OnReply = IdIcmpClientReply
    Left = 504
    Top = 8
  end
end
