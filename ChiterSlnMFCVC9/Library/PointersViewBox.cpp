//---------------------------------------------------------------------------

#include "stdafx.h"
#pragma hdrstop
#include <algorithm>
#include <fstream>
#include <boost/bind.hpp>
#include "PointersViewBox.h"
#include "MyListBox.h"

std::vector< boost::shared_ptr<std::vector<PointerType> > > PoitersDB;

IMPLEMENT_DYNAMIC(TPointersViewBox, CMyBaseForm)

BEGIN_MESSAGE_MAP(TPointersViewBox, CMyBaseForm)
  ON_WM_KEYDOWN()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_INITMENU()
  ON_WM_INITMENUPOPUP()
  ON_WM_VSCROLL()
  ON_WM_CREATE()
  ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

//---------------------------------------------------------------------------
TPointersViewBox::TPointersViewBox(CWnd* pParent /*= NULL*/)
: CMyBaseForm(pParent)
{
  
  NotifyDblClick=NULL;
  FList = boost::shared_ptr< std::vector<PointerType> >((std::vector<PointerType>*)0);
//  d_pointersBox->OnKeyDown=KeyDown; ---> OnKeyDown
}
//---------------------------------------------------------------------------


void  TPointersViewBox::SetList(boost::shared_ptr<std::vector<PointerType> > value)
{
  FList=value;
  Update();
}

void  TPointersViewBox::Update()
{
  INT MinPos,MaxPos,Position;
  
  MinPos=0;
  Position=0;
  if(FList)
    if(! FList->empty())
    {
      MaxPos=FList->size()-1;
      d_scrollBar->EnableWindow(TRUE);
      d_scrollBar->SetScrollRange(MinPos, MaxPos);
      Reload(0);
      return;
    }
  MaxPos=0;
  d_scrollBar->EnableWindow(TRUE);
  d_scrollBar->SetScrollRange(MinPos, MaxPos);
  Reload(0);
}

//---------------------------------------------------------------------------


void  TPointersViewBox::PointersBoxDblClick(UINT nFlags, CPoint point)
{
  int locSel = d_pointersBox->GetCurSel();
  int globPage = d_scrollBar->GetScrollPos();
  if(NotifyDblClick!=NULL)
    if(FList)
      if(FList->size()>0)
        NotifyDblClick((int)((*FList)[locSel+globPage]));
}
//---------------------------------------------------------------------------

