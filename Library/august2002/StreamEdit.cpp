//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "StreamEdit.h"
#include "EditForm.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
int NumberOfCopies=0;
char* WhatFind="Что искать";
 TList* ClipBoard;     // clipboard for selections

static inline void ValidCtrCheck(TStreamEdit *)
{
        new TStreamEdit(NULL);
}
//---------------------------------------------------------------------------
namespace Streamedit
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TStreamEdit)};
                 RegisterComponents("Samples", classes, 0);
        }
}
//---------------------------------------------------------------------------
__fastcall TStreamEdit::TStreamEdit(TComponent* Owner)
        : TGroupBox(Owner)
{
InitInterface();
if(NumberOfCopies==0)
 {
  ClipBoard=new TList();
 }
Fstream=NULL;
FCurrentPos=0;
ViewedLen=0;
NumberOfString=20;
StringLen=16;
IsProcessKeyDown=false;


// throw Exception("Sergey12");

// throw Exception("Sergey13");
SearcherProperties->OnSelectPointer=PointersNotifyEvent;
// throw Exception("Sergey11");
NumberOfCopies++;
}
//---------------------------------------------------------------------------
void __fastcall TStreamEdit::InitInterface(void)
{
BottomPanel=new TPanel(this);
BottomPanel->Align=alBottom;
InsertControl(BottomPanel);
ProgressBar=new TProgressBar(this);
ProgressBar->Smooth=true;
ProgressBar->Align=alRight;
BottomPanel->InsertControl(ProgressBar);
Splitter5=new TSplitter(this);
Splitter5->Align=alRight;
BottomPanel->InsertControl(Splitter5);

InfoEdit=new TRichEdit(this);
InfoEdit->Align=alClient;
InfoEdit->ScrollBars=ssBoth;
BottomPanel->InsertControl(InfoEdit);
//-------------------Upper all OK--------------------------------------
PopupMenu1=new TPopupMenu(this);
PopupMenu1->OnPopup=PopupMenu1Popup;
PopupMenu1->AutoHotkeys=maManual;
PopupMenu=PopupMenu1;
SlowSearchMenuItem=new TMenuItem(PopupMenu1);
SlowSearchMenuItem->Caption=AnsiString("Slow Search");
SlowSearchMenuItem->OnClick=SlowSearchMenuItemClick;
PopupMenu->Items->Add(SlowSearchMenuItem);
//---------------------------------------
SearchMenuItem=new TMenuItem(PopupMenu);
SearchMenuItem->Caption=AnsiString("Search");
SearchMenuItem->OnClick=SearchMenuItemClick;
PopupMenu->Items->Add(SearchMenuItem);
//----------------------------------
ContinueSearchMenuItem=new TMenuItem(PopupMenu);
ContinueSearchMenuItem->Caption=AnsiString("Continue Search");
ContinueSearchMenuItem->OnClick=ContinueSearchMenuItemClick;
PopupMenu->Items->Add(ContinueSearchMenuItem);
//----------------------------------
LoadFromStreamMenuitem=new TMenuItem(PopupMenu);
LoadFromStreamMenuitem->Caption=AnsiString("ReLoad");
LoadFromStreamMenuitem->OnClick=LoadFromStreamMenuitemClick;
PopupMenu->Items->Add(LoadFromStreamMenuitem);
//------------------------------------------
GotoMenuItem=new TMenuItem(PopupMenu);
GotoMenuItem->Caption=AnsiString("Goto...");
GotoMenuItem->OnClick=GotoMenuItemClick;
PopupMenu->Items->Add(GotoMenuItem);
//----------------------------------
TMenuItem* work=new TMenuItem(PopupMenu);
work->Caption=AnsiString("-");
PopupMenu->Items->Add(work);
//-------------------------------------------
CopyMenuItem=new TMenuItem(PopupMenu);
CopyMenuItem->Caption=AnsiString("Copy");
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
CopytoMenuItem->Caption=AnsiString("Copyto");
PopupMenu->Items->Add(CopytoMenuItem);
//----------------------------------------------
PasteFromMenuItem=new TMenuItem(PopupMenu);
PasteFromMenuItem->Caption=AnsiString("Paste from");
PopupMenu->Items->Add(PasteFromMenuItem);
//----------------------------------------------
EditBufMenuItem=new TMenuItem(PopupMenu);
EditBufMenuItem->Caption=AnsiString("Edit buf");
PopupMenu->Items->Add(EditBufMenuItem);
//--------------------------------------------
DeleteMenuItem=new TMenuItem(PopupMenu);
DeleteMenuItem->Caption=AnsiString("Delete");
PopupMenu->Items->Add(DeleteMenuItem);
//--------------------------------------------
ClearClipBoardMenuItem=new TMenuItem(PopupMenu);
ClearClipBoardMenuItem->Caption=AnsiString("Clear ClipBoard");
ClearClipBoardMenuItem->OnClick=ClearClipBoardMenuItemClick;
PopupMenu->Items->Add(ClearClipBoardMenuItem);
//--------------------------------------------------
work=new TMenuItem(PopupMenu);
work->Caption=AnsiString("-");
PopupMenu->Items->Add(work);
//---------------------------------------
Color1=new TMenuItem(PopupMenu);
Color1->Caption=AnsiString("Color");
Color1->OnClick=Color1Click;
PopupMenu->Items->Add(Color1);
//---------------------------------------
Font1=new TMenuItem(PopupMenu);
Font1->Caption=AnsiString("Font");
Font1->OnClick=Font1Click;
PopupMenu->Items->Add(Font1);
//--------------------------------------------------
FontMenuItem=new TMenuItem(PopupMenu);
FontMenuItem->Caption=AnsiString("Font");
PopupMenu->Items->Add(FontMenuItem);
 StringFontMenuItem=new TMenuItem(FontMenuItem);
 StringFontMenuItem->Caption=AnsiString("String");
 StringFontMenuItem->OnClick=StringFontMenuItemClick;
 FontMenuItem->Add(StringFontMenuItem);
 HexFontMenuItem=new TMenuItem(FontMenuItem);
 HexFontMenuItem->Caption=AnsiString("Hex");
 HexFontMenuItem->OnClick=HexFontMenuItemClick;
 FontMenuItem->Add(HexFontMenuItem);
 PointerFontMenuItem=new TMenuItem(FontMenuItem);
 PointerFontMenuItem->Caption=AnsiString("Pointer");
 PointerFontMenuItem->OnClick=PointerFontMenuItemClick;
 FontMenuItem->Add(PointerFontMenuItem);
//--------------------------------------------------
ColorMenuItem=new TMenuItem(PopupMenu);
ColorMenuItem->Caption=AnsiString("Color");
PopupMenu->Items->Add(ColorMenuItem);
 StringColorMenuItem=new TMenuItem(ColorMenuItem);
 StringColorMenuItem->Caption=AnsiString("String");
 StringColorMenuItem->OnClick=StringColorMenuItemClick;
 ColorMenuItem->Add(StringColorMenuItem);
 HexColorMenuItem=new TMenuItem(ColorMenuItem);
 HexColorMenuItem->Caption=AnsiString("Hex");
 HexColorMenuItem->OnClick=HexColorMenuItemClick;
 ColorMenuItem->Add(HexColorMenuItem);
 PointerColorMenuItem=new TMenuItem(ColorMenuItem);
 PointerColorMenuItem->Caption=AnsiString("Pointer");
 PointerColorMenuItem->OnClick=PointerColorMenuItemClick;
 ColorMenuItem->Add(PointerColorMenuItem);
//--------------------------------------------------
work=new TMenuItem(PopupMenu);
work->Caption=AnsiString("-");
PopupMenu->Items->Add(work);
//---------------------------------------
ReplaceAllMenuItem=new TMenuItem(PopupMenu);
ReplaceAllMenuItem->Caption=AnsiString("Replace All");
ReplaceAllMenuItem->OnClick=ReplaceAllMenuItemClick;
PopupMenu->Items->Add(ReplaceAllMenuItem);
//------------------------------------------------------
ColorDialog=new TColorDialog(this);
FontDialog=new TFontDialog(this);
//--------------Upper all is ok-----------------------------------
//-----------------GROUP BOX 2-------------------------------------
GroupBox2=new TGroupBox(this);
GroupBox2->Align=alRight;
GotoInputGroupBox=new TInputGroupBox(GroupBox2);
GotoInputGroupBox->Caption=AnsiString("Go to");
GotoInputGroupBox->Top=10;GotoInputGroupBox->Left=5;
GroupBox2->InsertControl(GotoInputGroupBox);

 StringNumberEdit=new TCountEdit(GroupBox2);
 StringNumberEdit->Caption=AnsiString("Number of lines");
 StringNumberEdit->Left=5;
 StringNumberEdit->Top=GotoInputGroupBox->Top+GotoInputGroupBox->Height;
 StringNumberEdit->OnChangeEvent=StringsOptionChangedEvent;
 StringNumberEdit->Height=40;
 StringNumberEdit->Width=90;
 GroupBox2->InsertControl(StringNumberEdit);
 StringLengthEdit=new TCountEdit(GroupBox2);
 StringLengthEdit->Left=StringNumberEdit->Left+StringNumberEdit->Width+2;
 StringLengthEdit->Top=StringNumberEdit->Top;
 StringLengthEdit->OnChangeEvent=StringsOptionChangedEvent;
 StringLengthEdit->Height=40;
 StringLengthEdit->Width=80;
 StringLengthEdit->Caption=AnsiString("Line's length");
 GroupBox2->InsertControl(StringLengthEdit);


 SearcherProperties=new TSearcherProperties(GroupBox2);
 SearcherProperties->Align=alBottom;
 GroupBox2->InsertControl(SearcherProperties);
 Splitter2=new TSplitter(GroupBox2);
 Splitter2->Beveled=true;
 Splitter2->Align=alBottom;
 GroupBox2->InsertControl(Splitter2);
InsertControl(GroupBox2);
//-----------------GROUP BOX 2-------------------------------------
// throwException("Sergey3");
Splitter1=new TSplitter(this);
Splitter1->Align=alRight;
InsertControl(Splitter1);
//-----------------GROUP BOX 1-------------------------------------
GroupBox1=new TGroupBox(this);
GroupBox1->Align=alClient;
GroupBox2->Width=280;

   StringMemo=new TMemo(GroupBox1);
   StringMemo->OnKeyDown=StringRichEditKeyDown;
   StringMemo->OnKeyPress=StringRichEditKeyPress;
   StringMemo->OnMouseMove=StringRichEditMouseMove;
   StringMemo->Align=alRight;
   StringMemo->Width=150;
   StringMemo->Font->Name="Fixedsys";
   GroupBox1->InsertControl(StringMemo);

   Splitter3=new TSplitter(GroupBox1);
   Splitter3->Align=alRight;
   GroupBox1->InsertControl(Splitter3);


   PointerMemo=new TMemo(GroupBox1);
   PointerMemo->Align=alLeft;
   PointerMemo->Width=80;
   PointerMemo->Font->Name="Fixedsys";
   GroupBox1->InsertControl(PointerMemo);

   Splitter4=new TSplitter(GroupBox1);
   Splitter4->Left=PointerMemo->Width+10;
   Splitter4->Align=alLeft;
   GroupBox1->InsertControl(Splitter4);

   HexMemo=new TMemo(GroupBox1);
   HexMemo->OnKeyDown=HexRichEditKeyDown;
   HexMemo->OnKeyPress=HexRichEditKeyPress;
   HexMemo->OnMouseMove=HexRichEditMouseMove;
   HexMemo->Align=alClient;
   HexMemo->Width=550;
   HexMemo->Font->Name="Fixedsys";
   GroupBox1->InsertControl(HexMemo);



PointerMemo->WordWrap=false;
HexMemo->WordWrap=false;
StringMemo->WordWrap=false;

StringMemo->HideSelection=false;
HexMemo->HideSelection=false;
InsertControl(GroupBox1);
}


