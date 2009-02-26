object ProcessChoice1: TProcessChoice1
  Left = 0
  Top = 0
  Width = 451
  Height = 304
  Align = alClient
  TabOrder = 0
  object Splitter1: TSplitter
    Left = 320
    Top = 0
    Height = 304
    ExplicitHeight = 277
  end
  object ProcessTree: TTreeView
    Left = 0
    Top = 0
    Width = 320
    Height = 304
    Align = alLeft
    Indent = 19
    ReadOnly = True
    TabOrder = 0
    OnDblClick = ProcessTreeDblClick
    OnEnter = ProcessTreeEnter
  end
  object AdditionalInfo: TTreeView
    Left = 323
    Top = 0
    Width = 128
    Height = 304
    Align = alClient
    Indent = 19
    TabOrder = 1
  end
end
