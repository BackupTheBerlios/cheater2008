//---------------------------------------------------------------------------

#include "stdafx.h"
#pragma hdrstop

#include <list>
#include <vector>
#include <sstream>
#include <fstream>
#include <fstream>
#include <stdexcept>
#include <iomanip>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

#include <map>
#include <vector>

#include "MessageReporterBase.h"
#include "StreamEdit.h"

int NumberOfCopies=0;
char* WhatFind="Search query";
std::vector< boost::shared_ptr<std::stringstream> > ClipBoard;     // clipboard for selections

inline int getHexEditLineLength(int i_bytes)
{
  return 3*i_bytes;
}
class WindowsMessageHandler
{
private:

  struct cmp1 {
    bool operator()( HWND s1, HWND s2 ) const {
      return (LONG)s1 < (LONG)s2;
    }
  };

  struct cmp2 {
    bool operator()( TStreamEdit* s1, TStreamEdit* s2 ) const {
      return (LONG)s1 < (LONG)s2;
    }
  };

  std::map<HWND,WNDPROC_TYPE,cmp1> d_wndToOldProc;
  std::map<HWND,TStreamEdit*,cmp1> d_wndToParentMap;
  std::map<TStreamEdit*,HWND,cmp2> d_ParentToWndMap;
public:

  WindowsMessageHandler()
  {

  }
  static WindowsMessageHandler& getWindowsMessageHandler()
  {
    static WindowsMessageHandler handler;
    return handler;
  }

  ~WindowsMessageHandler()
  {
    revert();
  }

  void revert ()
  {
    typedef std::pair<HWND,WNDPROC_TYPE> wndProcPair;
    BOOST_FOREACH(wndProcPair pair,d_wndToOldProc)
    {
      CWnd* wnd = CWnd::FromHandle((pair.first));
      if(wnd)
        replaceWndProc(*wnd,pair.second);
    }
    d_wndToOldProc.clear();
    d_wndToParentMap.clear();
    d_ParentToWndMap.clear();

  }

  static LRESULT CALLBACK myWndProc(HWND hWnd, UINT event, WPARAM wParam, LPARAM lParam)
  {
    WindowsMessageHandler& handler = getWindowsMessageHandler();
    CWnd& child = getCWnd(hWnd);

    std::stringstream msg;
    msg << " child [" << std::string( CT2CA(CString(child.GetRuntimeClass()->m_lpszClassName)) ) << "]got " << " ["<< event <<"]<< event\r\n";
    bool processed = false;
    if (event==WM_RBUTTONDBLCLK) 
      msg << "Right button dblck\r\n";
    else if (event==WM_RBUTTONDOWN) 
    {
      msg << "Right button down\r\n";
      TStreamEdit* parent = handler.d_wndToParentMap[hWnd];
      /*
      CPoint pt(lParam);

      CWnd::FromHandle(hWnd)->MapWindowPoints(parent,&pt,1);
      lParam=MAKEWORD(pt.y,pt.x);
      */
      parent->SendMessage(event,wParam,lParam);
    }
    else if ( (event==WM_KEYDOWN)  ) 
    {
      processed = true;
      msg << "WM_KEYDOWN\r\n";
      TStreamEdit* parent = handler.d_wndToParentMap[hWnd];
      INT nKey = (INT) wParam;
      UINT nIndex = (UINT) lParam;
      CRichEditCtrl*  pListBox = dynamic_cast<CRichEditCtrl* >( &child) ;
      parent->processKey(nKey,pListBox,nIndex);

      /*
      CPoint pt(lParam);

      CWnd::FromHandle(hWnd)->MapWindowPoints(parent,&pt,1);
      lParam=MAKEWORD(pt.y,pt.x);
      */
    }
    else if ((event==WM_CHAR) ) 
    {
      processed = true;
      msg << "WM_CHAR\r\n";

      /*
      CPoint pt(lParam);

      CWnd::FromHandle(hWnd)->MapWindowPoints(parent,&pt,1);
      lParam=MAKEWORD(pt.y,pt.x);
      */
      //parent->SendMessage(event,wParam,lParam);
    }
    ::OutputDebugString(CString(msg.str().c_str()));
    WNDPROC_TYPE old = handler.d_wndToOldProc[hWnd];
    if(! processed)
      return old(hWnd,event,wParam,lParam);
    return 0; // message processed
  }

  void append(CWnd* wnd, TStreamEdit* parent)
  {
    if(! wnd->m_hWnd)
      throw std::runtime_error("Corrupted child window");
    if(! parent->m_hWnd)
      throw std::runtime_error("Corrupted parent window");

    std::map<HWND,WNDPROC_TYPE,cmp1>::iterator it = d_wndToOldProc.find(wnd->m_hWnd);
    if(it!=d_wndToOldProc.end())
      throw std::runtime_error("Can't redefine windows procedure twice");


    d_wndToParentMap.insert (std::pair<HWND,TStreamEdit*>(wnd->m_hWnd,parent) );
    d_ParentToWndMap.insert ( std::pair<TStreamEdit*,HWND>(parent,wnd->m_hWnd) );
    WNDPROC_TYPE old = replaceWndProc(*wnd,myWndProc);
    d_wndToOldProc.insert( std::pair<HWND,WNDPROC_TYPE>(wnd->m_hWnd,old) );
  }


};

enum
{
  ProgressBar_ID = 0xF000,
  InfoEdit_ID,
  StringMemo_ID,
  PointerMemo_ID,
  HexMemo_ID,
  SplitterWnd_ID,
  SplitterWndTop_ID,
  SplitterWndBottom_ID,
  SplitterWndTopLeft_ID,
  SplitterWndTopRight_ID
};

IMPLEMENT_DYNAMIC(TStreamEdit, CMyBaseForm)

BEGIN_MESSAGE_MAP(TStreamEdit, CMyBaseForm)
  ON_WM_KEYDOWN()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_INITMENU()
  ON_WM_INITMENUPOPUP()
  ON_WM_VSCROLL()
  ON_WM_SIZE()
  ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()
void append(CRichEditCtrl& edit, const std::string& str)
{

  int lines = edit.GetLineCount();
  if(lines<=0)
  {
    edit.SetWindowText(CString(str.c_str()));
  }
  {
    int nBegin;
    // Replace the second line, if it exists, of the edit control
    // with the text lpszmyString.
    if ((nBegin=edit.LineIndex(lines-1)) != -1)
    {
      nBegin = nBegin + edit.LineLength(nBegin); 
      edit.SetSel(nBegin+1, nBegin+1);
      edit.ReplaceSel(CString(std::string("\r\n"+str).c_str()));
    }
  }

}
//---------------------------------------------------------------------------
TStreamEdit::TStreamEdit(CWnd* pParent /*= NULL*/):
CMyBaseForm(pParent),
OpenDialog(TRUE),
SaveDialog(FALSE)
{

  if(NumberOfCopies==0)
  {
    //  ClipBoard=new TList();
  }
  d_stream=boost::shared_ptr<std::iostream>((std::iostream*)0);
  d_streamPos=0;
  d_viewedLen=0;
  d_numberOfString=20;
  d_stringLen=16;
  IsProcessKeyDown=false;

  NumberOfCopies++;
}

void TStreamEdit::appendMenuItem(TPopupMenu& menu,
								 CommandsContainer& commandsContainer,
								 TStreamEditMethod method,
								 const std::string& i_caption)
{
	menu.AppendMenu(MF_STRING, 
		commandsContainer.createCommand( 
		CommandPtr(new Command(boost::bind(method,this)) )), 
		CString( i_caption.c_str()) );
}