//---------------------------------------------------------------------------
void __fastcall TStreamEdit::SearchMenuItemClick(TObject *Sender)
{
Search(true);

}
//---------------------------------------------------------------------------

void __fastcall TStreamEdit::SlowSearchMenuItemClick(TObject *Sender)
{
SlowSearch();
}
//---------------------------------------------------------------------------


__fastcall TStreamEdit::~TStreamEdit(void)
{
//delete MemoryStream;
NumberOfCopies--;
if(NumberOfCopies==0)
 {
  for(int i=0;i<ClipBoard->Count;i++)
   {
    delete (TMemoryStream*)ClipBoard->Items[i];
   }
  delete ClipBoard;
 }
}

void __fastcall TStreamEdit::LoadFromStream(void)
{
if(Showing)
 {
  ViewedLen=0;
  int RealRead=StringLen;
  char* ch=new char[StringLen];
  int HexMemoSelStart=HexMemo->SelStart;
  int StringMemoSelStart=StringMemo->SelStart;

  HexMemo->Lines->BeginUpdate();
  StringMemo->Lines->BeginUpdate();
  PointerMemo->Lines->BeginUpdate();
  HexMemo->Lines->Clear();
  StringMemo->Lines->Clear();
  PointerMemo->Lines->Clear();
try
 {
  if(stream!=NULL)
   {
     GroupBox1->Caption=AnsiString("StreamObject ")+stream->ClassName()+AnsiString(". Поле редактирования Размер : ")+AnsiString::IntToHex(stream->Size,8)+AnsiString(" (hex), (dec) - ")+AnsiString(stream->Size);
     stream->Position=CurrentPos;
    for(int i=0;(i<NumberOfString)&&(RealRead!=0);i++)
      {
       if((RealRead=stream->Read(ch,StringLen))>0)
        {
           PointerMemo->Lines->Add(ulongTo8digitHexString(stream->Position-RealRead).c_str());
		   HexMemo->Lines->Add(byteptrToHexAnsiWithSpace(ch,RealRead).c_str());
		   StringMemo->Lines->Add(ConvertToPrintString(ch,RealRead).c_str());
           ViewedLen+=RealRead;
        }
         else
           if(i==0)
              if(stream->Position>CurrentPos) CurrentPos=stream->Position;
      }
   }
 }
catch(Exception& e)
 {
  Application->ShowException(&e);
 }
  delete ch;
  HexMemo->Lines->EndUpdate();
  StringMemo->Lines->EndUpdate();
  PointerMemo->Lines->EndUpdate();
  StringNumberEdit->Text=ulongToAnsi(NumberOfString).c_str();
  StringLengthEdit->Text=ulongToAnsi(StringLen).c_str();

  HexMemo->SelStart=HexMemoSelStart;
  StringMemo->SelStart=StringMemoSelStart;
 }//enf if (Showing)
}
void __fastcall TStreamEdit::LoadFromStreamMenuitemClick(
      TObject *Sender)
{
LoadFromStream();
}
//---------------------------------------------------------------------------


