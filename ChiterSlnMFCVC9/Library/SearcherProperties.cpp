//---------------------------------------------------------------------------
#include "stdafx.h"
#pragma hdrstop
#include <sstream>
#include <stdexcept>
#include <boost/bind.hpp>

#include "SearcherProperties.h"

enum TSearcherPropertiesIDS
{
 SplitterWnd_ID =  0xA000
};
IMPLEMENT_DYNAMIC(TSearcherProperties, CMyBaseForm)

BEGIN_MESSAGE_MAP(TSearcherProperties, CMyBaseForm)
  ON_WM_KEYDOWN()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_INITMENU()
  ON_WM_INITMENUPOPUP()
  ON_WM_VSCROLL()
  ON_WM_CREATE()
  ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

TSearcherProperties::TSearcherProperties(CWnd* pParent /*= NULL*/):
CMyBaseForm(pParent)
{
}
void TSearcherProperties::initialize()
{
  CRect clientRect;
  GetClientRect(&clientRect);

  d_splitterWnd = new SplitterCtrl_NS::SplitterCtrl();
  d_splitterWnd->Create(this, WS_CHILD|WS_VISIBLE, CRect(0,0,500,500), SplitterWnd_ID);

  CRect upperBoxRect(0,0,0,0);
  d_upperBox = new CStatic();
  d_upperBox->Create(CString(""),SS_LEFT ,upperBoxRect,d_splitterWnd);
  d_upperBox->ShowWindow(SW_SHOW);

  CButton* c1 = new CButton();
  CButton* c2 = new CButton();
  c1->Create(CString("C1"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON ,CRect(0,0,0,0),d_splitterWnd,2222);
  c2->Create(CString("C2"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(0,0,0,0),d_splitterWnd,3333);



  FFind=new InputGroupBox(d_upperBox);
  FFind->Create ( d_upperBox );
  //FFind->Top=13;Find->Left=5;
  FFind->setCaption( "String to Find" );
  FFind->SetWindowPos ( 0, 5, 0, 200, 100, SWP_NOZORDER  );
  FFind->ShowWindow( SW_SHOW );

  CRect FFindRect;
  FFind->GetClientRect( FFindRect );
  FFind->MapWindowPoints(d_upperBox,FFindRect);
  FReplace=new InputGroupBox(d_upperBox);
  FReplace->Create ( d_upperBox );
  CRect FReplaceRect(FFindRect);
  FReplaceRect.MoveToY(FFindRect.top+FFindRect.Height()+4);
  FReplaceRect.left = 5;
  FReplace->SetWindowPos ( 0, FReplaceRect.left, FReplaceRect.top, FReplaceRect.Width(), FReplaceRect.Height(), SWP_NOZORDER  );
  //FReplace->Top=Find->Top+Find->Height+4;Replace->Left=5;
  FReplace->setCaption("String to Replace");
  FReplace->ShowWindow( SW_SHOW );


  CRect SetFindRect(FReplaceRect);
  SetFindRect.MoveToY(SetFindRect.bottom);
  SetFindRect.left = 5;
  SetFindRect.right = SetFindRect.left + 60;
  SetFindRect.bottom = SetFindRect.top + 20;
  //SetFind->Top=Replace->Top+Replace->Height+4;SetFind->Left=10;
  SetFind=new CButton();
  SetFind->Create(CString("Set Find"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,SetFindRect,d_upperBox,d_controlCommands.createCommand(CommandPtr(new Command(boost::bind(&TSearcherProperties::SetFindClick,this) ))));
  SetFind->ShowWindow( SW_SHOW );


  CRect SetReplaceRect(SetFindRect);
  SetReplaceRect.MoveToX(SetReplaceRect.right + 5);
  SetReplaceRect.right = SetReplaceRect.right + 25;
  SetReplace=new CButton();
  SetReplace->Create(CString("Set Replace"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,SetReplaceRect,d_upperBox,d_controlCommands.createCommand(CommandPtr(new Command(boost::bind(&TSearcherProperties::SetReplaceClick,this) ))));
  SetReplace->ShowWindow( SW_SHOW );

  CRect ReloadPropertiesRect(SetReplaceRect);
  ReloadPropertiesRect.MoveToX(ReloadPropertiesRect.right + 5);
  ReloadPropertiesRect.right = ReloadPropertiesRect.right + 40;
  ReloadProperties=new CButton();
  ReloadProperties->Create(CString("Reload Properties"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,ReloadPropertiesRect,d_upperBox,d_controlCommands.createCommand(CommandPtr(new Command(boost::bind(&TSearcherProperties::ReloadClick,this) ))));
  ReloadProperties->ShowWindow( SW_SHOW );

  PageSize=new InputGroupBox(d_upperBox);
  PageSize->Create ( d_upperBox );
  PageSize->setCaption( "Page Size" );
  CRect PageSizeRect;
  PageSize->GetClientRect( PageSizeRect );
  PageSize->MapWindowPoints(this,PageSizeRect);
  PageSizeRect.MoveToY(ReloadPropertiesRect.bottom +5);
  PageSizeRect.MoveToX(5);
  PageSizeRect.left=5;
  PageSize->MoveWindow(PageSizeRect);
  PageSize->ShowWindow( SW_SHOW );

  CRect SetPageSizeRect(SetReplaceRect);
  SetPageSizeRect.top = SetReplaceRect.top;
  SetPageSizeRect.MoveToX(SetReplaceRect.right + 4);
  SetPageSizeRect.right= SetPageSizeRect.right + 20;
  SetPageSize=new CButton();
  SetPageSize->Create ( CString("Set Page Size"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,SetPageSizeRect,d_upperBox,d_controlCommands.createCommand(CommandPtr(new Command(boost::bind(&TSearcherProperties::setPageSizeClick,this) ))) );
  SetPageSize->ShowWindow( SW_SHOW );

  ReloadProperties->GetClientRect(ReloadPropertiesRect);
  ReloadProperties->MapWindowPoints(d_upperBox,ReloadPropertiesRect);
  ReloadPropertiesRect.MoveToX(ReloadPropertiesRect.right+5);
  ReloadProperties->MoveWindow(ReloadPropertiesRect);


  Pointers=new TPointersViewBox(d_splitterWnd);
  Pointers->Create( d_splitterWnd );
  Pointers->ShowWindow(SW_SHOW);

  d_splitterWnd->AddRow();
  d_splitterWnd->AddRow();
  d_splitterWnd->AddColumn();
  d_splitterWnd->SetWindow(0,0,d_upperBox->m_hWnd);
  d_splitterWnd->SetWindow(1,0,Pointers->m_hWnd);

  //d_splitterWnd->SetWindow(0,0,c1->m_hWnd);
  //d_splitterWnd->SetWindow(1,0,c2->m_hWnd);
  
  d_splitterWnd->Update();

  d_splitterWnd->ShowWindow(SW_SHOW);


}

void  TSearcherProperties::setSearcher(boost::shared_ptr<TSearcher> value)
{
  FSearcher=value;
  if(FSearcher)
    Pointers->SetList(FSearcher->getPointers());
  else
    Pointers->SetList(boost::shared_ptr< std::vector<PointerType> >((std::vector<PointerType>*)0));
  Reload();
}

boost::shared_ptr<TSearcher>  TSearcherProperties::getSearcher(void)
{

  if(!FSearcher) FSearcher=boost::shared_ptr<TSearcher>(new TSearcher(boost::shared_ptr<std::iostream>((std::iostream*)0)));
  return FSearcher;

}

void  TSearcherProperties::Reload(void)
{
  if((FSearcher!=NULL))
  { FFind->InitBox(ConvertStreamToHexAnsi(FSearcher->getFind()),HEX_STRING);
  FReplace->InitBox(ConvertStreamToHexAnsi(FSearcher->getReplace()),HEX_STRING);
  Pointers->SetList(FSearcher->getPointers());
  PageSize->InitBox(IntToString(FSearcher->getPageSize()),DEC_NUM);
  }
}

void  TSearcherProperties::SetFindClick()
{
  if(FSearcher)
  {
    FSearcher->getFind().str("");
    FFind->writeDataToStream(FSearcher->getFind());
  }
}

void  TSearcherProperties::SetReplaceClick()
{
  if(FSearcher)
  {
    FSearcher->getReplace().str("");
    FReplace->writeDataToStream(FSearcher->getReplace());
  }
}

void  TSearcherProperties::ReloadClick()
{
  if(FSearcher)
  {
    Pointers->SetList(FSearcher->getPointers());
    Reload();
  }
}

TSearcherProperties::~TSearcherProperties(void)
{

}
void  TSearcherProperties::setOnSelectPointer(TPointerSelectEventPtr value)
{
  Pointers->NotifyDblClick=value;
}
//----------------------------------------------------------------------------
void  TSearcherProperties::setPageSizeClick()
{
  if(getSearcher())
    getSearcher()->setPageSize(PageSize->GetPointer());
}

//----------------------------------------------------------------------------
int  TSearcherProperties::Search(bool IsNewSearch, boost::shared_ptr<std::iostream> stream,AfterReadNotify DoProgress)
{
  if(! getSearcher())
  {
    std::stringstream msg;
    msg << "Searcher is null" << std::endl << std::endl
      << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__ << std::endl;
    throw std::runtime_error( msg.str() );
  }
  CTime StartSearch=CTime::GetCurrentTime();
  CTime EndSearch;

  bool SearchResult;
  VoidInt vi;
  getSearcher()->setStream(stream);
  getSearcher()->NotifyEvent=DoProgress;
  //ProgressBar->Max=stream->Size;
  //ProgressBar->Min=0;ProgressBar->Position=0;

  if(!getFind()->IsEmpty())
  {
    getSearcher()->getFind().str("");
    getFind()->writeDataToStream(FSearcher->getFind());
    if(IsNewSearch)
    {
      //    ProgressBar->Max=Searcher->stream->Size;
      SearchResult=getSearcher()->StartSearch();
    }
    else
    {
      //    ProgressBar->Max=Searcher->Pointers->Count;
      SearchResult=getSearcher()->ContinueSearch();
    }
    if(SearchResult)
    {
      if(getSearcher()->getPointers()->size()<200)
      {
        for(unsigned int i=0;i<getSearcher()->getPointers()->size();i++)
        {
          vi.ul=(*(getSearcher()->getPointers()))[i];
          //       GotoInputGroupBox->Strings->Add(ulongToHexAnsi(vi.ul));
        }
      }
    }
  }
  EndSearch=CTime::GetCurrentTime();

  Reload();
  return getSearcher()->getPointers()->size();
}
//----------------------------------------------------------------------------
int  TSearcherProperties::SlowSearch(boost::shared_ptr<std::iostream> stream,AfterReadNotify DoProgress)
{
  if(!getSearcher())
  {
    std::stringstream msg;
    msg << "Searcher is null" << std::endl << std::endl
      << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__ << std::endl;
    throw std::runtime_error( msg.str() );
  }
  CTime StartSearch=CTime::GetCurrentTime();;
  CTime EndSearch;
  bool SearchResult;
  VoidInt vi;
  getSearcher()->setStream(stream);

  getSearcher()->NotifyEvent = DoProgress;
  //ProgressBar->Max=stream->Size;
  //ProgressBar->Min=0;ProgressBar->Position=0;

  if(!FFind->IsEmpty())
  {
    getSearcher()->getFind().str("");
    FFind->writeDataToStream(FSearcher->getFind());
    //    ProgressBar->Max=Searcher->stream->Size;
    SearchResult=getSearcher()->SlowSearch();

    if(SearchResult)
    {
      if(getSearcher()->getPointers()->size()<200)
      {
        for(unsigned int i=0;i<getSearcher()->getPointers()->size();i++)
        {
          vi.ul=(*(getSearcher()->getPointers()))[i];
          //       GotoInputGroupBox->Strings->Add(ulongToHexAnsi(vi.ul));
        }
      }
    }
  }
  EndSearch=CTime::GetCurrentTime();

  Reload();
  return getSearcher()->getPointers()->size();
}

//----------------------------------------------------------------------------
void  TSearcherProperties::ReplaceAll(void)
{
  if(!getReplace()->IsEmpty())
  {
    getSearcher()->getReplace().str("");
    FReplace->writeDataToStream(getSearcher()->getReplace());
    getSearcher()->ReplaceAll();
  }
}


void TSearcherProperties::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  // TODO: Add your message handler code here and/or call default
  CMyBaseForm::OnKeyDown(nChar, nRepCnt, nFlags);
}

void TSearcherProperties::OnInitMenu(CMenu* pMenu)
{
  CMyBaseForm::OnInitMenu(pMenu);
}

void TSearcherProperties::OnInitMenuPopup(CMenu* pPopupMenu,UINT nIndex,BOOL bSysMenu)
{
  CMyBaseForm::OnInitMenuPopup(pPopupMenu,nIndex,bSysMenu);
}

void TSearcherProperties::OnRButtonDown(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  CMyBaseForm::OnRButtonDown(nFlags,point);
}

void TSearcherProperties::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  CMyBaseForm::OnLButtonDblClk(nFlags, point);
}

int TSearcherProperties::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CMyBaseForm::OnCreate(lpCreateStruct) == -1)
    return -1;
  CWnd * parent = this->GetOwner();
  if (parent)
  {
    CRect rect;
    this->GetWindowRect(&rect);
    this->MapWindowPoints(parent, &rect);
    rect.right = rect.left + 500;
    rect.bottom = rect.top + 500;
    parent->SetWindowPos(this,rect.left,rect.top,rect.Width(),rect.Height(),SWP_NOZORDER);
  }

  initialize();
  // TODO:  Add your specialized creation code here

  return 0;
}


BOOL TSearcherProperties::OnCmdMsg(UINT nID, int nCode, void* pExtra,AFX_CMDHANDLERINFO* pHandlerInfo)
{
  if (nCode== CN_COMMAND)
  { // pop-up menu sent CN_COMMAND

    // execute command
    if(d_controlCommands.hasCommand(nID))
      d_controlCommands.getCommand(nID)();
    else
      return CMyBaseForm::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
    return TRUE;
  }
  return CMyBaseForm::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
