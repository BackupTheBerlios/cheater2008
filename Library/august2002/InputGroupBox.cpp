//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "InputGroupBox.h"
#include <Ansi_stream.h>
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
 char* FOR_POINTERS ="для указателей";
static inline void ValidCtrCheck(TInputGroupBox *)
{
        new TInputGroupBox(NULL);
}
//---------------------------------------------------------------------------
unsigned long TInputGroupBox::Instance=0;

//----------------------------------------------------------------------------
__fastcall TInputGroupBox::TInputGroupBox(TComponent* Owner)
        : TGroupBox(Owner)
{
OnEnter=TInputGroupBoxOnEnter;
 TextBox=CreateTextBox();
 TypeBox=CreateTypeBox();
 CreateLabels();
 PopupMenu=CreatePopupMenu();
 Width=205;
 Height=85;
 OnResize=OnResizeBox;
}
//---------------------------------------------------------------------------
namespace Inputgroupbox
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TInputGroupBox)};
                 RegisterComponents("Samples", classes, 0);
        }
}
//---------------------------------------------------------------------------


TComboBox* TInputGroupBox::CreateTextBox(void)
{
    //TODO: Add your source code here
 TComboBox *Work=new TComboBox((TComponent*)this);
 Work->Width=200;
 Work->Top=25;
 Work->Left=2;
 Work->OnChange=TInputGroupBoxTextBoxChange;
 Work->Parent=this; 
 return Work;
}
//--------------------------------------------------------------------------------
TTypeComboBox* TInputGroupBox::CreateTypeBox(void)
{
    //TODO: Add your source code here
 TTypeComboBox *Work=new TTypeComboBox((TComponent*)this);
 Work->Top=60;
 Work->Left=2;
 Work->Width=200;
 Work->OnChange=TInputGroupBoxTypeBoxChange;
 Work->Parent=this;
 return Work;
}
//--------------------------------------------------------------------------------
void TInputGroupBox::CreateLabels(void)
{
 TLabel *Work=new TLabel((TComponent*)this);
 Work->Caption="Текст";
 Work->Top=12;
 Work->Left=2;
 Work->Parent=this;
 Work=new TLabel((TComponent*)this);
 Work->Caption="Тип";
 Work->Top=46;
 Work->Left=2;
 Work->Parent=this;
}




void __fastcall TInputGroupBox::InitBox(const std::string& i_str,int Type)
{
    //TODO: Add your source code here
if(Type<=FLOAT_NUM)
 {
  TypeBox->ItemIndex=Type;
  TextBox->Text=i_str.c_str();
 }

}


void __fastcall TInputGroupBox::SetTypeOfBox(AnsiString value)
{
  FTypeOfBox=value;
}

AnsiString __fastcall TInputGroupBox::GetTypeOfBox()
{
  return FTypeOfBox;
}


__fastcall TInputGroupBox::~TInputGroupBox(void)
{
    //TODO: Add your source code here


}
//----------------------------------------------------------------------------------
void __fastcall TInputGroupBox::TInputGroupBoxTextBoxChange(TObject*)
{
//
BoxesChange();

}
//----------------------------------------------------------------------------------
void __fastcall TInputGroupBox::TInputGroupBoxTypeBoxChange(TObject*)
{
//
BoxesChange();

}
//------------------------------------------------------------------------
void __fastcall TInputGroupBox::TInputGroupBoxOnEnter(TObject*)
{
 //
if(TypeBox->Items->Count==0)
 {
  TypeBox->Clear();
  TypeBox->Items->Add(StringType[HEX_NUM]);
  TypeBox->Items->Add(StringType[DEC_NUM]);
  if(FTypeOfBox!="для указателей")
   {
    TypeBox->Items->Add(StringType[STRING]);
    TypeBox->Items->Add(StringType[HEX_STRING]);
    TypeBox->Items->Add(StringType[FLOAT_NUM]);
    TypeBox->Items->Add(StringType[DOUBLE_NUM]);
   }
 }


}
//-----------------------------------------------------------------------------------
void __fastcall TInputGroupBox::WriteDataToStream(TStream* stream)
{
    //TODO: Add your source code here
WriteTypedAnsiToStream(std::string(TextBox->Text.c_str()),TypeBox->ItemIndex,stream);
}


unsigned long __fastcall TInputGroupBox::GetPointer(void)
{
    //TODO: Add your source code here
    return PointerAnsiToulong(std::string(TextBox->Text.c_str()),TypeBox->ItemIndex);
}


bool __fastcall TInputGroupBox::IsEmpty(void)
{
    //TODO: Add your source code here
    return TextBox->Text.IsEmpty()||TypeBox->Text.IsEmpty();
}


