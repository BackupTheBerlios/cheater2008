object EditMemoryForm: TEditMemoryForm
  Left = 105
  Top = 169
  HorzScrollBar.ButtonSize = 10
  HorzScrollBar.Color = clInfoText
  HorzScrollBar.ParentColor = False
  HorzScrollBar.Smooth = True
  HorzScrollBar.Size = 122
  HorzScrollBar.Tracking = True
  Caption = 'EditMemoryForm'
  ClientHeight = 544
  ClientWidth = 838
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object StreamEdit: TStreamEdit
    Left = 0
    Top = 0
    Width = 838
    Height = 544
    Align = alClient
    Caption = 'StreamEdit'
    TabOrder = 0
    ExplicitLeft = 8
    ExplicitTop = 8
  end
end
