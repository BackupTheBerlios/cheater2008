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

#include "StreamEdit.h"

int NumberOfCopies=0;
char* WhatFind="Search query";
std::vector< boost::shared_ptr<std::stringstream> > ClipBoard;     // clipboard for selections

enum
{
  ProgressBar_ID = 0xF000,
  InfoEdit_ID,
  StringMemo_ID,
  PointerMemo_ID,
  HexMemo_ID
};

IMPLEMENT_DYNAMIC(TStreamEdit, CMyBaseForm)

BEGIN_MESSAGE_MAP(TStreamEdit, CMyBaseForm)
  ON_WM_KEYDOWN()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_INITMENU()
  ON_WM_INITMENUPOPUP()
  ON_WM_VSCROLL()
  ON_WM_RBUTTONDOWN()
  ON_WM_VKEYTOITEM()
END_MESSAGE_MAP()

//---------------------------------------------------------------------------
 TStreamEdit::TStreamEdit(CWnd* pParent /*= NULL*/):
CMyBaseForm(pParent)
{

if(NumberOfCopies==0)
 {
//  ClipBoard=new TList();
 }
Fstream=boost::shared_ptr<std::iostream>((std::iostream*)0);
FCurrentPos=0;
ViewedLen=0;
NumberOfString=20;
StringLen=16;
IsProcessKeyDown=false;

SearcherProperties->setOnSelectPointer( TPointerSelectEventPtr (new TPointerSelectEvent(boost::bind(&TStreamEdit::PointersNotifyEvent,this,_1)) ) );

NumberOfCopies++;
}
//---------------------------------------------------------------------------
void TStreamEdit::initializePopupMenu()
{
  //-------------------Upper all OK--------------------------------------
  PopupMenu=new TPopupMenu();
  PopupMenu->CreatePopupMenu();

  PopupMenu->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::SlowSearchMenuItemClick,this)) )), 
    CString( "Slow Search") );