//--------------------------------------------------------------------------------


AnsiString __fastcall TInputGroupBox::GetText()
{
 return TextBox->Text;
}


void __fastcall TInputGroupBox::BoxesChange(void)
{
    //TODO: Add your source code here
if (!TypeBox->Text.IsEmpty()&&!TextBox->Text.IsEmpty())
 {
  int Work=WhatAnsiType(std::string(TextBox->Text.c_str()));
  if((TypeBox->ItemIndex!=DOUBLE_NUM)&&(TypeBox->ItemIndex!=FLOAT_NUM))
   {
    if((Work!=DEC_NUM)&&(TypeBox->ItemIndex==DEC_NUM))
     TextBox->Text="";
    else
     if((Work==STRING)&&(TypeBox->ItemIndex!=STRING))
      TextBox->Text="";
   }
 }

}


void __fastcall TInputGroupBox::Clear(void)
{
    //TODO: Add your source code here
    TextBox->Items->Clear();
}


void __fastcall TInputGroupBox::Add(const std::string& str)
{
    //TODO: Add your source code here
if(TextBox->Items->IndexOf(str.c_str())==-1)
 TextBox->Items->Add(str.c_str());
}


TStrings* __fastcall TInputGroupBox::GetStrings()
{
    return TextBox->Items;
}


TPopupMenu* __fastcall TInputGroupBox::CreatePopupMenu(void)
{
    //TODO: Add your source code here
    TPopupMenu* work=new TPopupMenu(this);
    work->OnPopup=TInputGroupBoxPopupMenuAppear;
    TMenuItem* MenuItem;
   // work->
    MenuItem=new TMenuItem(work);
    SavePointerMenuItem=MenuItem;
    MenuItem->Caption="Сохранить указатель";
    SavePointerMenuItem->Enabled=false;
    MenuItem->OnClick=TInputGroupBoxSavePointerMenuItemOnClick;
    work->Items->Add(MenuItem);

    MenuItem=new TMenuItem(work);
    ClearPointersMenuItem=MenuItem;
    MenuItem->Caption="Очистить TextBox";
    ClearPointersMenuItem->Enabled=true;
    MenuItem->OnClick=TInputGroupBoxClearPointersMenuItemOnClick;
    work->Items->Add(MenuItem);

    MenuItem=new TMenuItem(work);
    MenuItem->Caption="-";
    MenuItem->Enabled=true;
    work->Items->Add(MenuItem);

    MenuItem=new TMenuItem(work);
    ConvertToMenuItem=MenuItem;
    MenuItem->Caption="Перевести в...";
    ConvertToMenuItem->Enabled=true;
    work->Items->Add(MenuItem);



    MenuItem=new TMenuItem(ConvertToMenuItem);
    ConvertToDecNumMenuItem=MenuItem;
    MenuItem->Caption=StringType[DEC_NUM];
    ConvertToDecNumMenuItem->Enabled=true;
    MenuItem->OnClick=TInputGroupBoxConvertToDecNumMenuItemOnClick;
    ConvertToMenuItem->Add(MenuItem);

    MenuItem=new TMenuItem(ConvertToMenuItem);
    ConvertToHexNumMenuItem=MenuItem;
    MenuItem->Caption=StringType[HEX_NUM];
    ConvertToHexNumMenuItem->Enabled=true;
    MenuItem->OnClick=TInputGroupBoxConvertToHexNumMenuItemOnClick;
    ConvertToMenuItem->Add(MenuItem);

    MenuItem=new TMenuItem(ConvertToMenuItem);
    ConvertToStringMenuItem=MenuItem;
    MenuItem->Caption=StringType[STRING];
    ConvertToStringMenuItem->Enabled=true;
    MenuItem->OnClick=TInputGroupBoxConvertToStringMenuItemOnClick;
    ConvertToMenuItem->Add(MenuItem);

    MenuItem=new TMenuItem(ConvertToMenuItem);
    ConvertToHexStringMenuItem=MenuItem;
    MenuItem->Caption=StringType[HEX_STRING];
    ConvertToHexStringMenuItem->Enabled=true;
    MenuItem->OnClick=TInputGroupBoxConvertToHexStringMenuItemOnClick;
    ConvertToMenuItem->Add(MenuItem);

    MenuItem=new TMenuItem(ConvertToMenuItem);
    ConvertToDoubleNumMenuItem=MenuItem;
    MenuItem->Caption=StringType[DOUBLE_NUM];
    ConvertToDoubleNumMenuItem->Enabled=true;
    MenuItem->OnClick=TInputGroupBoxConvertToDoubleNumMenuItemOnClick;
    ConvertToMenuItem->Add(MenuItem);

    MenuItem=new TMenuItem(ConvertToMenuItem);
    ConvertToFloatNumMenuItem=MenuItem;
    MenuItem->Caption=StringType[FLOAT_NUM];
    ConvertToFloatNumMenuItem->Enabled=true;
    MenuItem->OnClick=TInputGroupBoxConvertToFloatNumMenuItemOnClick;
    ConvertToMenuItem->Add(MenuItem);

    return work;

}


