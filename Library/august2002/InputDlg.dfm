object InputForm: TInputForm
  Left = 253
  Top = 198
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Input'
  ClientHeight = 161
  ClientWidth = 241
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object InputBox: TInputGroupBox
    Left = 16
    Top = 16
    Width = 205
    Height = 85
    Caption = 'InputBox'
    TabOrder = 0
  end
  object Ok: TButton
    Left = 16
    Top = 120
    Width = 75
    Height = 25
    Caption = 'Ok'
    ModalResult = 1
    TabOrder = 1
  end
  object Cancel: TButton
    Left = 128
    Top = 120
    Width = 75
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
end
