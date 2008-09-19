//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include <fstream>

#include "PointersViewBox.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
std::vector< boost::shared_ptr<std::vector<PointerType> > > PoitersDB;
static inline void ValidCtrCheck(TPointersViewBox *)
{
        new TPointersViewBox(NULL);
}
//---------------------------------------------------------------------------
__fastcall TPointersViewBox::TPointersViewBox(TComponent* Owner)
        : TGroupBox(Owner)
{
Init();
NotifyDblClick=NULL;
SetList(boost::shared_ptr< std::vector<PointerType> >((std::vector<PointerType>*)0));
PointersBox->OnKeyDown=KeyDown;
}
//---------------------------------------------------------------------------


void __fastcall TPointersViewBox::SetList(boost::shared_ptr<std::vector<PointerType> > value)
{
FList=value;
Update();
}

void __fastcall TPointersViewBox::Update()
{
   ScrollBar->Min=0;
   ScrollBar->Position=0;
if(FList)
  if(FList->size()>0)
   {ScrollBar->Max=FList->size()-1;ScrollBar->Enabled=true;Reload(0);return;}
   ScrollBar->Max=0;ScrollBar->Enabled=false;
   Reload(0);
}

void __fastcall TPointersViewBox::ScrollBarScroll(TObject *Sender,
      TScrollCode ScrollCode, int &ScrollPos)
{
Reload(ScrollPos);
}
//---------------------------------------------------------------------------


void __fastcall TPointersViewBox::PointersBoxDblClick(TObject *Sender)
{
//
if(NotifyDblClick!=NULL)
  if(FList)
   if(FList->size()>0)
   NotifyDblClick((int)((*FList)[PointersBox->ItemIndex+ScrollBar->Position]));
}
//---------------------------------------------------------------------------
namespace Pointersviewbox
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TPointersViewBox)};
                 RegisterComponents("Samples", classes, 0);
        }
}
//---------------------------------------------------------------------------


void __fastcall TPointersViewBox::Init(void)
{
        TMenuItem* work;
     PointersBox=new TListBox(this);
      PointersBox->Align=alClient;
      PointersBox->OnDblClick=PointersBoxDblClick;
     this->InsertControl(PointersBox);
     ScrollBar=new TScrollBar(this);
      ScrollBar->Align=alRight;
      ScrollBar->OnScroll=ScrollBarScroll;
      ScrollBar->Kind=sbVertical;
     this->InsertControl(ScrollBar);
PopupMenu=new TPopupMenu(PointersBox);
PointersBox->PopupMenu=PopupMenu;
PopupMenu->OnPopup=PopupMenuOnPopup;
PopupMenu->AutoHotkeys=maManual;
TMenuItem* menu=new TMenuItem(PopupMenu);
menu->Caption=AnsiString("Load");
menu->OnClick=LoadPointersFromFile;
PopupMenu->Items->Add(menu);
menu=new TMenuItem(PopupMenu);
menu->Caption=AnsiString("Save");
menu->OnClick=WritePointersToFile;
PopupMenu->Items->Add(menu);


//---------------------------------------
work=new TMenuItem(PopupMenu);
work->Caption=AnsiString("-");
PopupMenu->Items->Add(work);
//--------------------------------------------------
CopyMenuItem=new TMenuItem(PopupMenu);
CopyMenuItem->Caption=AnsiString("Copy All");
CopyMenuItem->OnClick=CopyMenuItemClick;
PopupMenu->Items->Add(CopyMenuItem);
//-------------------------------------------
PasteMenuItem=new TMenuItem(PopupMenu);
PasteMenuItem->Caption=AnsiString("Paste");
PasteMenuItem->OnClick=PasteMenuItemClick;
PopupMenu->Items->Add(PasteMenuItem);
//---------------------------------------
work=new TMenuItem(PopupMenu);
work->Caption=AnsiString("-");
PopupMenu->Items->Add(work);
//--------------------------------------------------
CopytoMenuItem=new TMenuItem(PopupMenu);
CopytoMenuItem->Caption=AnsiString("Copyto All");
PopupMenu->Items->Add(CopytoMenuItem);
//----------------------------------------------
PasteFromMenuItem=new TMenuItem(PopupMenu);
PasteFromMenuItem->Caption=AnsiString("Paste from");
PopupMenu->Items->Add(PasteFromMenuItem);
//----------------------------------------------
DeleteMenuItem=new TMenuItem(PopupMenu);
DeleteMenuItem->Caption=AnsiString("Delete");
PopupMenu->Items->Add(DeleteMenuItem);
//--------------------------------------------
ClearClipBoardMenuItem=new TMenuItem(PopupMenu);
ClearClipBoardMenuItem->Caption=AnsiString("Clear ClipBoard");
ClearClipBoardMenuItem->OnClick=ClearClipBoardMenuItemClick;
PopupMenu->Items->Add(ClearClipBoardMenuItem);
//---------------------------------------
work=new TMenuItem(PopupMenu);
work->Caption=AnsiString("-");
PopupMenu->Items->Add(work);
//--------------------------------------------------
AddPointerToListMenuItem=new TMenuItem(PopupMenu);
AddPointerToListMenuItem->Caption=AnsiString("Add Pointers To List");
PopupMenu->Items->Add(AddPointerToListMenuItem);
//--------------------------------------------------
DeletePointerMenuItem=new TMenuItem(PopupMenu);
DeletePointerMenuItem->Caption=AnsiString("Delete Pointer");
DeletePointerMenuItem->OnClick=DeletePointerMenuItemClick;
PopupMenu->Items->Add(DeletePointerMenuItem);
}


