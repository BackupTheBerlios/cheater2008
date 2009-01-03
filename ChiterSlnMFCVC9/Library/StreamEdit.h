//---------------------------------------------------------------------------

#ifndef StreamEditH
#define StreamEditH
//---------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include "InputGroupBox.h"
#include "searcher.h"
#include "Standart/AnsiLib.h"
#include "PointersViewBox.h"
#include "ProcessManager.h"
#include "SearcherProperties.h"
#include "CountEdit.h"
#include "august2002Fwd.h"
#include "AuxiliaryTypedefs.h"
//class TForm1;
extern char* WhatFind;




//---------------------------------------------------------------------------
class TStreamEdit : public CDialog
{
public:
        TGroupBox *GroupBox1;
        TPopupMenu *PopupMenu1;
        TSplitter *Splitter1;
        TSplitter *Splitter2;
        TGroupBox *GroupBox2;
        TSplitter *Splitter3;

        CMenuItem *SearchMenuItem;
        CMenuItem *LoadFromStreamMenuitem;
        CMenuItem *GotoMenuItem;
        CMenuItem *CopyMenuItem;
        CMenuItem *PasteMenuItem;
        CMenuItem *N1;
        CMenuItem *N2;
        CMenuItem *CopytoMenuItem;
        CMenuItem *PasteFromMenuItem;
        CMenuItem *ClearClipBoardMenuItem;
        CMenuItem *N3;
        CMenuItem *FonCMenuItem;
        CMenuItem *StringFonCMenuItem;
        CMenuItem *HexFonCMenuItem;
        CMenuItem *PointerFonCMenuItem;
        CMenuItem *StringColorMenuItem;
        CMenuItem *HexColorMenuItem;
        CMenuItem *PointerColorMenuItem;
        CMenuItem *DeleteMenuItem;
        CMenuItem *N4;

        CFontDialog *FontDialog;
        CColorDialog *ColorDialog;
        CMenuItem *ColorMenuItem;
//        CMenuItem *PointerEdiCMenuItem;
        CMenuItem *Font1;
        CMenuItem *Color1;
//        CMenuItem *HexEdit1;
//        CMenuItem *StringEdit1;
//        CMenuItem *PointerEdit1;
//        CMenuItem *HexEdiCMenuItem;
        COpenFileDialog *OpenDialog;
        CSaveFileDialog *SaveDialog;
        CMenuItem *EditBufMenuItem;
        TCountEdit *StringNumberEdit;
        TCountEdit *StringLengthEdit;

        CEdit *StringMemo;
        CEdit *HexMemo;
        CEdit *PointerMemo;

        CMenuItem *ContinueSearchMenuItem;
        TPanel *BottomPanel;
        TProgressBar *ProgressBar;
        TSplitter *Splitter5;
/*
        TLabel *InfoLabel;
        TLabel *KeyLabel;
*/
        CRichEdit* InfoEdit;
        CMenuItem *SlowSearchMenuItem;
//        CMenuItem *N5;
        CMenuItem *ReplaceAllMenuItem;
        TSearcherProperties *SearcherProperties;
        InputGroupBox *GotoInputGroupBox;
        TSplitter *Splitter4;
        void  SearchMenuItemClick(TObject *Sender);
        void  LoadFromStreamMenuitemClick(TObject *Sender);

       void  HexRichEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void  GotoMenuItemClick(TObject *Sender);
        void  HexRichEditKeyPress(TObject *Sender, char &Key);
        void  CopyMenuItemClick(TObject *Sender);
        void  PopupMenu1Popup(TObject *Sender);
        void  PasteMenuItemClick(TObject *Sender);
        void  DeleteSubMenuClick(TObject *Sender);
        void  CopytoMenuClick(TObject *Sender);
        void  PasteFromMenuClick(TObject *Sender);
        void  EditBufMenuClick(TObject *Sender);
        void  ClearClipBoardMenuItemClick(TObject *Sender);
        void  Font1Click(TObject *Sender);
        void  Color1Click(TObject *Sender);
        void  ChangeColor(TObject *Sender);
        void  StringFonCMenuItemClick(TObject *Sender);
        void  PointerFonCMenuItemClick(TObject *Sender);
        void  HexFonCMenuItemClick(TObject *Sender);
        void  StringColorMenuItemClick(TObject *Sender);
        void  PointerColorMenuItemClick(TObject *Sender);
        void  HexColorMenuItemClick(TObject *Sender);
//        void  SetOptionButtonClick(TObject *Sender);
        void  StringRichEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void  StringRichEditKeyPress(TObject *Sender, char &Key);
        void  StringRichEditMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void  HexRichEditMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void  ContinueSearchMenuItemClick(TObject *Sender);
        void  SlowSearchMenuItemClick(TObject *Sender);
        void  ReplaceAllMenuItemClick(TObject *Sender);
private:
   void  PointersNotifyEvent(int);
   void  UpdateInfoString(void);
   void  InitInterface(void);
void  StringsOptionChangedEvent(TObject *Sender,DWORD value);
//        TMemoryStream* MemoryStream;
        int StringLen;
        int NumberOfString;

        int ViewedLen;
         int FCurrentPos;
        bool IsProcessKeyDown;
        boost::shared_ptr<std::iostream> Fstream;
        std::string FKeyPressed;
        std::string FCurrectEditSelection;
        void  LoadFromStream(void);
        int  ConvertHexPosToGlobal(int value);
        void  HexRichEditKeyPres(unsigned char value);
        void ShiftIfWrongHexSelStart();
        int  ConvertGlobalToHexPos(int value);
        int FindInStream(char* buf, int len);
        void  DoProgress(int pos);
        void  GetVariablesAtPos(int);
//        TMemoryStream*  GetFindStream();
        //TMemoryStream*  GetReplaceStream();
//        TList*  GetFindPointers();

public:		// User declarations
        TStreamEdit();
        virtual ~TStreamEdit(void);
        void  ShiftHexSelStart(int Shift);
        int  ConvertGlobalToStringPos(int value);
        int  ConvertStringPosToGlobal(int value);
        void  ShiftStringSelStart(int value);
        void  StringRichEditKeyPres(unsigned char);
        void  ShiftIfWrongStringSelStart(void);
        bool  Search(bool IsNewSearch);
        boost::shared_ptr<TSearcher>  GetSearcher(void);
        bool  SlowSearch(void);
        
        int getCurrentPos() const { return FCurrentPos;}
        void setCurrentPos(int i_value);

        boost::shared_ptr<std::iostream>  getStream(){ return Fstream;}
        void setStream(boost::shared_ptr<std::iostream>)

        std::string    getKeyPressed() const {return FKeyPressed;}
        void   setKeyPressed(const std::string& i_value );

        std::string    getCurrectEditSelection() const {return FCurrectEditSelection;}
        void   setCurrectEditSelection(const std::string& i_value );
};
//---------------------------------------------------------------------------
#endif

