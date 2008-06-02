//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "ChitThreadProperties.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TChitThreadProperties *)
{
        new TChitThreadProperties(NULL);
}
//---------------------------------------------------------------------------
__fastcall TChitThreadProperties::TChitThreadProperties(TComponent* Owner)
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
FSearchers=NULL;
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
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TChitThreadProperties)};
                 RegisterComponents("Samples", classes, 0);
        }
}
//---------------------------------------------------------------------------
void __fastcall TChitThreadProperties::SetSearchers(TList* value)
{
   FSearchers=value;
   Scroll->Min=0;Scroll->Position=0;
   Scroll->Max=0;
   if(FSearchers!=NULL)
    if(value->Count>0) {Scroll->Max=value->Count-1;Reload(0);}


}

//----------------------------------------------------------------------------
void __fastcall TChitThreadProperties::ScrollEvent(System::TObject* Sender, TScrollCode ScrollCode, int &ScrollPos)
{
Reload(ScrollPos);
}


void __fastcall TChitThreadProperties::DeleteUnitMenuItemClick(TObject*)
{
if(Searchers!=NULL)
 if(Scroll->Position<Searchers->Count)
   {
    delete      (TSearcher*)Searchers->Items[Scroll->Position];
    Searchers->Delete(Scroll->Position);
    if(Scroll->Max>0)
     Scroll->Max--;
   }
}

void __fastcall TChitThreadProperties::Reload(int ScrollPos)
{
if(Searchers!=NULL)
 {
  if(Scroll->Max>=Searchers->Count)
   {
    if(Searchers->Count!=0)
     Scroll->Max=Searchers->Count-1;
    else
     Scroll->Max=0;
   }
  else
   {
           Scroll->Max=Searchers->Count-1;
   }
  if(Searchers->Count>0)
   {
    if(ScrollPos<Searchers->Count) {SearcherProperties->Searcher=(TSearcher*)Searchers->Items[ScrollPos];
    Caption=AnsiString("Searcher - ")+AnsiString(ScrollPos+1)+AnsiString(" / ")+AnsiString(Searchers->Count);}
   }
  else
     Caption=AnsiString("! Empty !");
 }
}

void __fastcall TChitThreadProperties::ReloadMenuItemClick(TObject*)
{
 Reload(Scroll->Position);
}