void  TPointersViewBox::Init(void)
{
  d_pointersBox=new MyListBox();
  CRect pbRect;
  this->GetClientRect(&pbRect);
  pbRect.DeflateRect(1,1);
  pbRect.right = pbRect.right - 15;
  //d_pointersBox->Align=alClient;
  if ( d_pointersBox->Create(LBS_EXTENDEDSEL | LBS_NOTIFY, pbRect, this, LISTBOXFILED) ==FALSE)
    throw std::runtime_error("Can't create PointersBox ");
//  d_pointersBox->OnDblClick=PointersBoxDblClick;
  CRect scRect;
  scRect.top = pbRect.top;
  scRect.bottom = pbRect.bottom;
  scRect.left = pbRect.right;
  scRect.right = scRect.left+15;
  d_scrollBar=new CScrollBar();
  if ( d_scrollBar->Create(SBS_VERT, scRect, this, SCROLLBARFILED) ==FALSE)
    throw std::runtime_error("Can't create ScrollBar");
  d_scrollBar->ShowScrollBar();
  d_scrollBar->EnableWindow(FALSE);

  PopupMenu.CreatePopupMenu();

  //d_pointersBox->SetMenu(&PopupMenu);

  UINT LoadMenuItem = d_menuCommands.createCommand( CommandPtr(new Command(boost::bind(&TPointersViewBox::LoadPointersFromFile,this) ) ) );
  PopupMenu.AppendMenu(MF_STRING, LoadMenuItem, CString( "Load" ) ); //0
  UINT SaveMenuItem = d_menuCommands.createCommand(CommandPtr(new Command(boost::bind(&TPointersViewBox::WritePointersToFile,this)) ));
  PopupMenu.AppendMenu(MF_STRING, SaveMenuItem, CString( "Save" ) ); //1

  //---------------------------------------
  PopupMenu.AppendMenu(MF_SEPARATOR, 0, CString( "-" ) ); //2
  //--------------------------------------------------
  UINT CopyAllMenuItem = d_menuCommands.createCommand(CommandPtr(new Command(boost::bind(&TPointersViewBox::CopyMenuItemClick,this)) ));
  PopupMenu.AppendMenu(MF_STRING, CopyAllMenuItem, CString( "Copy all" ) ); //3
  
  //-------------------------------------------
  UINT PasteMenuItem = d_menuCommands.createCommand(CommandPtr(new Command(boost::bind(&TPointersViewBox::PasteMenuItemClick,this)) ));
  PopupMenu.AppendMenu(MF_STRING, PasteMenuItem, CString( "Paste" ) ); //4

  //---------------------------------------
  PopupMenu.AppendMenu(MF_SEPARATOR, 0, CString( "-" ) ); //5
  //--------------------------------------------------
  CopytoMenuItem.CreatePopupMenu();
  PopupMenu.AppendMenu(MF_POPUP, (UINT_PTR)CopytoMenuItem.m_hMenu, CString( "Coptyto..." ) ); //6
  //----------------------------------------------
  PasteFromMenuItem.CreatePopupMenu();
  PopupMenu.AppendMenu(MF_POPUP, (UINT_PTR)PasteFromMenuItem.m_hMenu, CString( "Paste from" ) ); //7
  //----------------------------------------------
  DeleteMenuItem.CreatePopupMenu();
  PopupMenu.AppendMenu(MF_POPUP, (UINT_PTR)DeleteMenuItem.m_hMenu,CString( "Delete" ) ); //8
  //--------------------------------------------
  UINT ClearClipBoardMenuItem = d_menuCommands.createCommand(CommandPtr(new Command(boost::bind(&TPointersViewBox::ClearClipBoardMenuItemClick,this)) ));
  PopupMenu.AppendMenu(MF_STRING, ClearClipBoardMenuItem, CString( "Clear ClipBoard" ) ); //9

  //---------------------------------------
  PopupMenu.AppendMenu(MF_SEPARATOR, 0, CString( "-" ) ); //10
  //--------------------------------------------------
  AddPointerToLisCMenuItem.CreatePopupMenu();
  PopupMenu.AppendMenu(MF_POPUP, (UINT_PTR)AddPointerToLisCMenuItem.m_hMenu, CString( "Add Pointers To List" ) ); //11
  //--------------------------------------------------
  UINT DeletePointerMenuItem = d_menuCommands.createCommand(CommandPtr(new Command(boost::bind(&TPointersViewBox::DeletePointerMenuItemClick,this)) ));
  PopupMenu.AppendMenu(MF_STRING, DeletePointerMenuItem, CString( "Delete pointer" ) );//12

  d_pointersBox->ShowWindow( SW_SHOW );
}

void clearMenu(CMenu& io_menu,Container& io_container)
{
  while(io_menu.GetMenuItemCount() >0)
  {
    io_container.removeCommand(io_menu.GetMenuItemID(0));
    io_menu.DeleteMenu(0,MF_BYPOSITION);
  }
}

