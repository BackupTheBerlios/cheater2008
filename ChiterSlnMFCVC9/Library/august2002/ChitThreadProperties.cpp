//---------------------------------------------------------------------------

#include "stdafx.h"
#pragma hdrstop
#include "ChitThreadProperties.h"

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TChitThreadProperties *)
{
    new TChitThreadProperties(NULL);
}

//---------------------------------------------------------------------------
 TChitThreadProperties::TChitThreadProperties(TComponent* Owner)
        : TGroupBox(Owner)
{
Scroll=new TScrollBar(this);
Scroll->Kind=sbVertical;
Scroll->OnScroll=ScrollEvent;
Scroll->Align=alLeft;
InsertControl(Scroll);
SearcherProperties=new TSearcherProperties(this);
SearcherProperties->Align=alClient;
InsertControl(SearcherProperties);
PopupMenu=new TPopupMenu(this);
PopupMenu->AutoHotkeys=maManual;
TMenuItem* menu=new TMenuItem(this);
menu->Caption="Delete Unit";
menu->OnClick=DeleteUnitMenuItemClick;
PopupMenu->Items->Add(menu);

menu=new TMenuItem(this);
menu->Caption="Reload";
menu->OnClick=ReloadMenuItemClick;
PopupMenu->Items->Add(menu);

}

//---------------------------------------------------------------------------

namespace Chitthreadproperties
{
        void  PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TChitThreadProperties)};
                 RegisterComponents("Samples", classes, 0);
        }
}

//---------------------------------------------------------------------------

void  TChitThreadProperties::SetSearchers(std::vector< boost::shared_ptr<TSearcher> > value)
{
   FSearchers=value;
   Scroll->Min=0;Scroll->Position=0;
   Scroll->Max=0;
   if(FSearchers.size()>0) {Scroll->Max=FSearchers.size()-1;Reload(0);}
}

//----------------------------------------------------------------------------
void  TChitThreadProperties::ScrollEvent(System::TObject* Sender, TScrollCode ScrollCode, int &ScrollPos)
{
Reload(ScrollPos);
}

void  TChitThreadProperties::DeleteUnitMenuItemClick(TObject*)
{
if(!FSearchers.empty())
 if(Scroll->Position<FSearchers.size())
   {
     FSearchers.erase(FSearchers.begin()+Scroll->Position);
     if(Scroll->Max>0)
      Scroll->Max--;
    }
}

void  TChitThreadProperties::Reload(int ScrollPos)
{
if(!FSearchers.empty())
 {
  if(Scroll->Max>=FSearchers.size())
   {
    if(FSearchers.size()!=0)
     Scroll->Max=FSearchers.size()-1;
    else
     Scroll->Max=0;
   }
  else
   {
           Scroll->Max=FSearchers.size()-1;
   }
  if(!Searchers.size()>0)
   {
    if(ScrollPos<Searchers.size()) {SearcherProperties->Searcher=Searchers[ScrollPos];
    Caption=AnsiString("Searcher - ")+AnsiString(ScrollPos+1)+AnsiString(" / ")+AnsiString(Searchers.size());}
   }
  else
     Caption=AnsiString("! Empty !");
 }
}

void  TChitThreadProperties::ReloadMenuItemClick(TObject*)
{
 Reload(Scroll->Position);
}

