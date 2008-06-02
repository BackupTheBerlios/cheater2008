object Form1: TForm1
  Left = 192
  Top = 172
  Width = 948
  Height = 778
  Caption = 'HCheater'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object StatusBar1: TStatusBar
    Left = 0
    Top = 705
    Width = 940
    Height = 27
    Panels = <
      item
        Width = 100
      end
      item
        Width = 100
      end
      item
        Width = 100
      end
      item
        Width = 100
      end>
    SimplePanel = False
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 940
    Height = 705
    ActivePage = TabSheet1
    Align = alClient
    TabIndex = 0
    TabOrder = 1
    object TabSheet1: TTabSheet
      Caption = 'Stream Edit'
      object EditMemoryFrame: TStreamEdit
        Left = 0
        Top = 0
        Width = 932
        Height = 677
        Align = alClient
        Caption = 'EditMemoryFrame'
        TabOrder = 0
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Process chose'
      ImageIndex = 1
      object Button1: TButton
        Left = 456
        Top = 8
        Width = 75
        Height = 25
        Caption = 'Reload'
        TabOrder = 0
        OnClick = Button1Click
      end
      object ListBox: TListBox
        Left = 0
        Top = 0
        Width = 449
        Height = 677
        Align = alLeft
        ItemHeight = 13
        TabOrder = 1
        OnClick = ListBoxClick
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'Prcess list'
      ImageIndex = 2
      inline ProcessChoice11: TProcessChoice1
        Left = 0
        Top = 0
        Width = 932
        Height = 677
        Align = alClient
        TabOrder = 0
        inherited Splitter1: TSplitter
          Height = 677
        end
        inherited ProcessTree: TTreeView
          Height = 677
        end
        inherited AdditionalInfo: TTreeView
          Width = 609
          Height = 677
        end
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'Memory Info'
      ImageIndex = 3
      object Button2: TButton
        Left = 696
        Top = 24
        Width = 75
        Height = 25
        Caption = 'Button2'
        TabOrder = 0
        OnClick = Button2Click
      end
      object Button3: TButton
        Left = 512
        Top = 264
        Width = 75
        Height = 25
        Caption = 'Button3'
        TabOrder = 1
        OnClick = Button3Click
      end
      object Edit1: TEdit
        Left = 592
        Top = 232
        Width = 281
        Height = 21
        TabOrder = 2
        Text = 'Edit1'
      end
      object Edit2: TEdit
        Left = 592
        Top = 280
        Width = 281
        Height = 21
        TabOrder = 3
        Text = 'Edit2'
      end
      object Edit3: TEdit
        Left = 624
        Top = 312
        Width = 281
        Height = 21
        TabOrder = 4
        Text = 'Edit3'
      end
      object Button4: TButton
        Left = 488
        Top = 72
        Width = 75
        Height = 25
        Caption = 'Button4'
        TabOrder = 5
        OnClick = Button4Click
      end
      object Button5: TButton
        Left = 496
        Top = 128
        Width = 75
        Height = 25
        Caption = 'Button5'
        TabOrder = 6
        OnClick = Button5Click
      end
      object Button6: TButton
        Left = 496
        Top = 184
        Width = 75
        Height = 25
        Caption = 'Button6'
        TabOrder = 7
        OnClick = Button6Click
      end
      object Button7: TButton
        Left = 832
        Top = 24
        Width = 75
        Height = 25
        Caption = 'Button7'
        TabOrder = 8
      end
      object MemoryInfo: TMemoryInfoEdit
        Left = 16
        Top = 16
        Width = 145
        Height = 513
        Caption = 'MemoryInfo'
        TabOrder = 9
      end
      object MemPtr: TInputGroupBox
        Left = 24
        Top = 536
        Width = 205
        Height = 85
        Caption = 'MemPtr'
        TabOrder = 10
      end
      object Set: TButton
        Left = 240
        Top = 560
        Width = 75
        Height = 25
        Caption = 'Set'
        TabOrder = 11
        OnClick = SetClick
      end
      object Reload: TButton
        Left = 240
        Top = 584
        Width = 75
        Height = 25
        Caption = 'Reload'
        TabOrder = 12
        OnClick = ReloadClick
      end
      object Memo1: TMemo
        Left = 176
        Top = 32
        Width = 289
        Height = 489
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'System'
        Font.Style = [fsBold]
        Lines.Strings = (
          'PAGE_NOACCESS                        0x01     '
          'PAGE_READONLY                       0x02     '
          'PAGE_READWRITE                     0x04     '
          'PAGE_WRITECOPY                     0x08     '
          'PAGE_EXECUTE                          0x10      '
          'PAGE_EXECUTE_READ              0x20     '
          'PAGE_EXECUTE_READWRITE  0x40     '
          'PAGE_EXECUTE_WRITECOPY  0x80     '
          'PAGE_GUARD                             0x100     '
          'PAGE_NOCACHE                         0x200     '
          'PAGE_WRITECOMBINE              0x400     '
          'MEM_COMMIT                             0x1000     '
          'MEM_RESERVE                          0x2000     '
          'MEM_DECOMMIT                        0x4000     '
          'MEM_RELEASE                           0x8000     '
          'MEM_FREE                                  0x10000    '
          'MEM_PRIVATE                           0x20000     '
          'MEM_MAPPED                           0x40000     '
          'MEM_RESET                              0x80000     '
          'MEM_TOP_DOWN                     0x100000    '
          'MEM_WRITE_WATCH               0x200000     '
          'MEM_PHYSICAL                        0x400000    '
          'MEM_4MB_PAGES                   0x80000000 '
          'SEC_FILE                                   0x800000    '
          'SEC_IMAGE                              0x1000000    '
          'SEC_VLM                                  0x2000000    '
          'SEC_RESERVE                         0x4000000    '
          'SEC_COMMIT                           0x8000000    '
          'SEC_NOCACHE                        0x10000000  '
          ' '
          '')
        ParentFont = False
        TabOrder = 13
      end
    end
    object TabSheet5: TTabSheet
      Caption = 'Searcher List'
      ImageIndex = 4
      object ChitProperties: TChitThreadProperties
        Left = 0
        Top = 0
        Width = 932
        Height = 677
        Align = alClient
        Caption = 'ChitProperties'
        TabOrder = 0
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 810
    Top = 440
  end
  object MainMenu1: TMainMenu
    AutoHotkeys = maManual
    Left = 596
    Top = 448
    object File1: TMenuItem
      Caption = 'File'
      object OpenFileMenuItem: TMenuItem
        AutoHotkeys = maManual
        Caption = 'Open'
        OnClick = OpenFileMenuItemClick
      end
    end
    object Process1: TMenuItem
      Caption = 'Process'
      object CreateProcessMenuItem: TMenuItem
        Caption = 'Create'
        OnClick = CreateProcessMenuItemClick
      end
      object SetStartMenuItem: TMenuItem
        Caption = 'Set Start'
        OnClick = SetStartMenuItemClick
      end
      object SetSizeMenuItem: TMenuItem
        Caption = 'Set Size'
        OnClick = SetSizeMenuItemClick
      end
    end
    object ChitOptions: TMenuItem
      Caption = 'Chiter'
      OnClick = ChitOptionsClick
      object Suspend: TMenuItem
        Caption = 'Suspended'
        OnClick = SuspendClick
      end
      object Interval: TMenuItem
        Caption = 'Interval'
        OnClick = IntervalClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object AddSearcherMenuItem: TMenuItem
        Caption = 'Add Searcher'
        OnClick = AddSearcherMenuItemClick
      end
    end
  end
  object ServerSocket1: TServerSocket
    Active = True
    Port = 50000
    ServerType = stNonBlocking
    OnClientRead = ServerSocket1ClientRead
    Left = 464
    Top = 64
  end
  object TcpServer1: TTcpServer
    Active = True
    BlockMode = bmNonBlocking
    LocalHost = '192.168.0.1'
    LocalPort = '50001'
    Left = 512
    Top = 64
  end
end