void  TPointersViewBox::PopupMenuOnPopup()
{
  unsigned int i;
  clearMenu(CopytoMenuItem,d_menuCommands);
  clearMenu(PasteFromMenuItem,d_menuCommands);
  clearMenu(DeleteMenuItem,d_menuCommands);
  clearMenu(AddPointerToLisCMenuItem,d_menuCommands);

  //DeleteMenuItem
  PopupMenu.EnableMenuItem (12,MF_BYPOSITION   | (PoitersDB.size()!=0 ? MF_ENABLED : MF_DISABLED |MF_GRAYED));
  
  //PasteFromMenuItem
  PopupMenu.EnableMenuItem (7,MF_BYPOSITION   | (PoitersDB.size()!=0 ? MF_ENABLED : MF_DISABLED |MF_GRAYED));
  
  //PasteMenuItem
  PopupMenu.EnableMenuItem (4,MF_BYPOSITION   | (PoitersDB.size()!=0 ? MF_ENABLED : MF_DISABLED |MF_GRAYED));

  //CopytoMenuItem
  bool copytoenabled = (bool)FList;
  if(copytoenabled)
    copytoenabled = ((PoitersDB.size()!=0)&&(FList->size()!=0));
  PopupMenu.EnableMenuItem (6,MF_BYPOSITION   | (copytoenabled ? MF_ENABLED : MF_DISABLED | MF_GRAYED));


  //ClearClipBoardMenuItem
  PopupMenu.EnableMenuItem (9,MF_BYPOSITION   | (PoitersDB.size()!=0 ? MF_ENABLED : MF_DISABLED |MF_GRAYED));

  //CopyMenuItem
  bool copyenabled = (bool)FList;
  if(copyenabled)
    copyenabled = (FList->size()!=0);
  PopupMenu.EnableMenuItem (3,MF_BYPOSITION   | (  copyenabled ? MF_ENABLED : MF_DISABLED |MF_GRAYED));

  //AddPointerToLisCMenuItem
  UINT PointersCount = d_pointersBox->GetCount();
  PopupMenu.EnableMenuItem (11,MF_BYPOSITION   | ( (PointersCount>0) ? MF_ENABLED : MF_DISABLED |MF_GRAYED));

  for(i=0;i<PoitersDB.size();i++)
  {
    std::string caption = ulongToAnsi(i);

    UINT DeleteEntryMenuItemID = d_menuCommands.createCommand(CommandPtr(new Command(boost::bind(&TPointersViewBox::DeleteSubMenuClick,this,(unsigned int)i)) ));
    DeleteMenuItem.AppendMenu(MF_STRING, DeleteEntryMenuItemID, CString( caption.c_str() ) );

    UINT CopyToMenuItemID = d_menuCommands.createCommand(CommandPtr(new Command(boost::bind(&TPointersViewBox::CopytoMenuClick,this,(unsigned int)i)) ));
    CopytoMenuItem.AppendMenu(MF_STRING, CopyToMenuItemID, CString( caption.c_str()  ) );

    UINT PasteFromMenuItemID = d_menuCommands.createCommand(CommandPtr(new Command(boost::bind(&TPointersViewBox::PasteFromMenuClick,this,(unsigned int)i)) ));
    PasteFromMenuItem.AppendMenu(MF_STRING, PasteFromMenuItemID, CString( caption.c_str()  ) );

    UINT AddPointerToLisMenuItemID = d_menuCommands.createCommand(CommandPtr(new Command(boost::bind(&TPointersViewBox::AddPointerToLisCMenuItemClick,this,(unsigned int)i)) ));
    AddPointerToLisCMenuItem.AppendMenu(MF_STRING, AddPointerToLisMenuItemID , CString( caption.c_str()  ) );


  }

}
void  TPointersViewBox::WritePointersToFile()
{
  if(FList)
    if(FList->size()>0)
    {
      CFileDialog fileSaveDialog(FALSE);
      if(IDOK == fileSaveDialog.DoModal())
      {
        std::string filename = std::string (  CT2CA(fileSaveDialog.GetPathName())  );
        std::ofstream file(filename.c_str());
        WriteTListToStream(*FList,file);

      }
    }
}

void  TPointersViewBox::LoadPointersFromFile()
{
  //if(FList)
  {
    CFileDialog fileOpenDialog(TRUE);
    if(IDOK == fileOpenDialog.DoModal())
    {
      std::string filename = std::string (  CT2CA(fileOpenDialog.GetPathName())  );
      boost::shared_ptr<std::ifstream> file=boost::shared_ptr<std::ifstream>(new std::ifstream(filename.c_str()));
      boost::shared_ptr< std::vector<PointerType> > list(new std::vector<PointerType>());
      ReadTListFromStream(*list,*file);
      SetList(list);
    }
  }
}


void  TPointersViewBox::CopyMenuItemClick()
{
  if(FList)
    if(FList->size()>0)
    {
      boost::shared_ptr< std::vector<PointerType> > destList = boost::shared_ptr<std::vector<PointerType> >(new std::vector<PointerType>());
      PoitersDB.push_back(destList);
      (*destList)=(*FList);
      /*for(int i=0;i<FList->size();i++)
      {
      destList->push_back((*FList)[i]);
      }*/
    }
}

void  TPointersViewBox::PasteMenuItemClick()
{
  if(FList)
    if((FList->size()>0)&&(PoitersDB.size()>0))
    {
      //FList->Clear();
      boost::shared_ptr<std::vector<PointerType> > list = boost::shared_ptr<std::vector<PointerType> >(new std::vector<PointerType>());
      std::vector<PointerType>& sourceList=*(PoitersDB[0]);
      (*list) = sourceList;
      SetList(list);
    }
}