//---------------------------------------------------------------------------
void TStreamEdit::initializePopupMenu()
{
  //-------------------Upper all OK--------------------------------------
  d_popupMenu.CreatePopupMenu();
  //0
  appendMenuItem(d_popupMenu, d_menuCommands,
	  &TStreamEdit::SlowSearchMenuItemClick,"Slow Search");
  
  //1
  appendMenuItem(d_popupMenu, d_menuCommands,
	  &TStreamEdit::SearchMenuItemClick,"Search");

  //2
  appendMenuItem(d_popupMenu, d_menuCommands,
    &TStreamEdit::ContinueSearchMenuItemClick,"Continue Search");

  //3
  appendMenuItem(d_popupMenu, d_menuCommands,
	  &TStreamEdit::LoadFromStreamMenuitemClick,"ReLoad");

  //4
  appendMenuItem(d_popupMenu, d_menuCommands,
    &TStreamEdit::GotoMenuItemClick,"Goto...");

  //5
  d_popupMenu.AppendMenu(MF_SEPARATOR,0,CString("-") );

  //6
  appendMenuItem(d_popupMenu, d_menuCommands,
    &TStreamEdit::CopyMenuItemClick,"Copy");

  //7
  appendMenuItem(d_popupMenu, d_menuCommands,
    &TStreamEdit::PasteMenuItemClick,"Paste");

  //8
  appendMenuItem(d_popupMenu, d_menuCommands,
    &TStreamEdit::PasteMenuItemClick,"Paste");


  //9
  d_popupMenu.AppendMenu(MF_SEPARATOR,0,CString("-"));

  d_copytoMenuItem.CreatePopupMenu();

  //10
  d_popupMenu.AppendMenu(MF_POPUP, (UINT_PTR)d_copytoMenuItem.m_hMenu, CString( "Coptyto..." ) ); //6


  d_pasteFromMenuItem.CreatePopupMenu();
  //11
  d_popupMenu.AppendMenu(MF_POPUP, (UINT_PTR)d_pasteFromMenuItem.m_hMenu, CString( "Paste from" ) ); //6

  EditBufMenuItem.CreatePopupMenu();
  //12
  d_popupMenu.AppendMenu(MF_POPUP, (UINT_PTR)EditBufMenuItem.m_hMenu, CString( "Edit buf" ) ); //6


  DeleteMenuItem.CreatePopupMenu();
  //13
  d_popupMenu.AppendMenu(MF_POPUP, (UINT_PTR)DeleteMenuItem.m_hMenu, CString( "Delete" ) ); //6

  //14
  appendMenuItem(d_popupMenu, d_menuCommands,
    &TStreamEdit::ClearClipBoardMenuItemClick,"Clear ClipBoard");

  //15
  d_popupMenu.AppendMenu(MF_SEPARATOR,0,CString("-"));

  //16
  appendMenuItem(d_popupMenu, d_menuCommands,
    &TStreamEdit::Color1Click,"Color");

  //17
  appendMenuItem(d_popupMenu, d_menuCommands,
    &TStreamEdit::Font1Click,"Font");

  FonCMenuItem.CreatePopupMenu();
  //18
  d_popupMenu.AppendMenu(MF_POPUP, (UINT_PTR)FonCMenuItem.m_hMenu, CString( "Font" ) ); //6

  //--------------------------------------------------

  appendMenuItem(FonCMenuItem, d_menuCommands,
    &TStreamEdit::StringFonCMenuItemClick,"String");

  appendMenuItem(FonCMenuItem, d_menuCommands,
    &TStreamEdit::HexFonCMenuItemClick,"Hex");

  appendMenuItem(FonCMenuItem, d_menuCommands,
    &TStreamEdit::PointerFonCMenuItemClick,"Pointer");

  //--------------------------------------------------

  ColorMenuItem.CreatePopupMenu();
  //19
  d_popupMenu.AppendMenu(MF_POPUP, (UINT_PTR)ColorMenuItem.m_hMenu, CString( "Color" ) ); //6


  appendMenuItem(ColorMenuItem, d_menuCommands,
    &TStreamEdit::StringColorMenuItemClick,"String");

  appendMenuItem(ColorMenuItem, d_menuCommands,
    &TStreamEdit::HexColorMenuItemClick,"Hex");

  appendMenuItem(ColorMenuItem, d_menuCommands,
    &TStreamEdit::PointerColorMenuItemClick,"Pointer");

  //--------------------------------------------------
  //20
  d_popupMenu.AppendMenu(MF_SEPARATOR,0,CString("-") );
  //---------------------------------------
  //21

  appendMenuItem(d_popupMenu, d_menuCommands,
    &TStreamEdit::ReplaceAllMenuItemClick,"Replace All");

  //------------------------------------------------------

}
void  TStreamEdit::initialize(void)
{
  CRect clientRect;
  this->GetClientRect(&clientRect);

  d_splitterWnd.Create(this, WS_CHILD|WS_VISIBLE, clientRect, SplitterWnd_ID);
  d_splitterWndTop.Create(&d_splitterWnd, WS_CHILD|WS_VISIBLE, CRect(0,0,30,30), SplitterWndTop_ID);
  d_splitterWndBottom.Create(&d_splitterWnd, WS_CHILD|WS_VISIBLE, CRect(0,0,30,30), SplitterWndBottom_ID);
  d_splitterWndTopLeft.Create(&d_splitterWndTop, WS_CHILD|WS_VISIBLE, CRect(0,0,30,30), SplitterWndTopLeft_ID);
  d_splitterWndTopRight.Create(&d_splitterWndTop, WS_CHILD|WS_VISIBLE, CRect(0,0,30,30), SplitterWndTopRight_ID);

  d_splitterWndTopRightBox.Create(CString(""),SS_SIMPLE,CRect(0,0,0,0),&d_splitterWndTopRight);


  initializePopupMenu();
  //-------------- Bottom pane --------------------------------
  ProgressBar.Create(PBS_SMOOTH , CRect(0,0,0,0), &d_splitterWndBottom, ProgressBar_ID);


  InfoEdit.Create(ES_MULTILINE | ES_WANTRETURN,CRect(0,0,0,0),&d_splitterWndBottom,InfoEdit_ID);


  d_splitterWndBottom.AddRow();
  d_splitterWndBottom.AddColumn();
  d_splitterWndBottom.AddColumn();
  d_splitterWndBottom.SetWindow(0,0,InfoEdit.m_hWnd);
  d_splitterWndBottom.SetWindow(0,1,ProgressBar.m_hWnd);
  d_splitterWndBottom.Update();


  //--------------Top right panel-----------------------------------
  CRect GotoInputGroupBoxRect(0,0,200,100);
  GotoInputGroupBox.Create(&d_splitterWndTopRightBox);
  GotoInputGroupBox.setCaption("Goto..");
  GotoInputGroupBox.MoveWindow(&GotoInputGroupBoxRect);

  CRect StringNumberEditRect(GotoInputGroupBoxRect);
  StringNumberEditRect.MoveToY(StringNumberEditRect.bottom +5);
  StringNumberEditRect.bottom = StringNumberEditRect.top + 20;

  StringNumberEdit.Create(&d_splitterWndTopRightBox);
  StringNumberEdit.SetWindowPos ( 0, StringNumberEditRect.left, StringNumberEditRect.top, StringNumberEditRect.Width(), StringNumberEditRect.Height(), SWP_NOZORDER | SWP_NOSIZE);
  StringNumberEdit.setText("Number of lines");
  StringNumberEdit.OnChangeEvent = CommandPtr(new Command(boost::bind(&TStreamEdit::StringsOptionChangedEvent,this) )); 


  StringLengthEdit.Create(&d_splitterWndTopRightBox);
  StringLengthEdit.OnChangeEvent = CommandPtr(new Command(boost::bind(&TStreamEdit::StringsOptionChangedEvent,this) )); 
  StringLengthEdit.SetWindowPos ( 0, StringNumberEditRect.right, StringNumberEditRect.top, StringNumberEditRect.Width(), StringNumberEditRect.Height(), SWP_NOZORDER | SWP_NOSIZE);
  StringLengthEdit.setText("Line's length");


  SearcherProperties.Create(&d_splitterWndTopRight);


  d_splitterWndTopRight.AddRow();
  d_splitterWndTopRight.AddRow();
  d_splitterWndTopRight.AddColumn();

  d_splitterWndTopRight.SetWindow(0,0,d_splitterWndTopRightBox.m_hWnd);
  d_splitterWndTopRight.SetWindow(1,0,SearcherProperties.m_hWnd);
  d_splitterWndTopRight.Update();

  //--------------Top left panel-----------------------------------
  // throwException("Sergey3");
  d_pointerMemoFont = boost::shared_ptr<CFont>(new CFont());
  d_stringMemoFont = boost::shared_ptr<CFont>(new CFont());
  d_hexMemoFont = boost::shared_ptr<CFont>(new CFont());
  d_pointerMemoFont->CreatePointFont(10,_T("Fixedsys"));
  d_stringMemoFont->CreatePointFont(10,_T("Fixedsys"));
  d_hexMemoFont->CreatePointFont(10,_T("Fixedsys"));

  d_stringMemo.Create(ES_MULTILINE | ES_WANTRETURN,CRect(0,0,0,0),&d_splitterWndTopLeft,StringMemo_ID);
  d_stringMemo.SetFont( d_stringMemoFont.get() );
#pragma message ("TODO: implement event handlers")
  //   StringMemo.OnKeyDown=StringRichEditKeyDown;
  //   StringMemo.OnKeyPress=StringRichEditKeyPress;
  // StringMemo.OnMouseMove=StringRichEditMouseMove;


  d_pointerMemo.Create(ES_MULTILINE | ES_WANTRETURN,CRect(0,0,0,0),&d_splitterWndTopLeft,PointerMemo_ID);
  d_pointerMemo.SetFont( d_pointerMemoFont.get() );

  d_hexMemo.Create(ES_MULTILINE| ES_WANTRETURN,CRect(0,0,0,0),&d_splitterWndTopLeft,HexMemo_ID);
  d_hexMemo.SetFont( d_hexMemoFont.get() );

#pragma message ("TODO: implement event handlers")
  //   HexMemo.OnKeyDown=HexRichEditKeyDown;
  //   HexMemo.OnKeyPress=HexRichEditKeyPress;
  //   HexMemo.OnMouseMove=HexRichEditMouseMove;

  d_splitterWndTopLeft.AddRow();
  d_splitterWndTopLeft.AddColumn();
  d_splitterWndTopLeft.AddColumn();
  d_splitterWndTopLeft.AddColumn();

  d_splitterWndTopLeft.SetWindow(0,0,d_pointerMemo.m_hWnd);
  d_splitterWndTopLeft.SetWindow(0,1,d_hexMemo.m_hWnd);
  d_splitterWndTopLeft.SetWindow(0,2,d_stringMemo.m_hWnd);

  //-------------------- Joining Top panes -----------------------
  d_splitterWndTop.AddRow();
  d_splitterWndTop.AddColumn();
  d_splitterWndTop.AddColumn();
  d_splitterWndTop.SetWindow(0,0,d_splitterWndTopLeft.m_hWnd);
  d_splitterWndTop.SetWindow(0,1,d_splitterWndTopRight.m_hWnd);

  d_splitterWnd.AddRow();
  d_splitterWnd.AddRow();
  d_splitterWnd.AddColumn();

  d_splitterWnd.SetWindow(0,0,d_splitterWndTop.m_hWnd);
  d_splitterWnd.SetWindow(1,0,d_splitterWndBottom.m_hWnd);

  //--------------- Joining Top and bottom ---------------------

  ProgressBar.ShowWindow( SW_SHOW );
  InfoEdit.ShowWindow( SW_SHOW );
  GotoInputGroupBox.ShowWindow(SW_SHOW);
  StringNumberEdit.ShowWindow(SW_SHOW);
  StringLengthEdit.ShowWindow(SW_SHOW);
  SearcherProperties.ShowWindow(SW_SHOW);
  d_pointerMemo.ShowWindow(SW_SHOW);
  d_hexMemo.ShowWindow(SW_SHOW);
  d_stringMemo.ShowWindow(SW_SHOW);

  d_splitterWndTopRightBox.ShowWindow(SW_SHOW);

  d_splitterWnd.ShowWindow(SW_SHOW);
  d_splitterWndTop.ShowWindow(SW_SHOW);
  d_splitterWndBottom.ShowWindow(SW_SHOW);

  d_splitterWndTopLeft.ShowWindow(SW_SHOW);
  d_splitterWndTopRight.ShowWindow(SW_SHOW);


  d_splitterWnd.SetResizeMode(SplitterCtrl_NS::SPLITTERCTRL_RESIZE_DYNAMIC);
  d_splitterWndTop.SetMinHeight(200);
  d_splitterWndBottom.SetMinHeight(70);
  d_splitterWnd.SetHeightForDynamic(0,80);
  d_splitterWnd.SetHeightForDynamic(1,20);
  d_splitterWnd.SetHeightForDynamic(0,80);

  d_splitterWndTop.SetWidthForStatic(0,800);
  d_splitterWndTop.SetWidthForStatic(1,100);
  d_splitterWndTopLeft.SetResizeMode(SplitterCtrl_NS::SPLITTERCTRL_RESIZE_STATIC);
  d_splitterWndTopLeft.SetMinWidth(100);
  d_splitterWndTopLeft.SetWidthForStatic(0,100);
  d_splitterWndTopLeft.SetWidthForStatic(1,500);
  d_splitterWndTopLeft.SetWidthForStatic(2,200);

  d_splitterWnd.Update();
  d_splitterWndTop.Update();
  d_splitterWndBottom.Update();

  d_splitterWndTopLeft.Update();
  d_splitterWndTopRight.Update();

  SearcherProperties.setOnSelectPointer( TPointerSelectEventPtr (new TPointerSelectEvent(boost::bind(&TStreamEdit::PointersNotifyEvent,this,_1)) ) );

  WindowsMessageHandler::getWindowsMessageHandler().append(&d_hexMemo,this);
  WindowsMessageHandler::getWindowsMessageHandler().append(&d_stringMemo,this);
  WindowsMessageHandler::getWindowsMessageHandler().append(&d_pointerMemo,this);
}