int __fastcall TStreamEdit::ConvertHexPosToGlobal(int value)
{
//value=HexRichEdit->SelStart;
int res=0;
res+=StringLen*(value/(3*StringLen+1))+(value%(3*StringLen+1))/3;

return res;
}
void __fastcall TStreamEdit::HexRichEditKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
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
            ShiftHexSelStart(-StringLen*2);
           }
          else
           {
            if(Key==0x28)//down
             {
               ShiftHexSelStart(StringLen*2);
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
               ShiftHexSelStart(StringLen*NumberOfString*2);
                 }
                else
                 {
                  if(Key==0x21)//PgUp
                   {
                      ShiftHexSelStart(-StringLen*NumberOfString*2);
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
                         int pos=ConvertHexPosToGlobal(HexMemo->SelStart);
                          StringMemo->SelStart=ConvertGlobalToStringPos(pos);
                         StringMemo->SetFocus();

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

 AnsiString _shift="";


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


KeyPressed=AnsiString("Key: ")+AnsiString(ulongToHexAnsi(Key).c_str())+AnsiString(" ")+_shift;
 Key=0;

}
//---------------------------------------------------------------------------

void __fastcall TStreamEdit::GotoMenuItemClick(TObject *Sender)
{
CurrentPos=GotoInputGroupBox->GetPointer();
}
//---------------------------------------------------------------------------


void __fastcall TStreamEdit::HexRichEditKeyPres(unsigned char value)
{
int pos=ConvertHexPosToGlobal(HexMemo->SelStart);
int rem;
int hexPos=ConvertGlobalToHexPos(pos);
const char* ch;
char print[2];
print[1]=0;
int StringPos=ConvertGlobalToStringPos(pos);
StringMemo->SelStart=StringPos;
StringMemo->SelLength=1;

stream->Position=CurrentPos+pos;
HexMemo->SelLength=1;

HexMemo->SelText=AnsiString::IntToHex(value,1);
rem=HexMemo->SelStart;
HexMemo->SelStart=hexPos;
HexMemo->SelLength=2;
ch=HexAnsiTobyteptr(HexMemo->SelText.c_str());
print[0]=ConvertToPrintSign(*ch);
StringMemo->SelText=AnsiString(print);
HexMemo->SelLength=0;
stream->Write(ch,1);
HexMemo->SelStart=rem;
ShiftIfWrongHexSelStart();

delete ch;

//char
}

void TStreamEdit::ShiftIfWrongHexSelStart()
{
int pos=ConvertHexPosToGlobal(HexMemo->SelStart);
if(pos==ViewedLen-1)
   {
    HexMemo->SelStart--;
    ShiftHexSelStart(1);
   }
else
{
  if(HexMemo->SelStart%(3*StringLen+1)==3*StringLen-1)
   {
     HexMemo->SelStart+=2;
   }
}
if((HexMemo->SelStart%(3*StringLen+1))%3==2)
HexMemo->SelStart++;
}

int __fastcall TStreamEdit::ConvertGlobalToHexPos(int value)
{
//Edit2->Text=AnsiString((3*StringLen+1)*(value/StringLen)+(value%StringLen)*3);

 return (3*StringLen+1)*(value/StringLen)+(value%StringLen)*3;
}
void __fastcall TStreamEdit::HexRichEditKeyPress(TObject *Sender,
      char &Key)
{
Key=0;
}
//---------------------------------------------------------------------------


int TStreamEdit::FindInStream(char* buf, int len)
{
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
delete work;
return -1;*/
}

void __fastcall TStreamEdit::SetCurrentPos( int value)
{
if((value<stream->Size)&&(value>=0))
 {
  FCurrentPos=value;
  LoadFromStream();
 }
}


void __fastcall TStreamEdit::ShiftHexSelStart(int Shift)
{
int value=HexMemo->SelStart;
int InBytePos=(value%(3*StringLen+1))%3;
int Offset=ConvertHexPosToGlobal(value);
int OldOffset=Offset;
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
if((Offset+CurrentPos>=0)&&(Offset+CurrentPos<stream->Size))
 {
   if((Offset<0)||(Offset>=ViewedLen))
    {
     CurrentPos+=((Shift/(2*StringLen)))*StringLen+Shift%2;
     Offset=OldOffset;
    }
    HexMemo->SelStart=ConvertGlobalToHexPos(Offset)+InBytePos;
 }
  GetVariablesAtPos(CurrentPos+Offset);
}


void __fastcall TStreamEdit::CopyMenuItemClick(TObject *Sender)
{
int Start;
int End;
int Len;
TMemoryStream* buf=new TMemoryStream();
if(HexMemo->SelLength>0)
    { Start=ConvertHexPosToGlobal(HexMemo->SelStart);
      End=ConvertHexPosToGlobal(HexMemo->SelStart+HexMemo->SelLength);}
 else
    {End=ConvertHexPosToGlobal(HexMemo->SelStart);
      Start=ConvertHexPosToGlobal(HexMemo->SelStart+HexMemo->SelLength); }
  Len=End-Start+1;
  stream->Position=Start+CurrentPos;
  Len=buf->CopyFrom(stream,Len);
ClipBoard->Add(buf);
}
//---------------------------------------------------------------------------

void __fastcall TStreamEdit::PopupMenu1Popup(TObject *Sender)
{

int i;
int len;
TMenuItem* menu;
AnsiString caption;
 CopytoMenuItem->Clear();
 PasteFromMenuItem->Clear();
 DeleteMenuItem->Clear();
 EditBufMenuItem->Clear();
 EditBufMenuItem->Enabled=(ClipBoard->Count!=0);
    DeleteMenuItem->Enabled=(ClipBoard->Count!=0);
    PasteFromMenuItem->Enabled=(ClipBoard->Count!=0);
    PasteMenuItem->Enabled=(ClipBoard->Count!=0);
    CopytoMenuItem->Enabled=((ClipBoard->Count!=0)&&(HexMemo->SelLength!=0));
    ClearClipBoardMenuItem->Enabled=(ClipBoard->Count!=0);
    CopyMenuItem->Enabled=(HexMemo->SelLength!=0);
for(i=0;i<ClipBoard->Count;i++)
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

   menu= new TMenuItem(EditBufMenuItem);
   menu->Caption=caption;
   menu->OnClick=EditBufMenuClick;
   EditBufMenuItem->Add(menu);


 }

}
//---------------------------------------------------------------------------

void __fastcall TStreamEdit::PasteMenuItemClick(TObject *Sender)
{
int Start=ConvertHexPosToGlobal(HexMemo->SelStart);
stream->Position=Start+CurrentPos;
stream->CopyFrom((TStream*)ClipBoard->Last(),0);

LoadFromStream();
}
//---------------------------------------------------------------------------

void __fastcall TStreamEdit::DeleteSubMenuClick(TObject *Sender)
{
TMenuItem* menu=(TMenuItem*)Sender;
int num=menu->Caption.ToInt();
delete (TMemoryStream*)ClipBoard->Items[num];
ClipBoard->Delete(num);
}
//---------------------------------------------------------------------------
void __fastcall TStreamEdit::CopytoMenuClick(TObject *Sender)
{
TMenuItem* menu=(TMenuItem*)Sender;
int num=menu->Caption.ToInt();


int Start;
int End;
int Len;
if(HexMemo->SelLength>0)
    { Start=ConvertHexPosToGlobal(HexMemo->SelStart);
      End=ConvertHexPosToGlobal(HexMemo->SelStart+HexMemo->SelLength);}
 else
    {End=ConvertHexPosToGlobal(HexMemo->SelStart);
      Start=ConvertHexPosToGlobal(HexMemo->SelStart+HexMemo->SelLength); }
  Len=End-Start+1;
  stream->Position=Start+CurrentPos;
((TMemoryStream*)(ClipBoard->Items[num]))->Clear();
((TMemoryStream*)ClipBoard->Items[num])->CopyFrom(stream,Len);


}
//---------------------------------------------------------------------------
void __fastcall TStreamEdit::PasteFromMenuClick(TObject *Sender)
{
TMenuItem* menu=(TMenuItem*)Sender;
int num=menu->Caption.ToInt();

int Start=ConvertHexPosToGlobal(HexMemo->SelStart);
stream->Position=Start+CurrentPos;
stream->CopyFrom(((TMemoryStream*)ClipBoard->Items[num]),0);

LoadFromStream();
}
//---------------------------------------------------------------------------
void __fastcall TStreamEdit::ClearClipBoardMenuItemClick(
      TObject *Sender)
{
for(int i=0;i<ClipBoard->Count;i++)
 {
  delete (TMemoryStream*)ClipBoard->Items[i];
 }
 ClipBoard->Clear();
}
//---------------------------------------------------------------------------




void __fastcall TStreamEdit::Font1Click(TObject *Sender)
{
FontDialog->Execute();
HexMemo->Font=FontDialog->Font;
StringMemo->Font=FontDialog->Font;
PointerMemo->Font=FontDialog->Font;

}
//---------------------------------------------------------------------------

void __fastcall TStreamEdit::Color1Click(TObject *Sender)
{
ColorDialog->Execute();
HexMemo->Color=ColorDialog->Color;
StringMemo->Color=ColorDialog->Color;
PointerMemo->Color=ColorDialog->Color;
}
//---------------------------------------------------------------------------
void __fastcall TStreamEdit::ChangeColor(TObject *Sender)
{
ColorDialog->Execute();
((TMemo*)Sender)->Color=ColorDialog->Color;

}
//---------------------------------------------------------------------------



void __fastcall TStreamEdit::StringFontMenuItemClick(TObject *Sender)
{
if(FontDialog->Execute())
 StringMemo->Font=FontDialog->Font;
}
//---------------------------------------------------------------------------

void __fastcall TStreamEdit::PointerFontMenuItemClick(TObject *Sender)
{
if(FontDialog->Execute())
PointerMemo->Font=FontDialog->Font;
}
//---------------------------------------------------------------------------

void __fastcall TStreamEdit::HexFontMenuItemClick(TObject *Sender)
{
if(FontDialog->Execute())
HexMemo->Font=FontDialog->Font;
}
//---------------------------------------------------------------------------
void __fastcall TStreamEdit::StringColorMenuItemClick(TObject *Sender)
{
if(ColorDialog->Execute())
 StringMemo->Color=ColorDialog->Color;
}
//---------------------------------------------------------------------------

void __fastcall TStreamEdit::PointerColorMenuItemClick(TObject *Sender)
{
if(ColorDialog->Execute())
PointerMemo->Color=ColorDialog->Color;
}
//---------------------------------------------------------------------------

void __fastcall TStreamEdit::HexColorMenuItemClick(TObject *Sender)
{
if(ColorDialog->Execute())
HexMemo->Color=ColorDialog->Color;
}
//---------------------------------------------------------------------------

void __fastcall TStreamEdit::EditBufMenuClick(TObject *Sender)
{
TMenuItem* menu=(TMenuItem*)Sender;

int num=menu->Caption.ToInt();

TEditMemoryForm* EditMemoryForm=new TEditMemoryForm(this);

//BufferClipBoard.Value->Write(stream,true);

EditMemoryForm->Width=500;
EditMemoryForm->Height=400;
EditMemoryForm->StreamEdit->stream=(TMemoryStream*)ClipBoard->Items[num];


EditMemoryForm->Show();
/*BufferClipBoard.BeginWork();
BufferClipBoard.SetPosition(num);
stream->Position=0;
BufferClipBoard.Value->Read(stream);*/


}
//---------------------------------------------------------------------------
void __fastcall TStreamEdit::StringsOptionChangedEvent(TObject *Sender,DWORD value)
{
try
{
 NumberOfString=AnsiToulong(StringNumberEdit->Text.c_str());
 StringLen=AnsiToulong(StringLengthEdit->Text.c_str());
}
catch(...)
 {

 }
 LoadFromStream();
}
//---------------------------------------------------------------------------


int __fastcall TStreamEdit::ConvertGlobalToStringPos(int value)
{

 return (StringLen+2)*(value/StringLen)+(value%StringLen);
}

int __fastcall TStreamEdit::ConvertStringPosToGlobal(int value)
{
int res=0;
res+=StringLen*(value/(StringLen+2))+(value%(StringLen+2));

return res;
}
void __fastcall TStreamEdit::StringRichEditKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
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
            ShiftStringSelStart(-StringLen);
           }
          else
           {
            if(Key==0x28)//down
             {
               ShiftStringSelStart(StringLen);
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
               ShiftStringSelStart(StringLen*NumberOfString);
                 }
                else
                 {
                  if(Key==0x21)//PgUp
                   {
                      ShiftStringSelStart(-StringLen*NumberOfString);
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
                         int pos=ConvertStringPosToGlobal(StringMemo->SelStart);
                          HexMemo->SelStart=ConvertGlobalToHexPos(pos);
                         HexMemo->SetFocus();
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


void __fastcall TStreamEdit::ShiftStringSelStart(int Shift)
{
int value=StringMemo->SelStart;
int Offset=ConvertStringPosToGlobal(value);
int OldOffset=Offset;
Offset+=Shift;

if((Offset+CurrentPos>=0)&&(Offset+CurrentPos<stream->Size))
 {
   if((Offset<0)||(Offset>=ViewedLen))
    {
     CurrentPos+=((Shift/(StringLen)))*StringLen +Shift%StringLen;
     Offset=OldOffset;
    }
    StringMemo->SelStart=ConvertGlobalToStringPos(Offset);
 }
}

void __fastcall TStreamEdit::StringRichEditKeyPres(unsigned char value)
{
int pos=ConvertStringPosToGlobal(StringMemo->SelStart);
int hexpos=ConvertGlobalToHexPos(pos);
HexMemo->SelStart=hexpos;
HexMemo->SelLength=2;
char ch[2];
HexMemo->SelText=byteptrToHexAnsi(&value,1).c_str();
ch[0]=ConvertToPrintSign(value);
ch[1]=0;
stream->Position=CurrentPos+pos;
StringMemo->SelLength=1;
StringMemo->SelText=AnsiString(ch);
StringMemo->SelLength=0;

stream->Write(ch,1);
ShiftIfWrongStringSelStart();

}
void __fastcall TStreamEdit::StringRichEditKeyPress(TObject *Sender,
      char &Key)
{
if(Key!=0x9)// not tab
 StringRichEditKeyPres(Key);
Key=0;
}
//---------------------------------------------------------------------------


void __fastcall TStreamEdit::ShiftIfWrongStringSelStart(void)
{
if(StringMemo->SelStart%(StringLen+2)==StringLen)
 {
  if(ViewedLen==(StringMemo->SelStart/(StringLen+2)+1)*StringLen)
   {
    StringMemo->SelStart--;
    ShiftStringSelStart(1);
   }
  else
   StringMemo->SelStart+=2;
 }
}

void __fastcall TStreamEdit::StringRichEditMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int Start=ConvertStringPosToGlobal(StringMemo->SelStart);
int End=ConvertStringPosToGlobal(StringMemo->SelStart+StringMemo->SelLength);
HexMemo->SelStart=ConvertGlobalToHexPos(Start);
HexMemo->SelLength=ConvertGlobalToHexPos(End)-ConvertGlobalToHexPos(Start);
}
//---------------------------------------------------------------------------

void __fastcall TStreamEdit::HexRichEditMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int Start=ConvertHexPosToGlobal(HexMemo->SelStart);
int End=ConvertHexPosToGlobal(HexMemo->SelStart+HexMemo->SelLength);
StringMemo->SelStart=ConvertGlobalToStringPos(Start);
StringMemo->SelLength=ConvertGlobalToStringPos(End)-ConvertGlobalToStringPos(Start);
}
//---------------------------------------------------------------------------
void __fastcall TStreamEdit::PointersNotifyEvent(int value)
 {
  CurrentPos=value;
 }
//---------------------------------------------------------------------------
void __fastcall TStreamEdit::Setstream(TStream* value)
{

   Fstream=value;
   LoadFromStream();
}
//------------------------------------------------------------
void __fastcall TStreamEdit::ContinueSearchMenuItemClick(
      TObject *Sender)
{
Search(false);

}
//---------------------------------------------------------------------------
/*bool __fastcall TStreamEdit::Search(bool IsNewSearch)
{
if(Searcher==NULL) throw Exception("SerError3");
TDateTime StartSearch=TDateTime::CurrentDateTime();
TDateTime EndSearch;
unsigned long len;
bool SearchResult;
VoidInt vi;
Searcher->NotifyEvent=DoProgress;
ProgressBar->Max=stream->Size;
ProgressBar->Min=0;ProgressBar->Position=0;
if(!SearcherProperties->Find->IsEmpty())
 {
  Searcher->Find->Clear();
  SearcherProperties->Find->WriteDataToStream(Searcher->Find);
if(IsNewSearch) {ProgressBar->Max=Searcher->stream->Size;SearchResult=Searcher->StartSearch();}
 else {ProgressBar->Max=Searcher->Pointers->Count;SearchResult=Searcher->ContinueSearch();}
if(SearchResult)
 {
  if(Searcher->Pointers->Count<200)
   {
     for(int i=0;i<Searcher->Pointers->Count;i++)
      {
       vi.v=Searcher->Pointers->Items[i];
       GotoInputGroupBox->Strings->Add(ulongToHexAnsi(vi.ul));
      }
   }
 }
 }
EndSearch=TDateTime::CurrentDateTime();
ProgressBar->Position=0;

InfoEdit->Lines->Add(AnsiString("Search started at ") +StartSearch.DateTimeString()+AnsiString(" ")+AnsiString("finished at ")+EndSearch.DateTimeString()+AnsiString(" search time ")+(EndSearch-StartSearch).TimeString()+AnsiString(" Found :")+AnsiString(Searcher->Pointers->Count)+AnsiString(" matches."));
SearcherProperties->Searcher=Searcher;
return Searcher->Pointers->Count>0;
}
*/

bool __fastcall TStreamEdit::Search(bool IsNewSearch)
{
TDateTime StartSearch=TDateTime::CurrentDateTime();
TDateTime EndSearch;
int PointersFound;
ProgressBar->Max=stream->Size;
ProgressBar->Min=0;ProgressBar->Position=0;

PointersFound=SearcherProperties->Search(IsNewSearch,stream,DoProgress);
ProgressBar->Position=0;
EndSearch=TDateTime::CurrentDateTime();
InfoEdit->Lines->Add(AnsiString("Search started at ") +StartSearch.DateTimeString()+AnsiString(" ")+AnsiString("finished at ")+EndSearch.DateTimeString()+AnsiString(" search time ")+(EndSearch-StartSearch).TimeString()+AnsiString(" Found :")+AnsiString(PointersFound)+AnsiString(" matches."));
return    PointersFound>0;
}


bool __fastcall TStreamEdit::SlowSearch(void)
{
TDateTime StartSearch=TDateTime::CurrentDateTime();
TDateTime EndSearch;
int PointersFound;
ProgressBar->Max=stream->Size;
ProgressBar->Min=0;ProgressBar->Position=0;

PointersFound=SearcherProperties->SlowSearch(stream,DoProgress);
ProgressBar->Position=0;
EndSearch=TDateTime::CurrentDateTime();
InfoEdit->Lines->Add(AnsiString("Search started at ") +StartSearch.DateTimeString()+AnsiString(" ")+AnsiString("finished at ")+EndSearch.DateTimeString()+AnsiString(" search time ")+(EndSearch-StartSearch).TimeString()+AnsiString(" Found :")+AnsiString(PointersFound)+AnsiString(" matches."));
return    PointersFound>0;
}




//----------------------------------------------
void __fastcall TStreamEdit::DoProgress(int pos)
{
 ProgressBar->Position=pos;
}

void __fastcall TStreamEdit::GetVariablesAtPos(int pos)
{
WORK_ANSILIB_UNION_FOR_CONVERT buf;
stream->Position=pos;
stream->Read(&buf,sizeof(buf));
AnsiString newInfo=AnsiString("char ")+AnsiString((int)buf.char_)+AnsiString(",");
newInfo+=AnsiString("byte ")+ulongToAnsi(buf.byte_).c_str()+AnsiString(",");
newInfo+=AnsiString("ulong ")+ulongToAnsi(buf._ulong).c_str()+AnsiString(",");
newInfo+=AnsiString("int ")+AnsiString(buf._int)+AnsiString(",");
try
{
newInfo+=AnsiString("double ")+AnsiString(buf.double_)+AnsiString(",");

}
catch(...)
{
newInfo+=AnsiString("double ")+AnsiString("NAN")+AnsiString(",");
}

try
{

newInfo+=AnsiString("float ")+AnsiString(buf.float_)+AnsiString(",");
}
catch(...)
{
newInfo+=AnsiString("float ")+AnsiString("NAN")+AnsiString(",");
}
CurrectEditSelection=newInfo;
//InfoLabel=AnsiString("int(")+
}



void __fastcall TStreamEdit::ReplaceAllMenuItemClick(TObject *Sender)
{
SearcherProperties->ReplaceAll();
}

//-------------------------------------------------------------------------
void __fastcall TStreamEdit::SetKeyPressed(AnsiString value)
{
FKeyPressed=value;
UpdateInfoString();
}

void __fastcall TStreamEdit::SetCurrectEditSelection(AnsiString value)
{
FCurrectEditSelection=value;
UpdateInfoString();
}

void __fastcall TStreamEdit::UpdateInfoString(void)
{
if(InfoEdit->Lines->Count==0)
 {
InfoEdit->Lines->Add(" ");
 }
InfoEdit->Lines->Strings[0]=KeyPressed+AnsiString("   ")+CurrectEditSelection;
InfoEdit->SelStart=0;
}


//-------------------------------------------------------------
TSearcher* __fastcall TStreamEdit::GetSearcher(void)
{
return SearcherProperties->Searcher;
}