//  PopupMenu->OnPopup=PopupMenu1Popup;


  PopupMenu->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::SearchMenuItemClick,this)) )), 
    CString( "Search") );


  PopupMenu->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::ContinueSearchMenuItemClick,this)) )), 
    CString( "Continue Search") );


  PopupMenu->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::LoadFromStreamMenuitemClick,this)) )), 
    CString( "ReLoad") );


  PopupMenu->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::GotoMenuItemClick,this)) )), 
    CString( "Goto...") );


  PopupMenu->AppendMenu(MF_SEPARATOR,0,CString("-") );


  PopupMenu->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::CopyMenuItemClick,this)) )), 
    CString( "Copy") );

  PopupMenu->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::PasteMenuItemClick,this)) )), 
    CString( "Paste") );

  PopupMenu->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::PasteMenuItemClick,this)) )), 
    CString( "Paste") );

  PopupMenu->AppendMenu(MF_SEPARATOR,0,CString("-"));

  CopytoMenuItem=new CMenuItem();
  CopytoMenuItem->CreatePopupMenu();
  PopupMenu->AppendMenu(MF_POPUP, (UINT_PTR)CopytoMenuItem->m_hMenu, CString( "Coptyto..." ) ); //6


  PasteFromMenuItem=new CMenuItem();
  PasteFromMenuItem->CreatePopupMenu();
  PopupMenu->AppendMenu(MF_POPUP, (UINT_PTR)PasteFromMenuItem->m_hMenu, CString( "Paste from" ) ); //6

  EditBufMenuItem=new CMenuItem();
  EditBufMenuItem->CreatePopupMenu();
  PopupMenu->AppendMenu(MF_POPUP, (UINT_PTR)EditBufMenuItem->m_hMenu, CString( "Edit buf" ) ); //6


  DeleteMenuItem=new CMenuItem();
  DeleteMenuItem->CreatePopupMenu();
  PopupMenu->AppendMenu(MF_POPUP, (UINT_PTR)DeleteMenuItem->m_hMenu, CString( "Delete" ) ); //6


  PopupMenu->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::ClearClipBoardMenuItemClick,this)) )), 
    CString( "Clear ClipBoard") );

  PopupMenu->AppendMenu(MF_SEPARATOR,0,CString("-"));

  PopupMenu->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::Color1Click,this)) )), 
    CString( "Color") );


  PopupMenu->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::Font1Click,this)) )), 
    CString( "Font") );



  FonCMenuItem=new CMenuItem();
  FonCMenuItem->CreatePopupMenu();
  PopupMenu->AppendMenu(MF_POPUP, (UINT_PTR)FonCMenuItem->m_hMenu, CString( "Font" ) ); //6

  //--------------------------------------------------

  FonCMenuItem->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::StringFonCMenuItemClick,this)) )), 
    CString( "String") );


  FonCMenuItem->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::HexFonCMenuItemClick,this)) )), 
    CString( "Hex") );

  FonCMenuItem->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::PointerFonCMenuItemClick,this)) )), 
    CString( "Pointer") );

  //--------------------------------------------------

  ColorMenuItem=new CMenuItem();
  ColorMenuItem->CreatePopupMenu();
  PopupMenu->AppendMenu(MF_POPUP, (UINT_PTR)ColorMenuItem->m_hMenu, CString( "Color" ) ); //6
  
  
  ColorMenuItem->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::StringColorMenuItemClick,this)) )), 
    CString( "String") );

  ColorMenuItem->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::HexColorMenuItemClick,this)) )), 
    CString( "Hex") );


  ColorMenuItem->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::PointerColorMenuItemClick,this)) )), 
    CString( "Pointer") );

  //--------------------------------------------------
  PopupMenu->AppendMenu(MF_SEPARATOR,0,CString("-") );
  //---------------------------------------
  PopupMenu->AppendMenu(MF_STRING, 
    d_menuCommands.createCommand( 
    CommandPtr(new Command(boost::bind(&TStreamEdit::ReplaceAllMenuItemClick,this)) )), 
    CString( "Replace All") );
  //------------------------------------------------------

}
void  TStreamEdit::initialize(void)
{
  d_splitterWnd = new SplitterCtrl_NS::SplitterCtrl();
  d_splitterWndTop = new SplitterCtrl_NS::SplitterCtrl();
  d_splitterWndBottom = new SplitterCtrl_NS::SplitterCtrl();

  d_splitterWndTopLeft = new SplitterCtrl_NS::SplitterCtrl();
  d_splitterWndTopRight = new SplitterCtrl_NS::SplitterCtrl();
  d_splitterWndTopRightBox = new CStatic();
  d_splitterWndTopRightBox->Create(CString(""),SS_SIMPLE,CRect(0,0,0,0),d_splitterWndTopRight);
  d_splitterWndTopRightBox->ShowWindow(SW_SHOW);

  initializePopupMenu();
//-------------- Bottom pane --------------------------------
  ProgressBar=new TProgressBar();
  ProgressBar->Create(PBS_SMOOTH , CRect(0,0,0,0), d_splitterWndBottom, ProgressBar_ID);
  ProgressBar->ShowWindow( SW_SHOW );

  InfoEdit=new TRichEdit(this);
  InfoEdit->Create(ES_MULTILINE,CRect(0,0,0,0),d_splitterWndBottom,InfoEdit_ID);
  InfoEdit->ShowWindow( SW_SHOW );
  d_splitterWndBottom->AddRow();
  d_splitterWndBottom->AddColumn();
  d_splitterWndBottom->AddColumn();
  d_splitterWndBottom->SetWindow(0,0,InfoEdit->m_hWnd);
  d_splitterWndBottom->SetWindow(0,0,ProgressBar->m_hWnd);
  d_splitterWndBottom->Update();

ColorDialog=new TColorDialog();
FontDialog=new TFontDialog();
//--------------Top right panel-----------------------------------
  GotoInputGroupBox=new InputGroupBox();
  GotoInputGroupBox->Create(d_splitterWndTopRightBox);
  GotoInputGroupBox->ShowWindow(SW_SHOW);

  StringNumberEdit=new TCountEdit();
  StringNumberEdit->Create(d_splitterWndTopRightBox);
  StringNumberEdit->setText("Number of lines");
  StringNumberEdit->OnChangeEvent = CommandPtr(new Command(boost::bind(&TStreamEdit::StringsOptionChangedEvent,this) )); 
  StringNumberEdit->ShowWindow(SW_SHOW);

  StringLengthEdit=new TCountEdit();
  StringLengthEdit->Create(d_splitterWndTopRightBox);
  StringLengthEdit->OnChangeEvent = CommandPtr(new Command(boost::bind(&TStreamEdit::StringsOptionChangedEvent,this) )); 
  StringLengthEdit->setText("Line's length");
  StringLengthEdit->ShowWindow(SW_SHOW);

  SearcherProperties=new TSearcherProperties();
  SearcherProperties->Create(d_splitterWndTopRight);
  SearcherProperties->ShowWindow(SW_SHOW);

  d_splitterWndTopRight->AddRow();
  d_splitterWndTopRight->AddRow();
  d_splitterWndTopRight->AddColumn();

  d_splitterWndTopRight->SetWindow(0,0,d_splitterWndTopRightBox->m_hWnd);
  d_splitterWndTopRight->SetWindow(0,1,SearcherProperties->m_hWnd);
  d_splitterWndTopRight->Update();

//--------------Top left panel-----------------------------------
// throwException("Sergey3");

   StringMemo=new CEdit();
   StringMemo->Create(ES_MULTILINE,CRect(0,0,0,0),d_splitterWndTopLeft,StringMemo_ID);
#pragma message ("TODO: implement event handlers")
//   StringMemo->OnKeyDown=StringRichEditKeyDown;
//   StringMemo->OnKeyPress=StringRichEditKeyPress;
// StringMemo->OnMouseMove=StringRichEditMouseMove;


   PointerMemo=new CEdit();
   PointerMemo->Create(ES_MULTILINE,CRect(0,0,0,0),d_splitterWndTopLeft,PointerMemo_ID);

   HexMemo=new CEdit();
   HexMemo->Create(ES_MULTILINE,CRect(0,0,0,0),d_splitterWndTopLeft,HexMemo_ID);
#pragma message ("TODO: implement event handlers")
   //   HexMemo->OnKeyDown=HexRichEditKeyDown;
   //   HexMemo->OnKeyPress=HexRichEditKeyPress;
   //   HexMemo->OnMouseMove=HexRichEditMouseMove;

  d_splitterWndTopLeft->AddRow();
  d_splitterWndTopLeft->AddColumn();
  d_splitterWndTopLeft->AddColumn();
  d_splitterWndTopLeft->AddColumn();

  d_splitterWndTopLeft->SetWindow(0,0,PointerMemo->m_hWnd);
  d_splitterWndTopLeft->SetWindow(0,1,HexMemo->m_hWnd);
  d_splitterWndTopLeft->SetWindow(0,2,StringMemo->m_hWnd);

//-------------------- Joining Top panes -----------------------
  d_splitterWndTop->AddRow();
  d_splitterWndTop->AddColumn();
  d_splitterWndTop->AddColumn();
  d_splitterWndTop->SetWindow(0,0,d_splitterWndTopLeft->m_hWnd);
  d_splitterWndTop->SetWindow(0,1,d_splitterWndTopRight->m_hWnd);

  d_splitterWnd->AddRow();
  d_splitterWnd->AddRow();
  d_splitterWnd->AddColumn();

  d_splitterWndTop->SetWindow(0,0,d_splitterWndTop->m_hWnd);
  d_splitterWndTop->SetWindow(1,0,d_splitterWndBottom->m_hWnd);

//--------------- Joining Top and bottom ---------------------

   d_splitterWnd->ShowWindow(SW_SHOW);
   d_splitterWndTop->ShowWindow(SW_SHOW);
   d_splitterWndBottom->ShowWindow(SW_SHOW);

   d_splitterWndTopLeft->ShowWindow(SW_SHOW);
   d_splitterWndTopRight->ShowWindow(SW_SHOW);


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
     GroupBox1->Caption = caption.str().c_str();
     getStream()->seekg(CurrentPos,std::ios_base::beg);

    for(int i=0;(i<NumberOfString)&&(RealRead!=0);i++)
      {
       if((RealRead=getStream()->read(ch,StringLen).gcount())>0)
        {
           PointerMemo->Lines->Add(ulongTo8digitHexString(getStream()->tellg()-RealRead).c_str());
		   HexMemo->Lines->Add(charptrToHexAnsiWithSpace(ch,RealRead).c_str());
		   StringMemo->Lines->Add(ConvertToPrintString(ch,RealRead).c_str());
           ViewedLen+=RealRead;
        }
       else
        if(i==0)
         if(getStream()->tellg()>FCurrentPos) FCurrentPos=getStream()->tellg();
      }
   }
 }