//---------------------------------------------------------------------------
void  TStreamEdit::SearchMenuItemClick()
{
  Search(true);
}
//---------------------------------------------------------------------------
void  TStreamEdit::SlowSearchMenuItemClick()
{
  SlowSearch();
}
//---------------------------------------------------------------------------
TStreamEdit::~TStreamEdit(void)
{
  //delete MemoryStream;
  NumberOfCopies--;
  if(NumberOfCopies==0)
  {
    /*  for(int i=0;i<ClipBoard->Count;i++)
    {
    delete (TMemoryStream*)ClipBoard->Items[i];
    }
    delete ClipBoard;
    */  
  }
}

void  TStreamEdit::LoadFromStream(void)
{
  d_viewedLen=0;
  int RealRead=d_stringLen;
  char* ch=new char[d_stringLen];
  long HexMemoSelStart,HexMemoSelEnd;
  long StringMemoSelStart,StringMemoSelEnd;
  d_hexMemo.GetSel(HexMemoSelStart,HexMemoSelEnd);
  d_stringMemo.GetSel(StringMemoSelStart,StringMemoSelEnd);

  d_hexMemo.SetWindowText(CString(""));
  d_stringMemo.SetWindowText(CString(""));
  d_pointerMemo.SetWindowText(CString(""));

  try
  {
    if(getStream())
    {
      //
      std::string streamName;
      if(dynamic_cast<std::ifstream*>(getStream().get()))
        streamName = "std::ifstream";
      else if(dynamic_cast<std::stringstream*>(getStream().get()))
        streamName = "std::stringstream";
      else
        streamName = "ProcessStream";

      std::stringstream caption;
      caption << "StreamObject "
        << streamName
        << ". Edit field size : "
        << std::setfill('0') << std::setw(8) << getStream()->seekg(0,std::ios_base::end).tellg()
        << " (hex), (dec) - " << getStream()->seekg(0,std::ios_base::end).tellg();
      //GroupBox1->Caption = caption.str().c_str();
      getStream()->seekg(d_streamPos,std::ios_base::beg);

      std::stringstream pointerStream;
      std::stringstream hexStream;
      std::stringstream stringStream;
      for(int i=0;(i<d_numberOfString)&&(RealRead!=0);i++)
      {
        if((RealRead=getStream()->read(ch,d_stringLen).gcount())>0)
        {
          /*if(RealRead < StringLen)
          getStream()->clear();
          */
          unsigned long streamPos = (unsigned long)getStream()->seekg(0,std::ios_base::cur).tellg();
          pointerStream << ulongTo8digitHexString(streamPos-RealRead) << "\r\n";
          hexStream << charptrToHexAnsiWithSpace(ch,RealRead)<< "\r\n";
          stringStream << ConvertToPrintString(ch,RealRead)<< "\r\n";
          d_viewedLen+=RealRead;
        }
        else
          if(i==0)
            if(getStream()->tellg()>d_streamPos) d_streamPos=getStream()->tellg();
      }
      d_hexMemo.SetWindowText(CString(hexStream.str().c_str()));
      d_stringMemo.SetWindowText(CString(stringStream.str().c_str()));
      d_pointerMemo.SetWindowText(CString(pointerStream.str().c_str()));
    }
  }
  catch(std::exception& e)
  {
    getReporter().showMessage(e.what(),MessageReporterBase::TYPE_EXCEPTION);
  }
  delete ch;

  StringNumberEdit.SetWindowText(CString(ulongToAnsi(d_numberOfString).c_str()));
  StringLengthEdit.SetWindowText(CString(ulongToAnsi(d_stringLen).c_str()));

  d_hexMemo.SetSel(HexMemoSelStart,HexMemoSelStart);
  d_stringMemo.SetSel(StringMemoSelStart,StringMemoSelStart);
}