void __fastcall TPointersViewBox::PopupMenuOnPopup(TObject *Sender)
{
int i;
int len;
TMenuItem* menu;
AnsiString caption;
 CopytoMenuItem->Clear();
 PasteFromMenuItem->Clear();
 DeleteMenuItem->Clear();
 AddPointerToListMenuItem->Clear();
    DeleteMenuItem->Enabled=(PoitersDB.size()!=0);
    PasteFromMenuItem->Enabled=(PoitersDB.size()!=0);
    PasteMenuItem->Enabled=(PoitersDB.size()!=0);
    CopytoMenuItem->Enabled=((PoitersDB.size()!=0)&&(FList->size()!=0));
    ClearClipBoardMenuItem->Enabled=(PoitersDB.size()!=0);
    CopyMenuItem->Enabled=(FList->size()!=0);
    AddPointerToListMenuItem->Enabled=(PointersBox->Count>0);
for(i=0;i<PoitersDB.size();i++)
 {
   caption=ulongToAnsi(i).c_str();
   menu= new TMenuItem(DeleteMenuItem);
   menu->Caption=caption;
   menu->OnClick=DeleteSubMenuClick;
   DeleteMenuItem->Add(menu);

   menu= new TMenuItem(CopytoMenuItem);
   menu->Caption=caption;
   menu->OnClick=CopytoMenuClick;
   CopytoMenuItem->Add(menu);

   menu= new TMenuItem(PasteFromMenuItem);
   menu->Caption=caption;
   menu->OnClick=PasteFromMenuClick;
   PasteFromMenuItem->Add(menu);

   menu= new TMenuItem(AddPointerToListMenuItem);
   menu->Caption=caption;
   menu->OnClick=AddPointerToListMenuItemClick;
   AddPointerToListMenuItem->Add(menu);


 }

}
void __fastcall TPointersViewBox::WritePointersToFile(TObject*)
{
if(FList)
 if(FList->size()>0)
 {
 TSaveDialog* dialog=new TSaveDialog(NULL);
 if(dialog->Execute())
  {
   FileClose(FileCreate(dialog->FileName));
   std::ofstream file(dialog->FileName.c_str());
   WriteTListToStream(*FList,file);

  }
 delete dialog;
 }
}

