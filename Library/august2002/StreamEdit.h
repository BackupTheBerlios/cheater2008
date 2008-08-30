//---------------------------------------------------------------------------
#ifndef StreamEditH
#define StreamEditH
//---------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <ComCtrls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
#include "InputGroupBox.h"
#include "searcher.h"
#include "AnsiLib.h"
#include "PointersViewBox.h"
#include "ProcessManager.h"
#include "SearcherProperties.h"
#include "CountEdit.h"
//
//class TForm1;
extern char* WhatFind;
//---------------------------------------------------------------------------
class PACKAGE TStreamEdit : public TGroupBox
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TPopupMenu *PopupMenu1;
        TSplitter *Splitter1;
        TSplitter *Splitter2;
        TGroupBox *GroupBox2;
        TSplitter *Splitter3;

        TMenuItem *SearchMenuItem;
        TMenuItem *LoadFromStreamMenuitem;
        TMenuItem *GotoMenuItem;
        TMenuItem *CopyMenuItem;
        TMenuItem *PasteMenuItem;
        TMenuItem *N1;
        TMenuItem *N2;
        TMenuItem *CopytoMenuItem;
        TMenuItem *PasteFromMenuItem;
        TMenuItem *ClearClipBoardMenuItem;
        TMenuItem *N3;
        TMenuItem *FontMenuItem;
        TMenuItem *StringFontMenuItem;
        TMenuItem *HexFontMenuItem;
        TMenuItem *PointerFontMenuItem;
        TMenuItem *StringColorMenuItem;
        TMenuItem *HexColorMenuItem;
        TMenuItem *PointerColorMenuItem;
        TMenuItem *DeleteMenuItem;
        TMenuItem *N4;
        TFontDialog *FontDialog;
        TColorDialog *ColorDialog;
        TMenuItem *ColorMenuItem;
//        TMenuItem *PointerEditMenuItem;
        TMenuItem *Font1;
        TMenuItem *Color1;
//        TMenuItem *HexEdit1;
//        TMenuItem *StringEdit1;
//        TMenuItem *PointerEdit1;
//        TMenuItem *HexEditMenuItem;
        TOpenDialog *OpenDialog;
        TSaveDialog *SaveDialog;
        TMenuItem *EditBufMenuItem;
        TCountEdit *StringNumberEdit;
        TCountEdit *StringLengthEdit;

        TMemo *StringMemo;
        TMemo *HexMemo;
        TMemo *PointerMemo;
        TMenuItem *ContinueSearchMenuItem;
        TPanel *BottomPanel;
        TProgressBar *ProgressBar;
        TSplitter *Splitter5;
/*
        TLabel *InfoLabel;
        TLabel *KeyLabel;
*/
        TRichEdit* InfoEdit;
        TMenuItem *SlowSearchMenuItem;
//        TMenuItem *N5;
        TMenuItem *ReplaceAllMenuItem;
        TSearcherProperties *SearcherProperties;
        TInputGroupBox *GotoInputGroupBox;
        TSplitter *Splitter4;
        void __fastcall SearchMenuItemClick(TObject *Sender);
        void __fastcall LoadFromStreamMenuitemClick(TObject *Sender);

       void __fastcall HexRichEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall GotoMenuItemClick(TObject *Sender);
        void __fastcall HexRichEditKeyPress(TObject *Sender, char &Key);
        void __fastcall CopyMenuItemClick(TObject *Sender);
        void __fastcall PopupMenu1Popup(TObject *Sender);
        void __fastcall PasteMenuItemClick(TObject *Sender);
        void __fastcall DeleteSubMenuClick(TObject *Sender);
        void __fastcall CopytoMenuClick(TObject *Sender);
        void __fastcall PasteFromMenuClick(TObject *Sender);
        void __fastcall EditBufMenuClick(TObject *Sender);
        void __fastcall ClearClipBoardMenuItemClick(TObject *Sender);
        void __fastcall Font1Click(TObject *Sender);
        void __fastcall Color1Click(TObject *Sender);
        void __fastcall ChangeColor(TObject *Sender);
        void __fastcall StringFontMenuItemClick(TObject *Sender);
        void __fastcall PointerFontMenuItemClick(TObject *Sender);
        void __fastcall HexFontMenuItemClick(TObject *Sender);
        void __fastcall StringColorMenuItemClick(TObject *Sender);
        void __fastcall PointerColorMenuItemClick(TObject *Sender);
        void __fastcall HexColorMenuItemClick(TObject *Sender);
//        void __fastcall SetOptionButtonClick(TObject *Sender);
        void __fastcall StringRichEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall StringRichEditKeyPress(TObject *Sender, char &Key);
        void __fastcall StringRichEditMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall HexRichEditMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall ContinueSearchMenuItemClick(TObject *Sender);
        void __fastcall SlowSearchMenuItemClick(TObject *Sender);
        void __fastcall ReplaceAllMenuItemClick(TObject *Sender);
private:
   void __fastcall PointersNotifyEvent(int);
   void __fastcall UpdateInfoString(void);
   void __fastcall InitInterface(void);
void __fastcall StringsOptionChangedEvent(TObject *Sender,DWORD value);
//        TMemoryStream* MemoryStream;
        int StringLen;
        int NumberOfString;

        int ViewedLen;
         int FCurrentPos;
        bool IsProcessKeyDown;
        boost::shared_ptr<TStream> Fstream;
        AnsiString FKeyPressed;
        AnsiString FCurrectEditSelection;
        void __fastcall LoadFromStream(void);
        int __fastcall ConvertHexPosToGlobal(int value);
        void __fastcall HexRichEditKeyPres(unsigned char value);
        void ShiftIfWrongHexSelStart();
        int __fastcall ConvertGlobalToHexPos(int value);
        int FindInStream(char* buf, int len);
        void __fastcall SetCurrentPos( int value);
        void __fastcall Setstream(boost::shared_ptr<TStream> value);
        void __fastcall DoProgress(int pos);
        void __fastcall GetVariablesAtPos(int);
        TMemoryStream* __fastcall GetFindStream();
        TMemoryStream* __fastcall GetReplaceStream();
//        TList* __fastcall GetFindPointers();
        void __fastcall SetKeyPressed(AnsiString value);
        void __fastcall SetCurrectEditSelection(AnsiString value);

public:		// User declarations
        __fastcall TStreamEdit(TComponent* Owner);
        __fastcall ~TStreamEdit(void);
        void __fastcall ShiftHexSelStart(int Shift);
        int __fastcall ConvertGlobalToStringPos(int value);
        int __fastcall ConvertStringPosToGlobal(int value);
        void __fastcall ShiftStringSelStart(int value);
        void __fastcall StringRichEditKeyPres(unsigned char);
        void __fastcall ShiftIfWrongStringSelStart(void);
        bool __fastcall Search(bool IsNewSearch);
        TSearcher* __fastcall GetSearcher(void);
        bool __fastcall TStreamEdit::SlowSearch(void);
        __property  int CurrentPos  = { read=FCurrentPos, write=SetCurrentPos };
        __property boost::shared_ptr<TStream> stream  = { read=Fstream, write=Setstream };
        __property AnsiString KeyPressed  = { read=FKeyPressed, write=SetKeyPressed };
        __property AnsiString CurrectEditSelection  = { read=FCurrectEditSelection,write=SetCurrectEditSelection };
protected:
};
//---------------------------------------------------------------------------
#endif