void  TStreamEdit::LoadFromStreamMenuitemClick()
{
  LoadFromStream();
}
//---------------------------------------------------------------------------

int  TStreamEdit::ConvertHexPosToGlobal(int value)
{
  //value=HexRichEdit->SelStart;
  int res=0;
  res+=d_stringLen*(value/(getHexEditLineLength(d_stringLen)))+(value%(getHexEditLineLength(d_stringLen)))/3;
  return res;
}

void  TStreamEdit::HexRichEditKeyDown(UINT Key)
{
  /*
  ssShift	The Shift key is held down.
  ssAlt	The Alt key is held down.
  ssCtrl	The Ctrl key is held down.
  ssLeft	The left mouse button is held down.
  ssRight	The right mouse button is held down.
  ssMiddle	The middle mouse button is held down.
  ssDouble	The mouse was double-clicked.
  */
  if((Key>=0x30)&&(Key<=0x39))       // '0'-'9'
  {
    HexRichEditKeyPres(Key-0x30);
  }
  else
  {
    if((Key>=0x41)&&(Key<=0x46))     // 'a' - 'f'
    {
      HexRichEditKeyPres(Key-0x41+0xa);
    }
    else
    {
      if(Key==0x27)//right
      {
        ShiftHexSelStart(1);
      }
      else
      {
        if(Key==0x25) //left
        {
          ShiftHexSelStart(-1);
        }
        else
        {
          if(Key==0x26)//up
          {
            ShiftHexSelStart(-d_stringLen*2);
          }
          else
          {
            if(Key==0x28)//down
            {
              ShiftHexSelStart(d_stringLen*2);
            }
            else
            {
              if(Key==0x23)//end
              {

              }
              else
              {
                if(Key==0x22)//PgDown
                {
                  ShiftHexSelStart(d_stringLen*d_numberOfString*2);
                }
                else
                {
                  if(Key==0x21)//PgUp
                  {
                    ShiftHexSelStart(-d_stringLen*d_numberOfString*2);
                  }
                  else
                  {
                    if(Key==0x24)//Home
                    {

                    }
                    else
                    {
                      if(Key==0x9) //tab
                      {
                        long hexStart,hexEnd;
                        d_hexMemo.GetSel(hexStart,hexEnd);
                        int pos=ConvertHexPosToGlobal(hexStart);
                        d_stringMemo.SetSel(ConvertGlobalToStringPos(pos),ConvertGlobalToStringPos(pos));
                      }
                      else
                      {
                        if(Key==0x1)   //??
                        {

                        }
                        else
                        {

                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  std::string _shift="";
  /*
  if(Shift.Contains(ssShift))
  _shift+=AnsiString("+Shift");

  if(Shift.Contains(ssAlt))
  _shift+=AnsiString("+Alt");

  if(Shift.Contains(ssCtrl))
  _shift+=AnsiString("+Ctrl");

  if(Shift.Contains(ssLeft))
  _shift+=AnsiString("+MouseLeft");

  if(Shift.Contains(ssRight))
  _shift+=AnsiString("+MouseRight");



  if(Shift.Contains(ssMiddle))
  _shift+=AnsiString("+MouseMiddle");

  if(Shift.Contains(ssDouble))
  _shift+=AnsiString("+MouseDobleClick");
  */
  InfoEdit.SetWindowText(  CString(std::string("Key: "+ulongToHexAnsi(Key)+" "+_shift).c_str()) );
  Key=0;
}
//---------------------------------------------------------------------------
void  TStreamEdit::GotoMenuItemClick()
{
  setCurrentPos(GotoInputGroupBox.GetPointer());
}
//---------------------------------------------------------------------------
void  TStreamEdit::HexRichEditKeyPres(unsigned char value)
{
  long hexStart,hexEnd;
  d_hexMemo.GetSel(hexStart,hexEnd);
  int pos=ConvertHexPosToGlobal(hexStart);
  int rem;
  int hexPos=ConvertGlobalToHexPos(pos);
  const char* ch;
  char print[2];
  print[1]=0;
  int StringPos=ConvertGlobalToStringPos(pos);
  d_stringMemo.SetSel(StringPos,StringPos+1);

  getStream()->clear();
  getStream()->seekp(d_streamPos+pos,std::ios_base::beg);

  d_hexMemo.SetSel(hexStart,hexStart+1);
  std::stringstream stream;
  stream << std::hex << (unsigned int)value;
  std::string str = stream.str();
  d_hexMemo.ReplaceSel(CString(str.c_str()));

  d_hexMemo.GetSel(hexStart,hexEnd);
  rem=hexStart;
  d_hexMemo.SetSel(hexPos,hexPos+2);

  CString hexSelStr = d_hexMemo.GetSelText();
  ch=HexAnsiTocharptr( std::string( CT2CA(hexSelStr) ) );
  print[0]=ConvertToPrintSign(*ch);
  d_stringMemo.ReplaceSel(CString(print));

  d_hexMemo.SetSel(rem,rem);
  d_hexMemo.SetSel(-1,0);
  getStream()->write(ch,1);
  ShiftIfWrongHexSelStart();

  delete[] ch;

  //char

}



void TStreamEdit::ShiftIfWrongHexSelStart()
{
  long hexStart, hexEnd;
  d_hexMemo.GetSel(hexStart,hexEnd);
  int pos=ConvertHexPosToGlobal(hexStart);
  if(pos==d_viewedLen-1)
  {
    hexStart--;
    hexEnd--;
    d_hexMemo.SetSel(hexStart,hexEnd);
    ShiftHexSelStart(1);
  }
  else
  {
    if(hexStart%(getHexEditLineLength(d_stringLen))==3*d_stringLen-1)
    {
      hexStart+=2;
      hexEnd+=2;
      d_hexMemo.SetSel(hexStart,hexEnd);
    }
  }

  if((hexStart%(getHexEditLineLength(d_stringLen)))%3==2)
  {
    hexStart++;
    hexEnd++;
    d_hexMemo.SetSel(hexStart,hexEnd);
  }
}

int  TStreamEdit::ConvertGlobalToHexPos(int value)
{
  //Edit2->Text=AnsiString((3*StringLen+1)*(value/StringLen)+(value%StringLen)*3);
 /* return (3*d_stringLen+1)*(value/d_stringLen)+(value%d_stringLen)*3;*/
  return getHexEditLineLength(d_stringLen)*(value/d_stringLen)+(value%d_stringLen)*3;
}

void  TStreamEdit::HexRichEditKeyPress(unsigned char Key)
{
  Key=0;
}
//---------------------------------------------------------------------------
int TStreamEdit::FindInStream(char* buf, int len)
{
  getReporter().showMessage("TStreamEdit::FindInStream not implemented yet",MessageReporterBase::TYPE_WARNING);
  //Stream->Search(buf,len);
  /*int res=0;
  int pos=0;
  int curPosition=stream->Position;
  char* work=new char[len];

  for(;res+stream->Position<stream->Size-len;res++)
  {
  pos=stream->Position;
  stream->Position+=res;
  if(stream->Read(&work,len)==len);
  if(memcmp(work,buf,len)==0)
  break;
  }
  delete work;*/
  return -1;
}

void  TStreamEdit::setCurrentPos( int value)
{
  if((value<getStream()->seekg(0,std::ios_base::end).tellg())&&(value>=0))
  {
    d_streamPos=value;
    LoadFromStream();
  }
}

void  TStreamEdit::ShiftHexSelStart(int Shift)
{
  long Start,End,Len;
  d_hexMemo.GetSel(Start,End);
  long value=Start;
  long InBytePos=(value%getHexEditLineLength(d_stringLen))%3;
  long Offset=ConvertHexPosToGlobal(value);
  long OldOffset=Offset;
  Offset+=Shift/2;
  if(Shift%2!=0)
  {
    switch(Shift%2+InBytePos)
    {
    case -1:
      Offset--;
      InBytePos=1;
      break;
    case 2:
      Offset++;
      InBytePos=0;
      break;
    default :
      InBytePos+=Shift%2;
    }
  }

  if((Offset+d_streamPos>=0)&&(Offset+d_streamPos<getStream()->seekg(0,std::ios_base::end).tellg()))
  {
    if((Offset<0)||(Offset>=d_viewedLen))
    {
      long newStreamPos = d_streamPos + (((long)Shift/(long)(2*d_stringLen)))*d_stringLen+Shift%2;
      setCurrentPos( newStreamPos );
      Offset = OldOffset;
    }
    d_hexMemo.SetSel(ConvertGlobalToHexPos(Offset)+InBytePos,ConvertGlobalToHexPos(Offset)+InBytePos);
  }
  GetVariablesAtPos(d_streamPos + Offset);
}

void  TStreamEdit::CopyMenuItemClick()
{
  long Start,End,Len;
  boost::shared_ptr<std::stringstream> buf=boost::shared_ptr<std::stringstream>(new std::stringstream());

  d_hexMemo.GetSel(Start,End);
  Len = Start - End;

  End=ConvertHexPosToGlobal(End);
  Start=ConvertHexPosToGlobal(Start);

  Len=End-Start+1;
  getStream()->seekg(Start+d_streamPos,std::ios_base::beg);
  //   copy
  std::vector<char> myBuf;
  myBuf.resize(Len);
  Len = getStream()->read(&(myBuf[0]),Len).gcount();
  buf->write(&(myBuf[0]),Len);
  //  Len=buf->CopyFrom(stream.get(),Len);
  ClipBoard.push_back(buf);
}
namespace
{
  void clearMenu(CMenu& io_menu,CommandsContainer& io_container)
  {
    while(io_menu.GetMenuItemCount() >0)
    {
      io_container.removeCommand(io_menu.GetMenuItemID(0));
      io_menu.DeleteMenu(0,MF_BYPOSITION);
    }
  }
}


//---------------------------------------------------------------------------
void  TStreamEdit::PopupMenu1Popup()
{
  unsigned int i;
  unsigned int len;
  CMenuItem* menu;
  std::string caption;

  clearMenu(d_copytoMenuItem,d_menuCommands);
  clearMenu(d_pasteFromMenuItem,d_menuCommands);
  clearMenu(DeleteMenuItem,d_menuCommands);
  clearMenu(EditBufMenuItem,d_menuCommands);
  clearMenu(EditBufMenuItem,d_menuCommands);

  long hexStart,hexEnd;
  d_hexMemo.GetSel(hexStart,hexEnd);
  d_popupMenu.EnableMenuItem (12,MF_BYPOSITION   |  ( (! ClipBoard.empty()) ? MF_ENABLED : MF_DISABLED |MF_GRAYED ) );
  d_popupMenu.EnableMenuItem (13,MF_BYPOSITION   | ( (! ClipBoard.empty()) ? MF_ENABLED : MF_DISABLED |MF_GRAYED) );
  d_popupMenu.EnableMenuItem (11,MF_BYPOSITION   | ( (! ClipBoard.empty()) ? MF_ENABLED : MF_DISABLED |MF_GRAYED) );

  d_popupMenu.EnableMenuItem (6,MF_BYPOSITION    | ( (hexEnd-hexStart!=0) ? MF_ENABLED : MF_DISABLED |MF_GRAYED) );
  d_popupMenu.EnableMenuItem (7,MF_BYPOSITION    | ( (! ClipBoard.empty()) ? MF_ENABLED : MF_DISABLED |MF_GRAYED) );
  d_popupMenu.EnableMenuItem (10,MF_BYPOSITION   | ( ((! ClipBoard.empty())&&(hexEnd-hexStart!=0)) ? MF_ENABLED : MF_DISABLED |MF_GRAYED) );
  d_popupMenu.EnableMenuItem (14,MF_BYPOSITION   | ( (! ClipBoard.empty()) ? MF_ENABLED : MF_DISABLED |MF_GRAYED) );

  for(i=0;i<ClipBoard.size();i++)
  {
    std::string caption = ulongToAnsi(i);

    UINT DeleteEntryMenuItemID = d_menuCommands.createCommand(CommandPtr(new Command(boost::bind(&TStreamEdit::DeleteSubMenuClick,this,(unsigned int)i)) ));
    DeleteMenuItem.AppendMenu(MF_STRING, DeleteEntryMenuItemID, CString( caption.c_str() ) );

    UINT CopytoMenuItemID = d_menuCommands.createCommand(CommandPtr(new Command(boost::bind(&TStreamEdit::CopytoMenuClick,this,(unsigned int)i)) ));
    d_copytoMenuItem.AppendMenu(MF_STRING, CopytoMenuItemID, CString( caption.c_str() ) );

    UINT PastefromMenuItemID = d_menuCommands.createCommand(CommandPtr(new Command(boost::bind(&TStreamEdit::PasteFromMenuClick,this,(unsigned int)i)) ));
    d_pasteFromMenuItem.AppendMenu(MF_STRING, PastefromMenuItemID, CString( caption.c_str() ) );

    UINT EditBufMenuItemID = d_menuCommands.createCommand(CommandPtr(new Command(boost::bind(&TStreamEdit::EditBufMenuClick,this,(unsigned int)i)) ));
    EditBufMenuItem.AppendMenu(MF_STRING, EditBufMenuItemID, CString( caption.c_str() ) );
  }
}

// read in from current position to the end
// read data transfered to out
void copyTheRest(std::ostream& out, std::istream& in,
                 std::istream::traits_type::int_type len =  std::istream::traits_type::eof())
{
  if( len == std::istream::traits_type::eof())
  {
    while (out && in)
    {
      char ch;
      in.read(&ch,1);
      out.write(&ch,1);
    }
  }
  else
  {
    std::istream::traits_type::int_type count = 0 ;
    while (out && in && (count++ < len ))
    {
      char ch;
      in.read(&ch,1);
      out.write(&ch,1);
    }
  }

}
//---------------------------------------------------------------------------
void  TStreamEdit::PasteMenuItemClick()
{
  long Start,End;
  d_hexMemo.GetSel(Start,End);
  Start=ConvertHexPosToGlobal(Start);
  // initialize current position
  getStream()->seekp(Start+d_streamPos, std::ios_base::beg);
  // set read pointer to the beginnig
  ClipBoard.back().get()->seekg(0,std::ios_base::beg);
  copyTheRest(*getStream(),*(ClipBoard.back()));
  //  stream->CopyFrom(ClipBoard.back().get(),0);
  LoadFromStream();
}
//---------------------------------------------------------------------------
void  TStreamEdit::DeleteSubMenuClick(int idx)
{
  unsigned int num = (unsigned int)idx;
  if(num < ClipBoard.size() )
    ClipBoard.erase( ClipBoard.begin()+num );
  else
  {
    std::stringstream msg;
    msg << "Can't delete element at position " << num << " while ClipBoard size is " <<  ClipBoard.size() << std::endl << std::endl
      << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__  << std::endl;
    throw std::runtime_error( msg.str() );
  }
}
//---------------------------------------------------------------------------
void  TStreamEdit::CopytoMenuClick(int idx)
{
  unsigned int num = (unsigned int)idx;
  long Start, End, Len;

  d_hexMemo.GetSel(Start,End);
  Len = Start - End;

  End=ConvertHexPosToGlobal(End);
  Start=ConvertHexPosToGlobal(Start);

  Len=End-Start+1;
  getStream()->seekg(Start+d_streamPos, std::ios_base::beg);
  if( num < ClipBoard.size() )
  {
    boost::shared_ptr<std::stringstream> memory = ((*(ClipBoard.begin()+num)));
    memory->str("");
    copyTheRest(*memory,*getStream(),Len);  // clear content
    //    memory->CopyFrom(*stream,Len);
  }
  else
  {
    std::stringstream msg;
    msg << "Can't copy to element at position " << num << " while ClipBoard size is " <<  ClipBoard.size() << std::endl << std::endl
      << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__  << std::endl;
    throw std::runtime_error( msg.str() );
  }

}
//---------------------------------------------------------------------------
void  TStreamEdit::PasteFromMenuClick(int idx)
{
  unsigned int num = (unsigned int)idx;
  long Start,End;

  d_hexMemo.GetSel(Start,End);
  End=ConvertHexPosToGlobal(End);
  Start=ConvertHexPosToGlobal(Start);

  getStream()->seekp(Start+d_streamPos,std::ios_base::beg);
  if( num < ClipBoard.size() )
  {
    (*(ClipBoard.begin() + num)).get()->seekg(0,std::ios_base::beg);
    copyTheRest(*getStream(),*(*(ClipBoard.begin() + num)));
    //    stream->CopyFrom((*(ClipBoard.begin() + num)).get(),0);
  }
  else
  {
    std::stringstream msg;
    msg << "Can't copy from element at position " << num << " while ClipBoard size is " <<  ClipBoard.size() << std::endl << std::endl
      << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__  << std::endl;
    throw std::runtime_error( msg.str() );
  }

  LoadFromStream();
}
//---------------------------------------------------------------------------
void  TStreamEdit::ClearClipBoardMenuItemClick()
{
  ClipBoard.clear();
}
//---------------------------------------------------------------------------

void  TStreamEdit::Font1Click()
{
  if(FontDialog.DoModal()==IDOK)
  {
    // Create the font using the selected font from CFontDialog.
    LOGFONT lf;
    memcpy(&lf, FontDialog.m_cf.lpLogFont, sizeof(LOGFONT));

    d_pointerMemoFont = boost::shared_ptr<CFont>(new CFont());
    d_stringMemoFont = boost::shared_ptr<CFont>(new CFont());
    d_hexMemoFont = boost::shared_ptr<CFont>(new CFont());
    VERIFY(d_pointerMemoFont->CreateFontIndirect(&lf));
    VERIFY(d_stringMemoFont->CreateFontIndirect(&lf));
    VERIFY(d_hexMemoFont->CreateFontIndirect(&lf));

    d_pointerMemo.SetFont(d_pointerMemoFont.get());
    d_stringMemo.SetFont(d_stringMemoFont.get());
    d_hexMemo.SetFont(d_hexMemoFont.get());

  }


}
//---------------------------------------------------------------------------
void  TStreamEdit::Color1Click()
{
  if(ColorDialog.DoModal()==IDOK)
  {
    getReporter().showMessage("TStreamEdit::Color1Click not implemented yet",MessageReporterBase::TYPE_WARNING);
  }

}
//---------------------------------------------------------------------------
void  TStreamEdit::ChangeColor()
{
  if(ColorDialog.DoModal()==IDOK)
  {
    getReporter().showMessage("TStreamEdit::ChangeColor not implemented yet",MessageReporterBase::TYPE_WARNING);
    //((TMemo*)Sender)->Color=ColorDialog.Color;
  }
}
//---------------------------------------------------------------------------
void  TStreamEdit::StringFonCMenuItemClick()
{
  if(FontDialog.DoModal()==IDOK)
  {
    // Create the font using the selected font from CFontDialog.
    LOGFONT lf;
    memcpy(&lf, FontDialog.m_cf.lpLogFont, sizeof(LOGFONT));

    d_stringMemoFont = boost::shared_ptr<CFont>(new CFont());
    VERIFY(d_stringMemoFont->CreateFontIndirect(&lf));

    d_stringMemo.SetFont(d_stringMemoFont.get());

  }

}
//---------------------------------------------------------------------------
void  TStreamEdit::PointerFonCMenuItemClick()
{
  if(FontDialog.DoModal()==IDOK)
  {
    // Create the font using the selected font from CFontDialog.
    LOGFONT lf;
    memcpy(&lf, FontDialog.m_cf.lpLogFont, sizeof(LOGFONT));

    d_pointerMemoFont = boost::shared_ptr<CFont>(new CFont());
    VERIFY(d_pointerMemoFont->CreateFontIndirect(&lf));

    d_pointerMemo.SetFont(d_pointerMemoFont.get());

  }

}
//---------------------------------------------------------------------------
void  TStreamEdit::HexFonCMenuItemClick()
{
  if(FontDialog.DoModal()==IDOK)
  {
    // Create the font using the selected font from CFontDialog.
    LOGFONT lf;
    memcpy(&lf, FontDialog.m_cf.lpLogFont, sizeof(LOGFONT));

    d_hexMemoFont = boost::shared_ptr<CFont>(new CFont());
    VERIFY(d_hexMemoFont->CreateFontIndirect(&lf));

    d_hexMemo.SetFont(d_hexMemoFont.get());

  }
}
//---------------------------------------------------------------------------
void  TStreamEdit::StringColorMenuItemClick()
{
  if(FontDialog.DoModal()==IDOK)
  {
    // Create the font using the selected font from CFontDialog.
    LOGFONT lf;
    memcpy(&lf, FontDialog.m_cf.lpLogFont, sizeof(LOGFONT));

    d_stringMemoFont = boost::shared_ptr<CFont>(new CFont());
    VERIFY(d_hexMemoFont->CreateFontIndirect(&lf));

    d_stringMemo.SetFont(d_stringMemoFont.get());

  }
}
//---------------------------------------------------------------------------
void  TStreamEdit::PointerColorMenuItemClick()
{
  if(ColorDialog.DoModal()==IDOK)
  {
    d_pointerMemoColor = ColorDialog.GetColor();
  }
}
//---------------------------------------------------------------------------
void  TStreamEdit::HexColorMenuItemClick()
{
  if(ColorDialog.DoModal()==IDOK)
  {
    getReporter().showMessage("Color adjustmnet not implemented yet",MessageReporterBase::TYPE_WARNING);
    d_hexMemoColor = ColorDialog.GetColor();
  }
}
//---------------------------------------------------------------------------
void  TStreamEdit::EditBufMenuClick(int idx)
{
  unsigned int num = (unsigned int)idx;
  if( num < ClipBoard.size() )
  {
    getReporter().showMessage("Buffer edit not implemented yet",MessageReporterBase::TYPE_WARNING);
    /*
    TEditMemoryForm* EditMemoryForm=new TEditMemoryForm(this);
    //BufferClipBoard.Value->Write(stream,true);
    EditMemoryForm->Width=500;
    EditMemoryForm->Height=400;
    EditMemoryForm->StreamEdit->stream=(*(ClipBoard.begin() + num));
    EditMemoryForm->Show();
    */


    /*BufferClipBoard.BeginWork();
    BufferClipBoard.SetPosition(num);
    stream->Position=0;
    BufferClipBoard.Value->Read(stream);*/
  }
  else
  {
    std::stringstream msg;
    msg << "Can't edit element at position " << num << " while ClipBoard size is " <<  ClipBoard.size() << std::endl << std::endl
      << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__  << std::endl;
    throw std::runtime_error( msg.str() );
  }

}
//---------------------------------------------------------------------------
void  TStreamEdit::StringsOptionChangedEvent()
{
  try
  {

    d_numberOfString=AnsiToulong(StringNumberEdit.getText());
    d_stringLen=AnsiToulong(StringLengthEdit.getText());
  }
  catch(...)
  {
  }
  LoadFromStream();
}
//---------------------------------------------------------------------------
long  TStreamEdit::ConvertGlobalToStringPos(long value)
{
  return (d_stringLen+2)*(value/d_stringLen)+(value%d_stringLen);
}

long  TStreamEdit::ConvertStringPosToGlobal(long value)
{
  long res=0;
  res+=d_stringLen*(value/(d_stringLen+2))+(value%(d_stringLen+2));
  return res;
}

void  TStreamEdit::StringRichEditKeyDown(UINT Key)
{
  if(Key==0x27)//right
  {
    ShiftStringSelStart(1);
  }
  else
  {
    if(Key==0x25) //left
    {
      ShiftStringSelStart(-1);
    }
    else
    {
      if(Key==0x26)//up
      {
        ShiftStringSelStart(-d_stringLen);
      }
      else
      {
        if(Key==0x28)//down
        {
          ShiftStringSelStart(d_stringLen);
        }
        else
        {
          if(Key==0x23)//end
          {

          }
          else
          {
            if(Key==0x22)//PgDown
            {
              ShiftStringSelStart(d_stringLen*d_numberOfString);
            }
            else
            {
              if(Key==0x21)//PgUp
              {
                ShiftStringSelStart(-d_stringLen*d_numberOfString);
              }
              else
              {
                if(Key==0x24)//Home
                {

                }
                else
                {
                  if(Key==0x9) //Tab
                  {
                    long stringStart, hexStart;
                    d_stringMemo.GetSel(stringStart,hexStart);
                    int pos=ConvertStringPosToGlobal(stringStart);
                    d_hexMemo.SetSel(ConvertGlobalToHexPos(pos),ConvertGlobalToHexPos(pos));
                    d_hexMemo.SetFocus();
                  }
                  else
                  {
                    if(Key==0x1)   //??
                    {

                    }
                    else
                    {
                      //                        StringRichEditKeyPres(Key);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  Key=0;
}
//---------------------------------------------------------------------------
void  TStreamEdit::ShiftStringSelStart(int Shift)
{
  long StartString, EndString;
  d_stringMemo.GetSel(StartString,EndString);
  int len = EndString - StartString;
  int Offset=ConvertStringPosToGlobal(StartString);
  int OldOffset=Offset;
  Offset+=Shift;

  if((Offset+d_streamPos>=0)&&(Offset+d_streamPos<getStream()->seekg(0,std::ios_base::end).tellg()))
  {
    if((Offset<0)||(Offset>=d_viewedLen))
    {
      d_streamPos+=((Shift/(d_stringLen)))*d_stringLen +Shift%d_stringLen;
      Offset=OldOffset;
    }
    d_stringMemo.SetSel(ConvertGlobalToStringPos(Offset), ConvertGlobalToStringPos(Offset) + len);
  }
}

void  TStreamEdit::StringRichEditKeyPres(unsigned char value)
{

  long StartString, EndString;
  d_stringMemo.GetSel(StartString,EndString);

  int pos=ConvertStringPosToGlobal(StartString);
  int hexpos=ConvertGlobalToHexPos(pos);
  d_hexMemo.SetSel(hexpos,hexpos+2);

  char ch[2];
  d_hexMemo.ReplaceSel(CString(charptrToHexAnsi((char*)&value,1).c_str()));

  ch[0]=ConvertToPrintSign(value);
  ch[1]=0;
  getStream()->clear();
  getStream()->seekp(d_streamPos+pos,std::ios_base::beg);

  d_stringMemo.SetSel(StartString,StartString+1);
  d_stringMemo.ReplaceSel(CString(ch));
  d_stringMemo.SetSel(-1,0); // remove selection

  getStream()->write(ch,1);
  ShiftIfWrongStringSelStart();
}

void  TStreamEdit::StringRichEditKeyPress(unsigned char Key)
{
  if(Key!=0x9)// not tab
    StringRichEditKeyPres(Key);
  Key=0;
}
//---------------------------------------------------------------------------

void  TStreamEdit::ShiftIfWrongStringSelStart(void)
{
  long Start, End;
  d_stringMemo.GetSel(Start,End);
  if(Start%(d_stringLen+2)==d_stringLen)
  {
    if(d_viewedLen==(Start/(d_stringLen+2)+1)*d_stringLen)
    {
      Start--;
      End--;
      d_stringMemo.SetSel(Start,End);
      ShiftStringSelStart(1);
    }
    else
    {
      Start+=2;
      End+=2;
      d_stringMemo.SetSel(Start,End);
    }

  }
}

void  TStreamEdit::StringRichEditMouseMove(TObject *Sender,
                                           TShiftState Shift, int X, int Y)
{
  long Start,End;
  d_stringMemo.GetSel(Start,End);
  Start=ConvertStringPosToGlobal(Start);
  End=ConvertStringPosToGlobal(End);
  d_hexMemo.SetSel(ConvertGlobalToHexPos(Start),
    ConvertGlobalToHexPos(Start));
}
//---------------------------------------------------------------------------

void  TStreamEdit::HexRichEditMouseMove(TObject *Sender,
                                        TShiftState Shift, int X, int Y)
{
  long Start,End;
  d_hexMemo.GetSel(Start,End);
  Start=ConvertHexPosToGlobal(Start);
  End=ConvertHexPosToGlobal(End);
  d_stringMemo.SetSel(ConvertGlobalToStringPos(Start),ConvertGlobalToStringPos(End));
}
//---------------------------------------------------------------------------
void  TStreamEdit::PointersNotifyEvent(int value)
{
  d_streamPos=value;
}
//---------------------------------------------------------------------------
void  TStreamEdit::setStream(boost::shared_ptr<std::iostream> value)
{
  d_stream=value;
  LoadFromStream();
}
//------------------------------------------------------------
void  TStreamEdit::ContinueSearchMenuItemClick()
{
  Search(false);
}

//---------------------------------------------------------------------------
/*bool  TStreamEdit::Search(bool IsNewSearch)
{
if(Searcher==NULL) throw Exception("SerError3");
TDateTime StartSearch=TDateTime::CurrentDateTime();
TDateTime EndSearch;
unsigned long len;
bool SearchResult;
VoidInt vi;
Searcher->NotifyEvent=DoProgress;
ProgressBar.Max=stream->Size;
ProgressBar.Min=0;ProgressBar.Position=0;
if(!SearcherProperties.Find->IsEmpty())
{
Searcher->Find->Clear();
SearcherProperties.Find->WriteDataToStream(Searcher->Find);
if(IsNewSearch) {ProgressBar.Max=Searcher->stream->Size;SearchResult=Searcher->StartSearch();}
else {ProgressBar.Max=Searcher->Pointers->Count;SearchResult=Searcher->ContinueSearch();}
if(SearchResult)
{
if(Searcher->Pointers->Count<200)
{
for(int i=0;i<Searcher->Pointers->Count;i++)
{
vi.v=Searcher->Pointers->Items[i];
GotoInputGroupBox.Strings->Add(ulongToHexAnsi(vi.ul));
}
}
}
}
EndSearch=TDateTime::CurrentDateTime();
ProgressBar.Position=0;

InfoEdit.Lines->Add(AnsiString("Search started at ") +StartSearch.DateTimeString()+AnsiString(" ")+AnsiString("finished at ")+EndSearch.DateTimeString()+AnsiString(" search time ")+(EndSearch-StartSearch).TimeString()+AnsiString(" Found :")+AnsiString(Searcher->Pointers->Count)+AnsiString(" matches."));
SearcherProperties.Searcher=Searcher;
return Searcher->Pointers->Count>0;
}
*/

bool  TStreamEdit::Search(bool IsNewSearch)
{
  CTime StartSearch=CTime::GetCurrentTime();
  CTime EndSearch;
  int PointersFound;
  getStream()->clear();// clear flags 
  DWORD streamSize = getStream()->seekg(0,std::ios_base::end).tellg();
  ProgressBar.SetRange32(0,100);
  ProgressBar.SetStep(1);
  ProgressBar.SetPos(0);

  PointersFound=SearcherProperties.Search(IsNewSearch,getStream(),TSearcherAfterReadNotifyPtr(
    new TSearcherAfterReadNotify(boost::bind(&TStreamEdit::DoProgress,this,_1))));
  ProgressBar.SetPos(0);
  EndSearch=CTime::GetCurrentTime();

  std::stringstream msg;
  msg << "Search started at "; 
  CString startCString = StartSearch.Format("%Y-%m-%d %H:%M:%S");
  msg <<  CT2CA(startCString) << " ";
  msg << "finished at "; 
  CString endCString = EndSearch.Format("%Y-%m-%d %H:%M:%S");
  msg << CT2CA(endCString);
  msg << " search time ";
  CTimeSpan delta = EndSearch-StartSearch;
  msg << CT2CA(delta.Format("%H:%M:%S"));
  msg << " Found :" << PointersFound << " matches.";
  append(InfoEdit, msg.str());
  return    PointersFound>0;
}

bool  TStreamEdit::SlowSearch(void)
{
  CTime StartSearch=CTime::GetCurrentTime();
  CTime EndSearch;
  int PointersFound;
  DWORD streamSize = getStream()->seekg(0,std::ios_base::end).tellg();
  ProgressBar.SetRange32(0,100);
  ProgressBar.SetStep(1);
  ProgressBar.SetPos(0);

  PointersFound=SearcherProperties.SlowSearch(getStream(),TSearcherAfterReadNotifyPtr(
    new TSearcherAfterReadNotify(boost::bind(&TStreamEdit::DoProgress,this,_1))));
  ProgressBar.SetPos(0);

  EndSearch=CTime::GetCurrentTime();
  std::stringstream msg;
  msg << "Search started at " << 
    std::string(CT2CA(StartSearch.Format("%Y%m%d %H:%M:%S"))) << " "
    "finished at " << 
    std::string(CT2CA(EndSearch.Format("%Y%m%d %H:%M:%S")))<<" search time "<<std::string(CT2CA((EndSearch-StartSearch).Format("%D %H:%M:%S"))) << 
    " Found :" << PointersFound << " matches.";
  append(InfoEdit, msg.str());
  return    PointersFound>0;
}
//----------------------------------------------
void  TStreamEdit::DoProgress(int pos)
{
  ProgressBar.SetPos(pos);
}

void  TStreamEdit::GetVariablesAtPos(int pos)
{
  WORK_ANSILIB_UNION_FOR_CONVERT buf;
  getStream()->seekg(pos,std::ios_base::beg);
  getStream()->read((char*)&buf,sizeof(buf));
  std::stringstream msg;
  msg << "char " << ((int)buf.char_) << ","
    << "byte " << ulongToAnsi(buf.byte_) << ","  
    << "ulong " << ulongToAnsi(buf._ulong) << ","
    << "int " << buf._int << ",";
  try
  {
    msg << "double " << buf.double_ << (",");

  }
  catch(...)
  {
    msg << "double " << "NAN" << (",");
  }

  try
  {

    msg << "float " << buf.float_ << (",");
  }
  catch(...)
  {
    msg << "float " << "NAN" << (",");
  }
  setCurrectEditSelection(msg.str());
  //InfoLabel=AnsiString("int(")+
}



void  TStreamEdit::ReplaceAllMenuItemClick()
{
  SearcherProperties.ReplaceAll();
}

//-------------------------------------------------------------------------
void  TStreamEdit::setKeyPressed(const std::string& i_value)
{
  FKeyPressed=i_value;
  UpdateInfoString();
}

void  TStreamEdit::setCurrectEditSelection(const std::string& i_value)
{
  FCurrectEditSelection = i_value;
  UpdateInfoString();
}

unsigned char getCharacter(UINT nKey)
{
  return nKey;
}

void TStreamEdit::processKey(INT nKey,CRichEditCtrl* pListBox,UINT nIndex)
{
  if(!pListBox)
  {
    std::stringstream msg;
    msg << "Corrupted pListBox!!!"  << std::endl << std::endl
      << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__ << std::endl;
    throw std::runtime_error( msg.str() );
  }
  unsigned char ch = getCharacter(nKey);

  if(pListBox->m_hWnd==d_hexMemo.m_hWnd)
  {
    if(ch)
      HexRichEditKeyDown(ch);
    else
      HexRichEditKeyPress(nKey);
  }
  else if(pListBox->m_hWnd==d_stringMemo.m_hWnd)
  {
    if(ch)
      StringRichEditKeyPress(ch);
    else
      StringRichEditKeyDown(nKey);
  }
  else
  {
    std::stringstream msg;
    msg << "Neither HexMemo nor StringMemo"  << std::endl << std::endl
      << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__ << std::endl;
    throw std::runtime_error( msg.str() );

  }

}

void  TStreamEdit::UpdateInfoString(void)
{
  std::stringstream msg;
  msg << FKeyPressed << FCurrectEditSelection;
  append(InfoEdit,msg.str());
}


//-------------------------------------------------------------
boost::shared_ptr<TSearcher>  TStreamEdit::GetSearcher(void)
{
  return SearcherProperties.getSearcher();
}


void TStreamEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  //  Reload(nPos);  
  CMyBaseForm::OnVScroll(nSBCode,nPos,pScrollBar);
}

void TStreamEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  // TODO: Add your message handler code here and/or call default
  /*  if(Key==0x2E) //delete key
  {
  DeleteSelectedPointers();
  Key=0;
  }
  else*/
  CMyBaseForm::OnKeyDown(nChar, nRepCnt, nFlags);
}

void TStreamEdit::OnInitMenu(CMenu* /*pMenu*/)
{
  PopupMenu1Popup();
  //CMyBaseForm::OnInitMenu(pMenu);
}

void TStreamEdit::OnInitMenuPopup(CMenu* pPopupMenu,UINT nIndex,BOOL bSysMenu)
{
  PopupMenu1Popup();
  //  CMyBaseForm::OnInitMenuPopup(pPopupMenu,nIndex,bSysMenu);
}

void TStreamEdit::OnRButtonDown(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default

  CMyBaseForm::OnRButtonDown(nFlags, point);
  CMenu* menu_bar = AfxGetMainWnd()->GetMenu();
  //boost::scoped_ptr<CMenu> myMenu(CreatePopupMenu());    
  //ASSERT(myMenu.get());
  CPoint screen(point);
  this->ClientToScreen(&screen);
  d_popupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, screen.x, 
    screen.y, this);

}

void TStreamEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  CRect pbRect;
  /*  d_pointersBox->GetWindowRect(&pbRect);
  if(pbRect.PtInRect(point) == TRUE)
  PointersBoxDblClick(nFlags,point);
  else
  */
  CMyBaseForm::OnLButtonDblClk(nFlags, point);
}

void TStreamEdit::OnSize(UINT, int cx,int cy)
{
  CRect clientRect;
  this->GetClientRect(&clientRect);
  if(d_splitterWnd)
  {
    d_splitterWnd.MoveWindow(clientRect);
  }
}

BOOL TStreamEdit::OnCmdMsg(UINT nID, int nCode, void* pExtra,AFX_CMDHANDLERINFO* pHandlerInfo)
{

  if (nCode== CN_COMMAND)
  { // pop-up menu sent CN_COMMAND

    // execute command
    if(d_menuCommands.hasCommand(nID))
      d_menuCommands.getCommand(nID)();
    return TRUE;
  }
  // If the object(s) in the extended command route don't handle
  // the command, then let the base class OnCmdMsg handle it.
  return CMyBaseForm::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL TStreamEdit::OnInitDialog()
{
  CMyBaseForm::OnInitDialog();
  initialize();
  return TRUE;
}