catch(std::exception& e)
 {
   ShowMessage()
  Application->ShowException(&e);
 }
  delete ch;
  HexMemo->Lines->EndUpdate();
  StringMemo->Lines->EndUpdate();
  PointerMemo->Lines->EndUpdate();
  StringNumberEdit->SetWindowText(CString(ulongToAnsi(NumberOfString).c_str()));
  StringLengthEdit->SetWindowText(CString(ulongToAnsi(StringLen).c_str()));

  HexMemo->SelStart=HexMemoSelStart;
  StringMemo->SelStart=StringMemoSelStart;
 }//enf if (Showing)
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
res+=StringLen*(value/(3*StringLen+1))+(value%(3*StringLen+1))/3;
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
InfoEdit->SetWindowText(  CString(std::string("Key: "+ulongToHexAnsi(Key).c_str()+" "+_shift).c_str()) );
Key=0;
}
//---------------------------------------------------------------------------
void  TStreamEdit::GotoMenuItemClick()
{
  setCurrentPos(GotoInputGroupBox->GetPointer());
}
//---------------------------------------------------------------------------
void  TStreamEdit::HexRichEditKeyPres(unsigned char value)
{
int hexStart,hexEnd;
HexMemo->GetSel(hexStart,hexEnd);
int pos=ConvertHexPosToGlobal(hexStart);
int rem;
int hexPos=ConvertGlobalToHexPos(pos);
const char* ch;
char print[2];
print[1]=0;
int StringPos=ConvertGlobalToStringPos(pos);


StringMemo->SetSel(StringPos,StringPos+1);


getStream()->seekp(FCurrentPos+pos,std::ios_base::beg);

StringMemo->SetSel(hexStart,hexStart+1);
std::stringstream str;
str << std::hex << value;
HexMemo->ReplaceSel(CString(str.str().c_str()));

HexMemo->GetSel(hexStart,hexEnd);
rem=hexStart;

HexMemo->SetSel(hexPos,hexPos+2);

ch=HexAnsiTobyteptr(HexMemo->SelText.c_str());
print[0]=ConvertToPrintSign(*ch);
StringMemo->SelText=AnsiString(print);
HexMemo->SelLength=0;
getStream()->write(ch,1);
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

int  TStreamEdit::ConvertGlobalToHexPos(int value)
{
  //Edit2->Text=AnsiString((3*StringLen+1)*(value/StringLen)+(value%StringLen)*3);
  return (3*StringLen+1)*(value/StringLen)+(value%StringLen)*3;
}

void  TStreamEdit::HexRichEditKeyPress(unsigned char Key)
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

void  TStreamEdit::setCurrentPos( int value)
{
if((value<getStream()->seekg(0,std::ios_base::end).tellg())&&(value>=0))
 {
  FCurrentPos=value;
  LoadFromStream();
 }
}

void  TStreamEdit::ShiftHexSelStart(int Shift)
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

if((Offset+FCurrentPos>=0)&&(Offset+FCurrentPos<getStream()->seekg(0,std::ios_base::end).tellg()))
 {
   if((Offset<0)||(Offset>=ViewedLen))
    {
     FCurrentPos+=((Shift/(2*StringLen)))*StringLen+Shift%2;
     Offset=OldOffset;
    }
    HexMemo->SelStart=ConvertGlobalToHexPos(Offset)+InBytePos;
 }
  GetVariablesAtPos(CurrentPos+Offset);
}