void __fastcall TPointersViewBox::LoadPointersFromFile(TObject*)
{
if(FList)
 {
  boost::shared_ptr<TOpenDialog> dialog=boost::shared_ptr<TOpenDialog>(new TOpenDialog(NULL));
  if(dialog->Execute())
    {
     boost::shared_ptr<std::ifstream> file=boost::shared_ptr<std::ifstream>(new std::ifstream(dialog->FileName.c_str()));
     boost::shared_ptr< std::vector<PointerType> > list(new std::vector<PointerType>());
     ReadTListFromStream(*list,*file);
     SetList(list);
    }
 }
}


void __fastcall TPointersViewBox::CopyMenuItemClick(TObject*)
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

void __fastcall TPointersViewBox::PasteMenuItemClick(TObject*)
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




void __fastcall TPointersViewBox::DeleteSubMenuClick(TObject *Sender)
{
TMenuItem* menu=(TMenuItem*)Sender;
int num=menu->Caption.ToInt();
if(num < PoitersDB.size())
{
    PoitersDB.erase(PoitersDB.begin() + num );
}
}
//---------------------------------------------------------------------------
void __fastcall TPointersViewBox::CopytoMenuClick(TObject *Sender)
{
TMenuItem* menu=(TMenuItem*)Sender;
int num=menu->Caption.ToInt();

if(FList)
 if((FList->size()>0)&&(PoitersDB.size()>0)&&(num<PoitersDB.size()))
 {
 boost::shared_ptr< std::vector<PointerType> > destList = PoitersDB[num];
 for(int i=0;i<FList->size();i++)
  {
    destList->push_back((*FList)[i]);
  }
 }

}
//---------------------------------------------------------------------------
void __fastcall TPointersViewBox::PasteFromMenuClick(TObject *Sender)
{
TMenuItem* menu=(TMenuItem*)Sender;
int num=menu->Caption.ToInt();

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

void __fastcall TPointersViewBox::ClearClipBoardMenuItemClick(TObject* Sender)
{
  PoitersDB.clear();
}


void __fastcall TPointersViewBox::AddPointerToListMenuItemClick (TObject* Sender)
{
 TMenuItem* menu=(TMenuItem*)Sender;
int num=menu->Caption.ToInt();

if(FList)
 if((PoitersDB.size()>0)&&(num<PoitersDB.size()))
 {

    std::vector<PointerType>& destList=*(PoitersDB[num]);
    for(int i=0;i<PointersBox->Count;i++)
     {
        if(PointersBox->Selected[i])
        destList.push_back((HexAnsiToulong(PointersBox->Items->Strings[i].c_str())));
     }
 }
}

void __fastcall TPointersViewBox::DeletePointerMenuItemClick (TObject* Sender)
{
DeleteSelectedPointers();
}

void __fastcall TPointersViewBox::Reload(int pos)
{
if(Showing)
 {
  PointersBox->Items->BeginUpdate();
  PointersBox->Items->Clear();
  PointersBox->MultiSelect=true;
   if(pos<FList->size())
    {
     int itemCount=abs(PointersBox->Height/(abs(PointersBox->Font->Height)+2));
     if (itemCount>FList->size()-pos) itemCount=FList->size()-pos;
     for(int i=0;i<itemCount;i++)
      {
       PointersBox->Items->Add(ulongToHexAnsi((unsigned long)((*FList)[pos+i]),8).c_str());
      }
    }
    PointersBox->Items->EndUpdate();

 }
}


void __fastcall TPointersViewBox::DeleteSelectedPointers(void)
{
if(FList)
{

    for(int i=0;i<PointersBox->Count;i++)
     {
        if(PointersBox->Selected[i])
        {
          FList->erase(std::remove(FList->begin(),FList->end(),(HexAnsiToulong(PointersBox->Items->Strings[i].c_str()))));
        }
     }

    Update();
}

}

void __fastcall TPointersViewBox::KeyDown(TObject *Sender,WORD &Key, TShiftState Shift)
{
if(Key==0x2E) //delete key
{
 DeleteSelectedPointers();
Key=0;
}

}

