//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CountEdit.h"
#include <Ansi_stream.h>
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TCountEdit *)
{
        new TCountEdit(NULL);
}
//---------------------------------------------------------------------------
unsigned long TCountEdit::Instance=0;

//----------------------------------------------------------------------------
__fastcall TCountEdit::TCountEdit(TComponent* Owner)
        : TGroupBox(Owner)
{
CreateInstance();
}
//---------------------------------------------------------------------------
namespace Countedit
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TCountEdit)};
                 RegisterComponents("Samples", classes, 0);
        }
}
//---------------------------------------------------------------------------

__fastcall TCountEdit::~TCountEdit(void)
{
    //TODO: Add your source code here


}

void TCountEdit::CreateInstance(void)
{
    //TODO: Add your source code here
 EditField=new TEdit((TComponent*)this);
 EditField->Width=30;

 EditField->Top=15;
 EditField->Left=2;
 EditField->Parent=this;

 UpDownField=new TUpDown((TComponent*)this);
 UpDownField->Min=1;
 UpDownField->Max=255;
 UpDownField->Position=16;
 UpDownField->OnChangingEx=UpDownFieldChangingEventEx;
 UpDownField->Top=EditField->Top;
 UpDownField->Left=EditField->Left+EditField->Width+1;
 UpDownField->Height=EditField->Height;
 UpDownField->Width=10;

 UpDownField->Parent=this; 
 OnChangeEvent=NULL;
 OnResize=OnResizeEvent;

Min=1;
Max=100;
}

void __fastcall TCountEdit::UpDownFieldChangingEventEx(System::TObject* Sender, bool &AllowChange, short NewValue, TUpDownDirection Direction)
{
if(Direction==updNone) throw "TCountEdit::UpDownFieldChangingEventEx Error";
DWORD value=AnsiToulong(EditField->Text);
if(Direction==updUp)
{
value++;
}
else
{
if(Direction==updDown)
{
value--;
}
}
if((value>=Min)&&(value<=Max))
{
  EditField->Text=ulongToAnsi(value);
  if(OnChangeEvent) OnChangeEvent(dynamic_cast<TObject *>(this),value);

}  
AllowChange=false;
}

void __fastcall TCountEdit::OnResizeEvent(TObject*)
{
 EditField->Top=15;
 EditField->Left=2;
 UpDownField->Width=10;
 EditField->Width=this->Width-UpDownField->Width-2;
 EditField->Height=this->Height-EditField->Top-2;

 UpDownField->Top=EditField->Top;
 UpDownField->Left=EditField->Left+EditField->Width+1;
 UpDownField->Height=EditField->Height;
}

void __fastcall TCountEdit::SetText(AnsiString value)
{
        //TODO: Add your source code here
EditField->Text=value;
}
AnsiString __fastcall TCountEdit::GetText()
{
        //TODO: Add your source code here
return    EditField->Text;
}

void __fastcall TCountEdit::SetMin(DWORD value)
{
        //TODO: Add your source code here
        FMin=value;
}

void __fastcall TCountEdit::SetMax(DWORD value)
{
        //TODO: Add your source code here
        FMax=value;        
}
