//---------------------------------------------------------------------------

#ifndef StreamEditH
#define StreamEditH
//---------------------------------------------------------------------------

#include "Library/InputGroupBox.h"
#include "Library/AuxiliaryTypedefs.h"
#include "Library/MyBaseForm.h"
#include "Library/searcher.h"

#include "Library/PointersViewBox.h"
#include "Library/ProcessManager.h"
#include "Library/SearcherProperties.h"
#include "Library/CountEdit.h"
#include "Library/august2002Fwd.h"
#include "Library/AuxiliaryTypedefs.h"
#include "Library/Command.h"
#include "Library/WindowProcedure.h"


#include "Standart/AnsiLib.h"

#include "SplitterCtrl/SplitterCtrl.h"

#include <boost/shared_ptr.hpp>

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

  TPopupMenu d_popupMenu;
  CMenuItem d_copytoMenuItem;
  CMenuItem d_pasteFromMenuItem;

  CMenuItem d_searchMenuItem;
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

  void PointersNotifyEvent(int);
  void UpdateInfoString();
  void initializePopupMenu();
  void initialize();
  void  StringsOptionChangedEvent();
  //        TMemoryStream* MemoryStream;
  //! Length of editable string
  unsigned int d_stringLen;
  //! Number of editable strings
  unsigned int d_numberOfString;

  //! Number of bytes being currently represented
  unsigned int d_viewedLen;
  //! Position on the beginning of  stream window being represented
  int d_streamPos;
  bool IsProcessKeyDown;
  //! Stream being edited
  boost::shared_ptr<std::iostream> d_stream;
  std::string FKeyPressed;
  std::string FCurrectEditSelection;
  void  LoadFromStream();
  int  ConvertHexPosToGlobal(int value);

  void ShiftIfWrongHexSelStart();
  int  ConvertGlobalToHexPos(int value);
  int FindInStream(char* buf, int len);
  void  DoProgress(int pos);
  void  GetVariablesAtPos(int);
  //        TMemoryStream*  GetFindStream();
  //TMemoryStream*  GetReplaceStream();
  //        TList*  GetFindPointers();

public:

  TStreamEdit(CWnd* pParent = NULL);
  virtual ~TStreamEdit();

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

  void  ShiftHexSelStart(int Shift);
  long  ConvertGlobalToStringPos(long value);
  long  ConvertStringPosToGlobal(long value);
  void  ShiftStringSelStart(int value);

  void  ShiftIfWrongStringSelStart();
  bool  Search(bool IsNewSearch);
  bool  SlowSearch();

  boost::shared_ptr<TSearcher>  GetSearcher();
  boost::shared_ptr<std::iostream>  getStream(){ return d_stream;}
  void setStream(boost::shared_ptr<std::iostream>);

  

  int getCurrentPos() const { return d_streamPos;}
  void setCurrentPos(int i_value);

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