void __fastcall TInputGroupBox::TInputGroupBoxSavePointerMenuItemOnClick(TObject*)
{
    //TODO: Add your source code here
Add(TextBox->Text.c_str());
}


void __fastcall TInputGroupBox::TInputGroupBoxClearPointersMenuItemOnClick(TObject*)
{
    //TODO: Add your source code here
    TextBox->Items->Clear();
}


void __fastcall TInputGroupBox::TInputGroupBoxPopupMenuAppear(TObject*)
{
    //TODO: Add your source code here
    SavePointerMenuItem->Enabled=!IsEmpty();
    ConvertToMenuItem->Enabled=!IsEmpty();
    ClearPointersMenuItem->Enabled=(TextBox->Items->Count!=0);
}


void __fastcall TInputGroupBox::TInputGroupBoxConvertToHexNumMenuItemOnClick(TObject*)
{
    //TODO: Add your source code here
unsigned long num;
num=0;
num=GetPointer();
TypeBox->ItemIndex=HEX_NUM;
TextBox->Text=AnsiString(ulongToHexAnsi(num).c_str());
}


void __fastcall TInputGroupBox::TInputGroupBoxConvertToHexStringMenuItemOnClick(TObject*)
{
    //TODO: Add your source code here
unsigned long len;
byte* x;
x=TypedAnsiTobyteptr(std::string(TextBox->Text.c_str()),TypeBox->ItemIndex,&len);
TypeBox->ItemIndex=HEX_STRING;
TextBox->Text=AnsiString(byteptrToHexAnsi(x,len).c_str());
delete x;
}


void __fastcall TInputGroupBox::TInputGroupBoxConvertToStringMenuItemOnClick(TObject*)
{
    //TODO: Add your source code here
unsigned long len;
byte* x;
AnsiString res("");
char ch[2];
ch[1]=0;
x=TypedAnsiTobyteptr(std::string(TextBox->Text.c_str()),TypeBox->ItemIndex,&len);
TypeBox->ItemIndex=STRING;
for(unsigned long i=0;i<len;i++)
  {
   ch[0]=ConvertToPrintSign(x[i]);
   res.Insert(ch,res.Length()+1);
  }
TextBox->Text=res;
delete x;
}


void __fastcall TInputGroupBox::TInputGroupBoxConvertToDecNumMenuItemOnClick(TObject*)
{
    //TODO: Add your source code here
unsigned long num;
num=0;
num=GetPointer();
TypeBox->ItemIndex=DEC_NUM;
TextBox->Text=AnsiString(ulongToAnsi(num).c_str());
}
void __fastcall TInputGroupBox::TInputGroupBoxConvertToDoubleNumMenuItemOnClick(TObject*)
{
    //TODO: Add your source code here
WORK_ANSILIB_UNION_FOR_CONVERT work;
work.double_=0;
unsigned long len;
byte* x;
 x=TypedAnsiTobyteptr(std::string(TextBox->Text.c_str()),TypeBox->ItemIndex,&len);
memcpy(&work,x,sizeof(double));
TypeBox->ItemIndex=DOUBLE_NUM;
TextBox->Text=AnsiString(work.double_);
delete x;
}
//------------------------
void __fastcall TInputGroupBox::TInputGroupBoxConvertToFloatNumMenuItemOnClick(TObject*)
{
    //TODO: Add your source code here
WORK_ANSILIB_UNION_FOR_CONVERT work;
work.double_=0;
unsigned long len;
byte* x;
 x=TypedAnsiTobyteptr(std::string(TextBox->Text.c_str()),TypeBox->ItemIndex,&len);
memcpy(&work,x,sizeof(float));
TypeBox->ItemIndex=FLOAT_NUM;
TextBox->Text=AnsiString(work.float_);
delete x;
}

//--------------------
void __fastcall TInputGroupBox::LoadPointers(TList* list)
{
 if(list->Count<=200)
  {
  for(int i=0;i<list->Count;i++)
    {
     Add(ulongToHexAnsi((unsigned long)list->Items[i]));
    }
  }
}
//----------------------------------------------------------------------------
void __fastcall TInputGroupBox::OnResizeBox(TObject*)
{
// TextBox->Width=Width-6;
// TypeBox->Width=Width-6;
}