void  TStreamEdit::CopyMenuItemClick()
{
int Start;
int End;
int Len;
boost::shared_ptr<std::stringstream> buf=boost::shared_ptr<std::stringstream>(new std::stringstream());
if(HexMemo->SelLength>0)
  {
    Start=ConvertHexPosToGlobal(HexMemo->SelStart);
    End=ConvertHexPosToGlobal(HexMemo->SelStart+HexMemo->SelLength);
  }
 else
  {
    End=ConvertHexPosToGlobal(HexMemo->SelStart);
    Start=ConvertHexPosToGlobal(HexMemo->SelStart+HexMemo->SelLength);
  }
  Len=End-Start+1;
  getStream()->seekg(Start+CurrentPos,std::ios_base::beg);
  //   copy
  std::vector<char> myBuf;
  myBuf.resize(Len);
  Len = getStream()->read(&(myBuf[0]),Len).gcount();
  buf->write(&(myBuf[0]),Len);
//  Len=buf->CopyFrom(stream.get(),Len);
ClipBoard.push_back(buf);
}
//---------------------------------------------------------------------------
void  TStreamEdit::PopupMenu1Popup(TObject *Sender)
{
int i;
int len;
CMenuItem* menu;
AnsiString caption;
 CopytoMenuItem->Clear();
 PasteFromMenuItem->Clear();
 DeleteMenuItem->Clear();
 EditBufMenuItem->Clear();
 EditBufMenuItem->Enabled=(! ClipBoard.empty());
    DeleteMenuItem->Enabled=(! ClipBoard.empty());
    PasteFromMenuItem->Enabled=(! ClipBoard.empty());
    PasteMenuItem->Enabled=(! ClipBoard.empty());
    CopytoMenuItem->Enabled=((! ClipBoard.empty())&&(HexMemo->SelLength!=0));
    ClearClipBoardMenuItem->Enabled=(! ClipBoard.empty());
    CopyMenuItem->Enabled=(HexMemo->SelLength!=0);
for(i=0;i<ClipBoard.size();i++)
 {
   caption=ulongToAnsi(i).c_str();
   menu= new CMenuItem(DeleteMenuItem);
   menu->Caption=caption;
   menu->OnClick=DeleteSubMenuClick;
   DeleteMenuItem->Add(menu);

   menu= new CMenuItem(CopytoMenuItem);
   menu->Caption=caption;
   menu->OnClick=CopytoMenuClick;
   CopytoMenuItem->Add(menu);

   menu= new CMenuItem(PasteFromMenuItem);
   menu->Caption=caption;
   menu->OnClick=PasteFromMenuClick;
   PasteFromMenuItem->Add(menu);

   menu= new CMenuItem(EditBufMenuItem);
   menu->Caption=caption;
   menu->OnClick=EditBufMenuClick;
   EditBufMenuItem->Add(menu);
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
  int Start=ConvertHexPosToGlobal(HexMemo->SelStart);
  // initialize current position
  getStream()->seekp(Start+CurrentPos, std::ios_base::beg);
  // set read pointer to the beginnig
  ClipBoard.back().get()->seekg(0,std::ios_base::beg);
  copyTheRest(*getStream(),*(ClipBoard.back()));
//  stream->CopyFrom(ClipBoard.back().get(),0);
  LoadFromStream();
}
//---------------------------------------------------------------------------
void  TStreamEdit::DeleteSubMenuClick(TObject *Sender)
{
  CMenuItem* menu=(CMenuItem*)Sender;
  int num = menu->Caption.ToInt();
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
void  TStreamEdit::CopytoMenuClick(TObject *Sender)
{
  CMenuItem* menu=(CMenuItem*)Sender;
  int num=menu->Caption.ToInt();
  int Start;
  int End;
  int Len;

  if(HexMemo->SelLength>0)
    {
      Start=ConvertHexPosToGlobal(HexMemo->SelStart);
      End=ConvertHexPosToGlobal(HexMemo->SelStart+HexMemo->SelLength);
    }
  else
    {
      End=ConvertHexPosToGlobal(HexMemo->SelStart);
      Start=ConvertHexPosToGlobal(HexMemo->SelStart+HexMemo->SelLength);
    }
  Len=End-Start+1;
  getStream()seekg(Start+CurrentPos, std::ios_base::beg);
  if( num < ClipBoard.size() )
  {
    boost::shared_ptr<std::stringstream> memory = ((*(ClipBoard.begin()+num)));
    memory->str("");
    copyTheRest(*memory,*stream,Len);  // clear content
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
void  TStreamEdit::PasteFromMenuClick(TObject *Sender)
{
CMenuItem* menu=(CMenuItem*)Sender;
int num=menu->Caption.ToInt();

int Start=ConvertHexPosToGlobal(HexMemo->SelStart);
getStream()seekp(Start+CurrentPos,std::ios_base::beg);
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
  FontDialog->Execute();
  HexMemo->Font=FontDialog->Font;
  StringMemo->Font=FontDialog->Font;
  PointerMemo->Font=FontDialog->Font;
}
//---------------------------------------------------------------------------
void  TStreamEdit::Color1Click()
{
  ColorDialog->Execute();
  HexMemo->Color=ColorDialog->Color;
  StringMemo->Color=ColorDialog->Color;
  PointerMemo->Color=ColorDialog->Color;
}
//---------------------------------------------------------------------------
void  TStreamEdit::ChangeColor()
{
  ColorDialog->Execute();
  ((TMemo*)Sender)->Color=ColorDialog->Color;
}
//---------------------------------------------------------------------------
void  TStreamEdit::StringFonCMenuItemClick()
{
if(FontDialog->Execute())
 StringMemo->Font=FontDialog->Font;
}
//---------------------------------------------------------------------------
void  TStreamEdit::PointerFonCMenuItemClick()
{
  if(FontDialog->Execute())
    PointerMemo->Font=FontDialog->Font;
}
//---------------------------------------------------------------------------
void  TStreamEdit::HexFonCMenuItemClick()
{
  if(FontDialog->Execute())
  HexMemo->Font=FontDialog->Font;
}
//---------------------------------------------------------------------------
void  TStreamEdit::StringColorMenuItemClick()
{
  if(ColorDialog->Execute())
    StringMemo->Color=ColorDialog->Color;
}
//---------------------------------------------------------------------------
void  TStreamEdit::PointerColorMenuItemClick()
{
  if(ColorDialog->Execute())
    PointerMemo->Color=ColorDialog->Color;
}
//---------------------------------------------------------------------------
void  TStreamEdit::HexColorMenuItemClick()
{
  if(ColorDialog->Execute())
    HexMemo->Color=ColorDialog->Color;
}
//---------------------------------------------------------------------------
void  TStreamEdit::EditBufMenuClick(TObject *Sender)
{
  CMenuItem* menu=(CMenuItem*)Sender;
  int num=menu->Caption.ToInt();
if( num < ClipBoard.size() )
  {
    TEditMemoryForm* EditMemoryForm=new TEditMemoryForm(this);
      //BufferClipBoard.Value->Write(stream,true);
      EditMemoryForm->Width=500;
      EditMemoryForm->Height=400;
      EditMemoryForm->StreamEdit->stream=(*(ClipBoard.begin() + num));
      EditMemoryForm->Show();
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
    NumberOfString=AnsiToulong(StringNumberEdit->Text.c_str());
    StringLen=AnsiToulong(StringLengthEdit->Text.c_str());
  }
  catch(...)
  {
  }
  LoadFromStream();
}
//---------------------------------------------------------------------------
int  TStreamEdit::ConvertGlobalToStringPos(int value)
{
 return (StringLen+2)*(value/StringLen)+(value%StringLen);
}

int  TStreamEdit::ConvertStringPosToGlobal(int value)
{
  int res=0;
  res+=StringLen*(value/(StringLen+2))+(value%(StringLen+2));
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
void  TStreamEdit::ShiftStringSelStart(int Shift)
{
int value=StringMemo->SelStart;
int Offset=ConvertStringPosToGlobal(value);
int OldOffset=Offset;
Offset+=Shift;

if((Offset+CurrentPos>=0)&&(Offset+CurrentPos<getStream()->seekg(0,std::ios_base::end).tellg()))
 {
   if((Offset<0)||(Offset>=ViewedLen))
    {
     CurrentPos+=((Shift/(StringLen)))*StringLen +Shift%StringLen;
     Offset=OldOffset;
    }
    StringMemo->SelStart=ConvertGlobalToStringPos(Offset);
 }
}

void  TStreamEdit::StringRichEditKeyPres(unsigned char value)
{
int pos=ConvertStringPosToGlobal(StringMemo->SelStart);
int hexpos=ConvertGlobalToHexPos(pos);
HexMemo->SelStart=hexpos;
HexMemo->SelLength=2;
char ch[2];
HexMemo->SelText=byteptrToHexAnsi(&value,1).c_str();
ch[0]=ConvertToPrintSign(value);
ch[1]=0;
getStream()->seekp(CurrentPos+pos,std::ios_base::beg);
StringMemo->SelLength=1;
StringMemo->SelText=AnsiString(ch);
StringMemo->SelLength=0;

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

void  TStreamEdit::StringRichEditMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int Start=ConvertStringPosToGlobal(StringMemo->SelStart);
int End=ConvertStringPosToGlobal(StringMemo->SelStart+StringMemo->SelLength);
HexMemo->SelStart=ConvertGlobalToHexPos(Start);
HexMemo->SelLength=ConvertGlobalToHexPos(End)-ConvertGlobalToHexPos(Start);
}
//---------------------------------------------------------------------------

void  TStreamEdit::HexRichEditMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int Start=ConvertHexPosToGlobal(HexMemo->SelStart);
int End=ConvertHexPosToGlobal(HexMemo->SelStart+HexMemo->SelLength);
StringMemo->SelStart=ConvertGlobalToStringPos(Start);
StringMemo->SelLength=ConvertGlobalToStringPos(End)-ConvertGlobalToStringPos(Start);
}
//---------------------------------------------------------------------------
void  TStreamEdit::PointersNotifyEvent(int value)
 {
  CurrentPos=value;
 }
//---------------------------------------------------------------------------
void  TStreamEdit::setStream(boost::shared_ptr<std::iostream> value)
{
   Fstream=value;
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

bool  TStreamEdit::Search(bool IsNewSearch)
{
DateTime StartSearch=TDateTime::CurrentDateTime();
TDateTime EndSearch;
int PointersFound;
getStream()->clear();// clear flags 
DWORD streamSize = getStream()->seekg(0,std::ios_base::end).tellg();
ProgressBar->Max=  100;
ProgressBar->Min=0;ProgressBar->Position=0;

PointersFound=SearcherProperties->Search(IsNewSearch,stream,DoProgress);
ProgressBar->Position=0;
EndSearch=TDateTime::CurrentDateTime();
InfoEdit->Lines->Add(AnsiString("Search started at ") +
StartSearch.DateTimeString()+
AnsiString(" ")+AnsiString("finished at ")+
EndSearch.DateTimeString()+AnsiString(" search time ")+(EndSearch-StartSearch).TimeString()+AnsiString(" Found :")+AnsiString(PointersFound)+AnsiString(" matches."));
return    PointersFound>0;
}

bool  TStreamEdit::SlowSearch(void)
{
TDateTime StartSearch=TDateTime::CurrentDateTime();
TDateTime EndSearch;
int PointersFound;
DWORD streamSize = getStream()->seekg(0,std::ios_base::end).tellg();
ProgressBar->Max=100;
ProgressBar->Min=0;ProgressBar->Position=0;

PointersFound=SearcherProperties->SlowSearch(stream,DoProgress);
ProgressBar->Position=0;
EndSearch=TDateTime::CurrentDateTime();
InfoEdit->Lines->Add(AnsiString("Search started at ") +StartSearch.DateTimeString()+AnsiString(" ")+AnsiString("finished at ")+EndSearch.DateTimeString()+AnsiString(" search time ")+(EndSearch-StartSearch).TimeString()+AnsiString(" Found :")+AnsiString(PointersFound)+AnsiString(" matches."));
return    PointersFound>0;
}
//----------------------------------------------
void  TStreamEdit::DoProgress(int pos)
{
 ProgressBar->Position=pos;
}

void  TStreamEdit::GetVariablesAtPos(int pos)
{
WORK_ANSILIB_UNION_FOR_CONVERT buf;
getStream()->seekg(pos,std::ios_base::beg);
getStream()->read((char*)&buf,sizeof(buf));
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



void  TStreamEdit::ReplaceAllMenuItemClick()
{
SearcherProperties->ReplaceAll();
}

//-------------------------------------------------------------------------
void  TStreamEdit::setKeyPressed(const std::string& i_value)
{
FKeyPressed=i_value;
UpdateInfoString();
}

void  TStreamEdit::setCurrectEditSelection(const std::string& i_value)
{
FCurrectEditSelection=i_value;
UpdateInfoString();
}

void  TStreamEdit::UpdateInfoString(void)
{
if(InfoEdit->Lines->Count==0)
 {
InfoEdit->Lines->Add(" ");
 }
InfoEdit->Lines->Strings[0]=KeyPressed+AnsiString("   ")+CurrectEditSelection;
InfoEdit->SelStart=0;
}

   
//-------------------------------------------------------------
boost::shared_ptr<TSearcher>  TStreamEdit::GetSearcher(void)
{
return SearcherProperties->Searcher;
}


void TStreamEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  Reload(nPos);  
  CMyBaseForm::OnVScroll(nSBCode,nPos,pScrollBar);
}

void TStreamEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

void TStreamEdit::OnInitMenu(CMenu* pMenu)
{
  CMyBaseForm::OnInitMenu(pMenu);
}

void TStreamEdit::OnInitMenuPopup(CMenu* pPopupMenu,UINT nIndex,BOOL bSysMenu)
{
CMyBaseForm::OnInitMenuPopup(pPopupMenu,nIndex,bSysMenu);
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
  PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, screen.x, 
    screen.y, this);

}

void TStreamEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  CRect pbRect;
  d_pointersBox->GetWindowRect(&pbRect);
  if(pbRect.PtInRect(point) == TRUE)
    PointersBoxDblClick(nFlags,point);
  else
    CMyBaseForm::OnLButtonDblClk(nFlags, point);
}

unsigned char getCharacter(UINT nKey)
{
  return nKey;
}

int TStreamEdit::OnVKeyToItem(UINT nKey,CListBox* pListBox,UINT nIndex)
{
if(!pListBox)
{
    std::stringstream msg;
    msg << "Corrupted pListBox!!!"  << std::endl << std::endl
        << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__ << std::endl;
    throw std::runtime_error( msg.str() );
}
unsigned char ch = getCharacter(nKey);

if(pListBox.m_hWnd==HexMemo.m_hWnd)
{
  if(ch)
    HexRichEditKeyPress(ch);
  else
    HexRichEditKeyDown(nKey);
}
else if(pListBox.m_hWnd==StringMemo.m_hWnd)
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
//http://msdn.microsoft.com/en-us/library/02z9kdt8.aspx
CMyBaseForm::OnVKeyToItem(nKey, pListBox,nIndex);
return -2;
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