void  TPointersViewBox::DeleteSubMenuClick(unsigned int idx)
{
  unsigned int num=idx;
  if(num < PoitersDB.size())
  {
    PoitersDB.erase(PoitersDB.begin() + num );
  }
}
//---------------------------------------------------------------------------
void  TPointersViewBox::CopytoMenuClick(unsigned int idx)
{
  unsigned int num = idx;

  if(FList)
    if((FList->size()>0)&&(PoitersDB.size()>0)&&(num<PoitersDB.size()))
    {
      boost::shared_ptr< std::vector<PointerType> > destList = PoitersDB[num];
      for(unsigned int i=0;i<FList->size();i++)
      {
        destList->push_back((*FList)[i]);
      }
    }

}
//---------------------------------------------------------------------------
void  TPointersViewBox::PasteFromMenuClick(unsigned int idx)
{
  unsigned int num = idx;

  if(FList)
    if((PoitersDB.size()>0)&&(num<PoitersDB.size()))
    {
      //FList->Clear();
      boost::shared_ptr<std::vector<PointerType> > list(new std::vector<PointerType>());
      std::vector<PointerType>& sourceList=*(PoitersDB[num]);
      *list = sourceList;
      SetList(list);
    }
}

void  TPointersViewBox::ClearClipBoardMenuItemClick()
{
  PoitersDB.clear();
}


void  TPointersViewBox::AddPointerToLisCMenuItemClick (unsigned int idx)
{
  unsigned int num = idx;

  if(FList)
    if((PoitersDB.size()>0)&&(num<PoitersDB.size()))
    {

      std::vector<PointerType>& destList=*(PoitersDB[num]);
      int itemCount = d_pointersBox->GetCount();
      for(int i=0;i<itemCount;i++)
      {
        if(d_pointersBox->GetSel( i ) > 0 )
        {
          CString str;
          d_pointersBox->GetText(i,str);
          destList.push_back((HexAnsiToulong(std::string( CT2CA(str)) )));
        }
      }
    }
}

void  TPointersViewBox::DeletePointerMenuItemClick ()
{
  DeleteSelectedPointers();
}

void  TPointersViewBox::Reload(unsigned int pos)
{
#pragma message("What is that?")
  /*if(Showing)*/
  {
    d_pointersBox->ResetContent();
//    d_pointersBox->MultiSelect=true;
    if(pos<FList->size())
    {
      CRect rect;
      d_pointersBox->GetClientRect( &rect );
      int itemsHeight = 0;

      
      unsigned int itemCount = FList->size()-pos;
      for(unsigned int i=0;i<itemCount && itemsHeight < rect.Height();i++)
      {
        int idx = d_pointersBox->AddString( CString ( ulongToHexAnsi((unsigned long)((*FList)[pos+i]),8).c_str())  );
        itemsHeight+=d_pointersBox->GetItemHeight( idx );
      }
    }
  }
}


void  TPointersViewBox::DeleteSelectedPointers(void)
{
  if(FList)
  {

    for(int i=0;i<d_pointersBox->GetCount();i++)
    {
      if(d_pointersBox->GetSel(i))
      {
        CString str;
        d_pointersBox->GetText(i,str);
        FList->erase(std::remove(FList->begin(),FList->end(),( HexAnsiToulong( std::string(CT2CA(str)) ))    ));
      }
    }

    Update();
  }

}

void TPointersViewBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  Reload(nPos);  
  CMyBaseForm::OnVScroll(nSBCode,nPos,pScrollBar);
}

void TPointersViewBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  WORD Key = nChar;
  // TODO: Add your message handler code here and/or call default
  if(Key==0x2E) //delete key
  {
    DeleteSelectedPointers();
    Key=0;
  }
  else
    CMyBaseForm::OnKeyDown(nChar, nRepCnt, nFlags);
}

void TPointersViewBox::OnInitMenu(CMenu* pMenu)
{
 PopupMenuOnPopup();
}

void TPointersViewBox::OnInitMenuPopup(CMenu* pPopupMenu,UINT nIndex,BOOL bSysMenu)
{
 PopupMenuOnPopup();
}

void TPointersViewBox::OnRButtonDown(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default

  CMyBaseForm::OnRButtonDown(nFlags, point);
  CMenu* menu_bar = AfxGetMainWnd()->GetMenu();
  //boost::scoped_ptr<CMenu> myMenu(CreatePopupMenu());    
  //ASSERT(myMenu.get());
  CPoint screen(point);
  this->ClientToScreen(&screen);
  PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, screen.x, 
    screen.y, this);

}

void TPointersViewBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  CRect pbRect;
  d_pointersBox->GetWindowRect(&pbRect);
  if(pbRect.PtInRect(point) == TRUE)
    PointersBoxDblClick(nFlags,point);
  else
    CMyBaseForm::OnLButtonDblClk(nFlags, point);
}

int TPointersViewBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CMyBaseForm::OnCreate(lpCreateStruct);
    Init();
    return 0;
}
BOOL TPointersViewBox::OnCmdMsg(UINT nID, int nCode, void* pExtra,AFX_CMDHANDLERINFO* pHandlerInfo)
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
