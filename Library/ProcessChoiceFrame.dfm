object ProcessChoice1: TProcessChoice1
  Left = 0
  Top = 0
  Width = 443
  Height = 277
  Align = alClient
  TabOrder = 0
  object Splitter1: TSplitter
    Left = 320
    Top = 0
    Width = 3
    Height = 277
    Cursor = crHSplit
  end
  object ProcessTree: TTreeView
    Left = 0
    Top = 0
    Width = 320
    Height = 277
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
    Width = 120
    Height = 277
    Align = alClient
    Indent = 19
    TabOrder = 1
  end
end
