//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <boost/shared_ptr.hpp>
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ProcessChoiceFrame"
#pragma link "SearcherProperties"
#pragma link "ChitThreadProperties"
#pragma link "InputGroupBox"
#pragma link "MemoryInfoEdit"
#pragma link "CountEdit"
#pragma resource "*.dfm"
#include "inputdlg.h"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
InputForm=new TInputForm(this);
file=boost::shared_ptr<std::fstream>((std::fstream*)0);
pos=0;
IsProcessKeyDown=false;
TcpServer1->OnReceive=TcpServer1OnRecieveEvent;
Process=boost::shared_ptr<TProcessManager>(new TProcessManager());
new TChitThread(true);
}
//---------------------------------------------------------------------------
__fastcall TForm1::~TForm1()
{
  if(GlobalChit!=NULL) delete GlobalChit;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OpenFileMenuItemClick(TObject *Sender)
{
if(OpenDialog1->Execute())
 {
  file=boost::shared_ptr<std::fstream>(new std::fstream(OpenDialog1->FileName.c_str(),std::ios_base::out));
  this->EditMemoryFrame->stream=file;
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
//this->ProcessChoice1->Align=alClient;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CreateProcessMenuItemClick(TObject *Sender)
{
if(OpenDialog1->Execute())
 {
   Process->createProcess(OpenDialog1->FileName.c_str());
   EditMemoryFrame->stream=Process;
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SetStartMenuItemClick(TObject *Sender)
{
InputForm->InputBox->InitBox(ulongToHexAnsi(Process->getStart()),HEX_NUM);

if(InputForm->ShowModal()==mrOk)
 {
  Process->setStart ( InputForm->InputBox->GetPointer() );
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SetSizeMenuItemClick(TObject *Sender)
{
InputForm->InputBox->InitBox(ulongToHexAnsi(Process->getSize()),HEX_NUM);

if(InputForm->ShowModal()==mrOk)
 {
  Process->setSize(InputForm->InputBox->GetPointer());
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
ListBox->Items->Clear();
processNum=0;
DWORD cb=1024*sizeof(DWORD);
DWORD *lpidProcess=new DWORD[cb];
HMODULE *lphModule=new HMODULE[cb];
DWORD cbNeeded;
 EnumProcesses(
lpidProcess,  // array of process identifiers
  cb,            // size of array
  &cbNeeded      // number of bytes returned
);
for(DWORD i=0;i<cbNeeded/sizeof(DWORD);i++)
 {
 DWORD cbNeededModule;
HANDLE handle=OpenProcess(
    PROCESS_ALL_ACCESS	,	// access flag
    FALSE,	// handle inheritance flag
    lpidProcess[i] 	// process identifier
   );
 EnumProcessModules(
  handle,      // handle to process
  lphModule,   // array of module handles
   cb,             // size of array
  &cbNeededModule    // number of bytes required
  );
//Memo1->Lines->Add(AnsiString("Process id=   ")+AnsiString(lpidProcess[i]));
//for(int j=0;j<cbNeededModule/sizeof(HMODULE);j++)
//  {
char FileName[1024];
int realRead=GetModuleFileNameEx(
  handle,    // handle to process
  lphModule[0],    // handle to module
  FileName,  // path buffer
  90         // maximum characters to retrieve
);
  if(realRead>0) {ListBox->Items->Add(AnsiString("   ")+AnsiString(FileName,realRead));
idProcess[processNum]=lpidProcess[i];
processNum++;
  }
//  }
 }

delete[] lphModule;
delete[] lpidProcess;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListBoxClick(TObject *Sender)
{
Process->openProcess(    idProcess[ListBox->ItemIndex]);
   EditMemoryFrame->stream=Process;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
 byte* ch;
 int len;
 ch=InsertDilimeters(Edit1->Text.c_str(),Edit1->Text.Length(),Edit2->Text.c_str(),Edit2->Text.Length(),2,dlLeft,&len);
 Edit3->Text=AnsiString((char*)ch,len);
 delete ch;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button4Click(TObject *Sender)
{
 byte* ch;
 int len;
 ch=InsertDilimeters(Edit1->Text.c_str(),Edit1->Text.Length(),Edit2->Text.c_str(),Edit2->Text.Length(),2,dlRight,&len);
 Edit3->Text=AnsiString((char*)ch,len);
 delete ch;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button5Click(TObject *Sender)
{
 byte* ch;
 int len;
 ch=InsertDilimeters(Edit1->Text.c_str(),Edit1->Text.Length(),Edit2->Text.c_str(),Edit2->Text.Length(),2,dlLeftRight,&len);
 Edit3->Text=AnsiString((char*)ch,len);
 delete ch;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button6Click(TObject *Sender)
{
 byte* ch;
 int len;
 ch=InsertDilimeters(Edit1->Text.c_str(),Edit1->Text.Length(),Edit2->Text.c_str(),Edit2->Text.Length(),1,dlNone,&len);
 Edit3->Text=AnsiString((char*)ch,len);
 delete ch;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
 byte* ch;
 int len;
 ch=RemoveDilimeters(Edit3->Text.c_str(),Edit3->Text.Length(),Edit2->Text.c_str(),Edit2->Text.Length(),&len);
 Edit1->Text=AnsiString((char*)ch,len);
 if(Edit1->Text.c_str()!=ch)delete ch;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormShow(TObject *Sender)
{
ChitProperties->Searchers=TChitThread::GetGlobal()->Searchers;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SuspendClick(TObject *Sender)
{
TChitThread::GetGlobal()->Suspended=!Suspend->Checked;
Suspend->Checked=!Suspend->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::IntervalClick(TObject *Sender)
{
TInputForm* dlg= new TInputForm(NULL);
dlg->InputBox->InitBox(IntToString(TChitThread::GetGlobal()->sleep),DEC_NUM);
dlg->ShowModal();
if(dlg->ModalResult==mrOk)
 TChitThread::GetGlobal()->sleep=dlg->InputBox->GetPointer();
 delete dlg;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AddSearcherMenuItemClick(TObject *Sender)
{
if(EditMemoryFrame->stream!=NULL)
 {
  boost::shared_ptr<TSearcher> work=boost::shared_ptr<TSearcher>( new TSearcher(boost::shared_ptr<std::iostream>((std::iostream*)0)) );
  boost::shared_ptr<TSearcher> work2=EditMemoryFrame->GetSearcher();
 work->Assign(work2);
 GlobalChit->Searchers.push_back(work);
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChitOptionsClick(TObject *Sender)
{
//
Suspend->Checked=TChitThread::GetGlobal()->Suspended;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SetClick(TObject *Sender)
{
  Process->setMemoryInfo(MemoryInfo->GetMemoryInfo());
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ReloadClick(TObject *Sender)
{
MemoryInfo->SetMemoryInfo(Process->getMemoryInfo(MemPtr->GetPointer()));
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ServerSocket1ClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
{int Length=Socket->ReceiveLength();
char *buf=new char[Length+1];
      this->ListBox->Items->Add(AnsiString("Begin. Length= ")+AnsiString(Length));
      TStringList* ServerStrings=new TStringList();
      Socket->ReceiveBuf(buf,Length);
      buf[Length]=0;
      AnsiString text=AnsiString(buf);
      this->ListBox->Items->Add(text);
      ServerStrings->Text=text;
      this->ListBox->Items->Add(AnsiString("Start = ")+ServerStrings->Values["Start"]);
  this->ListBox->Items->Add(AnsiString("End"));
  if(ServerStrings!=NULL) delete ServerStrings;
  if(buf)   delete[] buf;
 }
  int Length=Socket->ReceiveLength();
char *buf=new char[Length+1];
      this->ListBox->Items->Add(AnsiString("Begin2. Lenght= ")+AnsiString(Length));
      TStringList* ServerStrings=new TStringList();
      Socket->ReceiveBuf(buf,Length);
      buf[Length]=0;
      AnsiString text=AnsiString(buf);
      this->ListBox->Items->Add(text);
      ServerStrings->Text=text;
      this->ListBox->Items->Add(AnsiString("Start = ")+ServerStrings->Values["Start"]);
  this->ListBox->Items->Add(AnsiString("End2"));
  if(ServerStrings!=NULL) delete ServerStrings;
  if(buf)   delete[] buf;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TcpServer1OnRecieveEvent(TObject* Sender, char* buf, int & DataLen)
{
  this->ListBox->Items->Add(AnsiString("Start3"));
       AnsiString text=AnsiString(buf);
  this->ListBox->Items->Add(text);
  this->ListBox->Items->Add(AnsiString("End3"));
}

