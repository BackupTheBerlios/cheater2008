//---------------------------------------------------------------------------

#ifndef StreamEditH
#define StreamEditH
//---------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include "InputGroupBox.h"
#include "AuxiliaryTypedefs.h"
#include "MyBaseForm.h"
#include "Library/Command.h"
#include "searcher.h"
#include "Standart/AnsiLib.h"
#include "PointersViewBox.h"
#include "ProcessManager.h"
#include "SearcherProperties.h"
#include "CountEdit.h"
#include "august2002Fwd.h"
#include "AuxiliaryTypedefs.h"
#include "Library/WindowProcedure.h"
//class TForm1;
extern char* WhatFind;




//---------------------------------------------------------------------------
class AUGUST2002_EXPORT TStreamEdit : public CMyBaseForm
{
  DECLARE_DYNAMIC(TStreamEdit)
private:
  Container d_menuCommands;
  SplitterCtrl_NS::SplitterCtrl d_splitterWnd;
  SplitterCtrl_NS::SplitterCtrl d_splitterWndTop;
  SplitterCtrl_NS::SplitterCtrl d_splitterWndTopLeft;
  SplitterCtrl_NS::SplitterCtrl d_splitterWndTopRight;
  CStatic d_splitterWndTopRightBox;
  SplitterCtrl_NS::SplitterCtrl d_splitterWndBottom;

  TPopupMenu PopupMenu;
  CMenuItem CopytoMenuItem;
  CMenuItem PasteFromMenuItem;

  CMenuItem SearchMenuItem;
  CMenuItem LoadFromStreamMenuitem;
  CMenuItem GotoMenuItem;
  CMenuItem CopyMenuItem;
  CMenuItem PasteMenuItem;

  CMenuItem ClearClipBoardMenuItem;
  CMenuItem FonCMenuItem;
  CMenuItem StringFonCMenuItem;
  CMenuItem HexFonCMenuItem;
  CMenuItem PointerFonCMenuItem;
  CMenuItem StringColorMenuItem;
  CMenuItem HexColorMenuItem;
  CMenuItem PointerColorMenuItem;
  CMenuItem DeleteMenuItem;

  CFontDialog FontDialog;
  CColorDialog ColorDialog;

  boost::shared_ptr<CFont> d_pointerMemoFont;
  boost::shared_ptr<CFont> d_hexMemoFont;
  boost::shared_ptr<CFont> d_stringMemoFont;

  DWORD d_pointerMemoColor;
  DWORD d_hexMemoColor;
  DWORD d_stringMemoColor;


  CMenuItem ColorMenuItem;
  //        CMenuItem *PointerEdiCMenuItem;
  CMenuItem Font1;
  CMenuItem Color1;
  //        CMenuItem *HexEdit1;
  //        CMenuItem *StringEdit1;
  //        CMenuItem *PointerEdit1;
  //        CMenuItem *HexEdiCMenuItem;
  COpenFileDialog OpenDialog;
  CSaveFileDialog SaveDialog;
  CMenuItem EditBufMenuItem;
  TCountEdit StringNumberEdit;
  TCountEdit StringLengthEdit;

  CRichEditCtrl d_stringMemo;
  CRichEditCtrl d_hexMemo;
  CRichEditCtrl d_pointerMemo;

  CMenuItem ContinueSearchMenuItem;
  TProgressBar ProgressBar;
  /*
  TLabel *InfoLabel;
  TLabel *KeyLabel;
  */
  CRichEdit InfoEdit;
  CMenuItem SlowSearchMenuItem;
  //        CMenuItem *N5;
  CMenuItem ReplaceAllMenuItem;
  TSearcherProperties SearcherProperties;
  InputGroupBox GotoInputGroupBox;
public:
  void  SearchMenuItemClick();
  void  LoadFromStreamMenuitemClick();

  void  HexRichEditKeyDown(UINT Key);
  void  HexRichEditKeyPress(unsigned char Key);
  void  HexRichEditKeyPres(unsigned char value);

  void  StringRichEditKeyPress(unsigned char Key);        
  void  StringRichEditKeyPres(unsigned char);
  void  StringRichEditKeyDown(UINT Key);

  void  GotoMenuItemClick();

  void  CopyMenuItemClick();
  void  PopupMenu1Popup();
  void  PasteMenuItemClick();
  void  DeleteSubMenuClick(int idx);
  void  CopytoMenuClick(int idx);
  void  PasteFromMenuClick(int idx);
  void  EditBufMenuClick(int idx);
  void  ClearClipBoardMenuItemClick();
  void  Font1Click();
  void  Color1Click();
  void  ChangeColor();
  void  StringFonCMenuItemClick();
  void  PointerFonCMenuItemClick();
  void  HexFonCMenuItemClick();
  void  StringColorMenuItemClick();
  void  PointerColorMenuItemClick();
  void  HexColorMenuItemClick();
  //        void  SetOptionButtonClick(TObject *Sender);

  void  StringRichEditMouseMove(TObject *Sender,TShiftState Shift, int X, int Y);
  void  HexRichEditMouseMove(TObject *Sender,TShiftState Shift, int X, int Y);

  void  ContinueSearchMenuItemClick();
  void  SlowSearchMenuItemClick();
  void  ReplaceAllMenuItemClick();
private:
  void PointersNotifyEvent(int);
  void UpdateInfoString(void);
  void initializePopupMenu();
  void initialize(void);
  void  StringsOptionChangedEvent();
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

  void ShiftIfWrongHexSelStart();
  int  ConvertGlobalToHexPos(int value);
  int FindInStream(char* buf, int len);
  void  DoProgress(int pos);
  void  GetVariablesAtPos(int);
  //        TMemoryStream*  GetFindStream();
  //TMemoryStream*  GetReplaceStream();
  //        TList*  GetFindPointers();

public:		// User declarations
  TStreamEdit(CWnd* pParent = NULL);
  virtual ~TStreamEdit(void);
  void  ShiftHexSelStart(int Shift);
  long  ConvertGlobalToStringPos(long value);
  long  ConvertStringPosToGlobal(long value);
  void  ShiftStringSelStart(int value);

  void  ShiftIfWrongStringSelStart(void);
  bool  Search(bool IsNewSearch);
  boost::shared_ptr<TSearcher>  GetSearcher(void);
  bool  SlowSearch(void);

  int getCurrentPos() const { return FCurrentPos;}
  void setCurrentPos(int i_value);

  boost::shared_ptr<std::iostream>  getStream(){ return Fstream;}
  void setStream(boost::shared_ptr<std::iostream>);

  std::string    getKeyPressed() const {return FKeyPressed;}
  void   setKeyPressed(const std::string& i_value );

  std::string    getCurrectEditSelection() const {return FCurrectEditSelection;}
  void   setCurrectEditSelection(const std::string& i_value );

  void processKey(INT nKey,CRichEditCtrl* pListBox,UINT nIndex);
protected:
  //Windows events
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  //void  KeyDown(TObject *Sender,WORD &Key, TShiftState Shift);

  afx_msg void OnInitMenu(CMenu* pMenu);
  afx_msg void OnInitMenuPopup(CMenu* pPopupMenu,UINT nIndex,BOOL bSysMenu);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnSize(UINT, int cx,int cy);
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,AFX_CMDHANDLERINFO* pHandlerInfo);
  virtual BOOL OnInitDialog();
  DECLARE_MESSAGE_MAP()
};
//---------------------------------------------------------------------------
#endif

